#include "ATCComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "MTD_MFD_RS485.h"
#include "main.h"
/*
	����˵����
���ܣ�����豸���͸�ATC��״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendATCSdr(ATCSdrText * pInData/*,u8 * pOutData*/)
{
	ATCSdrFrame SdrDataFrame;
		
	SdrDataFrame.ATCSdrDle1 = ATC_DLE;
	SdrDataFrame.ATCSdrStx = ATC_STX;
	memcpy(&SdrDataFrame.ATCSdrData,pInData,sizeof(ATCSdrText));
	
	SdrDataFrame.ATCSdrDle2 = ATC_DLE;
	SdrDataFrame.ATCSdrEtx = ATC_ETX;
	SdrDataFrame.ATCSdrBcc =GenerateBccChcek (pInData,sizeof(ATCSdrText));
	
	//memcpy(pOutData,&SdrDataFrame,sizeof(ATCSdrFrame));
	MTDMFD_USART3_RS485_Send_Data((u8 *)&SdrDataFrame,sizeof(ATCSdrFrame));//ֱ�ӵ���Uart3��������
}


/*
	����˵����
���ܣ�����豸����ATCD��״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ��������ݽṹ��)��

����ֵ��0,�޴�  �����д�
*/
s8 GetATCSd(u8 *pInData,ATCSdFrame *SdFrame)
{
	s8 returnvale;
	ATCSdFrame SdDataFrame;
	u8 checkBccData;
	
	returnvale=CheckATCGetError(pInData);		//�������Ƿ����
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return ATC_GetError;
	}
	
	memcpy(&SdDataFrame,pInData,sizeof(SdDataFrame));	//�����ڽ��ܵ����ݿ������ṹ����
	
	checkBccData = GenerateBccChcek(&SdDataFrame.ATCSdData,sizeof(ATCSdText));	//�õ����ݵ�BccУ��ֵ
	
	if(checkBccData != SdDataFrame.ATCSdBcc)
	{
		#if DEBUG_RS485==1
			printf("ATC BCCУ��ֵ������\r\n");
		#endif		
		return ATC_BCCError;
	}
	
	else		//û�д���
	{
		memcpy(SdFrame,&SdDataFrame,sizeof(ATCSdFrame));
	}
		
	return 0;
}	


/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckATCGetError(u8 *pInData)
{
		//1.����SDR �󣬾���100ms ������û�������յ�SD ʱ,����
	u8 i=0;
	
	/*�ж�10ms���Ƿ���յ���ʼ��־*/
	for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_Uart3RxStartFlag == 1)
		{
			g_Uart3RxStartFlag =0;
			break;
		}	
		
		else
			delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ����   (�����������ʱ���൱�ڴ򿪶�ʱ����ֱ��ִ��������䣬�϶�������Ҫ��)
	}
	if(i>=10)
	{
		if(g_Uart3RxStartFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("ATC 10ms��û�н��ܵ�����\r\n");
			#endif
			return ATC_GetTimeoutError;
		}
	}
	
	/*�ٴ��ж������Ƿ�������*/
	for(i=0;i<100;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_Uart3RxDataFlag == 1)
		{
			g_Uart3RxDataFlag =0;
			break;
		}	
		
		else
			delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
	}
	if(i>=100)
	{
		if(g_Uart3RxDataFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("ATC �������ݲ�����!\r\n");
			#endif
			return ATC_GetTimeoutError;
		}
	}

	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart3FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		g_Uart3FrameErrorFlag = 0;  //���֡������żУ�����λ
		
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return ATC_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
	if(pInData[0]!=ATC_DLE ||pInData[1]!=ATC_STX || pInData[2]!=ATC_SD_TEXTCODEL8 || pInData[3]!=ATC_SD_TEXTCODEH8)
	{
		#if DEBUG_RS485==1
			printf("���ܵĿ�ʼ������ַ��������һ��������\r\n");
		#endif		
			return ATC_DataError;
	}

	//û�д������
	return 0;
}
