#include "DebugUart1.h"
#include "stdio.h"
#include <stdarg.h>

uint8_t Uart1_Tx_Buff[UART1BUFFERSIZE];
uint8_t Uart1_Rx_Buff[UART1BUFFERSIZE];

uint8_t UART1_Rx_Cnt=0;
static uint8_t print_buffer[UART1BUFFERSIZE];//��ӡ����
static volatile uint32_t USART1_ReadIndex = 0;//������
static volatile uint8_t sendDoneFlag = 0;//������ɱ�־
static volatile uint8_t recvDoneFlag = 0;//������ɱ�־

void Uart1_init(void)			/* ����1 TX = PA9   RX = PA10 */
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
	
	/* ��1���� ����GPIO */
	/* �� GPIO ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* �� UART ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* ʹ��  DMA clock */
	RCC_AHB1PeriphClockCmd(USARTx_DMAx_CLK, ENABLE);
	
	/* �� PA9 ӳ��Ϊ USART1_TX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	/* �� PA10 ӳ��Ϊ USART1_RX */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	/* ����ģʽ */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	/* �������Ϊ���� */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	/* �ڲ���������ʹ�� */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ���� USART Rx Ϊ���ù��� */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
/* USARTx configured as follows:
		- BaudRate = 5250000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 8
			 is: (USART APB Clock / 8) 
			 Example: 
				- (USART3 APB1 Clock / 8) = (42 MHz / 8) = 5250000 baud
				- (USART1 APB2 Clock / 8) = (84 MHz / 8) = 10500000 baud
		   - Maximum BaudRate that can be achieved when using the Oversampling by 16
			 is: (USART APB Clock / 16) 
			 Example: (USART3 APB1 Clock / 16) = (42 MHz / 16) = 2625000 baud
			 Example: (USART1 APB2 Clock / 16) = (84 MHz / 16) = 5250000 baud
		- Word Length = 8 Bits
		- one Stop Bit
		- No parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/ 
	/* ��2���� ���ô���Ӳ������ */
	USART_InitStructure.USART_BaudRate = 115200;	/* ������ */
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* When using Parity the word length must be configured to 9 bits */
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);


	/* Configure DMA controller to manage USART TX and RX DMA request ----------*/ 
  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = UART1BUFFERSIZE ;											/* ����DMA��С */
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;									/* �������������ʹ�ܻ��߽�ֹ�����Ե� */
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull ; 	/* ���÷�ֵ */
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;						/* �����ڴ�Ϊ���ֽ�ͻ��ģʽ */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;					/* �����ڴ����ݵ�λ�����ֽ� */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									/* ʹ�ܵ�ַ���� */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;														/* ����DMA������ģʽ */
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART1->DR)) ;	/* ���������ַ */
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;			/* ��������Ϊ���ֽ�ͻ��ģʽ */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; /* �����������ݵ�λ�����ֽ� */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				/* ��ֹ�����ַ���� */
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;											/* �������ȼ� */
  /* Configure TX DMA */
  DMA_InitStructure.DMA_Channel = USARTx_TX_DMA_CHANNEL ;									 /* ���÷���ͨ�� */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;								 /* ���ô��ڴ浽���� */
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)Uart1_Tx_Buff ;				 /* �����ڴ��ַ */
  DMA_Init(USARTx_TX_DMA_STREAM,&DMA_InitStructure);
	
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = USARTx_RX_DMA_CHANNEL ;									 /* ���ý���ͨ�� */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;								 /* ���ô����赽�ڴ� */
  DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)Uart1_Rx_Buff ; 				 /* �����ڴ��ַ */
  DMA_Init(USARTx_RX_DMA_STREAM,&DMA_InitStructure);


	
	//USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);		/*ʹ��UART1���ж�*/
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	/* ʹ�ܽ����ж� */
	/*
		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
		ע��: ��Ҫ�ڴ˴��򿪷����ж�
		�����ж�ʹ����SendUart()������
	*/
	USART_Cmd(USART1, ENABLE);		/* ʹ�ܴ��� */
	
	
	/* ʹ�� USART DMA RX ���� */
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	
	/* ʹ�� USART DMA TX ���� */
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	
	/* ʹ�ܷ��ʹ�������ж� */
	DMA_ITConfig(USARTx_TX_DMA_STREAM, DMA_IT_TC, ENABLE);  
    
  /* ʹ�ܽ��մ�������ж� */
	DMA_ITConfig(USARTx_RX_DMA_STREAM, DMA_IT_TC, ENABLE); 		
	
