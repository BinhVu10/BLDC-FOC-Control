/*
 * Author: Binhvt22
**/

#ifndef FOC_MATH_H
#define FOC_MATH_H

#include "stm32h7xx.h"
#include <math.h>


void clarke_transform(float ia, float ib, float ic, float *i_alpha, float *i_beta);

void park_transform(float i_alpha, float i_beta, float theta_e, float *i_d, float *i_q);

void clarke_park_transform(float ia, float ib, float ic, float theta_e, float *i_d, float *i_q);

void inverse_park_transform(float v_d, float v_q, float theta_e, float *v_alpha, float *v_beta);

void svpwm(float v_alpha, float v_beta, float v_bus, float *duty_a, float *duty_b, float *duty_c);

#endif /* FOC_MATH_H */
