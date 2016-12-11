#include "MTD_REP_CAN3_MCP2515.h"
#include "MCP2515_BitTime.h"
#include "Uart.h"
#include "delay.h"
#include "os.h"
#include "string.h"


u8 MCP2515_CAN3_PORT	=0x03;
/*RXB0's data registers:*/ 
unsigned char RXB0D[8]={RXB0D0,RXB0D1,RXB0D2,RXB0D3,RXB0D4,RXB0D5,RXB0D6,RXB0D7}; 
/*TXB0's data registers:*/ 
unsigned char TXB0D[8]={TXB0D0,TXB0D1,TXB0D2,TXB0D3,TXB0D4,TXB0D5,TXB0D6,TXB0D7};

#define MCP2515_INT1_RCC           RCC_AHB1Periph_GPIOE
#define MCP2515_INT1_GPIO          GPIOE
#define MCP2515_INT1_GPIO_PIN      GPIO_Pin_11
#define MCP2515_INT1_EXIT_PORT		 EXTI_PortSourceGPIOE
#define MCP2515_INT1_EXIT_PIN			 EXTI_PinSource11

#define MCP2515_REST1_RCC          RCC_AHB1Periph_GPIOE
#define MCP2515_REST1_GPIO         GPIOE
#define MCP2515_REST1_GPIO_PIN     GPIO_Pin_12
#define MCP2515_REST1()						 GPIO_ResetBits(MCP2515_REST1_GPIO,MCP2515_REST1_GPIO_PIN)
#define MCP2515_NOREST1()					 GPIO_SetBits(MCP2515_REST1_GPIO,MCP2515_REST1_GPIO_PIN)

void MCP2515_REST1_GpioInit(void)
{
	/*rest Gpio init*/
	  GPIO_InitTypeDef  GPIO_InitStructure;
   
		RCC_AHB1PeriphClockCmd( MCP2515_REST1_RCC , ENABLE); 
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Pin = MCP2515_REST1_GPIO_PIN;
		GPIO_Init(MCP2515_REST1_GPIO, &GPIO_InitStructure);
	
		MCP2515_REST1();		//��ʼ��֮�������λ
		delay_ms(50);
		MCP2515_NOREST1();

}


