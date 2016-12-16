#include "PISComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "MTD_MFD_RS485.h"
#include "main.h"
/*
	����˵����
���ܣ�����豸���͸�PIS��״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendPISSdr(PISSdrText * pInData/*,u8 * pOutData*/)
{
	PISSdrFrame  SdrDataFrame;
	
	SdrDataFrame.PISSdrStx = PIS_STX;
	memcpy(&SdrDataFrame.PISSdrData,pInData,sizeof(PISSdrText));
	SdrDataFrame.PISSdrBcc =GenerateBccChcek (pInData,sizeof(PISSdrText));
	
	//memcpy(pOutData,&SdrDataFrame,sizeof(PISSdrFrame));
	MTDMFD_USART2_RS485_Send_Data((u8 *)&SdrDataFrame,sizeof(PISSdrFrame));//ֱ�ӵ���Uart2��������
}


/*
	����˵����
���ܣ�����豸����PISD��״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ��������ݽṹ��)��

����ֵ��0,�޴�  �����д�
*/
s8 GetPISSd(u8 *pInData,void *pOutData)
{
	s8 returnvale;
	PISSdFrame SdDataFrame;
	u8 checkBccData;
	
	returnvale=CheckPISGetError(pInData);		//�������Ƿ����
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return returnvale;
	}
	
	memcpy(&SdDataFrame,pInData,sizeof(SdDataFrame));	//�����ڽ��ܵ����ݿ������ṹ����
	
	checkBccData = GenerateBccChcek(&SdDataFrame.PISSdData,sizeof(PISSdrText));	//�õ����ݵ�BccУ��ֵ
	
	if(checkBccData != SdDataFrame.PISSdBcc)
	{
		#if DEBUG_RS485==1
			printf("PIS BCCУ��ֵ������\r\n");
		#endif		
		return PIS_BCCError;
	}
	
	else		//û�д���
	{
		memcpy(pOutData,&SdDataFrame,sizeof(PISSdFrame));
	}
		return 0;
}	


/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckPISGetError(u8 *pInData)
{
	//1.����SDR �󣬾���10ms ������û�������յ�SD ʱ,����
	u8 i=0;
	for(i=0;i<10;i++)
	{
		if(g_Uart2RxStartFlag == 1)
		{
			g_Uart2RxStartFlag = 0;
			break;
		}
		
		else
			delay_ms(1);//��ʱ20ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
	}
	if(i>=10)
	{
		if(g_Uart2RxStartFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("PIS 10ms��û�н��ܵ�����!\r\n");
			#endif
			return PIS_GetTimeoutError;
		}
	}	

	/*�жϽ�������δ��ɱ�־*/
	for(i=0;i<40;i++)
	{
		if(g_Uart2RxDataFlag == 1)
		{
			g_Uart2RxDataFlag = 0;
			break;
		}
		
		else
			delay_ms(1);//��ʱ20ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
	}
	if(i>=40)
	{
		if(g_Uart2RxDataFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("PIS �������ݲ�������\r\n");
			#endif
			return PIS_GetTimeoutError;
		}
	}	
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart2FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		g_Uart2FrameErrorFlag = 0;
		
		#if DEBUG_RS485==1
			printf("PIS ��⵽֡������żУ�����\r\n");
		#endif
		return PIS_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
	if(pInData[0]!=PIS_STX || pInData[1]!=PIS_SD_DATALEN )
	{
		#if DEBUG_RS485==1
			printf("PIS ���ܵĿ�ʼ������ַ��������һ��������\r\n");
		#endif		
			return PIS_DataError;
	}
	
	//û�д������
	return 0;
}
