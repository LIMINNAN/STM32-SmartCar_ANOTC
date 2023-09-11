#include "main.h"
#include "realize.h"
#include "hardwareInterface.h"
#include "usart.h"


void NoUse(uint8_t data){}
//���ڽ��շ��Ϳ��ٶ��壬ֱ���޸Ĵ˴��ĺ������ƺ꣬�޸ĳ��Լ��Ĵ��ڽ����ͷ��ͺ������Ƽ��ɣ�ע�⺯��������ʽ��ͳһ
#define U1GetOneByte	AnoPTv8HwRecvByte
#define U2GetOneByte	NoUse
#define U3GetOneByte	NoUse
#define U4GetOneByte	NoUse
#define U5GetOneByte	NoUse	
uint8_t Tx1Buffer[256];
uint8_t Tx1Counter = 0;
uint8_t count1 = 0;
void DrvUart1SendBuf(unsigned char *DataToSend, uint8_t data_num)
{
    uint8_t i;
    for (i = 0; i < data_num; i++)
    {
        Tx1Buffer[count1++] = *(DataToSend + i);
    }

    HAL_UART_Transmit_DMA(&huart1, Tx1Buffer, count1);
}

extern uint8_t U1RxBuffer[U1RXBUFF_SIZE];
uint8_t U1RxDataTmp[U1RXBUFF_SIZE];
uint8_t U1RxInCnt = 0;
uint8_t U1RxOutCnt = 0;
void drvU1GetByte(uint8_t data)
{
	U1RxDataTmp[U1RxInCnt++] = data;
	if(U1RxInCnt >= U1RXBUFF_SIZE)
		U1RxInCnt = 0;
}

void drvU1DataCheck(void)
{
	while(U1RxInCnt!=U1RxOutCnt)
	{
		U1GetOneByte(U1RxDataTmp[U1RxOutCnt++]);
		if(U1RxOutCnt >= 100)
			U1RxOutCnt = 0;
	}
}
void DrvUartDataCheck(void)
{
	  drvU1DataCheck();
//	drvU2DataCheck();
//	drvU3DataCheck();
//	drvU4DataCheck();
//	drvU5DataCheck();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern TIM_HandleTypeDef htim4;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern UART_HandleTypeDef huart1;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
		uint16_t len =0;
    if (huart == &huart1)
    {      
        // ����������ݵĳ���
        len =  huart->RxXferSize - huart->hdmarx->Instance->CNDTR;  
			// �����յ������ݴ���ѭ������
        for (uint16_t i = 0; i < len; i++)
        {
            uint8_t data = U1RxBuffer[i];
            drvU1GetByte(data);
        }
        // ������������DMA
        HAL_UART_Receive_DMA(&huart1, U1RxBuffer, sizeof(U1RxBuffer)-1);
    }			
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	
    if (huart->Instance == USART1)
    {
        count1 = 0;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
 if(htim==&htim4)
 {
	 //==user code==
	 //��
	DrvUartDataCheck();
	 //��
	AnoPTv8HwTrigger1ms();
 }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
