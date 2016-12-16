#ifndef __ACCCOMPRO_H__
#define __ACCCOMPRO_H__

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
9 ��·���� 1:1 ����					(��Ϊ��Ե�Ĵ��䷽ʽ)
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
(3)״̬����Ҫ���SDR�ݣ����װ�á��յ�װ�ã�
char
No.
bit7 bit6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0
0 STX(02H)
1 Address(01H)
2 Sub-address(00H)
3 Command(20H)
4 																		����ʱ��*1(bit0)
5 ��
6 ��
7 ��
8 ��
9 ��
10 ��
11 Dummy (00H) δʹ��
12 Dummy (00H) δʹ��
13 BCC ����
*1������ʱ��ʱ�������1��һ�������ϡ�
*/

/*
(4)״̬������Ӧ��SD�ݣ��յ�װ�á����װ�ã�
char
No.
bit7 bit6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0
0 STX(02H)
1 Address(01H)
2 Sub-address(00H)
3 Command(20H)
4 ȫů ��ů  0   ǿ���� ������ 0 �ͷ� �Զ� Unit1����״̬
5 ȫů ��ů  0 ǿ���� ������ 0 �ͷ� �Զ� Unit2����״̬
6 ��ȴ��1	��ȴ��2	ͨ���1	ͨ���2	ͨ���3		Unit1
	����		����		����		����			����		����
COF11 COF12 VEF11 VEF12 VEF13
7 ѹ����1		ѹ����1	ѹ����2		ѹ����2			Unit1
	�R�����o			����	�R�����o		����				����
	CPP11 CPF11 CPP12 CPF12
8 ��ȴ��1		��ȴ��2	ͨ���1		ͨ���2		ͨ���3		Unit2
	����			����		����			����			����			����
	COF21 COF22 VEF21 VEF22 VEF23	
9 ѹ����1		ѹ����1		ѹ����2		ѹ����2			Unit2
	ѹ������		����			ѹ�����o			����			����

	CPP21 CPF21 CPP22 CPF22
10 �����¶� D0H��F3H��5��40�棩 [1��:1bit]
11 Dummy��00H�� δʹ��
12 BCC ����

*/

#include "TMS_ComunDefine.h"

#define  MONITOR_ACC_DEVICE    1 		//�������1��Ϊ���������ڼ���豸��
#define  ACCCONDIR_DEVICE			 0		//�������1��Ϊ���������ڿյ��豸��

#define  ACC_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  ACC_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ4�ֽڵ�����*/
#define  ACC_STX						0x02	//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x20
#define  ACC_ADDRES   			0x01	//��ַ
#define  ACC_SUB_ADDRES			0x00	//�ӵ�ַ
#define  ACC_COMMAND    		0x20	//������

/*����������ֵ*/
#define  ACC_DataError  			-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  ACC_GetTimeoutError  -2			//��������֮��20ms����û�н��յ�����
#define  ACC_FrameError				-3			//��⵽֡������żУ�����ʱ
#define  ACC_BCCError         -4  	  //BCC ���Ų����ϼƻ�ֵʱ
#define  ACC_GetError					-5			//��ʾ�������ݴ���


typedef struct 			//���������ı�
{
	u8 Address;				//��ַ
	u8 Sub_Address;		//�ӵ�ַ
	u8 Command;				//������
	
	/*byte4*/
  u8 SetClock							:1;			//����ʱ��(ֻ�����һλ��Ч����ֻ��0 ��1)
	u8 TargetTempVaild			:1;
	u8 Byte4Undef6Bit				:6;
	
	u8 Year;					//��	
	u8 Month;					//��
	u8 Day;						//��
	u8 Hour;					//ʱ
	u8 Minute;				//��
	u8 Second;				//��
	
	u8 NetTargetTemp;		//�������Ŀ���¶�10��30(10.0��30.0�� 1Bit:1��)  //����Ŀ���¶�Ϊ10����
	
	/*A7Э������Ԥ�� 2�ֽ�*/
	u8 Undef2Byte[2];
}ACCSdrText;

typedef struct 			//����豸 ���͵� �յ�����������֡
{
	u8 ACCSdrStx	;					//��ʼ�ַ� 0x02
	ACCSdrText ACCSdrData;	//���ݶ�
	u8 ACCSdrBcc;						//bcc�㷨
}ACCSdrFrame; 


