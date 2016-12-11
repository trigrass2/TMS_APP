#ifndef  __BRAKECOMPRO_H__
#define  __BRAKECOMPRO_H__

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

/*
��1������Ҫ��ָ�������ƶ���װ�ã�
	ָ��	˵��						�Ǻ�		�շ���Ϣ�ı����ı�����
1 20H ״̬����Ҫ��				SDR 		11Byte(�ֽ�)
2 21H ����ɨ������Ҫ��		TDR 		3Byte(�ֽ�)

//״̬���ݵ�Ҫ��   11�ֽ��ı���ʽ
��1��״̬����Ҫ�󣨼���� -> �ƶ���װ�ã�
	0 �ֽ�	1~10�ֽ�
	ָ��	״̬����Ҫ��
	20H			10Byte
	
1	�꣺00~99����������2 λ���� BCD
2 �£�01~12 BCD
3 �գ�01~31 BCD
4 ʱ��00~23 BCD
5 �֣�00~59 BCD
6 �룺00~59 BCD
7 Ԥ����00H��
8 Ԥ����00H��
9 Ԥ����00H��
10 ǣ���غ�			ʱ���趨		���ɨ������
	��Ч�ź�				3��bit1			1�� bit0
	2��bit7

ע1�����ɨ������ʱ�趨Ϊ1
ע2��VVVF �Ŀ���״̬��KA ��ʱ�趨Ϊ1
ע3��Ҫ���趨ʱ��Ϊ1 ��ʾԼΪ1 ���ӵ���˼
���ƶ�������װ����0 �仯��1 ʱ����Ҫ����ͬ�ı���ʱ������趨��


//����ɨ������Ҫ��	 3�ֽ��ı�
��1��Ҫ�����ɨ�����ݣ�������ƶ�������װ�ã�
0 					1							 						2
ָ��21H		Ҫ��ɨ����NO��1Byte)		Ҫ��ģ����

��ע1��ɨ������1 ��5 ��Χ����ָ������ȥ�����ݿ�����Ϊ1.
��ע2��ģ������0 ��40 ��Χ�ڽ���ָ����


(2)Ӧ��ָ��ƶ���װ�ü������
	ָ��		˵��					 �Ǻ�	�շ���Ϣ�ı����ı�����
1 20H 	״̬����Ӧ��				SD 		50Byte(�ֽ�)
2 21H 	����ɨ������Ӧ��		TD 		51Byte(�ֽ�)

//״̬����Ӧ��			50�ֽ�
0					1~49
ָ��			״̬����
20H			��49Byte)

��a) ״̬���ݵ���ϸ����������ʾ��
	Bit 7			 6			 	5			 4			 3			 2			 1			 0	 ��ע
1			���ڲ����쳣																								�쳣��1	
			RAM������
			�����쳣
			�����ƶ������Ʒ��쳣
			��ת��ѹ�����Ʒ��쳣�� 1 ת��ܣ�
			��ת��ѹ�����Ʒ��쳣�� 2 ת��ܣ�
			�����ƶ���ģʽ�ź��쳣
			KA �и��ز�����װ�ã��ź��쳣
				
2 		�����ƶ�����ӳ�ź��쳣																					�쳣��1
			�����ƶ�������ָ���쳣
			������·�쳣
			0���գ�
			��1 ���ٶ��쳣
			��2 ���ٶ��쳣
			��3 ���ٶ��쳣
			��4 ���ٶ��쳣
																											�������в�����1						
3 		����
			��1 ���ת
			��2 ���ת
			��3 ���ת
			��4 ���ת
			����ƶ�����������
			�����в�����
			ǿ�ƻ���
	
4																		�쳣�������ţ� 1�� ��ѹ��
			T ����ASѹ���쳣
			AS2 ѹ���쳣
			AS1 ѹ���쳣
			BC ѹ���쳣
			AC ѹ���쳣
			��ؽ�������Ч�ź�
			����ʧЧԤ���ź�
			KA ����Ч�ź�

5 									����ע4����ע4�������ƶ���ָ��1 ��3 ��ָ�ڼ�ѹʱ������Ϣ1���ǳ��ƶ���ָ��ʱָ�ڷǳ��ƶ�����������ʱ������Ϣ1��
			0
			0
			0
			�ǳ��ƶ���ָ��
			�����ƶ���ָ��3
			�����ƶ���ָ��2
			�����ƶ���ָ��1
			0

6 AS1 ѹ��0~765kPa ��OOH~FFH (3Kpa/bit) �ջ�ѹ��
7 AS2 ѹ��0~765kPa ��OOH~FFH (3Kpa/bit)
8 ���ز���ָ��0~25.5V��OOH~FFH ��0.1v/bit)
9 �����ƶ���ģʽָ��0~25.5V��OOH~FFH ��0.1v/bit)
10 �����ƶ�����Ӧָ��0~25.5V��OOH~FFH ��0.1v/bit)
11 �����ƶ�������ָ��0~102% :OOH~FFH ��0.4%/bit)
12 T ����AS ѹ���ź�0~102% :OOH~FFH ��0.4%/bit)
13 ��ŷ�����ָ��0~510mA :OOH~FFH ��2mA/bit)
14 AC ѹ��0~765kPa ��OOH~FFH (3Kpa/bit)
15 BC ѹ��0~765kPa ��OOH~FFH (3Kpa/bit)
16 �����ƶ���0~102kN :OOH~FFH ��0.4KN /bit)
17 ��������15~66ton :OOH~FFH ��0.2ton /bit)
18 ��1 ���ٶ�0~127.5km/h :OOH~FFH ��0.5km/h/bit)
19 ��2 ���ٶ�0~127.5km/h :OOH~FFH ��0.5km/h/bit)
20 ��3 ���ٶ�0~127.5km/h :OOH~FFH ��0.5km/h/bit)
21 ��4 ���ٶ�0~127.5km/h :OOH~FFH ��0.5km/h/bit)
22 �����õĳ����ٶ�0~127.5km/h :OOH~FFH ��0.5km/h/bit)�� Ԥ����OOH)
47 Ԥ����OOH)
48 0 0 0   	��¼5 ��¼4 ��¼3 ��¼2 ��¼1 						�м�¼��1
49 Ԥ����OOH��

��ע1���հ״�ΪԤ��������Ϊ0)����
��ע2����1 �ֽڵ�bit5 ��ʾ��1 �ֽڵ�bit4��bit6 ���4 �ֽڵ�bit3 ֮�������ֻ�͡�
��ע3����48 �ֽڵļ�¼1 ��5 ��ʾ��¼��ɵ�ɨ�����ݡ�


//����ɨ�����ݵ�Ӧ���ƶ�������װ�� ->	�������
0 									1~51
ָ��21H						ɨ�����ݣ�50Byte)

(a) Ҫ��ģ����Ϊ0 ʱ�����ϱ��⣩
Bit7 6 5 4 3 2 1 0 ��ע
1 ģ����No��OOH��
2 ���ϴ���
3 �꣺00~99��������ݵĺ�2 λ����
4 �£�00~12 BCD
5 �գ�01~31 BCD
6 ʱ��00~23 BCD
7 �룺00~59 BCD
8 Ԥ����OOH��
9
��	�������ݣ�42Byte)
51

��b) Ҫ��ģ����Ϊ1 ��40 ʱ
Bit7 6 5 4 3 2 1 0 ��ע
1 ģ���ţ�01~40��
2
:		�������ݣ�50Byte)
51
��ע1������ʱ���趨�ڵ�0 ģ��
��ע2�����ģ��û������ʱ����Ϊ�����ݿ�¼��OOH��
��ע3������������ָ�ӵ�0 ģ��ĵ�9byte��λ������ʼ����40 ģ��ĵ�8 λ��Ϊֹ����

*/


