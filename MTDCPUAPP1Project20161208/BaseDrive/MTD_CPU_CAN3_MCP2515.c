#include "MTD_CPU_CAN3_MCP2515.h"
#include "MCP2515_BitTime.h"
#include "delay.h"
#include "Uart.h"
#include "os.h"
#include "string.h"
#include "can_stm32.h"
#include "canfestival.h"

extern CO_Data MTDCAN3Master_Data;
extern OS_MUTEX	CAN3_MUTEX;		//����һ�������ź���,����can3���ͽ�������Ļ���

#define MCP2515_INT1_RCC           RCC_AHB1Periph_GPIOG
#define MCP2515_INT1_GPIO          GPIOG
#define MCP2515_INT1_GPIO_PIN      GPIO_Pin_7
#define MCP2515_INT1_EXIT_PORT		 EXTI_PortSourceGPIOG
#define MCP2515_INT1_EXIT_PIN			 EXTI_PinSource7


void	MCP2515_INT1_GpioInit(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(MCP2515_INT1_RCC, ENABLE); 		//ʹ��IOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //�����Ͻ�
  GPIO_InitStructure.GPIO_Pin = MCP2515_INT1_GPIO_PIN;  //IO��Ϊ7
  GPIO_Init(MCP2515_INT1_GPIO, &GPIO_InitStructure);    //��������gpio

  SYSCFG_EXTILineConfig(MCP2515_INT1_EXIT_PORT, MCP2515_INT1_EXIT_PIN);//ʵʼ���ж���7

  EXTI_InitStructure.EXTI_Line = EXTI_Line7;                   //�����ж���Ϊ�ж���7
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

/*mcp2515 �ж���7*/
//extern void MCP2515_INT1_IRQ(void);
void EXTI9_5_IRQHandler(void)
{
		MCP2515_INT1_IRQ();
}


void MCP2515_INT1_IRQ(void)
{
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
			OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif	

    if(EXTI_GetITStatus(EXTI_Line7) != RESET)
		{
			OS_ERR err;
			
			//�����ź���
			OSSemPost(&can3recv_sem,OS_OPT_POST_1,&err);		
			//printf("RxOs3sem=%d\r\n",can3recv_sem.Ctr);	
			
			//g_can3RxCount++;
			//printf("RxOs3=%d\r\n",g_can3RxCount);
			EXTI_ClearITPendingBit(EXTI_Line7);		//����жϱ�־ �ж���7
    }
				
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    	//�˳��ж�
	#endif
}


unsigned char MTDCPU_CAN3_MCP2515_Init(const unsigned int CanBaudRate)
{  
//	unsigned char dummy; 
	unsigned char res;
	
    /*First initialize the SPI periphere*/ 
    /*Then initialize the MCP2515.Step as follow: 
    1>Get into configuration mode by reset MCP2515 or write the bit of CANCTRL.REQOP. 
    2>Check the CANSTAT by ReadByte_MCP2515(CANSTAT) if MCP2515 is already into configuration mode. 
    3>Configurate the registers to set the baudrate:CN1,CN2,CN3. 
    4>Configurate TXRTSCTRL to set the function of TXnRTS pin. 
    5>Configurate TRXBnCTRL to set the pirority of transmit mailbox:TXB0,TXB1,TXB2. 
    6>Configurate TXBn'SID ,EID and DLC.This step can configurate when you are in normal mode. 
    7>Configurate RXFnSIDH and RXFnSIDL to resceive the specific ID. 
    8>Configurate RXMnSIDH and RXMnSIDL to mask the RXFnSID's ID. 
    9>Configurate CANINTE to enable or disable the interrupts. 
    10>Return to the normal mode and double check if it is return to the normal mode by CANSTAT. 
    */   
	
	//	MCP2515_REST1_GpioInit();  //��λ���ų�ʼ��		MTDCPU ���ӵĸ�λ��ʽ��Ӳ����Դֱ�Ӹ�λ
	
	MCP2515_INT1_GpioInit();	//�ж����ų�ʼ��
	
	MTDCPU_SPI1_Init();		//��ʼ��MCU��SPI����
	
	debugprintf("mpc2515_init!\r\n");
	
	/*Flip into the Configuration Mode*/ 
	do{
		 //MCP2515��λ,���ϵ��λʱ���������Զ���������ģʽ
		MTDCPU_CAN3_MCP2515_Reset();		//���͸�λָ�������λMCP2515
		res = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT) & REQOP_CONFIG ;
	}while(res != REQOP_CONFIG);	// �ж��Ƿ��������ģʽ

	
	//	res = MTDCPU_CAN3_MCP2515_SetControlRegister_Mode(REQOP_CONFIG); 	 //����Ϊ����ģʽ
	//	if(res == MCP2515_FAIL)	//����ʧ��ֱ�ӷ���
	//	{
	//		return res; 	/* function exit on error */
	//	}
	
	//����Ϊ����ģʽ
	MTDCPU_CAN3_MCP2515_Set_Register(CANCTRL,REQOP_CONFIG); 
	
	res = MTDCPU_CAN3_MCP2515_Config_Rate(CanBaudRate);		//���ò�����
	if(res == MCP2515_OK)
	{
		MTDCPU_CAN3_MCP2515_Init_CAN_Buffer();  //������Σ��˲������ͣ����ܼĴ���
		
		// enable both receive-buffers to receive messages
		// with std. and ext. identifiers
		// and enable rollover
		MTDCPU_CAN3_MCP2515_Register_BitModify(RXB0CTRL,RXM /*| BUKT*/,/*RXM_VALID_ALL*/RXM_RCV_ALL /*| BUKT_ROLLOVER*/);	//���ý��ܿ��ƼĴ���0,����ģʽ00�����ܹ���
		MTDCPU_CAN3_MCP2515_Register_BitModify(RXB1CTRL,RXM,/*RXM_VALID_ALL*/ RXM_RCV_ALL);								//���ý��ܿ��ƼĴ���1����������id
		//	MTDCPU_CAN3_MCP2515_Set_Register(RXB0CTRL,0x00);// ���뻺����0���ƼĴ���, �������з����˲������ı��ģ������ֹ
		//	MTDCPU_CAN3_MCP2515_Set_Register(RXB1CTRL,0x00);
		
		
		//�����ж�
		/*�����־�Ĵ��� ����ʹ�ܣ��ᵼ�½�������ж�*/
		//MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTE,MERRE | ERRIE | RX0IE /*| RX1IE*/,IVRE_ENABLED|ERRIE_ENABLED |RX0IE_ENABLED /*G_RXIE_ENABLED*/);  //0,1ȫ�������ж�ʹ��  //ʹ��ERRIE�������ж�ʹ��λ\���Ľ����ж�RX0IE\RX1IE
		MTDCPU_CAN3_MCP2515_Set_Register(CANINTE,/*MERRE|*/ERRIE|RX0IE/*|RX1IE|TX0IE|TX1IE|TX2IE*/);
		MTDCPU_CAN3_MCP2515_Set_Register(CANINTF,0x00);
		
		//��λMCP2515�Ĵ����־�Ĵ���
		MTDCPU_CAN3_MCP2515_Set_Register(EFLG,0x00);

		//�����������
		MTDCPU_CAN3_MCP2515_Set_Register(TXRTSCTRL,0x00);	// TXnRST��Ϊ�������ţ��Ƿ�����������
		MTDCPU_CAN3_MCP2515_Set_Register(BFPCTRL,0x00);		// RXnRST��ֹ���
	}

	//����Ϊ����ģʽ
	MTDCPU_CAN3_MCP2515_Set_Register(CANCTRL,REQOP_NORMAL); 
	do{
		res = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT) & REQOP_NORMAL ;
	}while( res != REQOP_NORMAL);

	//	res= MTDCPU_CAN3_MCP2515_Read_Register(CNF2);		//��ȡ�����ʼĴ���
	//	debugprintf("cnf2=0x%x\r\n",res);
	
	return res;		
				
}	


