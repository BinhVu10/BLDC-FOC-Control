/*
 * Author: Binhvt22
**/

#include "foc.h"
#include "foc_math.h"
#include "config.h"
#include "pwm_port.h"
#include "tim.h"
#include <string.h>
#include <math.h>
#include <mt6826s.h>
#include "adc_port.h"


// Khởi tạo foc
void foc_init(foc_t *foc){

	if (foc == NULL) {
		return;
	}

	memset(foc, 0, sizeof(*foc));

	// Limit điện áp
	foc->voltage_limit_max = foc->v_bus * ONE_BY_SQRT3;

	foc->current_limit_max = CURRENT_LIMIT_FACTOR * CURRENT_RATED;

	foc->offset_encorder = ENCODER_OFFSET;									// Lấy ra từ file Config

	foc->speed_alpha_filter = SPEED_FILTER_ALPHA;


	// Khởi tạo tham số bộ điều kiển
	pi_init(&foc->pi_id, KP_Id, KI_Id, FOC_TS);

	pi_init(&foc->pi_iq, KP_Iq, KI_Iq, FOC_TS);

	pi_init(&foc->pi_speed, KP_SPEED, KI_SPEED, FOC_SPEED_TS);

	pi_init(&foc->pi_pos, KP_POS, KI_POS, FOC_POS_TS);

}

// Điều khiển dòng
void foc_current_ctrl_run(foc_t *foc, float id_ref, float iq_ref){

	uint32_t pwm_arr;

	if (foc == NULL) {
		return;
	}

	if (foc->v_bus <= 0.0f) {

		foc_off(foc);

		return;
	}

	// Clark - Park transform

	clarke_park_transform(foc->ia_fb, foc->ib_fb, foc->ic_fb, foc->angle_e, &foc->id_fb, &foc->iq_fb);


	// Run controller
	foc->vd = pi_update(&foc->pi_id, id_ref, foc->id_fb);

	foc->vq = pi_update(&foc->pi_iq, iq_ref, foc->iq_fb);

	// Chặn vd/căn(3)
	foc_voltage_limit_dq(foc, &foc->vd, &foc->vq);

	// Anti-windup back-calculation
	pi_anti_windup(&foc->pi_id, foc->vd, AW_GAIN_CURRENT);

	pi_anti_windup(&foc->pi_iq, foc->vq, AW_GAIN_CURRENT);

	// Inversed Park
	inverse_park_transform(foc->vd, foc->vq, foc->angle_e, &foc->v_alpha, &foc->v_beta);

	// Băm xung
	svpwm(foc->v_alpha, foc->v_beta, foc->v_bus, &foc->duty_a, &foc->duty_b, &foc->duty_c);

	// Cập nhật CCR
	pwm_arr = __HAL_TIM_GET_AUTORELOAD(&htim1);

	pwm_set((uint32_t)(foc->duty_a * (float)pwm_arr),

			(uint32_t)(foc->duty_b * (float)pwm_arr),

			(uint32_t)(foc->duty_c * (float)pwm_arr));
}

// Điều khiển tốc độ
void foc_speed_ctrl_run(foc_t *foc, float rpm_ref){

	float omega_rpm;

	if (foc == NULL) {
		return;
	}

	foc->speed_cnt++;

	if(foc->speed_cnt >= SPEED_DIV){

		// Tốc độ phản hồi sau hộp số
		foc->omega_m = foc->angle_delta_sum / (FOC_SPEED_TS * (float)GEAR_RATIO);

		omega_rpm = foc->omega_m * RADS_TO_RPM;

		foc->omega_filter_rpm = foc->speed_alpha_filter * omega_rpm + (1.0f - foc->speed_alpha_filter) * foc->omega_filter_rpm;

		foc->omega_e = foc->omega_m * (float)GEAR_RATIO * (float)POLE_PAIRS;

		foc->angle_delta_sum = 0.0f;

		// Điều khiển vận tốc
		foc->iq = pi_update(&foc->pi_speed, rpm_ref, foc->omega_filter_rpm);

		foc->id = 0.0f;

		foc_current_limit_dq(foc, &foc->id, &foc->iq);

		pi_anti_windup(&foc->pi_speed, foc->iq, AW_GAIN_SPEED);

		foc->speed_cnt = 0U;
	}

	// Điều khiển dòng
	foc_current_ctrl_run(foc, foc->id, foc->iq);

}

