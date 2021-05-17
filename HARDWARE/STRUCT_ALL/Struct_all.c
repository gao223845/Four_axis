#include "Struct_all.h"

uint8_t Rc_LOCK=1;//�������0������1����
uint8_t report=1;//�����ϴ����ݵ���λ��,0,�رգ�1����
uint16_t MOT_Speed[4]={0};//PWM��

struct _acc  acc;			//ԭʼ����
struct _gyro gyro;

struct _acc  filter_acc;	//�˲�������
struct _gyro filter_gyro;

struct _acc  offset_acc;	//��ƫ����
struct _gyro offset_gyro;

struct _SI_float  SI_acc;	//���ٶ����ݣ�m/s2��
struct _SI_float  SI_gyro;	//���ٶ����ݣ�rad��

struct _Rc Rc;				//ң��ͨ��
struct _out_angle out_angle;//��̬����-�Ƕ�ֵ

/* pid */
struct _pid acc_pitch;
struct _pid acc_roll;
struct _pid acc_yaw;
struct _pid gyro_pitch;
struct _pid gyro_roll;
struct _pid gyro_yaw;

void PID_Reset(void)
{
//	roll.kp  = 4.9f;
//	roll.ki  = 0.02f;
//	roll.kd  = 8.0f;
//	pitch.kp = 4.9f;
//	pitch.ki = 0.02f;
//	pitch.kd = 8.0f;
//	yaw.kp = 4.9f;
//	yaw.ki = 0.02f;
//	yaw.kd = 8.0f;
	
	gyro_roll.kp  =1.0f;
	gyro_roll.ki  =0.0f;
	gyro_roll.kd  =0.0f;
	
	gyro_pitch.kp =1.0f;
	gyro_pitch.ki =0.0f;
	gyro_pitch.kd =0.0f;
	
	gyro_yaw.kp   =1.0f;
	gyro_yaw.ki   =0.0f;
	gyro_yaw.kd   =0.0f;
}





