#ifndef  __VVVFCOMPRO_H__
#define  __VVVFCOMPRO_H__

#include "TMS_ComunDefine.h"

#define  VVVF_RX_BUF_SIZE  			20  	//������� ���ݻ������ĳ���
#define  VVVF_TX_BUF_SIZE  			20  	//���巢�����ݻ������ĳ���

#define VVVF1 0x01
#define VVVF2 0x02

/*���忪ʼ2�ֽڵ�����*/
#define  VVVF_PAD	   						0x7E	//��·PAD
#define  VVVF_STARTFLAG					0x7E	//��ʼ��־
#define  VVVF_ADDRESS1					0x00	//VVVF��ַ��8λ
#define  VVVF_ADDRESS2					0x00	//VVVF��ַ��8λ
#define  VVVF_CONTROLBYTE				0x13	//������
#define  VVVF_ENDFLAG						0x7E	//������־

#define  VVVF_SDR_TEXTNUM				20
#define  VVVF_SDR_COMMAND    		0xE8	//������
#define  VVVF_SDR_STATE    			0x80	//״̬��
#define  VVVF_SDR_SENDER    		0x30
#define  VVVF_SDR_RECEIVER1    	0x40
#define  VVVF_SDR_RECEIVER2    	0x50
#define  VVVF_SDR_NO          	0x00

#define  VVVF_SD_TEXTNUM				41
#define  VVVF_SD_COMMAND    		0xE9	//������
#define  VVVF_SD_STATE    			0x80	//״̬��
#define  VVVF_SD_SENDER1    		0x40
#define  VVVF_SD_SENDER2    		0x50
#define  VVVF_SD_RECEIVER  	  	0x30
#define  VVVF_SD_NO         	 	0x00

#define  VVVF_TDR_TEXTNUM				10
#define  VVVF_TDR_COMMAND    		0xE2	//������
#define  VVVF_TDR_STATE    			0x80	//״̬��
#define  VVVF_TDR_SENDER    		0x30
#define  VVVF_TDR_RECEIVER1    	0x40
#define  VVVF_TDR_RECEIVER2  	  0x50
#define  VVVF_TDR_NO         	 	0x00

#define  VVVF_TD_TEXTNUM				110
#define  VVVF_TD_COMMAND    		0xE3	//������
#define  VVVF_TD_STATE    			0x80	//״̬��
#define  VVVF_TD_SENDER1    		0x40
#define  VVVF_TD_SENDER2    		0x50
#define  VVVF_TD_RECEIVER  		  0x30
#define  VVVF_TD_NO         	 	0x00

#define  VVVF_TCR_TEXTNUM				10
#define  VVVF_TCR_COMMAND    		0xE6	//������
#define  VVVF_TCR_STATE    			0x80	//״̬��
#define  VVVF_TCR_SENDER    		0x30
#define  VVVF_TCR_RECEIVER1    	0x40
#define  VVVF_TCR_RECEIVER2  		0x50
#define  VVVF_TCR_NO         	 	0x00

/*Э���еĹ��ϴ���*/
#define  VVVF_HBT_FAULT					0x01		//HB�ѿ�
#define	 VVVF_CONTROL_FAULT			0x02		//���ƹ���
#define  VVVF_MMOCD_FAULT				0x03		//ǣ���������������
#define  VVVF_FOVD_FAULT				0x04		//FC��ѹ1
#define  VVVF_FLVD_FAULT				0x05		//FC�͵�ѹ
#define  VVVF_SOVD_FAULT				0x06		//FC��ѹ2
#define  VVVF_PGD_FAULT					0x07		//���˼��
#define  VVVF_BSLP_FAULT				0x08		//���ת
#define  VVVF_BSLD_FAULT				0x09		//����
#define  VVVF_LGD_FAULT					0x0B		//�ܿ��߽ӵ�
#define	 VVVF_FMEF_FAULT				0x0C		//PF����
#define  VVVF_THD1_FAULT				0x0A		//��ȴ������
#define  VVVF_BCHF_FAULT				0x0D		//BCHԪ������
#define	 VVVF_T_PWMF_FAULT			0x0E		//T_PWMFָ���쳣	
#define	 VVVF_TEST_FAULT				0x0F		//TESTģ�����
	
	
/*����������ֵ*/
#define  VVVF_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  VVVF_GetTimeoutError 	-2			//��������֮��20ms����û�н��յ�����
#define  VVVF_FrameError				-3			//��⵽֡������żУ�����ʱ
#define  VVVF_CRCError          -4   		//BCC ���Ų����ϼƻ�ֵʱ
#define  VVVF_GetError					-5			//��ʾ�������ݴ���
#define  VVVF_TransException		-6			//��ʾ�����쳣

