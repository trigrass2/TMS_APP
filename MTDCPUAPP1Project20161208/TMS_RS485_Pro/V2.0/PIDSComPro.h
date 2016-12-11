#ifndef __AIRSYSTEMCOMPRO_H__
#define __AIRSYSTEMCOMPRO_H__

/*
(1)������
1 ͨѶ��ʽ RS-485��3��ʽ��˫��
2 ͨѶ�ٶ� 19200bps��0.2%
3 ͬ����ʽ ��ͬ��
4 ���Ʋ��� ����HDLC ����
5 ���ӷ��� ��ѯѡ��
6 ������� NRZ
7 ���Ʒ�ʽ ����
8 ��� CRC-CCITT
9 ��·���� RS-485
10 ���õ��� ����120�� ƽ��,������3 о����
11 ��·��ѹ DC5V��10%
*/

/*
////////(1)ͨѶ֡			//ע�⣺ʹ����У�鷽ʽ
////////Start bit DATA bit Parity bit Stop bit
////////1bit 8bit 1bit(ODD) 1bit
*/

/*
(2)֡�ṹ
��·PAD		��ʼ��־			��ַ			����		�շ���Ϣ�ı�			CRC			��־
2��4Byte	1Byte			��λ	��λ	(13H)			N Byte				2Byte		1Byte
					(7EH)		 ���ֽ�								���ɱ䳤��							 ��7EH��		
										(60H)	(01H)		
*/

/*
(3)״̬����Ҫ��SDR���г����װ�á���������װ�� PIDS��
TEXT							Bit No						�� ע
				7 	6 	5	 4 	3 	2	 1	 0
0 Command ��20H
1 �� �� �� �� �� �� ʱ������ ʱ���Є� *1
2��ǰվ		�г����		վ�����		��	��һվ			�г�Ŀ�ĵ�		���վ			��·
 ID ��Ч	ID ��Ч		��Ч					ID ��Ч		ID ��Ч			ID ��Ч		ID ��Ч
3	�����		�Ҳ���		 1 ��	 ATO	 �۷�		������վ  ���ﳵվ		���ﳵվԤ��
	״̬		״̬
4 ���� ��00H
5 �� (10 λ) �� (1 λ)			�����ĺ���	2 λ�� BCD
6 �� (10 λ) �� (1 λ) BCD
7 �� (10 λ) �� (1 λ) BCD
8 ʱ (10 λ) ʱ (1 λ) BCD
9 �� (10 λ) �� (1 λ) BCD
10 �� (10 λ) �� (1 λ) BCD
11 �г����ID ��1000 λ BCD *4
12 �г����ID �� 100 λ BCD *4
13 �г����ID �� 10 λ BCD *4
14 �г����ID �� 1 λ BCD *4
15 ��·ID ��00H
16 ���վID(H)		��0000H��FFFFH (2�ֽ�)
17 ���վID(L)
18 �г�Ŀ�ĵ�ID(H)	��0000H��FFFFH *4,*5
19 �г�Ŀ�ĵ�ID(L)
20 ��ǰվID(H)	��0000H��FFFFH *4,*5
21 ��ǰվID(L)
22 ��һվID(H)��0000H��FFFFH *4,*5
23 ��һվID(L)
24 ���� ��00H
25 ���� ��00H
26 վ�����(H)	��0��65535m *5
27 վ�����(L)
28 ���� ��00H
29 ���� ��00H
*/

/*
��4��״̬������ӦSD����������װ�á��г����װ�ã�
TEXT				Bit No				�� ע
				7 6 5 4 3 2 1 0
0 Command ��30H
1
|	����		��00H
27
*/

#include "TMS_ComunDefine.h"

#define  PIDS_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  PIDS_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ2�ֽڵ�����*/
#define  PIDS_PAD1   						0x7E	//��·PAD1
#define  PIDS_PAD2    					0x7E	//��·PAD2
#define  PIDS_PAD3    					0x7E	//��·PAD3
#define  PIDS_PAD4    					0x7E	//��·PAD4
#define  PIDS_STARTFLAG					0x7E	//��ʼ��־
#define  PIDS_ADDRESSL8					0x60	//PIDS��ַ��8λ
#define  PIDS_ADDRESSH8					0x01	//PIDS��ַ��8λ
#define  PIDS_CONTROLBYTE				0x13	//������
#define  PIDS_ENDFLAG						0x7E	//������־

#define  PIDS_SDR_COMMAND				0x20	//״̬����Ҫ��SDR �����֣��г����װ�á���������װ�ã�
#define  PIDS_SD_COMMAND				0x30	//״̬������ӦSD  �����֣���������װ�á��г����װ�ã�


/*����������ֵ*/
#define  PIDS_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  PIDS_GetTimeoutError   -2			//��������֮��20ms����û�н��յ�����
#define  PIDS_FrameError				-3			//��⵽֡������żУ�����ʱ
#define  PIDS_CRCError          -4   		//BCC ���Ų����ϼƻ�ֵʱ
#define  PIDS_GetError					-5			//��ʾ�������ݴ���
#define  PIDS_TransException		-6			//��ʾ�����쳣



