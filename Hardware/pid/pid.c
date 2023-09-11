#include "pid.h"
#include "niming.h"
#include "math.h"
//定义一个结构体类型变量
tPid pidLpos_speed;
tPid pidRpos_speed;
tPid pidLpos_pos;
tPid pidRpos_pos;
//给结构体类型变量赋初值
void PID_init(void)
{
	
	/* 位置相关初始化参数 */
    pidLpos_pos.target_val=0.0;				
    pidLpos_pos.actual_val=0.0;
    pidLpos_pos.err=0.0;
    pidLpos_pos.err_last=0.0;
    pidLpos_pos.err_sum=0.0;
  
		pidLpos_pos.Kp = 0.38;
		pidLpos_pos.Ki = 0;
		pidLpos_pos.Kd = 0;
  
  	/* 速度相关初始化参数 */
    pidLpos_speed.target_val=0.0;				
    pidLpos_speed.actual_val=0.0;
    pidLpos_speed.err=0.0;
    pidLpos_speed.err_last=0.0;
    pidLpos_speed.err_sum=0.0;
  
		pidLpos_speed.Kp = 2.2;
		pidLpos_speed.Ki = 0.48;
		pidLpos_speed.Kd = 0.8;
		
			/* 位置相关初始化参数 */
    pidRpos_pos.target_val=0.0;				
    pidRpos_pos.actual_val=0.0;
    pidRpos_pos.err=0.0;
    pidRpos_pos.err_last=0.0;
    pidRpos_pos.err_sum=0.0;
  
		pidRpos_pos.Kp = 0.38;
		pidRpos_pos.Ki = 0;
		pidRpos_pos.Kd = 0;
  
  	/* 速度相关初始化参数 */
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
  pid->target_val = temp_val;    // 设置当前的目标值
}
float gettPid_target(tPid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}
void set_p_i_d(tPid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // 设置比例系数 P
		pid->Ki = i;    // 设置积分系数 I
		pid->Kd = d;    // 设置微分系数 D
}
float locationtPid_realize1(tPid *pid)
{
	  static float output = 0;
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-pid->actual_val;
		/*PID算法实现*/
    output = pid->Kp*pid->err              
		         +pid->Kd*(pid->err-pid->err_last);
  
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return output;
}
float locationtPid_realize2(tPid *pid)
{
	  static float output = 0;
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-pid->actual_val;
		/*PID算法实现*/
    output = pid->Kp*pid->err              
		         +pid->Kd*(pid->err-pid->err_last);
  
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return output;
}
/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
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