// Điều khiển vị trí
void foc_ctrl_pos_run(foc_t *foc, float pos_ref_rad){

	if (foc == NULL) {
		return;
	}

	foc->pos_cnt++;

	if (foc->pos_cnt >= POS_DIV) {

		foc->omega_ref_rpm = pi_update(&foc->pi_pos, pos_ref_rad, foc->multi_angle);

		// giới hạn vận tốc command
		foc->omega_ref_rpm = CONSTRAIN(foc->omega_ref_rpm,-SPEED_LIMIT, SPEED_LIMIT);

		foc->pos_cnt = 0U;
	}

	foc_speed_ctrl_run(foc, foc->omega_ref_rpm);
}

// Cập nhật góc encoder, góc điện và góc multi-angle đầu tải
void foc_update_angle(foc_t *foc, float angle_m){

	float delta_angle;

	if (foc == NULL) {
		return;
	}

	if (foc->angle_initialized == false) {

		foc->angle_m = angle_m;

		foc->prev_angle_m = angle_m;

		foc->position_zero_angle = angle_m;						// Lấy góc hiện tại làm mốc

		foc->multi_angle = 0.0f;

		foc->turn_count = 0;

		foc->angle_initialized = true;
	}
	else {

		delta_angle = angle_m - foc->prev_angle_m;

		if (delta_angle > PI) {

			delta_angle -= 2.0f * PI;

			foc->turn_count--;
		}
		else if (delta_angle < -PI) {

			delta_angle += 2.0f * PI;

			foc->turn_count++;
		}

		foc->angle_delta_sum += delta_angle;

		foc->angle_m = angle_m;

		foc->prev_angle_m = angle_m;

		foc->multi_angle = (angle_m + 2.0f * PI * (float)foc->turn_count - foc->position_zero_angle) / (float)GEAR_RATIO;
	}

	foc->multi_angle_deg = RAD_TO_DEG(foc->multi_angle);

	// Wrap up [0;2pi)

	foc->angle_e = fmodf((foc->angle_m - foc->offset_encorder) * (float)POLE_PAIRS, 2.0f * PI);

	if (foc->angle_e < 0.0f) {

		foc->angle_e += 2.0f * PI;
	}
}

// Điều khiển vòng hở áp
void foc_open_loop_control(foc_t *foc, float vd, float vq, float theta_e){
	uint32_t pwm_arr;

	if ((foc == NULL) || (foc->v_bus <= 0.0f)) {
		return;
	}

	foc_voltage_limit_dq(foc, &vd, &vq);

	inverse_park_transform(vd, vq, theta_e, &foc->v_alpha, &foc->v_beta);

	svpwm(foc->v_alpha, foc->v_beta, foc->v_bus, &foc->duty_a, &foc->duty_b, &foc->duty_c);

	pwm_arr = __HAL_TIM_GET_AUTORELOAD(&htim1);

	pwm_set((uint32_t)(foc->duty_a * (float)pwm_arr),

			(uint32_t)(foc->duty_b * (float)pwm_arr),

			(uint32_t)(foc->duty_c * (float)pwm_arr));
}