//	/* ʹ�� DMA USART TX Stream */
//	DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE); 		//���ﻹ����ʹ�ܣ����ͺ����� ����д���� ���ͻ������ڴ��ַ
	
	/* ʹ�� DMA USART RX Stream */
//	DMA_Cmd(USARTx_RX_DMA_STREAM, ENABLE); 

	

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART1, USART_FLAG_TC);     /* �巢����Ǳ�־��Transmission Complete flag */

	/*uart1�ж����ȼ�������*/
	Uart1_NVIC_Config();
}

/*********************************************************************************************************
** Function name:       User_Uart3SendChar
** Descriptions:        �Ӵ��ڷ�������
** input parameters:    ch: ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void User_Uart1SendChar(unsigned char ch)
{
     USART_SendData (USART1,ch);                                        /* �����ַ�                     */
     while( USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET );         /* �ȴ��������                 */
     USART_ClearFlag(USART1,USART_FLAG_TC);                             /* ���������ɱ�ʶ             */
}

/*********************************************************************************************************
** Function name:       User_Uart3SendString
** Descriptions:        �򴮿ڷ����ַ���
** input parameters:    s:   Ҫ���͵��ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void User_Uart1SendString(unsigned char *s)
{
    while (*s != '\0') {
           User_Uart1SendChar(*s++);
    }
}

void Uart1_IRQ(void)
{
	unsigned char ch;				
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		ch = (USART_ReceiveData(USART1));
    ch=ch;           
	} 
}


/**
* ���ڷ��ͺ���
* @param buffer	��������
* @param length	���ͳ���
* @return
*/
uint16_t USART1_DMA_SendBuffer(const uint8_t* buffer, uint16_t length)
{
	if( (buffer==0) || (length==0) )
	{
		return 0;
	}
	sendDoneFlag = 0;
	DMA_Cmd(USARTx_TX_DMA_STREAM, DISABLE);
	USARTx_TX_DMA_STREAM->M0AR = (uint32_t)buffer;				/* �����ڴ��ַ */
	DMA_SetCurrDataCounter(USARTx_TX_DMA_STREAM, length);
	DMA_Cmd(USARTx_TX_DMA_STREAM, ENABLE);
	return length;
}

/*
�Զ����ӡ����
*/
void Debug_Printf(const char* format,...)
{
  	uint32_t length;
		va_list args;
		va_start(args, format);
		length = vsnprintf((char*)print_buffer,sizeof(print_buffer), (char*)format, args);//��ʽ������
		while(!sendDoneFlag);//�ȴ��������
		USART1_DMA_SendBuffer(print_buffer,length);//����
		va_end(args);
}

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fputc(int ch, FILE *f)
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART1, (uint8_t) ch);

	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
	{}

	return ch;
}

/*
*********************************************************************************************************
*	�� �� ��: fgetc
*	����˵��: �ض���getc��������������ʹ��scanff�����Ӵ���1��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int fgetc(FILE *f)
{
	/* �ȴ�����1�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

	return (int)USART_ReceiveData(USART1);
}


/*uart1 ���ж�Ƕ���������Ƶ�����*/
void Uart1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
			/* NVIC configuration */
			/* Configure the Priority Group to 2 bits */
		//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

		/* ʹ�� DMA Stream �ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	/* ʹ�� DMA Stream �ж�ͨ�� */
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_DMA_RX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}


