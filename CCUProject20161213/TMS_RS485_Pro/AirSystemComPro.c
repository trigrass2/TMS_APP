#include "AirSystemComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "CRC_CCITT.h"
#include "stdlib.h"
#include "math.h"

/*
	����˵����
���ܣ�����豸���͸��յ���״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendAirSystem_SDR(S_TMS_AirSystem_SDR_Text * pInData,u8 * pOutData)
{
	S_TMS_AirSystem_SDR_Frame DataFrame;
	uint16_t CrcValue;
	
	DataFrame.PAD1 = AIRSYSTEM_PAD1;
	DataFrame.PAD2 = AIRSYSTEM_PAD2;
	DataFrame.StartFlag = AIRSYSTEM_STARTFLAG;
	DataFrame.AddressL8 = AIRSYSTEM_ADDRESSL8;
	DataFrame.AddressH8 = AIRSYSTEM_ADDRESSH8;
	DataFrame.ControlByte = AIRSYSTEM_CONTROLBYTE;
	
	memcpy(&DataFrame.DataText,pInData,sizeof(S_TMS_AirSystem_SDR_Text));	 //�����͵��ı���Ϣȫ������������֡
	/*���꣬�£��գ�ʱ���֣��룬ת��ΪBCD�����ʽ*/
	DataFrame.DataText.Year = DecToBcd(pInData->Year);
	DataFrame.DataText.Month = DecToBcd(pInData->Month);
	DataFrame.DataText.Day = DecToBcd(pInData->Day);
	DataFrame.DataText.Hour = DecToBcd(pInData->Hour);
	DataFrame.DataText.Minute = DecToBcd(pInData->Minute);
	DataFrame.DataText.Second = DecToBcd(pInData->Second);
	
	CrcValue = crcCompute((u8 *)&DataFrame.AddressL8,sizeof(S_TMS_AirSystem_SDR_Text)+3);
	DataFrame.CRCL8 =	CrcValue&0xff;
	DataFrame.CRCH8	= (CrcValue>>8)&0xff;
	DataFrame.EndFlag = AIRSYSTEM_ENDFLAG;
	
	memcpy(pOutData,&DataFrame,sizeof(DataFrame));
}


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
s8 GetAirSystem_SD(u8 *pInData,void *pOutData,u8 GetMode)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 i=0 /*,j=0*/;						//i��¼�������ݵ�0x7E�ĸ���,  j����ѭ����ǰ����0x7E������������
	u8 CRCH8,CRCL8;
	u8 *pDireInData;			//����һ��ָ�룬ָ����������� pInData;
//	u8 *pDireOutData;			//����һ��ָ�룬ָ����������� pOutData;
	u8 s_AIRSYSTEMRxErrCount = 0;							 //��̬�ֲ���������⵽����5 �ν���ʧ��ʱ�����ж�Ϊ�����쳣	
	
	pDireInData = pInData;