/** 
  * @brief  Reset the MCP2515 then U go into the configruation mode 
  * @param  none 
  * @retval none 
  */ 
void MTDCPU_CAN3_MCP2515_Reset(void)	     //MCP2515������λ
{
   MTDCPU_SPI1_CS_LOW();										/* Ƭѡ�����õ�ѡ��  */
   MTDCPU_SPI1_SendByte(SPICAN_RESET);		//���ͼĴ�����λ����
   MTDCPU_SPI1_CS_HIGH();									/* Ƭѡ�����ø߲�ѡ�� */
	 delay_ms(10);
}

/*��ȡ�Ĵ�����ֵ*/
unsigned char MTDCPU_CAN3_MCP2515_Read_Register(const unsigned char Addr)
{
    unsigned char Data;   
	
    MTDCPU_SPI1_CS_LOW();                //��MCP2515��CSΪ�͵�ƽ
    MTDCPU_SPI1_SendByte(SPICAN_READ);   //���Ͷ�����
    MTDCPU_SPI1_SendByte(Addr); 				 //���͵�ַ	
    Data = MTDCPU_SPI1_ReadByte();   		 //��һ�ֽ� 
    MTDCPU_SPI1_CS_HIGH();               //��MCP2515��CSΪ�ߵ�ƽ 
    return Data; 
}

/*********read the several register*********************/
void MTDCPU_CAN3_MCP2515_Read_RegisterS(const unsigned char Addr, unsigned char RegValues[], const unsigned char RegNum)
{
		unsigned char i;
		
	  MTDCPU_SPI1_CS_LOW();                //��MCP2515��CSΪ�͵�ƽ
		MTDCPU_SPI1_SendByte(SPICAN_READ);   //���Ͷ�����
    MTDCPU_SPI1_SendByte(Addr); 				 //���͵�ַ	
	
		// mcp2515 has auto-increment of address-pointer	
		for(i=0;i<RegNum;i++)
		{
			RegValues[i]=MTDCPU_SPI1_ReadByte();	//��ȡ����
		}
		MTDCPU_SPI1_CS_HIGH();               //��MCP2515��CSΪ�ߵ�ƽ 
}

//���üĴ�����ֵ
/** 
  * @brief  Send a byte to MCP2515 through SPI to return corresponding status you need 
  * @param  Add:Address of register 
  * @param  Data:the data U need to be sent from MCU to MCP2515 
  * @retval none 
  */ 
void MTDCPU_CAN3_MCP2515_Set_Register(const unsigned char Addr,const unsigned char Data)
{
	  MTDCPU_SPI1_CS_LOW();   						//ʹ��Ƭѡ     
    MTDCPU_SPI1_SendByte(SPICAN_WRITE); //����д����
    MTDCPU_SPI1_SendByte(Addr); 				//���͵�ַ				
    MTDCPU_SPI1_SendByte(Data); 				//д������
    MTDCPU_SPI1_CS_HIGH(); 							//�ر�Ƭѡ
}


void MTDCPU_CAN3_MCP2515_Set_RegisterS(const unsigned char Addr,const unsigned char SetValues[],const unsigned char RegNum) //���ö���Ĵ�����ֵ
{
		unsigned char i;
		MTDCPU_SPI1_CS_LOW();   						//ʹ��Ƭѡ
	  MTDCPU_SPI1_SendByte(SPICAN_WRITE); //����д����
    MTDCPU_SPI1_SendByte(Addr); 				//���͵�ַ	
	
		// mcp2515 has auto-increment of address-pointer
		for(i=0;i<RegNum;i++)
		{
			MTDCPU_SPI1_SendByte(SetValues[i]);//д������
		}
    MTDCPU_SPI1_CS_HIGH(); 							//�ر�Ƭѡ		
		
		//		unsigned char i;
		//		unsigned char address;
		//		address = Addr;
		//		for(i=0;i<RegNum;i++)
		//		{
		//			MTDCPU_CAN3_MCP2515_Set_Register(address,SetValues[i]);
		//			address ++;
		//		}
}


