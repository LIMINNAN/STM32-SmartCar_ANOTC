#ifndef ENCODER_ENCODER_H_
#define ENCODER_ENCODER_H_

#include "stm32f1xx_hal.h" //HAL���ļ�����
#include <main.h>

extern long g_lMotorPulseSigma;//���25ms���ۼ������ܺ�
extern long g_lMotor2PulseSigma;
extern short EncoderLCount,EncoderRCount;//ȫ�ֱ����� ������������ֵ

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;

void GET_NUM(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

typedef struct _Motor
{
    int32_t lastCount;   //��һ�μ���ֵ
    int32_t totalCount;  //�ܼ���ֵ
    int16_t overflowNum; //�������
    float speed;         //���ת��
    uint8_t direct;      //��ת����
}Motor;

#endif /* ENCODER_ENCODER_H_ */
