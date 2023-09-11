#ifndef GY_25_MPU6050
#define GY_25_MPU6050

#include "main.h"

//BYTE2��ʽ����λ
/*���ڲ��������������ռ��ᣨx��y �� z���ϵļ��ٶȡ��������ṩ������������Լ��ٶ���Ϣ��*/
#define ACC        0X01//���ٶȼƿ���λ

/*���ڲ�������Χ�������ռ��ᣨx��y �� z����ת�Ľ��ٶȡ��������ṩ��������ĽǶȱ仯����Ϣ��*/
#define GYR        0X02//���ݿ���λ

/*���ڲ�������Ĵų���Ϣ�������ų���ǿ�Ⱥͷ���*/
#define MAG        0X04  // �ų�����λ

/*���ڲ����������Ԫ����Ϣ����Ԫ����һ�����ڱ�ʾ��ת����ѧ���ߡ�*/
#define Q4         0X08  // ��Ԫ������λ

/*ŷ������һ��������������ά�ռ�����̬�ķ�����ͨ����ʾΪ�����ǣ�Roll���������ǣ�Pitch����ƫ���ǣ�Yaw�������������������� x ����ת������������������ y ����ת��ƫ�������������� z ����ת��*/
#define RPY        0X10//ŷ���ǿ���λ

/*���ڲ�������ѹǿ��Ϣ���ṩ������������ѹֵ��*/
#define PRESSURE   0x20  // ��ѹ����λ

/*���ڲ���������¶ȡ�*/
#define TEMP       0x40  // �¶ȿ���λ

/*���ڲ�������ĺ��θ߶ȡ�*/
#define ALTITUDE   0x80  // ���ο���λ

//���������ʽ START_BYTE,����/���/У׼/������/����Ƶ�ʣ�BYTE2��ʽ����λ��ǰ���⼸�����ۼӺ�

// ָ���ֽ���ʼλ
#define START_BYTE 0xA5 
//������������
#define ACC_CMD				     (uint8_t[]){START_BYTE, 0x55, 0x01 , 0xFB}
#define GYRO_CMD					 (uint8_t[]){START_BYTE, 0x55, 0x02 , 0xFC}
#define MAGNETIC_FIELD_CMD (uint8_t[]){START_BYTE, 0x55, 0x04 , 0xFE}
#define QUATERNION_CMD     (uint8_t[]){START_BYTE, 0x55, 0x08 , 0x02}
#define RPY_CMD            (uint8_t[]){START_BYTE, 0x55, 0x10 , 0x0A}
#define PRESSURE_CMD       (uint8_t[]){START_BYTE, 0x55, 0x20 , 0x1A}
#define TEMP_CMD           (uint8_t[]){START_BYTE, 0x55, 0x40 , 0x3A}
#define ALTITUDE_CMD       (uint8_t[]){START_BYTE, 0x55, 0x80 , 0x7A}

//���Ͽ�������
#define ACC_GYRO_CMD       (uint8_t[]){START_BYTE, 0x55, 0x03 , 0xFD}
#define ACC_RPY_CMD        (uint8_t[]){START_BYTE, 0x55, 0x11 , 0x0B}
#define GYRO_RPY_CMD       (uint8_t[]){START_BYTE, 0x55, 0x12 , 0x0C}
#define ACC_GYRO_RPY_CMD   (uint8_t[]){START_BYTE, 0x55, 0x13 , 0x0D}
#define ACC_TEMP_CMD       (uint8_t[]){START_BYTE, 0x55, 0x41 , 0x3B}
#define GYRO_TEMP_CMD      (uint8_t[]){START_BYTE, 0x55, 0x42 , 0x3C}
#define ACC_GYRO_TEMP_CMD  (uint8_t[]){START_BYTE, 0x55, 0x43 , 0x3D}

// �Զ��������ָ��     
#define AUTO_OUTPUT_DATA_CMD 					(uint8_t[]){START_BYTE, 0x56, 0x02, 0xFD}
// ��ѯ�������ָ��  
#define QUERY_OUTPUT_DATA_CMD 				(uint8_t[]){START_BYTE, 0x56, 0x01, 0xFC}

/*����У׼������ˮƽ����ģ�飨ˮƽģʽʱ�򣩣�
���ͼ���У׼ָ���ģ�鼴�ɡ�
У׼������ݽ��Զ����浽 flash,�������Զ�����У׼������
*/
// ����У׼ָ��   
#define ACCELEROMETER_CALIBRATION_CMD 	(uint8_t[]){START_BYTE, 0x57, 0x01, 0xFD}
// ������У׼ָ��     
#define MAGNETOMETER_CALIBRATION_CMD 	(uint8_t[]){START_BYTE, 0x57, 0x02, 0xFE}
// ����������ָ��             
#define SET_BAUDRATE_115200_CMD 			(uint8_t[]){START_BYTE, 0x58, 0x01, 0xFE}
#define SET_BAUDRATE_9600_CMD 				(uint8_t[]){START_BYTE, 0x58, 0x02, 0xFF}
// ����Ƶ������ָ��   
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