// Limit điện áp dq trong vòng tròn 1/căn 3
void foc_voltage_limit_dq(foc_t *foc, float *v_d, float *v_q){

	float voltage_square;

	float voltage_magnitude;

	float scale;

	if ((foc == NULL) || (v_d == NULL) || (v_q == NULL)) {
		return;
	}

	if (foc->voltage_limit_max <= 0.0f) {

		*v_d = 0.0f;

		*v_q = 0.0f;

		return;
	}

	voltage_square = (*v_d * *v_d) + (*v_q * *v_q);

	if (voltage_square > foc->voltage_limit_max * foc->voltage_limit_max) {

        voltage_magnitude = sqrtf(voltage_square);

		scale = foc->voltage_limit_max / voltage_magnitude;

		*v_d *= scale;

		*v_q *= scale;
	}
}

// Limit dòng điện trong giới hạn (đầu ra của bộ vận tốc)
void foc_current_limit_dq(foc_t *foc, float *id, float *iq){

	float current_square;

	float current_magnitude;

	float scale;

	if ((foc == NULL) || (id == NULL) || (iq == NULL)) {
		return;
	}

	if (foc->current_limit_max <= 0.0f) {

		*id = 0.0f;

		*iq = 0.0f;

		return;
	}

	current_square = (*id * *id) + (*iq * *iq);

	if (current_square > foc->current_limit_max * foc->current_limit_max) {

		current_magnitude = sqrtf(current_square);

		scale = foc->current_limit_max / current_magnitude;

		*id *= scale;

		*iq *= scale;
	}
}

// Ta sẽ không xuất xung pwm --> Không cấp áp
void foc_zero_pwm_output(foc_t *foc)
{
    if (foc == NULL) {
        return;
    }

    foc->duty_a = 0.0f;

    foc->duty_b = 0.0f;

    foc->duty_c = 0.0f;
}

// Hàm calib encoder
void foc_run_calib_encoder(foc_t *foc, encoder_t *encoder){

	float angle_m;

	if ((foc == NULL) || (encoder == NULL) || (foc->v_bus <= 0.0f)) {
		return;
	}

	// Cho vd > 0 , vq = 0 và theta_e = 0 để align rotor về trùng với trục d

	foc_open_loop_control(foc, VD_CALIB, 0.0f, 0.0f);

	HAL_Delay(500U);										// Delay để trục giữ ổn định

	// Gửi lệnh đọc encoder để lấy offset
	encoder_start_TransmitReceive(encoder);

	if(encoder->data_valid == true){

		angle_m = encoder_read_angle(encoder);

		if ((encoder->crc_fault == false) && (encoder->status == 0U)) {

			foc->offset_encorder = angle_m;

			foc->angle_initialized = false;

			foc_update_angle(foc, angle_m);
		}
	}

	foc_zero_pwm_output(foc);

	pwm_set(0U, 0U, 0U);

}

// Hàm tắt foc
void foc_off(foc_t *foc){

	if (foc == NULL) {

		return;
	}

	foc_zero_pwm_output(foc);

	pwm_set(0U, 0U, 0U);

	pi_reset(&foc->pi_id);

	pi_reset(&foc->pi_iq);

	pi_reset(&foc->pi_speed);

	pi_reset(&foc->pi_pos);

	foc->id = 0.0f;

	foc->iq = 0.0f;

	foc->vd = 0.0f;

	foc->vq = 0.0f;

	foc->omega_m = 0.0f;

	foc->omega_filter_rpm = 0.0f;

	foc->prev_angle_m = foc->angle_m;

	foc->angle_delta_sum = 0.0f;

	foc->omega_ref_rpm = 0.0f;

	foc->speed_cnt = 0U;

	foc->pos_cnt = 0U;
}

void foc_voltage_bus_read(foc_t *foc){
	uint16_t voltage_bus_raw;

	if (foc == NULL) {
		return;
	}

	voltage_bus_raw = read_voltage_bus_raw();

	foc->v_bus = (float)voltage_bus_raw * ADC_VREF * VBUS_DIV_GAIN / ADC_MAX;

	foc->voltage_limit_max = foc->v_bus * ONE_BY_SQRT3;
}