void	MCP2515_INT1_GpioInit(void)
{
  EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  RCC_AHB1PeriphClockCmd(MCP2515_INT1_RCC, ENABLE); 		//ʹ��IOʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          //ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //�����Ͻ�
  GPIO_InitStructure.GPIO_Pin = MCP2515_INT1_GPIO_PIN;  //IO��Ϊ11
  GPIO_Init(MCP2515_INT1_GPIO, &GPIO_InitStructure);    //��������gpio

  SYSCFG_EXTILineConfig(MCP2515_INT1_EXIT_PORT, MCP2515_INT1_EXIT_PIN);//ʵʼ���ж���11

  EXTI_InitStructure.EXTI_Line = EXTI_Line11;                   //�����ж���Ϊ�ж���11
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;          //�����ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;      //����Ϊ�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                    //�����ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);                              

  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;             
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F; 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


void MCP2515_INT1_IRQ(void)
{
	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
		CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
		CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
		CPU_CRITICAL_EXIT();	//���ж�
	#endif	

    if(EXTI_GetITStatus(EXTI_Line11) != RESET)
		{
			CanRxMsg frame;
			u8 i;
			printf("can3,rec int!\r\n");
	
			memset(&frame,0,sizeof(CanRxMsg));
			
				MCP2515_CAN3_Interrupt(&frame);
			
        EXTI_ClearITPendingBit(EXTI_Line11);
			
			for(i=0;i<8 ;i++)
			{
				printf("can3[%d]=0x%x\r\n",i,frame.Data[i]);
			}
    }
				
	#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
		OSIntExit();    	//�˳��ж�
	#endif
}



unsigned char MTDREP_CAN3_MCP2515_Init(const unsigned char CanBaudRate)
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
	
	MCP2515_REST1_GpioInit();  //��λ���ų�ʼ��
	
	MCP2515_INT1_GpioInit();	//�ж����ų�ʼ��
	
	MTDREP_SPI1_Init();		//��ʼ��MCU��SPI����
	
	debugprintf("mpc2515_init!\r\n");
	
	/*Flip into the Configuration Mode*/ 
	MTDREP_CAN3_MCP2515_Reset();						//���͸�λָ�������λMCP2515
	
	res = MTDREP_CAN3_MCP2515_SetControlRegister_Mode(REQOP_CONFIG); 	 //����Ϊ����ģʽ
	if(res == MCP2515_FAIL)	//����ʧ��ֱ�ӷ���
	{
		return res; 	/* function exit on error */
	}
	
	res = MTDREP_CAN3_MCP2515_Config_Rate(CanBaudRate);		//���ò�����
	if(res == MCP2515_OK)
	{
		MTDREP_CAN3_MCP2515_Init_CAN_Buffer();  //������Σ��˲������ͣ����ܼĴ���
		
		// enable both receive-buffers to receive messages
		// with std. and ext. identifiers
		// and enable rollover
		MTDREP_CAN3_MCP2515_Register_BitModify(RXB0CTRL,RXM | BUKT,RXM_VALID_ALL | BUKT_ROLLOVER);	//���ý��ܿ��ƼĴ���0,����ģʽ00�����ܹ���
		
		MTDREP_CAN3_MCP2515_Register_BitModify(RXB1CTRL,RXM,RXM_VALID_ALL);								//���ý��ܿ��ƼĴ���1����������id
		
		//enable recive interrupt
		MTDREP_CAN3_MCP2515_Register_BitModify(CANINTE,RX0IE | RX1IE,G_RXIE_ENABLED);  //0,1ȫ�������ж�ʹ��
	}
	
	res = MTDREP_CAN3_MCP2515_SetControlRegister_Mode(REQOP_NORMAL);		//����Ϊ����ģʽ
	if(res == MCP2515_FAIL)
	{
		return res;/* function exit on error */
	}
	return res;
	
	
//	//Set the baudrate 				Fsoc=20Mhz
//	//set CNF1,SJW=00,lengthe is 1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/20M=5us 
//	SPISendByte_MCP2515(CNF1,CAN_125Kbps); 
//		
//	//set CNF2,SAM=0,caputre one time,PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ 
//	SPISendByte_MCP2515(CNF2,0x80|PHSEG1_7TQ|PRSEG_2TQ);	
//	
//	//set CNF3,PHSEG2=(2+1)TQ=3TQ,when CANCTRL.CLKEN=1,set CLKOUT pin to output 
//  SPISendByte_MCP2515(CNF3,PHSEG2_6TQ); 
	
//	/* 
//    3 Transimit Buffers:TXB0,TXB1,TXB2 
//    2 Receive   Buffers:RXB0,RXB1 
//    6 ID Filter:RXF0~RXF5 
//    2 ID Mask Regieter:RXM0,RXM1 
//  */ 
//	
//	//Configurate TXB0's ID and DLC registers 
//    SPISendByte_MCP2515(TXB0SIDH,0x00);//Standard ID 	//���ͻ�����0��׼��ʶ����λ
//    SPISendByte_MCP2515(TXB0SIDL,0x00);//Standard ID 	//���ͻ�����0��׼��ʶ����λ
////    SPISendByte_MCP2515(TXB0DLC,DLC_1);//DLC 
////		SPISendByte_MCP2515(TXB0EID8,0xFF);//���ͻ�����0��չ��ʶ����λ
////		SPISendByte_MCP2515(TXB0EID0,0xFF);//���ͻ�����0��չ��ʶ����λ
//	
//	//configurate RXB0' registers 
//		SPISendByte_MCP2515(RXB0SIDH,0x00);//��ս��ջ�����0�ı�׼��ʶ����λ
//		SPISendByte_MCP2515(RXB0SIDL,0x00);//��ս��ջ�����0�ı�׼��ʶ����λ
//		SPISendByte_MCP2515(RXB0CTRL,0x60);//Receive all frames from CAN bus 
////    SPISendByte_MCP2515(RXB0CTRL,0x20);//RXB0 just receive the standard frames  //�������ձ�׼��ʶ������Ч��Ϣ
//		SPISendByte_MCP2515(RXB0DLC,DLC_8);//���ý������ݵĳ���Ϊ8���ֽ�
//		
//    SPISendByte_MCP2515(RXF0SIDH,0x00);//Fileter register 						//���������˲��Ĵ���n��׼��ʶ����λ
//    SPISendByte_MCP2515(RXF0SIDL,0x00);//Just receive ID=0x7FF frame 	//���������˲��Ĵ���n��׼��ʶ����λ
//    SPISendByte_MCP2515(RXF1SIDH,0x00);//Fileter register 						
//    SPISendByte_MCP2515(RXF1SIDL,0x00);//Just receive ID=0x7EE frame 	

//		SPISendByte_MCP2515(RXM0SIDH,0x00);//Mask register 								//�����������μĴ���n��׼��ʶ����λ
//    SPISendByte_MCP2515(RXM0SIDL,0x00); 															//�����������μĴ���n��׼��ʶ����λ	
// 	
//  /*Set the RXB0 or RXB1 interrupt enableling*/ 
//		SPISendByte_MCP2515(CANINTF,0x00);//���CAN�жϱ�־�Ĵ���������λ(������MCU���)
//		SPISendByte_MCP2515(CANINTE,RX0IE);//Enable RXB0 interrupt		//����CAN�ж�ʹ�ܼĴ����Ľ��ջ�����0���ж�ʹ��,����λ��ֹ�ж�
//	
//    SPISendByte_MCP2515(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);//Go into the normal mode //��MCP2515����Ϊ����ģʽ,�˳�����ģʽ
//    
//		dummy=SPIReadByte_MCP2515(CANSTAT);//��ȡCAN״̬�Ĵ�����ֵ    
//		if (OPMODE_NORMAL != (dummy & 0xE0)) //�ж�MCP2515�Ƿ��Ѿ���������ģʽ
//    {
//				SPISendByte_MCP2515(CANCTRL,REQOP_NORMAL | CLKOUT_ENABLED);//If normal ok //�ٴν�MCP2515����Ϊ����ģʽ,�˳�����ģʽ       	
//		}	
}	

