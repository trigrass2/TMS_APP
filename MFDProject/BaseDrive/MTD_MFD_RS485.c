#include "MTD_MFD_RS485.h"
#include "delay.h"
#include "sys.h"
#include "os.h"
#include "main.h"
#include "CommunPro.h"
#include "Uart.h"
#include "bsp.h"

u8 USART2_RS485_RxBuffer[100]; 	//���ջ���,���100���ֽ�
//u8 USART2_RS485_TxBuffer[100]; 	//���ջ���,���100���ֽ�
u8 USART2_RS485_RxLength = 0;   //���յ������ݳ���


u8 USART3_RS485_RxBuffer[100]; 	//���ջ���,���100���ֽ�
//u8 USART3_RS485_TxBuffer[100]; 	//���ջ���,���100���ֽ�
u8 USART3_RS485_RxLength = 0;   //���յ������ݳ���

/*
��ʼ��
*/
void MTDMFD_USART2_RS485_Init(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ����GPIO_A��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	/* ��������2��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
	/*����2���Ÿ���ӳ��*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2����ΪUSART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3����ΪUSART2
	
	/*USART2*/    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					 //���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				 //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //�ٶ�100MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2��GPIOA3
	GPIO_Init(GPIOA,&GPIO_InitStructure); 								 //��ʼ��PA2��PA3
	
	//PA4���������RS485ģʽ����  
		/* ����GPIO_A��ʱ�� */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 					//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Pin = MTDMFD_USART2_DR_GPIO_PIN; //GPIOA4
	GPIO_Init(MTDMFD_USART2_DR_GPIO,&GPIO_InitStructure); 	//��ʼ��PA4	
	
	/*USART2 ����*/
	USART_InitStructure.USART_BaudRate = BaudRate;								//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;		//�ֳ�Ϊ9λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_Odd;					//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure);										  //��ʼ������2
	
	/* ʹ�ܴ���2 */
	USART_Cmd(USART2, ENABLE);  
	
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART2, USART_FLAG_TC); 	 				/* �巢����ɱ�־��Transmission Complete flag */   
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);		//����UART2�����ж�
	USART_ITConfig(USART2, USART_IT_PE, 	ENABLE);    //����PE��������ж�Bit 8PEIE: PE interrupt enable
	USART_ITConfig(USART2, USART_IT_ERR, 	ENABLE);		//CR2 ����ERR�ж�
	//USART_ITConfig(USART2, USART_IT_FE, 	ENABLE);

	/*uart2�ж����ȼ�������*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;		//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;					//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	USART2_RS485_RX_EN();					 //Ĭ��Ϊ����ģʽ	
}

/*
	RS485����len���ֽ�.
	buf:�������׵�ַ
	len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
*/
void MTDMFD_USART2_RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	USART2_RS485_TX_EN();				//����Ϊ����ģʽ
	for(t=0;t<len;t++)
	{
		while(RESET == USART_GetFlagStatus(USART2, USART_FLAG_TXE));	//�ȴ����ͻ�����Ϊ��
		USART_SendData(USART2,buf[t]); //��������
		while(RESET == USART_GetFlagStatus(USART2,USART_FLAG_TC)); 	//�ȴ����ͽ���
	}
	
	USART2_RS485_RxLength = 0;			//�������ݵĳ���Ϊ0
	USART2_RS485_RX_EN();					 //Ĭ��Ϊ����ģʽ	
}


/*
	RS485��ѯ���յ�������
	buf:���ջ����׵�ַ
	len:���������ݳ���
*/
void MTDMFD_USART2_RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 RxLen = USART2_RS485_RxLength ;
	u8 i=0;
	*len =0; //Ĭ��Ϊ0
	
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if((RxLen==USART2_RS485_RxLength)&&RxLen)//���յ�������,�ҽ��������
	{
		for(i=0;i<RxLen;i++)
		{
			buf[i]=USART2_RS485_RxBuffer[i];	
		}		
		*len=USART2_RS485_RxLength;	//��¼�������ݳ���
		//USART2_RS485_RxLength=0;		//����
	}
}	

