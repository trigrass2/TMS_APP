#include "SAF82525.h"
#include "delay.h"
#include "os.h"
#include "Uart.h"
#include "malloc.h"
#include "string.h"
#include "DebugUart1.h"
#include "VVVFComPro.h"

#define Bank1_SRAM1_ADDR  ((uint32_t)0x60000000)  
#define SAF82525_BASE_ADDR 	Bank1_SRAM1_ADDR
#define SAF82525_FIFO_LEN		32		//оƬ�Ļ�������СΪ32�ֽ�

__IO uint8_t HDLCARxBuf[200];
__IO uint8_t HDLCBRxBuf[200];
__IO uint16_t HDLCARxLen=0;
__IO uint16_t HDLCBRxLen=0;

u8  g_HDLCARxStartFlag=0;		//Aͨ�����ܿ�ʼ��־
u8  g_HDLCARxEndFlag=0;			//Aͨ�����ܽ�����־

u8  g_HDLCBRxStartFlag=0;		//Bͨ�����ܿ�ʼ��־
u8  g_HDLCBRxEndFlag=0;			//Bͨ�����ܽ�����־



/*�����жϼĴ��� ״̬ ȫ�ֱ���*/
uint8_t  g_ucBISTAStatus,g_ucBEXIRStatus,g_ucBRSTAStatus,	 g_ucAISTAStatus,g_ucAEXIRStatus,g_ucARSTAStatus;

void SAF82525_INT_GPIO_Init(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(SAF82525_INT_RCC, ENABLE); 		//ʹ��IOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //�����Ͻ�
  GPIO_InitStructure.GPIO_Pin = SAF82525_INT_GPIO_PIN;  //IO��Ϊ6
  GPIO_Init(SAF82525_INT_GPIO, &GPIO_InitStructure);    //��������gpio

  SYSCFG_EXTILineConfig(SAF82525_INT_EXIT_PORT, SAF82525_INT_EXIT_PIN);//ʵʼ���ж���11

  EXTI_InitStructure.EXTI_Line = EXTI_Line6;                   //�����ж���Ϊ�ж���11
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;          //�����ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //����Ϊ�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                    //�����ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);                              

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;             
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//Ӳ���жϺ����ķ������
void EXTI9_5_IRQHandler(void)		
{
	//static u8 i=0;
	//static u8 HDLCAComandVale = 0;
	//static u8 HDLCBComandVale = 0;
	//u8 StartVale=0;
	
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     		//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif	

	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		/*��ȡ״̬�Ĵ�����ֵ*/
		/*�ȶ�ȡB�Ĵ�*/
		g_ucBISTAStatus = SAF82525_Read_Register(BISTA);
		g_ucBEXIRStatus = SAF82525_Read_Register(BEXIR);
		g_ucBRSTAStatus = SAF82525_Read_Register(BRSTA);
		
		/*�ڶ�ȡA�Ĵ���*/
		g_ucAISTAStatus = SAF82525_Read_Register(AISTA);
		g_ucAEXIRStatus = SAF82525_Read_Register(AEXIR);	
		g_ucARSTAStatus = SAF82525_Read_Register(ARSTA);
			
		if(((g_ucBISTAStatus & RME) == RME)||((g_ucBISTAStatus & RPF) == RPF) || ((g_ucBISTAStatus & EXB) == EXB))
		{
			BChanleRxData(HDLCBRxBuf,&HDLCBRxLen);	//B��������
					
					//			if((HDLCBRxBuf[0] == 0x7E) && (HDLCBRxLen<=32))		//��һ�ν���HDLCʱ�����¾Ϳ���
					//			{
					//				g_HDLCBRxStartFlag =1;	//�������ݿ�ʼ
									
					//				i=0;		//������ж��ٸ�0x7E
					//				while(1)
					//				{
					//					StartVale = *(HDLCBRxBuf+i);
					//					if(StartVale == VVVF_PAD)
					//					{
					//						i++;
					//					}
					//					else
					//					{
					//						break; //����whileѭ��
					//					}
					//				}
					//				
					//				HDLCBComandVale = HDLCBRxBuf[i+4];		//�õ�B�����ֽ� 
					//			}
					
					//			switch(HDLCBComandVale)		//����B�õ�������
					//			{
					//				case VVVF_SD_COMMAND:
					//					if(g_HDLCBRxStartFlag==1 && HDLCBRxLen >= 51)
					//					{
					//						g_HDLCBRxEndFlag =1; //���ܽ���
					//						
					//						g_HDLCBRxStartFlag = 0;//�����ʼ��־
					//					}
					//				break;
					//				
					//				case VVVF_TD_COMMAND:
					//					if(g_HDLCBRxStartFlag==1 && HDLCBRxLen >= 120)
					//					{
					//						g_HDLCBRxEndFlag =1; //���ܽ���
					//						
					//						g_HDLCBRxStartFlag = 0;//�����ʼ��־
					//					}					
					//					break;
					//				
					//				default:
					//					printf("HDLC B Comdand err!\r\n");
					//			}
		}
		
		if(((g_ucBISTAStatus &ICA) ==ICA)||((g_ucBISTAStatus & EXA )==EXA) )
		{
			AChanleRxData(HDLCARxBuf,&HDLCARxLen);	//A��������
						
					//			if((HDLCARxBuf[0] == 0x7E) && (HDLCARxLen<=32))	//��һ�ν���HDLCʱ�����¾Ϳ���
					//			{
					//				g_HDLCARxStartFlag =1;	//�������ݿ�ʼ
									
					//				i=0;		//������ж��ٸ�0x7E
					//				while(1)
					//				{
					//					StartVale = *(HDLCBRxBuf+i);
					//					if(StartVale == VVVF_PAD)
					//					{
					//						i++;
					//					}
					//					else
					//					{
					//						break; //����whileѭ��
					//					}
					//				}
									
					//				HDLCAComandVale = HDLCARxBuf[i+4];		//�õ�A�����ֽ� 				
					//			}
					
					//			switch(HDLCAComandVale)		//����A�õ�������
					//			{
					//				case VVVF_SD_COMMAND:
					//					if(g_HDLCARxStartFlag==1 && HDLCARxLen >= 51)
					//					{
					//						g_HDLCARxEndFlag =1; //���ܽ���
					//						
					//						g_HDLCARxStartFlag = 0;//�����ʼ��־
					//					}
					//				break;
					//				
					//				case VVVF_TD_COMMAND:
					//					if(g_HDLCARxStartFlag==1 && HDLCARxLen >= 120)
					//					{
					//						g_HDLCARxEndFlag =1; //���ܽ���
					//						
					//						g_HDLCARxStartFlag = 0;//�����ʼ��־
					//					}					
					//					break;
					//				
					//				default:
					//					printf("HDLC A Comdand err!\r\n");
					//			}			
		}
				
		/* �����ش����жϷ�ʽ��CPU,�������ж�Ϊ������ʱ,����ָ����ٴβ����ж� �ֲ�40ҳ */	
		SAF82525_Write_Register(BMASK,0xFF);
		SAF82525_Write_Register(BMASK,(RSC|TIN|XPR/*|ICA|EXA|EXB*/));	//ʹ��RPF, RME, XPR�ж�
		SAF82525_Write_Register(AMASK,0xFF);
		SAF82525_Write_Register(AMASK,(RSC|TIN|XPR/*|ICA|EXB|EXA*/));	//ʹ��RPF, RME, XPR�ж�
		
		EXTI_ClearITPendingBit(EXTI_Line6);		//����жϱ�־ �ж���6
	}
	
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    		//�˳��ж�
	#endif
}

