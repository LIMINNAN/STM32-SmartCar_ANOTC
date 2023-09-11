#ifndef GY_25_MPU6050
#define GY_25_MPU6050

#include "main.h"

//BYTE2格式控制位
/*用于测量物体在三个空间轴（x、y 和 z）上的加速度。它可以提供关于物体的线性加速度信息。*/
#define ACC        0X01//加速度计控制位

/*用于测量物体围绕三个空间轴（x、y 和 z）旋转的角速度。它可以提供关于物体的角度变化率信息。*/
#define GYR        0X02//陀螺控制位

/*用于测量物体的磁场信息，包括磁场的强度和方向。*/
#define MAG        0X04  // 磁场控制位

/*用于测量物体的四元数信息，四元数是一种用于表示旋转的数学工具。*/
#define Q4         0X08  // 四元数控制位

/*欧拉角是一种描述物体在三维空间中姿态的方法，通常表示为滚动角（Roll）、俯仰角（Pitch）和偏航角（Yaw）。滚动角描述物体绕 x 轴旋转，俯仰角描述物体绕 y 轴旋转，偏航角描述物体绕 z 轴旋转。*/
#define RPY        0X10//欧拉角控制位

/*用于测量大气压强信息，提供物体所处的气压值。*/
#define PRESSURE   0x20  // 气压控制位

/*用于测量物体的温度。*/
#define TEMP       0x40  // 温度控制位

/*用于测量物体的海拔高度。*/
#define ALTITUDE   0x80  // 海拔控制位

//控制命令格式 START_BYTE,数据/输出/校准/波特率/更新频率，BYTE2格式控制位，前面这几个的累加和

// 指令字节起始位
#define START_BYTE 0xA5 
//单独控制命令
#define ACC_CMD				     (uint8_t[]){START_BYTE, 0x55, 0x01 , 0xFB}
#define GYRO_CMD					 (uint8_t[]){START_BYTE, 0x55, 0x02 , 0xFC}
#define MAGNETIC_FIELD_CMD (uint8_t[]){START_BYTE, 0x55, 0x04 , 0xFE}
#define QUATERNION_CMD     (uint8_t[]){START_BYTE, 0x55, 0x08 , 0x02}
#define RPY_CMD            (uint8_t[]){START_BYTE, 0x55, 0x10 , 0x0A}
#define PRESSURE_CMD       (uint8_t[]){START_BYTE, 0x55, 0x20 , 0x1A}
#define TEMP_CMD           (uint8_t[]){START_BYTE, 0x55, 0x40 , 0x3A}
#define ALTITUDE_CMD       (uint8_t[]){START_BYTE, 0x55, 0x80 , 0x7A}

//联合控制命令
#define ACC_GYRO_CMD       (uint8_t[]){START_BYTE, 0x55, 0x03 , 0xFD}
#define ACC_RPY_CMD        (uint8_t[]){START_BYTE, 0x55, 0x11 , 0x0B}
#define GYRO_RPY_CMD       (uint8_t[]){START_BYTE, 0x55, 0x12 , 0x0C}
#define ACC_GYRO_RPY_CMD   (uint8_t[]){START_BYTE, 0x55, 0x13 , 0x0D}
#define ACC_TEMP_CMD       (uint8_t[]){START_BYTE, 0x55, 0x41 , 0x3B}
#define GYRO_TEMP_CMD      (uint8_t[]){START_BYTE, 0x55, 0x42 , 0x3C}
#define ACC_GYRO_TEMP_CMD  (uint8_t[]){START_BYTE, 0x55, 0x43 , 0x3D}

// 自动输出数据指令     
#define AUTO_OUTPUT_DATA_CMD 					(uint8_t[]){START_BYTE, 0x56, 0x02, 0xFD}
// 查询输出数据指令  
#define QUERY_OUTPUT_DATA_CMD 				(uint8_t[]){START_BYTE, 0x56, 0x01, 0xFC}

/*加陀校准方法：水平放置模块（水平模式时候），
发送加陀校准指令给模块即可。
校准后的数据将自动保存到 flash,重启后自动加载校准的数据
*/
// 加陀校准指令   
#define ACCELEROMETER_CALIBRATION_CMD 	(uint8_t[]){START_BYTE, 0x57, 0x01, 0xFD}
// 磁力计校准指令     
#define MAGNETOMETER_CALIBRATION_CMD 	(uint8_t[]){START_BYTE, 0x57, 0x02, 0xFE}
// 波特率设置指令             
#define SET_BAUDRATE_115200_CMD 			(uint8_t[]){START_BYTE, 0x58, 0x01, 0xFE}
#define SET_BAUDRATE_9600_CMD 				(uint8_t[]){START_BYTE, 0x58, 0x02, 0xFF}
// 更新频率设置指令   
#define SET_UPDATE_RATE_10HZ_CMD 			(uint8_t[]){START_BYTE, 0x59, 0x01, 0xFF}
#define SET_UPDATE_RATE_50HZ_CMD			(uint8_t[]){START_BYTE, 0x59, 0x02, 0x00}
#define SET_UPDATE_RATE_100HZ_CMD 		(uint8_t[]){START_BYTE, 0x59, 0x03, 0x01}
#define SET_UPDATE_RATE_200HZ_CMD 		(uint8_t[]){START_BYTE, 0x59, 0x04, 0x02}

void USART_send_byte(uint8_t byte);
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length);
void send_out(int16_t *data,uint8_t length,uint8_t send);
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send);
uint8_t CHeck(uint8_t *data_buf);
extern uint8_t stata;
extern uint8_t RX_BUF[50];
void send_Instruction(void);



void Recive_data(void);
void GY_25_Init(void);
	
#endif

