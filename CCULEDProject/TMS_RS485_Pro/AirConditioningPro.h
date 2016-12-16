#ifndef __AIRCONDITIONINGPRO_H__
#define __AIRCONDITIONINGPRO_H__

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

#define  MONITOR_AIR_DEVICE    1 		//�������1��Ϊ���������ڼ���豸��
#define  AIRCONDIR_DEVICE			 0		//�������1��Ϊ���������ڿյ��豸��

#define  AIR_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  AIR_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ4�ֽڵ�����*/
#define  AIR_STX						0x02	//���巢�� �Ŀ�ʼ�ֽ�Ϊ0x20
#define  AIR_ADDRES   			0x01	//��ַ
#define  AIR_SUB_ADDRES			0x00	//�ӵ�ַ
#define  AIR_COMMAND    		0x20	//������

/*����������ֵ*/
#define  AIR_DataError  			-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  AIR_GetTimeoutError  -2		//��������֮��20ms����û�н��յ�����
#define  AIR_FrameError				-3		//��⵽֡������żУ�����ʱ
#define  AIR_BCCError         -4   //BCC ���Ų����ϼƻ�ֵʱ
#define  AIR_GetError					-5		//��ʾ�������ݴ���

/*�յ�״̬��ʶ��λ��*/
#define  AIR_ALLWAEM_POSIT				7	//ȫů����λ��
#define  AIR_HALFWAEM_POSIT				6
#define  AIR_HIGHCOOL_POSIT				4
#define  AIR_WEAKCOLD_POSIT				3
#define  AIR_BLOWING_POSIT				1
#define  AIR_AUTOMATIC_POSIT			0

/*��ȴ�� ͨ���*/
#define  AIR_COOLER1FAULT_POSIT			7
#define  AIR_COOLER2FAULT_POSIT			6
#define  AIR_VETILATOR1FAULT_POSIT 	3
#define  AIR_VETILATOR2FAULT_POSIT	2
#define  AIR_VETILATOR3FAULT_POSIT 	1

/*ѹ����*/
#define  AIR_COMPRESSOR1PROTECT_POSIT	 7
#define  AIR_COMPRESSOR1FAULT_POSIT		 6
#define  AIR_COMPRESSOR2PROTECT_POSIT	 1
#define  AIR_COMPRESSOR2FAULT_POSIT		 0


typedef struct SDR_TEXT		//���������ı�
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
	u8 Dummy1;				//δʹ��1
	u8 Dummy2;				//δʹ��2
	
}S_TMS_AIR_SDR_Text;

typedef struct SDR_Frame		//����豸 ���͵� �յ�����������֡
{
	u8 SDR_STX	;					//��ʼ�ַ� 0x02
	S_TMS_AIR_SDR_Text SDR_Data;	//���ݶ�
	u8 SDR_Bcc;						//bcc�㷨
} S_TMS_AIR_SDR_Frame;


typedef struct SD_Text			//��Ӧ�����ı�
{
	u8 Address;						//��ַ
	u8 Sub_Address;				//�ӵ�ַ
	u8 Command;						//������
	u8 U1ActiveState;			//��Ԫ1�Ķ���״̬
	u8 U2ActiveState;			//��Ԫ2�Ķ���״̬
	u8 U1Fault1;					//��Ԫ1����1��״̬
	u8 U1Fault2;					//��Ԫ1����2��״̬
	u8 U2Fault1;					//��Ԫ2����1��״̬
	u8 U2Fault2;					//��Ԫ2����2��״̬
	u8 RoomTemperature;		//�����¶�
	u8 Dummy;							//δʹ��
	
}S_TMS_AIR_SD_Text;

typedef struct SD_Frame   // ����豸 ���յ� �յ��豸����Ӧ֡
{
	u8 SD_STX;						//��ʼ�ַ� 0x02
	S_TMS_AIR_SD_Text	SD_Data;
	u8 SD_Bcc;						//Bcc�㷨
} S_TMS_AIR_SD_Frame;

typedef struct Air_SD_State		//�յ�����Ӧ״̬����,��ʾĳλ��״̬λ
{
	u8	U1AllWarm;				//u1ȫů״̬λ
	u8	U1HalfWarm;				//u1��ů״̬λ
	u8  U1HighCool;				//u1ǿ����״̬λ
	u8  U1WeakCold;				//u1������״̬λ
	u8  U1Blowing; 				//u1�ͷ�״̬λ
	u8 	U1Automatic;  	 	//u1�Զ�״̬λ
	
	u8	U2AllWarm;				//u2ȫů״̬λ
	u8	U2HalfWarm;				//u2��ů״̬λ
	u8  U2HighCool;				//u2ǿ����״̬λ
	u8  U2WeakCold;				//u2������״̬λ
	u8  U2Blowing; 			//u2�ͷ�״̬λ
	u8 	U2Automatic;   		//u2�Զ�״̬λ
	
	u8	U1Cooler1Fault;					//u1��ȴ��1����״̬λ
	u8	U1Cooler2Fault;					//u1��ȴ��2����״̬λ
	u8  U1Vetilator1Fault;			//u1ͨ���1����״̬λ
	u8  U1Vetilator2Fault;			//u1ͨ���2����״̬λ
	u8  U1Vetilator3Fault; 			//u1ͨ���3����״̬λ
	
	u8 	U1Compressor1Protect; 	//u1ѹ����1ѹ������
	u8	U1Compressor1Fault;			//u1ѹ����1����
	u8 	U1Compressor2Protect; 	//u1ѹ����2ѹ������
	u8	U1Compressor2Fault;			//u1ѹ����2����	
	
	u8	U2Cooler1Fault;					//u2��ȴ��1����״̬λ
	u8	U2Cooler2Fault;					//u2��ȴ��2����״̬λ
	u8  U2Vetilator1Fault;			//u2ͨ���1����״̬λ
	u8  U2Vetilator2Fault;			//u2ͨ���2����״̬λ
	u8  U2Vetilator3Fault; 			//u2ͨ���3����״̬λ
	
	u8 	U2Compressor1Protect; 	//u2ѹ����1ѹ������
	u8	U2Compressor1Fault;			//u2ѹ����1����
	u8 	U2Compressor2Protect; 	//u2ѹ����2ѹ������
	u8	U2Compressor2Fault;			//u2ѹ����2����	
	
	u8  RoomTemperature;				//�����¶�			D0H��F3H��5��40�棩 [1��:1bit]
	
}S_AIR_SD_State;

/*
	����˵����
���ܣ����Ϳյ���״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendAirCondit_SDR(S_TMS_AIR_SDR_Text * pInData,u8 * pOutData);  //���Ϳյ���״̬�������ݡ�


/*
	����˵����
���ܣ����ܿյ���״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:������豸����õ�ÿ��״̬��ʶ(�ŵ�S_AIR_SD_State�ṹ��)��
					���յ��豸�����ܵ����ݾ����ı�����
����ֵ��0,�޴�  �����д�
*/
s8 GetAirCondit_SD(u8 *pInData,S_AIR_SD_State *pOutData);		//���ܿյ���״̬��Ӧ���ݡ�

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

	
//Ҫ���ĳλ�����ݣ��Ƚ������ƶ������λ��Ȼ�����Ƶ������λ��	//W H H ͳһ�ŵ�CommunPro.h��
/*
����˵����
���ܣ�Ҫ���ĳλ������
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
//u8 GetDataBit(u8 * Data,u8 GetPosit);

void test(void);
#endif  //__AIRCONDITIONINGPRO_H__

