// This file contain the congiguation of the project

#ifndef CONFIG_H
#define CONFIG_H

/*
 * MOTOR CONFIGUARE
 * */
#include <stdint.h>

#define POLE_PAIRS 				        1
#define GEAR_RATIO 				        29

// #define ENCODER_OFFSET                  1.42564344f                 						 // Encoder offset của động cơ Nthảo
#define ENCODER_OFFSET                  3.25721645f                    						 // Encoder offset của động cơ 5.2
#define MOTOR_R 						100
#define MOTOR_L 						100
#define L_D 							100
#define L_Q 							100
#define MOTOR_FLUX 						100
#define CURRENT_RATED					1.0f						                         // Dòng điện đinh mức của động cơ
#define CURRENT_LIMIT_FACTOR			2.5f							                     // Hệ số giới hạn dòng
#define SPEED_LIMIT						18000.0f										        // Tốc độ tối đa (để clamp đầu ra của bộ vị trí)

#define VD_CALIB						5.0f										        // Điện áp Vd để calib

/*
 * CONTROL CONFIGUARE
 * */
#define FOC_FREQ_HZ						20000												 // Tần số vòng dòng điện
#define FOC_TS						    (1.0f/(float) FOC_FREQ_HZ)							 // Chu kỳ foc
#define FOC_SPEED_LOOP_HZ				1000												 // Tần số điều khiển tốc độ
#define FOC_SPEED_TS					(1.0f / (float)FOC_SPEED_LOOP_HZ)					 // Chu kỳ lấy mẫu vòng vận tốc
#define SPEED_DIV						((uint32_t)(FOC_FREQ_HZ / FOC_SPEED_LOOP_HZ))		 // Biến đếm thực hiện so sánh để điều khiển vận tốc
#define FOC_POS_LOOP_HZ                 1                                                    // Tần số vòng vị trí
#define FOC_POS_TS                      1/(float)(FOC_POS_LOOP_HZ)                           // Chu kỳ lấy mẫu vòng vị trí
#define POS_DIV                         ((uint32_t)(FOC_FREQ_HZ/FOC_POS_LOOP_HZ))            // Biến đếm thức hiện so sánh để điều khiển vị trí


// CONTROLLER PI CURRENT D-Q
#define KP_Iq							0.0f
#define KI_Iq							2000.0f
#define KP_Id							0.0f
#define KI_Id							2000.0f
#define AW_GAIN_CURRENT					0.1f												 // Hệ số anti-windup Back-calculation

// CONTROLLER PI SPEED
//#define KP_SPEED						1.0f
//#define KI_SPEED						150.0f
#define KP_SPEED						0.1f
#define KI_SPEED						0.5f
#define SPEED_FILTER_ALPHA				0.95f
#define AW_GAIN_SPEED					0.15f

// CONTROLLER PI POSITION
#define KP_POS			        		10.0f
#define KI_POS					    	0.0f
#define AW_GAIN_POS				    	0.1f

/*
 * ADC CONFIGUARE
 * */
#define ADC_VREF 						3.3f
#define ADC_MAX						    65535.0f									  	 	 // 16 bit
#define GAIN 							0.25f											 	 // V/A
#define ADC_TO_CURRENT                  (ADC_VREF / (ADC_MAX * GAIN))
#define OFFSET_SAMPLE_NUM		        4096										  	 	 // 4096 Mẫu adc

#define VBUS_DIV_GAIN			        5.7f								              	 // Gain điện áp từ mạch chia áp 47k/10k


// HELPER PARAMETER
#define PI				               3.14159265f
#define TWO_BY_SQRT3 	               1.15470053838f
#define TWO_BY_THREE                   0.6666666666666667f
#define SQRT3                          1.7320508075688773f
#define SQRT3_BY_TWO                   0.8660254037844386f
#define ONE_BY_SQRT3 	               0.57735026919f

#define CONSTRAIN(val, min, max) \
    ((val) <= (min) ? (min) : ((val) >= (max) ? (max) : (val)))

#define RAD_TO_DEG(rad)                 ((rad) * 57.29577951308232f)  // 180/π
#define DEG_TO_RAD(deg)                 ((deg) * 0.017453292519943f)  // π/180
#define RADS_TO_RPM			            9.54929659f

#endif
