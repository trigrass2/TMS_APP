#ifndef __MTD_REP_CAN3_MCP2515_H__
#define __MTD_REP_CAN3_MCP2515_H__

#include "MTD_REP_SPI1.h"
#include "MCP2515_RegisterDefine.h"


/*���ܻ��������������ͻ�����*/
#define MCP2515_TXBUF_NUM  (3)		//MCP2515 ��3�����ͻ�����

#define MCP2515_TXBUF_0 (TXB0SIDH)
#define MCP2515_TXBUF_1 (TXB1SIDH)
#define MCP2515_TXBUF_2 (TXB2SIDH)

#define MCP2515_RXBUF_0 (RXB0SIDH)
#define MCP2515_RXBUF_1 (RXB1SIDH)

/*����ֵ����*/
#define MCP2515_OK         (0)
#define MCP2515_FAIL       (1)
#define MCP2515_ALLTXBUSY  (2)		//���еķ��ͻ�������

#define CAN_OK         (0)
#define CAN_FAILINIT   (1)	 //��ʼ��ʧ��
#define CAN_FAILTX     (2)	 //����ʧ��
#define CAN_MSGAVAIL   (3)	 //��Ϣ����
#define CAN_NOMSG      (4)	 //û����Ϣ
#define CAN_CTRLERROR  (5)	 //����
#define CAN_FAIL       (0xff)	//ʧ��

/*�����ʵ�ѡ��*/
#define MCP2515_CAN_BAUD_5K     (1)
#define MCP2515_CAN_BAUD_10K    (MCP2515_CAN_BAUD_5K + 1)
#define MCP2515_CAN_BAUD_20K    (MCP2515_CAN_BAUD_10K + 1)
#define MCP2515_CAN_BAUD_40K    (MCP2515_CAN_BAUD_20K + 1)
#define MCP2515_CAN_BAUD_50K    (MCP2515_CAN_BAUD_40K + 1)
#define MCP2515_CAN_BAUD_80K    (MCP2515_CAN_BAUD_50K + 1)
#define MCP2515_CAN_BAUD_100K   (MCP2515_CAN_BAUD_80K + 1)
#define MCP2515_CAN_BAUD_125K   (MCP2515_CAN_BAUD_100K + 1)
#define MCP2515_CAN_BAUD_200K   (MCP2515_CAN_BAUD_125K + 1)
#define MCP2515_CAN_BAUD_250K   (MCP2515_CAN_BAUD_200K + 1)
#define MCP2515_CAN_BAUD_400K   (MCP2515_CAN_BAUD_250K + 1)
#define MCP2515_CAN_BAUD_500K   (MCP2515_CAN_BAUD_400K + 1)
#define MCP2515_CAN_BAUD_800K   (MCP2515_CAN_BAUD_500K + 1)
#define MCP2515_CAN_BAUD_1M     (MCP2515_CAN_BAUD_800K + 1)


extern 	u8 MCP2515_CAN3_PORT;
#define CAN3 	(CAN_TypeDef *)(&MCP2515_CAN3_PORT)

///*Frame Definition*/ 
//typedef enum 
//{ 
//    CAN_STD=(unsigned char)(0x00), 
//    CAN_RTR=(unsigned char)(0x40) 
//}CAN_FrameType_TypeDef;

//typedef struct Frame_Strut 
//{ 
//    unsigned int SID; 
//    unsigned char DLC; 
//    CAN_FrameType_TypeDef Type; 
//}Frame_TypeDef;

/* Exported variables --------------------------------------------------------*/ 
extern unsigned char RXB0D[8]; 
extern unsigned char TXB0D[8]; 

