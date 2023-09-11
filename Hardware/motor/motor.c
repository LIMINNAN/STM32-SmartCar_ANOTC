#include "main.h"
#include "tim.h"
#include "motor.h"
#include "stdio.h"
#include "string.h"
#include "pid.h"
#include "encoder.h"
#include <math.h>

extern tPid pidLpos_speed;
extern tPid pidRpos_speed;
extern tPid pidLpos_pos;
extern tPid pidRpos_pos;

/*****************����Ŀ��ƺ���***************/

uint8_t is_motorL_en, is_motorR_en;   // ���ʹ�ܱ�־
extern short EncoderLCount;						//������������ֵ
extern short EncoderRCount;
extern float Motor_journey_cm, Motor2_journey_cm;
float g_fTargetJourney = 0; 				 //���С������������·�̺� ����λcm����Ҫ����һ�׶�����������
float MotorLSpeedFilter;
float MotorRSpeedFilter;
float MotorLSetpossum;  //����λ��
float MotorRSetpossum;  //�ҵ��λ��
/***********************************/

/*******************PID************************/
extern float speed_Outval, location_Outval;
extern float speed2_Outval, location2_Outval;
extern float Line_Outval;
extern int Turn_val;
/**************************************************/
/*******************��־λ*********************************/
uint8_t Spin_start_flag , Spin_succeed_flag , Stop_Flag;
uint8_t GoStraighti_flag = 0, Turn_flag = 0 ;
uint8_t stop_count , spin_count;
uint16_t WaitTimeCount;
uint8_t WaitFlag , BackFlag;
/***********************************************************/
int Xianfu(int data,int max)
{	
    if(data<-max) data=-max;	
    if(data> max) data= max;	
    return data;
}

void Motor_SetL(int speed) {
	if(abs(speed) <=10 ){ PWML =0; return ;}
    if (speed < 0) {
        BIN_1H;
        BIN_2L;
    } else {
        BIN_2H;
        BIN_1L;
    }
    speed = abs(speed) + Dead_Voltage;/* ȡ����ֵ + ������ѹ*/
    speed = Xianfu(speed,Amplitude);   /* �޷� */
    PWML = speed;
}
void Motor_SetR(int speed) {
  	if(abs(speed) <=10 ){ PWMR =0; return ;}
    if (speed < 0) {
        AIN_1L;
        AIN_2H;
    } else {
        AIN_2L;
        AIN_1H;
    }
    speed = abs(speed) + Dead_Voltage; /* ȡ����ֵ + ������ѹ*/
    speed = Xianfu(speed,Amplitude);   /* �޷� */
    PWMR = speed;
}

void Motor_SetALL( int speedL,int speedR)
{
	Motor_SetR(speedR);
	Motor_SetL(speedL);
}
void set_motorR_disable(void)
{
	 is_motorR_en  = 0;
	 AIN_1H;
	 AIN_2H;
   HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_3);
}   
void set_motorR_enable(void)
{
	 is_motorR_en  = 1;
	 HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3); // R
} 

void set_motorL_disable(void)
{
	is_motorL_en  = 0;
  BIN_1H;
	BIN_2H;
	HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_4);

}
void set_motorL_enable(void)
{	
	is_motorL_en  = 1;
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4); // L    
  
}
float Lspeed_Record[SPEED_RECORD_NUM]={0};
float Rspeed_Record[SPEED_RECORD_NUM]={0};
/*
 * �����ٶȵ�ƽ���˲�
 * �����²��������ٶȣ�����ٶȵ����飬
 * �����˲�����ٶ�
 */
float Speed_Low_Filter(float new_Spe, float *speed_Record)
{
	float sum = 0.0f;
	for (uint8_t i = SPEED_RECORD_NUM - 1; i > 0; i--)
	{
		speed_Record[i] = speed_Record[i-1];
		sum = fmaf(speed_Record[i-1], 1.0f, sum); // ʹ��fma�����ۼӺ�
	}
		speed_Record[0] = new_Spe;
		sum = fmaf(new_Spe, 1.0f, sum);
	return sum / SPEED_RECORD_NUM;
}

float speedR_pid_control(void)  
{ 
	float cont_val = 0.0;                         
	cont_val = speedtPid_realize2(&pidRpos_speed); 
	return cont_val;
}
float speedL_pid_control(void)  
{
	 float cont_val = 0.0;                        
	 cont_val = speedtPid_realize1(&pidLpos_speed);
	 return cont_val;
}
float locationR_pid_control(void)  
{
		float cont_val = 0.0; 
	  cont_val = locationtPid_realize1(&pidRpos_pos);   
		if (cont_val > TARGET_SPEED_MAX)
		{
			cont_val = TARGET_SPEED_MAX;
		}
		else if (cont_val < -TARGET_SPEED_MAX)
		{
			cont_val = -TARGET_SPEED_MAX;
		}
	  return cont_val;
}
float locationL_pid_control(void)  
{
	float cont_val = 0.0; 
	cont_val = locationtPid_realize2(&pidLpos_pos);   
	if (cont_val > TARGET_SPEED_MAX)
	{
		cont_val = TARGET_SPEED_MAX;
	}
	else if (cont_val < -TARGET_SPEED_MAX)
	{
		cont_val = -TARGET_SPEED_MAX;
	}
	return cont_val;
}


