/*
 * Author: Binhvt22
**/

#ifndef PID_H
#define PID_H

typedef struct {

    float kp;

    float ki;

    float integral;

    float ts;

    float output;

    float error;

} pi_t;

void pi_init(pi_t *pi, float kp, float ki, float ts);

float pi_update(pi_t *pi, float setpoint, float feedback);

void pi_anti_windup(pi_t *pi, float output_limited, float aw_gain);

void pi_reset(pi_t *pi);


#endif /* PID_H */
