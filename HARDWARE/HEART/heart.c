#include "heart.h"

/**  
  *  ���ܣ����������������������TIM7�趨��ͬƵ�ʵķɿ�����
  *  ��ڲ�������
  *  ����ֵ����
  *  �Ѿ��趨���Ƶ��Ϊ1KHz���������������Ϊ1000KHz��500KHz��250KHz
  */
void HEART_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);   //ʹ�ܶ�ӦRCCʱ��
	//���ö�ʱ�������ṹ��
	TIM_DeInit(TIM7);
	TIM_TimeBaseStructure.TIM_Period = (1000-1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ,������1000Ϊ1ms
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ,1M�ļ���Ƶ�� 1US����
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ         
								
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;             //ѡ�񴮿�1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռʽ�ж����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //��Ӧʽ�ж����ȼ�����Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        //ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
    
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);   //��������жϣ����һ���ж����������ж�
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);    //�򿪶�ʱ�������ж�
    TIM_Cmd(TIM7,ENABLE);     
}



/**  
  *  ���ܣ� TIM7�жϷ�����������ִ�в�ͬƵ�ʵ�����
  *  ��ڲ�������
  *  ����ֵ����
  */
extern uint8_t Count_1ms=0;//1000Hz��ʱ
extern uint8_t Count_2ms=0;//500Hz��ʱ
extern uint8_t Count_4ms=0;//250Hz��ʱ
extern uint8_t Count_25ms=0;//20Hz��ʱ
extern uint16_t Count_LED=0;//��������ʾ��

void TIM7_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		Count_1ms++;
		Count_2ms++;
		Count_4ms++;
		Count_25ms++;
		Count_LED++;		
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  //���TIM7�����жϱ�־ 
}










