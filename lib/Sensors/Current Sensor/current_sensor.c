/*
 * Author: Binhvt22
 *
 * */

#include "current_sensor.h"

#include "config.h"


void current_sensor_init(current_sensor_t *sensor){

	sensor->ia_fb = 0.0f;

	sensor->ib_fb = 0.0f;

	sensor->ic_fb = 0.0f;

	sensor->offset_a = 0 ;

	sensor->offset_b = 0;

}

// Get offset adc when three PWM duty cycles are equal
bool current_sensor_get_offset(current_sensor_t *sensor){

	static uint32_t sum_a = 0;

	static uint32_t sum_b = 0;

	static uint32_t sample_count = 0;

	sum_a += read_phase_a_raw();

	sum_b += read_phase_b_raw();

	sample_count++;

	if (sample_count >= OFFSET_SAMPLE_NUM) {

		// Update offset

		sensor->offset_a = (uint16_t)(sum_a / OFFSET_SAMPLE_NUM);

		sensor->offset_b = (uint16_t)(sum_b / OFFSET_SAMPLE_NUM);

		return true;
	}

	return false;

}


void current_sensor_read(foc_t* foc, current_sensor_t *sensor){

	foc->ia_fb = sensor->ia_fb = ((float)read_phase_a_raw() - sensor->offset_a) * ADC_TO_CURRENT;

	foc->ib_fb = sensor->ib_fb = ((float)read_phase_b_raw() - sensor->offset_b) * ADC_TO_CURRENT;

	// 3 phase balance

	foc->ic_fb = sensor->ic_fb = -(sensor->ia_fb + sensor->ib_fb);
}
