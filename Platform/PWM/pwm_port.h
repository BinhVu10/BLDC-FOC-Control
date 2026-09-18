/*
 * timer.h
 *
 *  Created on: Sep 10, 2026
 *      Author: binhvt22
 */

#ifndef PWM_PORT_H
#define PWM_PORT_H

#include <stdint.h>

void pwm_start(void);

void pwm_set(uint32_t duty_a, uint32_t duty_b, uint32_t duty_c);

#endif
