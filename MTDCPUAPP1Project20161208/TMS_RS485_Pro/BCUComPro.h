#ifndef __BCUCOMPRO_H__
#define __BCUCOMPRO_H__

#include "TMS_ComunDefine.h"
/*
(1)������
ͨ�ŷ�ʽ		����ʽ˫��ͨ�ŷ�ʽ
ͨ���ٶ�		9600bps��0.1%
ͬ�ڷ�ʽ		�첽ͬ��
����˳��		����BASIC ����˳��
���ӷ���		���ڵ���ӽڵ㣨�ھ��и��ӹ�ϵ��ͨ��ϵͳ�з��䷢������Ȩ�ķ�ʽ����
���ͷ���		NRZ
���Ʒ�ʽ		��������
���			ˮƽ��ż��BCC
������·		20mA ������·
���õ���		��׼120��ƽ�⣬���α�ƽ������
��·��ѹ		DC24V ��10%
��·����		��ǣ���1������17-23mA������϶����0������0-2mA
��·��			1 ��1
*/

/*
(1)ͨѶ֡			//ע�⣺ʹ�õ���У�鷽ʽ
Start bit DATA bit Parity bit Stop bit
	1bit  	8bit 1		bit(ODD) 	1bit
*/

/*
(2)֡�ṹ
No.0				No.1��n							ETX		BCC1(ż���е�У��)		BCC2(�����е�У��)
STX(02H)  TEXT��DATA �ַ����� 		03H			1byte								1byte
*/

#define  BCU_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���		//�������û��Զ���
#define  BCU_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*�����ʽ��Ҫ����*/
#define  BCU_STX						0x02		//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x20
#define  BCU_ETX 						0x03		

/*�ƶ��� ָ���*/
#define  BCU_SDR_COMMAND		0x20	 	//״̬����Ҫ��ָ��
#define  BCU_TDR_COMMAND		0x21		//����ɨ������Ҫ��ָ��
#define  BCU_SD_COMMAND			0x20	 	//״̬����Ӧ��ָ��
#define  BCU_TD_COMMAND			0x21		//����ɨ������Ӧ��ָ��

/*����������ֵ*/
#define  BCU_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  BCU_GetTimeoutError	  -2			//��������֮��20ms����û�н��յ�����
#define  BCU_FrameError					-3			//��⵽֡������żУ�����ʱ
#define  BCU_BCCError       	  -4  	 	//BCC ���Ų����ϼƻ�ֵʱ
#define  BCU_GetError						-5			//��ʾ�������ݴ���

#define SEND_SDR			1
#define SEND_TDR			2

#define RECEIVE_SD		1
#define RECEIVE_TD0		2
#define RECEIVE_TD1		3

/*SDR*/
typedef struct 
{
	u8 Command;				//������
	u8 Year;					//��	00~99����������2 λ���� BCD
	u8 Month;					//��	BCD
	u8 Day;						//��	BCD
	u8 Hour;					//ʱ	BCD
	u8 Minute;				//��	BCD
	u8 Second;				//��	BCD
	u8 BrakeRise;			// �ƶ�������ָ�� �� FFH����
	u8 Reserve2;			//00H
	u8 Reserve3;			//00H

	u8 BCU_TraceDataClea 	:1; //���״̬���ݣ�2  ����������ֽڱ���Լ 1 �루�൱�ڴ������� 5��ʱ��ɾ�� BCU �ڲ����е�״̬���ݣ���¼����Ϊ 0
	u8 BCU_TimeSet 				:1; //ʱ���趨����  ʱ���趨��1 ���ֽڱ���Լ 1 �루�൱�ڴ������� 5��ʱ����������ʱ��ʱ����Ϣ Text �������趨�ƶ�����װ���ڲ���ʱ��
	u8 BCU_unUse6Bit 			:6; //û��ʹ�õ�λ
	
}BcuSdrText;

