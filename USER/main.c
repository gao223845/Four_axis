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

#include "hcsr04.h"

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
	hcsr04_Init();
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
		}
		
		/*500Hz����*/
		if(Count_2ms>=2)
		{
			Count_2ms=0;			
			Control_Gyro(Rc_LOCK);//�ڻ�����
		}
		
		/*250Hz����*/
		if(Count_4ms>=4)
		{
			Count_4ms=0;			
			Control_Angle();//�⻷����
		}
		
		/*20Hz����*/	
		if(Count_50ms>=50)
		{	
			Count_50ms=0;					
			PPM_DataArrange(PPM_Databuf);//ң��������PPM�źţ�PPM���������洢�� Rc�ṹ�� ��
			Lock_Rep_Ctrl();//�ж�Rc_LOCK��report
			if(0 == report)
				Report_FlyCtrl();//����λ���㱨			
		}
		
		/*5Hz����*/
		if(Count_LED>=200)
		{
			Count_LED=0;
			Get_Distance();//���ģ��						
			LED1=!LED1;//�̵���˸��������������			
		}		
	}	 
}

