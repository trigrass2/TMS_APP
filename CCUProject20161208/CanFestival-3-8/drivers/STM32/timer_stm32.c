/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdbool.h>
#include "canfestival.h"
#include "timer.h"
#include "Uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
// Store the last timer value to calculate the elapsed time
static TIMEVAL last_time_set = TIMEVAL_MAX;
//extern __IO uint16_t CCR1_Val ;
__IO uint16_t CCR1_Val = 20000;//49152;	//whh ��CCR1_Val=20000,���������Ϊ1M/20000 =50Hz��Ϊ20ms����һ�Ρ�
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  setTimer
  * @param  value:Set time value 0x0000-0xffff
  * @retval NONE
  */
void setTimer(TIMEVAL value)
{
//	TIM_SetAutoreload(TIM4, value);
	
	/*W H H,����Ƚ�ģʽ*/
		uint16_t capture = 0;
		capture = TIM_GetCapture1(TIM4);
		TIM_SetCompare1(TIM4, capture + value);
}
/**
  * @brief  getElapsedTime
  * @param  NONE
	* @retval TIMEVAL:Return current timer value
  */
TIMEVAL getElapsedTime(void)
{
	uint16_t timer = TIM_GetCounter(TIM4);

	return timer > last_time_set ? timer - last_time_set : last_time_set - timer; 	
}

/**
  * @brief  TIM4_Configuration
  * @param  NONE
  * @retval NONE
  */
static void TIM4_Configuration(void)
{
		/* ʱ�Ӽ���Ƶ���� */
	{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		//����ʱ����ʼ���ṹ�����
		/* Time Base configuration */
		TIM_TimeBaseStructure.TIM_Prescaler = 84- 1;	//							////��ʱ����Ƶ
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			//����ģʽ:���ϼ���
		TIM_TimeBaseStructure.TIM_Period = 0xFFFF;											//�����Զ���װ��ֵ
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;					//ʱ�ӷָ����֮��
		TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;		//���¼�������ʼֵ

		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);		
		 
		/* Prescaler configuration */
		TIM_PrescalerConfig(TIM4, 84-1, TIM_PSCReloadMode_Immediate);		//W H H 
	}
	
	/*
	    TIM4 Configuration: Output Compare Timing Mode:
    
    In this example TIM4 input clock (TIM4CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM4CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM4CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM4 counter clock at 6 MHz, the prescaler is computed as follows:
       Prescaler = (TIM4CLK / TIM4 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /6 MHz) - 1
                                              
    CC1 update rate = TIM4 counter clock / CCR1_Val = 146.48 Hz
    ==> Toggling frequency = 73.24 Hz
	
	*/
	
	//	{
	//    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	//		uint16_t PrescalerValue = 0;
	//		
	//		  /* Compute the prescaler value */
	//		PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 6000000) - 1;

	//		/* Time base configuration */
	//		TIM_TimeBaseStructure.TIM_Period = 65535;
	//		TIM_TimeBaseStructure.TIM_Prescaler = 0;
	//		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	//		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	
	//	  /* Prescaler configuration */
	//		TIM_PrescalerConfig(TIM4, PrescalerValue, TIM_PSCReloadMode_Immediate);
	//		
	//	}
	
		/*����Ƚ�ģʽ������*/
	{
		TIM_OCInitTypeDef  TIM_OCInitStructure;		//��������Ƚϳ�ʼ���ṹ�����

		/* Output Compare Timing Mode configuration: Channel1 */
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		

		TIM_OC1Init(TIM4, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable);		//��ֹTIM_CCR1�Ĵ�����Ԥװ��ʹ
	}
		
    /* TIM IT enable */
		TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);		//�ж�ʹ��
		/* TIM enable counter */
		TIM_Cmd(TIM4, ENABLE);				//W H H		//������ʱ����������CENλ��,һ��Ҫʹ��ʱ��
}
/**
  * @brief  NVIC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the TIM4 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  RCC_Configuration
  * @param  NONE
  * @retval NONE
  */
static void RCC_Configuration(void)
{
		/* TIM4 clock enable */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
}
/**
  * @brief  TIM4_start
  * @param  NONE
  * @retval NONE
  */
void TIM4_start(void)
{
		RCC_Configuration();

		/* configure TIM4 for remote and encoder */
		NVIC_Configuration();
		TIM4_Configuration();
}

/*�رն�ʱ��*/
void TIM4_stop(void)	
{
	TIM_Cmd(TIM4, DISABLE);			
}

/**
  * @brief  initTimer
  * @param  NONE
  * @retval NONE
  */
void initTimer(void)
{
		TIM4_start();
}
/**
  * @brief  TIM4_IRQHandler
  * @param  NONE
  * @retval NONE
  */
void TIM4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
  CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
	CPU_CRITICAL_EXIT();	//���ж�
#endif	
	
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)		//����/�Ƚ��ж�
	{	
			TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);		//����жϱ�־λ
			last_time_set = TIM_GetCounter(TIM4);
			TimeDispatch();		
	}

#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�
#endif	
}
/******************* (C) COPYRIGHT 2015 Personal Electronics *****END OF FILE****/