////CAN�������ⳤ���ֽ�
//void CAN_Send_anylength(u8 *CAN_TX_Buf,u8 length1)
//{
//     
//  unsigned char tempdata,j;
//  tempdata=SPIReadByte_MCP2515(SPICAN_READ_STATUS);
//  SPISendByte_MCP2515(TXB0DLC,length1);
//  for(j=0;j<length1;j++)
//  {
//    SPISendByte_MCP2515(TXB0D0+j,CAN_TX_Buf[j]);
//  }

//  if(tempdata&0x04)//�ж�TXREQ��־λ
//  {
//    delay_ms(1);
//    SPISendByte_MCP2515(TXB0CTRL,0);//���TXREQ��־λ
//    while(SPIReadByte_MCP2515(SPICAN_READ_STATUS)&0x04);//�ȴ�TXREQ����
//  }
//  MTDCPU_SPI_CS_LOW();
//  MTDCPU_SPI1_SendByte(SPICAN_RTS_TXB0);//���ͻ�����0������
//  MTDCPU_SPI_CS_HIGH();
//}



/** 
  * @brief  Reset the MCP2515 then U go into the configruation mode 
  * @param  none 
  * @retval none 
  */ 
void MTDREP_CAN3_MCP2515_Reset(void)	     //MCP2515������λ
{
   MTDREP_SPI1_CS_LOW();										/* Ƭѡ�����õ�ѡ��  */
   MTDREP_SPI1_SendByte(SPICAN_RESET);		//���ͼĴ�����λ����
   MTDREP_SPI1_CS_HIGH();									/* Ƭѡ�����ø߲�ѡ�� */
	 delay_ms(10);
}

/*��ȡ�Ĵ�����ֵ*/
unsigned char MTDREP_CAN3_MCP2515_Read_Register(const unsigned char Addr)
{
    unsigned char Data;   
	
    MTDREP_SPI1_CS_LOW();                //��MCP2515��CSΪ�͵�ƽ
    MTDREP_SPI1_SendByte(SPICAN_READ);   //���Ͷ�����
    MTDREP_SPI1_SendByte(Addr); 				 //���͵�ַ	
    Data = MTDREP_SPI1_ReadByte();   		 //��һ�ֽ� 
    MTDREP_SPI1_CS_HIGH();               //��MCP2515��CSΪ�ߵ�ƽ 
    return Data; 
}

/*********read the several register*********************/
void MTDREP_CAN3_MCP2515_Read_RegisterS(const unsigned char Addr, unsigned char RegValues[], const unsigned char RegNum)
{
		unsigned char i;
		
	  MTDREP_SPI1_CS_LOW();                //��MCP2515��CSΪ�͵�ƽ
		MTDREP_SPI1_SendByte(SPICAN_READ);   //���Ͷ�����
    MTDREP_SPI1_SendByte(Addr); 				 //���͵�ַ	
	
		// mcp2515 has auto-increment of address-pointer	
		for(i=0;i<RegNum;i++)
		{
			RegValues[i]=MTDREP_SPI1_ReadByte();	//��ȡ����
		}
		MTDREP_SPI1_CS_HIGH();               //��MCP2515��CSΪ�ߵ�ƽ 
}

//���üĴ�����ֵ
/** 
  * @brief  Send a byte to MCP2515 through SPI to return corresponding status you need 
  * @param  Add:Address of register 
  * @param  Data:the data U need to be sent from MCU to MCP2515 
  * @retval none 
  */ 
void MTDREP_CAN3_MCP2515_Set_Register(const unsigned char Addr,const unsigned char Data)
{
	  MTDREP_SPI1_CS_LOW();   						//ʹ��Ƭѡ     
    MTDREP_SPI1_SendByte(SPICAN_WRITE); //����д����
    MTDREP_SPI1_SendByte(Addr); 				//���͵�ַ				
    MTDREP_SPI1_SendByte(Data); 				//д������
    MTDREP_SPI1_CS_HIGH(); 							//�ر�Ƭѡ
}


