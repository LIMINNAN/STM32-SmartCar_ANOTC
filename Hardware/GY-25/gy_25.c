#include "main.h"
#include "string.h"
#include "gy_25.h"

uint8_t RX_BUF[50]={0},stata=0;
uint8_t data_buf[50]={0},data_rec=0;
int16_t rpy[3]={0},Acc[3]={0},Gyr[3]={0},Mag[3]={0},Temp=0;

/* 串口2被覆盖了 - . -*/
extern UART_HandleTypeDef huart2;

void USART_send_byte(uint8_t byte)
{
	while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET);
	huart2.Instance->DR = byte;
}

//发送多字节数据
void USART_Send_bytes(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		USART_send_byte(Buffer[i++]);
	}
}
//发送多字节数据+校验和
void USART_Send(uint8_t *Buffer, uint8_t Length)
{
	uint8_t i=0;
	while(i<Length)
	{
		if(i<(Length-1))
		Buffer[Length-1]+=Buffer[i];//累加Length-1前的数据
		USART_send_byte(Buffer[i++]);
	}
}
//发送一帧数据
void send_out(int16_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[30],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=2*length;//数据长度
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k]>>8;
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,i);	
}
void send_8bit_out(uint8_t *data,uint8_t length,uint8_t send)
{
	uint8_t TX_DATA[50],i=0,k=0;
	memset(TX_DATA,0,(2*length+5));//清零缓存TX_DATA
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=0X5A;//帧头
	TX_DATA[i++]=send;//功能字节
	TX_DATA[i++]=length;//数据长度
	for(k=0;k<length;k++)//存入数据到缓存TX_DATA
	{
		TX_DATA[i++]=(uint16_t)data[k];
	}
	USART_Send(TX_DATA,i);	
}
//校验和检查
uint8_t CHeck(uint8_t *data)
{
	uint8_t sum=0,number=0,i=0;
	number=RX_BUF[3]+5;
	if(number>42)//超过上传数据
		return 0;
	for(i=0;i<number-1;i++)
	 sum+=RX_BUF[i];
	if(sum==RX_BUF[number-1])
	{
		memcpy(data,RX_BUF,number);
		return 1;
	}
	else
    return 0;
}
void send_Instruction(void)
{
	//USART_Send_bytes(ACCELEROMETER_CALIBRATION_CMD,4);
	//HAL_Delay(100);
	
	USART_Send_bytes(RPY_CMD,4);
	HAL_Delay(100);
	
	USART_Send_bytes(SET_UPDATE_RATE_50HZ_CMD,4);
	HAL_Delay(100);
	
	USART_Send_bytes(AUTO_OUTPUT_DATA_CMD	,4);//发送自动输出指令
	HAL_Delay(100);
}
void GY_25_Init(void){
	send_Instruction();
}
void Recive_data(void){
	if(stata==1)
	{
		 stata=0;
		if(CHeck(data_buf))
		{
			 data_rec=0;
		   if(data_buf[2]&0x01) //ACC
			 {
			    Acc[0]=(data_buf[4]<<8)|data_buf[5];
				  Acc[1]=(data_buf[6]<<8)|data_buf[7];
				  Acc[2]=(data_buf[8]<<8)|data_buf[9];
          data_rec=6;
				  //WS_Debug("ACC_X: %.2f,ACC_Y:%.2f ,ACC_Z:%.2f \r\n",(float) Acc[0],(float) Acc[1],(float) Acc[2]);
			 }
			  if(data_buf[2]&0x02) //GYRO
			 {
			    Gyr[0]=(data_buf[4+data_rec]<<8)|data_buf[5+data_rec];
				  Gyr[1]=(data_buf[6+data_rec]<<8)|data_buf[7+data_rec];
				  Gyr[2]=(data_buf[8+data_rec]<<8)|data_buf[9+data_rec];
          data_rec+=6;
				  //WS_Debug("GYRO_X: %.2f,GYRO_Y:%.2f ,GYRO_Z:%.2f \r\n",(float) Gyr[0],(float) Gyr[1],(float) Gyr[2]);
			 }
			  if(data_buf[2]&0x04) //MAG
			 {
			    Mag[0]=(data_buf[4+data_rec]<<8)|data_buf[5+data_rec];
				  Mag[1]=(data_buf[6+data_rec]<<8)|data_buf[7+data_rec];
				  Mag[2]=(data_buf[8+data_rec]<<8)|data_buf[9+data_rec];
          data_rec+=6;
				 //	WS_Debug("Mag_X: %.2f,Mag_Y:%.2f ,Mag_Z:%.2f \r\n",(float) Mag[0],(float) Mag[1],(float) Mag[2]);
			 }
			 
			  if(data_buf[2]&0x10) //欧拉角
			 {
			    rpy[0]=(data_buf[4+data_rec]<<8)|data_buf[5+data_rec];
				  rpy[1]=(data_buf[6+data_rec]<<8)|data_buf[7+data_rec];
				  rpy[2]=(data_buf[8+data_rec]<<8)|data_buf[9+data_rec];
          data_rec+=6;
				  //WS_Debug("Roll: %.2f \r\n Pitch:%.2f \r\n Yaw :%.2f \r\n",(float) rpy[0]/100,(float) rpy[1]/100,(float) rpy[2]/100);
			 }

			   if(data_buf[2]&0x40) //温度
			 {
			    Temp=(data_buf[4+data_rec]<<8)|data_buf[5+data_rec]; 
          data_rec+=2;
				 	//WS_Debug(" ,Temp: %.2f \r\n",(float) Temp/100);
			 }		
		 }
	}
 }

 
 /*
 --V1.0	2023/7/2
    这是一个根据GY-25串口输出六轴姿态传感器输出的欧拉角的演示程序：

		#define RECT_X 64
		#define RECT_Y 2
		#define RECT_WIDTH 60
		#define RECT_HEIGHT 60
		#define DOT_RADIUS 4
		float map(float value, float in_min, float in_max, float out_min, float out_max) {
		return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
		}

		while(1){
		SetTempBuffer();
		DrawFastHLine(64,32,64);
		DrawFastVLine(96,0,64);
		DrawRect2(64, 0, 64, 64);
		
		WS_OLED_Printf(0,1,1,"GY-25");
		WS_OLED_Printf(0,3,0,"R:%.2f ",(float) rpy[0]/100);
		WS_OLED_Printf(0,4,0,"P:%.2f ",(float) rpy[1]/100);
		WS_OLED_Printf(0,5,0,"Y:%.2f ",(float) rpy[2]/100);
		
		// 将 dotX 映射到 RECT_X 至 RECT_X+RECT_WIDTH 范围内，并进行边界检查
		int dotX = (int)(map((float)rpy[0]/100, -90.0, 90.0, RECT_X, RECT_X + RECT_WIDTH));
		dotX = (dotX < RECT_X) ? RECT_X : dotX;
		dotX = (dotX > RECT_X + RECT_WIDTH) ? RECT_X + RECT_WIDTH : dotX;

		// 将 dotY 映射到 RECT_Y 至 RECT_Y+RECT_HEIGHT 范围内，并进行边界检查
		int dotY = (int)(map((float)rpy[1]/100, 90.0, -90.0, RECT_Y, RECT_Y + RECT_HEIGHT));
		dotY = (dotY < RECT_Y) ? RECT_Y : dotY;
		dotY = (dotY > RECT_Y + RECT_HEIGHT) ? RECT_Y + RECT_HEIGHT : dotY;

		
		// 绘制移动的点
		DrawFillCircle(dotX, dotY, DOT_RADIUS);
		
		// 更新屏幕显示
		UpdateScreen();
    HAL_Delay(20);
	}
 */