void SAF82525_Rest_CPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
		/* ����GPIO ��ʱ��  */
	RCC_AHB1PeriphClockCmd(SAF82525_REST_RCC ,ENABLE);		//GPIOF
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;				//GPIO_PuPd_NOPULL;	�޸�Ϊ��������
	GPIO_InitStructure.GPIO_Pin = SAF82525_REST_GPIO_PIN;
	
	GPIO_Init(SAF82525_REST_GPIO,&GPIO_InitStructure);
	
	SAF82525_REST_ON();		//�ϵ�֮��λ
}


void SAF82525_GPIO_Init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  ReadTim;
	FSMC_NORSRAMTimingInitTypeDef  WriteTim;
  GPIO_InitTypeDef GPIO_InitStructure; 
	
	  /* Enable GPIOs clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF ,ENABLE);

  /* Enable FSMC clock */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE); 
	
	/*-- GPIOs Configuration -----------------------------------------------------*/
/*
 +-------------------+--------------------+------------------+------------------+
 |   	 								PD14 <-> FSMC_D0  | PF0 <-> FSMC_A0  |  |
 | PD7  <-> FSMC_NE1	PD15 <-> FSMC_D1	| PF1 <-> FSMC_A1  |  |
 | PD4  <-> FSMC_NOE |PD0  <-> FSMC_D2	| PF2 <-> FSMC_A2  |  |
 | PD5  <-> FSMC_NWE |PD1  <-> FSMC_D3  | PF3 <-> FSMC_A3  |  |
 |									 |PE7  <-> FSMC_D4  | PF4 <-> FSMC_A4  |  |
 |									 |PE8  <-> FSMC_D5  | PF5 <-> FSMC_A5  |  |
 |									 |PE9  <-> FSMC_D6  | PF12 <-> FSMC_A6 |  |
 |									 |PE10 <-> FSMC_D7 	| 										|------------------+
	
	PE6  <-> SAF_INT
	PF6  <-> SAF_RES
*/

  /* GPIOD configuration */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_4  | GPIO_Pin_5  | 
                                GPIO_Pin_7  | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);


  /* GPIOE configuration */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 ;

  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* GPIOF configuration */
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource0 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource1 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource2 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource3 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource4 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource5 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF, GPIO_PinSource12 , GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  | GPIO_Pin_1  | GPIO_Pin_2  | GPIO_Pin_3  | 
                                GPIO_Pin_4  | GPIO_Pin_5  | GPIO_Pin_12 ;      

  GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	
