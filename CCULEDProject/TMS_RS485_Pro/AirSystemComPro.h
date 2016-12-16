#ifndef __AIRSYSTEMCOMPRO_H__
#define __AIRSYSTEMCOMPRO_H__

/*
(1)������
1 ͨѶ��ʽ RS-485��3��ʽ��˫��
2 ͨѶ�ٶ� 19200bps��0.2%
3 ͬ����ʽ ��ͬ��
4 ���Ʋ��� BASIC ˳��
5 ���ӷ��� ��ѯѡ��
6 ������� NRZ
7 ���Ʒ�ʽ ����
8 ��� ��ֱ��żУ��������ݡ�ˮƽ��żУ���������
9 ��·���� 1:1 ����					(��Ϊ��Ե�Ĵ��䷽ʽ)
10 ���õ��� ����120�� ƽ��,������3 о����
11 ��·��ѹ DC5V��10%
*/

/*
(1)ͨѶ֡			//ע�⣺ʹ����У�鷽ʽ
Start bit DATA bit Parity bit Stop bit
1bit 8bit 1bit(ODD) 1bit
*/

/*
(2)֡�ṹ
��·PAD		��ʼ��־			��ַ			����		�շ���Ϣ�ı�			CRC			��־
2��15Byte	1Byte			��λ	��λ	(13H)		2��255Byte			2Byte		1Byte
					(7EH)		 ���ֽ�								���ɱ䳤��							 ��7EH��		
										(40H)	(01H)		
*/

/*
(3)״̬���ϵ�Ҫ���SDR�ݣ�TMS���յ�ϵͳ��
���	bit7	bit6	bit 5	bit 4	bit 3	bit 2	bit 1	bit 0	
0	��ʷ���ϲ�ѯ	˳�������ź�	�����¶���Ч	Ŀ���¶���Ч	ʱ����Ч	�������
1����(1�ų������ų�)	1������
0��δ����
1	��ʷ���ϲ�ѯ����Ϣ���
1��100	
2	��(10)	��(1)	BCD
3	��(10)	��(1)	BCD
4	��(10)	��(1)	BCD
5	Сʱ(10)	Сʱ(1)	BCD
6	����(10)	����(1)	BCD
7	��(10)	��(1)	BCD
8	�����������Ŀ���¶� 	200��300(20.0��30.0�� 1Bit:0.1��)	
9	�����������Ŀ���¶� 	200��300(20.0��30.0�� 1Bit:0.1��)		
10	�����¶� -32768��32767 (-3276.8��3276.7�� 1Bit:0.1��)	
11		
12	����ģʽ��0x01��ͣ����0x02��ͨ�磬0x04�����䣬
		0x08��ȫ�䣬0x10:�Զ�	
13	����	
14	����	
*/

/*
��4��״̬���ϵ�Ӧ���SD�ݣ��յ�ϵͳ��TMS��
���	bit7	bit6	bit 5	bit 4	bit 3	bit 2	bit 1	bit 0	
0	�յ�����1������Ϣ	��

	ѹ����1����	ѹ����1����	�������2����	�������1����	ͨ���4����	ͨ���3����	ͨ���2����	ͨ���1����	
	CPP11	CPF11	CWF12	CWF11	EFF14	EFF13	EFF12	EFF11	
1							ѹ����2����	ѹ����2����	
							CPP12	CPF12	
2	�յ�����2������Ϣ	
	ѹ����1����	ѹ����1����	�������2����	�������1����	ͨ���4����	ͨ���3����	ͨ���2����	ͨ���1����	
	CPP21	CPF21	CWF22	CWF21	EFF24	EFF23	EFF22	EFF21	
3							ѹ����2����	ѹ����2����	
					CP022	CP021	CPP22	CPF22	
4	�ط��¶�(��λ)	-32768��32767 (-3276.8��3276.7�� 1Bit:0.1��)	
5	�ط��¶�(��λ)		
6	�����¶ȣ���λ��	-32768��32767 (-3276.8��3276.7�� 1Bit:0.1��)	
7	�����¶ȣ���λ��		
8	�յ�����1״̬	1������     0��δ����
	����
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5��Ԥ��
6��Ԥ�ȣ�7������ͨ��	����ģʽ
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5���Զ�
6������,7����չ���磬8������ͨ��	
9					ѹ����2	ѹ����1	�������	ͨ���	
10	�յ�����2״̬	
	����
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5��Ԥ��
6��Ԥ�ȣ�7������ͨ��	����ģʽ
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5���Զ�
6������,7����չ���磬8������ͨ��	
11					ѹ����2	ѹ����1	�������	ͨ���	
12	��¼������Ϣ����	
13	����	
14	����	
15	����	
*/

