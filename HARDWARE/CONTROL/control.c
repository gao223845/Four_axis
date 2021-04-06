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


//#define angle_max 	 		10.0f	
//#define angle_integral_max 	1000.0f	
///**  
//  *  ���ܣ�PID�⻷������
//  *  ��ڲ�����
//  *  ����ֵ��
//  */
//void Control_Angle(struct _out_angle *angle,struct _Rc *rc)
//{
//	static struct _out_angle control_angle;
//	static struct _out_angle last_angle;

////			����Ϊ�ǶȻ�
//	if(rc->ROLL>1490 && rc->ROLL<1510)	
//		rc->ROLL=1500;
//	if(rc->PITCH>1490 && rc->PITCH<1510)	
//		rc->PITCH=1500;

////////////////////////////////////////////////////////////////////
//	control_angle.roll  = angle->roll  - (rc->ROLL  -1500)/13.0f ;
//	control_angle.pitch = angle->pitch - (rc->PITCH -1500)/13.0f ;
////////////////////////////////////////////////////////////////////
//	if(control_angle.roll >  angle_max)	//ROLL
//		roll.integral +=  angle_max;
//	if(control_angle.roll < -angle_max)
//		roll.integral += -angle_max;
//	else
//		roll.integral += control_angle.roll;
//	
//	if(roll.integral >  angle_integral_max)
//	   roll.integral =  angle_integral_max;
//	if(roll.integral < -angle_integral_max)
//	   roll.integral = -angle_integral_max;
////////////////////////////////////////////////////////////////////
//	if(control_angle.pitch >  angle_max)//PITCH
//	   pitch.integral +=  angle_max;
//	if(control_angle.pitch < -angle_max)
//	   pitch.integral += -angle_max;
//	else
//		pitch.integral += control_angle.pitch;

//	if(pitch.integral >  angle_integral_max)
//	   pitch.integral =  angle_integral_max;
//	if(pitch.integral < -angle_integral_max)
//	   pitch.integral = -angle_integral_max;
////////////////////////////////////////////////////////////////////
//	if(control_angle.yaw >  angle_max)//YAW
//	   yaw.integral +=  angle_max;
//	if(control_angle.yaw < -angle_max)
//	   yaw.integral += -angle_max;
//	else
//		yaw.integral += control_angle.yaw;

//	if(yaw.integral >  angle_integral_max)
//	   yaw.integral =  angle_integral_max;
//	if(yaw.integral < -angle_integral_max)
//	   yaw.integral = -angle_integral_max;
////////////////////////////////////////////////////////////////////
//	if(rc->THROTTLE<1200)//���Ž�Сʱ����������
//	{
//		roll.integral  = 0;
//		pitch.integral = 0;
//		yaw.integral=0;
//	}
////////////////////////////////////////////////////////////////////
//	roll.output  = roll.kp *control_angle.roll  + roll.ki *roll.integral  + roll.kd *(control_angle.roll -last_angle.roll );
//	pitch.output = pitch.kp*control_angle.pitch + pitch.ki*pitch.integral + pitch.kd*(control_angle.pitch-last_angle.pitch);
//	yaw.output  = yaw.kp *control_angle.roll  + yaw.ki *yaw.integral  + yaw.kd *(control_angle.yaw -last_angle.yaw );
////////////////////////////////////////////////////////////////////
//	last_angle.roll =control_angle.roll;
//	last_angle.pitch=control_angle.pitch;
//	last_angle.yaw=control_angle.yaw;
//}

#define gyro_max 	 		50.0f	
#define gyro_integral_max 	5000.0f
/**  
  *  ���ܣ�PID�ڻ�������
  *  ��ڲ�����
  *  ����ֵ��
  */
