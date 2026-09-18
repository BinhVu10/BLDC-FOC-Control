#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H

#include <config.h>
#include "adc.h"
#include "adc_port.h"
#include "foc.h"
#include <stdbool.h>
#include <stdint.h>

// Define sensor current (ia, ib, ic)
typedef struct{

	// 3 phase current feedback
	float ia_fb;

	float ib_fb;

	float ic_fb;

	// offset ADC
	uint16_t offset_a;

	uint16_t offset_b;

}current_sensor_t;

void current_sensor_init(current_sensor_t *sensor);

bool current_sensor_get_offset(current_sensor_t *sensor);

void current_sensor_read(foc_t *foc, current_sensor_t *sensor);

#endif