void MTDREP_CAN3_MCP2515_Set_RegisterS(const unsigned char Addr,const unsigned char SetValues[],const unsigned char RegNum) //���ö���Ĵ�����ֵ
{
		unsigned char i;
		MTDREP_SPI1_CS_LOW();   						//ʹ��Ƭѡ
	  MTDREP_SPI1_SendByte(SPICAN_WRITE); //����д����
    MTDREP_SPI1_SendByte(Addr); 				//���͵�ַ	
	
		// mcp2515 has auto-increment of address-pointer
		for(i=0;i<RegNum;i++)
		{
			MTDREP_SPI1_SendByte(SetValues[i]);//д������
		}
    MTDREP_SPI1_CS_HIGH(); 							//�ر�Ƭѡ		
}


/** 
  * @brief  Modify the bit of the register 
  * @param  Add:Address of register 
  * @param  Mask:1-->U can modify the corresponding bit;0-->U can not modify the corresponding bit 
  * @param  Data:the data U need to be sent from MCU to MCP2515 
  * @retval none 
  */ 
void MTDREP_CAN3_MCP2515_Register_BitModify(const unsigned char Addr,const unsigned char Mask,unsigned char Data) 
{ 
    MTDREP_SPI1_CS_LOW();                			//ʹ��Ƭѡ
    MTDREP_SPI1_SendByte(SPICAN_BIT_MODIFY); 	//����λ�޸�����
    MTDREP_SPI1_SendByte(Addr); 							//���͵�ַ
    MTDREP_SPI1_SendByte(Mask); 							//��������
    MTDREP_SPI1_SendByte(Data); 							//д������
    MTDREP_SPI1_CS_HIGH();    								//�ر�Ƭѡ
}

//��ȡ�Ĵ���״̬
unsigned char MTDREP_CAN3_MCP2515_Read_Status(void)
{
		unsigned char State;
		MTDREP_SPI1_CS_LOW();                				//ʹ��Ƭѡ
		MTDREP_SPI1_SendByte(SPICAN_READ_STATUS); 	//���Ͷ�״̬����
		State = MTDREP_SPI1_ReadByte();							//��ȡ״̬
		MTDREP_SPI1_CS_HIGH();											//�ر�Ƭѡ
		
		return State;
}


 //��ȡ���ܵ�״̬
unsigned char MTDREP_CAN3_MCP2515_ReadRx_Status(void)
{
		unsigned char RxState;
		MTDREP_SPI1_CS_LOW();                				//ʹ��Ƭѡ
		MTDREP_SPI1_SendByte(SPICAN_RX_STATUS); 		//���Ͷ�״̬����
		RxState = MTDREP_SPI1_ReadByte();						//��ȡ״̬
		MTDREP_SPI1_CS_HIGH();											//�ر�Ƭѡ
		
		return RxState;
}


