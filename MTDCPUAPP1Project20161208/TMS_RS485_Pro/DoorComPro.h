#ifndef __DOORCOMPRO_H__
#define __DOORCOMPRO_H__

/*
(1)������
1 ͨѶ��ʽ 3 ��ʽ��˫��ͨѶ��ʽ
2 ͨѶ�ٶ� 19200bps��0.2%
3 ͬ����ʽ ��ͬ��
4 ���Ʋ��� BASIC ˳��
5 ���ӷ��� ��ѯѡ��
6 ������� NRZ
7 ���Ʒ�ʽ ����
8 ��� ��ֱ��żУ��������ݡ�ˮƽ��żУ���������
9 ��·���� 1:8 ����					(�������ߵķ�ʽ)
10 ���õ��� ����120�� ƽ��,������3 о����
11 ��·��ѹ DC5V��10%
*/

/*
(1)ͨѶ֡			//ע�⣺ʹ�õ���У�鷽ʽ
Start bit DATA bit Parity bit Stop bit
1bit 8bit 1bit(ODD) 1bit
*/

/*
(2)֡�ṹ
No.0				No.1��n						No.n+1
STX(02H)  TEXT��DATA �ַ�����  BCC ����
*/

/*
(3)״̬����Ҫ���SDR�ݣ����װ�á����ſ���װ�ã�
char
No.
bit7 bit6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0
0 STX(02H)
1 Address(01H��08H)
2 Sub-address(00H)
3 Command(20H)
4 																		����ʱ��*1(bit0)
5 ��
6 ��
7 ��
8 ��
9 ��
10 ��
11 Speed 00H��FFH��0��255km/h�� [1km/h:1bit]
12 Dummy (00H) δʹ��
13 BCC ����
*1������ʱ��ʱ�������1��һ�������ϡ�
*/

/*
(4)״̬������Ӧ��SD�ݣ����ſ���װ�á����װ�ã�
char
No.
bit7 bit6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0
0 STX(02H)
1 Address(01H��08H)
2 Sub-address(00H)
3 Command(20H)
4				5km/h		�Ÿ��� .	��,���Ŷ�	����			����
			�źż��						���� 			�ر�״̬		����״̬
5			���ŷ���		���ŷ���		��ϵͳ		����װ��
			ѹ				ѹ .			���� .	����	
6 Dummy(00H) δʹ��
7 BCC ����

*/

#include "TMS_ComunDefine.h"

#define  DOOR_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���		//�������û��Զ���
#define  DOOR_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ4�ֽڵ�����*/
#define  DOOR_STX							0x02	//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x20
#define  DOOR_ADDRES_MIN 			0x01	//��ַ (0x01 ~ 0x08 )
#define  DOOR_ADDRES_MAX			0x08
#define  DOOR_SUB_ADDRES			0x00	//�ӵ�ַ
#define  DOOR_COMMAND    			0x20	//������

/*����������ֵ*/
#define  DOOR_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  DOOR_GetTimeoutError	  -2			//��������֮��20ms����û�н��յ�����
#define  DOOR_FrameError				-3			//��⵽֡������żУ�����ʱ
#define  DOOR_BCCError       	  -4  	 	//BCC ���Ų����ϼƻ�ֵʱ
#define  DOOR_GetError					-5			//��ʾ�������ݴ���

typedef struct 		//���������ı�
{
	u8 Address;				//��ַ
	u8 Sub_Address;		//�ӵ�ַ
	u8 Command;				//������
  u8 SetClock;			//����ʱ��(ֻ�����һλ��Ч����ֻ��0 ��1)
	u8 Year;					//��	
	u8 Month;					//��
	u8 Day;						//��
	u8 Hour;					//ʱ
	u8 Minute;				//��
	u8 Second;				//��
	u8 Speed;					//00H��FFH��0��255km/h�� [1km/h:1bit]
	u8 Dummy2;				//δʹ��2
}DoorSdrText;//DoorSdrText;

typedef struct 		//����豸 ���͵� ���ſ��Ƶ���������֡
{
	u8 DoorSdrStx	;						//��ʼ�ַ� 0x02
	DoorSdrText DoorSdrData;	//���ݶ�
	u8 DoorSdrBcc;						//bcc�㷨
}DoorSdrFrame;	


typedef struct 			//��Ӧ�����ı�
{
	u8 Address;						//��ַ
	u8 Sub_Address;				//�ӵ�ַ
	u8 Command;						//������

	/*Byte4*/
		/*����״̬*/
	u8 DoorOpenState			:1;	//  ���� ��״̬
	u8 DoorCloseState			:1;	//	���� �ر�״̬
	u8 DoorOpenCloseing		:1;	//	���� �����ض�����
	u8 Byte4UndefBit3			:1;
	u8 DoorIsolation			:1;	//	�Ÿ���	
	u8 SpeedDetection5km	:1;	//	5km/h�źż��
	u8 Byte4UndefBit6			:1;
	u8 Byte4UndefBit7			:1;
	
	/*byte5*/
	/*���Ź���*/
	u8 EmergUnlock				:1;	//����װ�ò���
	u8 DoorSystemFault		:1;	//��ϵͳ����
	u8 Byte5UndefBit2			:1;
	u8 Byte5UndefBit3			:1;
	u8 OpenDoorExtrusion	:1;	//���ŷ���ѹ
	u8 CloseDoorExtrusion	:1;	//���ŷ���ѹ
	u8 Byte5UndefBit6			:1;
	u8 Byte5UndefBit7			:1;

	u8 Dummy;							//δʹ��
}DoorSdText;

typedef struct    // ����豸 ���յ� ���ſ����豸����Ӧ֡
{
	u8 DoorSdStx;							//��ʼ�ַ� 0x02
	DoorSdText	DoorSdData;
	u8 DoorSdBcc;							//Bcc�㷨
}DoorSdFrame;

/*
	����˵����
���ܣ����ͳ��ſ��Ƶ�״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendDoorSdr(DoorSdrText * pInData/*,u8 * pOutData*/);  //���ͳ��ſ��Ƶ�״̬�������ݡ�


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_DOOR_SD_State�ṹ��)��
����ֵ��0,�޴�  �����д�
*/
s8 GetDoorSd(u8 *pInData,DoorSdFrame *DoorSdFrame);	//����豸���ܳ��ſ��Ƶ�״̬��Ӧ���ݡ�

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckDoorGetError(u8 *pInData);

///*
//����˵����
//���ܣ������������������bcc��
//����: 
//	pData:��ڲ�������Ҫ����bcc�����ݡ�
//	len�� pData�����ݳ��� (�ֽڵ�λ )
//����ֵ���������ɵ�BCC��
//*/
//u8 GenerateDoorBccChcek(void * pData,u8 len);


void testDoor(void);
#endif  //__DOORCOMPRO_H__

