#include "BCUComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "MTD_MFD_CurrentLoop.h"
#include "main.h"
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

	/*��������*/
	switch(mode)
	{
		case SEND_SDR:
			{
				BcuSdrFrame SdrDataFrame;		//��������		
				
				SdrDataFrame.SdrStx = BCU_STX;
				memcpy(&SdrDataFrame.BcuSdrData,pInData,sizeof(BcuSdrText));	 //�����͵��ı���Ϣȫ������������֡
				SdrDataFrame.SdrEtx = BCU_ETX;
			
				GenerateBcuBccChcek((u8*)pInData,sizeof(BcuSdrText),&BCC1,&BCC2);	//����BCC
				
				SdrDataFrame.SdrBCC1 = BCC1;
				SdrDataFrame.SdrBCC2 = BCC2;
				//memcpy(pOutData,&SdrDataFrame,sizeof(BcuSdrFrame));
				
				//printf("BCUsize=%d\r\n",sizeof(BcuSdrFrame));
				MTDMFD_CL_Send_Data((u8 *)&SdrDataFrame,sizeof(BcuSdrFrame));	//ֱ�ӵ��õ��������ͺ���
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
				MTDMFD_CL_Send_Data((u8 *)&TdrDataFrame,sizeof(BcuTdrFrame));	//ֱ�ӵ��õ��������ͺ���
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
s8 GetBcuSdOrTd(u8 *pInData,void *pOutData,u8 mode,u8 GetNum)
{
	s8 returnvale=0;			//��¼����ֵ
	u8 BCC1,BCC2;
	u8 BccCount;
		
	if(GetNum == GET_NUM1)
	{
		returnvale =	CheckBcuGetError(pInData,mode);			//��һ�λ������
	}
	else 
	{
		returnvale = SecondCheckBcuGetError(pInData,mode);	//��ʾ�ڶ��λ������
	}
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("BCU Get SDorTD Error��\r\n");
		#endif	
		return returnvale;
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
		#if DEBUG_RS485==0
		printf("BCU Get SdOrTD Data Error!\r\n ");
		#endif
		return BCU_GetError;
	}
	return 0;
}

/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckBcuGetError(u8 *pInData,u8 mode)
{
	static u16 Uart4OldCount=0;	//�ϵĴ��ڽ�������ֵ
	u8 i=0;
	//1.����SDR �󣬾���15ms ������û�������յ�SD ʱ,����
	
	//1.��Ӧ��ʱ����һ��(TM1),�����־�������ܵ�����
	for(i=0;i<15;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_Uart4RxStartFlag == 1)
		{
			g_Uart4RxStartFlag =0;
			break;
		}	
		
		else
			delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ����   (�����������ʱ���൱�ڴ򿪶�ʱ����ֱ��ִ��������䣬�϶�������Ҫ��)
	}
	if(i>=15)
	{
		if(g_Uart4RxStartFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("BCU 15ms��û�н��ܵ�����\r\n");
			#endif
			return BCU_GetTIM1OutError;
		}
	}	
	
	//2.��ʾ���ܵ�NACK��Ϣ
	if(g_Uart4NackFlag ==1)
	{
		g_Uart4NackFlag = 0;
		
			#if DEBUG_RS485==1
				printf("BCU NACK Error!\r\n");
			#endif
		return BCU_NACKError;
	}
	
	else if(g_Uart4RxDataFlag == 1)			//��ʾ����������ɣ������ж�����else����
	{
		g_Uart4RxDataFlag = 0;
	}
	
	//3.�����ַ�֮�䳬ʱ
	//4.������Ϣ����
	else  //��ʾ����������Ϣ
	{
		Uart4OldCount = USART4_CL_RxLength;		//�����ڽ������ݳ��ȸ�ֵ��Uart4OledCount;
		
		if(mode == GET_SD)			//��ʾ��ȡ��ΪSD,���ҽ����ֽ�С��54ʱ
		{
			while(Uart4OldCount<54)	//ѭ���ж�����
			{
				for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
				{
					if(USART4_CL_RxLength > Uart4OldCount)
					{
						break;
					}		
					else
						delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
				}
				if(i>=10)
				{
					#if DEBUG_RS485==1
						printf("BCU 10ms��û����һ�ֽ�����!\r\n");
					#endif
					return BCU_GetTIM4OutError;
				}
				
				Uart4OldCount = USART4_CL_RxLength;		//�����ڽ������ݳ��ȸ�ֵ��Uart4OledCount;
			}
			
			g_Uart4RxDataFlag = 0;  //ִ�е�������������Ѿ���ɣ����������ɱ�־
		}
		
		else if(mode == GET_TD)			//��ʾ��ȡ��ΪTD,���ҽ����ֽ�С��56ʱ
		{
			while(Uart4OldCount<56)	//ѭ���ж�����  
			{
				for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
				{
					if(USART4_CL_RxLength > Uart4OldCount)
					{
						break;
					}		
					else
						delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
				}
				if(i>=10)
				{
					#if DEBUG_RS485==1
						printf("BCU 10ms��û����һ�ֽ�����!\r\n");
					#endif
					return BCU_GetTIM4OutError;
				}
				
				Uart4OldCount = USART4_CL_RxLength;	//�����ڽ������ݳ��ȸ�ֵ��Uart4OledCount;
			}
			
			g_Uart4RxDataFlag = 0;  //ִ�е�������������Ѿ���ɣ����������ɱ�־
		}	

		else
		{
			printf("BCU Get Error!\r\n");
		}
	}

				
	//3.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart4FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		g_Uart4FrameErrorFlag =0; //�����ż�����־
		
		#if DEBUG_RS485==1
			printf("BCU ��⵽֡������żУ�����\r\n");
		#endif
		return BCU_FrameError;
	}
	//û�д������
	return 0;
}


