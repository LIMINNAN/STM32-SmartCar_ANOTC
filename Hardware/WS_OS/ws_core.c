#include "ws_core.h"
#include <stdarg.h> //  ��׼ͷ�ļ�
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;

void WS_Delay_US(volatile uint32_t z)
{
    volatile int x;
    for (; z > 0; z--)
        for (x = 10; x > 0; x--)
            ;
}

//   ��ʽ���ɱ�����ַ�������
char formatBuf[64];
//   ��ʽ���ɱ�����ַ���
char *WS_Format(char *p, ...)
{
    va_list ap;
    va_start(ap, p);
    vsprintf(formatBuf, p, ap);
    va_end(ap);
    return formatBuf;
}

void WS_Debug(char *p, ...)
{
    /*  ��ʽ������  */
    va_list ap;
    va_start(ap, p);
    vsprintf(formatBuf, p, ap);
    va_end(ap);

//  �������
#ifdef DEBUG_EN
    //  ��Ϣ����ӿ�
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)formatBuf, strlen(formatBuf));

#endif
    WS_Delay_US(10000);
}
