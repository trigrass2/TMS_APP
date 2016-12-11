#include "MTD_CPU_CapturePwm.h"
#include "Uart.h"

//��ʱ��5ͨ��1���벶��pwm2����
//arr���Զ���װֵ(TIM2,TIM5��32λ��!!)
//psc��ʱ��Ԥ��Ƶ��
void MTDCPU_PWM2IN_TIM5_CH1_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  		//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��GPIOAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 					//GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//����  					
	GPIO_Init(GPIOA,&GPIO_InitStructure); 							//��ʼ��PA0

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0����λ��ʱ��5��ͨ��1
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  								//��ʱ����Ƶ   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_Period=arr;   									//�Զ���װ��ֵ  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;							 		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;			//�����ز���
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					  //���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;													//IC1F=0000 ���������˲��� ���˲�

	TIM_PWMIConfig(TIM5, &TIM5_ICInitStructure);     //PWM��������           

  /* Select the TIM5 Input Trigger: TI1FP1 */
	TIM_SelectInputTrigger(TIM5, TIM_TS_TI1FP1);     //ѡ����Ч�����     ѡ��IC1Ϊʼ�մ���Դ    

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(TIM5, TIM_SlaveMode_Reset);  //����Ϊ���Ӹ�λģʽ
  TIM_SelectMasterSlaveMode(TIM5, TIM_MasterSlaveMode_Enable);                                       	
		
		
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);		//��������ж� ,����CC1IE�����ж�	
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update|TIM_IT_CC1); //����жϱ�־λ
  TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
 
	/*�����ж����ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;				//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
}

void 	MTDCPU_PWM1IN_TIM2_CH2_Init(u32 arr,u16 psc)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  		//TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��GPIOAʱ��	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 					//GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 				//����  				
	GPIO_Init(GPIOA,&GPIO_InitStructure); 							//��ʼ��PA1

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2); 		//PA1����λ��ʱ��2��ͨ��2
  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  								//��ʱ����Ƶ   //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  
	TIM_TimeBaseStructure.TIM_Period=arr;   									//�Զ���װ��ֵ  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	//��ʼ��TIM2���벶�����
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2;							 		//CC1S=01 	ѡ������� IC1ӳ�䵽TI2��
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;			//�����ز���
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;  //ӳ�䵽TI2��
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;					  //���������Ƶ,����Ƶ 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;													//IC1F=0000 ���������˲��� ���˲�

	TIM_PWMIConfig(TIM2, &TIM2_ICInitStructure);     //PWM��������           

  /* Select the TIM2 Input Trigger: TI2FP2 */
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);     //ѡ����Ч�����     ѡ��IC2Ϊʼ�մ���Դ    

	/* Select the slave Mode: Reset Mode */
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);  //����Ϊ���Ӹ�λģʽ
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);                                       	
		
		
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);	 	 //��������ж� ,����CC2IE�����ж�	
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update|TIM_IT_CC2); //����жϱ�־λ
	
  TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
 
	/*�����ж����ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;	//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;				//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}


/*
���������������ػ��½�,�����ڰ���TIMʱ�����ڵĸ���(����Ĵ����в����ΪTIM�ļ�������n)

�ߵ�ƽ�����ڻ�͵�ƽ������,�����źŵļ�������m

    frequency=f��TIMʱ��Ƶ�ʣ�/n��
    duty cycle=���ߵ�ƽ��������/n����

*/
__IO uint16_t TIM5IC1Value = 0;		////ѡ��IC1Ϊʼ�մ���Դ����ic1Ϊ��õ�Ƶ�ʡ�
__IO uint16_t PWM2DutyCycle = 0;
__IO uint32_t PWM2Frequency = 0;

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM5CH1_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
void TIM5_IRQHandler(void)
{ 		
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
			OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif
	
//	RCC_ClocksTypeDef RCC_Clocks;
//	RCC_GetClocksFreq(&RCC_Clocks);  
		
	if(TIM_GetITStatus(TIM5,TIM_IT_CC1) !=RESET )//����1���������¼�
	{
//		/* Clear TIM5 Capture compare interrupt pending bit */
//		TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);		//����ж�
		
		/* Get the Input Capture value */
		TIM5IC1Value = TIM_GetCapture1(TIM5);		//�õ�n��ֵ	 //��ȡIC1����Ĵ�����ֵ����ΪPWM���ڵļ���ֵ		
		
		if(TIM5IC1Value !=0)
		{
			/* Duty cycle computation */
			PWM2DutyCycle = (TIM_GetCapture2(TIM5) * 100) / TIM5IC1Value;		//	��ȡIC2����Ĵ�����ֵ��������ռ�ձ�
			
			/* Frequency computation 
		 TIM5 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

		//	PWM2Frequency =  /*(RCC_Clocks.HCLK_Frequency)/2 /*/TIM5IC1Value;  //ʱ�ӵ�Ƶ�ʿ�arr,prc ,��Ƶ��	PWM2Frequency = TIMFre / TIM5IC1Value;   ����û��д��ʱ��Ƶ�ʣ���Ϊ�ڳ�ʼ����ʱ��Ż��Ƶ
			PWM2Frequency =  TIM5IC1Value;
		}
		
		else
		{
			PWM2DutyCycle = 0;
			PWM2Frequency = 0;
		}
	}
	
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
		
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    	//�˳��ж�
	#endif
}



__IO uint16_t TIM2IC1Value = 0;		////ѡ��IC1Ϊʼ�մ���Դ����ic1Ϊ��õ�Ƶ�ʡ�
__IO uint16_t PWM1DutyCycle = 0;
__IO uint32_t PWM1Frequency = 0;

//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM2CH2_CAPTURE_STA=0;	//���벶��״̬		    				
u32	TIM2CH2_CAPTURE_VAL;	//���벶��ֵ(TIM2/TIM5��32λ)
//��ʱ��5�жϷ������	 
void TIM2_IRQHandler(void)
{ 	
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
			OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif
	
//	RCC_ClocksTypeDef RCC_Clocks;
//	RCC_GetClocksFreq(&RCC_Clocks); 
	if(TIM_GetITStatus(TIM2,TIM_IT_CC2) !=RESET )//����1���������¼�
	{
//		/* Clear TIM2 Capture compare interrupt pending bit */
//		TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);		//����ж�
		
		/* Get the Input Capture value */
		TIM2IC1Value = TIM_GetCapture2(TIM2);		//�õ�n��ֵ	 //��ȡIC2����Ĵ�����ֵ����ΪPWM���ڵļ���ֵ		
		
		if(TIM2IC1Value !=0)
		{
			/* Duty cycle computation */
			PWM1DutyCycle = (TIM_GetCapture1(TIM2) * 100) / TIM2IC1Value;		//	��ȡIC2����Ĵ�����ֵ��������ռ�ձ�
			
			/* Frequency computation 
		 TIM2 counter clock = (RCC_Clocks.HCLK_Frequency)/2 */

			//PWM1Frequency = /*(RCC_Clocks.HCLK_Frequency)/2 /*/ TIM2IC1Value;  //ʱ�ӵ�Ƶ�ʿ�arr,prc ,��Ƶ��	PWM2Frequency = TIMFre / TIM5IC1Value;   ����û��д��ʱ��Ƶ�ʣ���Ϊ�ڳ�ʼ����ʱ��Ż��Ƶ
			PWM1Frequency =  TIM2IC1Value; 
		}
		
		else
		{
			PWM1DutyCycle = 0;
			PWM1Frequency = 0;
		}
	}
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update); //����жϱ�־λ
	
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    	//�˳��ж�
	#endif	
}



void PWM_Test_GPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}