/** 
  * @brief  Modify the bit of the register 
  * @param  Add:Address of register 
  * @param  Mask:1-->U can modify the corresponding bit;0-->U can not modify the corresponding bit 
  * @param  Data:the data U need to be sent from MCU to MCP2515 
  * @retval none 
  */ 
void MTDCPU_CAN3_MCP2515_Register_BitModify(const unsigned char Addr,const unsigned char Mask,unsigned char Data) 
{ 
    MTDCPU_SPI1_CS_LOW();                			//ʹ��Ƭѡ
    MTDCPU_SPI1_SendByte(SPICAN_BIT_MODIFY); 	//����λ�޸�����
    MTDCPU_SPI1_SendByte(Addr); 							//���͵�ַ
    MTDCPU_SPI1_SendByte(Mask); 							//��������
    MTDCPU_SPI1_SendByte(Data); 							//д������
    MTDCPU_SPI1_CS_HIGH();    								//�ر�Ƭѡ
}

//��ȡ�Ĵ���״̬
unsigned char MTDCPU_CAN3_MCP2515_Read_Status(unsigned char cmd)
{
		unsigned char State;
		MTDCPU_SPI1_CS_LOW();                				//ʹ��Ƭѡ
		MTDCPU_SPI1_SendByte(/*SPICAN_READ_STATUS*/cmd); 	//���Ͷ�״̬����
		State = MTDCPU_SPI1_ReadByte();							//��ȡ״̬
		MTDCPU_SPI1_CS_HIGH();											//�ر�Ƭѡ
		
		return State;
}


 //��ȡ���ܵ�״̬
unsigned char MTDCPU_CAN3_MCP2515_ReadRx_Status(void)
{
		unsigned char RxState;
		MTDCPU_SPI1_CS_LOW();                				//ʹ��Ƭѡ
		MTDCPU_SPI1_SendByte(SPICAN_RX_STATUS); 		//���Ͷ�״̬����
		RxState = MTDCPU_SPI1_ReadByte();						//��ȡ״̬
		MTDCPU_SPI1_CS_HIGH();											//�ر�Ƭѡ
		
		return RxState;
}


/*
//���ÿ��ƼĴ��� �Ĺ���ģʽ
	����0�ɹ�������1����ʧ�ܡ�
*/
unsigned char MTDCPU_CAN3_MCP2515_SetControlRegister_Mode(const unsigned char NewMode)	
{
		unsigned char mode;
		MTDCPU_CAN3_MCP2515_Register_BitModify(CANCTRL,REQOP,NewMode);	//�޸�ģʽ
		
		//verify as advised in datasheet
		mode = MTDCPU_CAN3_MCP2515_Read_Register(CANCTRL);
		mode &= REQOP;
		if(mode == NewMode)
		{
			return MCP2515_OK;
		}
		else
		{
			return MCP2515_FAIL;
		}
}


/*
	����mcp2515 �Ĳ�����
	����0�ɹ�������1ʧ�ܡ�
     * The CAN bus bit time (tbit) is determined by:
     *   tbit = (SyncSeg + PropSeg + PS1 + PS2) * TQ
     * with:
     *     SyncSeg = 1
     *     sample point (between PS1 and PS2) must be at 60%-70% of the bit time
     *     PropSeg + PS1 >= PS2
     *     PropSeg + PS1 >= Tdelay
     *     PS2 > SJW
     *     1 <= PropSeg <= 8, 1 <= PS1 <=8, 2 <= PS2 <= 8
     * SJW = 1 is sufficient in most cases.z
     * Tdelay is usually 1 or 2 TQ.
     
*/
unsigned char MTDCPU_CAN3_MCP2515_Config_Rate(const unsigned int  BaudRate)
{
		unsigned char Set,cfg1,cfg2,cfg3;
	
	//unsigned char canctrl;
	//	{
	//		int tqs; /* tbit/TQ */
	//    int brp;
	//    int ps1, ps2, propseg, sjw;
	//		
	//	/* Determine the BRP value that gives the requested bit rate. */
	//    for(brp = 0; brp < 25; brp++) 
	//		{
	//			tqs = CANFOSC/*20M����*/ / (2 * (brp + 1)) / BaudRate;
	//			if (tqs >= 5 && tqs <= 25	&& (CANFOSC/*20M����*/ / (2 * (brp + 1)) / tqs) == BaudRate)
	//				break;
	//    }
	//		 /* The CAN bus bit time (tbit) is determined by:
	//     *   tbit = (SyncSeg + PropSeg + PS1 + PS2) * TQ
	//     * with:
	//     *     SyncSeg = 1
	//     *     sample point (between PS1 and PS2) must be at 60%-70% of the bit time
	//     *     PropSeg + PS1 >= PS2
	//     *     PropSeg + PS1 >= Tdelay
	//     *     PS2 > SJW
	//     *     1 <= PropSeg <= 8, 1 <= PS1 <=8, 2 <= PS2 <= 8
	//     * SJW = 1 is sufficient in most cases.z
	//     * Tdelay is usually 1 or 2 TQ.
	//     */
	//    propseg = ps1 = ps2 = (tqs - 1) / 3;

	//    if (tqs - (1 + propseg + ps1 + ps2) == 2)
	//			ps1++;
	//    if (tqs - (1 + propseg + ps1 + ps2) == 1)
	//			ps2++;
	//    sjw = 1;
	//		
	//		debugprintf("bit rate: BRP = %d, Tbit = %d TQ, PropSeg = %d, PS1 = %d, PS2 = %d, SJW = %d\n", \
	//	    brp, tqs, propseg, ps1, ps2, sjw);
	//		canctrl = MTDCPU_CAN3_MCP2515_Read_Register(CANCTRL);			//������ǰ״̬		whh
	//		MTDCPU_CAN3_MCP2515_SetControlRegister_Mode(REQOP_CONFIG);//�޸�ģʽΪ ����ģʽ��	whh
	//		
	//		MTDCPU_CAN3_MCP2515_Set_Register(CNF1,((sjw-1) << 6) | brp);		//���ò�����
	//		MTDCPU_CAN3_MCP2515_Set_Register(CNF2,BTLMODE | ((ps1-1) << 3) | (propseg-1));
	//		MTDCPU_CAN3_MCP2515_Register_BitModify(CNF3,PHSEG2, (ps2-1));
	//		
	//		MTDCPU_CAN3_MCP2515_Set_Register(CANCTRL,canctrl);	//����Ϊ֮ǰ��ģʽ		whh
	//		
	//		return MCP2515_OK;
	//	}
		
	
		Set = 0;
		
		switch (BaudRate)
		{
			case (MCP2515_CAN_BAUD_125K):
						cfg1 = MCP2515_20MHz_CAN_BAUD_125K_CFG1;
						cfg2 = MCP2515_20MHz_CAN_BAUD_125K_CFG2;
						cfg3 = MCP2515_20MHz_CAN_BAUD_125K_CFG3;
						Set =1;
						break;
			case	MCP2515_CAN_BAUD_500K:
						cfg1 = MCP2515_20MHz_CAN_BAUD_500K_CFG1;
						cfg2 = MCP2515_20MHz_CAN_BAUD_500K_CFG2;
						cfg3 = MCP2515_20MHz_CAN_BAUD_500K_CFG3;
						Set = 1;
						break;
			default :
						Set = 0;
						break;
		}
		
		if(Set)
		{
	//			canctrl = MTDCPU_CAN3_MCP2515_Read_Register(CANCTRL);			//������ǰ״̬		whh
	//			MTDCPU_CAN3_MCP2515_SetControlRegister_Mode(REQOP_CONFIG);//�޸�ģʽΪ ����ģʽ��	whh
			//Set the baudrate 				Fsoc=20Mhz
			//set CNF1,SJW=00,lengthe is 1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/20M=5us 
			MTDCPU_CAN3_MCP2515_Set_Register(CNF1,cfg1);		//���ò�����
			//set CNF2,SAM=0,caputre one time,PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ 
			MTDCPU_CAN3_MCP2515_Set_Register(CNF2,cfg2);
			//set CNF3,PHSEG2=(2+1)TQ=3TQ,when CANCTRL.CLKEN=1,set CLKOUT pin to output 
			MTDCPU_CAN3_MCP2515_Set_Register(CNF3,cfg3);

	//			MTDCPU_CAN3_MCP2515_Set_Register(CANCTRL,canctrl);	//����Ϊ֮ǰ��ģʽ		whh			
			return MCP2515_OK;
		}
		else
		{
			return MCP2515_FAIL;
		}
}