/*
//���ÿ��ƼĴ��� �Ĺ���ģʽ
	����0�ɹ�������1����ʧ�ܡ�
*/
unsigned char MTDREP_CAN3_MCP2515_SetControlRegister_Mode(const unsigned char NewMode)	
{
		unsigned char mode;
		MTDREP_CAN3_MCP2515_Register_BitModify(CANCTRL,REQOP,NewMode);	//�޸�ģʽ
		
		//verify as advised in datasheet
		mode = MTDREP_CAN3_MCP2515_Read_Register(CANCTRL);
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
*/
unsigned char MTDREP_CAN3_MCP2515_Config_Rate(const unsigned char  BaudRate)
{
		unsigned char Set,cfg1,cfg2,cfg3;
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
			//Set the baudrate 				Fsoc=20Mhz
			//set CNF1,SJW=00,lengthe is 1TQ,BRP=49,TQ=[2*(BRP+1)]/Fsoc=2*50/20M=5us 
			MTDREP_CAN3_MCP2515_Set_Register(CNF1,cfg1);		//���ò�����
			//set CNF2,SAM=0,caputre one time,PHSEG1=(2+1)TQ=3TQ,PRSEG=(0+1)TQ=1TQ 
			MTDREP_CAN3_MCP2515_Set_Register(CNF2,cfg2);
			//set CNF3,PHSEG2=(2+1)TQ=3TQ,when CANCTRL.CLKEN=1,set CLKOUT pin to output 
			MTDREP_CAN3_MCP2515_Set_Register(CNF3,cfg3);
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
void MTDREP_CAN3_MCP2515_Read_CAN_ID(const unsigned char RxIdReg_Addr, unsigned char* IDE_ExtFlag, unsigned int* Std_id,unsigned int * Ext_id)
{
		unsigned char  IdBufData[4];
		
		/*��ʼ��*/
		*IDE_ExtFlag = CAN_ID_STD;
		*Std_id	=	0;
		*Ext_id = 0;
	
		MTDREP_CAN3_MCP2515_Read_RegisterS(RxIdReg_Addr,IdBufData,4);
		
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
void MTDREP_CAN3_MCP2515_Read_CAN_Message(const unsigned char RxBuf_SIDH_Addr, CanRxMsg* Message)
{
		unsigned char RxReg_Addr, RxControlReg_Value;
	
		RxReg_Addr = RxBuf_SIDH_Addr;
		
		/*���id*/
		MTDREP_CAN3_MCP2515_Read_CAN_ID(RxReg_Addr,&(Message->IDE),&(Message->StdId),&(Message->ExtId));	//�õ�id
		
		RxControlReg_Value = MTDREP_CAN3_MCP2515_Read_Register(RxReg_Addr-1);		//�ö���ܿ��ƼĴ���ֵ
	
		Message->DLC = MTDREP_CAN3_MCP2515_Read_Register(RxReg_Addr+4);					//��ȡ���ݳ���
	
		if(RxControlReg_Value & RXRTR_REMOTE/*0x08*/  || (Message->DLC &0x40) )		//˵����Զ��֡
		{
				Message->RTR = CAN_RTR_REMOTE;		//��ʾΪԶ��֡
		}
		else
		{
				Message->RTR = CAN_RTR_DATA;			//��ʾΪ����֡
		}
		
		Message->DLC &= 0x0F;   //�Ĵ����ĵ�4λ���������ݳ���
		
		MTDREP_CAN3_MCP2515_Read_RegisterS(RxReg_Addr+5,&(Message->Data[0]),Message->DLC);		//��ȡ����
}


/*
//д��Ҫ���͵�canid
IDE_ExtFlag =CAN_ID_EXT Ϊ��չ֡
*/
void MTDREP_CAN3_MCP2515_Write_CAN_ID(const unsigned char TxIdReg_Addr, const unsigned char IDE_ExtFlag,const unsigned int can_id)
{
		unsigned int canid;
		unsigned char IdBufData[4];
		
		if(IDE_ExtFlag ==CAN_ID_EXT)		//��ʾΪ��չ֡
		{
			canid =can_id;
			IdBufData[3] = (unsigned char )(canid & 0xFF);	//EID0
			IdBufData[2] = (unsigned char )(canid >> 8);		//EID8
			
			canid >>= 16;		//��ø�16λ
			
			IdBufData[1] = (unsigned char )(canid & 0x03);
			IdBufData[1] +=(unsigned char )((canid & 0x1C)<<3);
			IdBufData[1] |= EXIDE_SET;
			IdBufData[0] = (unsigned char )(canid >> 5);
		}
		else
		{
			canid = (unsigned short)(can_id & 0xFFFF);
			
			IdBufData[0] = (unsigned char )(canid >>3);
			IdBufData[1] = (unsigned char )(canid &0x07 <<5); 
			IdBufData[2] = 0;
			IdBufData[3] = 0;
		}
		
		MTDREP_CAN3_MCP2515_Set_RegisterS(TxIdReg_Addr,IdBufData,4);		//��idд��Ĵ���
}

/*
	д��Ҫ���͵�can��Ϣ   
// Buffer can be TXBUF0 TXBUF1 or TXBUF2
*/
void MTDREP_CAN3_MCP2515_Write_CAN_Message(const unsigned char TxBuf_SIDH_Addr, const CanTxMsg* Message)
{
		unsigned char TxReg_Addr, Dlc;		//��ַ�����ݵĳ���
	
		TxReg_Addr = TxBuf_SIDH_Addr;
		Dlc = Message->DLC;
		
		MTDREP_CAN3_MCP2515_Set_RegisterS(TxReg_Addr+5,&(Message->Data[0]),Dlc);		//��Ĵ���д������
		
		if(Message->IDE == CAN_ID_EXT)	//��չ֡
		{
				MTDREP_CAN3_MCP2515_Write_CAN_ID(TxReg_Addr,Message->IDE,Message->ExtId);		//д��canid
		}
		else		//��׼֡
		{
				MTDREP_CAN3_MCP2515_Write_CAN_ID(TxReg_Addr,Message->IDE,Message->StdId);
		}
		
		if(Message->RTR ==	CAN_RTR_REMOTE )		//��ʾԶ��֡
		{
			Dlc |= 0x40;		//�ֲ�21ҳ
		}
		
		MTDREP_CAN3_MCP2515_Set_Register((TxReg_Addr+4),Dlc);		//д��RTR��DLC
}		


/*
 ** Start the transmission from one of the tx buffers.
// Buffer can be TXBUF0 TXBUF1 or TXBUF2
 */
void MTDREP_CAN3_MCP2515_Start_Transimt(const unsigned char TxBuf_SIDH_Addr)
{
		MTDREP_CAN3_MCP2515_Register_BitModify(TxBuf_SIDH_Addr-1,TXREQ,TXREQ);
}


/*
	��� һ���յ������ͻ�����
*/
unsigned char MTDREP_CAN3_MCP2515_Get_Free_TxBuf(unsigned char * TxBufNum)
{
		unsigned char TempState, i, ControlRegValue;
		unsigned char ControlReg[3] ={TXB0CTRL,TXB1CTRL,TXB2CTRL};
		
		TempState = MCP2515_ALLTXBUSY;		//��ʼ�����ͻ�����״̬�����еķ��ͻ�������	
		*TxBufNum = 0x00;
		
		// check all 3 TX-Buffers
		for(i=0;i<3;i++)
		{
			ControlRegValue = MTDREP_CAN3_MCP2515_Read_Register(ControlReg[i]);		//��ȡ���ƼĴ�����ֵ
			if((ControlRegValue & TXREQ )== 0)
			{
				debugprintf("Selected TX-Buffer %d\r\n", i+1);
				*TxBufNum = ControlReg[i]+1; 	// return SIDH-address of Buffer
				TempState = MCP2515_OK;
				return TempState;
			}
			
		}
		
		return TempState;
}


/*
//�������μĴ������˲��Ĵ�������ʼ�����ͻ�����

*/
void MTDREP_CAN3_MCP2515_Init_CAN_Buffer(void)
{
		unsigned char i,TxContr1,TxContr2,TxContr3;
		// TODO: check why this is needed to receive extended and standard frames
		// Mark all filter bits as don't care:  	���μĴ���ȫ��Ϊ0���������е�canid
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXM0SIDH,0,0);	//��ʼ���������μĴ���0��Ϊ0��������id
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXM1SIDH,0,0);	//��ʼ���������μĴ���1

		// Anyway, set all filters to 0:		/*���ܻ�����0 */
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXF0SIDH,1,0);	// RXB0: extended 
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXF1SIDH,0,0);	//    AND standard
	
																				/*���ܻ�����1���˲��Ĵ�������*/
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXF2SIDH, 1, 0); // RXB1: extended 
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXF3SIDH, 0, 0); //  AND standard
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXF4SIDH, 0, 0);
		MTDREP_CAN3_MCP2515_Write_CAN_ID(RXF5SIDH, 0, 0);
		
	// Clear, deactivate the three transmit buffers		��գ�ͣ���������ͻ�����
	// TXBnCTRL -> TXBnD7
	
		TxContr1 = TXB0CTRL;
		TxContr2 = TXB1CTRL;
		TxContr3 = TXB2CTRL;
		for(i=0;i<14;i++)				// in-buffer loop
		{
			MTDREP_CAN3_MCP2515_Set_Register(TxContr1,0);
			MTDREP_CAN3_MCP2515_Set_Register(TxContr2,0);
			MTDREP_CAN3_MCP2515_Set_Register(TxContr3,0);
			
			TxContr1++;
			TxContr2++;
			TxContr3++;
		}
		
		// and clear, deactivate the two receive buffers. ��գ�ͣ���������ܻ�����
		MTDREP_CAN3_MCP2515_Set_Register(RXB0CTRL,0);
		MTDREP_CAN3_MCP2515_Set_Register(RXB1CTRL,0);
}


