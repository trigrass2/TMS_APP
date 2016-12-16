#include "ACPComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "CRC_CCITT.h"
#include "MTD_MFD_RS485.h"
#include "main.h"
/*
	����˵����
���ܣ����Ϳ�ѹ����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendACPSdr(ACPSdrFrame * pInData/*,u8 * pOutData*/)  
{
		ACPSdrFrame SdrDataFrame;
		u16	CrcVale;
		
		#if	 DEBUG_RS485==1
			printf("����豸������������\r\n");
		#endif
		
		SdrDataFrame.ACPDeviceAddress = ACP_DEVICEADDRES;
		SdrDataFrame.Command	= ACP_COMMAND;
		memcpy(&SdrDataFrame.StartAddressH8,&pInData->StartAddressH8,4);		//cope��ַ���Ĵ���ֵ
	
		CrcVale = crcCompute((u8*)&SdrDataFrame,sizeof(ACPSdrFrame)-2);
		SdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
		SdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
	
		//memcpy(pOutData,&SdrDataFrame,sizeof(ACPSdrFrame));		//��������
		MTDMFD_USART3_RS485_Send_Data((u8 *)&SdrDataFrame,sizeof(ACPSdrFrame));//ֱ�ӵ���Uart3��������
}

/*
	����˵����
���ܣ����ܿ�ѹ����״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_ACPre_SD_StateBit�ṹ��)��
����ֵ��0,�޴�  �����д�
*/
s8 GetACPSd(u8 *pInData,ACPSdFrame *pOutData)
{
	s8 returnvale=0;
	ACPSdFrame SdDataFrame;
	u16 SourCrcVale,ComputeCrcVale;
	
	returnvale=CheckACPGetError(pInData);		//�������Ƿ����
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return ACP_GetError;
	}
	
	memcpy(&SdDataFrame,pInData,sizeof(ACPSdFrame));	//�����ڽ��ܵ����ݿ������ṹ����

	SourCrcVale = ((u16)SdDataFrame.CRCH8<<8 )| (SdDataFrame.CRCL8);	//Դcrcֵ
	ComputeCrcVale = crcCompute((u8*)&SdDataFrame,sizeof(ACPSdFrame)-2);		//����õ���crcֵ

	if(SourCrcVale == ComputeCrcVale)
	{
		memcpy(pOutData,pInData,sizeof(ACPSdFrame));		//copy����
	}
	else
	{
		#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
		#endif		
		return ACP_CRCError;
	}
		//û�д��󷵻�0
		return 0;
}


//extern 	u8 g_Uart3RxDataFlag;
//extern	u8 g_Uart3FrameErrorFlag;
/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckACPGetError(u8 *pInData)
{
	//1.����SDR �󣬾���200ms ������û�������յ�SD ʱ,����
	u8 i=0;
	/*�ж�200ms���Ƿ���յ���ʼ��־*/
	for(i=0;i<20;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_Uart3RxStartFlag == 1)
		{
			g_Uart3RxStartFlag =0;
			break;
		}	
		
		else
			delay_ms(10);//��ʱ10ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ����   (�����������ʱ���൱�ڴ򿪶�ʱ����ֱ��ִ��������䣬�϶�������Ҫ��)
	}
	if(i>=20)
	{
		if(g_Uart3RxStartFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("ACP 200ms��û�н��ܵ�����\r\n");
			#endif
			return ACP_GetTimeoutError;
		}
	}	
	
	/*�ж�100ms���ǹ�������ɱ�־*/
	for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_Uart3RxDataFlag == 1)
		{
			g_Uart3RxDataFlag =0;
			break;
		}	
		else
			delay_ms(10);		//��ʱ10ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
	}
	if(i>=10)
	{
		if(g_Uart3RxDataFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("ACP û�н��ܵ���������\r\n");
			#endif
			return ACP_GetTimeoutError;
		}
	}

	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart3FrameErrorFlag == 1)	//��⵽֡������żУ�����ʱ
	{
		g_Uart3FrameErrorFlag = 0;  //���֡������żУ�����λ
		
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return ACP_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
	if(pInData[0]!=ACP_DEVICEADDRES || pInData[1]!=ACP_COMMAND )
	{
		#if DEBUG_RS485==1
			printf("ACP ���ܵĿ�ʼ������ַ��������һ��������\r\n");
		#endif		
			return ACP_DataError;
	}
	
	//û�д������
	return 0;
}



void test_ACP(void)
{
	
		
}