/*
��5������ѯ��ʷ��Ϣλ��Чʱ��������Ϣ���ݸ�����������Ϊһ����ʷ���ϵ�ȫ����Ϣ���ݣ�
���	bit7	bit6	bit 5	bit 4	bit 3	bit 2	bit 1	bit 0	
0	������Ϣ���
1��100	
1	�յ�����1������Ϣ	��
��
1������
 0��δ����
	ѹ����1����	ѹ����1����	�������2����	�������1����	ͨ���4����	ͨ���3����	ͨ���2����	ͨ���1����	
	CPP11	CPF11	CWF12	CWF11	EFF14	EFF13	EFF12	EFF11	
2							ѹ����2����	ѹ����2����	
							CPP12	CPF12	
3	�յ�����2������Ϣ	
	ѹ����1����	ѹ����1����	�������2����	�������1����	ͨ���4����	ͨ���3����	ͨ���2����	ͨ���1����	
	CPP21	CPF21	CWF22	CWF21	EFF24	EFF23	EFF22	EFF21	
4							ѹ����2����	ѹ����2����	
					CP022	CP021	CPP22	CPF22	
5	�ط��¶�(��λ)	-32768��32767 (-3276.8��3276.7�� 1Bit:0.1��)	
6	�ط��¶�(��λ)		
7	�����¶ȣ���λ��	-32768��32767 (-3276.8��3276.7�� 1Bit:0.1��)	
8	�����¶ȣ���λ��		
9	�յ�����1״̬	1������     0��δ����
10	����
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5��Ԥ��
6��Ԥ�ȣ�7������ͨ��	����ģʽ
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5���Զ�
6������,7����չ���磬8������ͨ��	
11					ѹ����2	ѹ����1	�������	ͨ���	
12	�յ�����2״̬	
	����
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5��Ԥ��
6��Ԥ�ȣ�7������ͨ��	����ģʽ
0��ͨ��,1������,2��ȫ��
3����ů,4��ȫů,5���Զ�
6������,7����չ���磬8������ͨ��	
13					ѹ����2	ѹ����1	�������	ͨ���	
14	��(10)	��(1)	BCD
15	��(10)	��(1)	BCD
16	��(10)	��(1)	BCD
17	Сʱ(10)	Сʱ(1)	BCD
18	����(10)	����(1)	BCD
19	��(10)	��(1)	BCD
20	����	
21	����	
22	����	
*/

#include "TMS_ComunDefine.h"

#define  AIRSYSTEM_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  AIRSYSTEM_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ2�ֽڵ�����*/
#define  AIRSYSTEM_PAD1   					0x7E	//��·PAD1
#define  AIRSYSTEM_PAD2    					0x7E	//��·PAD2
#define  AIRSYSTEM_PAD3    					0x7E	//��·PAD3
#define  AIRSYSTEM_PAD4    					0x7E	//��·PAD4
#define  AIRSYSTEM_STARTFLAG				0x7E	//��ʼ��־
#define  AIRSYSTEM_ADDRESSL8				0x40	//�յ���ַ��8λ
#define  AIRSYSTEM_ADDRESSH8				0x01	//�յ���ַ��8λ
#define  AIRSYSTEM_CONTROLBYTE			0x13	//������
#define  AIRSYSTEM_ENDFLAG					0x7E	//������־

/*����������ֵ*/
#define  AIRSYSTEM_DataError  			-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  AIRSYSTEM_GetTimeoutError  -2			//��������֮��20ms����û�н��յ�����
#define  AIRSYSTEM_FrameError				-3			//��⵽֡������żУ�����ʱ
#define  AIRSYSTEM_CRCError         -4   		//BCC ���Ų����ϼƻ�ֵʱ
#define  AIRSYSTEM_GetError					-5			//��ʾ�������ݴ���
#define  AIRSYSTEM_TransException		-6			//��ʾ�����쳣