/*
c)���ϴ���
���ϴ���					��������						����ˮƽ
90H 				KA �и���ָ���쳣					C
91H 				�����ƶ���ģʽ�ź��쳣			C
92H 				�����ƶ������Ʒ��쳣			A ��B
93H 			1 ת��ܿ�ת�õĿ��Ʒ��쳣		B
94H 			2 ת��ܿ�ת�õĿ��Ʒ��쳣		B
95H			  �����ƶ�����Ӧ�ź��쳣				C
96H 				RAM ������            A ��B
97H 				���ڲ����쳣B
98H 				��������߼��쳣						B
9AH 				��1 ���ٶ��쳣							C
9BH 				��2 ���ٶ��쳣							C
9CH 				��3 ���ٶ��쳣							C
9DH 				��4 ���ٶ��쳣							C
9EH 			�����ƶ�������ָ���쳣			A ��B
9FH 			AC ѹ���쳣								A ��B
AOH 			BC ѹ���쳣									B
A1H 			AS1 ѹ���쳣									B
A2H 			AS2 ѹ���쳣									B
A3H 			T ��AS ѹ���쳣							B
A4H				�����ƶ�������						A ��B
A5H 			����������								A ��B
A:�������е���һվ��
B:���е��յ�վ��ֹͣ��ʹ
C���������н�����������ά��
D��ֻ��ά��������м�¼��

*/

