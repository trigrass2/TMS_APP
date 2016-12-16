#include "SIVComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "CRC_CCITT.h"
#include "main.h"
#include "SAF82525.h"
#include "bsp.h"

/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��SIVSdrText,���� SIVSdText,SIVTcrText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
	u8 mode:���͵�����ΪSDR������TDR	TCR; 
����ֵ���޷���ֵ
*/
void SendSIVSdrOrTdrOrTcr(void * pInData,/*u8 * pOutData,*/u8 mode)
{
	u16	CrcVale;
	u8 PADFlag = SIV_PAD;
	
	switch(mode)
	{
		case	SIV_SEND_SDR:
		{
			SIVSdrFrame SdrDataFrame;	//�������
			
			memset(SdrDataFrame.StartPAD,PADFlag,sizeof(SdrDataFrame.StartPAD));	//д��PAD
			
			SdrDataFrame.StartFlag = SIV_STARTFLAG;
			SdrDataFrame.Address1 = SIV_ADDRESS1;
			SdrDataFrame.Address2 = SIV_ADDRESS2;
			SdrDataFrame.Control = SIV_CONTROLBYTE;
			memcpy(&SdrDataFrame.SIVSdrData,pInData,sizeof(SIVSdrText));//�����ı�����
			
			CrcVale = crcCompute((u8*)&SdrDataFrame+5,sizeof(SIVSdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			SdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			SdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			SdrDataFrame.EndFlag = SIV_ENDFLAG;
			
			//memcpy(pOutData,&SdrDataFrame,sizeof(SIVSdrFrame));
			AChanleTxData((u8 *)&SdrDataFrame,sizeof(SIVSdrFrame));
		}
		break;
		
		case	SIV_SEND_TDR:
		{
			SIVTdrFrame TdrDataFrame;	//�������
			
			memset(TdrDataFrame.StartPAD,PADFlag,sizeof(TdrDataFrame.StartPAD));	//д��PAD
			
			TdrDataFrame.StartFlag = SIV_STARTFLAG;
			TdrDataFrame.Address1 = SIV_ADDRESS1;
			TdrDataFrame.Address2 = SIV_ADDRESS2;
			TdrDataFrame.Control = SIV_CONTROLBYTE;
			memcpy(&TdrDataFrame.SIVTdrData,pInData,sizeof(SIVTdrText));//�����ı�����
			
			CrcVale = crcCompute((u8*)&TdrDataFrame+5,sizeof(SIVTdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TdrDataFrame.EndFlag = SIV_ENDFLAG;
			
			//memcpy(pOutData,&TdrDataFrame,sizeof(SIVSdrFrame));		
			AChanleTxData((u8 *)&TdrDataFrame,sizeof(SIVTdrFrame));
		}
		break;
		
		case	SIV_SEND_TCR:
		{
			SIVTcrFrame TcrDataFrame;	//�������
			
			memset(TcrDataFrame.StartPAD,PADFlag,sizeof(TcrDataFrame.StartPAD));	//д��PAD
			
			TcrDataFrame.StartFlag = SIV_STARTFLAG;
			TcrDataFrame.Address1 = SIV_ADDRESS1;
			TcrDataFrame.Address2 = SIV_ADDRESS2;
			TcrDataFrame.Control = SIV_CONTROLBYTE;
			memcpy(&TcrDataFrame.SIVTcrData,pInData,sizeof(SIVTcrText));//�����ı�����
			
			CrcVale = crcCompute((u8*)&TcrDataFrame+5,sizeof(SIVTcrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TcrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TcrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TcrDataFrame.EndFlag = SIV_ENDFLAG;
			
			//memcpy(pOutData,&TcrDataFrame,sizeof(SIVSdrFrame));			
			AChanleTxData((u8 *)&TcrDataFrame,sizeof(SIVTcrFrame));			
		}
		break;
		
		default:
			#if	 DEBUG_RS485==1
				printf("SIV Frame Format Error��\r\n");
			#endif			
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
	u8 StartVale=0,CommandVale=0;
	u16 SourCrcVale,ComputeCrcVale;
	u8 i=0;
	
	returnvale =	CheckSIVGetError(pInData);
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==0
			printf("SIV Get SDorTD Error��\r\n");
		#endif	
		//return SIV_GetError;
		return returnvale;
	}

	/*����CRC����*/
	i=0;		//������ж��ٸ�0x7E
	while(1)
	{
		StartVale = *(pInData+i);
		if(StartVale == SIV_PAD)
		{
			i++;
		}
		else
		{
			break; //����whileѭ��
		}
	}
	
	CommandVale = *(pInData+i+4);		//�õ������ֽ�  i+3+1 = 0x7E+��ַ2�ֽ�+������1�ֽ�+1�ֽ��ı��� 
	
	switch(CommandVale)
	{
		case	SIV_SD_COMMAND:
		{
			SIVSdFrame SdDataFrame;
			memcpy(&SdDataFrame,pInData,sizeof(SIVSdFrame));
			
			ComputeCrcVale = crcCompute((u8 *)&SdDataFrame.Address1,sizeof(SIVSdText)+3);
			SourCrcVale = ((u16)SdDataFrame.CRCH8<<8 )| (SdDataFrame.CRCL8);
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,&SdDataFrame.SIVSdData,sizeof(SIVSdText));	//ȥ��ǰ���0x7E	�͵�ַ �����ֽڣ�ֻҪ�ı�����
			}
			else
			{
				#if DEBUG_RS485==0
				printf("SIV SdCRCУ��ֵ������\r\n");
				#endif		
				return SIV_CRCError;
			}
		}
		break;
		
		case	SIV_TD_COMMAND:
		{
			SIVTdFrame TdDataFrame;
			memcpy(&TdDataFrame,pInData,sizeof(SIVTdFrame));
			
			ComputeCrcVale = crcCompute((u8 *)&TdDataFrame.Address1,sizeof(SIVSdText)+3);
			SourCrcVale = ((u16)TdDataFrame.CRCH8<<8 )| (TdDataFrame.CRCL8);
					
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,&TdDataFrame.SIVTdData,sizeof(SIVTdText));	//ȥ��ǰ���0x7E	�͵�ַ �����ֽڣ�ֻҪ�ı�����
			}
			else
			{
				#if DEBUG_RS485==1
				printf("SIV TdCRCУ��ֵ������\r\n");
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


//extern 	u8 g_RxDataFlag;
//extern	u8 g_FrameErrorFlag;

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckSIVGetError(u8 *pInData)
{
		//1.����SDR �󣬾���100ms ������û�������յ�SD ʱ,����
	u8 i=0;

	for(i=0;i<8;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_HDLCARxEndFlag == 1)
		{
			g_HDLCARxEndFlag = 0; //���������־
			break;
		}		
		
		else
			BSP_OS_TimeDlyMs(10);//��ʱ10ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 		
	}
	if(i>=8)
	{
		if(g_HDLCARxEndFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==0
				printf("SIV 80ms��û�н��ܵ�����\r\n");
			#endif
			return SIV_GetTimeoutError;
		}
	}
	//û�д������
	return 0;
}