//	pDireOutData = pOutData;
	
	returnvale =	CheckAirSystemGetError(pInData);
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		
		s_AIRSYSTEMRxErrCount++;
		if(s_AIRSYSTEMRxErrCount == 5)
		{
			s_AIRSYSTEMRxErrCount = 0;
			return AIRSYSTEM_TransException;
		}
		
		return AIRSYSTEM_GetError;
	}
			
	s_AIRSYSTEMRxErrCount = 0 ;	//���������ı�־����
	
	while(*pDireInData == AIRSYSTEM_PAD1)			//�����м����ֽ���0x7E
	{
		i++;
		pDireInData++;
	}
	
	if(GetMode == AIR_STATE_SD_MODE)
	{
		S_TMS_AirSystem_State_SD_Frame DataFrame;
		uint16_t CrcValue;
		
		memcpy(&DataFrame,pInData+i-1,sizeof(DataFrame));		//pInData+i-1 ����ʾ��startflag��ʼ��������		
																												//ע������õ��������գ�ʱ���룬����BCD�룬��û��ת��Ϊ10����
	
		CrcValue = crcCompute((u8 *)&(DataFrame.AddressL8),sizeof(S_TMS_AirSystem_State_SD_Text)+3);
		CRCH8 =	(CrcValue>>8) & 0xff;	//�õ����ݵ�CRCУ��ֵ
		CRCL8 =  CrcValue & 0xff;		//�õ����ݵ�CRCУ��ֵ
			
		if(CRCH8 != DataFrame.CRCH8 && CRCL8 != DataFrame.CRCL8)
		{
			#if DEBUG_RS485==1
				printf("BCCУ��ֵ������\r\n");
			#endif		
			return AIRSYSTEM_CRCError;
		}
		
		/*�����ݿ�����pOutData��ָ��Ŀռ�*/
//		for(j=0;j<i-1;j++)			//����ǰ�����ֽڵ�0x7E
//		{	
//			*pDireOutData = AIRSYSTEM_PAD1;
//			pDireOutData++;
//		}
		memcpy((u8*)pOutData/*+i-1*/,&DataFrame,sizeof(DataFrame));
		
	}
	
	else if(GetMode == AIR_HISFAULT_SD_MODE )
	{
		S_TMS_AirSystem_HisFault_SD_Frame DataFrame;
		uint16_t CrcValue;
		
		memcpy(&DataFrame,pInData+i-1,sizeof(DataFrame));		//pInData+i-1 ����ʾ��startflag��ʼ��������
																												//ע������õ��������գ�ʱ���룬����BCD�룬��û��ת��Ϊ10����
		
		CrcValue = crcCompute((u8 *)&(DataFrame.AddressL8),sizeof(S_TMS_AirSystem_HisFault_SD_Text)+3);
		CRCH8 = (CrcValue>>8) & 0xff;	//�õ����ݵ�CRCУ��ֵ
		CRCL8 =  CrcValue & 0xff;		//�õ����ݵ�CRCУ��ֵ
			
		if(CRCH8 != DataFrame.CRCH8 && CRCL8 != DataFrame.CRCL8)
		{
			#if DEBUG_RS485==1
				printf("BCCУ��ֵ������\r\n");
			#endif		
			return AIRSYSTEM_CRCError;
		}
		
		/*�����ݿ�����pOutData��ָ��Ŀռ�*/
//		for(j=0;j<i-1;j++)			//����ǰ�����ֽڵ�0x7E
//		{	
//			*pDireOutData = AIRSYSTEM_PAD1;
//			pDireOutData++;
//		}
		memcpy((u8*)pOutData/*+i-1*/,&DataFrame,sizeof(DataFrame));
		
	}
		
	//û�д��󷵻�0
	return 0;
}

