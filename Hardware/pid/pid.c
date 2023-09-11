#include "pid.h"
#include "niming.h"
#include "math.h"
//����һ���ṹ�����ͱ���
tPid pidLpos_speed;
tPid pidRpos_speed;
tPid pidLpos_pos;
tPid pidRpos_pos;
//���ṹ�����ͱ�������ֵ
void PID_init(void)
{
	
	/* λ����س�ʼ������ */
    pidLpos_pos.target_val=0.0;				
    pidLpos_pos.actual_val=0.0;
    pidLpos_pos.err=0.0;
    pidLpos_pos.err_last=0.0;
    pidLpos_pos.err_sum=0.0;
  
		pidLpos_pos.Kp = 0.38;
		pidLpos_pos.Ki = 0;
		pidLpos_pos.Kd = 0;
  
  	/* �ٶ���س�ʼ������ */
    pidLpos_speed.target_val=0.0;				
    pidLpos_speed.actual_val=0.0;
    pidLpos_speed.err=0.0;
    pidLpos_speed.err_last=0.0;
    pidLpos_speed.err_sum=0.0;
  
		pidLpos_speed.Kp = 2.2;
		pidLpos_speed.Ki = 0.48;
		pidLpos_speed.Kd = 0.8;
		
			/* λ����س�ʼ������ */
    pidRpos_pos.target_val=0.0;				
    pidRpos_pos.actual_val=0.0;
    pidRpos_pos.err=0.0;
    pidRpos_pos.err_last=0.0;
    pidRpos_pos.err_sum=0.0;
  
		pidRpos_pos.Kp = 0.38;
		pidRpos_pos.Ki = 0;
		pidRpos_pos.Kd = 0;
  
  	/* �ٶ���س�ʼ������ */
    pidRpos_speed.target_val=0.0;				
    pidRpos_speed.actual_val=0.0;
    pidRpos_speed.err=0.0;
    pidRpos_speed.err_last=0.0;
    pidRpos_speed.err_sum=0.0;
  
		pidRpos_speed.Kp = 2.2;
		pidRpos_speed.Ki = 0.48;
		pidRpos_speed.Kd = 0.8;
		
}
void settPid_target(tPid *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}
float gettPid_target(tPid *pid)
{
  return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
}
void set_p_i_d(tPid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // ���ñ���ϵ�� P
		pid->Ki = i;    // ���û���ϵ�� I
		pid->Kd = d;    // ����΢��ϵ�� D
}
float locationtPid_realize1(tPid *pid)
{
	  static float output = 0;
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-pid->actual_val;
		/*PID�㷨ʵ��*/
    output = pid->Kp*pid->err              
		         +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return output;
}
float locationtPid_realize2(tPid *pid)
{
	  static float output = 0;
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-pid->actual_val;
		/*PID�㷨ʵ��*/
    output = pid->Kp*pid->err              
		         +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return output;
}
/**
  * @brief  �ٶ�PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
*/
/*  pidRpos_speed.Kp = 1.32;
		pidRpos_speed.Ki = 0.22;
		pidRpos_speed.Kd = 0.09;*/

float speedtPid_realize1(tPid *pid){
	
		
		static float err_dublast =0,output;	
	  if( fabs(pid->target_val)<= 3 ){return 0;}
		pid->err=pid->target_val-pid->actual_val;
		output += (pid->Kp*(pid->err-pid->err_last))
						 +(pid->Ki*pid->err)
						 +(pid->Kd*(pid->err-2*pid->err_last+err_dublast));
   if(output >= 900){return 900;}
		err_dublast = pid->err_last;	
		pid->err_last=pid->err;
		return output;
}
float speedtPid_realize2(tPid *pid){
	
		
		static float err_dublast =0,output;	
	  if( fabs(pid->target_val)<= 3 ){return 0;}
		pid->err=pid->target_val-pid->actual_val;
		output += (pid->Kp*(pid->err-pid->err_last))
						 +(pid->Ki*pid->err)
						 +(pid->Kd*(pid->err-2*pid->err_last+err_dublast));
   if(output >= 900){return 900;}
		err_dublast = pid->err_last;	
		pid->err_last=pid->err;
		return output;
}