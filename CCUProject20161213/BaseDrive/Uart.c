
#include "stm32f4xx.h"
#include "stdio.h"
#include "Uart.h"
#include <stdarg.h>

void uart3_init(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ����GPIO_D��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* ��������3��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);


	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_Odd;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART3, &USART_InitStructure);

	/* ʹ�ܴ���3 */
	USART_Cmd(USART3, ENABLE);

	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
		�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */   

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	/*uart3�ж����ȼ�������*/
	Uart3_NVIC_Config();
}

/*********************************************************************************************************
** Function name:       User_Uart3SendChar
** Descriptions:        �Ӵ��ڷ�������
** input parameters:    ch: ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void User_Uart3SendChar(unsigned char ch)
{
     USART_SendData (USART3,ch);                                        /* �����ַ�                     */
     while( USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET );         /* �ȴ��������                 */
     USART_ClearFlag(USART3,USART_FLAG_TC);                             /* ���������ɱ�ʶ             */
}

/*********************************************************************************************************
** Function name:       User_Uart3SendString
** Descriptions:        �򴮿ڷ����ַ���
** input parameters:    s:   Ҫ���͵��ַ���ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void User_Uart3SendString(unsigned char *s)
{
    while (*s != '\0') {
           User_Uart3SendChar(*s++);
    }
}

void Uart3_IRQ(void)
{
	unsigned char ch;				
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		/* Read one byte from the receive data register */
		ch = (USART_ReceiveData(USART3));
    ch=ch;           
	} 
}

///*
//*********************************************************************************************************
//*	�� �� ��: fputc
//*	����˵��: �ض���putc��������������ʹ��printf�����Ӵ���1��ӡ���
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//int fputc(int ch, FILE *f)
//{
//	/* Place your implementation of fputc here */
//	/* e.g. write a character to the USART */
//	USART_SendData(USART3, (uint8_t) ch);

//	/* Loop until the end of transmission */
//	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
//	{}

//	return ch;
//}

///*
//*********************************************************************************************************
//*	�� �� ��: fgetc
//*	����˵��: �ض���getc��������������ʹ��scanff�����Ӵ���1��������
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//int fgetc(FILE *f)
//{
//	/* �ȴ�����3�������� */
//	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

//	return (int)USART_ReceiveData(USART3);
//}

/*uart3 ���ж�Ƕ���������Ƶ�����*/
void Uart3_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/* NVIC configuration */
	/* Configure the Priority Group to 2 bits */
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

static void _send(const char *str, unsigned int size)
{
    int pos = 0;

    while(size)
    {
        if (str[pos] == '\0')
            break;
        
				/* �ȴ��������� */
        while(!(USART3->SR & 0x80));
        /* �������� */
        USART3->DR = str[pos];

        pos ++;
        size --;
    }
}

void debug(const char* fmt,...)
{
    va_list ap;
    char string[129];

    string[128]='\0';
    va_start(ap,fmt);
    vsprintf(string,fmt,ap);
    va_end(ap);
    _send(string,64);
}



////�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  //W H H
//#if 1
//#pragma import(__use_no_semihosting)             
////��׼����Ҫ��֧�ֺ���                 
//struct __FILE 
//{ 
//	int handle; 
//}; 

//FILE __stdout;      
//FILE __stdin;

//void _ttywrch(int ch) 
//{
////	USART_SendData(USART3, (uint8_t) ch);
//	/*W H H ,��ͨ�߲����õĵ�uart1*/
//	USART_SendData(USART1, (uint8_t) ch);
//	
//}

//int ferror(FILE *f)
//{ 
//	// Your implementation of ferror
//	return EOF;
//}

////����_sys_exit()�Ա���ʹ�ð�����ģʽ    
//void _sys_exit( int x) 
//{ 
//	x = x; 
//} 

//#endif