/*-- FSMC Configuration ------------------------------------------------------*/
  ReadTim.FSMC_AddressSetupTime = 3;						//��ַ����ʱ�䣨ADDSET��Ϊ2��HCLK 1/168M=6ns*3=18ns	
  ReadTim.FSMC_AddressHoldTime = 3;							//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�
  ReadTim.FSMC_DataSetupTime = 12;						  //���ݱ���ʱ��Ϊ18��HCLK	=6*6=36ns
  ReadTim.FSMC_BusTurnAroundDuration = 0;
  ReadTim.FSMC_CLKDivision = 0;
  ReadTim.FSMC_DataLatency = 0;
  ReadTim.FSMC_AccessMode = FSMC_AccessMode_A;
	
	WriteTim.FSMC_AddressSetupTime = 3;						//��ַ����ʱ�䣨ADDSET��Ϊ4��HCLK 1/168M=6ns*3=18ns	
  WriteTim.FSMC_AddressHoldTime = 3;						//��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�
  WriteTim.FSMC_DataSetupTime = 11;							//���ݱ���ʱ��Ϊ18��HCLK	=6*3=18ns
  WriteTim.FSMC_BusTurnAroundDuration = 0;				
  WriteTim.FSMC_CLKDivision = 0;
  WriteTim.FSMC_DataLatency = 0;
  WriteTim.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;  
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;				//W H H   �޸�Ϊ��չģʽ��ѡ��ģʽA.
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &ReadTim;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &WriteTim;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 

  /*!< Enable FSMC Bank1_SRAM1 Bank */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE); 

}

/*
	��ָ���Ĵ�����ֵ
	���ض�ȡ��������
*/
uint8_t SAF82525_Read_Register(const uint8_t ReadAddr)
{
	//	/*__IO*/ uint8_t ReadData;		//W H H һ��Ҫʹvolatile
	//	ReadData =  *(__IO uint8_t *)(SAF82525_BASE_ADDR | ReadAddr);
	//	return ReadData;
	return  *(__IO uint8_t *)(SAF82525_BASE_ADDR | ReadAddr);
}

/*
	дָ���Ĵ�����ֵ
*/
void SAF82525_Write_Register(const uint8_t WriteAddr, uint8_t WriteData)
{
	*(uint8_t *) (SAF82525_BASE_ADDR | WriteAddr) = WriteData;
}


/*
	�޸ļĴ�����ĳλ
*/
void SAF82525_ModifyBit_Register(uint8_t Addr,uint8_t Mask,uint8_t Data)
{
	uint8_t CurrentRegVale,ModifyRegVale,temp;
	CurrentRegVale = SAF82525_Read_Register(Addr);
	
	if((CurrentRegVale&Mask)==Mask)
	{
		temp = 1;
	}

	else 
		temp = 0;
	
	if(Data)
	{
		if(temp == 0)
		{
			ModifyRegVale = CurrentRegVale | Mask;		//��ĳλ�޸�Ϊ1
			SAF82525_Write_Register(Addr,ModifyRegVale);
		}
	}
	else
	{
		if(temp ==1)
		{
			ModifyRegVale = CurrentRegVale &(!Mask);	//��ĳλ�޸�Ϊ0
			SAF82525_Write_Register(Addr,ModifyRegVale);
		}
	}
}



