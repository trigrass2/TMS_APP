/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    13-April-2012
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "Uart.h"
#include "os.h"


/*
�ж����ȼ������⡣
��������ͬʱ��������ռʽ���ȼ�����ͬ���жϷ�������ô�����˳����˭����Ӧ���ȼ�����˭���Ƚ�
���жϣ�����һ����Ҫע����ǣ������ʱ��������ж�֮��������һ����ռʽ���ȼ���ͬ������Ӧ���ȼ�
���ߵ��жϣ���ʱ���������е��жϡ�
*/



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(unsigned int * hardfault_args)
{
	unsigned int stacked_r0;
	unsigned int stacked_r1;
	unsigned int stacked_r2;
	unsigned int stacked_r3;
	unsigned int stacked_r12;
	unsigned int stacked_lr;
	unsigned int stacked_pc;
	unsigned int stacked_psr;
	
	stacked_r0 = ((unsigned long) hardfault_args[0]);
	stacked_r1 = ((unsigned long) hardfault_args[1]);
	stacked_r2 = ((unsigned long) hardfault_args[2]);
	stacked_r3 = ((unsigned long) hardfault_args[3]);
	stacked_r12 = ((unsigned long) hardfault_args[4]);
	stacked_lr = ((unsigned long) hardfault_args[5]);
	stacked_pc = ((unsigned long) hardfault_args[6]);
	stacked_psr = ((unsigned long) hardfault_args[7]);
	
	printf ("\n\n[Hard fault handler - all numbers in hex]\r\n");
	printf ("R0 = %x\r\n", stacked_r0);
	printf ("R1 = %x\r\n", stacked_r1);
	printf ("R2 = %x\r\n", stacked_r2);
	printf ("R3 = %x\r\n", stacked_r3);
	printf ("R12 = %x\r\n", stacked_r12);
	printf ("LR [R14] = %x subroutine call return address\r\n", stacked_lr);
	printf ("PC [R15] = %x program counter\r\n", stacked_pc);
	printf ("PSR = %x\r\n", stacked_psr);
	printf ("BFAR = %lx\r\n", (*((volatile unsigned long *)(0xE000ED38))));
	printf ("CFSR = %lx\r\n", (*((volatile unsigned long *)(0xE000ED28))));
	printf ("HFSR = %lx\r\n", (*((volatile unsigned long *)(0xE000ED2C))));
	printf ("DFSR = %lx\r\n", (*((volatile unsigned long *)(0xE000ED30))));
	printf ("AFSR = %lx\r\n", (*((volatile unsigned long *)(0xE000ED3C))));
	printf ("SCB_SHCSR = %x\r\n", SCB->SHCSR);

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
//		 printf("HardFault_Handler\r\n");			//������������ȴ����Ź����и�λ
  }
	
//	 /* ��os_core.c�ļ��ﶨ��,����и������ȼ������������,��ִ��һ�������л� */
//	OSIntExit();
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
//		printf("MemManage_Handler\r\n");
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
//		printf("BusFault_Handler\r\n");
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
//		printf("UsageFault_Handler\r\n");
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
	while(1)
	{
//		printf("SVC_Handler\r\n");
	}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{

//}


extern u8 g_RxDataFlag;
extern u8 g_FrameErrorFlag;
/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/
u8 g_RxDataFlag=0;
u8 g_FrameErrorFlag=0;
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{	
unsigned char ch;	

#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
  CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
	CPU_CRITICAL_EXIT();	//���ж�
#endif	
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)		//�����ж�
	{
		g_RxDataFlag=1;	//����ʼ�������ݵı�־λ����Ϊ1
		
		if(USART_GetITStatus(USART3,USART_IT_PE)!= RESET || USART_GetITStatus(USART3,USART_IT_FE)!= RESET)		//��δ��뽫�Ὣ�������� B . �����벿��
		{
			g_FrameErrorFlag=1;		//����ż���� ֡���� ��־����1
				
			USART_ClearITPendingBit(USART3,USART_IT_RXNE);		//����жϱ�־
			USART_ClearITPendingBit(USART3,USART_IT_PE);			//�����ż�����־
		}
		
			USART_ClearITPendingBit(USART3,USART_IT_FE);			//���֡�����־
		/* Read one byte from the receive data register */
		ch = (USART_ReceiveData(USART3));		//��ȡ���յ�������
		(void)ch;		//��ֹ����

	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
 if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
 {
		USART_ClearFlag(USART3,USART_FLAG_ORE); //��SR��ʵ���������־
		USART_ReceiveData(USART3);    //��DR
 }
	
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�
#endif	
}

// /*�ⲿ��0���ж�ִ�к���*/
//void EXTI0_IRQHandler(void)
//{
//    ExtiLine0_IRQ();  
//}
// /*�ⲿ��6���ж�ִ�к���*/
//void EXTI9_5_IRQHandler(void)
//{
//    ExtiLine6_IRQ();
//}
//

///**
//  * @brief  This function handles CAN1 RX0 request.
//  * @param  None
//  * @retval None
//  */
//void CAN1_RX0_IRQHandler(void)
//{
//#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
//	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//  CPU_CRITICAL_ENTER();	//���ж�
//		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
//	CPU_CRITICAL_EXIT();	//���ж�
//#endif
//	{
//	CanRxMsg RxMessage; 
//	 u8 i;
//    if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
//    {

//        CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);                    //����жϱ�־                
////     		
////				if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)	
////					return 1;		//û�н��յ�����,ֱ���˳�
//			
//				CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);                     //��ȡ����  
//				
//				for(i=0;i<8;i++)
//					printf("CAN1 Rx CAN2 Data,Rx1buf[%d]:0x%x\r\n",i,RxMessage.Data[i]);
//			printf("\r\n");

//    }
//	}


//#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
//	OSIntExit();    	//�˳��ж�
//#endif
//}

///**
//  * @brief  This function handles CAN2 RX0 request.
//  * @param  None
//  * @retval None
//  */
//extern void Ican_Receive_ISR(void);
//void CAN2_RX0_IRQHandler(void)
//{

////	iCANMSG g_piCANMSG;

//#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
//	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//  CPU_CRITICAL_ENTER();	//���ж�
//		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
//	CPU_CRITICAL_EXIT();	//���ж�
//#endif

//{		CanRxMsg RxMessage;
//		u8 i;
//	
//    if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
//    {
//        			   
//        CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);                        //����жϱ�־   
//				
////				if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)	
////					return 1;		//û�н��յ�����,ֱ���˳�
//			
//        CAN_Receive(CAN2,CAN_FIFO0,&RxMessage);                         //��ȡ����   
//				
//				for(i=0;i<8;i++)
//					printf("CAN2 Rx CAN1 Data,Rx2buf[%d]:0x%x\r\n",i,RxMessage.Data[i]);		
//			printf("\r\n");		
//			
//    }
//	}

//	
//#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
//	OSIntExit();    	//�˳��ж�
//#endif	
//}



//extern void MCP2515_INT1_IRQ(void);		//ִ��can3���жϺ���
//void EXTI9_5_IRQHandler(void)
//{
//		MCP2515_INT1_IRQ();
//}


///*SD�����жϷ������*/		/*SD�������Ѿ�д���жϷ���*/
//void SDIO_IRQHandler(void)
//{
//#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
//	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//  CPU_CRITICAL_ENTER();	//���ж�
//		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
//	CPU_CRITICAL_EXIT();	//���ж�
//#endif
//	
//	/* Process All SDIO Interrupt Sources */
//	SD_ProcessIRQSrc();
//	
//#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
//	OSIntExit();    	//�˳��ж�
//#endif	
//}