/*
	//��ȡ���յ���canid��ʶ
	���� �Ƿ�����չ֡��CAN_ID_STD��ʾ��׼��CAN_ID_EXT��ʾ��չ���Լ�canid	����������
*/
void MTDCPU_CAN3_MCP2515_Read_CAN_ID(const unsigned char RxIdReg_Addr, unsigned char* IDE_ExtFlag, unsigned int* Std_id,unsigned int * Ext_id)
{
		unsigned char  IdBufData[4];
		
		/*��ʼ��*/
		*IDE_ExtFlag = CAN_ID_STD;
		*Std_id	=	0;
		*Ext_id = 0;
	
		MTDCPU_CAN3_MCP2515_Read_RegisterS(RxIdReg_Addr,IdBufData,4);
		
		/*���ܵ��Ǳ�׼֡id*/
		*Std_id = (IdBufData[0]<<3) + (IdBufData[1]>>5);		//���ֲ�29ҳ��RXBnSIDH���������һ�����
	
		/*���ܵ�������չ֡*/
		if((IdBufData[1] & EXIDE_SET) == EXIDE_SET)	//˵������չ֡
		{
			// extended id
			*Ext_id = (*Std_id<<2) + (IdBufData[1] & 0x03);
			*Ext_id <<= 16;
		  *Ext_id = *Ext_id +((u16)IdBufData[2]<<8) + IdBufData[3];
      *IDE_ExtFlag = CAN_ID_EXT;		//����չ��ʶ��������չ��ʶ��
			*Std_id = 0;  								//����׼id����0
		}
}


	// Buffer can be RXBUF0 or RXBUF1 ����ȡ���ܵ�can��Ϣ����
void MTDCPU_CAN3_MCP2515_Read_CAN_Message(const unsigned char RxBuf_SIDH_Addr, CanRxMsg* Message)
{
		unsigned char RxReg_Addr, RxControlReg_Value;
	
		RxReg_Addr = RxBuf_SIDH_Addr;
		
		/*���id*/
		MTDCPU_CAN3_MCP2515_Read_CAN_ID(RxReg_Addr,&(Message->IDE),&(Message->StdId),&(Message->ExtId));	//�õ�id
		
		RxControlReg_Value = MTDCPU_CAN3_MCP2515_Read_Register(RxReg_Addr-1);		//�ö���ܿ��ƼĴ���ֵ
	
		Message->DLC = MTDCPU_CAN3_MCP2515_Read_Register(RxReg_Addr+4);					//��ȡ���ݳ���
	
		if(RxControlReg_Value & RXRTR_REMOTE/*0x08*/  || (Message->DLC &0x40) )		//˵����Զ��֡
		{
				Message->RTR = CAN_RTR_REMOTE;		//��ʾΪԶ��֡
		}
		else
		{
				Message->RTR = CAN_RTR_DATA;			//��ʾΪ����֡
		}
		
		Message->DLC &= 0x0F;   //�Ĵ����ĵ�4λ���������ݳ���
		
		MTDCPU_CAN3_MCP2515_Read_RegisterS(RxReg_Addr+5,&(Message->Data[0]),Message->DLC);		//��ȡ����
}


