/*****************************************************
* 						ͨ�������жϷ�ʽ���Ƶ��ת��
�����ȼ� KEY0��KEY1���ڲ�ͬ���������ȼ�KEY0>KEY1������>���٣���
*������  CCR 200 10%
*������	 CCR 100 5%	
*	KEY0	��PE4�� ����		[0,20]��
*	KEY1	��PE3�� ����		[0,20]��
*	KEY_UP	��PA0�� ɲ�� 		CCR=5%
*�ֽ׶ο�����ת�ٽ�������ȼ�,���ڰ��������ӣ����ȱ�������Ϊ 100,105,110,115,120,125,130,135,140,145,150,155,160,165,170,175,180
*����KEY_UP�����������ʼ������ʼCCR=200��10%����KEY_UPΪCCR=100��5%��������������ɲ��ʹ��
*
*
2021��1��31��10:13:03
*
**************************************************************/

#include "bsp_motor_tim4.h"

/**  TIM4��ʱ����������
  *  ���ܣ�
  *  ��ڲ�����arr��psc
  *  ����ֵ����
  */
void PWM_TIM_GPIO_Config()
{
	GPIO_InitTypeDef			GPIO_InitStructure;		
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	
	MOT_GPIO_APBxClock_FUN(MOT_GPIO_CLK,ENABLE);
	MOT_TIM_APBxClock_FUN(MOT_TIM_CLK,ENABLE);
		
	//PB6,7,8,9 ����
	GPIO_InitStructure.GPIO_Pin = MOT_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB,&GPIO_InitStructure);	

	//TIM5��������	
	//��ʱ��ʱ�� = APB����ʱ��/��Ƶ����
	//							psc
	TIM_TimeBaseStructure.TIM_Period = 20000-1;//��װ��ֵ/������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;//Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(MOT_TIM,&TIM_TimeBaseStructure);	
	
}

/**  PWM ����
  *  ���ܣ�������·PWM���Ƶ��,���ڿ���չΪ�����������
  *  ��ڲ�������
  *  ����ֵ����
  */
void PWM_OC_Config(void)
{
	TIM_OCInitTypeDef			TIM_OCInitStructure;
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 0;
	
	//ǰ���� CCR1 PB6     
	TIM_OC1Init(MOT_TIM, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(MOT_TIM, TIM_OCPreload_Enable); //ʹ��MOT_TIM��CCR1�ϵ�Ԥװ�ؼĴ���
	//ǰ�ҵ�� CCR2 PB7 
	TIM_OC2Init(MOT_TIM, &TIM_OCInitStructure); 
	TIM_OC2PreloadConfig(MOT_TIM, TIM_OCPreload_Enable); //ʹ��MOT_TIM��CCR2�ϵ�Ԥװ�ؼĴ���
	//������ CCR3 PB8
	TIM_OC3Init(MOT_TIM, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(MOT_TIM, TIM_OCPreload_Enable); //ʹ��MOT_TIM��CCR3�ϵ�Ԥװ�ؼĴ���
	//���ҵ�� CCR4 PB9
	TIM_OC4Init(MOT_TIM, &TIM_OCInitStructure); 
	TIM_OC4PreloadConfig(MOT_TIM, TIM_OCPreload_Enable); //ʹ��MOT_TIM��CCR4�ϵ�Ԥװ�ؼĴ���

	TIM_ARRPreloadConfig(MOT_TIM, ENABLE); //ʹ��MOT_TIM��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(MOT_TIM, ENABLE);  //ʹ��MOT_TIM����
	
}

void PWM_Init(void)
{
	PWM_TIM_GPIO_Config();
	PWM_OC_Config();	
}



/**  ������Ƴ���
  *  ���ܣ������ĸ�ͨ����CCR
  *  ��ڲ�����ccr1��ccr2��ccr3��ccr4
  *  ����ֵ����
  *	 ע��ȡֵ[0,0xFFFF]
  */
void MOT_Control(uint16_t ccr1,uint16_t ccr2,uint16_t ccr3,uint16_t ccr4)
{
	if(ccr1<1000)	ccr1=1000;			
	if(ccr1>2000)	ccr1=2000;
	if(ccr2<1000)	ccr2=1000;			
	if(ccr2>2000)	ccr2=2000;
	if(ccr3<1000)	ccr3=1000;			
	if(ccr3>2000)	ccr3=2000;		
	if(ccr4<1000)	ccr4=1000;			
	if(ccr4>2000)	ccr4=2000;
	
	TIM4->CCR1 = ccr1;
	TIM4->CCR2 = ccr2;
	TIM4->CCR3 = ccr3;
	TIM4->CCR4 = ccr4;
}

///**  �����ж�����
//  *  ���ܣ����������������ж�����
//  *  ��ڲ�������
//  *  ����ֵ����
//  */
//static void EXTI_Config(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);//���ټ�PE3
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);//���ټ�PE4
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//ֹͣ��PA0

//	EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line0;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
//	
//}

//static void NVIC_Config(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	//���ټ�PE3
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);  
//	//���ټ�PE4
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);  
//	//ֹͣ��PA0 
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);  
//}