typedef struct _TMS_TO_PIDS_SDR_TEXT
{
	/*0*/
	u8 Command	;						//PIDS������
	
	/*1*/
	/*λ���壬��stm32�������� memcpy�ӵ�λ��ʼ���Ƹ�ÿһλ*/	
	u8 TimeEffective 			:1;		//ʱ����Ч
	u8 TimeSet			 			:1;		//ʱ������
	u8 undef1							:6;		//δʹ�õ�6λ
	
	/*2*/
	u8 LineIDEffective		:1;		//��·ID��Ч
	u8 StartStationIDEff 	:1;		//���վID ��Ч
	u8 TrainDestIDEff			:1;		//�г�Ŀ�ĵ�ID ��Ч
	u8 NextStationIDEff		:1;		//��һվID ��Ч
	u8 undef2							:1;		//δʹ�õ�1λ
	u8 StationDistanceEff :1;		//վ�������Ч
	u8 TrainNumIDEff			:1;		//�г����ID ��Ч
	u8 CurrentStationIDEff:1;		//��ǰվID ��Ч
	
	/*3*/
	u8 ArrivStationForecast :1;	//���ﳵվԤ��
	u8 ArrivStation					:1;	//���ﳵվ
	u8 LeaveStation					:1;	//������վ
	u8 FoldBack							:1;	//�۷�
	u8 ATO									:1;	//ATO��Ч
	u8 Train1								:1;	//1��
	u8 RightDoorState				:1;	//�Ҳ���״̬
	u8 LeftDoorState				:1;	//�����״̬
	
	/*4*/
	u8 Reserve1;									//���� 00H
	
	/*5~10*/
	u8 Year;										//��	
	u8 Month;										//��
	u8 Day;											//��
	u8 Hour;										//ʱ
	u8 Minute;									//��
	u8 Second;									//��
	
	/*11~14*/
	u8 TrainNumID4;							//�г����ID ��1000 λ(10���Ƶ�ǧλ)
	u8 TrainNumID3;							//�г����ID ��100 λ(10���Ƶİ�λ)
	u8 TrainNumID2;							//�г����ID ��10 λ(10���Ƶ�ʮλ)
	u8 TrainNumID1;							//�г����ID ��1 λ(10���Ƶĸ�λ)
	
	/*15*/
	u8 LineID;									//��·ID ��00H
	
	/*16 17*/
	u8 StartStationIDH8;				//��ʼվID��8λ
	u8 StartStationIDL8;				//��ʼվID��8λ
	
	/*18 19*/
	u8 TrainDestIDH8;						//�г�Ŀ�ĵ�ID(H8λ��
	u8 TrainDestIDL8;						//�г�Ŀ�ĵ�ID(L8λ��	
	
	/*20 21*/
	u8 CurrentStationIDH8;			//��ǰվID(H)
	u8 CurrentStationIDL8;			//��ǰվID(L)
	
	/*22 23*/
	u8 NextStationIDH8;					//��һվID(H)
	u8 NextStationIDL8;					//��һվID(L)
	
	/*24 25*/				
	u8 Reserve2;								//���� ��00H
	u8 Reserve3;								//���� ��00H
	
	/*26 27*/
	u8 StationDistanceH8;				//	վ�����(H)
	u8 StationDistanceL8;				//	վ�����(L)	
	
	/*28 29*/
	u8 Reserve4;								//���� ��00H
	u8 Reserve5;								//���� ��00H

}S_TMS_PIDS_SDR_Text;

typedef struct _PIDS_SDR_Frame		//��������֡
{
	u8 PAD1;											//�յ���· PAD1
	u8 PAD2;											//�յ���· PAD2
	u8 StartFlag;									//��ʼ��־
	u8 AddressL8;									//��ַ��8λ
	u8 AddressH8;									//��ַ��8λ
	u8 ControlByte;								//������
	S_TMS_PIDS_SDR_Text	DataText; //�շ���Ϣ�ı�
	u8 CRCL8;											//CRC���ֽ�
	u8 CRCH8;											//CRC���ֽ�
	u8 EndFlag;										//������־

}S_TMS_PIDS_SDR_Frame;


typedef struct _PIDS_StateSD_Text			//��Ӧ״̬�����ı�
{
	/*0*/
	u8 Command	;						//PIDS������
	
	/*1 - 27*/
	u8 Reserve[26];				 //���� ��00H

}S_TMS_PIDS_SD_Text;

typedef struct _PIDS_SD_Frame			//��Ӧ״̬����֡
{
//	u8 PAD1;											//�յ���· PAD1
//	u8 PAD2;											//�յ���· PAD2
	u8 StartFlag;									//��ʼ��־
	u8 AddressL8;									//��ַ��8λ
	u8 AddressH8;									//��ַ��8λ
	u8 ControlByte;								//������
	
	S_TMS_PIDS_SD_Text DataText;	//��Ӧ״̬��Ϣ�ı�
	
	u8 CRCL8;											//CRC���ֽ�
	u8 CRCH8;											//CRC���ֽ�
	u8 EndFlag;										//������־
	
}S_TMS_PIDS_SD_Frame;

	
/*
	����˵����
���ܣ�����豸���͸�PIDS��״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendPIDS_SDR(S_TMS_PIDS_SDR_Text * pInData,u8 * pOutData);  


/*
	����˵����
���ܣ�����豸����PISD��״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ��������ݽṹ��)��

����ֵ��0,�޴�  �����д�
*/
s8 GetPIDS_SD(u8 *pInData,void *pOutData);		

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckPIDSGetError(u8 *pInData);




void test_pids(void);

#endif  //__AIRSYSTEMCOMPRO_H__

