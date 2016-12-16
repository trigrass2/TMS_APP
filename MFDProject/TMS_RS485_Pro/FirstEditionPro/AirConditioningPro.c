
#include "AirconditioningPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
//#include "stm32f4xx_it.h"		//��ȫ�ֱ����ı�־
#include "delay.h"
#include "CommunPro.h"

u8 TMS_RxBuffer[AIR_RX_BUF_SIZE];
u8 TMS_TxBuffer[AIR_TX_BUF_SIZE];

/*
	����˵����
���ܣ����Ϳյ���״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendAirCondit_SDR(S_TMS_AIR_SDR_Text *pInData,u8 * pOutData)
{
		S_TMS_AIR_SDR_Frame DataFrame;
	
		#if	 DEBUG_RS485==1
			printf("����豸������������\r\n");
		#endif
	
		DataFrame.SDR_STX=AIR_STX;
		memcpy(&DataFrame.SDR_Data,pInData,sizeof(S_TMS_AIR_SDR_Text));
	
		//����������Ӧ�ò����û���ֵ��
	
		DataFrame.SDR_Bcc=GenerateBccChcek(&(DataFrame.SDR_Data),sizeof(S_TMS_AIR_SDR_Text));
	
		memcpy(pOutData,&DataFrame,sizeof(S_TMS_AIR_SDR_Frame));
}


/*
	����˵����
���ܣ����ܿյ���״̬��Ӧ������ ת��Ϊʹ�� ��״̬���ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:������豸����õ�ÿ��״̬��ʶ(�ŵ�S_AIR_SD_State�ṹ��)��
					���յ��豸�����ܵ����ݾ����ı�����
����ֵ��0,�޴�  �����д�
*/
s8 GetAirCondit_SD(u8 *pInData,S_AIR_SD_State *pOutData)
{
	s8 returnvale;
	S_TMS_AIR_SD_Frame DataFrame;
	u8 checkBccData;
	returnvale=CheckGetError(pInData);		//�������Ƿ����
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return AIR_GetError;
	}
	
	memcpy(&DataFrame,pInData,sizeof(DataFrame));	//�����ڽ��ܵ����ݿ������ṹ����

	checkBccData = GenerateBccChcek(&DataFrame.SD_Data,sizeof(S_TMS_AIR_SD_Text));	//�õ����ݵ�BccУ��ֵ
	
	if(checkBccData != DataFrame.SD_Bcc)
	{
		#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
		#endif		
		return AIR_BCCError;
	}
	
	if(returnvale==0)	//��ʾû�д���
	{
		/*Unit1 ����״̬*/
		pOutData->U1AllWarm = GetDataBit(&DataFrame.SD_Data.U1ActiveState,AIR_ALLWAEM_POSIT);
		pOutData->U1HalfWarm = GetDataBit(&DataFrame.SD_Data.U1ActiveState,AIR_HALFWAEM_POSIT);
		pOutData->U1HighCool = GetDataBit(&DataFrame.SD_Data.U1ActiveState,AIR_HIGHCOOL_POSIT);
		pOutData->U1WeakCold = GetDataBit(&DataFrame.SD_Data.U1ActiveState,AIR_WEAKCOLD_POSIT);
		pOutData->U1Blowing	=	GetDataBit(&DataFrame.SD_Data.U1ActiveState,AIR_BLOWING_POSIT);
		pOutData->U1Automatic	=	GetDataBit(&DataFrame.SD_Data.U1ActiveState,AIR_AUTOMATIC_POSIT);
		
		/*Unit2 ����״̬*/
		pOutData->U2AllWarm = GetDataBit(&DataFrame.SD_Data.U2ActiveState,AIR_ALLWAEM_POSIT);
		pOutData->U2HalfWarm = GetDataBit(&DataFrame.SD_Data.U2ActiveState,AIR_HALFWAEM_POSIT);
		pOutData->U2HighCool = GetDataBit(&DataFrame.SD_Data.U2ActiveState,AIR_HIGHCOOL_POSIT);
		pOutData->U2WeakCold = GetDataBit(&DataFrame.SD_Data.U2ActiveState,AIR_WEAKCOLD_POSIT);
		pOutData->U2Blowing	=	GetDataBit(&DataFrame.SD_Data.U2ActiveState,AIR_BLOWING_POSIT);
		pOutData->U2Automatic	=	GetDataBit(&DataFrame.SD_Data.U2ActiveState,AIR_AUTOMATIC_POSIT);
		
		/*Unit1	����*/
		pOutData->U1Cooler1Fault = GetDataBit(&DataFrame.SD_Data.U1Fault1,AIR_COOLER1FAULT_POSIT);
		pOutData->U1Cooler2Fault = GetDataBit(&DataFrame.SD_Data.U1Fault1,AIR_COOLER2FAULT_POSIT);
		pOutData->U1Vetilator1Fault = GetDataBit(&DataFrame.SD_Data.U1Fault1,AIR_VETILATOR1FAULT_POSIT);
		pOutData->U1Vetilator2Fault = GetDataBit(&DataFrame.SD_Data.U1Fault1,AIR_VETILATOR2FAULT_POSIT);
		pOutData->U1Vetilator3Fault = GetDataBit(&DataFrame.SD_Data.U1Fault1,AIR_VETILATOR3FAULT_POSIT);
		
		pOutData->U1Compressor1Protect = GetDataBit(&DataFrame.SD_Data.U1Fault2,AIR_COMPRESSOR1PROTECT_POSIT);
		pOutData->U1Compressor1Fault	= GetDataBit(&DataFrame.SD_Data.U1Fault2,AIR_COMPRESSOR1FAULT_POSIT);
		pOutData->U1Compressor2Protect = GetDataBit(&DataFrame.SD_Data.U1Fault2,AIR_COMPRESSOR2PROTECT_POSIT);
		pOutData->U1Compressor2Fault	=	GetDataBit(&DataFrame.SD_Data.U1Fault2,AIR_COMPRESSOR2FAULT_POSIT);
		
		/*Uint2 ����*/
		pOutData->U2Cooler1Fault = GetDataBit(&DataFrame.SD_Data.U2Fault1,AIR_COOLER1FAULT_POSIT);
		pOutData->U2Cooler2Fault = GetDataBit(&DataFrame.SD_Data.U2Fault1,AIR_COOLER2FAULT_POSIT);
		pOutData->U2Vetilator1Fault = GetDataBit(&DataFrame.SD_Data.U2Fault1,AIR_VETILATOR1FAULT_POSIT);
		pOutData->U2Vetilator2Fault = GetDataBit(&DataFrame.SD_Data.U2Fault1,AIR_VETILATOR2FAULT_POSIT);
		pOutData->U2Vetilator3Fault = GetDataBit(&DataFrame.SD_Data.U2Fault1,AIR_VETILATOR3FAULT_POSIT);
		
		pOutData->U2Compressor1Protect = GetDataBit(&DataFrame.SD_Data.U2Fault2,AIR_COMPRESSOR1PROTECT_POSIT);
		pOutData->U2Compressor1Fault	= GetDataBit(&DataFrame.SD_Data.U2Fault2,AIR_COMPRESSOR1FAULT_POSIT);
		pOutData->U2Compressor2Protect = GetDataBit(&DataFrame.SD_Data.U2Fault2,AIR_COMPRESSOR2PROTECT_POSIT);
		pOutData->U2Compressor2Fault	=	GetDataBit(&DataFrame.SD_Data.U2Fault2,AIR_COMPRESSOR2FAULT_POSIT);
		
		/*�����¶�*/
		pOutData->RoomTemperature = (DataFrame.SD_Data.RoomTemperature - 0xD0)+5;		//�����¶� D0H��F3H��5��40�棩 [1��:1bit]		

	}
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
s8 CheckGetError(u8 *pInData)
{
	//1.����SDR �󣬾���20ms ������û�������յ�SD ʱ,����
	{
		u8 i=0;
		for(i=0;i<10;i++)
		{
			delay_ms(2);//��ʱ20ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
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
					printf("20ms��û�н��ܵ�����\r\n");
				#endif
				return AIR_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return AIR_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
	if(pInData[0]!=AIR_STX || pInData[1]!=AIR_ADDRES || pInData[2]!=AIR_SUB_ADDRES ||pInData[3]!=AIR_COMMAND)
	{
		#if DEBUG_RS485==1
			printf("���ܵĿ�ʼ������ַ��������һ��������\r\n");
		#endif		
			return AIR_DataError;
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
//u8 GenerateBccChcek(void * pData,u8 len)
//{
//	u8 i=0;
//	u8 BccCheckCode=0;
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

///*
//����˵����
//���ܣ�Ҫ���ĳλ������
//����: 
//	Data:��ڲ���,����ָ�롣
//	GetPosit�� Ҫ�õ����ݵ�λ��
//����ֵ�����صõ�ĳλ��ֵ
//*/
//u8 GetDataBit(u8 * Data,u8 GetPosit)
//{
//	u8 vale;
//	vale=*Data <<(7-GetPosit);
//	return vale>>7;
//}


//���Դ�ӡ��Ϣ
void test(void)
{
	u8 i,a;
	S_TMS_AIR_SDR_Text rx;
S_AIR_SD_State	State;
	rx.Address=0x01;
	rx.Sub_Address=0x00;
	rx.Command=0x20;
	rx.Year=16;
	rx.Month=6;
	rx.Day=24;
	rx.Hour=1;
	rx.Minute=2;
	rx.Second=3;
	rx.SetClock=1;
//	rx.Dummy1=0;
//	rx.Dummy2=0;

	#if	 DEBUG_RS485==1
	printf("rx_size=%d\r\n",sizeof(rx));
//	memcpy(TMS_TxBuffer,&rx,sizeof(rx));
//	for(i=0;i<sizeof(rx);i++)
//	{
//		printf("rx=%d\r\n",TMS_TxBuffer[i]);
//	}
	
	printf("SendAirCondit_SDR  test\r\n");
	SendAirCondit_SDR(&rx,TMS_TxBuffer);
	for(i=0;i<14;i++)
	{
		printf("rx=%d\r\n",TMS_TxBuffer[i]);
	}
	printf("\r\n");
	
	GetAirCondit_SD(TMS_TxBuffer,&State);
	
	i=GenerateBccChcek( &rx,sizeof(S_TMS_AIR_SDR_Text));	//����bcc
	printf("code=%x,%d\r\n",i,i);
	
	
	{
		a=0xDB;
		i=0;
		i=GetDataBit(&a,7);
		printf("i=%d\r\n",i);
			i=GetDataBit(&a,6);
		printf("i=%d\r\n",i);
		i=GetDataBit(&a,5);
		printf("i=%d\r\n",i);
			i=GetDataBit(&a,3);
		printf("i=%d\r\n",i);
			i=GetDataBit(&a,2);
		printf("i=%d\r\n",i);
	}
	
	#endif 
	

}
