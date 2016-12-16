#ifndef __ATCCOMPRM_H__
#define __ATCCOMPRM_H__

#include "TMS_ComunDefine.h"

#define  ATC_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  ATC_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

#define  ATC_DLE							0x10
#define  ATC_STX							0x02	//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x20
#define  ATC_ETX							0x03

#define  ATC_SDR_TEXTCODEH8		0x05
#define  ATC_SDR_TEXTCODEL8		0x0F

/*����������ֵ*/
#define  ATC_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  ATC_GetTimeoutError	  -2			//��������֮��20ms����û�н��յ�����
#define  ATC_FrameError					-3			//��⵽֡������żУ�����ʱ
#define  ATC_BCCError       	  -4  	 	//BCC ���Ų����ϼƻ�ֵʱ
#define  ATC_GetError						-5			//��ʾ�������ݴ���


typedef struct
{
	u8 ATC_TextCodeH8;				//�����루Text Code��
	u8 ATC_TextCodeL8;
	u8 ATC_TotalweightH8;			//�г�������
	u8 ATC_TotalweightL8;
	
	//u8 TrainSpeed;			
	
	/*byte5*/
	u8 Byte5Undef5Bit						:5;		
	u8 ATC_No6VVVF1Vaild				:1;		//6�ų�VVVF1����
	//u8 ATC_No6VVVF2Vaild				:1;		//6�ų�VVVF2����	
	u8 ATC_No3VVVF1Vaild				:1;		//3�ų�VVVF1����
	//u8 ATC_No3VVVF2Vaild				:1;		//3�ų�VVVF2����	
	u8 ATC_No1VVVF1Vaild				:1;		//1�ų�VVVF1����
	//u8 ATC_No1VVVF2Vaild				:1;		//1�ų�VVVF2����

	
	/*byte6*/
	u8 Byte6Undef5Bit						:5;
	u8 ATC_No6VVVF1EleBrake			:1;
	//u8 ATC_No6VVVF2EleBrake			:1;
	u8 ATC_No3VVVF1EleBrake			:1;
	//u8 ATC_No3VVVF2EleBrake			:1;	
	u8 ATC_No1VVVF1EleBrake			:1;		//1 �ų�VVVF1������Ч
	//u8 ATC_No1VVVF2EleBrake			:1;

	/*Byte7*/
	u8 Byte7Undef2Bit						:2;
	u8 ATC_BCU6vaild						:1;
	u8 ATC_BCU5vaild						:1;
	u8 ATC_BCU4vaild						:1;
	u8 ATC_BCU3vaild						:1;
	u8 ATC_BCU2vaild						:1;
	u8 ATC_BCU1vaild						:1; 	//1 �ų�BCU ����
	
	//u8 OperatStatus;
	/*Byte8*/
	u8 UndefByte8;
	
	/*byte9*/
	u8 Byte9Undef6Bit						:6;
	u8 ATC_Slide6								:1;		//6 �ų�����
	u8 ATC_Slide1								:1;		//1 �ų�����
	
	/*byte10*/
	u8 Byte10Undef4Bit					:4;
	u8 ATC_No6VVVF1SlideStatu		:2;
	//u8 ATC_No6VVVF2SlideStatu		:2;
	u8 ATC_No1VVVF1SlideStatu		:2;		//1 �ų� VVVF1����״̬
	//u8 ATC_No1VVVF2SlideStatu		:2;		
	
	
	u8 ATC_TrainNumber;								//�г����  BCD
	u8 Byte12Spare;					
	u8 ATC_TimeVaild;								//ʱ����Ч 0 1
	
	u8 ATC_Year;										//��		BCD
	u8 ATC_Month;                   //��
	u8 ATC_Day;		                  //��
	u8 ATC_Hour;	                  //ʱ
	u8 ATC_Minute;                  //��
	u8 ATC_Second;                  //��	
	
	u8 ATC_TBFroce1;								//1��ǣ��/�ƶ���
	u8 ATC_TBFroce2;								//6��ǣ��/�ƶ���
	
	u8 ATC_DoorStatu;								//����״̬ �� [00H��ȫ���Źر� ]�� [01H����೵�ų���]��[02H���Ҳ೵�ų���]�� [03H��˫�೵�ų���]
	u8 ATC_LoadRate;								//��ƽ�����˳���
	
	u8 Undef24Byte[24];
}ATCSdrText;

typedef struct
{
	u8 ATCSdrDle1;									//��ʼ�ֽ�DLE
	
	u8 ATCSdrStx;										//��ʼ�ַ� 0x02
	ATCSdrText 		ATCSdrData;				//���ݶ�
	u8 ATCSdrDle2;									//�����ֽ�DLE
	
	u8 ATCSdrEtx;
	u8 ATCSdrBcc;
}ATCSdrFrame;


