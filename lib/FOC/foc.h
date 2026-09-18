/*
 * Author: Binhvt22
**/

#ifndef HAND_MOTOR_CONTROL_FOC_H
#define HAND_MOTOR_CONTROL_FOC_H

#include <stdbool.h>
#include <stdint.h>
#include "pid.h"
#include "mt6826s.h"
#include "pwm_port.h"
#include "stm32h7xx_hal.h"
#include "config.h"
#include "app_state.h"


typedef struct {

	float omega_ref_rpm;										// Output of position controller

	float id;													// Output of speed controller

	float iq;													// Output of speed controller

    float vd;													// Output of current controller

    float vq;													// Output of current controller

    float v_alpha;												// Output of Inversed Park

    float v_beta;												// Output of Inversed Park

    float id_fb;												// feedback current d

    float iq_fb;												// feedback current q

    float ia_fb;												// feedback current phase a

    float ib_fb;												// feedback current phase b

    float ic_fb;												// feedback current phase c

    float omega_m;												// Tốc độ cơ sau hộp số (rad/s)

    float angle_m;												// Góc cơ encoder trong [0, 2*pi)

    float prev_angle_m;											// Góc encoder ở mẫu hợp lệ trước

    float position_zero_angle;									// Góc encoder khi bật nguồn, quy ước là 0 để tính vị trí

    float angle_delta_sum;										// Tổng delta góc để tính vận tốc

    float omega_filter_rpm;										// Vận tốc đã qua lọc

    float omega_e;												// Tốc độ điện

    float angle_e;												// Góc điện

    float multi_angle;											// Góc đa vòng ở đầu tải (rad)

    float multi_angle_deg;										// Góc đa vòng ở đầu tải (degree)

    float offset_encorder;										// Offset góc cơ (rad)

    float duty_a;												// duty cycle cấp cho pha a

    float duty_b;												// duty cycle cấp cho pha b

    float duty_c;												// duty cycle cấp cho pha c

    float v_bus;												// điện áp bus

    float voltage_limit_max;									// Vdc/căn(3)

    float current_limit_max;									// 2.5*CURRENT_RATED

    float speed_alpha_filter;									// Hệ số alpha của bộ lọc lpf dùng đề lọc tín hiệu encorder

    int32_t turn_count;											// Số vòng quay của encoder kể từ khi bật nguồn

    bool angle_initialized;										// Đã lấy góc ban đầu làm mốc 0

    uint32_t speed_cnt;											// Biến đếm chu kỳ vòng vận tốc

    uint32_t pos_cnt;											// Biến đếm chu kỳ vòng vị trí

    // Controller
     pi_t pi_id;												// Bộ điều khiển dòng id

     pi_t pi_iq;												// Bộ điều khiển dòng iq

     pi_t pi_speed;												// Bộ điều khiển vận tốc

     pi_t pi_pos;												// Bộ điều khiển vị trí

} foc_t;

void foc_init(foc_t *foc);

void foc_current_ctrl_run(foc_t *foc, float id_ref, float iq_ref);

void foc_speed_ctrl_run(foc_t *foc, float rpm_ref);

void foc_ctrl_pos_run(foc_t *foc, float pos_ref_rad);

void foc_update_angle(foc_t *foc, float angle_m);

void foc_open_loop_control(foc_t *foc, float vd, float vq, float theta_e);

void foc_voltage_limit_dq(foc_t *foc, float *v_d, float *v_q);

void foc_current_limit_dq(foc_t *foc, float *id, float *iq);

void foc_zero_pwm_output(foc_t *foc);

void foc_run_calib_encoder(foc_t *foc, encoder_t *encoder);

void foc_off(foc_t *foc);

void foc_voltage_bus_read(foc_t *foc);

#endif /* HAND_MOTOR_CONTROL_FOC_H */