/*
��ʼ��
//W H H  ,ע�⣬Ҫ���ȡ��������ȷ��HDLCоƬ������������ִ�У�����ȵ�����ִ�������ȥ��ȡ�Ĵ�����
*/
void SAF82525_Init(uint8_t HDLC_BautRate)
{
	//	uint8_t a,b;
	SAF82525_INT_GPIO_Init();
	SAF82525_Rest_CPIO_Init();
	SAF82525_GPIO_Init();
	
	//step1: SAF82525оƬ��λ
	SAF82525_REST_ON();		//������λ
	delay_ms(50);
	SAF82525_REST_OFF();  //�رո�λ

	
	/*ͨ��A����*/
	//step2: SAF82525 оƬREG����							// W H H ֡�����FLAG���� 1(������) ����������
	SAF82525_Write_Register(ACCR1,/*0x5F*/0x5F);	//�µ�ģʽ 0| NRZI���ݱ��� 1 0|����������� 1|  ֡�����FLAG���� 1(������),��������IDLE���������T��D���ű����ڡ�1��״̬��0|ѡ��ʱ��ģʽ7	(�ص㣬©����·�������ʱ��7)
	SAF82525_Write_Register(AMODE,/*0x98*/MDS1|TMD|RAC/*|TLP(�Ի�ģʽ)*/);	//(MDS1 MDS0)transparent mode | (TMD)internal mode | (RAC)HDLC receiver active
	SAF82525_Write_Register(ACCR2,/*0x28*/BDF | TIO);	//(BDF ) BR9 - BR0 [CCR2 BRG] |( TIO) Tx CLKA, Tx CLKB pins are outputs
	
	/*�ֲ�57ҳ the DPLL is supplied with a reference clock from BRG which is 16 times the
		data clock rate (clock mode 2, 3, 6, 7). */
	SAF82525_Write_Register(ABGR,HDLC_BautRate);	//k = (0x11 + 1)*2 =36 <== >���ò�����Ϊ19200 bps		11059200 / 19200 =576 /16 =36
	
	/*�ж�״̬λ��Ϊ0��ʾ��Ӧλ���ж�ʹ��*/
	SAF82525_Write_Register(AMASK,/*~(RME|RPF|XPR|EXA)*/(RSC|TIN|XPR/*|ICA|EXB|EXA*/));	//ʹ��RPF, RME, XPR�ж�

	/*��λ���ܣ�����״̬*/	
	SetACMDRegister(RHR|XRES);
	SAF82525_Write_Register(ACCR1,PU|/*0x5F*/0x5F);	//PU :power up (active)

		/*
		����ȡISTA�Ĵ����������жϲ��ᱻ��ʾ��
		�෴��������ж� ��λ���ᱻ��λ��
		Masked interrupts are not indicated when reading
		ISTA. Instead, they remain internally stored and will be indicated after the respective MASK bit
		is reset
		*/
		//	b=SAF82525_Read_Register(AISTA);		//���ܶ�ȡISTA�Ĵ����������ḴλMASK�Ĵ���ֵΪ0x00��
		//	debugprintf("AMASK:%x\r\n",b);
	
	//	/*��λ���ܣ�����״̬*/	
	//	SetACMDRegister(RHR|XRES);

	//	#ifdef DEBUG_PRINTF
	//		a=CheckASTAR();			//W H H  ,ע�⣬Ҫ���ȡ��������ȷ��HDLCоƬ������������ִ�У�����ȵ�����ִ�������ȥ��ȡ�Ĵ�����
	//		debugprintf("A state=0x%x\r\n",a);
	//		
	//			/*����ʹ�ã����Զ�ȡ���� ��ȷ*/
	//		a=SAF82525_Read_Register(ACCR1);
	//		b=SAF82525_Read_Register(ASTAR);
	//		debugprintf("ACCR1:%x,ASTAR:%x\r\n",a,b);

	//		a=SAF82525_Read_Register(AMODE);
	//		b=SAF82525_Read_Register(ACCR2);
	//		debugprintf("AMODE:%x,ACCR2:%x\r\n",a,b);
	//	#endif



	
	/*ͨ��B����*/
	//step2: SAF82525 оƬREG����
	SAF82525_Write_Register(BCCR1,/*0x5F*/0x5F);	//�µ�ģʽ| NRZI���ݱ��� |�����������|  ֡�����FLAG���� |ѡ��ʱ��ģʽ7  (�ص㣬©����·�������ʱ��7)
	SAF82525_Write_Register(BMODE,/*0x98*/MDS1|TMD|RAC/*|TLP*/);	//(MDS1 MDS0)transparent mode | (TMD)internal mode | (RAC)HDLC receiver active
	SAF82525_Write_Register(BCCR2,/*0x28*/BDF | TIO);	//(BDF ) BR9 - BR0 [CCR2 BRG] |( TIO) Tx CLKA, Tx CLKB pins are outputs

	/*�ֲ�57ҳ the DPLL is supplied with a reference clock from BRG which is 16 times the
		data clock rate (clock mode 2, 3, 6, 7). */
	SAF82525_Write_Register(BBGR,HDLC_BautRate);	//k = (0x11 + 1)*2 =36 <== >���ò�����Ϊ19200 bps		11059200 / 19200 =576 /16 =36
																								//k= (0x02+1)*2 = 6 <== >���ò�����Ϊ125000 bps		12000000 / 125000 =96 /16 = 6
	/*�ж�״̬λ��Ϊ0��ʾ��Ӧλ���ж�ʹ��*/
	SAF82525_Write_Register(BMASK,/*~(RME|RPF|XPR|EXB)*/(RSC|TIN|XPR/*|ICA|EXA|EXB*/));	//ʹ��RPF, RME, XPR,EXB�ж�

	/*��λ���ܣ�����״̬*/	
	SetBCMDRegister(RHR|XRES);
	SAF82525_Write_Register(BCCR1,PU|/*0x5F*/0x5F);	//PU :power up (active)
		
	//	/*��λ���ܣ�����״̬*/	
	//SetBCMDRegister(RHR|XRES);

	//	#ifdef DEBUG_PRINTF
	//		b=CheckBSTAR();				//����������ִ�е�ʱ�򣬶�ȡ�Ĵ�����ֵ �᲻׼��������ִ����ɵ�ʱ���ȡ���ݾ�׼ȷ�ˡ�
	//		debugprintf("B state=0x%x\r\n",b);
	//		
	//		/*����ʹ�ã����Զ�ȡ���� ��ȷ*/
	//		a=SAF82525_Read_Register(BCCR1);
	//		b=SAF82525_Read_Register(BSTAR);
	//		debugprintf("BCCR1:%x,BSTAR:%x\r\n",a,b);

	//		a=SAF82525_Read_Register(BMODE);
	//		b=SAF82525_Read_Register(BCCR2);
	//		debugprintf("BMODE:%x,BCCR2:%x\r\n",a,b);
	//	#endif
}