/*
	�ڶ��μ��BCU����
	������BCUͨ�Ž��ܴ����ʱ����NCK���ٴεȴ���������
*/
s8 SecondCheckBcuGetError(u8 *pInData,u8 mode)
{
	static u16 Uart4OldCount=0;	//�ϵĴ��ڽ�������ֵ
	u8	IsNoACK = 0;
	u8 i=0;
	//��Ҫ�ȷ���NACK����ʾ���ܳ��� ,Ȼ�����½�������
	IsNoACK = BCU_NACK;
	MTDMFD_CL_Send_Data(&IsNoACK,1);
	
	
	//1.��Ӧ��ʱ���ڶ���(TM2),�����־�������ܵ�����
	for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
	{
		if(g_Uart4RxStartFlag == 1)
		{
			g_Uart4RxStartFlag =0;
			break;
		}	
		
		else
			delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ����   (�����������ʱ���൱�ڴ򿪶�ʱ����ֱ��ִ��������䣬�϶�������Ҫ��)
	}
	if(i>=10)		
	{
		if(g_Uart4RxStartFlag == 0)	//˵��û�н��ܵ�����
		{
			#if DEBUG_RS485==1
				printf("BCU 15ms��û�н��ܵ�����\r\n");
			#endif
			return BCU_GetTIM1OutError;
		}
	}	


	//2.�����ַ�֮�䳬ʱ
	//3.������Ϣ����
	
	if(g_Uart4RxDataFlag == 1)	//��ʾ����������ɣ������ж�����else����
	{
		g_Uart4RxDataFlag = 0;
	}
	
	else  //��ʾ����������Ϣ
	{
		Uart4OldCount = USART4_CL_RxLength;		//�����ڽ������ݳ��ȸ�ֵ��Uart4OledCount;
		
		if(mode == GET_SD)			//��ʾ��ȡ��ΪSD,���ҽ����ֽ�С��54ʱ
		{
			while(Uart4OldCount<54)	//ѭ���ж�����
			{
				for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
				{
					if(USART4_CL_RxLength > Uart4OldCount)
					{
						break;
					}		
					else
						delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
				}
				if(i>=10)
				{
					#if DEBUG_RS485==1
						printf("BCU 10ms��û����һ�ֽ�����!\r\n");
					#endif
					return BCU_GetTIM4OutError;
				}
				
				Uart4OldCount = USART4_CL_RxLength;		//�����ڽ������ݳ��ȸ�ֵ��Uart4OledCount;
			}
			
			g_Uart4RxDataFlag = 0;  //ִ�е�������������Ѿ���ɣ����������ɱ�־
		}
		
		else if(mode == GET_TD)			//��ʾ��ȡ��ΪTD,���ҽ����ֽ�С��56ʱ
		{
			while(Uart4OldCount<56)	//ѭ���ж�����  
			{
				for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
				{
					if(USART4_CL_RxLength > Uart4OldCount)
					{
						break;
					}		
					else
						delay_ms(1);//��ʱ1ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
				}
				if(i>=10)
				{
					#if DEBUG_RS485==1
						printf("BCU 10ms��û����һ�ֽ�����!\r\n");
					#endif
					return BCU_GetTIM4OutError;
				}
				
				Uart4OldCount = USART4_CL_RxLength;	//�����ڽ������ݳ��ȸ�ֵ��Uart4OledCount;
			}
			
			g_Uart4RxDataFlag = 0;  //ִ�е�������������Ѿ���ɣ����������ɱ�־
		}	

		else
		{
			printf("BCU Get Error!\r\n");
		}
	}

				
	//3.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_Uart4FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		g_Uart4FrameErrorFlag =0; //�����ż�����־
		
		#if DEBUG_RS485==1
			printf("BCU ��⵽֡������żУ�����\r\n");
		#endif
		return BCU_FrameError;
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
	/*copy ����*/
	memcpy(BccData,pData,len);
	BccData[len] = BCU_ETX;
	*BCC1 = BccData[0];		//����ʼֵ
	*BCC2 = BccData[1];
	
	for(i=2;i<DataTotalLen;i++)  	/*�ȴ�������ݳ��ȶ���1�ֽڣ���ʼֵ�Ѿ����ˣ��ʴ�2��ʼ*/
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


