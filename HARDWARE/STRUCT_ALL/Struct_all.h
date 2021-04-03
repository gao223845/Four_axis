#ifndef __STRUCT_ALL_H
#define __STRUCT_ALL_H

#include "sys.h"

/******************************************************************************
							ȫ�ֱ�������
*******************************************************************************/ 
extern uint8_t Rc_LOCK;//�������0������1����
extern uint8_t report;//�����ϴ����ݵ���λ��,0,�رգ�1����

#define Radian_to_Angle	   57.2957795f
#define RawData_to_Angle	0.0610351f	//���²�����Ӧ2000��ÿ��
#define RawData_to_Radian	0.0010653f

/******************************************************************************
							�ṹ������
*******************************************************************************/ 
/* MPU6050--���ٶȼƽṹ�� */
struct _acc
{
	short x;
	short y;
	short z;
};
extern struct _acc acc;
extern struct _acc filter_acc;
extern struct _acc offset_acc;

/* MPU6050--�����ǽṹ�� */
struct _gyro
{
	short x;
	short y;
	short z;
};
extern struct _gyro gyro;
extern struct _gyro filter_gyro;
extern struct _gyro offset_gyro;

/* float�ṹ�� */
struct _SI_float
{
	float x;
	float y;
	float z;
};
extern struct _SI_float SI_acc;	
extern struct _SI_float SI_gyro;

/* ��̬����--�Ƕ�ֵ */
struct _out_angle
{
	float yaw;
	float roll;
	float pitch;
};
extern struct _out_angle out_angle;

/* ң������ */
struct _Rc
{	
	uint16_t ROLL;		//���������
	uint16_t PITCH;		//������ǰ��	
	uint16_t THROTTLE;	//������������
	uint16_t YAW;		//ƫ��������
	
	uint16_t AUX1;		//����ͨ��1
	uint16_t AUX2;		//����ͨ��2
	uint16_t AUX3;		//����ͨ��3
	uint16_t AUX4;		//����ͨ��4
	
	uint16_t mot1;
	uint16_t mot2;
	uint16_t mot3;
	uint16_t mot4;
};
extern struct _Rc Rc;

/* pid���� */
struct _pid
{
	float kp;
	float ki;
	float kd;
	float integral;
	
	float output;
};
extern struct _pid pitch;
extern struct _pid roll;
extern struct _pid yaw;
extern struct _pid gyro_pitch;
extern struct _pid gyro_roll;
extern struct _pid gyro_yaw;


void PID_Reset(void);


#endif /*__STRUCT_ALL_H*/