/*
	�жϽ�������
*/
void USART2_IRQHandler(void)
{
	//OS_ERR err;
	static unsigned char StartFlag = 0;	
	uint8_t RxChar;

	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
			OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif	
	
	if(USART_GetITStatus(USART2,USART_IT_PE)!= RESET || USART_GetITStatus(USART2,USART_IT_FE)!= RESET)		//��δ��뽫�Ὣ�������� B . �����벿��
	{
		g_Uart2FrameErrorFlag=1;		//����ż���� ֡���� ��־����1
		
		//		/*������żУ�����֡���� �¼���־*/
		//		OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
		//			 (OS_FLAGS	  )RxParityFrameError,				//������żУ�����֡���� �¼���־
		//			 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
		//			 (OS_ERR*	    )&err);
					
		USART_ClearITPendingBit(USART2,USART_IT_FE);			//���֡�����־
		USART_ClearITPendingBit(USART2,USART_IT_PE);			//�����ż�����־
		printf("Uart2 ��żУ�����!\r\n");		
	}
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)		//�����ж�
	{
		if(USART2_RS485_RxLength >=100 )
		{
			USART2_RS485_RxLength =0;//��ʱ��0��ѭ������
		}
		
		/* Read one byte from the receive data register */
		//USART2_RS485_RxBuffer[USART2_RS485_RxLength++] = USART_ReceiveData(USART2);	//��ȡ���յ�������
		RxChar = USART_ReceiveData(USART2);	//��ȡ���յ�������
		USART2_RS485_RxBuffer[USART2_RS485_RxLength++] = RxChar;
		
		switch(ChooseBoard)
		{	
			case MC1_MFD1_NODEID:			//�յ������ж�
			case T_MFD1_NODEID:	
			case M_MFD1_NODEID:
			case T1_MFD1_NODEID:
			case T2_MFD1_NODEID:
			case MC2_MFD1_NODEID:	
				//if(RxChar == 0x02)  //��ʾ���ܵ�Ϊ��ʼ����
				if((USART2_RS485_RxBuffer[0] ==  0x02 ) && (USART2_RS485_RxLength ==1 )) //��ʾ��һ�ֽ� Ϊ0x02 ��Ϊ��ʼ
				{
					StartFlag = 1;
					g_Uart2RxStartFlag = 1;
					
					//					/*���ͽ��ܿ�ʼ�¼���־*/
					//					OSFlagPost((OS_FLAG_GRP*)&ACCEventFlags,
					//						 (OS_FLAGS	  )RxStartFlag,				//��������ܿ�ʼ �¼���־
					//						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
					//						 (OS_ERR*	    )&err);
				}
				if((StartFlag == 1) && (USART2_RS485_RxLength == 14))		//ACC��Ӧ���ݳ���Ϊ14����ʾ���ݽ������
				{
					g_Uart2RxDataFlag = 1;
					StartFlag = 0;
					
					//					/*���ͽ��ܽ����¼���־*/
					//					OSFlagPost((OS_FLAG_GRP*)&ACCEventFlags,
					//						 (OS_FLAGS	  )RxEndFlag,										//��������ܽ��� �¼���־
					//						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
					//						 (OS_ERR*	    )&err);			
				}
			break;
				
			case	MC1_MFD2_NODEID:		//PIS�����ж�
			case	MC2_MFD2_NODEID:
				if((USART2_RS485_RxBuffer[0] ==  0x02 ) && (USART2_RS485_RxLength ==1 )) //��ʾ��һ�ֽ� Ϊ0x02 ��Ϊ��ʼ
				{
					StartFlag = 1;
					g_Uart2RxStartFlag = 1;				
				}
				if((StartFlag == 1) && (USART2_RS485_RxLength == 29))	//PIS��Ӧ���ݳ���Ϊ29����ʾ���ݽ������
				{
					g_Uart2RxDataFlag = 1;
					StartFlag = 0;					
				}
				break;

			default:
				printf("uart2 Rx ChooseBoard Error !\r\n");
				break;				
		}
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);				//��������жϱ�־
	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART2,USART_FLAG_ORE); //��SR��ʵ���������־
		USART_ReceiveData(USART2);   					  //��DR
		printf("Uart2 �����\r\n");	
	}
	
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    		 //�˳��ж�
	#endif	
}



/*
	����3�ĳ�ʼ������
*/