#include "TMS_ComunDefine.h"

#define  BRAKE_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���		//�������û��Զ���
#define  BRAKE_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*�����ʽ��Ҫ����*/
#define  BRAKE_STX						0x02		//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x20
#define  BRAKE_ETX 						0x03		

/*�ƶ��� ָ���*/
#define  BRAKE_SDR_COMMAND		0x20	 	//״̬����Ҫ��ָ��
#define  BRAKE_TDR_COMMAND		0x21		//����ɨ������Ҫ��ָ��
#define  BRAKE_SD_COMMAND			0x20	 	//״̬����Ӧ��ָ��
#define  BRAKE_TD_COMMAND			0x21		//����ɨ������Ӧ��ָ��

/*����������ֵ*/
#define  BRAKE_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  BRAKE_GetTimeoutError	  -2			//��������֮��20ms����û�н��յ�����
#define  BRAKE_FrameError					-3			//��⵽֡������żУ�����ʱ
#define  BRAKE_BCCError       	  -4  	 	//BCC ���Ų����ϼƻ�ֵʱ
#define  BRAKE_GetError						-5			//��ʾ�������ݴ���

typedef struct _BRAKE_SDR_TEXT		//���������ı�
{
	u8 Command;				//������
	u8 Year;					//��	00~99����������2 λ���� BCD
	u8 Month;					//��	BCD
	u8 Day;						//��	BCD
	u8 Hour;					//ʱ	BCD
	u8 Minute;				//��	BCD
	u8 Second;				//��	BCD
	u8 Reserve1;			//00H
	u8 Reserve2;			//00H
	u8 Reserve3;			//00H
	
	u8 ClearScanDataBit :1;		//���ɨ������		���ɨ������ʱ�趨Ϊ1
	u8 TimeSetBit				:1;		//ʱ���趨λ  
														//ע3��Ҫ���趨ʱ��Ϊ1 ��ʾԼΪ1 ���ӵ���˼
														//���ƶ�������װ����0 �仯��1 ʱ����Ҫ����ͬ�ı���ʱ������趨��
	u8 undef5bit				:5;		//5λΪʹ��
	u8 TracLoadSigEff		:1;		//ǣ���غ���Ч�ź�
	
}S_TMS_BRAKE_SDR_Text;

typedef struct _BRAKE_SDR_Frame		//����豸 ���͵� ���ſ��Ƶ���������֡
{
	u8 SDR_STX;											//��ʼ�ַ� 0x02
	S_TMS_BRAKE_SDR_Text SDR_Data;	//���ݶ�
	u8 SDR_ETX;											//��չ�ֶ� 0x3
	u8 SDR_BCC1EVEN;								//ż����bcc�㷨 
	u8 SDR_BCC1ODD;									//������bcc�㷨 
} S_TMS_BRAKE_SDR_Frame;


typedef struct _BRAKE_TDR_TEXT		//�������ɨ�������ı�
{
	u8 Command;				//������
	u8 ScanNumber;		//ɨ����		��ע1��ɨ������1 ��5 ��Χ����ָ������ȥ�����ݿ�����Ϊ1.
	u8 ModuleNumber;	//ģ����		��ע2��ģ������0 ��40 ��Χ�ڽ���ָ����
}S_TMS_BRAKE_TDR_Text;

typedef struct _BRAKE_TDR_Frame		//����豸 ���͵� ���ſ��Ƶ���������֡
{
	u8 TDR_STX;											//��ʼ�ַ� 0x02
	S_TMS_BRAKE_TDR_Text TDR_Data;	//���ݶ�
	u8 TDR_ETX;											//��չ�ֶ� 0x3
	u8 TDR_BCC1EVEN;								//ż����bcc�㷨 
	u8 TDR_BCC1ODD;									//������bcc�㷨 
} S_TMS_BRAKE_TDR_Frame;


