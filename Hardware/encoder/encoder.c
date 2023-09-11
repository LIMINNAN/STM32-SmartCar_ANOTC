#include "encoder.h"
#include "ws_drive_oled.h"
#include "niming.h"
#include "tim.h"
#include "bsp_graySensor.h"
#include "motor.h"
#include "math.h"

short EncoderLCount = 0;//±àÂëÆ÷¼ÆÊýÆ÷Öµ
short EncoderRCount = 0;

long g_lMotorPulseSigma =0;
long g_lMotor2PulseSigma=0;
void GET_NUM(void)
{
	EncoderRCount=-(short) __HAL_TIM_GET_COUNTER(&htim2);
	__HAL_TIM_SET_COUNTER(&htim2,0);
	
	EncoderLCount=(short) __HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3,0);
	
	g_lMotorPulseSigma  += EncoderLCount;
	g_lMotor2PulseSigma += EncoderRCount;

}