typedef struct 		//����豸 ���͵� ���ſ��Ƶ���������֡
{
	u8 SdrStx;											//��ʼ�ַ� 0x02
	BcuSdrText 		BcuSdrData;				//���ݶ�
	u8 SdrEtx;											//��չ�ֶ� 0x3
	u8 SdrBCC1;											//ż����bcc�㷨 
	u8 SdrBCC2;											//������bcc�㷨 
	
} BcuSdrFrame;


/*SD*/
typedef struct 
{
	u8 Command;									//����
	
	/*��1Byte*/
	u8 BCU_LoadValueErrtx 			:1;		//���س���ʻ�ź��쳣��ǣ�����غ��ź��쳣��
	u8 BCU_ElecticBrakeCmdErr		:1;		//���ƶ�ģʽ�ź��쳣
	u8 BCU_PressureValErr2			:1;		//���з�ֹ���쳣����ת��ܣ�
	u8 BCU_PressureValErr1			:1;		//���з�ֹ���쳣��ǰת��ܣ�
	u8 BCU_MagnetValueErr				:1;		//�����ƶ������Ʒ��쳣
	u8 BCU_FunctionFaul					:1;		//�����쳣
	u8 BCU_RAMCheckErr					:1;		//RAM������
	u8 BCU_TestErro							:1;		//��ʼ�����쳣
	
	/*��2Byte*/
	u8 BCU_Axle4SpeedErr				:1;		//��4���ٶ��쳣
	u8 BCU_Axle3SpeedErr				:1;		//��3���ٶ��쳣
	u8 BCU_Axle2SpeedErr				:1;		//��2���ٶ��쳣
	u8 BCU_Axle1SpeedErr				:1;		//��1���ٶ��쳣
	u8 BCU_12VFault							:1;		//���Ƶ�Դ�쳣
	u8 BCU_TramWireLogicCheck		:1;		//��ͨ���߼��쳣
	u8 BCU_PneuBrakeReduceCmdErr:1;		//�����ƶ�����ָ���쳣
	u8 BCU_ElecticBrakeFbCmdErr	:1;		//���ƶ������ź��쳣
	
	/*��3Byte*/
	u8 BCU_CompulsoryRelease		:1;		//ǿ�ƻ�����
	u8 BCU_NonreleaseBrakeDet		:1;		//��������
	u8 BCU_InsufficientBraketDet:1;		//�����⣨�����ƶ���
	u8 BCU_Axle4Skid						:1;		//��4�Ử��
	u8 BCU_Axle3Skid						:1;		//��3�Ử��
	u8 BCU_Axle2Skid						:1;		//��2�Ử��
	u8 BCU_Axle1Skid						:1;		//��1�Ử��
	u8 BCU_Test									:1;		//����
	
	/*��4Byte*/
	u8 BCU_PowerSignal					:1;		//������ʻ�źţ�ǣ���źţ���
	u8 BCU_ElecticBrakeFail			:1;		//���ƶ�ʧЧԤ���ź�
	u8 BCU_ElecticBrakeSignal		:1;		//���ƶ��ź�
	u8 BCU_ACPressureSensorErr	:1;		//ACѹ���������쳣
	u8 BCU_BCPressureSensorErr	:1;		//BCѹ���������쳣
	u8 BCU_AS1PressureSensorErr	:1;		//AS1ѹ���������쳣
	u8 BCU_AS2PressureSensorErr	:1;		//AS2ѹ���������쳣
	u8 BCU_ASSignalErrTCar			:1;		//T���Ŀ��س��ź��쳣��ASѹ���쳣?��
	
	/*��5Byte*/
	u8 BCU_ForcedEase						:1;		//ǿ�ƿ���ָ�ǿ�ƻ��⣿��
	u8 Byte5Bit2Undef						:1;		//
	u8 BCU_ATOMode							:1;		//ATOģʽ�ź�
	u8 BCU_BrakeSignal					:1;		//�ƶ��ź�
	u8 BCU_HillStartCmd					:1;		//����ָ��
	u8 BCU_ATPMaxCmd						:1;		//ATP�������ָ��
	u8 BCU_PwmToOther						:1;		//�����ƶ�PWM����һ��
	u8 BCU_ASCompensated				:1;		//����ASѹ���ѳ���
	
	/*��6Byte*/
	u8 BCU_BrakeForceUping			:1;		//�ƶ�������������
	u8 BCU_BrakeForceUpCmd			:1;		//�ƶ�������ָ��
	u8 BCU_HBEase								:1;		//HB����ָ��
	u8 BCU_HBCmd								:1;		//HB����ָ��
	u8 BCU_HBOperating					:1;		//HB������
	u8 BCU_30kmSignal						:1;		//30�����ٶ��ź�
	u8 BCU_BCPressureExist			:1;		//BCѹ�������ź�
	u8 BCU_EmergencyBrakeCmd		:1;		//�����ƶ��ź�
	
	u8 BCU_AS1Pressure;								//AS1ѹ��
	u8 BCU_AS2Pressure;								//AS2ѹ�� 
	u8 BCU_PropulsionLoadCmd;					//���س���ʻ�ź�(���ز���ָ��?) 
	u8 BCU_ElecticBrakeMode;					//���ƶ���ģʽ�ź�
	u8 BCU_ElecticBrakeFB;						//���ƶ��������ź�
	u8 BCU_PneuBrakeReducCmd;					//�����ƶ�����ָ��
	u8 BCU_ASPressureTcar;						//T���Ŀ��س��źţ�ASѹ���źţ���
	u8 BCU_MagnetControlValCmd;				//��ŷ�����ָ��
	u8 BCU_ACPressure;								//ACѹ��  
	u8 BCU_BCPressure;								//BCѹ�� 
	u8 BCU_PneuBrakeForce;						//�����ƶ���
	u8 BCU_CarWeight;									//��������
	u8 BCU_PWMCmd;										//�����ƶ�PWMָ��
	u8 BCU_CarSpeedControl;						//�����ٶȿ���
	u8 BCU_Axle1Speed;								//��1���ٶ�
	u8 BCU_Axle2Speed;								//��2���ٶ�	
	u8 BCU_Axle3Speed;								//��3���ٶ�	
	u8 BCU_Axle4Speed;								//��4���ٶ�

	/*25 - 47*/
	u8 BCU_Dummy[23];										//Ԥ����OOH) 25�ֽ�
	
	u8 BCU_Record1								:1;			//��¼1
	u8 BCU_Record2								:1;			//��¼2
	u8 BCU_Record3								:1;			//��¼3
	u8 BCU_Record4								:1;			//��¼4
	u8 BCU_Record5								:1;			//��¼5
	u8 BCU_Record6								:1;			//��¼6
	u8 Byte48Bit7Undef						:1;			//δʹ��
	u8 Byte48Bit8Undef						:1;			//δʹ��
	
	u8 BCU_Byte49Dummy;												//
	
} BcuSdText;

