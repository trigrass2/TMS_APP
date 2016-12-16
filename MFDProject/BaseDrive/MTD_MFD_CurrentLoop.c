#include "MTD_MFD_CurrentLoop.h"
#include "Uart.h"
#include "sys.h"
#include "bsp_os.h"
#include "main.h"
#include "BCUComPro.h"

u8 USART4_CL_RxBuffer[100]; 	//���ջ���,���100���ֽ�
//u8 USART4_CL_TxBuffer[100]; 	//���ջ���,���100���ֽ�
u8 USART4_CL_RxLength;   			//���յ������ݳ���

#ifdef MTD_MFD_1_0_ENABLE	
void MTDMFD_CL_Gpio_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* ����GPIO ��ʱ��  */
		RCC_AHB1PeriphClockCmd(MTDMFD_CLTX_RCC ,ENABLE);		//��Ϊ���е����Ŷ�ʹ�õ�CPIOF
	
		/*TX*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = MTDMFD_CLTX_GPIO_PIN;
		
		GPIO_Init(MTDMFD_CLTX_GPIO, &GPIO_InitStructure);
	
		/*RX*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	
		GPIO_InitStructure.GPIO_Pin = MTDMFD_CLRX_GPIO_PIN;
	
		GPIO_Init(MTDMFD_CLRX_GPIO, &GPIO_InitStructure);
}
#endif


/*�汾 V2.0*/
#ifdef MTD_MFD_2_0_ENABLE

