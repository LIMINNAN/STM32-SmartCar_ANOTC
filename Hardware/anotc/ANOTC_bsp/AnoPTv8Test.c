#include "AnoPTv8Test.h"
#include "Ano_Math.h"
#include "math.h"
#include "main.h"
void AnoPTv8TxFrameF1(void)
{
//	static uint16_t scnt = 0;
//	scnt++;
//	if(scnt >= 360)
//		scnt = 0;
//	

//	float v2 = my_sin((double)scnt / 180 * 3.14159) * 180 + 360;
//	uint8_t databuf[6];

//	databuf[0] = BYTE0(v2);
//	databuf[1] = BYTE1(v2);
//	databuf[2] = BYTE2(v2);
//	databuf[3] = BYTE3(v2);
//	AnoPTv8SendBuf(ANOPTV8_SWJID, 0xF1, databuf, 4);
}

void AnoPTv8TxFrameF2(void)
{
	static uint16_t scnt = 0;
	scnt++;
	if(scnt >= 360)
		scnt = 0;
	
	uint16_t v1 = scnt;
	float v2 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
	float v3 = sin((double)(scnt+120) / 180 * 3.14159) * 180 + 180;
	float v4 = sin((double)(scnt+240) / 180 * 3.14159) * 180 + 180;
	
	
	uint8_t databuf[14];
	databuf[0] = BYTE0(v1);
	databuf[1] = BYTE1(v1);
	databuf[2] = BYTE0(v2);
	databuf[3] = BYTE1(v2);
	databuf[4] = BYTE2(v2);
	databuf[5] = BYTE3(v2);
	databuf[6] = BYTE0(v3);
	databuf[7] = BYTE1(v3);
	databuf[8] = BYTE2(v3);
	databuf[9] = BYTE3(v3);
	databuf[10] = BYTE0(v4);
	databuf[11] = BYTE1(v4);
	databuf[12] = BYTE2(v4);
	databuf[13] = BYTE3(v4);
	
	AnoPTv8SendBuf(ANOPTV8_SWJID, 0xF2, databuf, 14);

}