#include "iap.h"
void USART1_IRQHandler(void)
{	
unsigned char ch;	

#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
  CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
	CPU_CRITICAL_EXIT();	//���ж�
#endif	
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)		//�����ж�
	{
		/* Read one byte from the receive data register */
		ch = (USART_ReceiveData(USART1));		//��ȡ���յ�������
		IAP_UART_Proceed_Cell(ch);
		if(UART1_Rx_Cnt < UART1BUFFERSIZE)
		{
			Uart1_Rx_Buff[UART1_Rx_Cnt] = ch;
			UART1_Rx_Cnt++;
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);				//��������жϱ�־
	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
 if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET)
 {
		USART_ClearFlag(USART1,USART_FLAG_ORE); //��SR��ʵ���������־
		USART_ReceiveData(USART1);    //��DR
 }

	/*DMA ʹ�õ�IDLE ���ж�*/ 
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{
		//		USART_ClearITPendingBit(USART1, USART_IT_IDLE);//����
		USART_GetITStatus(USART1, USART_IT_IDLE);	//ֻ��ͨ��:��ȡUSART_SR->��ȡUSART_DR�����USART1_IT_IDLE��־
		USART_ReceiveData(USART1);//�ն���״̬ ע�⣺������Ҫ�������ܹ�����жϱ�־λ����Ҳ��֪��Ϊɶ��
		recvDoneFlag = 1;
	}
	
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�
#endif	
}

/*
*
*********************************************************************************************************
*	�� �� ��: USARTx_DMA_TX_IRQHandler
*	����˵��: ����1��DMA��ʽ�µķ����ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USARTx_DMA_TX_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
  CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
	CPU_CRITICAL_EXIT();	//���ж�
#endif	
	/*
	  ����DMA_GetITStatus��ʹ���ϴ������⣬������־�ˣ�����޷�ͨ��������ʹ�ú���
	  DMA_GetFlagStatusȴ���ԣ������¼һ��������⣬�����������
	*/
//	if (DMA_GetFlagStatus(USARTx_TX_DMA_STREAM,USARTx_TX_DMA_FLAG_TCIF)==SET)
//// 	if (DMA_GetITStatus(USARTx_TX_DMA_STREAM,USARTx_TX_DMA_FLAG_TCIF) == SET)
// 	{
// 		/* ���DMA������ɱ�־ */
// 		DMA_ClearITPendingBit(USARTx_TX_DMA_STREAM, USARTx_TX_DMA_FLAG_TCIF);			
// 	}
	
//	/* ������üĴ���ֱ�Ӳ��� */
//	if(DMA2->HISR & USARTx_TX_DMA_FLAG_TCIF)
// 	{
//		DMA2->HIFCR = USARTx_TX_DMA_FLAG_TCIF;
//		sendDoneFlag = 1;
//	}
	
	/*�⺯���汾*/
	 /* Test on DMA Stream Transfer Complete interrupt */
	if(DMA_GetITStatus(USARTx_TX_DMA_STREAM, DMA_IT_TCIF7))
	{
		 /* Clear DMA Stream Transfer Complete interrupt pending bit */
    DMA_ClearITPendingBit(USARTx_TX_DMA_STREAM, DMA_IT_TCIF7);
		sendDoneFlag = 1;
	}
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�
#endif	
}

/*
*********************************************************************************************************
*	�� �� ��: USARTx_DMA_RX_IRQHandler
*	����˵��: ����1��DMA��ʽ�µĽ����ж�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void USARTx_DMA_RX_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
  CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
	CPU_CRITICAL_EXIT();	//���ж�
#endif		
	
	/* ������üĴ���ֱ�Ӳ��� */
	if(DMA2->LISR & USARTx_RX_DMA_FLAG_TCIF)
 	{
		recvDoneFlag = 1;		//��������жϱ�־
		DMA2->LIFCR = USARTx_RX_DMA_FLAG_TCIF;
		printf("uart1!\2\3\4\r\n");
 	}
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�
#endif		
}




//static void uart1_send(const char *str, unsigned int size)
//{
//    int pos = 0;

//    while(size)
//    {
//        if (str[pos] == '\0')
//            break;
//        
//				/* �ȴ��������� */
//        while(!(USART1->SR & 0x80));
//        /* �������� */
//        USART1->DR = str[pos];

//        pos ++;
//        size --;
//    }
//}




//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  //W H H
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;      
FILE __stdin;

void _ttywrch(int ch) 
{
//	USART_SendData(USART1, (uint8_t) ch);
	/*W H H ,��ͨ�߲����õĵ�uart1*/
	USART_SendData(USART1, (uint8_t) ch);
	
}

int ferror(FILE *f)
{ 
	// Your implementation of ferror
	return EOF;
}

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit( int x) 
{ 
	x = x; 
} 

#endif