/* Exported functions --------------------------------------------------------*/ 
unsigned char MTDREP_CAN3_MCP2515_Init(const unsigned char CanBaudRate); 
void MTDREP_CAN3_MCP2515_Reset(void); 			//���͸�λָ���λmcp2515
unsigned char MTDREP_CAN3_MCP2515_Read_Register(const unsigned char Addr);	/*��ȡһ���Ĵ�����ֵ*/
void MTDREP_CAN3_MCP2515_Read_RegisterS(const unsigned char Addr, unsigned char RegValues[], const unsigned char RegNum);  /*��ȡ����Ĵ�����ֵ*/
void MTDREP_CAN3_MCP2515_Set_Register(const unsigned char Addr,const unsigned char Data); //����һ���Ĵ�����ֵ
void MTDREP_CAN3_MCP2515_Set_RegisterS(const unsigned char Addr,const unsigned char SetValues[],const unsigned char RegNum); //���ö���Ĵ�����ֵ
void MTDREP_CAN3_MCP2515_Register_BitModify(const unsigned char Addr,const unsigned char Mask, unsigned char Data); //�޸ļĴ���ĳλ��ֵ
unsigned char MTDREP_CAN3_MCP2515_Read_Status(void);		//��ȡ�Ĵ���״̬
unsigned char MTDREP_CAN3_MCP2515_ReadRx_Status(void);	 //��ȡ���ܵ�״̬
unsigned char MTDREP_CAN3_MCP2515_SetControlRegister_Mode(const unsigned char NewMode);		//���ÿ��ƼĴ��� �Ĺ���ģʽ
unsigned char MTDREP_CAN3_MCP2515_Config_Rate(const unsigned char  BaudRate);							//����mcp2515 �Ĳ�����
void MTDREP_CAN3_MCP2515_Read_CAN_ID(const unsigned char RxIdReg_Addr, unsigned char* IDE_ExtFlag, unsigned int* Std_id,unsigned int * Ext_id);	//��ȡ���յ���canid��ʶ
void MTDREP_CAN3_MCP2515_Read_CAN_Message(const unsigned char RxBuf_SIDH_Addr, CanRxMsg* Message);			//��ȡ���ܵ�can��Ϣ����
void MTDREP_CAN3_MCP2515_Write_CAN_ID(const unsigned char TxIdReg_Addr, const unsigned char IDE_ExtFlag, const unsigned int can_id);		//д��Ҫ���͵�canid
void MTDREP_CAN3_MCP2515_Write_CAN_Message(const unsigned char TxBuf_SIDH_Addr, const CanTxMsg* Message);		//д��Ҫ���͵�can��Ϣ
void MTDREP_CAN3_MCP2515_Start_Transimt(const unsigned char TxBuf_SIDH_Addr);				//��ʼ������Ϣ
unsigned char MTDREP_CAN3_MCP2515_Get_Free_TxBuf(unsigned char * TxBufNum);
void MTDREP_CAN3_MCP2515_Init_CAN_Buffer(void);//�������μĴ������˲��Ĵ�������ʼ�����ͻ�����

/*
��can3 �ĳ�ʼ�������ͣ������� can_stm32.c�ļ��У�	
*/
/*������Ϣ*/
unsigned char MCP2515_CAN3_Receive_Message(CanRxMsg* msg);
unsigned char MCP2515_CAN3_Check_Receive(void);
unsigned char MCP2515_CAN3_Check_Error(void);
void MCP2515_CAN3_Interrupt(CanRxMsg *msg);

void MCP2515_REST1_GpioInit(void);
void MCP2515_INT1_GpioInit(void);
void MCP2515_INT1_IRQ(void);




//void MTDREP_CAN3_SPISendByte_MCP2515(unsigned char Addr,unsigned char Data); 
//unsigned char MTDREP_CAN3_SPIReadByte_MCP2515(unsigned char Addr); 
//unsigned char MTDREP_CAN3_SPI_CMD_MCP2515(unsigned char CMD); 
//void MTDREP_CAN3_SPICAN_SendData(unsigned char *CAN_TX_Buf,Frame_TypeDef *Frame); 
//unsigned char MTDREP_CAN3_SPICAN_ReceiveData(unsigned char *CAN_RX_Buf,Frame_TypeDef *Frame);
//void MTDREP_CAN3_Send_anylength(u8 *CAN_TX_Buf,u8 length1);



#endif  //__MTD_REP_CAN3_MCP2515_H__