/*����can3��Ϣ*/
unsigned char MCP2515_CAN3_Receive_Message(CanRxMsg* msg)
{
	unsigned char state, res;
	state = MTDREP_CAN3_MCP2515_Read_Status();
	
	if(state & RX0IF)
	{
		//Msg in Buffer 0
		MTDREP_CAN3_MCP2515_Read_CAN_Message(RXB0SIDH,msg);			//��ȡ���յ�����Ϣ
		MTDREP_CAN3_MCP2515_Register_BitModify(CANINTF,0xFC | RX0IF,0);		//����ж�����־λ
		res= CAN_OK;
	}
	else if(state & RX1IF)
	{
		// Msg in Buffer 1
		MTDREP_CAN3_MCP2515_Read_CAN_Message(RXB1SIDH,msg);
		MTDREP_CAN3_MCP2515_Register_BitModify(CANINTF,0xFC | RX1IF,0);		//����ж�����־λ
		res= CAN_OK;
	}
	else
	{
		res =CAN_NOMSG;
	}
	return res;
}

/* returns either 
 #define CAN_MSGAVAIL   (3) - a message has been received
 #define CAN_NOMSG      (4) - no new message
*/
unsigned char MCP2515_CAN3_Check_Receive(void)
{
	unsigned char res;
	res = MTDREP_CAN3_MCP2515_Read_Status();	// RXnIF in Bit 1 and 0
	
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
	unsigned char eflg = MTDREP_CAN3_MCP2515_Read_Register(EFLG);

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
	CanRxMsg *tempmsg;
	unsigned char state=CAN_FAIL;
	tempmsg = msg;
	state = MCP2515_CAN3_Receive_Message(tempmsg);
	
}