/*
	���SAB82525״̬����   
*/
uint8_t CheckASTAR(void)		
{
	uint8_t ucStatus;
	while(1)
	{
		ucStatus = SAF82525_Read_Register(ASTAR);
		if((ucStatus&CEC)==0x00) 	/* CMDR��д */
		{
			break;
		}
	}
	return ucStatus;
}

/*
	���SAB82525״̬����   
*/
uint8_t CheckBSTAR(void)		
{
	uint8_t ucStatus;
	while(1)
	{
		ucStatus = SAF82525_Read_Register(BSTAR);
		if((ucStatus&CEC) == 0x00) 	/* CMDR��д */
		{
			break;
		}
	}
	return ucStatus;
}


/*
	�ֲ�87ҳ

CEC :
//  0 : no command is currently executed the CMDR Reg can be written to.
//  1 : a command(written previously to CMDR) is currently executed.no
//      further command can be temporarily written via CMDR Register.

	The maximum time between writing to the CMDR register and the execution of the
	command is 2.5 clock cycles. Therefore, if the CPU operates with a very high clock in
	comparison with the HSCX��s clock, it's recommended that the CEC bit of the STAR
	register is checked before writing to the CMDR register to avoid any loss of commands.
	
	д�뵽CMDR�Ĵ��� �� ����ִ������ʱ����2.5��ʱ�����ڣ����cpu�Ĳ���ʱ�ӱ� HSCX��ʱ�ӷǳ���ʱ��
	������д��CMDR�Ĵ���֮ǰ�����STAR�Ĵ�����CECλ(0,��ʾCMDR����д��)���Ա������ʧ��
*/
/*
	����ͨ��A������Ĵ���
*/
void SetACMDRegister(uint8_t CMD_Data)
{
	uint8_t ucStatus;
	while(1)					//WAIT_UNTIL_NO_CMD_EXECUTE,�ȴ�ֱ��û��cmd������ִ��
	{
		ucStatus = SAF82525_Read_Register(ASTAR);
		if((ucStatus & CEC)==0x00) 	/* CMDR��д */
		{
			SAF82525_Write_Register(ACMDR,CMD_Data);
			break;
		}
	}
}

/*
	����ͨ��B������Ĵ���
*/
void SetBCMDRegister(uint8_t CMD_Data)
{
	uint8_t ucStatus;
	while(1)					//WAIT_UNTIL_NO_CMD_EXECUTE,�ȴ�ֱ��û��cmd������ִ��
	{
		ucStatus = SAF82525_Read_Register(BSTAR);
		if((ucStatus & CEC)==0x00) 	/* CMDR��д */
		{
			SAF82525_Write_Register(BCMDR,CMD_Data);
			break;
		}
	}
}

/*
	Aͨ�����ͻ�����дʹ��
*/
__inline void WaitXFIFOWriteEnable(uint8_t ChanleRegister)
{
	uint8_t ucStatus;
	while(1)
	{
		ucStatus = SAF82525_Read_Register(ChanleRegister);
		if((ucStatus & XFW_CEC) == XFW)	// ���ȵȴ����ͻ�����дʹ��  WAIT_UNTIL_XFIFO_WRITE_ENABLE 
		{
			break;
		}
	}
}


/*
	//Aͨ����������
*/
void AChanleTxData(uint8_t *buf,int len)
{
	int RecordLen;	//��¼���ݳ���
	u8  RecordWriteCount =0 ;//��¼д��32�ֽڵĴ���
	u8  i=0;						//����for
	
	RecordLen = len;		//���ݳ��ȼ�¼

	while(RecordLen > SAF82525_FIFO_LEN)		//���ͳ��ȴ���32�ֽڲ���
	{
		// ���ȵȴ����ͻ�����дʹ��  WAIT_UNTIL_XFIFO_WRITE_ENABLE 
		WaitXFIFOWriteEnable(ASTAR);		//while  whh
		
		for(i=0;i<SAF82525_FIFO_LEN;i++)
		{
			SAF82525_Write_Register(AFIFO,buf[i+RecordWriteCount*SAF82525_FIFO_LEN]);
		}
		
		SetACMDRegister(XTF);					//while  whh
		
		RecordLen -= SAF82525_FIFO_LEN;
		RecordWriteCount++;
	}
	

	//���ͳ���ʣ�ಿ�� ���� С�ڵ���32�ֽ�
	{
			// ���ȵȴ����ͻ�����дʹ��  WAIT_UNTIL_XFIFO_WRITE_ENABLE 
		WaitXFIFOWriteEnable(ASTAR);		//while  whh

		for(i=0;i<RecordLen;i++)
		{
			SAF82525_Write_Register(AFIFO,buf[i+RecordWriteCount*SAF82525_FIFO_LEN]);
		}
				
		SetACMDRegister(XTF | XME);		//while  whh
	}
			
}