typedef struct _TMS_TO_ARI_SDR_TEXT
{
//	u8 TrainNumberState;		//ĳ�ڳ��յ�ϵͳ״̬	,���涨��ɰ�λ�Ķ���		/*��stm32�������� memcpy�ӵ�λ��ʼ���Ƹ�ÿһλ*/	
		u8 TrainNumber			 :3;			//�������
		u8 TimeEffective		 :1;  		// ʱ����Ч
		u8 TarTemperEffective:1;			//Ŀ���¶���Ч
		u8 EnvTemperEffective:1;			//�����¶���Ч
		u8 OrderStartSignal  :1;			//˳�������ź�
		u8 HistoryFaultQuery :1;			//��ʷ���ϲ�ѯ
	
	u8 HistoryFaultNumber;	//��ʷ���ϲ�ѯ����Ϣ���
	u8 Year;								//��	
	u8 Month;								//��
	u8 Day;									//��
	u8 Hour;								//ʱ
	u8 Minute;							//��
	u8 Second;							//��
	u8 RefriTargetTemperL8;	//�����������Ŀ���¶ȵ�8λ
	u8 RefriTargetTemperH8;	//�����������Ŀ���¶ȸ�8λ
	u8 EnvironmentTemperL8;	//�����¶ȵ�8λ
	u8 EnvironmentTemperH8;	//�����¶ȸ�8λ			//�ϲ�Ӧ�õ�ʱ��ע���¶��и�ֵ����s16�����ߵ��ֽ�ƴ�÷���
	u8 WorkMode;						//����ģʽ 0x01��ͣ����0x02��ͨ�磬0x04�����䣬0x08��ȫ�䣬0x10:�Զ�
	u8 Dummy1;							//δʹ��1
	u8 Dummy2;							//δʹ��2
}S_TMS_AirSystem_SDR_Text;

typedef struct _AIR_SYSTEM_SDR_Frame		//��������֡
{
	u8 PAD1;											//�յ���· PAD1
	u8 PAD2;											//�յ���· PAD2
	u8 StartFlag;									//��ʼ��־
	u8 AddressL8;									//��ַ��8λ
	u8 AddressH8;									//��ַ��8λ
	u8 ControlByte;								//������
	S_TMS_AirSystem_SDR_Text	DataText; //�շ���Ϣ�ı�
	u8 CRCL8;											//CRC���ֽ�
	u8 CRCH8;											//CRC���ֽ�
	u8 EndFlag;										//������־

}S_TMS_AirSystem_SDR_Frame;


typedef struct _AIR_SYSTEM_StateSD_Text			//��Ӧ״̬�����ı�
{
//	u8 AHU1FaultL8;								//�յ�����1������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 EFF11			:1;							//ͨ���1����
	u8 EFF12			:1;							//ͨ���2����
	u8 EFF13			:1;							//ͨ���3����
	u8 EFF14			:1;							//ͨ���4����
	u8 CWF11			:1;							//�������1����
	u8 CWF12			:1;							//�������2����
	u8 CPF11			:1;							//ѹ����1����
	u8 CPP11			:1;							//ѹ����1����
	
//	u8 AHU1FaultH8;								//�յ�����1������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 CPF12			:1;							//ѹ����2����
	u8 CPP12			:1;							//ѹ����2����
	u8 undef1			:6;							//δʹ��6λ
	
//	u8 AHU2FaultL8;								//�յ�����2������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 EFF21			:1;							//ͨ���1����
	u8 EFF22			:1;							//ͨ���2����
	u8 EFF23			:1;							//ͨ���3����
	u8 EFF24			:1;							//ͨ���4����
	u8 CWF21			:1;							//�������1����
	u8 CWF22			:1;							//�������2����
	u8 CPF21			:1;							//ѹ����1����
	u8 CPP21			:1;							//ѹ����1����
	
//	u8 AHU2FaultH8;								//�յ�����2������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 CPF22			:1;							//ѹ����2����
	u8 CPP22			:1;							//ѹ����2����
	u8 CP021			:1;
	u8 CP022			:1;
	u8 undef2			:4;							//δʹ��4λ
	
	u8 ReturnAirTemperL8;					//�ط��¶ȵ�8λ
	u8 ReturnAirTemperH8;					//�ط��¶ȸ�8λ 	//�ϲ�Ӧ�õ�ʱ��ע���¶��и�ֵ����s16�����ߵ��ֽ�ƴ�÷���
	u8 EnvironmentTemperL8;				//�����¶ȵ�8λ
	u8 EnvironmentTemperH8;				//�����¶ȸ�8λ			//�ϲ�Ӧ�õ�ʱ��ע���¶��и�ֵ����s16�����ߵ��ֽ�ƴ�÷���

//	u8 AHU1State;									//�յ�����1״̬
	/*��λ�����־*/
	u8 AHU1WorkMode		:4;					//�յ�����1����ģʽ
	u8 AHU1WorkState	:4;					//�յ�����1����״��
		
//	u8 AirState1;									//���9 �������ʵ��ֽ�
	/*��λ�����־*/
	u8 EF1State				:1;					//ͨ���״̬
	u8 CW1State				:1;					//������״̬
	u8 CP11State			:1;					//ѹ����1״̬
	u8 CP12State			:1;					//ѹ����2״̬
	u8 AHU1undefBit		:4;					//δʹ��4λ��

//	u8 AHU2State;									//�յ�����2״̬
	/*��λ�����־*/
	u8 AHU2WorkMode		:4;					//�յ�����2����ģʽ
	u8 AHU2WorkState	:4;					//�յ�����2����״��
	
//	u8 AirState2;									//���11 �������ʵ��ֽ�
	/*��λ�����־*/
	u8 EF2State				:1;					//ͨ���״̬
	u8 CW2State				:1;					//������״̬
	u8 CP21State			:1;					//ѹ����1״̬
	u8 CP22State			:1;					//ѹ����2״̬
	u8 AHU2undefBit		:4;					//δʹ��4λ��

	u8 RecordFaultNumber;					//��¼������Ϣ����
	u8 Dummy1;										//δʹ��1
	u8 Dummy2;										//δʹ��2
	u8 Dummy3;										//δʹ��3

}S_TMS_AirSystem_State_SD_Text;