typedef struct _Receive_ComDataFormat
{
	u8 KALoadCompensSigExce 	 :1;	//KA���ز����ź��쳣
	u8 RegenerBrakModeSigExce  :1;	//�����ƶ���ģʽ�ź��쳣
	u8 IdlPressureControl1Exce :1;	//��ת��ѹ�����Ʒ�1�쳣
	u8 IdlPressureControl2Exce :1;	//��ת��ѹ�����Ʒ�2�쳣
	u8 CommonBrakeControlExce	 :1;	//�����ƶ������Ʒ��쳣
	u8 PerformanceExce				 :1;	//�����쳣
	u8 RAMDetectionError			 :1;	//RAM������
	u8 EarlyTestExce					 :1;	// ���ڲ����쳣
	
	u8 Axis4SpeedExce					 :1;	//��4 ���ٶ��쳣
	u8 Axis3SpeedExce					 :1;	//��3 ���ٶ��쳣
	u8 Axis2SpeedExce					 :1;	//��2 ���ٶ��쳣
	u8 Axis1SpeedExce					 :1;	//��1 ���ٶ��쳣
	u8 undef2thByte4Bit				 :1;	//δʹ��1
	u8 CrossLineExce					 :1;	//������·�쳣
	u8 AirBrakeSubInstruExce	 :1;	//�����ƶ�������ָ���쳣
	u8 RegenerBrakReflectInforExce :1;	//�����ƶ�����ӳ��Ϣ�쳣
	
	u8 ForceRemission				 	 :1;	//ǿ�ƻ���
	u8 CheckNoRemission			 	 :1;	//�����в�����
	u8 CheckBrakeFault			 	 :1;	//����ƶ�����������
	u8 Axis4Idle							 :1;	//��4 ���ת
	u8 Axis3Idle							 :1;	//��3 ���ת
	u8 Axis2Idle							 :1;	//��2 ���ת
	u8 Axis1Idle							 :1;	//��1 ���ת
	u8 Test										 :1;	//����
	
	u8 KAEffSignal						 :1;	//KA ����Ч�ź�
	u8 RegenerFailWarnSig			 :1;	//����ʧЧԤ���ź�
	u8 EleControlThrottleEffSig :1;	//��ؽ�������Ч�ź�
	u8 ACPressureExce					 :1;	//ACѹ���쳣
	u8 BCPressureExce					 :1;	//BCѹ���쳣
	u8 AS1PressureExce				 :1;	//AS1ѹ���쳣
	u8 AS2PressureExce				 :1;	//AS2ѹ���쳣
	u8 TASPressureExce				 :1;	//TASѹ���쳣			
	
	u8 undef5thByte0Bit				 :1;	//�����ֽڵ�0λδʹ��
	u8 ComUseBrakeInstruct1		 :1;	//�����ƶ���ָ��1	
	u8 ComUseBrakeInstruct2		 :1;	//�����ƶ���ָ��2	
	u8 ComUseBrakeInstruct3		 :1;	//�����ƶ���ָ��3	
	u8 unComUseBrakeInstruct	 :1;	//�ǳ����ƶ���ָ��
	u8 undef5thByte1Bit5to7Bit :3;	//�����ֽڵ�5��7λδʹ��
	
	u8 AS1Pressure;									//AS1ѹ��
	u8 AS2Pressure;									//AS2ѹ��
	u8 LoadCompensInstruct;					//���ز���ָ��
	u8 RegenerBrakModeInstruct;			//�����ƶ���ģʽָ��
	u8 RegenerBrakReflectInstruct;	//�����ƶ�����ӳָ��
	u8 AirBrakeSubInstruct;					//�����ƶ�������ָ��
	u8 TASPressureSingal;						//T ����AS ѹ���ź�
	u8 SolenoidCurrentInstruct;			//��ŷ�����ָ��
	u8 ACPressure;									//ACѹ��
	u8 BCPressure;									//BCѹ��
	u8 AirBrakeForce;								//�����ƶ���
	u8 VehicleWeight;								//��������
	u8 Axis1Speed;									//��1 ���ٶ�
	u8 Axis2Speed;									//��2 ���ٶ�
	u8 Axis3Speed;									//��3 ���ٶ�
	u8 Axis4Speed;									//��4 ���ٶ�
	u8 ControlTrainSpeed;						//�����õĳ����ٶ�

}S_TMS_BRAKE_RX_ComDataFormat;

