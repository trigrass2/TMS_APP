#ifndef __AIRCOMPRESSORPRO_H__
#define __AIRCOMPRESSORPRO_H__

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
(1)ͨѶ֡			//ע�⣺ʹ�õ�żУ�鷽ʽ
Start bit DATA bit Parity bit Stop bit
1bit 8bit 1bit(EVEN) 1bit
*/

/*
(2)֡�ṹ
Byte0			BYte1			Byte2			Byte3		Byte4			Byte5			Byte6			Byte7	
��ѹ��			������		��ʼ��ַ��	 ��ʼ��ַ	�Ĵ�����	 �Ĵ�����		CRC��			CRC���ֽ�
ԭ��ַ			�ֽ�			���ֽ�		���ֽ�			���ֽ�			�ֽ�
0xB0 			0x03			00				00				00			00											

*/

/*
(3)״̬����Ҫ���SDR�ݣ����װ�á���ѹ����Դ��
No. bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0
0 0XB0 �豸��ַ
1 0X03 ������
2 0x 00 ��ʼ��ַ���ֽ�
3 0x 01 ��ʼ��ַ���ֽ�
4 0x 00 �Ĵ��������ֽ�
5 0x 03 �Ĵ��������ֽ�
6 CRC ���ֽ� CRC ���ֽ�
7 CRC ���ֽ� CRC ���ֽ�
*/

/*
(4)״̬������Ӧ��SD�ݣ���ѹ����Դ�����װ�ã�
char
No. bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 ��ע
0 0XB0 �豸��ַ
1 0X03 ������
2 0x05 ���ݳ���
3 ��ѹǷѹ ��ѹ��ѹ FC Ƿѹ �������1 �������2 �������3 �����· ���ȱ��  ���ϴ���
4 IGBT ��ɢ�������� �Լ���� KM2�պϹ��� IGBT���� �������� Ԥ�� Ԥ�� Ԥ�� ���ϴ���
5 ���� ���� ���� ���� ���� Bit10 Bit9 Bit8 �����ѹ0-2048V �ֱ��ʣ�
6 bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 1V
7 bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 ��������ֱ� ��0-70A �ֱ��ʣ�1A
8 ���� ���� ���� ���� ���� ���� ���� Bit8 �����ѹ0-512V �ֱ��ʣ�
9 bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 1V
10 bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 ��������ֱ���0-70A �ֱ��ʣ�1A
11 ���� ���� ���� ���� ���� ���� ���� ���� ���Ƶ��
0 �C 50HZ����Χ0-250���ֱ��ʣ�
12 bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 0.2 HZ
13 bit 7 bit 6 bit 5 bit 4 bit 3 bit 2 bit 1 bit 0 �¶�
-127 - 127 �ֱ��ʣ�1 ��
14 CRC ���ֽ� CRC ���ֽ�
15 CRC ���ֽ� CRC ���ֽ�
*/

#include "TMS_ComunDefine.h"

#define  AIRCOMP_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  AIRCOMP_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ2�ֽڵ�����*/
#define  AIRCOMP_SOURCEADDRES   			0xB0	//��ѹ��Դ��ַ
#define  AIRCOMP_COMMAND    					0x03	//������

/*����������ֵ*/
#define  AIRCOMP_DataError  			-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  AIRCOMP_GetTimeoutError  -2			//��������֮��20ms����û�н��յ�����
#define  AIRCOMP_FrameError				-3			//��⵽֡������żУ�����ʱ
#define  AIRCOMP_CRCError         -4   		//BCC ���Ų����ϼƻ�ֵʱ
#define  AIRCOMP_GetError					-5			//��ʾ�������ݴ���

/*��ѹ��״̬��ʶ��λ��*/
/*����1*/
#define  AIRCOMP_NETVOLTLOW_POSIT			7		//��ѹǷѹ
#define  AIRCOMP_NETVOLTHIG_POSIT			6
#define  AIRCOMP_FCLOW_POSIT					5
#define  AIRCOMP_OUTOVER1_POSIT				4
#define  AIRCOMP_OUTOVER2_POSIT				3
#define  AIRCOMP_OUTOVER3_POSIT				2
#define  AIRCOMP_OUTSHORT_POSIT	 			1
#define  AIRCOMP_OUTDEFPHASE_POSIT		0

/*����2*/
#define  AIRCOMP_IGBT_OR_RAHeat_POSIT		7
#define  AIRCOMP_SELFFAULT_POSIT				6
#define  AIRCOMP_KM2CLOSEFAULT_POSIT 		5
#define  AIRCOMP_IGBTFAULT_POSIT				4
#define  AIRCOMP_OTHERFAULT_POSIT 			3

