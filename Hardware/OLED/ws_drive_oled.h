

#ifndef __DRIVE_OLED_H__
#define __DRIVE_OLED_H__	

#include "main.h"
#include "stdlib.h"	

//  1 : 显存模式，DMA传输 ，节省CPU时间，需要周期（50ms）调用 WS_OLED_Start_Buffer_Update  函数
//  0 ：寄存器模式，直接写数据OLED，比较耗时，数据写入是阻塞模式
#define   WS_OLED_EN_DIS_BUFFER    1

#define 	SCREEN_PAGE_NUM			   	(8)
#define   SCREEN_PAGEDATA_NUM   	(128)
#define 	SCREEN_COLUMN 					(128)
#define  	SCREEN_ROW 							(64)

#define		SCREEN_WIDTH 	SCREEN_COLUMN
#define 	SCREEN_HEIGHT SCREEN_ROW

#define ASCII12X24Code_EN  1    //  设置0 ， 失能12*24点字符显示 ， 设置1 ， 使能12*24点字符显示 
#define ASCII16X32Code_EN  1    //  设置0 ， 失能16*36点字符显示 ， 设置1 ， 使能16*32点字符显示  


typedef enum 
{
	TEXT_BK_NULL=0,
	TEXT_BK_NOT_NULL,
}Type_textbk;
  

//  开启OLED显示    
void WS_OLED_Open_Dis(void);

// 关闭OLED显示     
void WS_OLED_Close_Dis(void); 

//初始化SSD1306					    
void OLEDInit(void);

// 全屏熄灭	  
void WS_OLED_Set_Clear(void) ;

// 连续写数据进入OLED 
char WS_OLED_Write_Buffer(char adds , unsigned  char *data  , int len);

//  启动一次DMA传输，实训缓存更新
void UpdateScreenBuffer(void);
void UpdateTempBuffer(void);

//   初始化
void   WS_OLED_Init(void );

//   字符串显示
void WS_OLED_Dis_String(int  x,int   y,   char disMode, char  *string);

//   格式化显示
void WS_OLED_Printf(int  x,int  y, char disMode , char *p,... );

void WS_OLED_Clear(void)  ;

int WS_OLED_Task_Buffer_Update(void *date);
void WS_OLED_FILL_BMP(unsigned char BMP[]);


void WS_OLED_Write_Cmd(unsigned char cmd);
#endif  

