/*
 * Author: Binhvt22
 *
 * */
#ifndef ADC_PORT_H
#define ADC_PORT_H

// This file contain ADC API used for current sensor



#include <stdint.h>
#include <adc.h>
#include "config.h"

// Start ADC Injected conversion
void adc_start(void);


// READ ADC CURRENT RAW
uint16_t read_phase_a_raw(void);
uint16_t read_phase_b_raw(void);

// READ ADC VOLTAGE

uint16_t read_voltage_bus_raw(void);

#endif
