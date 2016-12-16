#ifndef __SIVCOMPRO_H__
#define __SIVCOMPRO_H__

#include "TMS_ComunDefine.h"

#define  SIV_RX_BUF_SIZE  			20  	//������� ���ݻ������ĳ���
#define  SIV_TX_BUF_SIZE  			20  	//���巢�����ݻ������ĳ���

#define  SIV_PAD	   						0x7E	//��·PAD
#define  SIV_STARTFLAG					0x7E	//��ʼ��־
#define  SIV_ADDRESS1						0x00	//SIV��ַ��8λ
#define  SIV_ADDRESS2						0x03	//SIV��ַ��8λ
#define  SIV_CONTROLBYTE				0x13	//������
#define  SIV_ENDFLAG						0x7E	//������־

#define  SIV_SDR_TEXTNUM				20
#define  SIV_SDR_COMMAND    		0xE8	//������
#define  SIV_SDR_STATE    			0x80	//״̬��
#define  SIV_SDR_SENDER    			0x30
#define  SIV_SDR_RECEIVER    		0x60
#define  SIV_SDR_NO          		0x00

#define  SIV_SD_TEXTNUM					37
#define  SIV_SD_COMMAND    			0xE9	//������
#define  SIV_SD_STATE    				0x80	//״̬��
#define  SIV_SD_SENDER    			0x60
#define  SIV_SD_RECEIVER  	 		0x30
#define  SIV_SD_NO         	 		0x00

#define  SIV_TDR_TEXTNUM				10
#define  SIV_TDR_COMMAND    		0xE2	//������
#define  SIV_TDR_STATE    			0x80	//״̬��
#define  SIV_TDR_SENDER    			0x30
#define  SIV_TDR_RECEIVER	    	0x60
#define  SIV_TDR_NO         	 	0x00

#define  SIV_TD_TEXTNUM					110
#define  SIV_TD_COMMAND    			0xE3	//������
#define  SIV_TD_STATE    				0x80	//״̬��
#define  SIV_TD_SENDER    			0x60
#define  SIV_TD_RECEIVER  		  0x30
#define  SIV_TD_NO         	 		0x00

#define  SIV_TCR_TEXTNUM				10
#define  SIV_TCR_COMMAND    		0xE6	//������
#define  SIV_TCR_STATE    			0x80	//״̬��
#define  SIV_TCR_SENDER    			0x30
#define  SIV_TCR_RECEIVER	    	0x60
#define  SIV_TCR_NO         	 	0x00
#define  SIV_RECORD_KIND				0x02

/*���������������*/
#define  SIV_TCR_HEADER_DATA		1
#define  SIV_TCR_TRACE_DATA			2
#define  SIV_TCR_HDL_DATA				4
#define  SIV_TCR_ALL_DATA				7

/*����ģʽ*/
#define  SIV_SEND_SDR						1
#define  SIV_SEND_TDR						2
#define  SIV_SEND_TCR						3

/*����������ֵ*/
#define  SIV_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  SIV_GetTimeoutError 		-2			//��������֮��20ms����û�н��յ�����
#define  SIV_FrameError					-3			//��⵽֡������żУ�����ʱ
#define  SIV_CRCError         	-4   		//BCC ���Ų����ϼƻ�ֵʱ
#define  SIV_GetError						-5			//��ʾ�������ݴ���
#define  SIV_TransException			-6			//��ʾ�����쳣

typedef struct 
{
	u8	TextNumber;			//�ı���
	u8	Command;				//����	E8h
	u8  State;					//״̬
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������   60hSIV
	u8	OrderNo;				//���NO   00h	
	
	u8 Year;						//��	00~99����������2 λ���� BCD
	u8 Month;						//��	BCD
	u8 Day;							//��	BCD
	u8 Hour;						//ʱ	BCD
	u8 Minute;					//��	BCD
	u8 Second;					//��	BCD
	
	u8 SIV_TimeEffect		:1;			//ʱ��������Ч
	u8 SIV_TimeSet			:1;			//ʱ����������
	u8 Byte13Undef6bit	:6;			//δʹ��6λ
	
	u8 Spare[5];								//00h
}SIVSdrText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//03h	
	u8 Control;					//13h
	SIVSdrText SIVSdrData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}SIVSdrFrame;

