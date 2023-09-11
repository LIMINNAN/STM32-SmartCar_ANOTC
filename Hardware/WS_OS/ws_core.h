/*****************************
编写核心代码，尽量做到不依赖硬件



*****************************/


#ifndef __CORE_H__
#define __CORE_H__

#include "main.h"

#define DEBUG_EN 1


//   格式化可变变量字符串
char *WS_Format(char *p,...);

//   调试输出
void WS_Debug( char *p,...);


void WS_Delay_US(uint32_t z);






#endif