/*SD�ı����ȷŵ��ֽڣ��ڷ�����ֽ�*/		//WHHH
typedef struct
{
	u8 ATC_TextCodeL8;
	u8 ATC_TextCodeH8;					//�����루Text Code��
	
	/*byte3,ATCģʽ*/
	u8 Byte3Undef5Bit				:5;
	u8 ATC_AMC_AVAIL				:1;	//AMC_AVAIL;
	u8 ATC_MCS_AVAIL				:1;	//MCS_AVAIL
	u8 ATC_RM_AVAIL					:1;	//RM_AVAIL
	
	
	/*byte4*/
		u8 ATC_BLOCK						:1;//BLOCK
		u8 ATC_CBTC							:1;//CBTC
		u8 Byte4Undef2bit						:2;
		u8 ATC_AMC							:1;//AMC
		u8 ATC_MCS							:1;//MCS
	u8 ATC_RMR							:1;//RMR
	u8 ATC_RMF							:1;//RMF





	
	u8 ATC_Speed;							//�ٶ�
	u8 ATC_ObjSpeed;					//Ŀ���ٶ�
	
	/*byte7*/
	u8 ATC_EmergenBrak			:1;//�����ƶ�
	u8 ATC_OverSpeed				:1;//����
	u8 ATC_DrivReq					:1;//����Ҫ��
	u8 Byte7Undef5Bit				:5;
	
	u8 ATC_CrewNumberH8;			//����Ա���
	u8 ATC_CrewNumberL8;
	u8 ATC_TrainOperatNoH8;		//�г����ñ��
	u8 ATC_TrainOperatNoL8;
	u8 ATC_TrainDestNoH8;			//�г�Ŀ�ĵر��
	u8 ATC_TrainDestNoL8;
	
	/*byte18*/
	u8 Byte18Undef3Bit			:3;
	u8 ATC_HoldBrakCmd			:1;//�����ƶ�ָ��
	u8 ATC_LeftDoorLockSta	:1;//��������״̬
	u8 ATC_RightDoorLockSta	:1;//�ҳ�������״̬
	u8 Byte18Undef2Bit			:2;
	
	/*19*/
	u8 ATC_PrePaSig					:1;	//Ԥ��վ�ź�
	u8 ATC_ArrivePa					:1;	//��վ�ź�
	u8 ATC_LeavePa					:1;	//������վ
	u8 Byte19Undef5Bit			:5;
	
	u8 ATC_TrainNowIDH8;				//����ͣվID	
	u8 ATC_TrainNowIDL8;				
	u8 ATC_TrainNextIDH8;				//��һվID
	u8 ATC_TrainNextIDL8;
	
	u8 Undef4Byte[4];
	
		/*byte5*/						
	u8 ATC_Year;								//��		10����
	u8 ATC_Month;								//��
	u8 ATC_Day;									//��
	u8 ATC_Hour;								//ʱ
	u8 ATC_Minute;							//��
	u8 ATC_Second;							//��	

	u8 ATC_DoorControlMode;			//���ſ���ģʽ ��[00H:�ֶ�����]��[01H:�Զ���?�ֶ���]��[02H:�Զ�����]
	
	/*35byte*/
	u8 ATC_RightOpenCmd		 	:1;
	u8 ATC_RightCloseCmd	 	:1;
	u8 ATC_LeftOpenCmd		 	:1;
	u8 ATC_LeftCloseCmd	 	 	:1;
	u8 Byte35Undef4Bit			:4;
	
	u8 Spare4Byte[4];
	
	/*byte40*/
	u8 ATC_BMModeRelays			:1;	//BMģʽ�̵���
	u8 ATC_EBRD1Mode				:1;//�ź�ϵͳEBRD1״̬
	u8 ATC_EBRD2Mode				:1;//�ź�ϵͳEBRD2״̬
	u8 ATC_TractionRemove		:1;//ǣ���г�
	u8 ATC_HoldBrakeApplied	:1;//�����ƶ�ʩ��
	u8 ATC_AAideVaild				:1;//A����ʹ���ź�
	u8 ATC_BSideVaild				:1;//B����ʹ���ź�
	u8 ATC_TractionForward	:1;//ǣ����ǰ�ź�
	
	/*byte41*/
	u8 Byte41Bit0						:1;
	u8 ATC_TractionCmd			:1;//ǣ��ָ��
	u8 ATC_BrakeCmd					:1;//�ƶ�ָ��
	u8 Undef4Bit						:4;
	u8 ATC_DriveRoomMaster	:1;//˾��������
	
	/*byte42*/
	u8 ATC_CBTCBMButton			:1;//CBTC/BMģʽת����ť
	u8 ATC_RMModeButt				:1;//RMģʽ��ť
	u8 Undef6Bit						:6;
	
	u8 Undef7Byte[7];		//43-49
	
}ATCSdText;

typedef struct
{
	u8 ATCSdStx;												//��ʼ�ַ� 0x02
	ATCSdText 		ATCSdData;				//���ݶ�
	u8 ATCsdEtx;
	u8 ATCSdBcc;
}ATCSdFrame;

/*
	����˵����
���ܣ�����豸���͸�ATC��״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendATCSdr(ATCSdrText * pInData/*,u8 * pOutData*/);  


/*
	����˵����
���ܣ�����豸����ATCD��״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ��������ݽṹ��)��

����ֵ��0,�޴�  �����д�
*/
s8 GetATCSd(u8 *pInData,void *pOutData);		

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckATCGetError(u8 *pInData);


#endif  //__ATCCOMPRM_H__
