#include "ACCComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
//#include "stm32f4xx_it.h"		//��ȫ�ֱ����ı�־
#include "delay.h"
#include "CommunPro.h"
#include "MTD_MFD_RS485.h"
#include "main.h"

u8 TMS_RxBuffer[ACC_RX_BUF_SIZE];
u8 TMS_TxBuffer[ACC_TX_BUF_SIZE];

/*
	����˵����
���ܣ����Ϳյ���״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵�����
����ֵ���޷���ֵ
*/
void SendACCSdr(ACCSdrText *pInData/*,u8 * pOutData*/)
{
		ACCSdrFrame SdrDataFrame;
	
		#if	 DEBUG_RS485==1
			printf("����豸������������\r\n");
		#endif
	
		SdrDataFrame.ACCSdrStx = ACC_STX;
		memcpy(&SdrDataFrame.ACCSdrData,pInData,sizeof(ACCSdrText));		//����������Ӧ�ò����û���ֵ��
		SdrDataFrame.ACCSdrBcc=GenerateBccChcek(&(SdrDataFrame.ACCSdrData),sizeof(ACCSdrText));
		
		//memcpy(pOutData,&SdrDataFrame,sizeof(ACCSdrFrame));
		MTDMFD_USART2_RS485_Send_Data((u8 *)&SdrDataFrame,sizeof(ACCSdrFrame));//ֱ�ӵ���Uart2��������
}


/*
	����˵����
���ܣ����ܿյ���״̬��Ӧ������ ת��Ϊʹ�� ��״̬���ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:������豸����õ�ÿ��״̬��ʶ(�ŵ�ACCSdText�ṹ��)��
					���յ��豸�����ܵ����ݾ����ı�����
����ֵ��0,�޴�  �����д�
*/
s8 GetACCSd(u8 *pInData,ACCSdText *pOutData)
{
	s8 returnvale=0;
	ACCSdFrame 	SdDataFrame;
	u8 checkBccData;
	
	returnvale=CheckGetError(pInData);		//�������Ƿ����
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return returnvale;
	}
	
	memcpy(&SdDataFrame,pInData,sizeof(ACCSdFrame));	//�����ڽ��ܵ����ݿ������ṹ����
	checkBccData = GenerateBccChcek(&SdDataFrame.ACCSdData,sizeof(ACCSdText));	//�õ����ݵ�BccУ��ֵ
	
	if(checkBccData != SdDataFrame.ACCSdBcc)
	{
		#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
		#endif		
		return ACC_BCCError;
	}
	
	else//��ʾû�д���
	{
		memcpy(pOutData,&SdDataFrame.ACCSdData,sizeof(ACCSdText));	//copy����
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
s8 CheckGetError(u8 *pInData)
{
	//1.����SDR �󣬾���20ms ������û�������յ�SD ʱ,����
	u8 i=0;
	
	for(i=0;i<20;i++)
	{
		if(g_Uart2RxStartFlag == 1)
		{
			g_Uart2RxStartFlag = 0;
			break;
		}
		
		else
			delay_ms(1);//��ʱ20ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
	}
	if(i>=20)
	{
		if(g_Uart2RxStartFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("ACC 20ms��û�н��ܵ�����!\r\n");
			#endif
			return ACC_GetTimeoutError;
		}
	}	
	
	
	for(i=0;i<10;i++)
	{
		if(g_Uart2RxDataFlag == 1)
		{
			g_Uart2RxDataFlag = 0;
			break;
		}
		
		else
			delay_ms(1);//��ʱ20ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
	}
	if(i>=10)
	{
		if(g_Uart2RxDataFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("ACC �������ݲ�������\r\n");
			#endif
			return ACC_GetTimeoutError;
		}
	}

	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart2FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		g_Uart2FrameErrorFlag = 0;
		
		#if DEBUG_RS485==1
			printf("ACC ��⵽֡������żУ�����\r\n");
		#endif
		return ACC_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
	if(pInData[0]!=ACC_STX || pInData[1]!=ACC_ADDRES || pInData[2]!=ACC_SUB_ADDRES ||pInData[3]!=ACC_COMMAND)
	{
		#if DEBUG_RS485==1
			printf("���ܵĿ�ʼ������ַ��������һ��������\r\n");
		#endif		
			return ACC_DataError;
	}
	
	//û�д������
	return 0;
}



//���Դ�ӡ��Ϣ
void test_ACC(void)
{
//			u8 i,a;
//			ACCSdrText rx;
//		ACCSdText	State;
//			rx.Address=0x01;
//			rx.Sub_Address=0x00;
//			rx.Command=0x20;
//			rx.Year=16;
//			rx.Month=6;
//			rx.Day=24;
//			rx.Hour=1;
//			rx.Minute=2;
//			rx.Second=3;
//			rx.SetClock=1;
//		//	rx.Dummy1=0;
//		//	rx.Dummy2=0;

//			#if	 DEBUG_RS485==1
//			printf("rx_size=%d\r\n",sizeof(rx));
//		//	memcpy(TMS_TxBuffer,&rx,sizeof(rx));
//		//	for(i=0;i<sizeof(rx);i++)
//		//	{
//		//		printf("rx=%d\r\n",TMS_TxBuffer[i]);
//		//	}
//			
//			printf("SendACCCondit_SDR  test\r\n");
//			SendACCCondit_SDR(&rx,TMS_TxBuffer);
//			for(i=0;i<14;i++)
//			{
//				printf("rx=%d\r\n",TMS_TxBuffer[i]);
//			}
//			printf("\r\n");
//			
//			GetACCCondit_SD(TMS_TxBuffer,&State);
//			
//			i=GenerateBccChcek( &rx,sizeof(ACCSdrText));	//����bcc
//			printf("code=%x,%d\r\n",i,i);
//			
//			
//			{
//				a=0xDB;
//				i=0;
//				i=GetDataBit(&a,7);
//				printf("i=%d\r\n",i);
//					i=GetDataBit(&a,6);
//				printf("i=%d\r\n",i);
//				i=GetDataBit(&a,5);
//				printf("i=%d\r\n",i);
//					i=GetDataBit(&a,3);
//				printf("i=%d\r\n",i);
//					i=GetDataBit(&a,2);
//				printf("i=%d\r\n",i);
//			}
//			
//			#endif 
//			

}