/*
//д��Ҫ���͵�canid
IDE_ExtFlag =CAN_ID_EXT Ϊ��չ֡
*/
void MTDCPU_CAN3_MCP2515_Write_CAN_ID(const unsigned char TxIdReg_Addr, const unsigned char IDE_ExtFlag,const unsigned int can_id)
{
		unsigned int canid;
		unsigned char IdBufData[4];
		
		if(IDE_ExtFlag ==CAN_ID_EXT)		//��ʾΪ��չ֡
		{
			canid =can_id;
			IdBufData[3] = (unsigned char )(canid & 0xFF);				//EID0
			IdBufData[2] = (unsigned char )(canid >> 8)& 0xFF;		//EID8
			
			canid >>= 16;		//��ø�16λ
			
			IdBufData[1] = (unsigned char )(canid & 0x03);
			IdBufData[1] +=(unsigned char )((canid & 0x1C)<<3);
			IdBufData[1] |= EXIDE_SET;
			IdBufData[0] = (unsigned char )(canid >> 5)&0xFF;
		}
		else
		{
			canid = (unsigned short)(can_id & 0xFFFF);
			
			IdBufData[0] = (unsigned char )(canid >>3)&0xFF;
			IdBufData[1] = (unsigned char )((canid &0x07) <<5);		//W H H ����һ�����ţ�����id���� 
			IdBufData[2] = 0;
			IdBufData[3] = 0;
		}
		
		MTDCPU_CAN3_MCP2515_Set_RegisterS(TxIdReg_Addr,IdBufData,4);		//��idд��Ĵ���
}

/*
	д��Ҫ���͵�can��Ϣ   
// Buffer can be TXBUF0 TXBUF1 or TXBUF2
*/
void MTDCPU_CAN3_MCP2515_Write_CAN_Message(const unsigned char TxBuf_SIDH_Addr, const CanTxMsg* Message)
{
		unsigned char TxReg_Addr, Dlc;		//��ַ�����ݵĳ���
	
		TxReg_Addr = TxBuf_SIDH_Addr;
		Dlc = Message->DLC;
		
		MTDCPU_CAN3_MCP2515_Set_RegisterS(TxReg_Addr+5,&(Message->Data[0]),Dlc);		//��Ĵ���д������
		
		if(Message->IDE == CAN_ID_EXT)	//��չ֡
		{
				MTDCPU_CAN3_MCP2515_Write_CAN_ID(TxReg_Addr,Message->IDE,Message->ExtId);		//д��canid
		}
		else		//��׼֡
		{
				MTDCPU_CAN3_MCP2515_Write_CAN_ID(TxReg_Addr,Message->IDE,Message->StdId);
		}
		
		if(Message->RTR ==	CAN_RTR_REMOTE )		//��ʾԶ��֡
		{
			Dlc |= 0x40;		//�ֲ�21ҳ
		}
		
		MTDCPU_CAN3_MCP2515_Set_Register((TxReg_Addr+4),Dlc);		//д��RTR��DLC
		MTDCPU_CAN3_MCP2515_Set_Register(TxBuf_SIDH_Addr-1,TXREQ );	//������  //WHH
}		


/*
 ** Start the transmission from one of the tx buffers.
// Buffer can be TXBUF0 TXBUF1 or TXBUF2
 */
void MTDCPU_CAN3_MCP2515_Start_Transimt(const unsigned char TxBuf_SIDH_Addr)
{
		MTDCPU_CAN3_MCP2515_Register_BitModify(TxBuf_SIDH_Addr-1,TXREQ,TXREQ);
}


/*
	��� һ���յ������ͻ�����
*/
unsigned char MTDCPU_CAN3_MCP2515_Get_Free_TxBuf(unsigned char * TxBufNum)
{
		unsigned char TempState, i, ControlRegValue;
		unsigned char ControlReg[3] ={TXB0CTRL,TXB1CTRL,TXB2CTRL};
		
		TempState = MCP2515_ALLTXBUSY;		//��ʼ�����ͻ�����״̬�����еķ��ͻ�������	
		*TxBufNum = 0x00;
		
		// check all 3 TX-Buffers
		for(i=0;i<3;i++)
		{
			ControlRegValue = MTDCPU_CAN3_MCP2515_Read_Register(ControlReg[i]);		//��ȡ���ƼĴ�����ֵ
			if((ControlRegValue & TXREQ )== 0)
			{
				//debugprintf("Selected TX-Buffer %d\r\n", i+1);
				*TxBufNum = ControlReg[i]+1; 	// return SIDH-address of Buffer
				TempState = MCP2515_OK;
				return TempState;
			}
			
		}
		
//		/*WHH*/
//		ControlRegValue = MTDCPU_CAN3_MCP2515_Read_Status(SPICAN_READ_STATUS);
//		
//		if (!(ControlRegValue & CAN_STATE_TX0REQ)) 
//		{
//        *TxBufNum = ControlReg[0];
//				TempState = MCP2515_OK;		
//    }
//		else if (!(ControlRegValue & CAN_STATE_TX1REQ)) 
//		{
//        *TxBufNum = ControlReg[1];
//				TempState = MCP2515_OK;
//    }
//		else if (!(ControlRegValue & CAN_STATE_TX2REQ))
//		{
//        *TxBufNum = ControlReg[2];
//				TempState = MCP2515_OK;	 
//    }		
		
		return TempState;
}


