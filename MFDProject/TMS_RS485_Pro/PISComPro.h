#ifndef __PISCOMPRO_H__
#define __PISCOMPRO_H__

#include "TMS_ComunDefine.h"

#define  PIS_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  PIS_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

#define  PIS_STX							0x02	//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x02
#define  PIS_SDR_DATALEN			0x1A
//#define  PIS_ETX							0x7E	

#define  PIS_SD_DATALEN				0x1A

/*����������ֵ*/
#define  PIS_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  PIS_GetTimeoutError	  -2			//��������֮��20ms����û�н��յ�����
#define  PIS_FrameError					-3			//��⵽֡������żУ�����ʱ
#define  PIS_BCCError       	  -4  	 	//BCC ���Ų����ϼƻ�ֵʱ
#define  PIS_GetError						-5			//��ʾ�������ݴ���


typedef struct
{	
	u8 PIS_DataLongth;					//���������ݳ���
	u8 PIS_TimeVaild;						//ʱ����Ч
	u8 Byte3Undef;							//
	
	/*byte4*/
	u8 PIS_PISClosed			:1;		//�Źغ�	
	u8 PIS_ArrivePa				:1;		//��վ�㲥	
	u8 PIS_PredictPa			:1;		//Ԥ��վ�㲥
	u8 PIS_LeavePa				:1;		//��վ�㲥
	u8 PIS_CarNumber			:1;		//λ��
	u8 PIS_LeftPISOp			:1;		//��վ��A���ţ�������
	u8 PIS_RightPISOp			:1;		//��վ��B���ţ�������
	u8 PIS_Atomode				:1;		//ATOģʽ
	
	/*byte5*/						
	u8 Year;										//��		BCD
	u8 Month;										//��
	u8 Day;											//��
	u8 Hour;										//ʱ
	u8 Minute;									//��
	u8 Second;									//��	
	
	u8 PIS_TrainNumberH8;				//�г����		
	u8 PIS_TrainNumberL8;	
	
	u8 Undef5Byte[5];
	
	u8 PIS_TrainDesIDH8;				//�г�Ŀ�ĵ����
	u8 PIS_TrainDesIDL8;	
	
	u8 PIS_TrainNowIDH8;				//����ͣվID
	u8 PIS_TrainNowIDL8;
	
	u8 PIS_TrainNextIDH8;				//��һվID
	u8 PIS_TrainNextIDL8;
	
	u8 Undef4Byte[4];
}PISSdrText;

typedef struct
{
	u8 PISSdrStx;
	PISSdrText PISSdrData;
	u8 PISSdrBcc;
}PISSdrFrame;

typedef struct
{
	u8 PIS_DataLongth;					//���������ݳ���
	u8 Byte2Undef;							//
	
	/*byte3*/
	u8 PIS_AutoPa					:1;		//�Զ��㲥ģʽ
	u8 PIS_ManPa					:1;		//�ֶ��㲥ģʽ	
	u8 Byte3Undef6Bit			:6;
		
	/*byte4*/		
	u8 PIS_Pea1						:1;		//1���˿ͽ�������
	u8 PIS_Pea2						:1;		//2���˿ͽ�������
	u8 PIS_Pea3						:1;		//3���˿ͽ�������
	u8 PIS_Pea4						:1;		//4���˿ͽ�������
	u8 PIS_Pea5						:1;		//5���˿ͽ�������
	u8 PIS_Pea6						:1;		//6���˿ͽ�������
	u8 Byte4Undef2Bit			:2;
	
	u8 PIS_TrainOriIDH8;				//��ʼվID
	u8 PIS_TrainOriIDL8;				
	u8 PIS_TrainNowIDH8;				//����ͣվID	
	u8 PIS_TrainNowIDL8;				
	u8 PIS_TrainNextIDH8;				//��һվID
	u8 PIS_TrainNextIDL8;
	u8 PIS_TrainDesIDH8;				//�յ�վID
	u8 PIS_TrainDesIDL8;
	
	u8 PIS_ControlLevel;				//����״̬		0=Ĭ��ֵ��1=����2=�ӣ�3=����
	
	u8 Undef14Byte[14];
}PISSdText;		

typedef struct
{
	u8 PISSdStx;
	PISSdText PISSdData;
	u8 PISSdBcc;
}PISSdFrame;


	
/*
	����˵����
���ܣ�����豸���͸�PIS��״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendPISSdr(PISSdrText * pInData/*,u8 * pOutData*/);  


/*
	����˵����
���ܣ�����豸����PISD��״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ��������ݽṹ��)��

����ֵ��0,�޴�  �����д�
*/
s8 GetPISSd(u8 *pInData,void *pOutData);		

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckPISGetError(u8 *pInData);




#endif  //__PISCOMPRO_H__
