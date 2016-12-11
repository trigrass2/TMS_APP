#include "BCUComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
//#include "MTD_MFD_CurrentLoop.h"

/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��BcuSdrText,���� BcuSdText)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
//	u8 mode:���͵�����ΪSDR������TDR	; 
����ֵ���޷���ֵ
*/
void SendBcuSdrOrTdr(void * pInData/*,u8 * pOutData */,u8 mode)
{
	u8 BCC1 ,BCC2;		//���BCCУ��λ
	//	u8 CmdByte,mode;
		
	//	CmdByte=((u8 *)pInData)[0];
	//	if( CmdByte == BCU_SDR_COMMAND)
	//		mode = SEND_SDR;
	//	else if( CmdByte == BCU_TDR_COMMAND)
	//		mode = SEND_TDR;
	
	/*��������*/
	switch(mode)
	{
		case SEND_SDR:
			{
				BcuSdrFrame SdrDataFrame;		//��������		
				
				SdrDataFrame.SdrStx = BCU_STX;
				memcpy(&SdrDataFrame.BcuSdrData,pInData,sizeof(BcuSdrText));	 //�����͵��ı���Ϣȫ������������֡
				SdrDataFrame.SdrEtx = BCU_ETX;
			
				GenerateBcuBccChcek(pInData,sizeof(BcuSdrText),&BCC1,&BCC2);	//����BCC
				
				SdrDataFrame.SdrBCC1 = BCC1;
				SdrDataFrame.SdrBCC2 = BCC2;
				//memcpy(pOutData,&SdrDataFrame,sizeof(BcuSdrFrame));
				//MTDMFD_CL_Send_Data((u8 *)&SdrDataFrame,sizeof(BcuSdrFrame));	//ֱ�ӵ��õ��������ͺ���
			}	
			break;
		
		case SEND_TDR:
			{
				BcuTdrFrame TdrDataFrame;		//��������	
				
				TdrDataFrame.TdrStx = BCU_STX;
				memcpy(&TdrDataFrame.BcuTdrData,pInData,sizeof(BcuTdrText));	 //�����͵��ı���Ϣȫ������������֡
				TdrDataFrame.TdrEtx = BCU_ETX;
				
				GenerateBcuBccChcek(pInData,sizeof(BcuTdrText),&BCC1,&BCC2);		//����BCC
				
				TdrDataFrame.TdrBCC1 = BCC1;
				TdrDataFrame.TdrBCC2 = BCC2;
				//memcpy(pOutData,&TdrDataFrame,sizeof(BcuTdrFrame));
				//MTDMFD_CL_Send_Data((u8 *)&TdrDataFrame,sizeof(BcuTdrFrame));	//ֱ�ӵ��õ��������ͺ���
			}
			break;
		
		default :
			#if	 DEBUG_RS485==1
				printf("BCU Frame Format Error��\r\n");
			#endif
			break;
	}
}


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_TMS_BCU_SD_Text�ṹ��)��
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetBcuSdOrTd(u8 *pInData,void *pOutData)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 BCC1,BCC2;
	u8 BccCount;
		
	returnvale =	CheckBcuGetError(pInData);
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("BCU Get SDorTD Error��\r\n");
		#endif	
		return BCU_GetError;
	}
	
	/*BCCУ��*/
	if(pInData[1] == BCU_SD_COMMAND)			//���ܵ�ΪSD��Ӧ
	{
		BcuSdFrame SdDataFrame;		//�������
		BccCount =0;
		
		memcpy(&SdDataFrame,pInData,sizeof(BcuSdFrame));  //����������ݸ��Ƹ�����
		GenerateBcuBccChcek(&SdDataFrame.BcuSdData,sizeof(BcuSdText),&BCC1,&BCC2);//����BCC
		
		if(BCC1 != SdDataFrame.SdBCC1 )
				BccCount++;
		if(BCC2 != SdDataFrame.SdBCC2)
				BccCount ++;
		
		if(BccCount >0)		//BCCУ�����
		{
			#if DEBUG_RS485==1
			printf("BCU BCCУ��ֵ������\r\n");
			#endif		
			return BCU_BCCError;
		}
		
		else		//BCC��ȷ
		{
			memcpy(pOutData,&SdDataFrame,sizeof(BcuSdFrame));		//������copy��ȥ
		}
	}
		
	else if(pInData[1] == BCU_TD_COMMAND ) 	//���ܵ�ΪTD��Ӧ
	{
			BcuTdFrame TdDataFrame;		//�������
			BccCount =0;
		
			memcpy(&TdDataFrame,pInData,sizeof(BcuTdFrame));  //����������ݸ��Ƹ�����
			GenerateBcuBccChcek(&TdDataFrame.BcuTdData,sizeof(BcuTdText),&BCC1,&BCC2);
			
			if(BCC1 != TdDataFrame.TdBCC1 )
				BccCount++;
			if(BCC2 != TdDataFrame.TdBCC2)
				BccCount ++;
			
			if(BccCount >0)
			{
				#if DEBUG_RS485==1
					printf("BCU BCCУ��ֵ������\r\n");
				#endif		
				return BCU_BCCError;
			}
			else
			{
				memcpy(pOutData,&TdDataFrame,sizeof(BcuTdFrame));
			}
	}
	
	else
	{
		#if DEBUG_RS485==1
		printf("BCU Get SdOrTD Data Error!\r\n ");
		#endif
		return BCU_GetError;
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
s8 CheckBcuGetError(u8 *pInData)
{
		//1.����SDR �󣬾���100ms ������û�������յ�SD ʱ,����
	{
		u8 i=0;
		for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
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
				return BCU_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return BCU_FrameError;
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


/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
	BCC1Even���õ���bccEvenУ����
	Bcc1Odd:	�õ���bccOddУ����
����ֵ����
*/
void GenerateBcuBccChcek(void * pData,u8 len,u8* BCC1,u8* BCC2)
{
	u8 i=0;
	u8 *BccData;
	u8 DataTotalLen= 0;
	DataTotalLen = len +1;
	
	BccData = mymalloc(SRAMIN,len+1);		//��̬�����ڴ� ,��keil�ﲻ��ʹ��BccData[len],����������ʾ���鳤��δ����
																			/* ����ĳ��ȱ����ݳ��ȶ�1�ֽڣ�Ϊ�˴��ETX*/	
	if(BccData == NULL)
	{
		printf("BCC�ڴ�����ʧ��!\r\n");
	}
	/*copy ����*/
	memcpy(BccData,pData,len);
	BccData[len] = BCU_ETX;
	
	for(i=0;i<DataTotalLen;i++)  	/*�ȴ�������ݳ��ȶ���1�ֽ�*/
	{
		if((i&0x01) ==0 )	//�ж�Ϊż����
		{
			*BCC1 = (*BCC1) ^BccData[i]; 
		}
		
		else		//����Ϊ������
		{
			*BCC2 = (*BCC2) ^BccData[i]; 
		}
	};
	myfree(SRAMIN,BccData);			//�ͷ��ڴ�
}