/*
//�������μĴ������˲��Ĵ�������ʼ�����ͻ�����

*/
void MTDCPU_CAN3_MCP2515_Init_CAN_Buffer(void)
{
		unsigned char i,TxContr1,TxContr2,TxContr3;
	
		/*whh �˲��������μĴ���ֻ��������ģʽ�����ã�Ϊ�˷�ֹ��������ģʽ��*/
		//		unsigned char canctrl;
		//		canctrl = MTDCPU_CAN3_MCP2515_Read_Register(CANCTRL);			//������ǰ״̬		whh
		//		MTDCPU_CAN3_MCP2515_SetControlRegister_Mode(REQOP_CONFIG);//�޸�ģʽΪ ����ģʽ��	whh	
	
		// TODO: check why this is needed to receive extended and standard frames
		// Mark all filter bits as don't care:  	���μĴ���ȫ��Ϊ0���������е�canid
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXM0SIDH,0,0);	//��ʼ���������μĴ���0��Ϊ0��������id
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXM1SIDH,0,0);	//��ʼ���������μĴ���1

		// Anyway, set all filters to 0:		/*���ܻ�����0 */
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXF0SIDH,CAN_ID_EXT,0);	// RXB0: extended 
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXF1SIDH,0,0);	//    AND standard
	
																				/*���ܻ�����1���˲��Ĵ�������*/
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXF2SIDH, CAN_ID_EXT, 0); // RXB1: extended 
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXF3SIDH, 0, 0); //  AND standard
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXF4SIDH, 0, 0);
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXF5SIDH, 0, 0);
		
	// Clear, deactivate the three transmit buffers		��գ�ͣ���������ͻ�����
	// TXBnCTRL -> TXBnD7
	
		TxContr1 = TXB0CTRL;
		TxContr2 = TXB1CTRL;
		TxContr3 = TXB2CTRL;
		for(i=0;i<14;i++)				// in-buffer loop
		{
			MTDCPU_CAN3_MCP2515_Set_Register(TxContr1,0);
			MTDCPU_CAN3_MCP2515_Set_Register(TxContr2,0);
			MTDCPU_CAN3_MCP2515_Set_Register(TxContr3,0);
			
			TxContr1++;
			TxContr2++;
			TxContr3++;
		}
		//		MTDCPU_CAN3_MCP2515_Set_Register(CANCTRL,canctrl);	//����Ϊ֮ǰ��ģʽ		whh
		
		// and clear, deactivate the two receive buffers. ��գ�ͣ���������ܻ�����
		MTDCPU_CAN3_MCP2515_Set_Register(RXB0CTRL,0);
		MTDCPU_CAN3_MCP2515_Set_Register(RXB1CTRL,0);
}

/*
�жϴ�����λMCP25515оƬ
*/
void MTDCPU_CAN3_MCP2515_IntError_Init(const unsigned int  BaudRate)
{
	unsigned char res;

	/*Flip into the Configuration Mode*/ 
	do{
		 //MCP2515��λ,���ϵ��λʱ���������Զ���������ģʽ	
	
		//MTDCPU_CAN3_MCP2515_Reset();		//���͸�λָ�������λMCP2515
		MTDCPU_SPI1_CS_LOW();										/* Ƭѡ�����õ�ѡ��  */
		MTDCPU_SPI1_SendByte(SPICAN_RESET);			//���ͼĴ�����λ����
		MTDCPU_SPI1_CS_HIGH();									/* Ƭѡ�����ø߲�ѡ�� */
		
		res = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT) & REQOP_CONFIG ;
		debugprintf("bbvbvb!\r\n");
	}while(res != REQOP_CONFIG);	// �ж��Ƿ��������ģʽ

	res = MTDCPU_CAN3_MCP2515_Config_Rate(BaudRate);		//���ò�����
	if(res == MCP2515_OK)
	{
		//MTDCPU_CAN3_MCP2515_Init_CAN_Buffer();  //������Σ��˲������ͣ����ܼĴ���
		// TODO: check why this is needed to receive extended and standard frames
		// Mark all filter bits as don't care:  	���μĴ���ȫ��Ϊ0���������е�canid
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXM0SIDH,0,0);	//��ʼ���������μĴ���0��Ϊ0��������id
		MTDCPU_CAN3_MCP2515_Write_CAN_ID(RXM1SIDH,0,0);	//��ʼ���������μĴ���1

		// and clear, deactivate the two receive buffers. ��գ�ͣ���������ܻ�����
		MTDCPU_CAN3_MCP2515_Set_Register(RXB0CTRL,0);
		MTDCPU_CAN3_MCP2515_Set_Register(RXB1CTRL,0);
		
		// enable both receive-buffers to receive messages
		// with std. and ext. identifiers
		// and enable rollover
		MTDCPU_CAN3_MCP2515_Register_BitModify(RXB0CTRL,RXM /*| BUKT*/,/*RXM_VALID_ALL*/RXM_RCV_ALL /*| BUKT_ROLLOVER*/);	//���ý��ܿ��ƼĴ���0,����ģʽ00�����ܹ���
		MTDCPU_CAN3_MCP2515_Register_BitModify(RXB1CTRL,RXM,/*RXM_VALID_ALL*/ RXM_RCV_ALL);								//���ý��ܿ��ƼĴ���1����������id
		//	MTDCPU_CAN3_MCP2515_Set_Register(RXB0CTRL,0x00);// ���뻺����0���ƼĴ���, �������з����˲������ı��ģ������ֹ
		//	MTDCPU_CAN3_MCP2515_Set_Register(RXB1CTRL,0x00);
		
		//enable recive interrupt
		//MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTE,MERRE | ERRIE | RX0IE /*| RX1IE*/,IVRE_ENABLED|ERRIE_ENABLED |RX0IE_ENABLED /*G_RXIE_ENABLED*/);  //0,1ȫ�������ж�ʹ��  //ʹ��ERRIE�������ж�ʹ��λ\���Ľ����ж�RX0IE\RX1IE
		MTDCPU_CAN3_MCP2515_Set_Register(CANINTE,/*MERRE|*/ERRIE|RX0IE/*|RX1IE*/);
		MTDCPU_CAN3_MCP2515_Set_Register(CANINTF,0x00);
		MTDCPU_CAN3_MCP2515_Set_Register(EFLG,0x00);

		/*W H H */
		MTDCPU_CAN3_MCP2515_Set_Register(TXRTSCTRL,0x00);	// TXnRST��Ϊ�������ţ��Ƿ�����������
		/*W H H */
		MTDCPU_CAN3_MCP2515_Set_Register(BFPCTRL,0x00);		// RXnRST��ֹ���
	}
	
		MTDCPU_CAN3_MCP2515_Set_Register(CANCTRL,REQOP_NORMAL); 	//����Ϊ����ģʽ
	do{
		res = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT) & REQOP_NORMAL ;
	}while( res != REQOP_NORMAL);
}


