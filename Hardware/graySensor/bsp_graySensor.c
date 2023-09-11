#include "bsp_GraySensor.h"
#include "main.h"
#include "adc.h"
#include "motor.h"

u8 L2_Val, L1_Val, M_Val, R1_Val, R2_Val;
int Line_Num;


u16 ADC_Value;
float ADC_Volt;
uint8_t Load_flag =0;  

u8 LoadCount,NotLoadCount;

/*******************灰度传感器巡线*************/
//只要把速度环控制好，补偿值即可确定。  不同速度值需要不同的补偿值。  测试好一个最好的。  不同的转速要对应不同的补偿系数或数值
void Light_GoStraight_control(void)   //灰度巡线直行, 需要有个判断需要直行多长距离的函数    //补偿要到中间的传感器测到回到线上为止
{
	  Get_Light_TTL();  
		if(L2_Val==0&&L1_Val==0&&  M_Val == 1&&R1_Val==0&&R2_Val==0) Line_Num =  0;
	
		if(L1_Val==1&&R1_Val==1&&L2_Val==1&&R2_Val==1){ Line_Num =  0;return;}
		else if(L2_Val==1&&L1_Val==1&&  M_Val == 1&&R1_Val==1&&R2_Val==1){ Line_Num =  0;return;}
	
	  if(L2_Val == 0 && L1_Val == 0 &&  M_Val == 0 && R1_Val == 1 && R2_Val == 0) Line_Num   = 95;
	  if(L2_Val == 0 && L1_Val == 1 &&  M_Val == 0 && R1_Val == 0 && R2_Val == 0) Line_Num   = -95;
		
		
	  
}


//把灰度传感器当作只输出高低电平。 高电平是识别到红线了。
void Get_Light_TTL(void)  
{
	if(SensorLL == GPIO_PIN_SET){L2_Val = 1;} else {L2_Val = 0;}
	if(SensorL == GPIO_PIN_SET) {L1_Val = 1;} else {L1_Val = 0;}
	if(SensorM== GPIO_PIN_SET)  {M_Val = 1;} 	else {M_Val = 0;}
	if(SensorR== GPIO_PIN_SET)  {R1_Val = 1;} else {R1_Val = 0;}
	if(SensorRR == GPIO_PIN_SET){R2_Val = 1;} else {R2_Val = 0;}
}
uint16_t ADC_Value_convert_result[6];
float ADC_Value_Record[6]={0};
/*
 * 进行速度的平均滤波
 * 输入新采样到的速度，存放速度的数组，
 * 返回滤波后的速度
 */
uint16_t ADC_Value_Low_Filter(float new_Spe,float *ADC_Value_Record)
{
    uint16_t sum = 0;  
    for(uint8_t i=6-1;i>0;i--)//将现有数据后移一位
    {
        ADC_Value_Record[i] = ADC_Value_Record[i-1];
        sum += ADC_Value_Record[i-1];
    }
    ADC_Value_Record[0] = new_Spe;//第一位是新的数据
    sum += new_Spe;
    return sum/6;//返回均值
}

uint16_t ADC_Value;
uint8_t LoADC_Valueount,NotLoADC_Valueount;
float ADC_Value_Volt;

void LoadOrNot(void)
{
//			HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Value_convert_result, 6);//u16 ADC_Value_convert_result[6];
//			if(HAL_IS_BIT_SET(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_Value_convert_result, 6),HAL_ADC_STATE_REG_EOC)==0)
//			{				
//				for(uint8_t i =0;i<=5;i++)
//				{
//	        ADC_Value=ADC_Value_Low_Filter(ADC_Value_convert_result[i],ADC_Value_Record);
//					// WS_OLED_Printf(0,6,0,"%d",ADC_Value);
//				  ADC_Value_Volt	 =  (float)ADC_Value*3.3/4096.0;
//				  WS_OLED_Printf(35,6,0,"%f",ADC_Value_Volt);	
//					
//				}
//			}			
//			if(ADC_Value_Volt >= 1.6)
//			{
//				NotLoADC_Valueount = 0;				
//				LoADC_Valueount++;
//				if(LoADC_Valueount > 1) //3次程序执行到这之后再次判断
//				{
//					Load_flag = 1;
//					WS_OLED_Printf(0,7,0,"Drug is Load!");

//				}
//			}
//			else if(ADC_Value_Volt < 1.42)
//			{
//				LoADC_Valueount = 0;
//				
//				if(Load_flag == 1)    //必须先装载过药品的情况下才能判断是否拿走药品
//				{
//					NotLoADC_Valueount++;
//					if(NotLoADC_Valueount > 1 )  //3次程序执行到这之后再次判断
//					{
//						Load_flag = 2;
//						WS_OLED_Printf(0,7,0,"Drug is UnLoad!");
//					}
//			  }
//			}
}