/*���巢�ͽ���VVVF1����2*/
#define SEND_VVVF1			1
#define SEND_VVVF2			2
#define RECEIVE_VVVF1		1
#define RECEIVE_VVVF2		2

/*����ģʽ*/
#define VVVF_SEND_SDR		1
#define VVVF_SEND_TDR		2
#define VVVF_SEND_TCR		3

typedef struct 
{
	u8	TextNumber;				//�ı���
	u8	Command;				//����	E8h
	u8  State;					//״̬
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������   40hVVVF1,50hVVVF2
	u8	OrderNo;				//���NO   00h				
	
	u8 Year;						//��	00~99����������2 λ���� BCD
	u8 Month;						//��	BCD
	u8 Day;							//��	BCD
	u8 Hour;						//ʱ	BCD
	u8 Minute;					//��	BCD
	u8 Second;					//��	BCD
	
	u8 TCU_TimeEffect		:1;			//ʱ��������Ч
	u8 TCU_TimeSet			:1;			//ʱ����������
	u8 Byte13Undef6bit	:6;			//δʹ��6λ
	
	u8 Ready[8];								//00h
}VVVFSdrText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//00h	
	u8 Control;					//13h
	VVVFSdrText VVVFSdrData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}VVVFSdrFrame;

typedef struct
{
	u8	TextNumber;				//�ı���
	u8	Command;				//����
	u8  State;					//״̬
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������   40hVVVF1,50hVVVF2
	u8	OrderNo;				//���NO   00h		
	
	/*byte7*/
	u8	Byte7bit0Undef		:1;//��
	u8	TCU_RBDRVL				:1;//(���˶��ٻ���)
	u8	TCU_RBDRVM				:1;//(��������)
	u8 	TCU_PWMF					:1;//(Ť��ָ���쳣)
	u8 	TCU_ElectricBrake	:1;//T_PWMF(ָ���쳣)
	u8 	TCU_B							:1;//B(����)
	u8 	TCU_Retral				:1;//R(����)
	u8 	TCU_Forward				:1;//F(ǰ��)

	/*byte8*/
	u8 	TCU_THN3					:1;//THN3
	u8 	Byte8bit1Undef		:1;//��
	u8 	TCU_CWN						:1;//CWN
	u8 	TCU_L_RD					:1;//VVVF1:L2RD, VVVF2:L3RD
	u8 	TCU_L1R_D					:1;//VVVF1:L1R1D, VVVF2:L1R2D
	u8 	TCU_CR_D					:1;//VVVF1:CR1D, VVVF2:CR2D
	u8 	TCU_FDR_D8				:1;//VVVF1:FDR1D, VVVF2:FDR2D	
	u8 	TCU_HBR_D					:1;//VVVF1:HBR1D, VVVF2:HBR2D		 
	
	/*byte9*/
	u8 	Byte9bit0Undef		:1;//��
	u8 	TCU_NRBRD					:1;//NRBRD(������)
	u8 	TCU_UCORSR				:1;//UCORSR
	u8 	TCU_UCOR_D				:1;//VVVF1:UCOR2D, VVVF2:UCOR1D		
	u8 	TCU_FDR_D9				:1;//VVVF1:FDR2D, VVVF2:FDR1D		
	u8 	TCU_L_D9					:1;//VVVF1:L3D, VVVF2:L2D
	u8 	TCU_DFR_D					:1;//VVVF1:DFR1D, VVVF2:DFR2D
	u8 	TCU_UCOR					:1;//UCOR
	
	/*byte10*/
	u8 	TCU_RS						:1;//RS
	u8 	TCU_L_D10					:1;//VVVF1:L2D, VVVF2:L1D
	u8 	TCU_HT						:1;//HT(�߼���)
	u8 	TCU_1A						:1;//1A(ǰ�����ˡ�����)
	u8 	TCU_HB						:1;//HB(HB����)
	u8 	TCU_EB						:1;//EB(�����ƶ�)
	u8 	TCU_L1D						:1;//	L1D
	u8 	TCU_EMCTQ					:1;//EMCTQ(�������д洢)

	/*byte11*/
	u8 	Byte11bit0Undef		:1;//��
	u8 	TCU_KS						:1;//ks
	u8 	TCU_TS1						:1;//TS1(�����ƶ�����)
	u8 	TCU_LFDR					:1;//LFDR(��΢����)
	u8 	TCU_TS2						:1;//TS2(HB�ѿ�����)
	u8	Byte11bit5Undef		:1;
	u8	Byte11bit6Undef		:1;
	u8	Byte11bit7Undef		:1;
		
	/*byte12*/
	u8 	TCU_WDSYS					:1;//WD_SYS
	u8 	TCU_REMF					:1;//REM_F
	u8 	TCU_CONTF					:1;//CONT_F
	u8 	TCU_CFDW					:1;//TCU_CFDW
	u8 	TCU_CFDV					:1;//CFD_V
	u8 	TCU_CFDU					:1;//CFD_U
	u8	Byte12bit6Undef		:1;
	u8	Byte12bit7Undef		:1;
	
	/*byte13*/
	u8 	TCU_PGEA					:1;//PGE_A(PG1����)
	u8 	TCU_PGEB					:1;//PGE_B(PG2����)
	u8 	TCU_PGDA					:1;//PGD_A
	u8 	TCU_PGDB					:1;//PGD_B
	u8 	TCU_SPG1					:1;//SPG1(PGѡ��)
	u8 	TCU_SPG2					:1;//SPG2
	u8 	Byte13bit6Undef		:1;//
	u8 	TCU_BSL						:1;//BSL

	/*byte14*/
	u8 	Byte14bit0Undef		:1;
	u8 	Byte14bit1Undef		:1;	
	u8 	Byte14bit2Undef		:1;
	u8 	TCU_FCLW					:1;//FCL_W
	u8 	TCU_FCLV					:1;//FCL_V
	u8 	TCU_FCLU					:1;//FCL_U
	u8 	Byte14bit6Undef		:1;
	u8 	TCU_FOVD					:1;//FOVD

	/*byte15*/
	u8 	TCU_CNTFDR				:1;//CNT_FDR
	u8 	TCU_CNTHBT				:1;//CNT_HBT
	u8 	TCU_HBOFF					:1;//HB_OFF
	u8 	TCU_MONCWN				:1;//MON_CWN
	u8	Byte15Undef4			:4;
	
	/*byte16*/
	u8 	TCU_CHOPC					:1;//CHOP_C
	u8 	TCU_MRSC					:1;//MRSC
	u8 	TCU_ARSC					:1;//ARSC
	u8 	TCU_GECHOP				:1;//GE_CHOP
	u8 	Byte16Undef3			:3;
	u8 	TCU_HDLTRG				:1;//HDL_TRG

	/*byte17*/
	u8 	TCU_FCF						:1;//FC_F
	u8 	TCU_FRF						:1;//FR_F
	u8 	TCU_MMOCD2F				:1;//MMOCD2_F
	u8 	TCU_PUDF					:1;//PUD_F
	u8 	TCU_CFDF					:1;//CFD_F
	u8 	TCU_HBTF					:1;//HBT_F
	u8 	TCU_RYTF					:1;//RYT_F
	u8 	TCU_FDRF					:1;//FDR_F
	
	/*byte18*/
	u8 	TCU_WDF						:1;//WD_F
	u8 	TCU_OVCRF					:1;//OVCR_F
	u8	TCU_CTLVF					:1;//TCU_CTLVF
	u8 	TCU_THD3F					:1;//THD3_F
	u8 	TCU_GPokF					:1;//GPok_F
	u8 	TCU_D10okF				:1;//D10ok_F
	u8 	TCU_OVTF					:1;//OVT_F
	u8 	TCU_THD2F					:1;//THD2_F

	/*byte19*/
	u8 	TCU_LGDF					:1;//LGD_F(���߽ӵ�)
	u8 	TCU_BSLBF					:1;//BSLB_F
	u8 	TCU_BSLPF					:1;//BSLP_F
	u8 	TCU_BSMF					:1;//BSM_F
	u8 	TCU_LVDF					:1;//LVD_F
	u8 	TCU_SOVDF					:1;//SOVD_F
	u8 	TCU_OVDF					:1;//OVD_F
	u8 	TCU_LOFMF					:1;//TCU_LOFMF
	
	/*byte20*/
	u8 	TCU_THD1F					:1;//THD1_F
	u8 	TCU_OVTC					:1;//OVTC(OVT��ͨ)
	u8 	TCU_DETFMF				:1;//DET_FM_F
	u8 	TCU_CR1F					:1;//CR1_F
	u8 	TCU_BROHF					:1;//BROH_F
	u8 	TCU_TESTF					:1;//TEST_F
	u8 	TCU_SLDM					:1;//SLDM
	u8 	TCU_NVDF					:1;//NVD_F(�޵�ѹ)

	/*other*/
	u8 	TCU_FM;							//FM  �ٶ�
	u8 	TCU_FMC;						//�����ٶ� FMC 
	u8 	TCU_ALPHA;					//��ѹ�� ALPHA 
	u8 	TCU_VC;							//�˲���������ѹVC
	u8 	TCU_LineUValue;			//����������ѹVL
	u8 	TCU_IqP;						//���ز��ֵ���ָ��IqP
	u8 	TCU_TorqueCurrent;	//���ص��� Iq
	u8 	TCU_IdP;						//��ͨ���ֵ���ָ��IdP
	u8 	TCU_Id;							//��ͨ���ֵ���Id
	u8 	TCU_LineIValue;			//��������� IL
	u8 	TCU_FS;							//����Ƶ��FS
	u8 	TCU_PWMSignal;			//PWM�ź�
	u8 	TCU_IM;							//�������ʵЧIM
	u8 	TCU_EP;							//�������ź� EP
	u8 	TCU_PGH;						//PG�����ۼƼ�ʱ������λ��
	u8 	TCU_PGL;						//PG�����ۼƼ�ʱ������λ��
	u8 	TCU_WHMP;						//ǣ����¼������������λWHMP��
	u8 	TCU_WHMB;						//������¼������������λ��WHMB
	u8 	TCU_TraceEventSum;	//�������ݣ��ش���ϣ�����
	u8 	TCU_HeadEventSum;		//�������ݣ���΢���ϣ�����
	u8 	TCU_HDLEventSum	;		//HDL���ݼ���
	u8 	TCU_TE;							//ǣ��/�ƶ����ź�TE
	
}VVVFSdText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//00h	
	u8 Control;					//13h
	VVVFSdText	 VVVFSdData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}VVVFSdFrame;

