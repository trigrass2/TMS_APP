#include "VVVFComPro.h"
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
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��VVVFSdrText,���� VVVFSdText,VVVFTcrText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
	u8 mode:���͵�����ΪSDR������TDR	TCR; 
����ֵ���޷���ֵ
*/
void SendVVVFSdrOrTdrOrTcr(void * pInData,u8 * pOutData,u8 mode)
{	
	u16	CrcVale;
	u8 PADFlag = VVVF_PAD;
	
	switch(mode)
	{
		case	VVVF_SEND_SDR:
		{
			VVVFSdrFrame SdrDataFrame;	//�������
			
			memcpy(SdrDataFrame.StartPAD,&PADFlag,sizeof(SdrDataFrame.StartPAD));	//д��PAD
			SdrDataFrame.StartFlag = VVVF_STARTFLAG;
			SdrDataFrame.Address1 = VVVF_ADDRESS1;
			SdrDataFrame.Address2 = VVVF_ADDRESS2;
			SdrDataFrame.Control = VVVF_CONTROLBYTE;
			memcpy(&SdrDataFrame.VVVFSdrData,pInData,sizeof(VVVFSdrText));		//�����ı�����
			SdrDataFrame.VVVFSdrData.TextNumber = VVVF_SDR_TEXTNUM;
			SdrDataFrame.VVVFSdrData.Command	= VVVF_SDR_COMMAND;
			SdrDataFrame.VVVFSdrData.State = VVVF_SDR_STATE;
			SdrDataFrame.VVVFSdrData.Sender = VVVF_SDR_SENDER;
			SdrDataFrame.VVVFSdrData.OrderNo = VVVF_SDR_NO;
			
			CrcVale=crcCompute((u8*)pInData+5,sizeof(VVVFSdrText)+3); //����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			SdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			SdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			SdrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy���ݵ����������*/
			memcpy(pOutData,&SdrDataFrame,sizeof(VVVFSdFrame));
		}
		break;
		
		case VVVF_SEND_TDR:
		{
			VVVFTdrFrame TdrDataFrame;
			
			memcpy(TdrDataFrame.StartPAD,&PADFlag,sizeof(TdrDataFrame.StartPAD));
			TdrDataFrame.StartFlag = VVVF_STARTFLAG;
			TdrDataFrame.Address1 = VVVF_ADDRESS1;
			TdrDataFrame.Address2 = VVVF_ADDRESS2;
			TdrDataFrame.Control = VVVF_CONTROLBYTE;
			memcpy(&TdrDataFrame.VVVFTdrData,pInData,sizeof(VVVFTdrText));//�����ı�����
			TdrDataFrame.VVVFTdrData.TextNumber = VVVF_TDR_TEXTNUM;
			TdrDataFrame.VVVFTdrData.Command = VVVF_TDR_COMMAND;
			TdrDataFrame.VVVFTdrData.State = VVVF_TDR_STATE;
			TdrDataFrame.VVVFTdrData.Sender = VVVF_TDR_SENDER;
			TdrDataFrame.VVVFTdrData.OrderNo = VVVF_TDR_NO;
			
			CrcVale = crcCompute((u8*)pInData+5,sizeof(VVVFTdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TdrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy���ݵ����������*/
			memcpy(pOutData,&TdrDataFrame,sizeof(VVVFTdrFrame));
		}
		break;
		
		case VVVF_SEND_TCR:
		{
			VVVFTcrFrame TcrDataFrame;
			
			memcpy(TcrDataFrame.StartPAD,&PADFlag,sizeof(TcrDataFrame.StartPAD));
			TcrDataFrame.StartFlag = VVVF_STARTFLAG;
			TcrDataFrame.Address1 = VVVF_ADDRESS1;
			TcrDataFrame.Address2 = VVVF_ADDRESS2;
			TcrDataFrame.Control = VVVF_CONTROLBYTE;
			memcpy(&TcrDataFrame.VVVFTcrData,pInData,sizeof(VVVFTdrText));//�����ı�����
			TcrDataFrame.VVVFTcrData.TextNumber = VVVF_TCR_TEXTNUM;
			TcrDataFrame.VVVFTcrData.Command = VVVF_TCR_COMMAND;
			TcrDataFrame.VVVFTcrData.State = VVVF_TCR_STATE;
			TcrDataFrame.VVVFTcrData.Sender = VVVF_TCR_SENDER;
			TcrDataFrame.VVVFTcrData.OrderNo = VVVF_TCR_NO;
			
			CrcVale = crcCompute((u8*)pInData+5,sizeof(VVVFTdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TcrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TcrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TcrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy���ݵ����������*/
			memcpy(pOutData,&TcrDataFrame,sizeof(VVVFTcrFrame));
		}
		break;
		
		default: 
			#if	 DEBUG_RS485==1
				printf("VVVF Frame Format Error��\r\n");
			#endif
			break;
	}
}  


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_TMS_BRAKE_SD_Text�ṹ��)�� �������� ȥ��ǰ���0x7E
//	u8 mode :���ܵ�����ΪSD������ TD
����ֵ��0,�޴�  �����д�
*/
s8 GetVVVFSdOrTd(u8 *pInData,void *pOutData)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 StartVale=0,ComandVale=0;
	u16 SourCrcVale,ComputeCrcVale;
	u8 i=0;		//��¼��ʼ�����ж��ٸ�0x7E
	
	returnvale =	CheckVVVFGetError(pInData);
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("VVVF Get SDorTD Error��\r\n");
		#endif	
		return VVVF_GetError;
	}
	
	/*����CRC����*/
	i=0;		//������ж��ٸ�0x7E
	while(1)
	{
		StartVale = *(pInData+i);
		if(StartVale == VVVF_PAD)
		{
			i++;
		}
		else
		{
			break; //����whileѭ��
		}
	}

	ComandVale = *(pInData+i+3+1);		//�õ������ֽ�  i+3+1 = 0x7E����+��ַ2�ֽڣ�������1�ֽ�+1�ֽ��ı��� 
	switch(ComandVale)
	{
		case VVVF_SD_COMMAND:
		{
			ComputeCrcVale = crcCompute(pInData+i,sizeof(VVVFSdText)+3);
			SourCrcVale = (*(pInData+i+3+sizeof(VVVFSdText)) <<8) | *(pInData+i+3+sizeof(VVVFSdText));
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData+i,sizeof(VVVFSdFrame)-5);//ȥ��ǰ���0x7E
			}
			else
			{
				#if DEBUG_RS485==1
				printf("VVVF SdУ��ֵ������\r\n");
				#endif		
				return VVVF_CRCError;
			}
		}
		break;		//sd
		
		case VVVF_TD_COMMAND:
		{
			ComputeCrcVale = crcCompute(pInData+i,sizeof(VVVFTdText)+3);
			SourCrcVale = (*(pInData+i+3+sizeof(VVVFTdText)) <<8) | *(pInData+i+3+sizeof(VVVFTdText)) ;
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData+i,sizeof(VVVFTdFrame)-5);//ȥ��ǰ���0x7E	
			}
			else
			{
				#if DEBUG_RS485==1
				printf("VVVF TdУ��ֵ������\r\n");
				#endif		
				return VVVF_CRCError;				
			}			
		}
		break;	//td
		
		default:
			break;	//����
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
s8 CheckVVVFGetError(u8 *pInData)
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
				return VVVF_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return VVVF_FrameError;
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