typedef struct
{
	/*byte 0*/
	u8	TextNumber;			//�ı��� 37
	u8  Command;				//���� E9h
	u8  State;					//״̬ 80h
	u8  Sender;					//������		 60hSIV
	u8  Receiver; 			//������  	 30h���װ��
	u8	OrderNo;				//���NO   00h	
	

	u8 FaultCode;				//���ϴ���
	u8 Year;						//��	00~99����������2 λ���� BCD
	u8 Month;						//��	BCD
	u8 Day;							//��	BCD
	u8 Hour;						//ʱ	BCD
	u8 Minute;					//��	BCD
	u8 Second;					//��	BCD
	
	/*byte8 */
	u8 Byte8Bit0Undef		:1;//��
	u8 SIV_CGK					:1;//(Answer from CGK)
	u8 SIV_HB						:1;//(Answer from HB)
	u8 SIV_DCHKIN				:1;//(Answer from DCHK)
	u8 SIV_ORSTIN				:1;//(Reset in cab)
	u8 SIV_IRSTIN				:1;//(Reset in SIV equipment)
	u8 SIV_TR1					:1;//(Answer from 3 phase contactor)
	u8 SIV_TSWT					:1;//(In testing without power )
	
	/*byte9*/
	u8 SIV_RY0DCHK			:1;//(Excitation of DCHK)
	u8 SIV_RY1HB				:1;//(Excitation of HB)
	u8 SIV_RY2MKAR1			:1;//(Excitation of MKAR1)
	u8 SIV_RY3TR1				:1;//(Excitation of TR1)
	u8 SIV_RY4CGK				:1;//(Excitation of CGK)
	u8 SIV_RY5HEATER		:1;//(Excitation of heater relay)
	u8 SIV_RY6HBHOLD		:1;//RY6_HBHOLD
	u8 SIV_RY7HBHOLD		:1;//(Command to hold HB)

	/*byte10*/
	u8 SIV_MOGI					:1;//MOGI(Occurrence of simulated failure (manual))
	u8 SIV_GAOK					:1;//GAOK(Normal gate power source)
	u8 Byte10Bit2Undef	:1;//
	u8 SIV_NVD					:1;//NVD(No voltage detection of line voltage)
	u8 Byte10Bit4Undef	:1;
	u8 Byte10Bit5Undef	:1;
	u8 SIV_INVLMT				:1;//INV_LMT(In selective interruption mode of inverter load)
	u8 SIV_INVRUN				:1;//INV_RUN(In operation of inverter)
	
	/*byte11*/
	u8 SIV_FAULT				:1;//FAULT(Occurrence of failure)
	u8 SIV_FAULT2				:1;//FAULT_2(Occurrence of minor failure)
	u8 SIV_FAULT1				:1;//FAULT_1(Occurrence of minor failure)
	u8 SIV_FaultM1			:1;//FAULT_M1(In monitoring state of minor failure (for 120 sec) after automatic resetting)
	u8 SIV_FaultM2			:1;//FAULT_M2(Twice successive occurrence of minor failures (doubled minor failure))	
	u8 Byte11Bit5Undef	:1;//
	u8 SIV_RHD					:1;//RHD(In the keeping state of time delay for minimum starting interval)
	u8 SIV_ORSTM				:1;//ORST_M(Reset is used in cab)
	
	/*byte12*/
	u8 SIV_HBT					:1;//HBT(HB�ѿ�)
	u8 Byte12Bit1Undef	:1;//
	u8 SIV_OUTOV				:1;//OUTOV���������ѹ��
	u8 SIV_OUTLV				:1;//OUTLV������͵�ѹ��
	u8 SIV_IVOC					:1;//IVOC�������������
	u8 SIV_OUTOC				:1;//OUTOC�������������
	u8 SIV_FCOV					:1;//FCOV���˲�����������ѹ��
	u8 SIV_DIFF					:1;//DIF_F

	/*byte13*/
	u8 SIV_THR					:1;//THR��IGBT��ȴ�����ȣ�
	u8 SIV_MKF					:1;//MKF��MK���ϣ�
	u8 SIV_WDC					:1;//WD_C��CPUֹͣ��
	u8 SIV_CTF					:1;//CTF��CT���ϣ�
	u8 SIV_FRQF					:1;//FRQF�����Ƶ���쳣��
	u8 SIV_PWF					:1;//PWF�����Ƶ�Դ�쳣��
	u8 SIV_FCF					:1;//FCF���˲��������쳣��
	u8 SIV_RCDF					:1;//RCDF���������쳣��

	u8 SIV_VL;						//���ߵ�ѹ
	u8 Byte15Spare;				//��
	u8 SIV_VOUT;					//�����ѹ
	u8 SIV_IOUT;					//�������
	u8 SIV_VFC;						//�˲���������ѹFilterCapacitorVolt
	u8 SIV_DC;						//���Ƶ�Դ��ѹ
	u8 SIV_FRQ;						//���Ƶ��
	u8 SIV_ATMP;					//�Ŵ����¶�
	u8 SIV_PVTMP;					//���ʵ�λ(V ��) �¶�
	
	u8 Undef6Byte[6];
	
	u8 SIV_TraceDataNum;	//�������ݼ���
	u8 SIV_HeaderDataNum; //�������ݼ���
	u8 SIV_HDLDataNum;		//HDL ���ݼ���
	u8 Byte32Spare;
	
}SIVSdText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD	
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//03h	
	u8 Control;					//13h
	SIVSdText 	SIVSdData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E
}SIVSdFrame;

