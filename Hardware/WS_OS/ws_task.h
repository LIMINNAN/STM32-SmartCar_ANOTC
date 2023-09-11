/******************************************************************************
 * @file     				 ws_task.c
 * @brief    				 ws程序框架下的任务管理
 * @version  				 1.0.0
 * @creationTime     2020_02_29
 * @creator					 
 * @readme
 *        1.STM32F103系列   
 *				2. 
          3.
*******************************************************************************/


#ifndef _WS_TASK_H__
#define _WS_TASK_H__

#include "main.h"

//   任务ID定义
#define WS_OS_Task_Openmv_Dynamic_Recognition_Target_define 1
#define WS_OS_Task_LoadOrNot_define 2
#define WS_OS_Task_Dynamic_Recognition_define 3
#define WS_OS_Task_Goto12_define 4
#define task_timer_define 5
#define WS_OS_Task_Dynamic_Recognitiondd_define 6
#define WS_OLED_Task_Buffer_Update_define 7
#define WS_OS_Task_Load_define 8
#define WS_LoRa_Task_ReceData_define 9
extern void WS_OS_Task_Create(void) ;



#endif

