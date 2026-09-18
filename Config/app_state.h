/*
 * state.h
 *
 *  Created on: Sep 12, 2026
 *      Author: binhvt22
 */

#ifndef APP_STATE_H_
#define APP_STATE_H_

typedef struct{

    float id_ref;							// Current d reference

    float iq_ref;							// Current q reference

    float rpm_ref;							// Speed reference at output shaft (rpm)

    float pos_ref_deg;						// Position reference at output shaft (degree)

}user_command_t;

typedef enum{

	MOTOR_IDLE,								// Chế độ chờ

	MOTOR_INIT,								// Chế độ khởi động, init các tham số (init + calib)

	MOTOR_RUNNING,							// Chế độ chạy, sẽ thực hiện các mode

	MOTOR_FAULT								// Chế độ lỗi

}state_t;


// Quản lý các chế độ điều khiển (Các hàm sẽ được quản lý trong foc.c)

typedef enum{

	CTRL_NONE,									// Chế độ không điều khiển gì (ban đầu mặc định sẽ là như thế)

	CALIB_ENCODER_MODE,

	CURRENT_CTRL_MODE,

	SPEED_CTRL_MODE,

	POS_CTRL_MODE

}mode_t;







#endif /* APP_STATE_H_ */
