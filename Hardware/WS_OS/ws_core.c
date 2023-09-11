#include "ws_core.h"
#include <stdarg.h> //  标准头文件
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

//   格式化可变变量字符串缓存
char formatBuf[64];
//   格式化可变变量字符串
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
    /*  格式化数据  */
    va_list ap;
    va_start(ap, p);
    vsprintf(formatBuf, p, ap);
    va_end(ap);

//  编译控制
#ifdef DEBUG_EN
    //  信息输出接口
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)formatBuf, strlen(formatBuf));

#endif
    WS_Delay_US(10000);
}