//void KEY_EXTI_Config(void)
//{
//	EXTI_Config();
//	NVIC_Config();
//	EXTI_ClearFlag(EXTI_Line0);
//	EXTI_ClearFlag(EXTI_Line3);
//	EXTI_ClearFlag(EXTI_Line4);
//}


//uint16_t ccr_val = 100;//5%��CCR����ʼ״̬
//int add_val = 0;//ÿ�ΰ���CCR���ӵ�ֵ
//int a =0;
////ʵ������д���CCR=ccr_val+add_val

///**  �����жϷ�����
//  *  ���ܣ����ݲ�ͬ�İ������ò�ͬ��CCR��ֵ
//  *  ��ڲ�������
//  *  ����ֵ����
//  */
////����
//void EXTI3_IRQHandler(void)
//{
//	delay_ms(10);
//	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
//	{
//		if(KEY1 == 1)
//		{
//			add_val += 5;
//			LED1 = !LED1;
//			if(add_val<81)//���������ٵ�80%
//			{	
//				MOT_Control(ccr_val+add_val,ccr_val+add_val,ccr_val+add_val,ccr_val+add_val);	
//				printf("3CCR = %d\r\n",TIM_GetCapture1(TIM4));
//			}
//			else		
//			{
//				BEEP;
//				add_val = 80;
//			}				
//		}
//		EXTI_ClearITPendingBit(EXTI_Line3);
//	}
//}
////����
//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);
//	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
//	if(KEY0 == 1)
//	{
//		add_val -=5;
//		printf("%d\r\n",add_val);
//		LED0 = !LED0;
//		if(add_val > -1)
//		{
//			MOT_Control(ccr_val+add_val,ccr_val+add_val,ccr_val+add_val,ccr_val+add_val);
//			printf("4CCR = %d\r\n",TIM_GetCapture1(TIM4));			
//		}
//		else
//		{	
//			BEEP;
//			add_val = 0;
//			printf("add_val�ѹ���,add_val = %d\r\n",add_val);
//		}
//	}	
//	EXTI_ClearITPendingBit(EXTI_Line4);
//}
////ֹͣ
//void EXTI0_IRQHandler(void)
//{
//	delay_ms(10);
//	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
//	{	
//		if(KEY1 == 1)
//		{
//			BEEP = !BEEP;
//			MOT_Control(ccr_val,ccr_val,ccr_val,ccr_val);
//			printf("0CCR = %d\r\n",TIM_GetCapture1(TIM4));
//		}
//		printf("ɲ���ɹ�\r\n");
//		EXTI_ClearITPendingBit(EXTI_Line0);
//	}
//}
