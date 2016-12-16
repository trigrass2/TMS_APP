#include "VVVFComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "CRC_CCITT.h"
#include "SAF82525.h"
#include "bsp.h"

/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��VVVFSdrText,���� VVVFSdText,VVVFTcrText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
	u8 mode:���͵�����ΪSDR������TDR	TCR; 
����ֵ���޷���ֵ
*/
void SendVVVFSdrOrTdrOrTcr(void * pInData,/*u8 * pOutData,*/u8 mode)
{	
	u16	CrcVale;
	u8 PADFlag = VVVF_PAD;
	
	switch(mode)
	{
		case	VVVF_SEND_SDR:
		{
			VVVFSdrFrame SdrDataFrame;	//�������
			
			memset(SdrDataFrame.StartPAD,PADFlag,sizeof(SdrDataFrame.StartPAD));	//д��PAD
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
			
			CrcVale=crcCompute((u8*)&SdrDataFrame+5,sizeof(VVVFSdrText)+3); //����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			SdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			SdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			SdrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy���ݵ����������*/
			//memcpy(pOutData,&SdrDataFrame,sizeof(VVVFSdFrame));
			if(SdrDataFrame.VVVFSdrData.Receiver == VVVF_SDR_RECEIVER1)		//VVVF1����
			{
				printf("VVVF1 Send!\r\n");
				AChanleTxData((u8 *)&SdrDataFrame,sizeof(VVVFSdrFrame));
			}
			else		//VVVF2����
			{
				printf("VVF2 Send!\r\n");
				BChanleTxData((u8 *)&SdrDataFrame,sizeof(VVVFSdrFrame));
			}
			
		}
		break;
		
		case VVVF_SEND_TDR:
		{
			VVVFTdrFrame TdrDataFrame;
			
			memset(TdrDataFrame.StartPAD,PADFlag,sizeof(TdrDataFrame.StartPAD));
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
			
			CrcVale = crcCompute((u8*)&TdrDataFrame+5,sizeof(VVVFTdrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TdrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy���ݵ����������*/
			//memcpy(pOutData,&TdrDataFrame,sizeof(VVVFTdrFrame));
			if(TdrDataFrame.VVVFTdrData.Receiver == VVVF_SDR_RECEIVER1)		//VVVF1����
			{
				AChanleTxData((u8 *)&TdrDataFrame,sizeof(VVVFTdrFrame));
			}
			else		//VVVF2����
			{
				BChanleTxData((u8 *)&TdrDataFrame,sizeof(VVVFTdrFrame));
			}
		}
		break;
		
		case VVVF_SEND_TCR:
		{
			VVVFTcrFrame TcrDataFrame;
			
			memset(TcrDataFrame.StartPAD,PADFlag,sizeof(TcrDataFrame.StartPAD));
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
			
			CrcVale = crcCompute((u8*)&TcrDataFrame+5,sizeof(VVVFTcrText)+3);//����CRCУ��ֵ,У����ǵ�ַλ������λ���ı�����֡������
			TcrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TcrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TcrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy���ݵ����������*/
			//memcpy(pOutData,&TcrDataFrame,sizeof(VVVFTcrFrame));
			if(TcrDataFrame.VVVFTcrData.Receiver == VVVF_SDR_RECEIVER1)		//VVVF1����
			{
				AChanleTxData((u8 *)&TcrDataFrame,sizeof(VVVFTcrFrame));
			}
			else		//VVVF2����
			{
				BChanleTxData((u8 *)&TcrDataFrame,sizeof(VVVFTcrFrame));
			}			
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
	VVVF_No: ��ʾ��ȡVVVF1������VVVF2
//	u8 mode :���ܵ�����ΪSD������ TD
����ֵ��0,�޴�  �����д�
*/
s8 GetVVVFSdOrTd(u8 *pInData,void *pOutData,u8 VVVF_No)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 StartVale=0,ComandVale=0;
	u16 SourCrcVale,ComputeCrcVale;
	u8 i=0;		//��¼��ʼ�����ж��ٸ�0x7E
	
	returnvale =	CheckVVVFGetError(pInData,VVVF_No);
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==0
		switch(VVVF_No)
		{
			case  VVVF1:
				printf("VVVF11 Get SDorTD Error��\r\n");
				break;
			case VVVF2:
				printf("VVVF22Get SDorTD Error��\r\n");
				break;
		}
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
		//printf("0x7e-%d\r\n",i);
	ComandVale = *(pInData+i+3+1);		//�õ������ֽ�  i+3+1 = 0x7E����+��ַ2�ֽڣ�������1�ֽ�+1�ֽ��ı��� 
	
	switch(ComandVale)
	{
		case VVVF_SD_COMMAND:
		{
			ComputeCrcVale = crcCompute(pInData+i,sizeof(VVVFSdText)+3);
			SourCrcVale = (u16)(*(pInData+i+3+sizeof(VVVFSdText)) <<8) | *(pInData+i+3+sizeof(VVVFSdText)+1);
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData+i+3,sizeof(VVVFSdText));		//ȥ��ǰ���0x7E	�͵�ַ �����ֽڣ�ֻҪ�ı�����
			}
			else
			{
				#if DEBUG_RS485==0
				printf("VVVF SdУ��ֵ������\r\n");
				#endif		
				return VVVF_CRCError;
			}
		}
		break;		//sd
		
		case VVVF_TD_COMMAND:
		{
			ComputeCrcVale = crcCompute(pInData+i,sizeof(VVVFTdText)+3);
			SourCrcVale = (u16)(*(pInData+i+3+sizeof(VVVFTdText)) <<8) | *(pInData+i+3+sizeof(VVVFTdText)+1) ;
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData+i+3,sizeof(VVVFTdText));//ȥ��ǰ���0x7E	�͵�ַ �����ֽڣ�ֻҪ�ı�����
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


//extern 	u8 g_RxDataFlag;
//extern	u8 g_FrameErrorFlag;

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
	VVVF_No: ��ʾ��ȡVVVF1������VVVF2
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckVVVFGetError(u8 *pInData,u8 VVVF_No)
{
	//1.����SDR �󣬾���80ms ������û�������յ�SD ʱ,����
	u8 i=0;
	
	switch(VVVF_No)
	{
		case VVVF1:
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
						printf("VVVF1 80ms��û�н��ܵ�����\r\n");
					#endif
					return VVVF_GetTimeoutError;
				}
			}
		break;
			
		case VVVF2:
			for(i=0;i<8;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
			{
				if(g_HDLCBRxEndFlag == 1)
				{
					g_HDLCBRxEndFlag = 0; //���������־
					break;
				}		
				
				else
					BSP_OS_TimeDlyMs(10);//��ʱ10ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 		
			}
			if(i>=8)
			{
				if(g_HDLCBRxEndFlag == 0)	//˵��û�н��ܵ�����
				{
					#if DEBUG_RS485==0
						printf("VVVF2 80ms��û�н��ܵ�����\r\n");
					#endif
					return VVVF_GetTimeoutError;
				}
			}			
		break;
	}

	return 0;
}