/*
//Bͨ����������
*/
void BChanleTxData(uint8_t *buf,int len)
{
	int RecordLen;	//��¼���ݳ���
	u8  RecordWriteCount =0 ;//��¼д��32�ֽڵĴ���
	u8  i=0;						//����for
	
	RecordLen = len;

	while(RecordLen > SAF82525_FIFO_LEN)		//���ͳ��ȴ���32�ֽڲ���
	{
		// ���ȵȴ����ͻ�����дʹ��  WAIT_UNTIL_XFIFO_WRITE_ENABLE 
		WaitXFIFOWriteEnable(BSTAR);		//while  whh
		
		for(i=0;i<SAF82525_FIFO_LEN;i++)
		{
			SAF82525_Write_Register(BFIFO,buf[i+RecordWriteCount*SAF82525_FIFO_LEN]);
		}
		
		SetBCMDRegister(XTF);					//while  whh
		
		RecordLen -= SAF82525_FIFO_LEN;
		RecordWriteCount++;
	}
	

	//���ͳ���ʣ�ಿ�� ���� С�ڵ���32�ֽ�
	{
			// ���ȵȴ����ͻ�����дʹ��  WAIT_UNTIL_XFIFO_WRITE_ENABLE 
		WaitXFIFOWriteEnable(BSTAR);		//while  whh
		
		for(i=0;i<RecordLen;i++)
		{
			SAF82525_Write_Register(BFIFO,buf[i+RecordWriteCount*SAF82525_FIFO_LEN]);
		}
		
		SetBCMDRegister(XTF | XME);			//while  whh
	}
}


