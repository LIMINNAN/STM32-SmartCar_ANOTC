#ifndef _REALIZE_H__
#define _REALIZE_H__

#include <stdint.h>
#define TX_BUFFER_SIZE 256
#define U1RXBUFF_SIZE  100
void DrvUart1SendBuf(unsigned char *DataToSend, uint8_t data_num);
void DrvUartDataCheck(void);
void drvU1DataCheck(void);
void drvU1GetByte(uint8_t data);
uint32_t GetSysRunTimeMs(void);
uint32_t GetSysRunTimeUs(void);

#endif
