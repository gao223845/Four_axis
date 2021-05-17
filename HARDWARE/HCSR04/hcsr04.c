#include "hcsr04.h"
#include "delay.h"
#include "beep.h"

/**  
  *  ���ܣ�������ʹ�õ����źͶ�ʱ����ʼ�� Trig-��PA5��Echo-��PA6
  *  ��ڲ�������
  *  ����ֵ����
  */
void hcsr04_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;     //�������ڶ�ʱ�����õĽṹ��
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
    RCC_APB2PeriphClockCmd(HCSR04_CLK, ENABLE);
     
    GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;       //���͵�ƽ����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
    GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
     
    GPIO_InitStructure.GPIO_Pin = HCSR04_ECHO;     //���ص�ƽ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
	GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
				 
	//��ʱ����ʼ�� ʹ�û�����ʱ��TIM3
	//���ö�ʱ�������ṹ��
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ,1M�ļ���Ƶ�� 1��=1us
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ  

	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//����Ƶ
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//��IC1ӳ�䵽IT1
	TIM_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
								
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;             //ѡ��TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռʽ�ж����ȼ�����Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //��Ӧʽ�ж����ȼ�����Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
		
	TIM_ClearFlag(TIM3, TIM_FLAG_Update|TIM_FLAG_CC1);   //��������жϣ����һ���ж����������ж�
	TIM_ITConfig(TIM3, TIM_IT_CC1,ENABLE);    //�򿪶�ʱ�������ж�
    TIM_Cmd(TIM3,ENABLE);       
}

/**  
  *  ���ܣ� ��¼����ߵ�ƽʱ��
  *  ��ڲ�����
  *  ����ֵ��
  */
u8  TIM3CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM3CH1_CAPTURE_VAL;	//���벶��ֵ
void TIM3_IRQHandler(void)
{
	if(0 == (TIM3CH1_CAPTURE_STA & 0x80))//δ����ɹ�
	{
		if(TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET) //�ٴμ���Ƿ����ж�
		{	
			if(TIM3CH1_CAPTURE_STA & 0x40)//�Ѿ����񵽸ߵ�ƽ
			{
				if(0x3F == (TIM3CH1_CAPTURE_STA & 0x3F))//�����ʱ���ֵ�������ٽ��м������������ݸ������ֵ
				{	
					TIM3CH1_CAPTURE_STA |= 0x80;//������״̬λ��bit8����Ϊ1����ǳɹ�����
					TIM3CH1_CAPTURE_VAL = 0xFFFF;
				}
				else 
				{
					TIM3CH1_CAPTURE_STA++;
				}
			}
			
		}
		if(TIM_GetITStatus(TIM3,TIM_IT_CC1) != RESET)
		{
			if(TIM3CH1_CAPTURE_STA & 0x40)//�ӵ�һ���ߵ�ƽ��ʼ�����񵽵�һ���½���
			{
				TIM3CH1_CAPTURE_STA |= 0x80;//��λbit8�����Ϊһ�β������
				TIM3CH1_CAPTURE_VAL = TIM_GetCapture1(TIM3);
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising);	
			}				
			else//��һ���ߵ�ƽ�յ�
			{	
				TIM3CH1_CAPTURE_STA = 0;
				TIM3CH1_CAPTURE_VAL = 0;
				TIM_SetCounter(TIM3,0);	//ȫ�����
				TIM3CH1_CAPTURE_STA |= 0x40;//��ǲ���������
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		
			}
		}

	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_CC1 | TIM_IT_Update);
}
/**  
  *  ���ܣ� ���Ϳ�ʼ�ź�
  *  ��ڲ�����
  *  ����ֵ��
  */
static void Send_Trig(void)
{
	PAout(5)=1;
	delay_us(15);
	PAout(5)=0;
	delay_ms(70);
}

/**  
  *  ���ܣ� �õ����� t=Distance*0.000 001(s),s=340*t/2(m)
  *  ��ڲ�����
  *  ����ֵ��
  */
void Get_Distance(void)
{
	float Distance;
	Send_Trig();
	if(TIM3CH1_CAPTURE_STA&0x80)
	{
		Distance=TIM3CH1_CAPTURE_STA&0X3F;
		Distance*=65536;         //���ʱ���ܺ�
		Distance += TIM3CH1_CAPTURE_VAL;  //�õ��ܵĸߵ�ƽʱ��   ECHO����ĸߵ�ƽ��ʱ����ǳ������ӷ��䵽���ص�ʱ��	
		if(Distance<59)//�ɹ����䣬ֹͣ Distance=58.823529 �൱�� s<10cm
		{	
			TIM_Cmd(TIM6,DISABLE);
			BEEP=0;
			TIM_SetCounter(TIM6, 0);			
		}
		else if(Distance<588)//����С��100cm��
		{			
			TIM_SetCounter(TIM6, 0);
			TIM6->ARR=2000-1;
			TIM_Cmd(TIM6,ENABLE);
		}
		else
		{
			TIM_Cmd(TIM6,DISABLE);
			BEEP=0;
			TIM_SetCounter(TIM6, 0);	
		}
	}		
		TIM3CH1_CAPTURE_STA=0;	
}