///** 
//  * @brief  Read 8 bytes to CAN bus 
//  * @param  CAN_TX_Buf:the address of data'indicator,8bytes 
//  * @retval none 
//  */ 
//void MTDREP_CAN3_SPICAN_Send8bytes(unsigned char *CAN_TX_Buf) 
//{ 
//    unsigned char tempdata; 
//    tempdata=SPIReadByte_MCP2515(SPICAN_READ_STATUS); 
//    SPISendByte_MCP2515(TXB0D0,CAN_TX_Buf[0]); 
//    SPISendByte_MCP2515(TXB0D1,CAN_TX_Buf[1]); 
//    SPISendByte_MCP2515(TXB0D2,CAN_TX_Buf[2]); 
//    SPISendByte_MCP2515(TXB0D3,CAN_TX_Buf[3]); 
//    SPISendByte_MCP2515(TXB0D4,CAN_TX_Buf[4]); 
//    SPISendByte_MCP2515(TXB0D5,CAN_TX_Buf[5]); 
//    SPISendByte_MCP2515(TXB0D6,CAN_TX_Buf[6]); 
//    SPISendByte_MCP2515(TXB0D7,CAN_TX_Buf[7]); 
//    if(tempdata&0x04)			//check the TXREQ bit 
//    { 
//        delay_ms(1); 
//        MCP2515_BitModify(TXB0CTRL,0x80,0x00);	//clear TXREQ bit 
//        while(SPI_CMD_MCP2515(SPICAN_READ_STATUS)&0x04);//wait until TXREQ bit matchs 0 
//    } 
//    MTDCPU_SPI_CS_LOW(); 
//    MTDCPU_SPI1_SendByte(SPICAN_RTS_TXB0);//TXB0 send-request 
//    MTDCPU_SPI_CS_HIGH(); 
//}

///** 
//  * @brief  Send a byte to CAN bus by TXB0 
//  * @param  Data:data to be sent 
//  * @retval none 
//  */ 
//void SPICAN_SendOneByte(unsigned char Data) 
//{ 
//    unsigned char tempdata; 
///*Test the driver between MCP2515 and the SPI periphere :tempdata<--------------*/ 
//    tempdata=SPI_CMD_MCP2515(SPICAN_READ_STATUS); 
//    if(tempdata&0x04)//check the TXREQ bit 
//    { 
//        delay_ms(1); 
//        MCP2515_BitModify(TXB0CTRL,0x08,0x00);//clear TXREQ bit 
//        while(SPI_CMD_MCP2515(SPICAN_READ_STATUS)&0x04);//wait until TXREQ bit matchs 0 
//    } 
//    SPISendByte_MCP2515(TXB0D0,Data); 
//    MTDCPU_SPI_CS_LOW(); 
//    MTDCPU_SPI1_SendByte(SPICAN_RTS_TXB0);//TXB0 send-request 
//    MTDCPU_SPI_CS_HIGH(); 
//}

///** 
//  * @brief  Receive a byte from MCP2515 RXB0 
//  * @param  none 
//  * @retval Data:return the effectively data from RXB0 
//  */ 
//unsigned char CAN_Receive_onebyte() 
//{ 
//    unsigned char tempdata; 
//    tempdata=SPI_CMD_MCP2515(SPICAN_READ_STATUS);//confirm receive data=RXB0IF 
//    if(tempdata&0x01)    
//    { 
//        tempdata=SPI_CMD_MCP2515(SPICAN_RX_STATUS); 
//        if(tempdata&0x40)//true means RXB0's standard frame 
//        {            
//            tempdata=SPIReadByte_MCP2515(RXB0D0); 
//        } 
//        else 
//            {tempdata=254;}//no standard frame receive 
//        MCP2515_BitModify(CANINTF,0x01,0x00);    
//    } 
//    else 
//        {tempdata=255;}//no frame receive 
//    return tempdata; 
//}

