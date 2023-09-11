#ifndef _tPid_H
#define _tPid_H
#include "main.h"
//声明一个结构体类型
typedef struct
{
float target_val;//目标值
float actual_val;//实际值
float err;//当前偏差
float err_last;//上次偏差
float err_sum;//误差累计值
float Kp,Ki,Kd;//比例，积分，微分系数
} tPid;
extern tPid pidLpos_speed;
extern tPid pidRpos_speed;
extern tPid pidLpos_pos;//电机1的转速控制
extern tPid pidRpos_pos;//电机2的转速控制


//声明函数
void PID_init(void);

void settPid_target(tPid *pid, float temp_val);
float gettPid_target(tPid *pid);
void set_p_i_d(tPid *pid, float p, float i, float d);

float locationtPid_realize1(tPid *pid);
float locationtPid_realize2(tPid *pid);
float speedtPid_realize1(tPid *pid);
float speedtPid_realize2(tPid *pid);











////float SpeedtPid_Realize(tPid *pid, float actual_val,float target);
//float LPIDpos_realize(tPid *pid, float actual_val) ;
//float RPIDpos_realize(tPid *pid, float actual_val) ;//位置式pid控制器
//float LocationtPid_Realize(tPid *pid, float actual_val);//位置环

//float IncrementaltPidL(incPid *pid, float actual_val);//增量式pid
//float IncrementaltPidR(incPid *pid, float actual_val);//增量式pid
//float posIncrementaltPidL(incPid *pid, float actual_val);//增量式pid
//float posIncrementaltPidR(incPid *pid, float actual_val);//增量式pid
#endif