typedef struct 
{
	u8 SdStx;												//��ʼ�ַ� 0x02
	BcuSdText 		BcuSdData;				//���ݶ�
	u8 SdEtx;												//��չ�ֶ� 0x3
	u8 SdBCC1;											//ż����bcc�㷨 
	u8 SdBCC2;											//������bcc�㷨 
} BcuSdFrame;

/*TDR*/
typedef struct
{
	u8 Command;									//������
	u8 BCU_TraceDataNoRq;				//ɨ����		��ע1��ɨ������1 ��5 ��Χ����ָ������ȥ�����ݿ�����Ϊ1.
	u8 BCU_TraceDataBlackNoRq;	//ģ����		��ע2��ģ������0 ��40 ��Χ�ڽ���ָ����
}BcuTdrText;

typedef struct
{
	u8 TdrStx;												//��ʼ�ַ� 0x02
	BcuTdrText 		BcuTdrData;					//���ݶ�
	u8 TdrEtx;												//��չ�ֶ� 0x3
	u8 TdrBCC1;												//ż����bcc�㷨 
	u8 TdrBCC2;												//������bcc�㷨 
}BcuTdrFrame;


/*TD����Ӧ�ĳ������ݶ���Ϊ51+1������ֽ�*/
typedef struct
{
	u8 Command;        //������
	u8 ModuleNumber;	//ģ����	��00H��	
	u8 FaultCode;			//���ϴ���
	u8 Year;					//��	00~99����������2 λ���� BCD
	u8 Month;					//��	BCD
	u8 Day;						//��	BCD
	u8 Hour;					//ʱ	BCD
	u8 Minute;				//��	BCD
	u8 Reserve1;			//00H
	
	u8 SampleData[42];	//Ϊ00Hʱ��������42�ֽڡ�
}BcuTdHeardText;