/*
//Aͨ����������

�ֲ�83ҳ
The ICA, EXA, and EXB bit are present in channel B only and point to the ISTA (CHA),
EXIR (CHA), and EXIR (CHB) registers.
After the HSCX has requested an interrupt by turning its INT pin to low, the CPU must
first read the ISTA register of channel B and check the state of these bits in order to
determine which interrupt source(s) of which channel(s) has caused the interrupt. More
than one interrupt source may be indicated by a single interrupt request

���ж�����ʱ���ȼ��Bͨ����ISTA��ICA��EXA��EXBΪ��Ϊ����ȷ�ж�Դ���ж�ͨ����

�ֲ�76ҳ
RME �ж�֮�����һ���ֽ�ΪRSTA ����״ֵ̬�� RBCH��RBCL�Ĵ����д洢���ݳ��ȣ�
*/
void AChanleRxData(__IO uint8_t *buf,__IO uint16_t *len)
{

	/*
	˵��
		1. ���ܲ���RPF�ж�.
		2. ���ǰ�����32B FIFO��С�����.
		3��RME:Receive Message End. XPR��Transmit Pool Ready.
		4. ��Ϊ����С����256B������ʹ��RBCL,RBCH��
		5. ���RME�жϣ��գ���XPR�жϣ�����.
	*/
	//	uint8_t  m_ucISTAStatus,m_ucEXIRStatus,m_ucRSTAStatus;
	uint8_t m_ucRBCL,m_ucRBCH;
	uint8_t  i=0;
	uint16_t RxLen,RecordTempLen;
	static uint8_t FrameSectionRxCount=0;
	
	//	m_ucISTAStatus = SAF82525_Read_Register(AISTA);
	//	m_ucEXIRStatus = SAF82525_Read_Register(AEXIR);
	//	m_ucRSTAStatus = SAF82525_Read_Register(ARSTA);
	
	#ifdef DEBUG_PRINTF
	//	debugprintf("AAAm_ISTA=0x%x,m_EXIR=0x%x,m_RSTA=0x%x\r\n",g_ucAISTAStatus,g_ucAEXIRStatus,g_ucARSTAStatus);
	//	debugprintf("Acount=%d\r\n",FrameSectionRxCount);
	#endif 
	
	/*����Aͨ��EXIRָʾ�ж�*/
	/*��״̬A�Ĵ����в���ʾ ICA��EXA��EXB*/
	//if((g_ucAISTAStatus & EXA) == EXA)
	//{
		if((g_ucAEXIRStatus & RFO) == RFO)	//��ʾ����֡���
		{
			#ifdef DEBUG_PRINTF
	//				debugprintf("A_RFO!\r\n");
	//				debugprintf("A_���!\r\n");
	//				debugprintf("A_���!\r\n");
	//				debugprintf("A_���!\r\n");
	//				debugprintf("A_���!\r\n");
	//				debugprintf("A_���!\r\n");
	//				debugprintf("A_���!\r\n");
			#endif
					
			/* ����֡���--RECEIVE FRAME OVERFLOW */
			/* ��λ����FIFO��������RECEIVER     */
			SetACMDRegister(RHR);
		}
	//}
	

	/* ����Aͨ��RPF�ж�  */
	if((g_ucAISTAStatus & RPF) == RPF)
	{
		#ifdef DEBUG_PRINTF
	//		debugprintf("A_RPF!\r\n");
		#endif 

		 /* �յ�>32�ֽڵ�֡--RECEIVE POOL FULL   */
		for(i=0;i<SAF82525_FIFO_LEN;i++)
		{
			buf[i+FrameSectionRxCount*SAF82525_FIFO_LEN]=SAF82525_Read_Register(AFIFO);
		}
		SetACMDRegister(RMC);//���ͽ���������ݣ���HSCX���Խ�������
	
		//SAF82525_ModifyBit_Register(AISTA,RPF,0);//����ж�λ����ȡ�жϼĴ�����ʱ���жϾͱ�����ˡ�
		FrameSectionRxCount++;		
	}

	
  /* ����Aͨ��RME�ж�  */
	if((g_ucAISTAStatus & RME) == RME)
	{
		#ifdef DEBUG_PRINTF
	//		debugprintf("A_RME!\r\n");
		#endif 
		
		 /* �յ�<32�ֽڵ�֡--RECEIVE MESSAGE END */
		 /* �յ���Ч��֡--VALID FRAME RECEIVED,��CRC��ȷ,û��RDO--RECEIVE DATA OVERFLOW  */
		if( ((g_ucARSTAStatus & SAB_CRC) == SAB_CRC ) && ((g_ucARSTAStatus & VFR) == VFR) && ((g_ucARSTAStatus & RDO) != RDO ))
		{
			//AHDLC_Int_Flag = 1;		
			g_HDLCARxEndFlag = 1;	  //��������жϱ�־λ  //WHH
			
			m_ucRBCL = SAF82525_Read_Register(ARBCL);
			m_ucRBCH = SAF82525_Read_Register(ARBCH);

			//����õ����ݵĳ���
			RxLen	= (((uint16_t)m_ucRBCH & 0x0f)<<8) | m_ucRBCL;	
			
			/* ֡���� */
			*len = RxLen-1;		//��֡���ܵĳ��ȷ���,���ܼĴ��������һ���ֽ�Ϊ����״ֵ̬
			
			/*����һ֡�����У�ʣ��ĳ���*/
			RecordTempLen = RxLen - (FrameSectionRxCount * SAF82525_FIFO_LEN)-1;		//RME�ж�֮�󣬽��ܼĴ��������һ���ֽ�Ϊ����״ֵ̬
			
			//if((RecordTempLen>0) && (RecordTempLen <=32))
			//{
				for(i=0;i<RecordTempLen;i++)
				{
					buf[i+FrameSectionRxCount*SAF82525_FIFO_LEN] = SAF82525_Read_Register(AFIFO);
				}
				
				SetACMDRegister(RMC);//���ͽ���������ݣ���HSCX���Խ�������
			//}
			//SAF82525_ModifyBit_Register(AISTA,RME,0);//����ж�λ����ȡ�жϼĴ�����ʱ���жϾͱ�����ˡ�
		}
		
		else  /* ��Ч��֡ */
		{
			#ifdef DEBUG_PRINTF
			//			debugprintf("A_error!\r\n");
			#endif
			
			/* ��λ����FIFO��������RECEIVER     */
			SetACMDRegister(RHR);
		}
		
		FrameSectionRxCount=0;//����һ֡���ݽ���������̬������0
	}
	 /* �����ش����жϷ�ʽ��CPU,�������ж�Ϊ������ʱ,����ָ����ٴβ����ж� �ֲ�40ҳ */	
	//	SAF82525_Write_Register(AMASK,0xFF);
	//	SAF82525_Write_Register(AMASK,(RSC|TIN|XPR/*|ICA|EXB|EXA*/));	//ʹ��RPF, RME, XPR�ж�
}	

