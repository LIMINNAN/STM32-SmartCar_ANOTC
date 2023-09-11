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

/*******************�Ҷȴ�����Ѳ��*************/
//ֻҪ���ٶȻ����ƺã�����ֵ����ȷ����  ��ͬ�ٶ�ֵ��Ҫ��ͬ�Ĳ���ֵ��  ���Ժ�һ����õġ�  ��ͬ��ת��Ҫ��Ӧ��ͬ�Ĳ���ϵ������ֵ
void Light_GoStraight_control(void)   //�Ҷ�Ѳ��ֱ��, ��Ҫ�и��ж���Ҫֱ�ж೤����ĺ���    //����Ҫ���м�Ĵ������⵽�ص�����Ϊֹ
{
	  Get_Light_TTL();  
		if(L2_Val==0&&L1_Val==0&&  M_Val == 1&&R1_Val==0&&R2_Val==0) Line_Num =  0;
	
		if(L1_Val==1&&R1_Val==1&&L2_Val==1&&R2_Val==1){ Line_Num =  0;return;}
		else if(L2_Val==1&&L1_Val==1&&  M_Val == 1&&R1_Val==1&&R2_Val==1){ Line_Num =  0;return;}
	
	  if(L2_Val == 0 && L1_Val == 0 &&  M_Val == 0 && R1_Val == 1 && R2_Val == 0) Line_Num   = 95;
	  if(L2_Val == 0 && L1_Val == 1 &&  M_Val == 0 && R1_Val == 0 && R2_Val == 0) Line_Num   = -95;
		
		
	  
}


//�ѻҶȴ���������ֻ����ߵ͵�ƽ�� �ߵ�ƽ��ʶ�𵽺����ˡ�
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
 * �����ٶȵ�ƽ���˲�
 * �����²��������ٶȣ�����ٶȵ����飬
 * �����˲�����ٶ�
 */
uint16_t ADC_Value_Low_Filter(float new_Spe,float *ADC_Value_Record)
{
    uint16_t sum = 0;  
    for(uint8_t i=6-1;i>0;i--)//���������ݺ���һλ
    {
        ADC_Value_Record[i] = ADC_Value_Record[i-1];
        sum += ADC_Value_Record[i-1];
    }
    ADC_Value_Record[0] = new_Spe;//��һλ���µ�����
    sum += new_Spe;
    return sum/6;//���ؾ�ֵ
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
//				if(LoADC_Valueount > 1) //3�γ���ִ�е���֮���ٴ��ж�
//				{
//					Load_flag = 1;
//					WS_OLED_Printf(0,7,0,"Drug is Load!");

//				}
//			}
//			else if(ADC_Value_Volt < 1.42)
//			{
//				LoADC_Valueount = 0;
//				
//				if(Load_flag == 1)    //������װ�ع�ҩƷ������²����ж��Ƿ�����ҩƷ
//				{
//					NotLoADC_Valueount++;
//					if(NotLoADC_Valueount > 1 )  //3�γ���ִ�е���֮���ٴ��ж�
//					{
//						Load_flag = 2;
//						WS_OLED_Printf(0,7,0,"Drug is UnLoad!");
//					}
//			  }
//			}
}