///** 
//  * @brief   Send n bytes with a given standard ID  corresponding to frame type 
//  * @param   CAN_TX_Buf: data to be sent 
//  * @param     DLC:DLC<=8 
//  * @param   SID:<=0x7FF 
//  * @param   CAN_FrameType:CAN_STD,CAN_RTR        
//  * @retval None 
//  */ 
//void SPICAN_SendData(unsigned char *CAN_TX_Buf,Frame_TypeDef *Frame ) 
//{ 
//    unsigned char tempdata; 
//    unsigned char HSID,LSID; 
//    if(Frame->Type==CAN_STD) 
//    { 
//        /*Set the ID of the frame*/ 
//        HSID=(unsigned char)(Frame->SID>>3); 
//        LSID=(unsigned char)((Frame->SID<<5)&0xE0); 
//        SPISendByte_MCP2515(TXB0SIDH,HSID); 
//        SPISendByte_MCP2515(TXB0SIDL,LSID); 
//        /*Set the DLC and the type of the frame*/ 
//        SPISendByte_MCP2515(TXB0DLC,Frame->DLC|CAN_STD); 
//        /*Write the data into the TXB0 data registers */ 
//        for(tempdata=0;tempdata<Frame->DLC;tempdata++) 
//					SPISendByte_MCP2515(TXB0D[tempdata],CAN_TX_Buf[tempdata]);        
///*      SPISendByte_MCP2515(TXB0D0,CAN_TX_Buf[0]); 
//        SPISendByte_MCP2515(TXB0D1,CAN_TX_Buf[1]); 
//        SPISendByte_MCP2515(TXB0D2,CAN_TX_Buf[2]); 
//        SPISendByte_MCP2515(TXB0D3,CAN_TX_Buf[3]); 
//        SPISendByte_MCP2515(TXB0D4,CAN_TX_Buf[4]); 
//        SPISendByte_MCP2515(TXB0D5,CAN_TX_Buf[5]); 
//        SPISendByte_MCP2515(TXB0D6,CAN_TX_Buf[6]); 
//        SPISendByte_MCP2515(TXB0D7,CAN_TX_Buf[7]);*/ 
//    } 
//    else  /*if(CAN_FrameType==CAN_RTR)*/ 
//    { 
//        /*Set the ID of the frame*/ 
//        HSID=(unsigned char)(Frame->SID>>3); 
//        LSID=(unsigned char)((Frame->SID<<5)&0xE0); 
//        SPISendByte_MCP2515(TXB0SIDH,HSID); 
//        SPISendByte_MCP2515(TXB0SIDL,LSID); 
//        /*Set the type of the frame*/ 
//        SPISendByte_MCP2515(TXB0DLC,CAN_RTR); 
//    }

//    tempdata=SPI_CMD_MCP2515(SPICAN_READ_STATUS); 
//    if(tempdata&0x04) 
//    { 
//        delay_ms(1); 
//        MCP2515_BitModify(TXB0CTRL,0x80,0x00); 
//        while(SPI_CMD_MCP2515(SPICAN_READ_STATUS)&0x04); 
//    } 
//    /*Send the SPI_RTS_TXB0 request command to MCP2515 to send the data loaded in the data register*/ 
//    MTDCPU_SPI_CS_LOW(); 
//    MTDCPU_SPI1_SendByte(SPICAN_RTS_TXB0); 
//    MTDCPU_SPI_CS_HIGH(); 
//}

///** 
//  * @brief  Receive n bytes from MCP2515 RXB0 
//  * @param  none 
//  * @retval Data:return the effectively data from RXB0 
//  */ 
//unsigned char SPICAN_ReceiveData(unsigned char *CAN_RX_Buf,Frame_TypeDef *Frame) 
//{ 
//    unsigned char tempdata; 
//    unsigned int CAN_ID; 
////    tempdata=SPI_CMD_MCP2515(SPI_READ_STATUS);//check if it is received a  frame 
//    tempdata=SPI_CMD_MCP2515(SPICAN_RX_STATUS);

//    if(tempdata&0x40)//RXB0 
//    { 
//        CAN_ID=(unsigned int)((SPIReadByte_MCP2515(RXB0SIDH))<<8); 
//        CAN_ID|=(unsigned int)(SPIReadByte_MCP2515(RXB0SIDL)); 
//        CAN_ID>>=5; 
//        /*Check the CAN_ID you received that if it is matched with the SID you wish*/ 
//        if(/*CAN_ID==Frame->SID*/1) 
//        { 
//            for(tempdata=0;tempdata<Frame->DLC;tempdata++) 
//							CAN_RX_Buf[tempdata]=SPIReadByte_MCP2515(RXB0D[tempdata]); 
///*          CAN_RX_Buf[0]=ReadByte_MCP2515(RXB0D0); 
//            CAN_RX_Buf[1]=ReadByte_MCP2515(RXB0D1); 
//            CAN_RX_Buf[2]=ReadByte_MCP2515(RXB0D2); 
//            CAN_RX_Buf[3]=ReadByte_MCP2515(RXB0D3); 
//            CAN_RX_Buf[4]=ReadByte_MCP2515(RXB0D4); 
//            CAN_RX_Buf[5]=ReadByte_MCP2515(RXB0D5); 
//            CAN_RX_Buf[6]=ReadByte_MCP2515(RXB0D6); 
//            CAN_RX_Buf[7]=ReadByte_MCP2515(RXB0D7);*/ 
//					MCP2515_BitModify(CANINTF,0x01,0x00); 	//����жϱ�־λ(�жϱ�־�Ĵ���������MCU����)
//        } 
//        else 
//        { 
//            return 0;//ID is not correct 
//        } 
//        return 1;//receive ok 
//    } 
//    else 
//    return 2;//no standard frame receive 
//}