typedef struct
{
	u8 Command;        	//������
	u8 ModuleNumber;		//ģ����	��00H��	
	
	u8 SampleData[50];	//1~40�ĳ�������
}BcuTdOtherText;

typedef struct
{
	u8 Command;        	//������
	u8 ModuleNumber;		//ģ����	��00H��	
	
	u8 SampleData[50];	//1~40�ĳ�������	
}BcuTdText;

typedef struct
{
	u8 TdStx;												//��ʼ�ַ� 0x02
	BcuTdText 		BcuTdData;					//���ݶ�
	u8 TdEtx;												//��չ�ֶ� 0x3
	u8 TdBCC1;												//ż����bcc�㷨 
	u8 TdBCC2;												//������bcc�㷨 
}BcuTdFrame;

typedef struct
{
	/*��1Byte*/
	u8 BCU_LoadValueErrtx 			:1;		//���س���ʻ�ź��쳣��ǣ�����غ��ź��쳣��
	u8 BCU_ElecticBrakeCmdErr		:1;		//���ƶ�ģʽ�ź��쳣
	u8 BCU_PressureValErr2			:1;		//���з�ֹ���쳣����ת��ܣ�
	u8 BCU_PressureValErr1			:1;		//���з�ֹ���쳣��ǰת��ܣ�
	u8 BCU_MagnetValueErr				:1;		//�����ƶ������Ʒ��쳣
	u8 BCU_FunctionFaul					:1;		//�����쳣
	u8 BCU_RAMCheckErr					:1;		//RAM������
	u8 BCU_TestErro							:1;		//��ʼ�����쳣
	
	/*��2Byte*/
	u8 BCU_Axle4SpeedErr				:1;		//��4���ٶ��쳣
	u8 BCU_Axle3SpeedErr				:1;		//��3���ٶ��쳣
	u8 BCU_Axle2SpeedErr				:1;		//��2���ٶ��쳣
	u8 BCU_Axle1SpeedErr				:1;		//��1���ٶ��쳣
	u8 BCU_12VFault							:1;		//���Ƶ�Դ�쳣
	u8 BCU_TramWireLogicCheck		:1;		//��ͨ���߼��쳣
	u8 BCU_PneuBrakeReduceCmdErr:1;		//�����ƶ�����ָ���쳣
	u8 BCU_ElecticBrakeFbCmdErr	:1;		//���ƶ������ź��쳣
	
	/*��3Byte*/
	u8 BCU_CompulsoryRelease		:1;		//ǿ�ƻ�����
	u8 BCU_NonreleaseBrakeDet		:1;		//��������
	u8 BCU_InsufficientBraketDet:1;		//�����⣨�����ƶ���
	u8 BCU_Axle4Skid						:1;		//��4�Ử��
	u8 BCU_Axle3Skid						:1;		//��3�Ử��
	u8 BCU_Axle2Skid						:1;		//��2�Ử��
	u8 BCU_Axle1Skid						:1;		//��1�Ử��
	u8 BCU_Test									:1;		//����
	
	/*��4Byte*/
	u8 BCU_PowerSignal					:1;		//������ʻ�źţ�ǣ���źţ���
	u8 BCU_ElecticBrakeFail			:1;		//���ƶ�ʧЧԤ���ź�
	u8 BCU_ElecticBrakeSignal		:1;		//���ƶ��ź�
	u8 BCU_ACPressureSensorErr	:1;		//ACѹ���������쳣
	u8 BCU_BCPressureSensorErr	:1;		//BCѹ���������쳣
	u8 BCU_AS1PressureSensorErr	:1;		//AS1ѹ���������쳣
	u8 BCU_AS2PressureSensorErr	:1;		//AS2ѹ���������쳣
	u8 BCU_ASSignalErrTCar			:1;		//T���Ŀ��س��ź��쳣��ASѹ���쳣?��
	
	/*��5Byte*/
	u8 BCU_ForcedEase						:1;		//ǿ�ƿ���ָ�ǿ�ƻ��⣿��
	u8 Byte5Bit2Undef						:1;		//
	u8 BCU_ATOMode							:1;		//ATOģʽ�ź�
	u8 BCU_BrakeSignal					:1;		//�ƶ��ź�
	u8 BCU_HillStartCmd					:1;		//����ָ��
	u8 BCU_ATPMaxCmd						:1;		//ATP�������ָ��
	u8 BCU_PwmToOther						:1;		//�����ƶ�PWM����һ��
	u8 BCU_ASCompensated				:1;		//����ASѹ���ѳ���
	
	/*��6Byte*/
	u8 BCU_BrakeForceUping			:1;		//�ƶ�������������
	u8 BCU_BrakeForceUpCmd			:1;		//�ƶ�������ָ��
	u8 BCU_HBEase								:1;		//HB����ָ��
	u8 BCU_HBCmd								:1;		//HB����ָ��
	u8 BCU_HBOperating					:1;		//HB������
	u8 BCU_30kmSignal						:1;		//30�����ٶ��ź�
	u8 BCU_BCPressureExist			:1;		//BCѹ�������ź�
	u8 BCU_EmergencyBrakeCmd		:1;		//�����ƶ��ź�
	
	u8 BCU_AS1Pressure;								//AS1ѹ��
	u8 BCU_AS2Pressure;								//AS2ѹ�� 
	u8 BCU_PropulsionLoadCmd;					//���س���ʻ�ź�(���ز���ָ��?) 
	u8 BCU_ElecticBrakeMode;					//���ƶ���ģʽ�ź�
	u8 BCU_ElecticBrakeFB;						//���ƶ��������ź�
	u8 BCU_PneuBrakeReducCmd;					//�����ƶ�����ָ��
	u8 BCU_ASPressureTcar;						//T���Ŀ��س��źţ�ASѹ���źţ���
	u8 BCU_MagnetControlValCmd;				//��ŷ�����ָ��
	u8 BCU_ACPressure;								//ACѹ��  
	u8 BCU_BCPressure;								//BCѹ�� 
	u8 BCU_PneuBrakeForce;						//�����ƶ���
	u8 BCU_CarWeight;									//��������
	u8 BCU_PWMCmd;										//�����ƶ�PWMָ��
	u8 BCU_CarSpeedControl;						//�����ٶȿ���
	u8 BCU_Axle1Speed;								//��1���ٶ�
	u8 BCU_Axle2Speed;								//��2���ٶ�	
	u8 BCU_Axle3Speed;								//��3���ٶ�	
	u8 BCU_Axle4Speed;								//��4���ٶ�
	
	u8 Bcu_Reserve;											//00H

}BcuSampleData;


/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��BcuSdrText,���� BcuSdText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
//	u8 mode:���͵�����ΪSDR������TDR	; 
����ֵ���޷���ֵ
*/
void SendBcuSdrOrTdr(void * pInData/*,u8 * pOutData*/,u8 mode);  


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_TMS_BRAKE_SD_Text�ṹ��)��
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetBcuSdOrTd(u8 *pInData,void *pOutData);

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckBcuGetError(u8 *pInData);

/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
	BCC1Even���õ���bccEvenУ����
	Bcc1Odd:	�õ���bccOddУ����
����ֵ����
*/
void GenerateBcuBccChcek(void * pData,u8 len,u8* BCC1,u8* BCC2);


void testBcu(void);


#endif
