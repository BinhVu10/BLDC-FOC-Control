/*
 * Author: Binhvt22
 *
 * */

#include <adc_port.h>


/* Ngắt ADC sẽ là điểm bắt đầu của chu kỳ điều khiển, được kích bởi TRGO của timer
*
*
*	Trong ngắt sẽ thực hiện đọc cảm biến: encorder (theta_e), dòng, chạy bộ điều khiển và tính SVPWM
*
*/

// Start ADC Injected conversion
void adc_start(void){

	HAL_ADCEx_InjectedStart(&hadc2);				// Đọc dòng điện pha ib

	HAL_ADCEx_InjectedStart(&hadc3);				// Đọc điện áp

	HAL_ADCEx_InjectedStart_IT(&hadc1);				// Sử dụng adc1 làm master điều khiển luồng adc, đọc dòng điện pha ia
}

// READ CURRENT SENSOR
uint16_t read_phase_a_raw(void){

	return (uint16_t)ADC1->JDR1;
}

uint16_t read_phase_b_raw(void){

	return (uint16_t)ADC2->JDR1;
}


uint16_t read_voltage_bus_raw(void){

	return (uint16_t)ADC3->JDR1;

}
