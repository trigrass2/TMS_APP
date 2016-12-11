#include "SIVComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "CRC_CCITT.h"

/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��SIVSdrText,���� SIVSdText,SIVTcrText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
	u8 mode:���͵�����ΪSDR������TDR	TCR; 
����ֵ���޷���ֵ
*/
void SendSIVSdrOrTdrOrTcr(void * pInData,u8 * pOutData,u8 mode)
{
	u16	CrcVale;
	
	switch(mode)
	{
		case	SIV_SEND_SDR:
		{
			SIVSdrFrame SdrDataFrame;	//�������
			
			SdrDataFrame.StartFlag = SIV_STARTFLAG;
			SdrDataFrame.Address1 = SIV_ADDRESS1;
			SdrDataFrame.Address2 = SIV_ADDRESS2;
			SdrDataFrame.Control = SIV_CONTROLBYTE;
			memcpy(&SdrDataFrame.SIVSdrData,pInData,sizeof(SIVSdrText));//�����ı�����
			SdrDataFrame.SIVSdrData.Command = SIV_SDR_COMMAND;
			
			CrcVale = crcCompute((u8*)pInData+1,sizeof(SIVSdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			SdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			SdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			SdrDataFrame.EndFlag = SIV_ENDFLAG;
			
			memcpy(pOutData,&SdrDataFrame,sizeof(SIVSdrFrame));
		}
		break;
		
		case	SIV_SEND_TDR:
		{
			SIVTdrFrame TdrDataFrame;	//�������
			
			TdrDataFrame.StartFlag = SIV_STARTFLAG;
			TdrDataFrame.Address1 = SIV_ADDRESS1;
			TdrDataFrame.Address2 = SIV_ADDRESS2;
			TdrDataFrame.Control = SIV_CONTROLBYTE;
			memcpy(&TdrDataFrame.SIVTdrData,pInData,sizeof(SIVTdrText));//�����ı�����
			TdrDataFrame.SIVTdrData.Command = SIV_TDR_COMMAND;
			
			CrcVale = crcCompute((u8*)pInData+1,sizeof(SIVSdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TdrDataFrame.EndFlag = SIV_ENDFLAG;
			
			memcpy(pOutData,&TdrDataFrame,sizeof(SIVSdrFrame));			
		}
		break;
		
		case	SIV_SEND_TCR:
		{
			SIVTcrFrame TcrDataFrame;	//�������
			
			TcrDataFrame.StartFlag = SIV_STARTFLAG;
			TcrDataFrame.Address1 = SIV_ADDRESS1;
			TcrDataFrame.Address2 = SIV_ADDRESS2;
			TcrDataFrame.Control = SIV_CONTROLBYTE;
			memcpy(&TcrDataFrame.SIVTcrData,pInData,sizeof(SIVTcrText));//�����ı�����
			TcrDataFrame.SIVTcrData.Command = SIV_TDR_COMMAND;
			
			CrcVale = crcCompute((u8*)pInData+1,sizeof(SIVTcrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TcrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TcrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TcrDataFrame.EndFlag = SIV_ENDFLAG;
			
			memcpy(pOutData,&TcrDataFrame,sizeof(SIVSdrFrame));				
		}
		break;
		
		default:
			break;
	}
}  


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�SIVSdText�ṹ��)��
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetSIVSdOrTd(u8 *pInData,void *pOutData)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 CommandVale=0;
	u16 SourCrcVale,ComputeCrcVale;
	
	returnvale =	CheckSIVGetError(pInData);
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("SIV Get SDorTD Error��\r\n");
		#endif	
		return SIV_GetError;
	}
	
	CommandVale = *(pInData+4);		//�õ������ֽ�  3+1 = 0x7E,��ַ2�ֽڣ�������1�ֽ�
	switch(CommandVale)
	{
		case	SIV_SD_COMMAND:
		{
			SIVSdFrame SdDataFrame;
			memcpy(&SdDataFrame,pInData,sizeof(SIVSdFrame));
			
			ComputeCrcVale = crcCompute(&SdDataFrame.Address1,sizeof(SIVSdText)+3);
			SourCrcVale = ((u16)SdDataFrame.CRCH8<<8 )| (SdDataFrame.CRCL8);
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData,sizeof(SIVSdFrame));//ȥ��ǰ���0x7E
			}
			else
			{
				#if DEBUG_RS485==1
				printf("SIV SdУ��ֵ������\r\n");
				#endif		
				return SIV_CRCError;
			}
		}
		break;
		case	SIV_TD_COMMAND:
		{
			SIVTdFrame TdDataFrame;
			memcpy(&TdDataFrame,pInData,sizeof(SIVTdFrame));
			
			ComputeCrcVale = crcCompute(&TdDataFrame.Address1,sizeof(SIVSdText)+3);
			SourCrcVale = ((u16)TdDataFrame.CRCH8<<8 )| (TdDataFrame.CRCL8);
					
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData,sizeof(SIVTdFrame));//ȥ��ǰ���0x7E
			}
			else
			{
				#if DEBUG_RS485==1
				printf("SIV SdУ��ֵ������\r\n");
				#endif		
				return SIV_CRCError;
			}
			
		}
		break;
		
		default:
			break;
	}
	
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
s8 CheckSIVGetError(u8 *pInData)
{
		//1.����SDR �󣬾���100ms ������û�������յ�SD ʱ,����
	{
		u8 i=0;
		for(i=0;i<8;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
		{
			delay_ms(10);//��ʱ10ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
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
				return SIV_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return SIV_FrameError;
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
