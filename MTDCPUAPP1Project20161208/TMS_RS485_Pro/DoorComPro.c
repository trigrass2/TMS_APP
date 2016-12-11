#include "DoorComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
//#include "MTD_MFD_RS485.h"

/*
	����˵����
���ܣ����ͳ��ſ��Ƶ�״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendDoorSdr(DoorSdrText * pInData/*,u8 * pOutData*/)  //���ͳ��ſ��Ƶ�״̬�������ݡ�
{
		DoorSdrFrame SdrDataFrame;
	
		#if	 DEBUG_RS485==1
			printf("����豸���ſ������÷�����������\r\n");
		#endif
	
		SdrDataFrame.DoorSdrStx=DOOR_STX;
		memcpy(&SdrDataFrame.DoorSdrData,pInData,sizeof(DoorSdrText));	//����������Ӧ�ò����û���ֵ��
		SdrDataFrame.DoorSdrBcc = GenerateBccChcek(&(SdrDataFrame.DoorSdrData),sizeof(DoorSdrText));
	
		//memcpy(pOutData,&SdrDataFrame,sizeof(DoorSdrFrame));
		//MTDMFD_USART3_RS485_Send_Data((u8 *)&SdrDataFrame,sizeof(DoorSdrFrame));//ֱ�ӵ���Uart3��������
}


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_DOOR_SD_State�ṹ��)��
����ֵ��0,�޴�  �����д�
*/
s8 GetDoorSd(u8 *pInData,DoorSdFrame *SdFrame)		//����豸���ܳ��ſ��Ƶ�״̬��Ӧ���ݡ�
{
	s8 returnvale = 0;
	DoorSdFrame SdDataFrame;
	u8 checkBccData;
	
	returnvale=CheckDoorGetError(pInData);		//�������Ƿ����
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return DOOR_GetError;
	}
	
	memcpy(&SdDataFrame,pInData,sizeof(DoorSdFrame));	//�����ڽ��ܵ����ݿ������ṹ����
	
	checkBccData = GenerateBccChcek(&SdDataFrame.DoorSdData,sizeof(DoorSdText));	//�õ����ݵ�BccУ��ֵ
	
	if(checkBccData != SdDataFrame.DoorSdBcc)
	{
		#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
		#endif		
		return DOOR_BCCError;
	}
	else
	{
		memcpy(SdFrame,&SdDataFrame,sizeof(DoorSdFrame));
	}
	//USART3_RS485_RxLength=0;		//����
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
s8 CheckDoorGetError(u8 *pInData)
{
	//1.����SDR �󣬾���20ms ������û�������յ�SD ʱ,����
	{
		u8 i=0;
		for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
		{
			delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ����   (�����������ʱ���൱�ڴ򿪶�ʱ����ֱ��ִ��������䣬�϶�������Ҫ��)
			if(g_RxDataFlag == 1)
			{
				g_RxDataFlag =0;
				break;
			}	
		}
		if(i>=10)
		{
			if(g_RxDataFlag == 0)	//˵��û�н��ܵ�����
			{
				#if DEBUG_RS485==1
					printf("10ms��û�н��ܵ�����\r\n");
				#endif
				return DOOR_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return DOOR_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
	if(pInData[0]!=DOOR_STX || pInData[1]<0x01 ||pInData[1]>0x08 || pInData[2]!=DOOR_SUB_ADDRES ||pInData[3]!=DOOR_COMMAND )
	{
		#if DEBUG_RS485==1
			printf("���ܵĿ�ʼ������ַ��������һ��������\r\n");
		#endif		
			return DOOR_DataError;
	}
	
	//û�д������
	return 0;
}

///*
//����˵����
//���ܣ������������������bcc��
//����: 
//	pData:��ڲ�������Ҫ����bcc�����ݡ�
//	len�� pData�����ݳ��� (�ֽڵ�λ )
//����ֵ���������ɵ�BCC��
//*/
//u8 GenerateDoorBccChcek(void * pData,u8 len)
//{
//	u8 i=0;
//	u8 BccCheckCode=0xFF;
//	u8 *BccData;
//	BccData = mymalloc(SRAMIN,len);		//��̬�����ڴ� ,��keil�ﲻ��ʹ��BccData[len],����������ʾ���鳤��δ����
//	memcpy(BccData,pData,len);
//	for(i=0;i<len;i++)
//	{
//		BccCheckCode=BccCheckCode^BccData[i];
//	};
//	myfree(SRAMIN,BccData);			//�ͷ��ڴ�
//	
//	return BccCheckCode;
//}




