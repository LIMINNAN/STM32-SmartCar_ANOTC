#ifndef _tPid_H
#define _tPid_H
#include "main.h"
//����һ���ṹ������
typedef struct
{
float target_val;//Ŀ��ֵ
float actual_val;//ʵ��ֵ
float err;//��ǰƫ��
float err_last;//�ϴ�ƫ��
float err_sum;//����ۼ�ֵ
float Kp,Ki,Kd;//���������֣�΢��ϵ��
} tPid;
extern tPid pidLpos_speed;
extern tPid pidRpos_speed;
extern tPid pidLpos_pos;//���1��ת�ٿ���
extern tPid pidRpos_pos;//���2��ת�ٿ���


//��������
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
//float RPIDpos_realize(tPid *pid, float actual_val) ;//λ��ʽpid������
//float LocationtPid_Realize(tPid *pid, float actual_val);//λ�û�

//float IncrementaltPidL(incPid *pid, float actual_val);//����ʽpid
//float IncrementaltPidR(incPid *pid, float actual_val);//����ʽpid
//float posIncrementaltPidL(incPid *pid, float actual_val);//����ʽpid
//float posIncrementaltPidR(incPid *pid, float actual_val);//����ʽpid
#endif
