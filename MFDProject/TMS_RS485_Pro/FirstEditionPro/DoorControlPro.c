#include "DoorControlPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "main.h"

/*
	����˵����
���ܣ����ͳ��ſ��Ƶ�״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendDoorControl_SDR(S_TMS_DOOR_SDR_Text * pInData,u8 * pOutData)  //���ͳ��ſ��Ƶ�״̬�������ݡ�
{
		S_TMS_DOOR_SDR_Frame DataFrame;
	
		#if	 DEBUG_RS485==1
			printf("����豸���ſ������÷�����������\r\n");
		#endif
	
		DataFrame.SDR_STX=DOOR_STX;
		memcpy(&DataFrame.SDR_Data,pInData,sizeof(S_TMS_DOOR_SDR_Text));

		//����������Ӧ�ò����û���ֵ��
	
		DataFrame.SDR_Bcc=GenerateDoorBccChcek(&(DataFrame.SDR_Data),sizeof(S_TMS_DOOR_SDR_Text));
	
		memcpy(pOutData,&DataFrame,sizeof(S_TMS_DOOR_SDR_Frame));
}


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_DOOR_SD_State�ṹ��)��
����ֵ��0,�޴�  �����д�
*/
s8 GetDoorControl_SD(u8 *pInData,S_TMS_DOOR_SD_Frame *DoorSDFrame,S_DOOR_SD_State *pOutData)		//����豸���ܳ��ſ��Ƶ�״̬��Ӧ���ݡ�
{
	s8 returnvale;
	S_TMS_DOOR_SD_Frame DataFrame;
	u8 checkBccData;
	returnvale=CheckDoorGetError(pInData);		//�������Ƿ����
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return DOOR_GetError;
	}
	
	memcpy(&DataFrame,pInData,sizeof(DataFrame));	//�����ڽ��ܵ����ݿ������ṹ����

	memcpy(DoorSDFrame,&DataFrame,sizeof(DataFrame));//whh ����
	
	checkBccData = GenerateDoorBccChcek(&DataFrame.SD_Data,sizeof(S_TMS_DOOR_SD_Text));	//�õ����ݵ�BccУ��ֵ
	
	if(checkBccData != DataFrame.SD_Bcc)
	{
		#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
		#endif		
		return DOOR_BCCError;
	}
	
	if(returnvale==0)	//��ʾû�д���
	{
		/*����״̬*/
		pOutData->SignalDetection = GetDoorDataBit(&DataFrame.SD_Data.CarDoorState,DOOR_SIGNALDETECTION_POSIT);
		pOutData->DoorIsolation = GetDoorDataBit(&DataFrame.SD_Data.CarDoorState,DOOR_DOORISOLATION_POSIT);
		pOutData->DoorSwitchAction = GetDoorDataBit(&DataFrame.SD_Data.CarDoorState,DOOR_DOORSWITCHACTION_POSIT);
		pOutData->DoorCloseState = GetDoorDataBit(&DataFrame.SD_Data.CarDoorState,DOOR_DOORCLOSESTATE_POSIT);
		pOutData->DoorOpenState = GetDoorDataBit(&DataFrame.SD_Data.CarDoorState,DOOR_DOOROPENSTATE_POSIT);
		
		/*���Ź���*/
		pOutData->CloseDoorExtrusion = GetDoorDataBit(&DataFrame.SD_Data.CarDoorFault,DOOR_CLOSEDOOREXTRUSION_POSIT);
		pOutData->OpenDoorExtrusion = GetDoorDataBit(&DataFrame.SD_Data.CarDoorFault,DOOR_OPENDOOREXTRUSION_POSIT);
		pOutData->DoorSystemFault	=	GetDoorDataBit(&DataFrame.SD_Data.CarDoorFault,DOOR_DOORSYSTEMFAULT_POSIT);
		pOutData->EmergencyDeviceOper = GetDoorDataBit(&DataFrame.SD_Data.CarDoorFault,DOOR_EMERGENCYDEVICEOPER_POSIT);
		
	}
		//û�д��󷵻�0
		return 0;
}

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
			if(g_Uart3RxDataFlag == 1)
			{
				g_Uart3RxDataFlag =0;
				break;
			}	
		}
		if(i>=10)
		{
			if(g_Uart3RxDataFlag == 0)	//˵��û�н��ܵ�����
			{
				#if DEBUG_RS485==1
					printf("10ms��û�н��ܵ�����\r\n");
				#endif
				return DOOR_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart3FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
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

/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
����ֵ���������ɵ�BCC��
*/
u8 GenerateDoorBccChcek(void * pData,u8 len)
{
	u8 i=0;
	u8 BccCheckCode=0xFF;
	u8 *BccData;
	BccData = mymalloc(SRAMIN,len);		//��̬�����ڴ� ,��keil�ﲻ��ʹ��BccData[len],����������ʾ���鳤��δ����
	memcpy(BccData,pData,len);
	for(i=0;i<len;i++)
	{
		BccCheckCode=BccCheckCode^BccData[i];
	};
	myfree(SRAMIN,BccData);			//�ͷ��ڴ�
	
	return BccCheckCode;
}

	//Ҫ���ĳλ�����ݣ��Ƚ������ƶ������λ��Ȼ�����Ƶ������λ��
/*
����˵����
���ܣ�Ҫ���ĳλ������
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
u8 GetDoorDataBit(u8 * Data,u8 GetPosit)
{
	u8 vale;
	vale=*Data <<(7-GetPosit);
	return vale>>7;
}


