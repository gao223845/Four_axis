#include "Rc.h"
#include "led.h"
#include "usart.h"
#include "Struct_all.h"

/***********************��˹ I6X ң��������֡*******************
*
*	DATA 0		DATA 1		DATA 2		DATA 3		DATA 4		DATA 5		DATA 6		DATA 7
*	ǰ��		��ͷ����	����ͨ��2	����ͨ��4	����		����		����ͨ��1	����ͨ��3
*	ch2			ch4			ch6			ch8			ch1			ch3			ch5			ch7
*
****************/

/**  
  *  ���ܣ�PPM�����ʼ�� TIM2_CH2 PA1
  *  ��ڲ�����
  *  ����ֵ��
  */

void PPM_Init(void)//ʹ��PA1��Ϊ�����ж�Դ
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitType;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ����TIM2CLK Ϊ 72MHZ */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	TIM_TimeBaseStructure.TIM_Period=20000;//20ms�ж�һ��
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	/* ʱ��Ԥ��Ƶ��Ϊ72 */
	TIM_TimeBaseStructure.TIM_Prescaler=(72-1);//1us
	/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ICInitType.TIM_Channel=TIM_Channel_2;//PA1 ͨ��2
	TIM_ICInitType.TIM_ICFilter=0x0;
	TIM_ICInitType.TIM_ICPolarity=TIM_ICPolarity_Rising;//������
	TIM_ICInitType.TIM_ICPrescaler=TIM_ICPSC_DIV1;
	TIM_ICInitType.TIM_ICSelection=TIM_ICSelection_DirectTI;//����Դѡ��
	TIM_ICInit(TIM2,&TIM_ICInitType);        
	                                                  
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;    
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);    

	TIM_ClearFlag(TIM2, TIM_FLAG_Update|TIM_IT_CC2);    
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);
	TIM_Cmd(TIM2, ENABLE);                                                                        
}


uint16_t PPM_Sample_Cnt=0;
uint32_t PPM_Time=0;
uint16_t PPM_Okay=0;
uint16_t PPM_Databuf[8]={0};
uint8_t TIM2_CH2_CAPTURE_STA=0;
/**  
  *  ���ܣ�TIM2_ch1�жϷ�����������PPM�źŴ�����PPM_Databuf��
  *  ��ڲ�����
  *  ����ֵ��
  */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2)==!RESET)//�����ж�
	{
		if(TIM2_CH2_CAPTURE_STA&0x01)//���������Ļ�˵���ϴβ����˸ߵ�ƽ����ô��β����һ���ǵ͵�ƽ
		{
			PPM_Time=TIM_GetCapture2(TIM2);
			if(PPM_Time>0)
				PPM_Time++;			
			if(PPM_Okay==1)
			{
				PPM_Databuf[PPM_Sample_Cnt]=PPM_Time;
				PPM_Sample_Cnt++;
				if(PPM_Sample_Cnt>8)
					PPM_Okay=0;
			}
			if(PPM_Time>7000)//ʶ��֡β
			{
				PPM_Okay=1;
				PPM_Sample_Cnt=0;
			}    

				TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);
				TIM2_CH2_CAPTURE_STA=0;//�����־λ׼����ʼ��һ�������غ��½��ؼ��
		}
			else
			{
				TIM_SetCounter(TIM2,0);//����Ϊ����            
				TIM2_CH2_CAPTURE_STA|=0x01;//�ߵ�ƽָʾ����ֵ
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling); //�����������غ��Ϊ�����½���
			}        
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ��һ����Ҫ����Ҫ��Ȼ�´ν������ж�    
}


/**  
  *  ���ܣ�����RC����PPM�ź�����д���ӦRc�ṹ��
  *  ��ڲ�����
  *  ����ֵ��
  */
void PPM_DataArrange(uint16_t *data)
{
	Rc.ROLL = data[4];
	Rc.PITCH = data[0];
	Rc.THROTTLE = data[5];
	Rc.YAW = data[1];
	Rc.AUX1 = data[6];
	Rc.AUX2 = data[2];
	Rc.AUX3 = data[7];
	Rc.AUX4 = data[3];
}


