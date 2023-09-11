
#include "DWT.h"

uint32_t  dwtCountBuf[10] = {0};


//   计算计数差值
uint32_t  getDWTCountDt(uint32_t startTick , uint32_t  stopTick )
{
	   uint32_t tick;
     if(stopTick < startTick)
			{
				 tick = (0xffffffff - startTick) + stopTick  + 1 ;
			}
			else
			{
				 tick = stopTick - startTick;
			}
		 return tick;
}


/******************************************************************************
*函数名称： uint32_t WS_DWT_Get_Tick_Dt(uint8_t chx , uint8_t stu)
*功能：     利用调用的间隔测量系统时钟计数，72M频率对应1秒计数72000000个脉冲
              示范：
              WS_DWT_Get_Tick_Dt(0,0);
              WS_Delay_MS(10);
              time = WS_DWT_Get_Tick_Dt(0,2);
              time 变量存贮两次调用的系统时钟计数，
							测量WS_Delay_MS(10)函数执行完所需的系统时钟计数 
 形参：    mode:    0 ： 设定测量起点，返回0 ；
                   1 ： 测量终点，返回起点到终点得测量时间,起点时间不修改
                   2 ： 测量起点/重点，每次调用测量上次调用到本次调用的时间
           chx:    测量通道（0~9）， 支持多路同时测量
*返回:		  无
*******************************************************************************/

uint32_t WS_DWT_Get_Tick_Dt(uint8_t chx , uint8_t stu)
{
   uint32_t  tick  = 0 ; 
   static    uint8_t  initFlag = 0;
	
	 //  首次调用， 完成初始化
   if(initFlag == 0)
	 {
	    initFlag = 1;
		  //  DWT 计数初始化
			CoreDebug->DEMCR |= 1<<24 ;  // 使能DWT外设
			DWT->CYCCNT       = 0     ;  // 清零CYCCNT
			DWT->CTRL        |= 1<<0  ;  // 使能计数
	 }	
	 
	 if(chx >= 10)  return 0;  //  超过通道数，直接返回0
	 
	 switch(stu)
	 {
		 // 设定起点
		 case 0:  
							dwtCountBuf[chx]  = DWT->CYCCNT ;  // 存储起始值
			        break;
		 
		 //  设定重点，返回测量时间
		 case 1:
			        tick  = getDWTCountDt(dwtCountBuf[chx],DWT->CYCCNT);
		          break;
		 //  连续测量
		 case 2:    
			        tick  = getDWTCountDt(dwtCountBuf[chx],DWT->CYCCNT);
		          dwtCountBuf[chx]  = DWT->CYCCNT ;  
			        break;
	 }
	 return tick;
}