extern 	u8 g_RxDataFlag;
extern	u8 g_FrameErrorFlag;

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckAirSystemGetError(u8 *pInData)
{
		//1.����SDR �󣬾���20ms ������û�������յ�SD ʱ,����
	{
		u8 i=0;
		for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
		{
			delay_ms(5);//��ʱ5ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
			if(g_RxDataFlag == 1)
			{
				break;
			}				
		}
		if(i>=10)
		{
			if(g_RxDataFlag == 0)	//˵��û�н��ܵ�����
			{
				#if DEBUG_RS485==1
					printf("50ms��û�н��ܵ�����\r\n");
				#endif
				return AIRSYSTEM_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return AIRSYSTEM_FrameError;
	}
	
//	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
//	if(pInData[0]!=AIR_STX || pInData[1]!=AIR_ADDRES || pInData[2]!=AIR_SUB_ADDRES ||pInData[3]!=AIR_COMMAND)
//	{
//		#if DEBUG_RS485==1
//			printf("���ܵĿ�ʼ������ַ��������һ��������\r\n");
//		#endif		
//			return AIRSYSTEM_DataError;
//	}

	//û�д������
	return 0;
}


///*
//	����˵����
//���ܣ���10������ת��ΪBCD��
//����: 
//			���룺u8 Dec   ��ת����ʮ�������� ���������ֻ����0~99
//����ֵ��  ת�����BCD��
//˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
//*/
//u8 DecToBcd(u8 Dec)
//{
//	if(Dec>=99)
//	{
//		Dec %=100;
//	}
//	return ((Dec/10)<<4) | ((Dec%10) & 0x0F);
//}

///*
//	����˵����
//���ܣ���BCD��ת��Ϊ10������
//����: 
//			���룺u8 Bcd   ��ת��BCD��		ֻ��һ���ֽ�
//����ֵ��  ת�����10������
//˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
//*/
//u8 BcdToDec(u8 Bcd)
//{
//	return (Bcd>>4)*10 + (Bcd & 0x0f);
//}


void test_airsystem(void)
{
	{
		/*������ ���Ƹ� λ����*/
		u8 Buffer[30]={0xCE};
//		u8 datbuff[30];
//		u8 i;
		S_TMS_AirSystem_SDR_Text data;
		memset(&Buffer[1],0x88,sizeof(Buffer)-1);
		memcpy(&data,Buffer,sizeof(Buffer));
		
		printf("\r\n");
		printf("bit7=%d\r\n",data.HistoryFaultQuery);
		printf("bit6=%d\r\n",data.OrderStartSignal);
		printf("bit5=%d\r\n",data.EnvTemperEffective);
		printf("bit4=%d\r\n",data.TarTemperEffective);
		printf("bit3=%d\r\n",data.TimeEffective);
		printf("bit2~0=%d\r\n",data.TrainNumber);
		
		printf("HistoryFaultNumber=0x%.2X\r\n",data.HistoryFaultNumber);
		printf("Year=0x%.2X\r\n",data.Year);
		printf("\r\n");

	}
	
	{
		/*�������ݵĿ���*/
		/*��λ��������ݸ��Ƹ�����*/
		S_TMS_AirSystem_SDR_Frame SDRData;
		S_TMS_AirSystem_State_SD_Frame sdData;
		u8 datbuff[30];
		u16 i=0;
		
		SDRData.PAD1=AIRSYSTEM_PAD1;
		SDRData.PAD2=AIRSYSTEM_PAD2;
		SDRData.StartFlag= AIRSYSTEM_STARTFLAG;
		SDRData.AddressL8=AIRSYSTEM_ADDRESSL8;
		SDRData.AddressH8=AIRSYSTEM_ADDRESSH8;
		SDRData.ControlByte=AIRSYSTEM_CONTROLBYTE;
		SDRData.DataText.TrainNumber=6;
		SDRData.DataText.TimeEffective=1;
		SDRData.DataText.TarTemperEffective=0;
		SDRData.DataText.EnvTemperEffective=0;
		SDRData.DataText.OrderStartSignal=1;
		SDRData.DataText.HistoryFaultQuery=1;
		
		SDRData.DataText.HistoryFaultNumber=1;
		SDRData.DataText.Year=16;
		SDRData.DataText.Month=6;
		SDRData.DataText.Day=30;
		SDRData.DataText.Hour=12;
		SDRData.DataText.Minute=12;
		SDRData.DataText.Second=60;
		SDRData.DataText.RefriTargetTemperL8=8;
		SDRData.DataText.RefriTargetTemperH8=9;
		SDRData.DataText.EnvironmentTemperL8=10;
		SDRData.DataText.EnvironmentTemperH8=11;
		SDRData.DataText.WorkMode=12;
		SDRData.DataText.Dummy1=13;
		SDRData.DataText.Dummy2=14;
		SDRData.CRCL8=0xAA;
		SDRData.CRCH8=0xbb;
		SDRData.EndFlag=AIRSYSTEM_ENDFLAG;
		
		memcpy(datbuff,&SDRData,sizeof(SDRData));
		printf("\r\n");
		for(i=0;i<sizeof(SDRData);i++)
		{
			printf("data%d=0x%x\r\n",i,datbuff[i]);
		}
		printf("\r\n");

		/*crcУ��*/
		i=0;
		i=crcCompute((u8 *)&SDRData.DataText,sizeof(S_TMS_AirSystem_SDR_Text));
		printf("crc=0x%x\r\n",i);
		
		/**/
		SendAirSystem_SDR(&SDRData.DataText,datbuff);
		printf("\r\n");
		for(i=0;i<sizeof(SDRData);i++)
		{
			printf("send%d=0x%x\r\n",i,datbuff[i]);
		}
		printf("\r\n");
		
		GetAirSystem_SD(datbuff,&sdData,AIR_STATE_SD_MODE);
		
		printf("1=0x%x\r\n",sdData.StartFlag);
		printf("adH8=0x%x\r\n",sdData.AddressH8);
	}
	
	{
		u8 a=0;
		a=DecToBcd(16);
		printf("a1=%x\r\n",a);		//16����
		
		a=BcdToDec(0x16);
		printf("a2=%d\r\n",a);		//10����
	}
	
}