void Control_Gyro(struct _SI_float *gyro,struct _Rc *rc,uint8_t Lock)
{
	static struct _out_angle control_gyro;
	static struct _out_angle last_gyro;
	
//			����Ϊ���ٶȻ�

//////////////////////////////////////////////////////////////////
	control_gyro.roll  = Rc.ROLL - gyro->y*Radian_to_Angle;
	control_gyro.pitch = Rc.PITCH - gyro->x*Radian_to_Angle;
	control_gyro.yaw   = Rc.YAW - gyro->z*Radian_to_Angle;
//	printf("control:\r\n");
//	printf("%f,%f,%f\r\n",control_gyro.pitch,control_gyro.roll,control_gyro.yaw);

//////////////////////////////////////////////////////////////////
	if(control_gyro.roll >  gyro_max)	//GYRO_ROLL
		gyro_roll.integral +=  gyro_max;
	if(control_gyro.roll < -gyro_max)
		gyro_roll.integral += -gyro_max;
	else
		gyro_roll.integral += control_gyro.roll;
	
	if(gyro_roll.integral >  gyro_integral_max)
	   gyro_roll.integral =  gyro_integral_max;
	if(gyro_roll.integral < -gyro_integral_max)
	   gyro_roll.integral = -gyro_integral_max;
//////////////////////////////////////////////////////////////////
	if(control_gyro.pitch >  gyro_max)//GYRO_PITCH
		gyro_pitch.integral +=  gyro_max;
	if(control_gyro.pitch < -gyro_max)
		gyro_pitch.integral += -gyro_max;
	else
		gyro_pitch.integral += control_gyro.pitch;
	
	if(gyro_pitch.integral >  gyro_integral_max)
	   gyro_pitch.integral =  gyro_integral_max;
	if(gyro_pitch.integral < -gyro_integral_max)
	   gyro_pitch.integral = -gyro_integral_max;
//////////////////////////////////////////////////////////////////
//	if(control_gyro.yaw >  gyro_max)//GYRO_YAW
//		gyro_yaw.integral +=  gyro_max;
//	if(control_gyro.yaw < -gyro_max)
//		gyro_yaw.integral += -gyro_max;
//	else
		gyro_yaw.integral += control_gyro.yaw;
	
	if(gyro_yaw.integral >  gyro_integral_max)
	   gyro_yaw.integral =  gyro_integral_max;
	if(gyro_yaw.integral < -gyro_integral_max)
	   gyro_yaw.integral = -gyro_integral_max;
//////////////////////////////////////////////////////////////////
	if(rc->THROTTLE<1200)//���Ž�Сʱ����������
	{
		gyro_yaw.integral  = 0;
	}
//////////////////////////////////////////////////////////////////
	gyro_roll.output  = gyro_roll.kp *control_gyro.roll  + gyro_roll.ki *gyro_roll.integral  + gyro_roll.kd *(control_gyro.roll -last_gyro.roll );
	gyro_pitch.output = gyro_pitch.kp*control_gyro.pitch + gyro_pitch.ki*gyro_pitch.integral + gyro_pitch.kd*(control_gyro.pitch-last_gyro.pitch);
	gyro_yaw.output   = gyro_yaw.kp  *control_gyro.yaw   + gyro_yaw.ki  *gyro_yaw.integral   + gyro_yaw.kd  *(control_gyro.yaw  -last_gyro.yaw  );

//////////////////////////////////////////////////////////////////
	last_gyro.roll =control_gyro.roll;
	last_gyro.pitch=control_gyro.pitch;
	last_gyro.yaw  =control_gyro.yaw;
//////////////////////////////////////////////////////////////////
	if(Lock==0)
	{

				
		Rc.mot1 = rc->THROTTLE + (int)(gyro_pitch.output-1500) + (int)(gyro_roll.output-1500) - (int)(gyro_yaw.output-1500);
		Rc.mot2 = rc->THROTTLE + (int)(gyro_pitch.output-1500) - (int)(gyro_roll.output-1500) + (int)(gyro_yaw.output-1500);
		Rc.mot3 = rc->THROTTLE - (int)(gyro_pitch.output-1500) - (int)(gyro_roll.output-1500) - (int)(gyro_yaw.output-1500);
		Rc.mot4 = rc->THROTTLE - (int)(gyro_pitch.output-1500) + (int)(gyro_roll.output-1500) + (int)(gyro_yaw.output-1500);
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


/**  
  *  ���ܣ�����������ݿ��Ƶ��
  *  ��ڲ�����
  *  ����ֵ��
  */
void DataOutput_ToMOT(uint8_t rc_lock)
{		

	if(rc_lock == 0)//�ѽ���
	{
		Rc.mot1 = Rc.THROTTLE+((int)Rc.ROLL-1500)-((int)Rc.PITCH-1500)-((int)Rc.YAW-1500);
		Rc.mot2 = Rc.THROTTLE-((int)Rc.ROLL-1500)-((int)Rc.PITCH-1500)+((int)Rc.YAW-1500);
		Rc.mot3 = Rc.THROTTLE+((int)Rc.ROLL-1500)+((int)Rc.PITCH-1500)+((int)Rc.YAW-1500);
		Rc.mot4 = Rc.THROTTLE-((int)Rc.ROLL-1500)+((int)Rc.PITCH-1500)-((int)Rc.YAW-1500);
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