typedef struct _AIR_SYSTEM_StateSD_Frame			//��Ӧ״̬����֡
{
//	u8 PAD1;											//�յ���· PAD1
//	u8 PAD2;											//�յ���· PAD2
	u8 StartFlag;									//��ʼ��־
	u8 AddressL8;									//��ַ��8λ
	u8 AddressH8;									//��ַ��8λ
	u8 ControlByte;								//������
	
	S_TMS_AirSystem_State_SD_Text DataText;	//��Ӧ״̬��Ϣ�ı�
	
	u8 CRCL8;											//CRC���ֽ�
	u8 CRCH8;											//CRC���ֽ�
	u8 EndFlag;										//������־
	
}S_TMS_AirSystem_State_SD_Frame;


typedef struct _AIR_SYSTEM_HistoryFaultSD_Text			//��Ӧ��ʷ���������ı�
{
	u8 HistoryFaultNumber;				//������Ϣ���
	
//	u8 AHU1FaultL8;								//�յ�����1������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 EFF11			:1;							//ͨ���1����
	u8 EFF12			:1;							//ͨ���2����
	u8 EFF13			:1;							//ͨ���3����
	u8 EFF14			:1;							//ͨ���4����
	u8 CWF11			:1;							//�������1����
	u8 CWF12			:1;							//�������2����
	u8 CPF11			:1;							//ѹ����1����
	u8 CPP11			:1;							//ѹ����1����
	
//	u8 AHU1FaultH8;								//�յ�����1������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 CPF12			:1;							//ѹ����2����
	u8 CPP12			:1;							//ѹ����2����
	u8 undef1			:6;							//δʹ��6λ
	
//	u8 AHU2FaultL8;								//�յ�����2������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 EFF21			:1;							//ͨ���1����
	u8 EFF22			:1;							//ͨ���2����
	u8 EFF23			:1;							//ͨ���3����
	u8 EFF24			:1;							//ͨ���4����
	u8 CWF21			:1;							//�������1����
	u8 CWF22			:1;							//�������2����
	u8 CPF21			:1;							//ѹ����1����
	u8 CPP21			:1;							//ѹ����1����
	
//	u8 AHU2FaultH8;								//�յ�����2������Ϣ��8λ
	/*��λ�����ʾ*/
	u8 CPF22			:1;							//ѹ����2����
	u8 CPP22			:1;							//ѹ����2����
	u8 CP021			:1;
	u8 CP022			:1;
	u8 undef2			:4;							//δʹ��4λ	
	
	u8 ReturnAirTemperL8;					//�ط��¶ȵ�8λ
	u8 ReturnAirTemperH8;					//�ط��¶ȸ�8λ 	//�ϲ�Ӧ�õ�ʱ��ע���¶��и�ֵ����s16�����ߵ��ֽ�ƴ�÷���
	u8 EnvironmentTemperL8;				//�����¶ȵ�8λ
	u8 EnvironmentTemperH8;				//�����¶ȸ�8λ			//�ϲ�Ӧ�õ�ʱ��ע���¶��и�ֵ����s16�����ߵ��ֽ�ƴ�÷���

//	u8 AHU1State;									//�յ�����1״̬
	/*��λ�����־*/
	u8 AHU1WorkMode		:4;					//�յ�����1����ģʽ
	u8 AHU1WorkState	:4;					//�յ�����1����״��
	
//	u8 AirState1;									//���9 �������ʵ��ֽ�
	/*��λ�����־*/
	u8 EF1State				:1;					//ͨ���״̬
	u8 CW1State				:1;					//������״̬
	u8 CP11State			:1;					//ѹ����1״̬
	u8 CP12State			:1;					//ѹ����2״̬
	u8 AHU1undefBit		:4;					//δʹ��4λ��
	
//	u8 AHU2State;									//�յ�����2״̬
	/*��λ�����־*/
	u8 AHU2WorkMode		:4;					//�յ�����2����ģʽ
	u8 AHU2WorkState	:4;					//�յ�����2����״��
	
//	u8 AirState2;									//���11 �������ʵ��ֽ�
	/*��λ�����־*/
	u8 EF2State				:1;					//ͨ���״̬
	u8 CW2State				:1;					//������״̬
	u8 CP21State			:1;					//ѹ����1״̬
	u8 CP22State			:1;					//ѹ����2״̬
	u8 AHU2undefBit		:4;					//δʹ��4λ��
	
	u8 Year;								//��	
	u8 Month;								//��
	u8 Day;									//��
	u8 Hour;								//ʱ
	u8 Minute;							//��
	u8 Second;							//��
	u8 Dummy1;										//δʹ��1
	u8 Dummy2;										//δʹ��2
	u8 Dummy3;										//δʹ��3

}S_TMS_AirSystem_HisFault_SD_Text;

