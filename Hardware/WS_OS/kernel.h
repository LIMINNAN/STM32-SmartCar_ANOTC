


#ifndef __KERNEL_H__
#define __KERNEL_H__

#include "stm32f1xx_hal.h"


#define  TaskLen 20        //  任务最大个数



#define  WS_OK  0     
#define  WS_ERR 1


typedef enum 
{ 
  WS_Status_OK       = 0x00U,
  WS_Status_ERROR    = 0x01U,
  WS_Status_BUSY     = 0x02U,
	WS_Status_EMPTY    = 0x03U,
  WS_Status_TIMEOUT  = 0x04U
} WS_StatusTypeDef;



//       关闭、打开  CPU中断
#define  EN_INTERRUPT  __enable_irq();
#define  DI_INTERRUPT  __disable_irq();

#define  WS_OS_EN_INTERRUPT EN_INTERRUPT
#define  WS_OS_DI_INTERRUPT DI_INTERRUPT

//       任务控制块
struct wstask ;
typedef struct wstask  Task;
typedef Task   *WspTsak;
struct wstask 
{
	 short  int   ID;
	 int          Time   ;
	 int         (*fun)(void *) ;
	 void         *parameter    ;
	 WspTsak  next;
	 	
#if  WS_OS_MSG_EN > 0
    Message msg;
#endif	
};




void AddRunTask(WspTsak ptk)  ; //  添加待执行的任务
void AddOnTask(WspTsak ptk)   ;      //  添加任务到定时区
void AddHangTask(WspTsak ptk) ;    //  添加任务到挂起列表   


extern void InitTask(void) ;   //  初始化任务管理器
extern void OnTask(void)   ;  //  任务调度管理器
extern void RunTask(void)  ;  //  任务执行器



//   创建任务函数,将某个函数作为任务，可以完成定时调用等功能
//   参数：ID：任务ID，不能重复，否侧将创建失败
//         fun: 任务函数名
//         time: 创建后，延时time * 10 ms 后执行，如果是0，则暂时不执行，由StopTask启动
extern char CreateTask(char id,int (*fun)(void *),int time);  //  创建任务

//   创建定时函数,延时time * 10 ms 后，执行该函数
//         fun: 任务函数名
//         time: 创建后，延时time * 10 ms 后执行
extern char CreateTimer(int (*fun)(void *),int time);         //  创建定时器任务

//   让某个任务开始执行
//         id: 任务id
//         time: 启动后，延时time * 10 ms 后执行，如果为0，立刻执行
extern char StartTask(char id,int time);  //  启动挂起任务
//   停止一个任务
extern char StopTask(char id)   ;         //  将执行任务挂起
extern char DeleteTask(char id) ;         //  删除任务


//   WSOS重命名
#define WS_OS_Init_Task  InitTask
#define WS_OS_On_Task    OnTask
#define WS_OS_Run_Task   RunTask

#define WS_OS_Create_Task   CreateTask
#define WS_OS_Create_Timer  CreateTimer


#define WS_OS_Start_Task   StartTask
#define WS_OS_Stop_Task    StopTask
#define WS_OS_Delete_Task  DeleteTask

//   写入任务参数
char WS_OS_Write_Task_Parameter(short int id , void *parameter, int delayTime );




//char setTaskMsg(char taskid,Message msg);
#if WS_MSG_EN > 0



#endif


#endif
