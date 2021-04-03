#include "sys.h"
#include "heart.h"
#include "Struct_all.h"

#include "delay.h"
#include "led.h"
#include "usart.h"
#include "beep.h"

#include "bsp_motor_tim4.h"
#include "rc.h"
#include "control.h"

#include "iic_mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "protocol.h"


/**  
  *  ���ܣ�
  *  ��ڲ�����
  *  ����ֵ��
  */

int main(void)
{	
	delay_init();	    	 	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(5000000);	 
 	LED_Init();
	BEEP_Init();  
	PPM_Init();
	PWM_Init();
	MPU6050_Init();
	PID_Reset();	
	HEART_Init();

 	while(1)
	{
		/*1000Hz����*/
		if(Count_1ms>=1)
		{
			Count_1ms=0;			
			MPU_GetData();
			if(report==1)//�������ݸ���λ��
			{
				usart1_report_imu(acc.x,acc.y,acc.z,gyro.x,gyro.y,gyro.z,(int)(out_angle.roll*100),(int)(out_angle.pitch*100),(int)(out_angle.yaw*10));
			}			
		}
		
		/*500Hz����*/
		if(Count_2ms>=2)
		{
			Count_2ms=0;			
			Control_Gyro(&SI_gyro,&Rc,Rc_LOCK);//�ڻ�����
		}
		
		/*250Hz����*/
		if(Count_4ms>=4)
		{
			Count_4ms=0;			
//			Control_Angle(&out_angle,&Rc);//�⻷����
		}
		
		/*20Hzң��������PPM�ź�*/	
		if(Count_25ms>=25)
		{	
			Count_25ms=0;					
			PPM_DataArrange(PPM_Databuf);//PPM�������� �洢�� Rc�ṹ�� ��
//			DataOutput_ToMOT(Rc_LOCK);
			if(report==1)//�������ݸ���λ��
			{				
//				usart1_report_pid(u16 rol_p,u16 rol_i,u16 rol_d,u16 pit_p,u16 pit_i,u16 pit_d,u16 yaw_p,u16 yaw_i,u16 yaw_d);
//				usart1_report_rc(Rc.THROTTLE,Rc.YAW,Rc.ROLL,Rc.PITCH,Rc.AUX1,Rc.AUX2,Rc.AUX3,Rc.AUX4,0,(int)((Rc.mot1-1000)/10),(int)((Rc.mot2-1000)/10),(int)((Rc.mot3-1000)/10),(int)((Rc.mot4-1000)/10),0);
			}
			
		}
		
		/*4Hz����*/
		if(Count_LED>=250)
		{
			Count_LED=0;
//			Get_Distance();//���ģ��						
			LED1=!LED1;//�̵���˸��������������
			
		}		
	}	 
}