/****�ٶȻ�λ�û�����PID����*****/   
void Location_Speed_control(void)
{ 
		 	stop_count++;   
		static uint8_t location_control_count=0 ;
		 if (is_motorL_en == 1||is_motorR_en == 1)     
		 {
				 location_control_count++;
				if(location_control_count >= 2)
				{
					location_control_count = 0; 
					
					location_Outval  = locationL_pid_control();	
					location2_Outval = locationR_pid_control();
				}		
				settPid_target(&pidLpos_speed, location_Outval); 
				settPid_target(&pidRpos_speed, location2_Outval);		 
				speed_Outval =  speedL_pid_control();
				speed2_Outval = speedR_pid_control();
			}
}
/*******************ת��*******************************************/
void spin_Turn(spin_dir_t zhuanxiang)   
{
	
	  GoStraighti_flag = 0; 
	  Stop_Flag = 0;        
	  
	
    Spin_start_flag = 1;   
	  Spin_succeed_flag = 0; 
	
		float spin90_val;
		float Car_Turn_val;
		
		Motor_journey_cm = 0;
		Motor2_journey_cm = 0;
		
		spin90_val = 0.25*3.1415*20;
		
	/****ת���ѭ�߿��Ե�ת��ϵ��**********/
		if(zhuanxiang == left_90)  //openmvʶ����Ҫ����߲�����
		{
			Car_Turn_val = ( spin90_val / (6.5 * 3.14) ) *   (30*4*11) ; 
			Car_Turn_val = 0.86* Car_Turn_val;   //90*0.94 = 84.6   //����Ӱ�죬����ת��������趨�Ķࡣֱ����90�Ƚ�������Ѳ�߾�Ѱ��������
			
		}
		else if(zhuanxiang == right_90)  //openmvʶ����Ҫ���ұ߲�����
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.14) ) *   (30*4*11); 
			Car_Turn_val = 0.86*Car_Turn_val;  //90*0.96 = 86.4    //����Ӱ�죬����ת��������趨�Ķࡣ ��������Ѳ�߿��ܾ�Ѱ��������
		}
		else if(zhuanxiang == back_180)
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.14) ) *   (30*4*11);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
			Car_Turn_val = 1.89*Car_Turn_val;      //0.96*180 = 175.5    //*����Ӱ�죬��Ҫ�޸�ת��180���е�ϵ��1.9		*						
		}
			else if(zhuanxiang == back_190)
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.14) ) *   (30*4*11);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
			Car_Turn_val = 1.78*Car_Turn_val;      //0.96*180 = 175.5    //*����Ӱ�죬��Ҫ�޸�ת��180���е�ϵ��1.9		*						
		}
		else if(zhuanxiang == right_85)
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.14) ) *   (30*4*11);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
			Car_Turn_val = 0.85*Car_Turn_val;      //0.96*180 = 175.5    //*����Ӱ�죬��Ҫ�޸�ת��180���е�ϵ��1.9		*						
		}
		
			settPid_target(&pidLpos_pos, -Car_Turn_val);   
			settPid_target(&pidRpos_pos, Car_Turn_val);
		
			g_lMotorPulseSigma = 0;    
			g_lMotor2PulseSigma = 0; 
		
			set_motorL_enable();   //ʹ�ܵ������PWM���    
		  set_motorR_enable();   //ʹ�ܵ��2����PWM���
}

int Car_go(int32_t location_cm)   //ֱ�ߺ���     
{
	float Car_location;
	
	GoStraighti_flag = 1;   
	Stop_Flag = 0;
	
	Spin_start_flag = 0;
	Spin_succeed_flag = 0;  
	
	Motor_journey_cm = 0;
	Motor2_journey_cm = 0;
	
	g_fTargetJourney = location_cm;   //��ֹ��ʱ��PID������
	Car_location   = ( location_cm / 21.3628) *1320;  
 	
	settPid_target(&pidLpos_pos, Car_location);   
	settPid_target(&pidRpos_pos, Car_location);

	set_motorL_enable();   //ʹ�ܵ������PWM���    
	set_motorR_enable();   //ʹ�ܵ��2����PWM���
 
	g_lMotorPulseSigma = 0;    
	g_lMotor2PulseSigma = 0; 	
	if(Car_location <= 1320)return 30;
	if(Car_location <= 2640)return 25;
	if(Car_location >= 7920)return 10;
	return 15;
}

//void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed)
//{	  
//	  LEDROFF;			
//		GoStraighti_flag=1;
//		settPid_target(&pidLpos_speed,Motor1SetSpeed);
//		settPid_target(&pidRpos_speed,Motor2SetSpeed);
//		set_motorL_enable();   //ʹ�ܵ������PWM���    
//		set_motorR_enable();   //ʹ�ܵ��2����PWM���		
//}
void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed)
{	  
	
		GoStraighti_flag=1;
		settPid_target(&pidLpos_pos,Motor1SetSpeed);
		settPid_target(&pidRpos_pos,Motor2SetSpeed);
		set_motorL_enable();   //ʹ�ܵ������PWM���    
		set_motorR_enable();   //ʹ�ܵ��2����PWM���
		g_lMotorPulseSigma = 0;    
	  g_lMotor2PulseSigma = 0; 
}