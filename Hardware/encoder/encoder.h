#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_

#include "stm32f1xx_hal.h" //HAL库文件声明
#include <main.h>

extern long g_lMotorPulseSigma;//电机25ms内累计脉冲总和
extern long g_lMotor2PulseSigma;
extern short EncoderLCount,EncoderRCount;//全局变量， 保存电机脉冲数值

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;

void GET_NUM(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

typedef struct _Motor
{
    int32_t lastCount;   //上一次计数值
    int32_t totalCount;  //总计数值
    int16_t overflowNum; //溢出次数
    float speed;         //电机转速
    uint8_t direct;      //旋转方向
}Motor;

#endif /* ENCODER_ENCODER_H_ */