void MTDMFD_USART3_RS485_Init(u32 BaudRate,u16 VerifyMode)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	//PE14���������RS485ģʽ����  
	/* ����GPIO_E��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;						//���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 					//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 						//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; //GPIOE14
	GPIO_Init(GPIOE,&GPIO_InitStructure); 	//��ʼ��PE14	
	
	
	/* ����GPIO_D��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/* ��������3��ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		
	/*����3���Ÿ���ӳ��*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3); //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
				
	/*USART3*/    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;					 //���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 				 //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 					 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //�ٶ�100MHz
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*USART3 ����*/
	USART_InitStructure.USART_BaudRate = BaudRate;								//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;		//�ֳ�Ϊ9λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
	USART_InitStructure.USART_Parity = VerifyMode;		//USART_Parity_Odd;					//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure);										  //��ʼ������3
	
	/* ʹ�ܴ���3 */
	USART_Cmd(USART3, ENABLE);  
	
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(USART3, USART_FLAG_TC); 	 				/* �巢����ɱ�־��Transmission Complete flag */   
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);		//����UART3�����ж�
	USART_ITConfig(USART3, USART_IT_PE, 	ENABLE);    //����PE��������ж�Bit 8PEIE: PE interrupt enable
	USART_ITConfig(USART3, USART_IT_ERR, 	ENABLE);		//CR2 ����ERR�ж�
	//USART_ITConfig(USART3, USART_IT_FE, 	ENABLE);

	/*uart3�ж����ȼ�������*/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;					//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	USART3_RS485_RX_EN();					 //Ĭ��Ϊ����ģʽ	
}


/*
	RS485����len���ֽ�.
	buf:�������׵�ַ
	len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
*/
void MTDMFD_USART3_RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	USART3_RS485_TX_EN();						//����Ϊ����ģʽ
	for(t=0;t<len;t++)
	{
		while(RESET == USART_GetFlagStatus(USART3, USART_FLAG_TXE));	//�ȴ����ͻ�����Ϊ��
		USART_SendData(USART3,buf[t]); 	//��������	
		while(RESET ==  USART_GetFlagStatus(USART3,USART_FLAG_TC)); 	//�ȴ����ͽ���
	}
	USART3_RS485_RxLength = 0;			//�������ݵĳ���Ϊ0
	USART3_RS485_RX_EN();					 //Ĭ��Ϊ����ģʽ	
	//debugprintf("send u3!\r\n");
}


void MTDMFD_USART3_RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8  RxLen = 0 ;
	u8 i=0;
	*len =0; //Ĭ��Ϊ0
	
	RxLen = USART3_RS485_RxLength;
	//printf("rxlen=%d\r\n",RxLen);
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if((RxLen==USART3_RS485_RxLength)&&RxLen)//���յ�������,�ҽ��������
	{
		for(i=0;i<RxLen;i++)
		{
			buf[i]=USART3_RS485_RxBuffer[i];	
			//printf("u3r[%d]=0x%x\r\n",i,buf[i]);
		}		
		*len=USART3_RS485_RxLength;	//��¼�������ݳ���
	//	USART3_RS485_RxLength=0;		//����
	}
}