typedef struct
{
	u8	TextNumber;				//�ı���
	u8	Command;				//����
	u8  State;					//״̬
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������   40hVVVF1,50hVVVF2
	u8	OrderNo;				//���NO   00h		
	u8 	CategoryCode;		//������
	u8 	BlockNumH8;			//��Ÿ�λ  0000h-0140h;
	u8 	BlockNumL8;			//
	u8 	Undef10byte[10];	//����10�ֽ�
}VVVFTdrText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//00h	
	u8 Control;					//13h
	VVVFTdrText	 VVVFTdrData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}VVVFTdrFrame;


typedef struct
{
	u8	FalutCode;			//���ϴ���
	
	u8 Year;						//��	00~99����������2 λ���� BCD
	u8 Month;						//��	BCD
	u8 Day;							//��	BCD
	u8 Hour;						//ʱ	BCD
	u8 Minute;					//��	BCD
	u8 Second;					//��	BCD
	
	u8 TCU_FM_Speed;		//FM  �ٶ�
	
	/*byte8*/
	u8	TCU_1BK						:1;//1BK
	u8	TCU_RBDRVL				:1;//(���˶��ٻ���)
	u8	TCU_RBDRVM				:1;//(��������)
	u8 	TCU_PWMF					:1;//(Ť��ָ���쳣)
	u8 	TCU_ElectricBrake	:1;//T_PWMF(ָ���쳣)
	u8 	TCU_B							:1;//B(����)
	u8 	TCU_Retral				:1;//R(����)
	u8 	TCU_Forward				:1;//F(ǰ��)

	/*byte10*/
	u8 	TCU_THN3					:1;//THN3
	u8 	Byte10bit1Undef		:1;//��
	u8 	TCU_CWN						:1;//CWN
	u8 	TCU_L_RD					:1;//VVVF1:L2RD, VVVF2:L3RD
	u8 	TCU_L1R_D					:1;//VVVF1:L1R1D, VVVF2:L1R2D
	u8 	TCU_CR_D					:1;//VVVF1:CR1D, VVVF2:CR2D
	u8 	TCU_FDR_D8				:1;//VVVF1:FDR1D, VVVF2:FDR2D	
	u8 	TCU_HBR_D					:1;//VVVF1:HBR1D, VVVF2:HBR2D		 
	
	/*byte11*/
	u8 	TCU_ATTC					:1;//ATTC
	u8 	TCU_NRBRD					:1;//NRBRD(������)
	u8 	TCU_UCORSR				:1;//UCORSR
	u8 	TCU_UCOR_D				:1;//VVVF1:UCOR2D, VVVF2:UCOR1D		
	u8 	TCU_FDR_D9				:1;//VVVF1:FDR2D, VVVF2:FDR1D		
	u8 	TCU_L_D9					:1;//VVVF1:L3D, VVVF2:L2D
	u8 	TCU_DFR_D					:1;//VVVF1:DFR1D, VVVF2:DFR2D
	u8 	TCU_UCOR					:1;//UCOR
	
	/*byte12*/
	u8 	TCU_RS						:1;//RS
	u8 	TCU_L_D10					:1;//VVVF1:L2D, VVVF2:L1D
	u8 	TCU_HT						:1;//HT(�߼���)
	u8 	TCU_1A						:1;//1A(ǰ�����ˡ�����)
	u8 	TCU_HB						:1;//HB(HB����)
	u8 	TCU_EB						:1;//EB(�����ƶ�)
	u8 	TCU_L1D						:1;//	L1D
	u8 	TCU_EMCTQ					:1;//EMCTQ(�������д洢)

	/*byte13*/
	u8 	Byte13bit0Undef		:1;//��
	u8 	TCU_KS						:1;//ks
	u8 	TCU_TS1						:1;//TS1(�����ƶ�����)
	u8 	TCU_LFDR					:1;//LFDR(��΢����)
	u8 	TCU_TS2						:1;//TS2(HB�ѿ�����)
	u8	Byte13bit5Undef		:1;
	u8	Byte13bit6Undef		:1;
	u8	Byte13bit7Undef		:1;
		
	/*byte14*/
	u8 	TCU_WDSYS					:1;//WD_SYS
	u8 	TCU_REMF					:1;//REM_F
	u8 	TCU_CONTF					:1;//CONT_F
	u8 	TCU_CFDW					:1;//TCU_CFDW
	u8 	TCU_CFDV					:1;//CFD_V
	u8 	TCU_CFDU					:1;//CFD_U
	u8	Byte14bit6Undef		:1;
	u8	Byte14bit7Undef		:1;
	
	/*byte15*/
	u8 	TCU_PGEA					:1;//PGE_A(PG1����)
	u8 	TCU_PGEB					:1;//PGE_B(PG2����)
	u8 	TCU_PGDA					:1;//PGD_A
	u8 	TCU_PGDB					:1;//PGD_B
	u8 	TCU_SPG1					:1;//SPG1(PGѡ��)
	u8 	TCU_SPG2					:1;//SPG2
	u8 	Byte15bit6Undef		:1;//
	u8 	TCU_BSL						:1;//BSL

	/*byte16*/
	u8 	Byte16bit0Undef		:1;
	u8 	Byte16bit1Undef		:1;	
	u8 	Byte16bit2Undef		:1;
	u8 	TCU_FCLW					:1;//FCL_W
	u8 	TCU_FCLV					:1;//FCL_V
	u8 	TCU_FCLU					:1;//FCL_U
	u8 	Byte16bit6Undef		:1;
	u8 	TCU_FOVD					:1;//FOVD

	/*byte17*/
	u8 	TCU_CNTFDR				:1;//CNT_FDR
	u8 	TCU_CNTHBT				:1;//CNT_HBT
	u8 	TCU_HBOFF					:1;//HB_OFF
	u8 	TCU_MONCWN				:1;//MON_CWN
	u8 	TCU_V_CHOP				:1;//V_CHOP
	u8	Byte17Undef2			:2;
	u8 	TCU_HDL_TRG				:1;//HDL_TRG	
	
	/*byte17*/
	u8 	TCU_CHOPC					:1;//CHOP_C
	u8 	TCU_MRSC					:1;//MRSC
	u8 	TCU_ARSC					:1;//ARSC
	u8 	TCU_GECHOP				:1;//GE_CHOP
	u8 	Byte16Undef3			:3;
	u8 	TCU_HDLTRG				:1;//HDL_TRG

	/*byte18*/
	u8 	TCU_FCF						:1;//FC_F
	u8 	TCU_FRF						:1;//FR_F
	u8 	TCU_MMOCD2F				:1;//MMOCD2_F
	u8 	TCU_PUDF					:1;//PUD_F
	u8 	TCU_CFDF					:1;//CFD_F
	u8 	TCU_HBTF					:1;//HBT_F
	u8 	TCU_RYTF					:1;//RYT_F
	u8 	TCU_FDRF					:1;//FDR_F
	
	/*byte19*/
	u8 	TCU_WDF						:1;//WD_F
	u8 	TCU_OVCRF					:1;//OVCR_F
	u8	TCU_CTLVF					:1;//TCU_CTLVF
	u8 	TCU_THD3F					:1;//THD3_F
	u8 	TCU_GPokF					:1;//GPok_F
	u8 	TCU_D10okF				:1;//D10ok_F
	u8 	TCU_OVTF					:1;//OVT_F
	u8 	TCU_THD2F					:1;//THD2_F

	/*byte20*/
	u8 	TCU_LGDF					:1;//LGD_F(���߽ӵ�)
	u8 	TCU_BSLBF					:1;//BSLB_F
	u8 	TCU_BSLPF					:1;//BSLP_F
	u8 	TCU_BSMF					:1;//BSM_F
	u8 	TCU_LVDF					:1;//LVD_F
	u8 	TCU_SOVDF					:1;//SOVD_F
	u8 	TCU_OVDF					:1;//OVD_F
	u8 	TCU_LOFMF					:1;//TCU_LOFMF
	
	/*byte21*/
	u8 	TCU_THD1F					:1;//THD1_F
	u8 	TCU_OVTC					:1;//OVTC(OVT��ͨ)
	u8 	TCU_DETFMF				:1;//DET_FM_F
	u8 	TCU_CR1F					:1;//CR1_F
	u8 	TCU_BROHF					:1;//BROH_F
	u8 	TCU_TESTF					:1;//TEST_F
	u8 	TCU_SLDM					:1;//SLDM
	u8 	TCU_NVDF					:1;//NVD_F(�޵�ѹ)
	
	/*byte22*/
	u8 	TCU_ContModeM			:1;//ContModeM
	u8 	Byte22Undef3bit		:3;
	u8	TCU_P_MODE				:4;//TCU_P_MODE
	
	/*byte23*/
	u8 	TCU_ATTM					:1;//ATTM	
	u8 	TCU_CWN_D					:1;//CWN_D	
	u8 	TCU_BCH_C					:1;//BCH_C
	u8 	TCU_FMR						:1;//FMR	
	u8 	TCU_RMR						:1;//RMR
	u8 	TCU_BM						:1;//BM
	u8	TCU_PM						:1;//PM
	u8 	CWN_M							:1;//CWN_M
	
	u8 	Undef9Byte[9];		
	
}VVVFTdHeardText;