typedef struct AIR_COMPRE_SDR_TEXT		//���������ı�֡
{
	u8 AirCompressAddress;				//��ѹ��Դ��ַ
	u8 Command;										//������		������
  u8 StartAddressH8;						//��ʼ��ַ���ֽ�
	u8 StartAddressL8;						//��ʼ��ַ���ֽ�	
	u8 RegisterNumberH8;					//�Ĵ��������ֽ�
	u8 RegisterNumberL8;					//�Ĵ��������ֽ�
	u8 CRCH8;											//CRC���ֽ�
	u8 CRCL8;											//CRC���ֽ�
}S_TMS_AirCompre_SDR_Frame;

typedef struct AIR_COMPRE_SD_Text			//��Ӧ�����ı�֡
{
	u8 AirCompressAddress;		//�豸��ַ
	u8 Command;								//������
	u8 Datalen;								//���ݳ���
	u8 FaultCode1;						//���ϴ���1
	u8 FaultCode2;						//���ϴ���2
	u8 InputVoltageH8;				//�����ѹ��8λ
	u8 InputVoltageL8;				//�����ѹ��8λ	
	u8 InputElectric;					//�������
	u8 OutputVoltageH8;				//�����ѹ��8λ
	u8 OutputVoltageL8;				//�����ѹ��8λ
	
	u8 OutputElectric;				//�������
	u8 OutputHzH8;						//���Ƶ�ʸ�8λ
	u8 OutputHzL8;						//���Ƶ�ʵ�8λ
	s8 RoomTemperature;				//�¶� 	(-127 -- 127)
	u8 CRCH8;									//CRC���ֽ�
	u8 CRCL8;									//CRC���ֽ�
	
}S_TMS_AirCompre_SD_Frame;


typedef struct AirCompre_SD_State		//�յ�����Ӧ״̬����,��ʾĳλ��״̬λ
{
	u8 AirCompressAddress;		//�豸��ַ
	u8 Command;								//������
	u8 Datalen;								//���ݳ���
	
	/*���ϴ���1*/
	u8 NetVoltLowBit :1;			//��ѹǷѹ
	u8 NetVoltHigBit :1;			//��ѹ��ѹ
	u8 FCLowBit			 :1;			//FCǷѹ
	u8 OutputOver1	 :1;			//�������1
	u8 OutputOver2	 :1;			//�������2
	u8 OutputOver3	 :1;			//�������3
	u8 OutputShort	 :1;			//�����·
	u8 OutputDefPhase:1;			//���ȱ��
	
	/*���ϴ���2*/
	u8 IGBT_Or_RaOverHeat :1;		//IGBT �� ɢ��������
	u8 SelfFault 		 :1;				//�Լ����
	u8 KM2CloseFalut :1;				//KM2�պϹ���
	u8 IGBTFault		 :1;				//IGBT����
	u8 OtherFault		 :1;				//��������
	u8 undef1				 :1;				//Ԥ��
	u8 undef2				 :1;				//Ԥ��
	u8 undef3				 :1;				//Ԥ��
	
	u8 InputVoltageH8;				//�����ѹ��8λ
	u8 InputVoltageL8;				//�����ѹ��8λ	
	u8 InputElectric;					//�������
	u8 OutputVoltageH8;				//�����ѹ��8λ
	u8 OutputVoltageL8;				//�����ѹ��8λ
	u8 OutputElectric;				//�������
	u8 OutputHzH8;						//���Ƶ�ʸ�8λ
	u8 OutputHzL8;						//���Ƶ�ʵ�8λ
	s8 RoomTemperature;				//�¶� 	(-127 -- 127)
	u8 CRCH8;									//CRC���ֽ�
	u8 CRCL8;									//CRC���ֽ�
	
}S_AirCompre_SD_StateBit;

/*
	����˵����
���ܣ�����豸���Ϳ�ѹ����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendAirCompre_SDR(S_TMS_AirCompre_SDR_Frame * pInData,u8 * pOutData);  


/*
	����˵����
���ܣ����ܿ�ѹ����״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_AirCompre_SD_StateBit�ṹ��)��
����ֵ��0,�޴�  �����д�
*/
s8 GetAirCompre_SD(u8 *pInData,S_AirCompre_SD_StateBit *pOutData);		

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckAirCompreGetError(u8 *pInData);

/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
����ֵ���������ɵ�BCC��
*/
u8 GenerateAirCompreBccChcek(void * pData,u8 len);


//Ҫ���ĳλ�����ݣ��Ƚ������ƶ������λ��Ȼ�����Ƶ������λ��		//W H H ͳһ�ŵ�CommunPro.h��
/*
����˵����
���ܣ�Ҫ���ĳλ������
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
u8 GetAirCompreDataBit(u8 * Data,u8 GetPosit);

void test_aircomper(void);

#endif  //__AIRCOMPRESSORPRO_H__