/*
�жϴ���Ĵ�����
*/
void Pro_CAN3_ERROR(void)
{
	unsigned char EflgState;
	EflgState=MTDCPU_CAN3_MCP2515_Read_Register( EFLG ); // �������־�Ĵ������жϴ�������
	if( EflgState & EWARN )	 // ���󾯸�Ĵ�������TEC��REC���ڻ����96ʱ��1
	{
		debugprintf("31\r\n");
		 MTDCPU_CAN3_MCP2515_Set_Register(TEC, 0);
		 MTDCPU_CAN3_MCP2515_Set_Register(REC, 0);
	}
	if( EflgState & RXWAR )		  // ��REC���ڻ����96ʱ��1
	{debugprintf("RXWAR ������������״̬\r\n");;}
	if( EflgState & TXWAR )		  // ��TEC���ڻ����96ʱ��1
	{debugprintf("TXWAR ������������״̬\r\n");;}
	if( EflgState & RXEP )		  // ��REC���ڻ����128ʱ��1
	{
		debugprintf("RXEP���ڱ�������״̬\r\n");
		MTDCPU_CAN3_MCP2515_Set_Register(REC, 0);
	}
	if( EflgState & TXEP )		  // ��TEC���ڻ����128ʱ��1
	{
		debugprintf("TXEP���ڱ�������״̬\r\n");
		MTDCPU_CAN3_MCP2515_Set_Register(TEC, 0);
	}
	if( EflgState & TXBO )      // ��TEC���ڻ����255ʱ��1
	{
		debugprintf("MCP2515�������߹ر�״̬\r\n");
		MTDCPU_CAN3_MCP2515_IntError_Init(MCP2515_CAN_BAUD_500K);//��λCAN3  //W HH
	}   
	if( EflgState & RX0OVR )		      // ���ջ�����0���
	{
		 debugprintf("���ջ�����0���\r\n");
		 MTDCPU_CAN3_MCP2515_Register_BitModify( EFLG, RX0OVR,0 ); 
		// ���жϱ�־������ʵ���������һ�ִ���취�Ƿ���Զ���壬�����������·���
	}
	if( EflgState & RX1OVR )			 // ���ջ�����1���
	{
		debugprintf("���ջ�����1���\r\n");
		MTDCPU_CAN3_MCP2515_Register_BitModify( EFLG, RX1OVR,0 ); // ���жϱ�־;
	}	
	
	MTDCPU_CAN3_MCP2515_Set_Register(EFLG,0x00);	//WHH ���EFLG�Ĵ���
}	