/*
//Bͨ����������
*/
void BChanleRxData(__IO uint8_t *buf,__IO uint16_t  *len)
{
	//	uint8_t  m_ucISTAStatus,m_ucEXIRStatus,m_ucRSTAStatus;
	uint8_t m_ucRBCL,m_ucRBCH;
	uint8_t  i=0;
	uint16_t RxLen,RecordTempLen;
	static uint8_t FrameSectionRxCount=0;
		
	//	m_ucISTAStatus = SAF82525_Read_Register(BISTA);
	//	m_ucEXIRStatus = SAF82525_Read_Register(BEXIR);
	//	m_ucRSTAStatus = SAF82525_Read_Register(BRSTA);
	//	
	#ifdef DEBUG_PRINTF
	//	debugprintf("BBBm_ISTA=0x%x,m_EXIR=0x%x,m_RSTA=0x%x\r\n",g_ucBISTAStatus,g_ucBEXIRStatus,g_ucBRSTAStatus);
	//	debugprintf("Bcount=%d\r\n",FrameSectionRxCount);
	#endif
	
	/*����Bͨ��EXIRָʾ�ж�*/
	/*�ж�����Ĵ��� ���һ��*/
	//	if((g_ucBISTAStatus & EXB) == EXB)
	//	{
		if((g_ucBEXIRStatus & RFO) == RFO)	//��ʾ����֡���
		{
			#ifdef DEBUG_PRINTF
	//					debugprintf("B_RFO!\r\n");
	//				debugprintf("B_���!\r\n");
	//				debugprintf("B_���!\r\n");
	//				debugprintf("B_���!\r\n");
	//				debugprintf("B_���!\r\n");
	//				debugprintf("B_���!\r\n");
	//				debugprintf("B_���!\r\n");
			#endif
			
				/* ����֡���--RECEIVE FRAME OVERFLOW */
			  /* ��λ����FIFO��������RECEIVER     */
				SetBCMDRegister(RHR);
				return ;
		}
	//	}
	
	/* ����Bͨ��RPF�ж�  */
	if((g_ucBISTAStatus & RPF) == RPF)
	{
		#ifdef DEBUG_PRINTF
		//			debugprintf("B_RPF!\r\n");
		#endif
		
		 /* �յ�>32�ֽڵ�֡--RECEIVE POOL FULL   */
		for(i=0;i<SAF82525_FIFO_LEN;i++)
		{
			buf[i+FrameSectionRxCount*SAF82525_FIFO_LEN]=SAF82525_Read_Register(BFIFO);
		}
		SetBCMDRegister(RMC);//���ͽ���������ݣ���HSCX���Խ�������
		
		//SAF82525_ModifyBit_Register(BISTA,RPF,0);//����ж�λ,��ʵ�ڶ�ȡ�жϼĴ�����ʱ���жϾͱ�����ˡ�
		FrameSectionRxCount++;
	}
	
  /* ����Bͨ��RME�ж�  */
	if((g_ucBISTAStatus & RME) == RME)
	{
		#ifdef DEBUG_PRINTF
	//			debugprintf("B_RME!\r\n");
		#endif
		
		 /* �յ�<32�ֽڵ�֡--RECEIVE MESSAGE END */
		 /* �յ���Ч��֡--VALID FRAME RECEIVED,��CRC��ȷ,û��RDO--RECEIVE DATA OVERFLOW  */
		if( ((g_ucBRSTAStatus & SAB_CRC) == SAB_CRC ) && ((g_ucBRSTAStatus & VFR) == VFR) && ((g_ucBRSTAStatus & RDO) != RDO ))
		{
			
			//BHDLC_Int_Flag = 1;		//����Bͨ���Ľ����жϱ�־����ȡ�жϼĴ�����ʱ���жϾͱ�����ˡ�
			g_HDLCBRxEndFlag = 1;	  //��������жϱ�־λ  //WHH
			
			m_ucRBCL = SAF82525_Read_Register(BRBCL);
			m_ucRBCH = SAF82525_Read_Register(BRBCH);

			//����õ����ݵĳ���
			RxLen	= (((uint16_t)m_ucRBCH & 0x0f)<<8) |m_ucRBCL;		
			/* ֡���� */
			*len = RxLen-1;		//��֡���ܵĳ��ȷ��أ����ܼĴ��������һ���ֽ�Ϊ����״ֵ̬
			
			/*����һ֡�����У�ʣ��ĳ���*/
			RecordTempLen = RxLen - (FrameSectionRxCount * SAF82525_FIFO_LEN)-1;		//RME�ж�֮�󣬽��ܼĴ��������һ���ֽ�Ϊ����״ֵ̬

			//if((RecordTempLen>0) && (RecordTempLen <=32))
			//{
				for(i=0;i<RecordTempLen;i++)
				{
					buf[i+FrameSectionRxCount*SAF82525_FIFO_LEN] = SAF82525_Read_Register(BFIFO);
				}
				SetBCMDRegister(RMC);//���ͽ���������ݣ���HSCX���Խ�������
			//}
			//SAF82525_ModifyBit_Register(BISTA,RME,0);//����ж�λ   ,��ʵ�ڶ�ȡ�жϼĴ�����ʱ���жϾͱ�����ˡ�
		}
		
		else  /* ��Ч��֡ */
		{
			#ifdef DEBUG_PRINTF
	//				debugprintf("B_error!\r\n");
			#endif
			
			/* ��λ����FIFO��������RECEIVER     */
			SetBCMDRegister(RHR);
		}
		
		FrameSectionRxCount=0;//����һ֡���ݽ���������̬������0
	}
	 /* �����ش����жϷ�ʽ��CPU,�������ж�Ϊ������ʱ,����ָ����ٴβ����ж� �ֲ�40ҳ */	
	//	SAF82525_Write_Register(BMASK,0xFF);
	//	SAF82525_Write_Register(BMASK,(RSC|TIN|XPR/*|ICA|EXA|EXB*/));	//ʹ��RPF, RME, XPR�ж�
}	