void MTDMFD_CL_UART_Gpio_Init(u32 BaudRate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	
	/* ����GPIO_C��ʱ�� */
	RCC_AHB1PeriphClockCmd(MTDMFD_UARTx_RX_RCC | MTDMFD_UARTx_TX_RCC, ENABLE);
	/* ��������4��ʱ�� */
	RCC_APB1PeriphClockCmd(MTDMFD_USARTx_CLK, ENABLE);

	/*PC10,PC11 ���Ÿ��õ�UART*/
	GPIO_PinAFConfig(MTDMFD_UARTx_TX_GPIO_PORT, MTDMFD_UARTx_TX_SOURCE, MTDMFD_UARTx_TX_AF);
	GPIO_PinAFConfig(MTDMFD_UARTx_RX_GPIO_PORT, MTDMFD_UARTx_RX_SOURCE, MTDMFD_UARTx_RX_AF);	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Pin   = MTDMFD_UARTx_TX_GPIO_PIN;
	GPIO_Init(MTDMFD_UARTx_TX_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = MTDMFD_UARTx_RX_GPIO_PIN;
	GPIO_Init(MTDMFD_UARTx_RX_GPIO_PORT, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate   = BaudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;	//�ֲ�697ҳ�����ʹ����żУ�飬����λΪ9λ
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_Even;		//ż��У��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(MTDMFD_USARTx, &USART_InitStructure);

	/* ʹ�ܴ���4 */
	USART_Cmd(MTDMFD_USARTx, ENABLE);

	USART_ITConfig(MTDMFD_USARTx, USART_IT_RXNE, ENABLE);		//ʹ�ܽ����ж�
	USART_ITConfig(MTDMFD_USARTx, USART_IT_PE, 	 ENABLE); 	//����PE��������ж�Bit 8PEIE: PE interrupt enable����żУ������ж�λ
	USART_ITConfig(MTDMFD_USARTx, USART_IT_ERR,  ENABLE);	  //CR2 ����ERR�ж�
	//USART_ITConfig(MTDMFD_USARTx, USART_IT_FE, 	 ENABLE);		//֡�����ж�λ
	
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	USART_ClearFlag(MTDMFD_USARTx, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */  
	
	/*uart4�ж����ȼ�������*/
  NVIC_InitStructure.NVIC_IRQChannel = MTDMFD_USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;		//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;					//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;						//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

void MTDMFD_CL_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	
	/* CPU��Сȱ�ݣ��������úã����ֱ��Send�����1���ֽڷ��Ͳ���ȥ
	�����������1���ֽ��޷���ȷ���ͳ�ȥ������ */
	//USART_ClearFlag(MTDMFD_USARTx, USART_FLAG_TC);     /* �巢����ɱ�־��Transmission Complete flag */  
	
	for(t=0;t<len;t++)
	{  
		 while(RESET == USART_GetFlagStatus(MTDMFD_USARTx, USART_FLAG_TXE));				//�ȴ����ͻ�����Ϊ��
			USART_SendData (MTDMFD_USARTx,buf[t]);                                    /* �����ַ�                     */
		 while( USART_GetFlagStatus(MTDMFD_USARTx,USART_FLAG_TC)==RESET );         	/* �ȴ��������                 */
		 //USART_ClearFlag(MTDMFD_USARTx,USART_FLAG_TC);                           	/* ���������ɱ�ʶ             */
	}
	
	USART4_CL_RxLength =0; //����һ֡���ݣ��ȴ����ܵ�ʱ�򣬽����ݳ�������
}
	
void MTDMFD_USARTx_IRQHandler(void)
{
	//OS_ERR err;
	static unsigned char StartFlag = 0;	
	//static u8 RecordDataLen=0;
	uint8_t RxChar;
	
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
			OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif	

	if(USART_GetITStatus(MTDMFD_USARTx,USART_IT_PE)!= RESET || USART_GetITStatus(MTDMFD_USARTx,USART_IT_FE)!= RESET)		
	{
		g_Uart4FrameErrorFlag=1;		//����ż���� ֡���� ��־����1
		
		USART_ClearITPendingBit(MTDMFD_USARTx,USART_IT_FE);			//���֡�����־
		USART_ClearITPendingBit(MTDMFD_USARTx,USART_IT_PE);			//�����ż�����־
		
		printf("Uart4 ��żУ�����!\r\n");		
	}
	
	 /* USART in Receiver mode */
  if (USART_GetITStatus(MTDMFD_USARTx, USART_IT_RXNE) != RESET)
  {
		if(USART4_CL_RxLength >=100 )
		{
			USART4_CL_RxLength =0;//��ʱ��0��ѭ������
		}

		/* Read one byte from the receive data register */
		RxChar = USART_ReceiveData(MTDMFD_USARTx);	//��ȡ���յ�������
		USART4_CL_RxBuffer[USART4_CL_RxLength++] = RxChar;
		
		if(USART4_CL_RxBuffer[0] ==  BCU_NACK)		//������ܵ�����ΪNACK��־����Ϊ�ڷ���һ֡���ݵ�ʱ�������ݳ������㣬���ܵ�ʱ��һ����USART4_CL_RxBuffer[0]��ʼ����
		{
			g_Uart4RxStartFlag = 1;  //	�������ݿ�ʼ��־	
			g_Uart4NackFlag = 1;		//����NACK��־
		}
		
		else		// ���ܵ���ȷ����
		{
			//if(RxChar == 0x02)  //��ʾ���ܵ�Ϊ��ʼ����
			if((USART4_CL_RxBuffer[0] ==  0x02 ) && (USART4_CL_RxLength ==1 )) //��ʾ��һ�ֽ� Ϊ0x02 ��Ϊ��ʼ		
			{
				StartFlag = 1;
				
				g_Uart4RxStartFlag = 1;  //	�������ݿ�ʼ��־	
				
				//RecordDataLen = USART4_CL_RxLength-1;		//��¼0x02�������� �����±� ��λ��
					
				//			/*���ͽ��ܿ�ʼ�¼���־*/
				//			OSFlagPost((OS_FLAG_GRP*)&BCUEventFlags,
				//				 (OS_FLAGS	  )RxStartFlag,				//��������ܿ�ʼ �¼���־
				//				 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
				//				 (OS_ERR*	    )&err);
			}
			if((StartFlag == 1) && (USART4_CL_RxBuffer[/*RecordDataLen+1*/1] == BCU_SD_COMMAND) && (USART4_CL_RxLength ==54))		//BCU SD��Ӧ���ݳ���Ϊ54����ʾ���ݽ������
			{										
				g_Uart4RxDataFlag = 1;
				StartFlag = 0;
				
				//g_BCURxSdOrTdFlag = BCU_RxSD;		// ��ʾ���ܵ�SD��Ϣ
				
				//			/*���ͽ��ܽ����¼���־*/
				//			OSFlagPost((OS_FLAG_GRP*)&BCUEventFlags,
				//				 (OS_FLAGS	  )RxEndFlag,										//��������ܽ��� �¼���־
				//				 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
				//				 (OS_ERR*	    )&err);			
			}
			else if((StartFlag == 1) && (USART4_CL_RxBuffer[/*RecordDataLen+1*/1] == BCU_TD_COMMAND) && (USART4_CL_RxLength ==56))		//Td ���� Ϊ56�ֽ�
			{
				g_Uart4RxDataFlag = 1;
				StartFlag = 0;
				
				//g_BCURxSdOrTdFlag = BCU_RxTD;		// ��ʾ���ܵ�TD��Ϣ
				
				//			/*���ͽ��ܽ����¼���־*/
				//			OSFlagPost((OS_FLAG_GRP*)&BCUEventFlags,
				//				 (OS_FLAGS	  )RxEndFlag,										//��������ܽ��� �¼���־
				//				 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
				//				 (OS_ERR*	    )&err);			
			}			
		}
		
		USART_ClearITPendingBit(MTDMFD_USARTx,USART_IT_RXNE);				//��������жϱ�־	
	}

	//���-������������Ҫ�ȶ�SR,�ٶ�DR�Ĵ����������������жϵ�����[ţ��˵Ҫ����]
	if(USART_GetFlagStatus(MTDMFD_USARTx,USART_FLAG_ORE)==SET)
	{
		USART_ClearFlag(MTDMFD_USARTx,USART_FLAG_ORE); //��SR��ʵ���������־
		USART_ReceiveData(MTDMFD_USARTx);   					  //��DR
		printf("Uart4 �����\r\n");
	}	
	
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    		 //�˳��ж�
	#endif	
}

#endif

