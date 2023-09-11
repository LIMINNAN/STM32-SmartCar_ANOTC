#ifndef __MOTOR_H
#define __MOTOR_H
#include "pid.h"

#define Amplitude     1000      /* PWM������1000 */
#define Dead_Voltage  40      /* ������ѹ */


#define PWML TIM5->CCR4 
#define PWMR TIM5->CCR3 

#define SPEED_RECORD_NUM 20 // �����ԣ�50Hz������ֵ�����˲���Ч���ȽϺ�

/* ����������ֱ��� */
#define ENCODER_RESOLUTION                     11

/* ������Ƶ֮����ֱܷ��� */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4��Ƶ����ֱܷ��� */

/* ���ٵ�����ٱ� */
#define REDUCTION_RATIO  30

/*��sysTick�����PID��������ڣ��Ժ���Ϊ��λ*/
#define SPEED_PID_PERIOD  20    //���Ҫ����ʱ��7���ж�����


#define viewdis           38 	 //*�޸Ĺ۲���ֵ*
#define TARGET_SPEED_MAX  110  //*�޸��ٶ���ֵ*


#define HeadToWheelCM 10

typedef enum
{
  left_90,
	right_90,
	back_180,
	back_190,
	right_85
}spin_dir_t;



/* ����������ö�� */
typedef enum
{
  MOTOR_FWD = 0,
  MOTOR_REV,
}motor_dir_t;

//extern unsigned char TASK_NUM;
extern uint8_t is_motor_en;


extern float g_fTargetJourney;  //���С������������·�̺� �� ��λcm����Ҫ����һ�׶�����������

extern uint8_t GoStraighti_flag , Turn_flag;
extern  uint8_t  Spin_succeed_flag , Stop_Flag;
extern uint8_t Spin_start_flag , Spin_succeed_flag , Stop_Flag;
extern uint8_t stop_count , spin_count;
extern uint8_t  WaitFlag;
extern uint16_t WaitTimeCount;

void set_motorL_enable(void);
void set_motorR_enable(void);
void set_motorR_disable(void);
void set_motorL_disable(void);
void Motor_SetALL( int speedL,int speedR);
void Motor_SetL( int speed);
void Motor_SetR( int speed);
float Speed_Low_Filter(float new_Spe, float *speed_Record);
void motorPidSetSpeed(float Motor1Setpos,float Motor2Setpos);
void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed);	
void Location_Speed_control(void) ;
int Car_go(int32_t location_cm) ;
void spin_Turn(spin_dir_t zhuanxiang)  ;
#endif /* __MOTOR_H */


