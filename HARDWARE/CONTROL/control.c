#include "control.h"
#include "Rc.h"
#include "bsp_motor_tim4.h"
#include "Struct_all.h"
/*****************************���ڴ���ң�����������PID������ۺϿ��Ƴ���**********************************
*2021.2.17 ���û��PID��Ԥ��ң������ͨ����ǰ�����ң����ţ�ת�򣩵��ۺϿ���,��������֡������ֱ�Ӵ���
*
*
*
*
*1�ŵ����ǰ��= ��ͣ���� + ������� - ǰ�ɵ��� - ��ת����
*2�ŵ����ǰ�ң�= ��ͣ���� - ������� - ǰ�ɵ��� + ��ת����
*3�ŵ��������= ��ͣ���� + ������� + ǰ�ɵ��� + ��ת����
*4�ŵ�������ң�= ��ͣ���� - ������� + ǰ�ɵ��� - ��ת����
**************/

/**  
  *  ���ܣ�����ʽ��ɢPID
  *  ��ڲ�����
  *  ����ֵ��
  */
void PID_Control(struct _pid *PID,uint16_t target,float measure,float max,float Integral_max)
{
	PID->Error = target - measure;
	PID->Deriv = PID->Error - PID->PreError;
	PID->PreError = PID->Error;
	
	//PID�޷�
	if(PID->Error > max)
		PID->Integral += max;
	if(PID->Error < -max)
		PID->Integral += -max;
	else
		PID->Integral += PID->Error;	
	if(PID->Integral > Integral_max)
	   PID->Integral = Integral_max;
	if(PID->Integral < -Integral_max)
	   PID->Integral = -Integral_max;
	
	if(Rc.THROTTLE <= 1300)//���Ž�Сʱ��������
	{
		PID->Integral=0;
	}
	PID->Output = PID->kp * PID->Error + PID->ki *PID->Integral +PID->kd * PID->Deriv;

}

/**  
  *  ���ܣ��������������
  *  ��ڲ�����
  *  ����ֵ��
  */
static uint16_t MOT_Compute(uint16_t throttle,int p,int r,int y)
{
	return (throttle + p*(int)(gyro_pitch.Output-1500) + r*(int)(gyro_roll.Output-1500) + y*(int)(gyro_yaw.Output-1500));
}

const float angle_max = 10;
const float angle_integral_max = 1000;
/**  
  *  ���ܣ�PID�⻷������
  *  ��ڲ�����
  *  ����ֵ��acc_pitch.Output,acc_roll.Output,acc_yaw.Output
  */
void Control_Angle(void)
{
	PID_Control(&acc_pitch, out_angle.pitch, ((Rc.PITCH-1500)/13.0f), angle_max, angle_integral_max);
	PID_Control(&acc_roll,  out_angle.roll,  ((Rc.ROLL-1500)/13.0f),  angle_max, angle_integral_max);
	PID_Control(&acc_yaw,   out_angle.yaw,   ((Rc.YAW-1500)/13.0f),   angle_max, angle_integral_max);
}


const float gyro_max = 50;
const float gyro_integral_max = 5000;
/**  
  *  ���ܣ�PID�ڻ�������
  *  ��ڲ�����Rc_LOCK
  *  ����ֵ��
  */
void Control_Gyro(uint8_t Lock)
{
	PID_Control(&gyro_pitch, acc_pitch.Output, (SI_gyro.x*Radian_to_Angle), gyro_max, gyro_integral_max);
	PID_Control(&gyro_roll,	 acc_roll.Output,  (SI_gyro.y*Radian_to_Angle), gyro_max, gyro_integral_max);
	PID_Control(&gyro_yaw,	 acc_yaw.Output,   (SI_gyro.z*Radian_to_Angle), gyro_max, gyro_integral_max);
	
	if(Lock==0)
	{
		Rc.mot1 = MOT_Compute(Rc.THROTTLE,1,1,-1);
		Rc.mot2 = MOT_Compute(Rc.THROTTLE,1,-1,1);
		Rc.mot3 = MOT_Compute(Rc.THROTTLE,-1,-1,-1);
		Rc.mot4 = MOT_Compute(Rc.THROTTLE,-1,1,1);
				
	}
	else
	{
		Rc.mot1=0;
		Rc.mot2=0;
		Rc.mot3=0;
		Rc.mot4=0;
	}
	MOT_Control(Rc.mot1,Rc.mot2,Rc.mot3,Rc.mot4);
}


