#ifndef __SAF82525_H__
#define __SAF82525_H__

#include "stm32f4xx.h"

#define SAF82525_INT_RCC           RCC_AHB1Periph_GPIOE
#define SAF82525_INT_GPIO          GPIOE
#define SAF82525_INT_GPIO_PIN      GPIO_Pin_6
#define SAF82525_INT_EXIT_PORT		 EXTI_PortSourceGPIOE
#define SAF82525_INT_EXIT_PIN			 EXTI_PinSource6

#define SAF82525_REST_RCC           RCC_AHB1Periph_GPIOF
#define SAF82525_REST_GPIO          GPIOF
#define SAF82525_REST_GPIO_PIN      GPIO_Pin_6
#define SAF82525_REST_OFF() 				GPIO_ResetBits(SAF82525_REST_GPIO,SAF82525_REST_GPIO_PIN);
#define SAF82525_REST_ON()					GPIO_SetBits(SAF82525_REST_GPIO,SAF82525_REST_GPIO_PIN);		//�ߵ�ƽ���и�λ,���ٳ���1.8us

#define SAF_BAUD_RATE_125000		0x02		//0x02 Ϊ12M����	
#define SAF_BAUD_RATE_115200		0x02		//
#define SAF_BAUD_RATE_19200			0x14  	//0x11 Ϊ11.0592M ����  0x14Ϊ12M
#define SAF_BAUD_RATE_9600			0x26		//0x23 Ϊ11.0592M ����  0x26Ϊ12M


extern __IO uint8_t HDLCARxBuf[200];
extern __IO uint8_t	HDLCBRxBuf[200];
extern __IO uint16_t HDLCARxLen;
extern __IO uint16_t HDLCBRxLen;

extern u8  g_HDLCARxStartFlag;		//Aͨ�����ܿ�ʼ��־
extern u8  g_HDLCARxEndFlag;			//Aͨ�����ܽ�����־
extern u8  g_HDLCBRxStartFlag;		//Bͨ�����ܿ�ʼ��־
extern u8  g_HDLCBRxEndFlag;			//Bͨ�����ܽ�����־

void SAF82525_INT_GPIO_Init(void);
void SAF82525_Rest_CPIO_Init(void);
void SAF82525_GPIO_Init(void);
void SAF82525_Init(uint8_t HDLC_BautRate);
uint8_t SAF82525_Read_Register(const uint8_t ReadAddr);
void SAF82525_Write_Register(const uint8_t WriteAddr,uint8_t WriteData);
void SAF82525_ModifyBit_Register(uint8_t Addr,uint8_t Mask,uint8_t Data);
uint8_t CheckASTAR(void);		//���SAB82525״̬����        
uint8_t CheckBSTAR(void);   //���Bͨ��״̬
void SetACMDRegister(uint8_t CMD_Data);	//�������������Ժ�Ϊһ��
void SetBCMDRegister(uint8_t CMD_Data);

//void WaitAXFIFOWriteEnable(uint8_t ChanleRegister);			(ʹ�������� __inline)			//���ͻ�����дʹ��

void AChanleTxData(uint8_t *buf,int len);		//Aͨ����������
void BChanleTxData(uint8_t *buf,int len);		//Bͨ����������
void AChanleRxData(__IO uint8_t *buf,__IO uint16_t *len);		//Aͨ����������
void BChanleRxData(__IO uint8_t *buf,__IO uint16_t *len);		//Bͨ����������

/*CHANNEL A REGISTER ADDRESS*/
#define AFIFO       0x00    /*//read\write*/	//Receive/Transmit FIFO
#define AISTA       0x20    /*/READ*/       	//Interrupt STAtus
#define AMASK       0x20    /*/WRITE*/      	//Mask
#define ASTAR       0x21    /*/read*/       	//STAtus
#define ACMDR       0x21    /*/write*/      	//COManD
#define AMODE       0x22    /*/read\write */	//MODE
#define ATIMR       0x23    /*/read\write*/ 	//TIMer
#define AEXIR       0x24    /*/read*/       	//EXtended Interrupt
#define AXAD1       0x24    /*/write*/      	//Transmit ADdress 1
#define ARBCL       0x25    /*/read*/       	//Receive Byte Count Low
#define AXAD2       0x25    /*/write*/      	//Transmit ADdress 2
#define ARAH1       0x26    /*/WRITE*/      	//Receive Address High 1
#define ARSTA       0x27    /*/read*/       	//Receive STAtus
#define ARAH2       0x27    /*/write*/      	//Rec. Addr. High 2
#define ARAL1       0x28    /*/read\write*/ 	//Receive Address Low 1
#define ARHCR       0x29                    	//Receive HDLC Control
#define ARAL2       0x29                    	//Receive Addr. Low 2
#define AXBCL       0x2a                    	//Transmit Byte Count Low
#define ABGR        0x2b    									//Baudrate Generator Register
#define ACCR2       0x2c    									//Channel Configuration Register 2
#define ARBCH       0x2d    									//Receive Byte Count High
#define AXBCH       0x2d    									//Transmit Byte Count High
#define AVSTR       0x2e    									//Version STatus
#define ARLCR       0x2e											//Receive Frame Length Check
#define ACCR1       0x2f    									//Channel Configuration Register 1
#define ATSAX       0x30    						  	 	//ʱ����䷢��
#define ATSAR       0x31    							 		//ʱ��۷������
#define AXCCR       0x32    									//Transmit Channel Capacity ����ͨ������
#define ARCCR       0x33    									//Receive Channel Capacity  ����ͨ������

/*CHANNEL B REGISTER ADDRESS*/
#define BFIFO       0x40  
#define BISTA       0x60  
#define BMASK       0x60    
#define BSTAR       0x61    
#define BCMDR       0x61    
#define BMODE       0x62    
#define BTIMR       0x63    
#define BEXIR       0x64    
#define BXAD1       0x64   
#define BRBCL       0x65    
#define BXAD2       0x65    
#define BRAH1       0x66    
#define BRSTA       0x67    
#define BRAH2       0x67    
#define BRAL1       0x68    
#define BRHCR       0x69    
#define BRAL2       0x69    
#define BXBCL       0x6a    
#define BBGR        0x6b    
#define BCCR2       0x6c    
#define BRBCH       0x6d    
#define BXBCH       0x6d    
#define BVSTR       0x6e    
#define BRLCR       0x6e    
#define BCCR1       0x6f    
#define BTSAX       0x70    
#define BTSAR       0x71    
#define BXCCR       0x72    
#define BRCCR       0x73    


//W H H��ȡ�жϼĴ�����ʱ���жϾͱ�����ˡ�
/*iSTA*/
#define RME         0x80			//������Ϣ����
#define RPF         0x40			//���ܳ���
#define RSC         0x20			//����״̬�ı䣨���Զ�ģʽ�£�
#define XPR         0x10			//���ͳؾ���
#define TIN         0x08			//��ʱ���ж�
#define ICA         0x04			//ͨ��A�ж�
#define EXA         0x02			//ͨ��A��չ�ж�
#define EXB         0x01			//ͨ��B��չ�ж�


/*exir*/
#define XMR         0x80			//���ͱ����ظ�
#define XDU_EXE     0x40			//��������Ƿ�� �� ��չ���ͽ���
#define PCE         0x20			//Э�����
#define RFO         0x10			//����֡���
#define CSC         0x08			//�������״̬�ı�
#define RFS         0x04			//����֡��ʼ

/*star*/
#define XDOV        0x80			//�����������
#define XFW         0x40			//����FIFOдʹ��		XFW:1: Transmit FIFO Write Enable.Data can be written to the XFIFO.
#define XRNR        0x20			//����RNR���Զ�ģʽ�£�
#define RRNR        0x10			//����RNR���Զ�ģʽ�£�
#define RLI         0x08			//������·��Ч
#define CEC         0x04			//����ִ��			
#define CTS         0x02			//�������״̬
#define WFA         0x01			//�ȴ�ȷ�ϣ��Զ�ģʽ��
#define XFW_CEC     0x44			//ֻ��CECΪ0�ǣ�XFW����Ч

/*rsta*/  //RFIFO�����һ���ֽ��д洢
#define VFR         0x80			//��Ч֡(1 ��Ч��0 ��Ч)
#define RDO         0x40			//�����������()
#define SAB_CRC     0x20			//CRC�Ƚϼ��(0 ʧ�ܣ�1 0k)
#define RAB         0x10			//������Ϣ��ֹ(CPU���붪����֡)
#define HA          0x0C			//��λ�ֽڵ�ַ���бȽ�
#define C_R         0x02			//����/��Ӧ
#define LA          0x01			//���ֽڵ�ַ���бȽ�

/*cmdr*/
#define RMC         0x80			//������Ϣ���
#define RHR         0x40			//��λHDLC����								Reset HDLC Receiver.All data in the RFIFO and the HDLC receiver deleted.
#define RNR_XREP    0x20			//���ܲ�����/�����ظ�
#define STI         0x10			//������ʱ��
#define XTF         0x08			//����͸��֡
#define XIF         0x04			//����I֡���Զ�ģʽ�£�
#define XME         0x02			//���ͱ��Ľ������ж�ģʽ��
#define XRES        0x01			//���͸�λ (����ֹ֡����)		Transmit Reset.The contents of the XFIFO is deleted and IDLE is transmitted.

/*control*/
#define UFRAME      0x00
#define IFRAME      0x01

/*mode register bit*/
#define MDS1				0x80			//ģʽѡ��
#define MDS0 				0x40
#define ADM					0x20			//��ַģʽ
#define TMD					0x10			//��ʱ��ģʽ
#define RAC					0x08			//��������Ч		
#define RTS					0x04			//������		(0,HSCX�Զ����ƣ�1��cpu����)
#define TRS					0x02			//��ʱ���ֱ���
#define TLP					0x01			//����ѭ��	

/*CCR2 bit*/
#define SOC1			  0x80			//�����������
#define SOC0				0x40			
#define BR9					0x80			//������ ��9λ
#define BR8					0x40		
#define BDF					0x20			//�����ʷ�Ƶ����
#define TSS					0x10			//����ʱ��Դѡ��
#define XCS0				0x20			//����/����ʱ����λ��bit0
#define RCS0				0x10			//
#define TIO					0x08			//����ʱ�������������
#define CIE					0x04			//��������ж�ʹ��
#define RIE					0x02			//���ܿ�ʼ�ж�ʹ��(RFS=1)
#define DIV					0x01			//���ݷ�ת ������NRZģʽʱ��

/*CCR1 bit*/
#define PU					0x80 			//�ϵ硢�����л�(0���磬1�ϵ�)
#define SC1					0x40			//���ж˿�����  ( 00 NRZ,10 NRZI,01��ʱ1,11 ��ʱ2)
#define SC0					0x20			
#define ODS					0x10			//�������ѡ��	��0 ©����·��1 ���������
#define ITF_OIN			0x08			//֡��ʱ�����/����(ѡ��OIN)
#define CM2					0x04			//ʱ��ģʽѡ��
#define CM1					0x02
#define CM0					0x01


#endif //__SAF82525_H__