typedef struct _AIR_SYSTEM_HistoryFaultSD_Frame //��Ӧ��ʷ��������֡
{
//	u8 PAD1;											//�յ���· PAD1
//	u8 PAD2;											//�յ���· PAD2
	u8 StartFlag;									//��ʼ��־
	u8 AddressL8;									//��ַ��8λ
	u8 AddressH8;									//��ַ��8λ
	u8 ControlByte;								//������
	
	S_TMS_AirSystem_HisFault_SD_Text DataText;	//��Ӧ״̬��Ϣ�ı�
	
	u8 CRCL8;											//CRC���ֽ�
	u8 CRCH8;											//CRC���ֽ�
	u8 EndFlag;		
}S_TMS_AirSystem_HisFault_SD_Frame;



/*
	����˵����
���ܣ�����豸���͸��յ���״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendAirSystem_SDR(S_TMS_AirSystem_SDR_Text * pInData,u8 * pOutData);  


#define AIR_STATE_SD_MODE 	  0
#define AIR_HISFAULT_SD_MODE	1
/*
	����˵����
���ܣ�����豸���ܿյ���״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ��������ݽṹ��)��
	GetMode:	STATE_SD_MODE  ��ʾ���ܵ�����Ϊ״̬��Ӧ
						HISFAULT_SD_MODE	��ʾ���ܵ�����Ϊ��ʷ������Ӧ	
����ֵ��0,�޴�  �����д�
*/
s8 GetAirSystem_SD(u8 *pInData,void *pOutData,u8 GetMode);		

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckAirSystemGetError(u8 *pInData);


/*
	����˵����
���ܣ���10������ת��ΪBCD��
����: 
			���룺u8 Dec   ��ת����ʮ��������
����ֵ��  ת�����BCD��
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 DecToBcd(u8 Dec);

/*
	����˵����
���ܣ���BCD��ת��Ϊ10������
����: 
			���룺u8 Bcd   ��ת��BCD��
����ֵ��  ת�����10������
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 BcdToDec(u8 Bcd);


void test_airsystem(void);

#endif  //__AIRSYSTEMCOMPRO_H__

