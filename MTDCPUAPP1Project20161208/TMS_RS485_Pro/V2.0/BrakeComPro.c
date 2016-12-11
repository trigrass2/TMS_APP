#include "BrakeComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"

/*
	����˵����
���ܣ������ƶ�����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ�� (ָ�������Ϊ��S_TMS_BRAKE_SDR_Text,���� S_TMS_BRAKE_TDR_Text)   
	pOutData:ָ��ײ㴮�ڷ��͵�����
//	u8 mode:���͵�����ΪSDR������TDR	; 
����ֵ���޷���ֵ
*/
void SendBrake_SDR_Or_TDR(void * pInData,u8 * pOutData /*,u8 mode*/)
{
	u8 BCC1Even ,BCC1Odd;		//���BCCУ��λ

	u8 a,mode;
	a=((u8 *)pInData)[0];
	if( a == BRAKE_SDR_COMMAND)
		mode = SEND_SDR;
	else if( a == BRAKE_TDR_COMMAND)
		mode = SEND_TDR;
	
	
	switch(mode)
	{
		case SEND_SDR:
		{
				S_TMS_BRAKE_SDR_Frame DataFrame;
				
				DataFrame.SDR_STX = BRAKE_STX;
				
				memcpy(&DataFrame.SDR_Data,pInData,sizeof(S_TMS_BRAKE_SDR_Text));	 //�����͵��ı���Ϣȫ������������֡

				DataFrame.SDR_ETX = BRAKE_ETX;
				
				GenerateBrakeBccChcek(pInData,sizeof(S_TMS_BRAKE_SDR_Text),&BCC1Even,&BCC1Odd);
				
				DataFrame.SDR_BCC1EVEN = BCC1Even;
				DataFrame.SDR_BCC1ODD = BCC1Odd;
				
				memcpy(pOutData,&DataFrame,sizeof(DataFrame));
		}	
			break;
		
		case SEND_TDR:
		{
			S_TMS_BRAKE_TDR_Frame DataFrame;
			
			DataFrame.TDR_STX = BRAKE_STX;
			
			memcpy(&DataFrame.TDR_Data,pInData,sizeof(S_TMS_BRAKE_TDR_Text));	 //�����͵��ı���Ϣȫ������������֡

			DataFrame.TDR_ETX = BRAKE_ETX;
			
			GenerateBrakeBccChcek(pInData,sizeof(S_TMS_BRAKE_TDR_Text),&BCC1Even,&BCC1Odd);
			
			DataFrame.TDR_BCC1EVEN = BCC1Even;
			DataFrame.TDR_BCC1ODD = BCC1Odd;
			
			memcpy(pOutData,&DataFrame,sizeof(DataFrame));
		}
			break;
		
		default :
			break;
	}
}


/*
	����˵����
���ܣ����ܳ��ſ��Ƶ�״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݵ�ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_TMS_BRAKE_SD_Text�ṹ��)��
//	u8 mode :���ܵ�����ΪSD������ TD0,TD1
����ֵ��0,�޴�  �����д�
*/
s8 GetBrake_SD_Or_TD(u8 *pInData,void *pOutData)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 BCC1Even,Bcc1Odd;
	u8 BccCount=0;
		
	returnvale =	CheckBrakeGetError(pInData);
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	

		return BRAKE_GetError;
	}
	
	
	if(pInData[1] == BRAKE_SD_COMMAND)			//���ܵ�ΪSD��Ӧ
	{
		S_TMS_BRAKE_SD_Frame DataFrame;
		
		memcpy(&DataFrame,pInData,sizeof(DataFrame));  //����������ݸ��Ƹ�����
		GenerateBrakeBccChcek(&DataFrame.SD_Data,sizeof(S_TMS_BRAKE_SD_Text),&BCC1Even,&Bcc1Odd);
		
		if(BCC1Even != DataFrame.SD_BCC1EVEN )
			BccCount++;
		if(Bcc1Odd != DataFrame.SD_BCC1ODD)
			BccCount ++;
		
		if(BccCount >0)
		{
			#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
			#endif		
			return BRAKE_BCCError;
		}
	}
		
	else if(pInData[1] == BRAKE_TD_COMMAND ) 	//���ܵ�ΪTD��Ӧ
	{
		if(pInData[2] == 0x00)				//TD0  ģ����No��OOH��
		{
			S_TMS_BRAKE_TD0_Frame DataFrame;
			
			memcpy(&DataFrame,pInData,sizeof(DataFrame));  //����������ݸ��Ƹ�����
			
			GenerateBrakeBccChcek(&DataFrame.TD0_Data,sizeof(S_TMS_BRAKE_TD0_Text),&BCC1Even,&Bcc1Odd);
			
			if(BCC1Even != DataFrame.TD0_BCC1EVEN )
				BccCount++;
			if(Bcc1Odd != DataFrame.TD0_BCC1ODD)
				BccCount ++;
			
			if(BccCount >0)
			{
				#if DEBUG_RS485==1
				printf("BCCУ��ֵ������\r\n");
				#endif		
				return BRAKE_BCCError;
			}
		
		}
			
		else													//TD1	 ģ���ţ�01~40��
		{
			S_TMS_BRAKE_TD1_Frame DataFrame;
			
			memcpy(&DataFrame,pInData,sizeof(DataFrame));  //����������ݸ��Ƹ�����
			
			GenerateBrakeBccChcek(&DataFrame.TD1_Data,sizeof(S_TMS_BRAKE_TD1_Text),&BCC1Even,&Bcc1Odd);
			
			if(BCC1Even != DataFrame.TD1_BCC1EVEN )
				BccCount++;
			if(Bcc1Odd != DataFrame.TD1_BCC1ODD)
				BccCount ++;
			
			if(BccCount >0)
			{
				#if DEBUG_RS485==1
				printf("BCCУ��ֵ������\r\n");
				#endif		
				return BRAKE_BCCError;
			}
		}
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
s8 CheckBrakeGetError(u8 *pInData)
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
				return BRAKE_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return BRAKE_FrameError;
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
void GenerateBrakeBccChcek(void * pData,u8 len,u8* BCC1Even,u8* BCC1Odd)
{
	u8 i=0;
	u8 *BccData;
	u8 DataTotalLen= 0;
	DataTotalLen = len +1;
	
	BccData = mymalloc(SRAMIN,len+1);		//��̬�����ڴ� ,��keil�ﲻ��ʹ��BccData[len],����������ʾ���鳤��δ����
																			/* ����ĳ��ȱ����ݳ��ȶ�1�ֽڣ�Ϊ�˴��ETX*/	
	memcpy(BccData,pData,len);
	BccData[len] = BRAKE_ETX;
	
	
	for(i=0;i<DataTotalLen;i++)  	/*�ȴ�������ݳ��ȶ���1�ֽ�*/
	{
		if((i&0x01) ==0 )	//�ж�Ϊż����
		{
			*BCC1Even = (*BCC1Even) ^BccData[i]; 
		}
		
		else		//����Ϊ������
		{
			*BCC1Odd = (*BCC1Odd) ^BccData[i]; 
		}
	};
	myfree(SRAMIN,BccData);			//�ͷ��ڴ�
}

