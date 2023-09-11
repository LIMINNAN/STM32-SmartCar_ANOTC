#include "ws_drive_oled.h"
#include "oled_buffer.h"
#include "oled_color.h"
#include "string.h"

//  基于STM32硬件IIC的DMA传输实现OLED全缓存设计


uint8_t ScreenBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN]={0};
//定义双缓存
uint8_t TempBuffer[SCREEN_PAGE_NUM][SCREEN_COLUMN]={0};

static _Bool _SelectedBuffer=SCREEN_BUFFER;

#define BUFFERSIZE  sizeof(ScreenBuffer)
uint8_t oledDidBufPageCount = 0 ;
extern void UpdateTempBuffer(void);
extern void UpdateScreenBuffer(void);


void SetTempBuffer(void)
{

	_SelectedBuffer=TEMP_BUFFER;
}

void SetScreenBuffer(void)
{
	_SelectedBuffer=SCREEN_BUFFER;
}
uint8_t GetSelectedBuffer(void)
{
	return _SelectedBuffer;
}

uint8_t ReadByteBuffer(int page,int x)
{
	return _SelectedBuffer? ScreenBuffer[page][x] :TempBuffer[page][x];
}
	

void WriteByteBuffer(int page,int x,uint8_t byte)
{
	if(_SelectedBuffer)
	{
			ScreenBuffer[page][x] =byte;
	}
	else
	{
			TempBuffer[page][x] =byte;
	}
}

void SetPointBuffer(int x,int y,int value)
{
	if(x>SCREEN_COLUMN-1 ||y>SCREEN_ROW-1)   //超出范围
		return;
	if(_SelectedBuffer)
	{
		if(value)
			ScreenBuffer[y/SCREEN_PAGE_NUM][x] |= 1<<(y%SCREEN_PAGE_NUM);
		else
			ScreenBuffer[y/SCREEN_PAGE_NUM][x] &= ~(1<<(y%SCREEN_PAGE_NUM));	
	}
	else
	{
		if(value)
			TempBuffer[y/SCREEN_PAGE_NUM][x] |= 1<<(y%SCREEN_PAGE_NUM);
		else
			TempBuffer[y/SCREEN_PAGE_NUM][x] &= ~(1<<(y%SCREEN_PAGE_NUM));	
	
	}
}


uint8_t GetPointBuffer(int x,int y)
{
	if(x>SCREEN_COLUMN-1 ||y>SCREEN_ROW-1)   //超出范围
		return 0;
	if(_SelectedBuffer)
		return (ScreenBuffer[y/SCREEN_PAGE_NUM][x]>>(y%SCREEN_PAGE_NUM))&0x01;
	else
		return (TempBuffer[y/SCREEN_PAGE_NUM][x]>>(y%SCREEN_PAGE_NUM))&0x01;
}

void UpdateScreenDisplay(void)
{
    // Copy TempBuffer to ScreenBuffer
    TempBufferFunc(TEMPBUFF_COPY_TO_SCREEN);

    // Perform screen buffer update
    UpdateScreenBuffer();

    // Clear TempBuffer
    TempBufferFunc(TEMPBUFF_CLEAN);
		HAL_Delay(20);
}

void ClearScreenBuffer(uint8_t val)
{
	memset(ScreenBuffer,val,sizeof(ScreenBuffer));
}

void ClearTempBuffer(void)
{
	memset(TempBuffer,0,sizeof(TempBuffer));
}
//1.TEMPBUFF_COPY_TO_SCREEN
//2.TEMPBUFF_CLEAN
//3.TEMPBUFF_COVER_H
//4.TEMPBUFF_COVER_L
void TempBufferFunc(int func)
{
	int i,j;
	switch (func)
	{
		case TEMPBUFF_COPY_TO_SCREEN:
			memcpy(ScreenBuffer,TempBuffer,BUFFERSIZE);
			break;
		case TEMPBUFF_CLEAN:
			ClearTempBuffer();
			break;
		case TEMPBUFF_COVER_H:
				for(i=0;i<8;i++)
					for(j=0;j<128;j++)
						ScreenBuffer[i][j] |=TempBuffer[i][j];
				break;
		case TEMPBUFF_COVER_L:
				for(i=0;i<8;i++)
					for(j=0;j<128;j++)
						ScreenBuffer[i][j] &=~TempBuffer[i][j];
				break;
		default:
			break;	
	}	
}
