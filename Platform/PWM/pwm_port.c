/*
 * timer.c
 *
 *  Created on: Sep 10, 2026
 *      Author: binhvt22
 */
#include "pwm_port.h"
#include "tim.h"

void pwm_start(void)
{
    (void)HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    (void)HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    (void)HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    (void)HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_1);
    (void)HAL_TIM_Base_Start(&htim2);
}

void pwm_set(uint32_t duty_a, uint32_t duty_b, uint32_t duty_c)
{
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, duty_a);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, duty_b);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, duty_c);
}

