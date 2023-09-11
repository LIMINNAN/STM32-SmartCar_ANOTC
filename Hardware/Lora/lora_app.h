#ifndef _LORA_APP_H_
#define _LORA_APP_H_

/************************************************
 ALIENTEK ������STM32F429������
 ATK-LORA-01ģ�鹦������
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

#include "lora_cfg.h"
#include "main.h"

#define USART3_MAX_RECV_LEN		512				
#define USART3_MAX_SEND_LEN		400					

#define LORA_AUX  	 HAL_GPIO_ReadPin(lora_AUX_GPIO_Port,lora_AUX_Pin)
#define LORA_MD0(x)  HAL_GPIO_WritePin(lora_MOD_GPIO_Port,lora_MOD_Pin,(x))

extern _LoRa_CFG LoRa_CFG;
extern uint8_t Lora_mode;

void lora_at_response(uint8_t mode);	
uint8_t* lora_check_cmd(uint8_t *str);
uint8_t lora_send_cmd(uint8_t *cmd,uint8_t *ack,uint16_t waittime);
uint8_t LoRa_Init(void);
void Aux_Int(uint8_t mode);
void LoRa_Set(void);
void LoRa_SendData(void);
void LoRa_ReceData(void);
void u3_printf(char *p, ...);
int WS_LoRa_Task_ReceData(void *date);
#endif