/*����can3��Ϣ*/
unsigned char MCP2515_CAN3_Receive_Message(CanRxMsg* msg)
{
	unsigned char state=0, res=CAN_NOMSG,SetMode=0/*,canstate=0*/;
	
	//	state = MTDCPU_CAN3_MCP2515_Read_Register(CANINTF);
	//	//debugprintf("TF=0x%x\r\n",state);

	//	if(state == 0x00)
	//	{
	//		//CanRxMsg msg;
	//		debugprintf("000!\r\n");
	//		
	//	//		canstate = MTDCPU_CAN3_MCP2515_Read_Register(TEC);
	//	//		debugprintf("TEC=0x%x\r\n",canstate);
	//			
	//	//		canstate = MTDCPU_CAN3_MCP2515_Read_Register(REC);
	//	//		debugprintf("REC=0x%x\r\n",canstate);
	//		canstate= MTDCPU_CAN3_MCP2515_Read_Register(CNF2);
	//		debugprintf("cnf2=0x%x\r\n",canstate);
	//		
	//		res= CAN_NOMSG;
	//		return res;
	//	//		
	//	//		canstate = MTDCPU_CAN3_MCP2515_Read_Register(CANCTRL);
	//	//		debugprintf("stat=0x%x\r\n",canstate);
	//	//		
	//	//		MTDCPU_CAN3_MCP2515_Read_CAN_Message(RXB0SIDH,&msg);
	//	//		printf("l=%d\r\n",msg.DLC);
	//	//		for(i=0;i<msg.DLC;i++)
	//	//		{
	//	//			debugprintf("a%d=0x%x\r\n",i,msg.Data[i]);
	//	//		}
	//	//		canstate = MTDCPU_CAN3_MCP2515_Read_Register(RXB1D0);
	//	//		printf("RDO=%d\r\n",canstate);
	//			
	//	//		canstate = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT);
	//	//		debugprintf("stat=0x%x\r\n",state);
	//	//	
	//	//		can_eflg = MTDCPU_CAN3_MCP2515_Read_Register(EFLG);
	//	//		debugprintf("eflg=0x%x\r\n",state);

	//	//		state = MTDCPU_CAN3_MCP2515_Read_Status(SPICAN_READ_STATUS);
	//	//		debugprintf("can3 int SR=0x%x\r\n",state);
	//	//		
	//	//		for(i=0;i<9;i++)
	//	//		{
	//	//			state = MTDCPU_CAN3_MCP2515_Read_Register(CANINTF);
	//	//			debugprintf("TF=0x%x\r\n",state);
	//	//		}
	//			
	//			//if(state ==0x00)
	//	//		{
	//	//				//MTDCPU_CAN3_MCP2515_IntError_Init(MCP2515_CAN_BAUD_125K);	//��λCAN3  //W HH,���ﲻһ��Ҫ��λ
	//	//				//printf("err errr errr can3 err int !\r\n");
	//	//		}
	//	}
	
	while(1)
	{
		state = MTDCPU_CAN3_MCP2515_Read_Register(CANINTF);
		if(state == 0x00)	//�������������ж�
		{
			break;
		}
		
		if(state & MERRF)  //���Ĵ����־λ
		{
			debugprintf("MERRF���Ĵ����жϣ�\r\n");
							//MTDCPU_CAN3_MCP2515_IntError_Init(MCP2515_CAN_BAUD_500K);	//��λCAN3  //W HH  /*WHH*/
			MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTF,MERRF,0);	// �屨�Ĵ����жϱ�־
		}
		
		if(state & WAKIF) // �����ж�
		{		
			debugprintf("WAKIF�����жϣ�\r\n");
	
			MTDCPU_CAN3_MCP2515_SetControlRegister_Mode(REQOP_NORMAL);		// ���Ѻ��ڽ�����ģʽ�������ý�����������ģʽ
			do{
				SetMode = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT) & REQOP_NORMAL ;
			}while( SetMode != REQOP_NORMAL);
	
			MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTF,WAKIF,0);	// �廽���жϱ�־
		}
		
		if(state & ERRIF) // �����ж�
		{
			debugprintf("ERRIF�����жϣ���\r\n");
			Pro_CAN3_ERROR( );   //�ֱ����������
			MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTF,ERRIF,0);	// ������жϱ�־
						//MTDCPU_CAN3_MCP2515_IntError_Init(MCP2515_CAN_BAUD_500K);	//��λCAN3 whh
		}

		if(state & TX2IF)  // ����2�ɹ��ж�
		{
			//debugprintf("����2�ɹ��ж�\r\n");
			MTDCPU_CAN3_MCP2515_Register_BitModify( CANINTF, TX2IF,0 ); // ���жϱ�־
		}
		if(state & TX1IF)  // ����1�ɹ��ж�
		{
			//debugprintf("����1�ɹ��ж�\r\n");
			MTDCPU_CAN3_MCP2515_Register_BitModify( CANINTF, TX1IF,0 );// ���жϱ�־
		}
		if(state & TX0IF)    // ����0�ɹ��ж�
		{
			//debugprintf("����0�ɹ��ж�\r\n");
			MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTF, TX0IF,0);  // ���жϱ�־
		}		
		
		if(state & RX0IF) 
		{
			//Msg in Buffer 0
			//debugprintf("Rx0 �ж�!\r\n");
			MTDCPU_CAN3_MCP2515_Read_CAN_Message(RXB0SIDH,msg);					//��ȡ���յ�����Ϣ
			MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTF,RX0IF,0);		//����ж�����־λ
			res= CAN_OK;
		}
		
		if(state & RX1IF)
		{
			// Msg in Buffer 1
			//debugprintf("Rx1 �ж�!\r\n");
			MTDCPU_CAN3_MCP2515_Read_CAN_Message(RXB1SIDH,msg);
			MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTF,RX1IF,0);		//����ж�����־λ
			res= CAN_OK;
		}

	}
		
		//	state = MTDCPU_CAN3_MCP2515_Read_Register(CANINTF);
		//	debugprintf("TF=0x%x\r\n",state);
		//		
		//	canstate = MTDCPU_CAN3_MCP2515_Read_Register(CANSTAT);
		//	debugprintf("stat=0x%x\r\n",state);	
		//	/*WHH ���������жϣ�ʹ���ж�*/
		//	MTDCPU_CAN3_MCP2515_Set_Register(CANINTE,0);		//�ر������ж�
		//	MTDCPU_CAN3_MCP2515_Register_BitModify(CANINTE,ERRIE | RX0IE | RX1IE,ERRIE_ENABLED | G_RXIE_ENABLED);  //0,1ȫ�������ж�ʹ��  //ʹ��ERRIE�������ж�ʹ��λ\���Ľ����ж�RX0IE\RX1IE
		//	MTDCPU_CAN3_MCP2515_Set_Register(CANINTF,0x00);
		//	MTDCPU_CAN3_MCP2515_Set_Register(EFLG,0x00);
				
		//	MTDCPU_CAN3_MCP2515_Set_Register(CANINTE,0);		//�ر������ж�
		//	MTDCPU_CAN3_MCP2515_Set_Register(CANINTE,MERRE|ERRIE|RX0IE/*|RX1IE*/);
	
		MTDCPU_CAN3_MCP2515_Set_Register(CANINTF,0x00);
		MTDCPU_CAN3_MCP2515_Set_Register(EFLG,0x00);
	
	return res;	
}

/* returns either 
 #define CAN_MSGAVAIL   (3) - a message has been received
 #define CAN_NOMSG      (4) - no new message
*/
unsigned char MCP2515_CAN3_Check_Receive(void)
{
	unsigned char res;
	res = MTDCPU_CAN3_MCP2515_Read_Status(SPICAN_READ_STATUS);	// RXnIF in Bit 1 and 0
	
	if(res &( RX0IF|RX1IF))
	{
		return CAN_MSGAVAIL;
	}
	else
	{
		return CAN_NOMSG;
	}
	
}

/* checks Controller-Error-State, returns CAN_OK or CAN_CTRLERROR 
   only errors (and not warnings) lead to "CTRLERROR" */
unsigned char MCP2515_CAN3_Check_Error(void)
{
	unsigned char eflg = MTDCPU_CAN3_MCP2515_Read_Register(EFLG);

	if ( eflg &  (0xF8) /* 5 MS-Bits */ ) 
	{
		return CAN_CTRLERROR;
	}
	else
	{
		return CAN_OK;
	}

}

/*�жϽ��ܺ���*/
void MCP2515_CAN3_Interrupt(CanRxMsg *msg)
{
	//	CanRxMsg *tempmsg;
	//	unsigned char state=CAN_FAIL;
	//	tempmsg = msg;
	//	state = MCP2515_CAN3_Receive_Message(tempmsg);
	
}