typedef struct
{		
	u8	TextNumber;					//�ı���
	u8	Command;						//����E2h
	u8  State;							//״̬		 80h
	u8  Sender;							//������		 30h���װ��
	u8  Receiver; 					//������   60hSIV
	u8	OrderNo;						//���NO   00h	
		
	u8 SIV_CategoryCode;		//������No 00 - 03h
	u8 SIV_TDBlackNoRqH8;		//00h
	u8 SIV_TDBlackNoRqL8;		//00h - 104h
	u8 SIV_BlockNum;				//����
	
	u8 Undef9Byte[9];
}SIVTdrText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD	
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//03h	
	u8 Control;					//13h
	SIVTdrText 	SIVTdrData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E	
}SIVTdrFrame;

typedef struct
{
	u8	TextNumber;			//�ı��� 110
	u8  Command;					//����E3h
	u8  State;					//״̬ 80h
	u8  Sender;					//������		 60hSIV
	u8  Receiver; 			//������  	 30h���װ��
	u8	OrderNo;				//���NO   00h	

	u8 SIV_RecordID;				//��¼����No 00- 03h
	u8 SIV_TDBlackNoRqH8;		//00h
	u8 SIV_TDBlackNoRqL8;		//00h - 104h
	u8 SIV_BlockNum;	
	u8 Undef1Byte[1];
	
	u8 TraceData[100];
}SIVTdText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD	
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//03h	
	u8 Control;					//13h
	SIVTdText 	SIVTdData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E	
}SIVTdFrame;

typedef struct
{
	SIVSdText SIVTdHeaderData;
}SIVTdHeader;

