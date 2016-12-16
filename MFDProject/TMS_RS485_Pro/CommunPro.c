#include "CommunPro.h"
#include "string.h"
#include "malloc.h"


/*����ȫ�����飬���ڱ�ʾDI�ɼ���Ӧ���±�����*/

u8 MCMFD1Di110VIndex[12]={MC_MFD1_LeftOpenDoor,MC_MFD1_LeftCloseDoor,MC_MFD1_RightOpenDoor,MC_MFD1_RightCloseDoor,
											MC_MFD1_LDoorOCACmd,MC_MFD1_RDoorOCACmd,MC_MFD1_BCUFAult,MC_MFD1_ParkingBrake,MC_MFD1_BCUIsolate,
											MC_MFD1_EmergBrakeBypass,MC_MFD1_ACPStart,MC_MFD1_VVVFPowerVaild,};

u8 MCMFD1Di24VIndex[2]= {MC_MFD1_ATPEmergBrake,MC_MFD1_ATPServiceBrake};

u8 MCMFD2DiIndex[7] = {MC_MFD2_HeardRelay,MC_MFD2_TailRelay,MC_MFD2_BrakeCmd,MC_MFD2_EmergBrakeRelay,
										MC_MFD2_ATCPowering,MC_MFD2_ATCBypass,MC_MFD2_TractionCmd};

u8 MCMFD3DiIndex[4]={MC_MFD3_DoorClosedAll,MC_MFD3_CabACCNormal,MC_MFD3_CabACCFault,MC_MFD3_EmergEvacuDoorSta};

u8 TDiIndex[7]={T_MFD1_BHBStatus,T_MFD1_BLBStatus,T_MFD1_SIVContactor,T_MFD1_SIVPower,T_MFD1_BCUFault,  
								T_MFD1_ParkingBrake,T_MFD1_BCUIsolate };

u8 MDiIndex[6]={M_MFD1_BHBStatus,M_MFD1_BLBStatus,M_MFD1_BCUFault,M_MFD1_ParkingBrake,M_MFD1_BCUIsolate, 
								M_MFD1_VVVFPowerVaild};				

u8 T1DiIndex[6]={T1_MFD1_ExtendPowering,T1_MFD1_SIVContactor,T1_MFD1_SIVPower,T1_MFD1_BCUFault,
								T1_MFD1_ParkingBrake,T1_MFD1_BCUIsolate};

u8 T2DiIndex[3] = {T2_MFD1_BCUFault,T2_MFD1_ParkingBrake,T2_MFD1_BCUIsolate};								
/*
����˵����
���ܣ�Ҫ���ĳλ������
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
u8 GetDataBit(u8 * Data,u8 GetPosit)
{
	u8 vale;
	vale=*Data <<(7-GetPosit);
	return vale>>7;
}



/*
	����˵����
���ܣ���10������ת��ΪBCD��
����: 
			���룺u8 Dec   ��ת����ʮ�������� ���������ֻ����0~99
����ֵ��  ת�����BCD��
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 DecToBcd(u8 Dec)
{
	if(Dec>=99)
	{
		Dec %=100;
	}
	return ((Dec/10)<<4) | ((Dec%10) & 0x0F);
}

/*
	����˵����
���ܣ���BCD��ת��Ϊ10������
����: 
			���룺u8 Bcd   ��ת��BCD��		ֻ��һ���ֽ�
����ֵ��  ת�����10������
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 BcdToDec(u8 Bcd)
{
	return (Bcd>>4)*10 + (Bcd & 0x0f);
}


/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
����ֵ���������ɵ�BCC��
*/
u8 GenerateBccChcek(void * pData,u8 len)
{
	u8 i=0;
	u8 BccCheckCode=0xFF;
	u8 *BccData;
	BccData = mymalloc(SRAMIN,len);		//��̬�����ڴ� ,��keil�ﲻ��ʹ��BccData[len],����������ʾ���鳤��δ����
	memcpy(BccData,pData,len);
	for(i=0;i<len;i++)
	{
		BccCheckCode=BccCheckCode^BccData[i];
	};
	myfree(SRAMIN,BccData);			//�ͷ��ڴ�
	
	return BccCheckCode;
}