typedef struct 					//��Ӧ�����ı�
{
	u8 Address;						//��ַ
	u8 Sub_Address;				//�ӵ�ַ
	u8 Command;						//������	

	/*byte4*/
	u8 ACCU1_AutoMode					:1;		//�Զ�ģʽ
	u8 ACCU1_VentilMode				:1;		//ͨ��
	u8 ACCU1_StopMode					:1;		//ֹͣ
	u8 ACCU1_HalfColdeMode		:1;		//����
	u8 ACCU1_AllColdMode			:1;		//ǿ��
	u8 ACCU1_EmergVentilMode	:1;		//����ͨ��
	u8 ACCU1_HalfWarmMode			:1;		//��ů
	u8 ACCU1_AllWarmMode			:1;		//ȫů
	
	/*byte5*/
	u8 ACCU2_AutoMode					:1;		//�Զ�ģʽ
	u8 ACCU2_VentilMode				:1;		//ͨ��
	u8 ACCU2_StopMode					:1;		//ֹͣ
	u8 ACCU2_HalfColdeMode		:1;		//����
	u8 ACCU2_AllColdMode			:1;		//ǿ��
	u8 ACCU2_EmergVentilMode	:1;		//����ͨ��
	u8 ACCU2_HalfWarmMode			:1;		//��ů
	u8 ACCU2_AllWarmMode			:1;		//ȫů	
	
	/*byte6*/
	u8 ACCU1_Ventil4Fault			:1;		//�յ�����1ͨ���4����
	u8 ACCU1_Ventil3Fault			:1;		//�յ�����1ͨ���3����
	u8 ACCU1_Ventil2Fault			:1;		//�յ�����1ͨ���2����
	u8 ACCU1_Ventil1Fault			:1;		//�յ�����1ͨ���1����
	u8 Byte6Undef2Bit					:2;
	u8 ACCU1_Cold2Fault				:1;		//�յ�����1�������2����
	u8 ACCU1_Cold1Fault				:1;		//�յ�����1�������1����
	
	/*byte7*/
	u8 ACCU1_Comp2Fault				:1;		//�յ�����1ѹ����2����
	u8 ACCU1_Comp2PrePro			:1;		//�յ�����1ѹ����2ѹ������
	u8 Byte7Undef4Bit					:4;		
	u8 ACCU1_Comp1Fault				:1;		//�յ�����1ѹ����1����
	u8 ACCU1_Comp1PrePro			:1;		//�յ�����1ѹ����1ѹ������
	
	/*byte8*/
	u8 ACCU2_Ventil4Fault			:1;		//�յ�����2ͨ���4����
	u8 ACCU2_Ventil3Fault			:1;		//�յ�����2ͨ���3����
	u8 ACCU2_Ventil2Fault			:1;		//�յ�����2ͨ���2����
	u8 ACCU2_Ventil1Fault			:1;		//�յ�����2ͨ���1����
	u8 Byte8Undef2Bit					:2;
	u8 ACCU2_Cold2Fault				:1;		//�յ�����2�������2����
	u8 ACCU2_Cold1Fault				:1;		//�յ�����2�������1����

	/*byte9*/
	u8 ACCU2_Comp2Fault				:1;		//�յ�����2ѹ����2����
	u8 ACCU2_Comp2PrePro			:1;		//�յ�����2ѹ����2ѹ������
	u8 Byte9Undef4Bit					:4;		
	u8 ACCU2_Comp1Fault				:1;		//�յ�����2ѹ����1����
	u8 ACCU2_Comp1PrePro			:1;		//�յ�����2ѹ����1ѹ������
	
	u8 RoomTemperature;		//�����¶�
	
	/*
	u8 OtherFault;				//�����豸����
	u8 DriveRoomACCStatu;	//˾���ҿյ�����ת������״̬
	*/
	
	/*byte11*/
	u8 ACC_WasteVentil1Fault	:1;		//���ŷ��1����
	u8 ACC_WasteVentil2Fault	:1;		//���ŷ��2����	
	u8 ACC_EmergInvetFault		:1;		//����ͨ�����������
	u8 ACC_PowerFault					:1;		//�յ���Դ����
	u8 ACC_FreshSensor1Fault	:1;		//�·��¶ȴ�����1����
	u8 ACC_FreshSensor2Fault	:1;		//�·��¶ȴ�����2����
	u8 ACC_ReturnSensor1Fault	:1;		//�ط��¶ȴ�����1����
	u8 ACC_ReturnSensor2Fault	:1;		//�ط��¶ȴ�����1����
	
	/*byte12,˾���ҿյ�����ת������״̬*/
	u8 ACC_DrivAllColdState		:1;		//ǿ��
	u8 ACC_DrivHalfColdState	:1;		//����
	u8 ACC_DrivVentilState		:1;		//ͨ��
	u8 ACC_DrivStopState			:1;		//ֹͣ
	u8 ACC_DrivAutoState			:1;		//�Զ�
	u8 ACC_DrivNetContrState	:1;		//����
	u8 ACC_DRivHalfWarmState	:1;		//��ů
	u8 ACC_DrivAllWarmState		:1;		//ȫů
	
}ACCSdText;

typedef struct    // ����豸 ���յ� �յ��豸����Ӧ֡
{
	u8 ACCSdStx;						//��ʼ�ַ� 0x02
	ACCSdText	ACCSdData;
	u8 ACCSdBcc;						//Bcc�㷨
}ACCSdFrame; 



/*
	����˵����
���ܣ����Ϳյ���״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendACCSdr(ACCSdrText * pInData/*,u8 * pOutData*/);  //���Ϳյ���״̬�������ݡ�


/*
	����˵����
���ܣ����ܿյ���״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:������豸����õ�ÿ��״̬��ʶ(�ŵ�S_ACC_SD_State�ṹ��)��
					���յ��豸�����ܵ����ݾ����ı�����
����ֵ��0,�޴�  �����д�
*/
s8 GetACCSd(u8 *pInData,ACCSdText *pOutData);		//���ܿյ���״̬��Ӧ���ݡ�

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckGetError(u8 *pInData);

///*
//����˵����
//���ܣ������������������bcc��
//����: 
//	pData:��ڲ�������Ҫ����bcc�����ݡ�
//	len�� pData�����ݳ��� (�ֽڵ�λ )
//����ֵ���������ɵ�BCC��
//*/
//u8 GenerateBccChcek(void * pData,u8 len);

void test_ACC(void);
#endif  //__ACCCOMPRO_H__