typedef struct
{
	/*byte1 */
	u8 Byte1Bit0Undef		:1;//��
	u8 SIV_CGK					:1;//(Answer from CGK)
	u8 SIV_H						:1;//(Answer from HB)
	u8 SIV_DCHKIN				:1;//(Answer from DCHK)
	u8 SIV_ORSTIN				:1;//(Reset in cab)
	u8 SIV_IRSTIN				:1;//(Reset in SIV equipment)
	u8 SIV_TR						:1;//(Answer from 3 phase contactor)
	u8 SIV_TSWT					:1;//(In testing without power ) 
	
	/*byte2*/
	u8 SIV_TSP					:1;//TSP
	u8 SIV_GAOK					:1;//GAOK
	u8 Byte2Bit2Undef		:1;
	u8 Byte2Bit3Undef		:1;
	u8 Byte2Bit4Undef		:1;	
	u8 SIV_RMTWDT				:1;//RMT_WDT
	u8 SIV_CTLWDT				:1;//CTL_WDT
	u8 SIV_SYSWDT				:1;//SYS_WDT 
	
	/*byte3*/
	u8 SIV_VFCLVD				:1;//VFC_LVD
	u8 SIV_DCTCTF				:1;//DCT_CTF
	u8 SIV_WDSYS				:1;//WD_SYS
	u8 SIV_STD					:1;//STD
	u8 SIV_VFCOVD				:1;//VFC_OVD
	u8 SIV_CTLOUTOC			:1;//CTL_OUTOC
	u8 Byte3Bit6Undef		:1;
	u8 Byte3Bit7Undef		:1;
	
	/*byte4*/
	u8 SIV_IWOCD				:1;//IW_OCD
	u8 SIV_IVOCD				:1;//IV_OCD
	u8 SIV_IUOCD				:1;//IU_OCD 
	u8 Byte4Bit3Undef		:1;
	u8 SIV_OPEN					:1;//OPEN
	u8 SIV_LOADRELEASE	:1;//LOAD_RELEASE
	u8 Byte4Bit6Undef		:1;
	u8 Byte4Bit7Undef		:1;	
	
	/*byte5*/
	
	u8 SIV_VL200V				:1;//VL_200V
	u8 SIV_VL500V				:1;//VL_500V
	u8 SIV_VL1200V			:1;//VL_1200V
	u8 SIV_VFC200V			:1;//VFC_200V
	u8 SIV_VFC500				:1;//VF_C500
	u8 SIV_VOUT494V			:1;//VOUT_494V
	u8 SIV_VOUT449V			:1;//VOUT_449V
	u8 SIV_VOUT342V			:1;//VOUT_342V
	
	/*byte6*/
	u8 SIV_VOUT300V			:1;//VOUT_300V
	u8 SIV_DC77V				:1;//DC_77V
	u8 SIV_DC50V				:1;//DC_50V
	u8 SIV_TA75C				:1;//TA_75C
	u8 SIV_TFWNeg20C		:1;//TFW_-20C
	u8 SIV_TFVNeg20C		:1;//TFV_-20C
	u8 SIV_TFUNeg20C		:1;//TFU_-20C
	u8 SIV_TANeg10C			:1;//TA_-10C
	
	/*byte7*/
	u8 SIV_VDC50				:1;//VDC_50
	u8 SIV_VFC88				:1;//VFC_88
	u8 SIV_VFC50				:1;//VFC_50
	u8 SIV_OUTOCTHR			:1;//OUTOC_THR
	u8 Byte7Bit4Undef		:1;//
	u8 Byte7Bit5Undef		:1;//
	u8 SIV_FRQ55				:1;//FRQ_55
	u8 SIV_FRQ45				:1;//FRQ_45
	
	/*byte8*/
	u8 SIV_RHDT					:1;//RHD_T 
	u8 SIV_RHDONT				:1;//RHD_ON_T
	u8 SIV_DCHKOFFT			:1;//DCHK_OFF_T
	u8 SIV_CGKONT				:1;//CGK_ON_T
	u8 SIV_HBOFFT				:1;//HB_OFF_T 
	u8 SIV_HBONT				:1;//HB_ON_T
	u8 SIV_HBHOLTT			:1;//HB_HOLT_T
	u8 SIV_HBHOLDT			:1;//HB_HOLD_T
	
	/*byte9*/
	u8 Byte9Bit0Undef		:1;
	u8 SIV_INVLMTT			:1;//INV_LMT_T
	u8 Byte9Bit2Undef		:1;//
	u8 SIV_RST3T				:1;//RST3_T
	u8 SIV_RST2T				:1;//RST2_T
	u8 SIV_RST12T				:1;//RST12_T
	u8 SIV_RST11T				:1;//RST11_T
	u8 SIV_DV100T				:1;//DV100_T
	
	/*byte10*/
	u8 SIV_FRQFT				:1;//FRQF_T
	u8 SIV_OUTLVT1			:1;//OUTLV_T1
	u8 SIV_OUTLVT2			:1;//OUTLV_T2
	u8 SIV_HBTT					:1;//HBT_T
	u8 SIV_MKFT					:1;//MKF_T
	u8 Byte10Bit5Undef	:1;//
	u8 Byte10Bit6Undef	:1;//
	u8 SIV_THRT					:1;//THR_T
	
	/*byte11*/
	u8 SIV_M1ONT				:1;//M1_ON_T
	u8 SIV_M1OFFT				:1;//M1_OFF_T
	u8 SIV_RDT					:1;//RD_T
	u8 SIV_RCT					:1;//RC_T
	u8 SIV_TRONT				:1;//TR_ON_T
	u8 SIV_GAT					:1;//GA_T
	u8 SIV_WDCOFFT			:1;//WD_C_OFF_T
	u8 SIV_WDCONT				:1;//WD_C_ON_T
	
	/*byte12*/
	u8 SIV_FAULTM2T			:1;//FAULTM2_T
	u8 SIV_WDST					:1;//WD_S_T
	u8 SIV_OUTOVT				:1;//OUTOV_T
	u8 Byte12Bit3Undef	:1;//
	u8 Byte12Bit4Undef	:1;//
	u8 Byte12Bit5Undef	:1;//
	u8 SIV_WDROFFT			:1;//WD_R_OFF_T
	u8 SIV_WDRONT				:1;//WD_R_ON_T
	
	/*byte13*/
	u8 SIV_FAULT				:1;//FAULT(Occurrence of failure)
	u8 SIV_FAULT2				:1;//FAULT_2(Occurrence of minor failure)
	u8 SIV_FAULT1				:1;//FAULT_1(Occurrence of minor failure)
	u8 SIV_FaultM1			:1;//FAULT_M1(In monitoring state of minor failure (for 120 sec) after automatic resetting)
	u8 SIV_FaultM2			:1;//FAULT_M2(Twice successive occurrence of minor failures (doubled minor failure))	
	u8 Byte13Bit5Undef	:1;//
	u8 SIV_RHD					:1;//RHD(In the keeping state of time delay for minimum starting interval)
	u8 SIV_ORSTM				:1;//ORST_M(Reset is used in cab)	
	
	/*byte14*/
	u8 SIV_RHDF2				:1;//RHD_F2
	u8 SIV_RHDF1				:1;//RHD_F1
	u8 IV_M1M						:1;//M1_M
	u8 SIV_GATECHK			:1;//GATE_CHK
	u8 SIV_ORST					:1;//ORST
	u8 SIV_IRST					:1;//IRST
	u8 SIV_DCHK					:1;//DCHK
	u8 SIV_NVD					:1;//NVD
	
	/*byte15*/
	u8 SIV_SELSTM				:1;//SEL_ST_M
	u8 SIV_RUMM					:1;//RUM_M
	u8 SIV_CGKM					:1;//CGK_M
	u8 Byte15Bite3Undef	:1;//
	u8 SIV_INVRUNM			:1;//INV_RUN_M
	u8 Byte15Bite5Undef	:1;//
	u8 SIV_DCHKONM			:1;//DCHK_ON_M
	u8 SIV_DCHKM				:1;//DCHK_M
	
	/*byte16*/
	u8 SIV_RST12M				:1;//RST12_M
	u8 SIV_RST2					:1;//RST_2
	u8 SIV_RST1					:1;//RST_1
	u8 SIV_RDFM1				:1;//RDF_M1
	u8 SIV_IRSTM				:1;//IRST_M
	u8 SIV_TRSTM				:1;//TRST_M
	u8 SIV_HBTM					:1;//HBT_M
	u8 SIV_RDFM					:1;//RDF_M
	
	/*byte17*/
	u8 SIV_RY0DCHK			:1;//(Excitation of DCHK)RY0_DCHK
	u8 SIV_RY1HB				:1;//(Excitation of HB)RY1_HB
	u8 SIV_RY2MKAR1			:1;//(Excitation of MKAR1)RY2_MKAR1
	u8 SIV_RY3TR1				:1;//(Excitation of TR1)RY3_TR1
	u8 SIV_RY4CGK				:1;//(Excitation of CGK)RY4_CGK
	u8 SIV_RY5HEATER		:1;//(Excitation of heater relay)RY5_HEATER
	u8 SIV_RY6HBHOLD		:1;//RY6_HBHOLD
	u8 SIV_RY7HBHOLD		:1;//(Command to hold HB)RY7_HBHOLD

	/*byte18*/
	u8 SIV_MOGI					:1;//MOGI(Occurrence of simulated failure (manual))
	u8 Undef5bit				:5;
	u8 SIV_INVLMT				:1;//INV_LMT(In selective interruption mode of inverter load)
	u8 SIV_INVRUN				:1;//INV_RUN(In operation of inverter)	
	
	/*byte19*/
	u8 SIV_HBT					:1;//HBT(HB�ѿ�)
	u8 Byte19Bit1Undef	:1;//
	u8 SIV_OUTOV				:1;//OUTOV���������ѹ��
	u8 SIV_OUTLV				:1;//OUTLV������͵�ѹ��
	u8 SIV_IVOC					:1;//IVOC�������������
	u8 SIV_OUTOC				:1;//OUTOC�������������
	u8 SIV_FCOV					:1;//FCOV���˲�����������ѹ��
	u8 SIV_DIFS					:1;//DIF_S

	/*byte20*/
	u8 SIV_THR					:1;//THR��IGBT��ȴ�����ȣ�
	u8 SIV_MKF					:1;//MKF��MK���ϣ�
	u8 SIV_WDC					:1;//WD_C��CPUֹͣ��
	u8 SIV_CTF					:1;//CTF��CT���ϣ�
	u8 SIV_FRQF					:1;//FRQF�����Ƶ���쳣��
	u8 SIV_PWF					:1;//PWF�����Ƶ�Դ�쳣��
	u8 SIV_FCF					:1;//FCF���˲��������쳣��
	u8 SIV_RCDF					:1;//RCDF���������쳣��

	u8 SIV_VL;						//���ߵ�ѹ
	u8 SIV_ATMP;					//�Ŵ����¶�
	u8 SIV_PUTMP;					//���ʵ�λ(U ��) �¶�
	u8 SIV_PVTMP;					//���ʵ�λ(V ��) �¶�	
	u8 SIV_PWTMP;					//���ʵ�λ(W ��) �¶�
	u8 SIV_VOUT;					//�����ѹ
	u8 SIV_IOUT;					//�������
	u8 SIV_VFC;						//�˲���������ѹFilterCapacitorVolt
	u8 SIV_DC;						//���Ƶ�Դ��ѹ
	u8 SIV_VINS;					//˲ʱ�������
	u8 SIV_IINS;					//˲ʱ���������ת����T0�Ĵ�Ҫ��ֵ)
	u8 SIV_IINV;					//˲ʱ�������(U��)
	
}SIVSampleText;