/*
	�жϽ�������
*/
void USART3_IRQHandler(void)
{
	//OS_ERR err;
	static unsigned char StartFlag = 0;	
	uint8_t RxChar;
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
			OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif	

	if(USART_GetITStatus(USART3,USART_IT_PE)!= RESET || USART_GetITStatus(USART3,USART_IT_FE)!= RESET)		//��δ��뽫�Ὣ�������� B . �����벿��
	{
		g_Uart3FrameErrorFlag=1;		//����ż���� ֡���� ��־����1
		
		//		/*������żУ�����֡���� �¼���־*/
		//		OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
		//			 (OS_FLAGS	  )RxParityFrameError,				//������żУ�����֡���� �¼���־
		//			 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
		//			 (OS_ERR*	    )&err);
		
		USART_ClearITPendingBit(USART3,USART_IT_FE);			//���֡�����־
		USART_ClearITPendingBit(USART3,USART_IT_PE);			//�����ż�����־
		
		printf("Uart3 ��żУ�����!\r\n");		
	}
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)		//�����ж�
	{
		if(USART3_RS485_RxLength >=100 )
		{
			USART3_RS485_RxLength =0;//��ʱ��0��ѭ������
		}

		/* Read one byte from the receive data register */
		//USART3_RS485_RxBuffer[USART3_RS485_RxLength++] = USART_ReceiveData(USART3);	//��ȡ���յ�������
		RxChar = USART_ReceiveData(USART3);	//��ȡ���յ�������
		USART3_RS485_RxBuffer[USART3_RS485_RxLength++] = RxChar;

		/*�������񼶱��ź���*/
		//	OSTaskSemPost(&USART3Rx_TCB,OS_OPT_POST_NONE,&err);	
		//printf("U3[0]=0x%x\r\n",USART3_RS485_RxBuffer[0]);
		//printf("u3len=%d\r\n",USART3_RS485_RxLength);
		
		switch(ChooseBoard)
		{	
			case MC1_MFD1_NODEID:			//�Ž����ж�
			case T_MFD1_NODEID:	
			case M_MFD1_NODEID:
			case T1_MFD1_NODEID:
			case T2_MFD1_NODEID:
			case MC2_MFD1_NODEID:	
				//if(RxChar == 0x02)  //��ʾ���ܵ�Ϊ��ʼ����
				if((USART3_RS485_RxBuffer[0] ==  0x02 ) && (USART3_RS485_RxLength ==1 )) //��ʾ��һ�ֽ� Ϊ0x02 ��Ϊ��ʼ
				{
					StartFlag = 1;
					
					g_Uart3RxStartFlag =1;	
					
					//printf("U3 Rx start!\r\n");

					//					/*���ͽ��ܿ�ʼ�¼���־*/
					//					OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
					//						 (OS_FLAGS	  )RxStartFlag,				//��������ܿ�ʼ �¼���־
					//						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
					//						 (OS_ERR*	    )&err);
				}
				if((StartFlag == 1) && (USART3_RS485_RxLength ==8))		//����Ӧ���ݳ���Ϊ8����ʾ���ݽ������
				{
					g_Uart3RxDataFlag = 1;
					StartFlag = 0;
					
					//					printf("U3 Rx end!\r\n");
					//					{
					//						u8 i=0;
					//						for(i=0;i<8;i++)
					//						{
					//							printf("u3[%d]=0x%x\r\n",i,USART3_RS485_RxBuffer[i]);
					//						}
					//					}
					
					//					/*���ͽ��ܽ����¼���־*/
					//					OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
					//						 (OS_FLAGS	  )RxEndFlag,				//��������ܽ��� �¼���־
					//						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
					//						 (OS_ERR*	    )&err);			
				}
			break;
				
			case	MC1_MFD2_NODEID:		//ATC�����ж�
			case	MC2_MFD2_NODEID:
					//�жϽ��ܿ�ʼ��־
					if((USART3_RS485_RxBuffer[0] == ATC_DLE) &&(USART3_RS485_RxLength ==1 ))
					{
						StartFlag =1;
						
						g_Uart3RxStartFlag =1;	
					}
					
					/*�жϽ��ܽ�����־*/
					if((StartFlag == 1) && (USART3_RS485_RxLength ==54))		//ATC ����SDΪ54�ֽ�
					{
						g_Uart3RxDataFlag = 1;
						
						StartFlag = 0;
					}
			
				break;
			
			case  MC1_MFD3_NODEID:		//ACP�����ж�
			case 	MC2_MFD3_NODEID:
					//�жϽ��ܿ�ʼ��־
					if((USART3_RS485_RxBuffer[0] == ACP_DEVICEADDRES) &&(USART3_RS485_RxBuffer[1] == ACP_COMMAND) &&(USART3_RS485_RxLength ==2 ))
					{
						StartFlag =1;
						g_Uart3RxStartFlag =1;	
					}
					
					/*�жϽ��ܽ�����־*/
					if((StartFlag == 1) && (USART3_RS485_RxLength ==16))		//ACP ����SDΪ16�ֽ�
					{
						g_Uart3RxDataFlag = 1;
						StartFlag = 0;
					}				
				break;
				
			default:
				printf("uart3 Rx ChooseBoard Error !\r\n");
				break;
		}
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);				//��������жϱ�־
	}
	
	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(USART3,USART_FLAG_ORE); //��SR��ʵ���������־
		USART_ReceiveData(USART3);   					  //��DR
		printf("Uart3 �����\r\n");
	}
	
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    		 //�˳��ж�
	#endif	
}
