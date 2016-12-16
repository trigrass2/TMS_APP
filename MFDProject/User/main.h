#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"
#include "os.h"
#include "stdbool.h"

/*�¼���־�� λ���嶨��*/
#define  CAN2TxLED_FLAG  		0x01
#define  CAN2RxLED_FLAG 		0x02
#define  CAN2LEDFLAG_VALUE 	0x00

/*��ϵͳЭ��ı�־λ*/
#define  TxSDRFlag			0x01			//����SDR��־  ����ʱ��ʱ�䵽��
#define  TxSDREndFlag		0x02			//����SDR������־
#define  RxStartFlag		0x04			//���ܿ�ʼ��־
#define  RxEndFlag			0x08			//���ܽ�����־
#define  RxOutTimeFlag	0x10
#define  RxParityFrameError	0x20	//��żУ����� ,֡����
#define	 RxBCCError			0x40			//BCCУ�����

#define TxSDROrTDRFlag			0x01			//BCU ���ܷ���SDR������TDR
#define TxSDROrTDREndFlag		0x02

/*BCU����ı�־λ*/
extern	bool 	BCUTxTdrFlag; //�Ƿ���Ҫ����TDR 0,����Ҫ�� 1 ��Ҫ
extern 	bool 	BCUClearStatDataFlag	;
extern	bool	BCUSetTimeFlag ;


extern u8  ChooseBoard ;

//*����ʹ�õ�ȫ�ֱ����Ķ���*/
extern u8 g_Uart2RxStartFlag;
extern u8 g_Uart2RxDataFlag;
extern u8 g_Uart2FrameErrorFlag;

extern u8 g_Uart3RxStartFlag;
extern u8 g_Uart3RxDataFlag;
extern u8 g_Uart3FrameErrorFlag;

extern u8 g_Uart4RxStartFlag;
extern u8 g_Uart4RxDataFlag;
extern u8 g_Uart4FrameErrorFlag;
extern u8 g_BCURxSdOrTdFlag;
extern u8 g_Uart4NackFlag;

/*�¼���־��*/
extern OS_FLAG_GRP  DoorEventFlags;			//��
extern OS_FLAG_GRP 	ACCEventFlags;			//�յ��¼���־��
extern OS_FLAG_GRP 	BCUEventFlags;			//�ƶ��¼���־��
extern OS_FLAG_GRP ACPEventFlags;				//ACP��ѹ�� �¼���־��
extern OS_FLAG_GRP SIVEventFlags;				//SIV������Դ �¼���־��
extern OS_FLAG_GRP VVVF1EventFlags;			//VVVF1ǣ��1 �¼���־��
extern OS_FLAG_GRP VVVF2EventFlags;			//VVVF2ǣ��2 �¼���־��
extern OS_FLAG_GRP ATCEventFlags;				//ATC�ź� �¼���־��
extern OS_FLAG_GRP PISEventFlags;				//PIS�㲥 �¼���־��

/*������ƿ�*/
extern OS_TCB  USART3Rx_TCB;

#define  TPDO_18D_MFD1DI					0
#define  TPDO_18E_MFD2DI					1
#define  TPDO_18F_MFD3DI					2

#define  TPDO_190_DoorSta1				3
#define  TPDO_191_DoorSta2				4

/*���豸����*/
#define  TPDO_1A9_SubDecFault			28
#define  TPDO_1AC_SubDecFault			31
#define  TPDO_1AD_SubDecFault			32
#define  TPDO_1AE_SubDecFault			33

/*ACC*/
#define  TPDO_192_ACCSta1					5
#define  TPDO_1AA_ACCSta2					29

/*BCU*/
#define  TPDO_193_BCUSta1					6
#define  TPDO_194_BCUSta2					7
#define  TPDO_195_BCUSta3					8

/*ATC*/
#define  TPDO_196_ATCSta1					9
#define  TPDO_197_ATCSta2					10
#define  TPDO_198_ATCSta3					11
#define  TPDO_199_ATCSta4					12
#define  TPDO_1AB_ATCTime					30

/*PIS*/
#define  TPDO_19A_PISSta1					13
#define  TPDO_19B_PISSta2					14

/*VVVF*/
#define  TDPO_19C_VVVFSta1				15
#define  TDPO_19D_VVVFSta2				16
#define  TDPO_19E_VVVFSta3				17
#define  TDPO_19F_VVVFSta4				18
#define  TDPO_1A0_VVVFSta5				19

#define  TDPO_1A1_VVVFSta1				20
#define  TDPO_1A2_VVVFSta2				21 
#define  TDPO_1A3_VVVFSta3				22
#define  TDPO_1A4_VVVFSta4				23
#define  TDPO_1A5_VVVFSta5				24  

/*SIV*/
#define  TDPO_1A6_SIVSta1					25
#define  TDPO_1A7_SIVSta2					26
#define  TDPO_1A8_SIVSta3					27

#endif //__MAIN_H__