typedef struct
{
	u8	TextNumber;			//�ı��� 10
	u8  Command;				//0xE6h
	u8  State;					//״̬ 80h
	u8  Sender;					//������		 30h���װ��
	u8  Receiver; 			//������  	 60h��SIV��
	u8	OrderNo;				//���NO   00h		
	u8 	KindOfRecord;		//������ݵ����� 0x02
	
	u8 Undef12Byte[12];
}SIVTcrText;

typedef struct
{
	u8 StartPAD[4];			//��ʼPAD	
	u8 StartFlag;				//0x7E
	u8 Address1;				//00h	
	u8 Address2;				//03h	
	u8 Control;					//13h
	SIVTcrText 	SIVTcrData;
	u8 CRCH8;
	u8 CRCL8;
	u8 EndFlag;					//0x7E	
}SIVTcrFrame;


/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��SIVSdrText,���� SIVSdText,SIVTcrText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
	u8 mode:���͵�����ΪSDR������TDR	TCR; 
����ֵ���޷���ֵ
*/
void SendSIVSdrOrTdrOrTcr(void * pInData,/*u8 * pOutData,*/u8 mode);  


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�SIVSdText�ṹ��)��
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetSIVSdOrTd(u8 *pInData,void *pOutData);

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckSIVGetError(u8 *pInData);

#endif //__SIVCOMPRO_H__