typedef struct
{
	/*byte1*/
	u8	TCU_1BK						:1;//1BK
	u8	TCU_RBDRVL				:1;//(���˶��ٻ���)
	u8	TCU_RBDRVM				:1;//(��������)
	u8 	TCU_PWMF					:1;//(Ť��ָ���쳣)
	u8 	TCU_ElectricBrake	:1;//T_PWMF(ָ���쳣)
	u8 	TCU_B							:1;//B(����)
	u8 	TCU_Retral				:1;//R(����)
	u8 	TCU_Forward				:1;//F(ǰ��)

	/*byte2*/
	u8 	TCU_THN3					:1;//THN3
	u8 	Byte2bit1Undef		:1;//��
	u8 	TCU_CWN						:1;//CWN
	u8 	TCU_L_RD					:1;//VVVF1:L2RD, VVVF2:L3RD
	u8 	TCU_L1R_D					:1;//VVVF1:L1R1D, VVVF2:L1R2D
	u8 	TCU_CR_D					:1;//VVVF1:CR1D, VVVF2:CR2D
	u8 	TCU_FDR_D8				:1;//VVVF1:FDR1D, VVVF2:FDR2D	
	u8 	TCU_HBR_D					:1;//VVVF1:HBR1D, VVVF2:HBR2D		 
	
	/*byte3*/
	u8 	TCU_ATTC					:1;//ATTC
	u8 	TCU_NRBRD					:1;//NRBRD(������)
	u8 	TCU_UCORSR				:1;//UCORSR
	u8 	TCU_UCOR_D				:1;//VVVF1:UCOR2D, VVVF2:UCOR1D		
	u8 	TCU_FDR_D9				:1;//VVVF1:FDR2D, VVVF2:FDR1D		
	u8 	TCU_L_D9					:1;//VVVF1:L3D, VVVF2:L2D
	u8 	TCU_DFR_D					:1;//VVVF1:DFR1D, VVVF2:DFR2D
	u8 	TCU_UCOR					:1;//UCOR
	
	/*byte4*/
	u8 	TCU_RS						:1;//RS
	u8 	TCU_L_D10					:1;//VVVF1:L2D, VVVF2:L1D
	u8 	TCU_HT						:1;//HT(�߼���)
	u8 	TCU_1A						:1;//1A(ǰ�����ˡ�����)
	u8 	TCU_HB						:1;//HB(HB����)
	u8 	TCU_EB						:1;//EB(�����ƶ�)
	u8 	TCU_L1D						:1;//	L1D
	u8 	TCU_EMCTQ					:1;//EMCTQ(�������д洢)

	/*byte5*/
	u8 	Byte5bit0Undef		:1;//��
	u8 	TCU_KS						:1;//ks
	u8 	TCU_TS1						:1;//TS1(�����ƶ�����)
	u8 	TCU_LFDR					:1;//LFDR(��΢����)
	u8 	TCU_TS2						:1;//TS2(HB�ѿ�����)
	u8	Byte5bit5Undef		:1;
	u8	Byte5bit6Undef		:1;
	u8	Byte5bit7Undef		:1;
		
	/*byte6*/
	u8 	TCU_WDSYS					:1;//WD_SYS
	u8 	TCU_REMF					:1;//REM_F
	u8 	TCU_CONTF					:1;//CONT_F
	u8 	TCU_CFDW					:1;//TCU_CFDW
	u8 	TCU_CFDV					:1;//CFD_V
	u8 	TCU_CFDU					:1;//CFD_U
	u8	Byte6bit6Undef		:1;
	u8	Byte6bit7Undef		:1;
	
	/*byte7*/
	u8 	TCU_PGEA					:1;//PGE_A(PG1����)
	u8 	TCU_PGEB					:1;//PGE_B(PG2����)
	u8 	TCU_PGDA					:1;//PGD_A
	u8 	TCU_PGDB					:1;//PGD_B
	u8 	TCU_SPG1					:1;//SPG1(PGѡ��)
	u8 	TCU_SPG2					:1;//SPG2
	u8 	Byte7bit6Undef		:1;//
	u8 	TCU_BSL						:1;//BSL

	/*byte8*/
	u8 	Byte8bit0Undef		:1;
	u8 	Byte8bit1Undef		:1;	
	u8 	Byte8bit2Undef		:1;
	u8 	TCU_FCLW					:1;//FCL_W
	u8 	TCU_FCLV					:1;//FCL_V
	u8 	TCU_FCLU					:1;//FCL_U
	u8 	Byte8bit6Undef		:1;
	u8 	TCU_FOVD					:1;//FOVD

//	/*byte9*/		?????
//	u8 	TCU_CNTFDR				:1;//CNT_FDR
//	u8 	TCU_CNTHBT				:1;//CNT_HBT
//	u8 	TCU_HBOFF					:1;//HB_OFF
//	u8 	TCU_MONCWN				:1;//MON_CWN
//	u8 	TCU_V_CHOP				:1;//V_CHOP
//	u8	Byte9Undef2			:2;
//	u8 	TCU_HDL_TRG				:1;//HDL_TRG	
	
	/*byte9*/
	u8 	TCU_CHOPC					:1;//CHOP_C
	u8 	TCU_MRSC					:1;//MRSC
	u8 	TCU_ARSC					:1;//ARSC
	u8 	TCU_GECHOP				:1;//GE_CHOP
	u8 	Byte10Undef3			:3;
	u8 	TCU_HDLTRG				:1;//HDL_TRG

	/*byte10*/
	u8 	TCU_FCF						:1;//FC_F
	u8 	TCU_FRF						:1;//FR_F
	u8 	TCU_MMOCD2F				:1;//MMOCD2_F
	u8 	TCU_PUDF					:1;//PUD_F
	u8 	TCU_CFDF					:1;//CFD_F
	u8 	TCU_HBTF					:1;//HBT_F
	u8 	TCU_RYTF					:1;//RYT_F
	u8 	TCU_FDRF					:1;//FDR_F
	
	/*byte11*/
	u8 	TCU_WDF						:1;//WD_F
	u8 	TCU_OVCRF					:1;//OVCR_F
	u8	TCU_CTLVF					:1;//TCU_CTLVF
	u8 	TCU_THD3F					:1;//THD3_F
	u8 	TCU_GPokF					:1;//GPok_F
	u8 	TCU_D10okF				:1;//D10ok_F
	u8 	TCU_OVTF					:1;//OVT_F
	u8 	TCU_THD2F					:1;//THD2_F

	/*byte12*/
	u8 	TCU_LGDF					:1;//LGD_F(���߽ӵ�)
	u8 	TCU_BSLBF					:1;//BSLB_F
	u8 	TCU_BSLPF					:1;//BSLP_F
	u8 	TCU_BSMF					:1;//BSM_F
	u8 	TCU_LVDF					:1;//LVD_F
	u8 	TCU_SOVDF					:1;//SOVD_F
	u8 	TCU_OVDF					:1;//OVD_F
	u8 	TCU_LOFMF					:1;//TCU_LOFMF
	
	/*byte13*/
	u8 	TCU_THD1F					:1;//THD1_F
	u8 	TCU_OVTC					:1;//OVTC(OVT��ͨ)
	u8 	TCU_DETFMF				:1;//DET_FM_F
	u8 	TCU_CR1F					:1;//CR1_F
	u8 	TCU_BROHF					:1;//BROH_F
	u8 	TCU_TESTF					:1;//TEST_F
	u8 	TCU_SLDM					:1;//SLDM
	u8 	TCU_NVDF					:1;//NVD_F(�޵�ѹ)
	
	/*byte14*/
	u8 	TCU_ContModeM			:1;//ContModeM
	u8 	Byte22Undef3bit		:3;
	u8	TCU_P_MODE				:4;//TCU_P_MODE
	
		/*other*/
	u8 	TCU_FM;							//FM  �ٶ�
	u8 	TCU_FMC;						//�����ٶ� FMC 
	u8 	TCU_ALPHA;					//��ѹ�� ALPHA 
	u8 	TCU_VC;							//�˲���������ѹVC
	u8 	TCU_LineUValue;			//����������ѹVL
	u8  TCU_TqP;						//����ָ��TqP
	u8 	TCU_Tq;							//�������Tq
	u8 	TCU_IqP;						//���ز��ֵ���ָ��IqP
	u8 	TCU_TorqueCurrent;	//���ص��� Iq
	u8 	TCU_IdP;						//��ͨ���ֵ���ָ��IdP
	u8 	TCU_Id;							//��ͨ���ֵ���Id
	u8 	TCU_LineIValue;			//��������� IL
	u8 	TCU_FS;							//����Ƶ��FS
	u8	TCU_LA;							//�����ź�LA
	u8 	TCU_DA;							//���ƶ�ָ��DA
	u8 	TCU_EP;							//�������ź� EP
	u8 	TCU_TE;							//ǣ��/�ƶ����ź�TE
	u8 	TCU_PWMSignal;			//PWM�ź�
	
}VVVFSampleText;

