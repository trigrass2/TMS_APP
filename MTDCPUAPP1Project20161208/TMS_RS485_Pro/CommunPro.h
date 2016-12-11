#ifndef __COMMUNPRO_H__
#define	__COMMUNPRO_H__

#include "TMS_ComunDefine.h"

//#define MC1_MTD_NODEID	 		3		//MC1�����װ�ã��ڵ�� 3
//#define T_MTD_NODEID   	 		4		//T�����װ�ã��ڵ��4
//#define M_MTD_NODEID		 		5		//M�����װ�ã� �ڵ��5
//#define T1_MTD_NODEID				6		//T1�����װ�ã��ڵ��6
//#define T2_MTD_NODEID   		7		//T2�����װ�ã��ڵ��7
//#define MC2_MTD_NODEID   		8		//MC2�����װ�ã��ڵ��8

#define MC1_MTD1_MFD1_NODEID				0x31
#define MC1_MTD1_MFD2_NODEID				0x32
#define MC1_MTD1_MFD3_NODEID				0x33
#define MC1_MTD1_MFD4_NODEID				0x34

#define T_MTD2_MFD1_NODEID					0x41
#define T_MTD2_MFD2_NODEID					0x42
#define T_MTD2_MFD3_NODEID					0x43
#define T_MTD2_MFD4_NODEID					0x44
		
#define M_MTD3_MFD1_NODEID					0x51
#define M_MTD3_MFD2_NODEID					0x52
#define M_MTD3_MFD3_NODEID					0x53
#define M_MTD3_MFD4_NODEID					0x54
		
#define T1_MTD4_MFD1_NODEID					0x61
#define T1_MTD4_MFD2_NODEID					0x62
#define T1_MTD4_MFD3_NODEID					0x63
#define T1_MTD4_MFD4_NODEID					0x64
		
#define T2_MTD5_MFD1_NODEID					0x71
#define T2_MTD5_MFD2_NODEID					0x72
#define T2_MTD5_MFD3_NODEID					0x73
#define T2_MTD5_MFD4_NODEID					0x74
		
#define MC2_MTD6_MFD1_NODEID				0x81
#define MC2_MTD6_MFD2_NODEID				0x82
#define MC2_MTD6_MFD3_NODEID				0x83
#define MC2_MTD6_MFD4_NODEID				0x84



/*DI�ڵ�忨����Ķ���*/

/*MC1/MC2 - MFD1�ӿ�*/
/*110V*/
#define MC_MFD1_LeftOpenDoor   			2			//��࿪��ָ��
#define MC_MFD1_LeftCloseDoor				4			//������ָ��
#define MC_MFD1_RightOpenDoor				6			//�Ҳ࿪��ָ��
#define MC_MFD1_RightCloseDoor			8			//�Ҳ����ָ��
#define MC_MFD1_LDoorOCACmd					10		//������ٿ���ָ��
#define MC_MFD1_RDoorOCACmd					12		//�Ҳ����ٿ���ָ��
                                  
#define MC_MFD1_BCUFAult				 		1			//�ƶ���Ԫ����
#define MC_MFD1_ParkingBrake		 		3			//ͣ���ƶ�
#define MC_MFD1_BCUIsolate		 			5			//�ƶ���������
#define MC_MFD1_EmergBrakeBypass		7			//�����ƶ���·
#define MC_MFD1_ACPStart						9			//��ѹ������
#define MC_MFD1_VVVFPowerVaild			11		//VVVF��Դ��Ч

/*24v*/
#define MC_MFD1_ATPEmergBrake				2			//ATP�����ƶ�
#define MC_MFD1_ATPServiceBrake			4			//ATP�����ƶ�

/*MC-MFD2�ӿ�*/
/*110V*/
#define	MC_MFD2_HeardRelay					2			//ͷ�̵���
#define MC_MFD2_TailRelay						4			//β�̵���
#define MC_MFD2_BrakeCmd						6			//�ƶ�ָ��
#define	MC_MFD2_EmergBrakeRelay			8			//�����ƶ��̵���
#define MC_MFD2_ATCPowering					10		//ATC����״̬
#define MC_MFD2_ATCBypass						12		//ATC�г�����

#define MC_MFD2_TractionCmd					1			//ǣ��ָ��

/*MC-MFD3�ӿ�*/
/*110V*/
#define MC_MFD3_DoorClosedAll				6			//�Źغü̵���
#define MC_MFD3_CabACCNormal				10		//˾���ҿյ���������
#define MC_MFD3_CabACCFault					12		//˾���ҿյ�����

#define MC_MFD3_EmergEvacuDoorSta		3			//������ɢ��״̬


/*T-MFD1�ӿ�*/
#define T_MFD1_BHBStatus					 	2			//BHB
#define T_MFD1_BLBStatus						4			//BLB
#define T_MFD1_SIVContactor					8			//SIV����Ӵ���
#define T_MFD1_SIVPower							10		//SIV��Դ����

#define T_MFD1_BCUFault							1			//�ƶ���Ԫ����
#define T_MFD1_ParkingBrake		 			3			//ͣ���ƶ�
#define T_MFD1_BCUIsolate		 				5			//�ƶ���������


/*M-MFD1�ӿ�*/
#define M_MFD1_BHBStatus					 	2			//BHB
#define M_MFD1_BLBStatus						4			//BLB
#define M_MFD1_BCUFault							6			//�ƶ���Ԫ����
#define M_MFD1_ParkingBrake		 			8			//ͣ���ƶ�
#define M_MFD1_BCUIsolate		 				10		//�ƶ���������

#define M_MFD1_VVVFPowerVaild				9			//VVVF��Դ��Ч

/*T1_MFD1�ӿ�*/
#define T1_MFD1_ExtendPowering			4			//��չ����
#define T1_MFD1_SIVContactor				10		//SIV����Ӵ���
#define T1_MFD1_SIVPower						12		//SIV��Դ����

#define T1_MFD1_BCUFault						1			//�ƶ���Ԫ����
#define T1_MFD1_ParkingBrake		 		3			//ͣ���ƶ�
#define T1_MFD1_BCUIsolate		 			5			//�ƶ���������


/*T2-MFD1�ӿ�*/
#define T2_MFD1_BCUFault				  	12		//�ƶ���Ԫ����
#define T2_MFD1_ParkingBrake		 		1			//ͣ���ƶ�
#define T2_MFD1_BCUIsolate		 			3			//�ƶ���������

/*����ȫ�����飬���ڱ�ʾDI�ɼ���Ӧ���±�����*/
extern u8 MCMFD1Di110VIndex[12];
extern u8 MCMFD1Di24VIndex[2];
extern u8 MCMFD2DiIndex[7];
extern u8 MCMFD3DiIndex[4];
extern u8 TDiIndex[7];
extern u8 MDiIndex[6];
extern u8 T1DiIndex[6];
extern u8 T2DiIndex[3];


/*
����˵����
���ܣ�Ҫ���ĳλ������ֵ
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
u8 GetDataBit(u8 * Data,u8 GetPosit);

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

/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
����ֵ���������ɵ�BCC��
*/
u8 GenerateBccChcek(void * pData,u8 len);

#endif //__COMMUNPRO_H__