typedef struct _BRAKE_StateSD_Text			//״̬��Ӧ�����ı�
{
	u8 Command;											//������
	
	S_TMS_BRAKE_RX_ComDataFormat SD_Text;

	/*23 - 47*/
	u8 Dummy[25];										//Ԥ����OOH) 25�ֽ�

	u8 Record1								:1;			//��¼1
	u8 Record2								:1;			//��¼2
	u8 Record3								:1;			//��¼3
	u8 Record4								:1;			//��¼4
	u8 Record5								:1;			//��¼5
	u8 undef48thByte5to7Bit		:3;			//δʹ��
	
}S_TMS_BRAKE_SD_Text;

typedef struct _BRAKE_SD_Frame   // ����豸 ���յ� ���ſ����豸����Ӧ֡
{
	u8 SD_STX;										//��ʼ�ַ� 0x02
	S_TMS_BRAKE_SD_Text	SD_Data;
	u8 SD_ETX;										//��չ�ֶ� 0x3
	u8 SD_BCC1EVEN;							//ż����bcc�㷨 
	u8 SD_BCC1ODD;								//������bcc�㷨 

} S_TMS_BRAKE_SD_Frame;


typedef struct _BRAKE_StateTD0_Text
{
	u8 Command;        //������
	u8 ModuleNumber;	//ģ����	��00H��	
	u8 FaultCode;			//���ϴ���
	u8 Year;					//��	00~99����������2 λ���� BCD
	u8 Month;					//��	BCD
	u8 Day;						//��	BCD
	u8 Hour;					//ʱ	BCD
	u8 Minute;				//��	BCD
	u8 Second;				//��	BCD
	u8 Reserve1;			//00H
	
	S_TMS_BRAKE_RX_ComDataFormat TD0SampleData;	//�������ݣ�42Byte)
	u8 Dummy0[42-sizeof(S_TMS_BRAKE_RX_ComDataFormat)]; //00H
	
}S_TMS_BRAKE_TD0_Text;

typedef struct _BRAKE_TD0_Frame   // ����豸 ���յ� ���ſ����豸����Ӧ֡
{
	u8 TD0_STX;										//��ʼ�ַ� 0x02
	S_TMS_BRAKE_TD0_Text	TD0_Data;
	u8 TD0_ETX;										//��չ�ֶ� 0x3
	u8 TD0_BCC1EVEN;							//ż����bcc�㷨 
	u8 TD0_BCC1ODD;								//������bcc�㷨 

} S_TMS_BRAKE_TD0_Frame;


typedef struct _BRAKE_StateTD1_Text
{
	u8 Command;       	 //������
	u8 ModuleNumber;		//ģ����	��01-0x40H��	
	
	S_TMS_BRAKE_RX_ComDataFormat TD1SampleData;	//�������ݣ�42Byte)
	u8 Dummy1[50-sizeof(S_TMS_BRAKE_RX_ComDataFormat)]; //00H
	
}S_TMS_BRAKE_TD1_Text;

typedef struct _BRAKE_TD1_Frame   // ����豸 ���յ� ���ſ����豸����Ӧ֡
{
	u8 TD1_STX;										//��ʼ�ַ� 0x02
	S_TMS_BRAKE_TD1_Text	TD1_Data;
	u8 TD1_ETX;										//��չ�ֶ� 0x3
	u8 TD1_BCC1EVEN;							//ż����bcc�㷨 
	u8 TD1_BCC1ODD;								//������bcc�㷨 

} S_TMS_BRAKE_TD1_Frame;



#define SEND_SDR			1
#define SEND_TDR			2

#define RECEIVE_SD		1
#define RECEIVE_TD0		2
#define RECEIVE_TD1		3

/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
//	u8 mode:���͵�����ΪSDR������TDR	; 
����ֵ���޷���ֵ
*/
void SendBrake_SDR_Or_TDR(void * pInData,u8 * pOutData/*,u8 mode*/);  


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_TMS_BRAKE_SD_Text�ṹ��)��
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetBrake_SD_Or_TD(u8 *pInData,void *pOutData);

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckBrakeGetError(u8 *pInData);

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
void GenerateBrakeBccChcek(void * pData,u8 len,u8* BCC1Even,u8* BCC1Odd);


void testBrake(void);

#endif  //__BRAKECOMPRO_H__