typedef struct
{
	u8	TextNumber;				//�ı���
	u8	Command;				//����
	u8  State;					//״̬
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������   40hVVVF1,50hVVVF2
	u8	OrderNo;				//���NO   00h		
	u8 	CategoryCode;		//������
	u8 	BlockNumH8;			//��Ÿ�λ  0000h-0140h;
	u8 	BlockNumL8;			//
	
	u8 	Undef1;					//00h
	u8 	Undef2;
	
	u8	TraceData[100]; 
}VVVFTdText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//00h	
	u8 Control;					//13h
	VVVFTdText	 VVVFTdData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}VVVFTdFrame;

typedef struct
{
	u8	TextNumber;				//�ı���
	u8	Command;				//����
	u8  State;					//״̬
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������   40hVVVF1,50hVVVF2
	u8	OrderNo;				//���NO   00h		
	u8 	CategoryCode;		//������  02h
	
	u8	Undef12Byte[12];
}VVVFTcrText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//00h	
	u8 Control;					//13h
	VVVFTcrText	 VVVFTcrData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}VVVFTcrFrame;


/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��VVVFSdrText,���� VVVFSdText,VVVFTcrText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
	u8 mode:���͵�����ΪSDR������TDR	TCR; 
����ֵ���޷���ֵ
*/
void SendVVVFSdrOrTdrOrTcr(void * pInData,/*u8 * pOutData,*/u8 mode);  


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_TMS_BRAKE_SD_Text�ṹ��)��
	VVVF_No: ��ʾ��ȡVVVF1������VVVF2
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetVVVFSdOrTd(u8 *pInData,void *pOutData,u8 VVVF_No);

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
			VVVF_No: ��ʾ��ȡVVVF1������VVVF2
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckVVVFGetError(u8 *pInData,u8 VVVF_No);

#endif //__VVVFCOMPRO_H__
