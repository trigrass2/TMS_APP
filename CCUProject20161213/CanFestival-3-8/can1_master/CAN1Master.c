/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <stm32f4xx.h>
#include <stdbool.h>
#include "CAN1Master.h"
#include "CAN1MasterSlave.h"
#include "Uart.h"
#include "bsp_os.h"


extern CO_Data CCUCAN2Master_Data;
extern UNS8 sendEMCY(CO_Data* d, UNS16 errCode, UNS8 errRegister, const void *Specific, UNS8 SpecificLength);

extern s_BOARD CAN1MasterBoard;

void Switch_LED_Display(void)				//��������ڷ��ͽ����źŵ�ʱ����е���
{
	if(CurrentUseCAN1 == UseState)		//��ǰʹ�õ�can1���ߣ��л���can2����
	{
		CurrentUseCAN1 = NoUseState;
		CurrentUseCAN2 = UseState;
	}
	else
	{
		CurrentUseCAN1 = UseState;
		CurrentUseCAN2 = NoUseState;		
	}
}

extern OS_TCB CAN1NetWorkMonitorTaskTCB;
extern void ConsumerHeartbeatAlarm(CO_Data* d, UNS32 id);
/*
ע��: CCU1 CCU2�豸�޷����ܵ����豸 (�Լ�) �������϶����ڽ�����������ص�����������Ӧ�ü����ж� ��ע��ע��ע�⣡����
*/
/*****************************************************************************/
void CAN1Master_heartbeatError(CO_Data* d, UNS8 heartbeatID)
{	
	OS_ERR err;
	//	EmcyMsgState CAN1EmcyState = NoEmcy;		//ʹ�þֲ�����������Ҫʹ��ȫ�ֱ�������ʶ
	//	UNS8	i, j, k;												  //i ��ʾѭ��������j���ڴ洢can1�м������豸û��������k���ڱ�ʾcan2�м������豸û������
	//	UNS16 RecordErrorCoode;
  TIMEVAL time = ( (d->ConsumerHeartbeatEntries[heartbeatID-1]) & (UNS32)0x0000FFFF ) ;
	
	printf("can1 heartbeatID=%#x,time=%#llx\r\n",heartbeatID,time);
	
	if(ChooseBoard == CCU1_NODEID)
	{
		if(heartbeatID !=1)       //CCU1 ���ܼ�ؽڵ�1
		{
			g_RxCAN1HeartFlag[heartbeatID] = NoRxHeart;	
			OSTaskSemPost(&CAN1NetWorkMonitorTaskTCB,OS_OPT_POST_NONE,&err);   //�ͷ�can1�����������ź���
			
			//DelAlarm(d->ConsumerHeartBeatTimers[heartbeatID-1]);
			//SetAlarm(d, heartbeatID-1, &ConsumerHeartbeatAlarm, MS_TO_TIMEVAL(time), 0);
			
			//eprintf("CAN1Master_heartbeatError %d\r\n", heartbeatID);
		}
	}

	else if(ChooseBoard == CCU2_NODEID)
	{
		if(heartbeatID !=2)       //CCU2 ���ܼ�ؽڵ�2
		{
			g_RxCAN1HeartFlag[heartbeatID] = NoRxHeart;	
			OSTaskSemPost(&CAN1NetWorkMonitorTaskTCB,OS_OPT_POST_NONE,&err);   //�ͷ�can1�����������ź���
			
			//DelAlarm(d->ConsumerHeartBeatTimers[heartbeatID-1]);
			//SetAlarm(d, heartbeatID-1, &ConsumerHeartbeatAlarm, MS_TO_TIMEVAL(time), 0);
			
			//eprintf("CAN1Master_heartbeatError %d\r\n", heartbeatID);
		}
	}
	
	/*whh ֮ǰ�Ƿ��ڻص��������൱���ڶ�ʱ���ж���ȥ���� can�����л���������Ϊ�Ժ���Է���*/	
	//	switch(heartbeatID)
	//	{
	//		case CCU1_NODEID:
	//				if(ChooseBoard == CCU2_NODEID)
	//				{
	//					CAN1EmcyState	= MainControlFaultSwitch;
	//					eprintf("CCU2 CAN1Master_heartbeatError %d\r\n", heartbeatID);
	//				}
	//			break;
	//	
	//		case CCU2_NODEID:
	//				if(ChooseBoard == CCU1_NODEID)
	//				{
	//					CAN1EmcyState	= MainControlFaultSwitch;	//���ع��Ͽ��л�
	//					eprintf("CCU1 CAN1Master_heartbeatError %d\r\n", heartbeatID);
	//				}
	//			break;
	//		
	//		case MC1_MTD_NODEID:

	//		case T_MTD_NODEID:

	//		case M_MTD_NODEID:

	//		case T1_MTD_NODEID:
	//		
	//		case T2_MTD_NODEID:

	//		case MC2_MTD_NODEID:

	//		case DDU1_NODEID:

	//		case DDU2_NODEID:
	//	
	//		//case ERM1_NODEID:

	//		//case ERM2_NODEID:
	//				CAN1EmcyState	= OnePointFaultSwitch;		//������Ͽ��л���
	//				eprintf("CAN1Master_heartbeatError %d\r\n", heartbeatID);
	//			break;
	//		
	//		default :
	//				CAN1EmcyState =NoEmcy;
	//			break;
	//			
	//	}

	//		/*can1 ���� �л���������*/
	//		if(CCU1 == 1)			//��ʾ��ǰ�豸Ϊccu1
	//		{
	//			if(CCU1SwitchState == ChooseOperate)	//�ж�ccu1�Ƿ�Ϊ����
	//			{
	//				/*����can2���豸���뵽����״̬֮����ܵ�����*/
	//				if (CurrentUseCAN1 == UseState)	//�ж�can1���Ƿ�Ϊ����
	//				{
	//					/*1.������ع���,*/
	//					if(CAN1EmcyState == MainControlFaultSwitch) 	//˵�����ܵ�Ϊ������������
	//					{
	//							//errCode=0x11xx ������ϣ����л�		(	// can1�ղ������豸������������can2���ܵ�������,���ҵ�ǰ�����豸Ϊccu1��)
	//							if(g_RxCAN2HeartFlag[heartbeatID] == RxHeart)		
	//							{
	//								k=0;		//��¼can2�ڵ��������豸�ĸ���	
	//								for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
	//								{
	//									if(g_RxCAN2HeartFlag[i] == NoRxHeart)
	//									{
	//										k++;
	//									}								
	//								}
	//								if(0==k)	//��ʾcan2���������豸������ͨ�š����͵�����Ͽ��л����л���can2���� 
	//								{
	//									#ifdef DEBUG_PRINTF
	//										debugprintf("ccu1 can2 Tx errCode=0x11xx Single point of failure, switchable!\r\n");
	//									#endif 
	//									
	//									Switch_LED_Display();  //�л�led��ʾ
	//									CAN1EmcyState = OnePointFaultSwitch;
	//									RecordErrorCoode = (0x1100  | heartbeatID);
	//									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���									
	//								}
	//								
	//								return ;				//ֱ�ӷ���
	//							}
	//														
	//							//��ʾcan2Ҳû�н��յ������������ͷ��͸��ػ������л����������߻���ʹ��
	//							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
	//							else
	//							{
	//								#ifdef DEBUG_PRINTF	
	//									debugprintf("ccu1 can2 Tx errCode=0x40xx  Slave2 Bad��Not Switch!a \r\n");
	//								#endif 
	//							
	//								CAN1EmcyState = 	OnePointFaultNotSwitch;
	//								RecordErrorCoode = 0x4000 |	heartbeatID;
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���	

	//								return ;
	//							}
	//					}
	//				
	//					 /*2.���������ϣ����������߹���*/
	//					if(CAN1EmcyState == OnePointFaultSwitch) 
	//					{
	//							j=0;		//��¼can1�ڵ��������豸�ĸ���
	//							k=0;		//��¼can2�ڵ��������豸�ĸ���
	//								
	//							for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
	//							{
	//								if(g_RxCAN1HeartFlag[i]	==  NoRxHeart ) 
	//								{
	//									j++;  
	//								}
	//								if(g_RxCAN2HeartFlag[i] == NoRxHeart)
	//								{
	//									k++;
	//								}
	//							}
	//							
	//							//errCode=0x11xx ������ϣ����л�
	//							if((j==1) && (k==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can2 Tx errCode=0x11xx Single point of failure, switchable!\r\n");
	//								#endif 
	//								
	//								Switch_LED_Display();  //�л�led��ʾ
	//								CAN1EmcyState = 	OnePointFaultSwitch;
	//								RecordErrorCoode = 0x1100 | heartbeatID;
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
	//								return ;
	//							}
	//							
	//							//errCode=0x10xx  ������ϣ������л� 
	//							else if((j==1) && (k>1))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can2 Tx errCode=0x10xx  Single point of failure, Not switchable\r\n");
	//								#endif 
	//								
	//								CAN1EmcyState = 	OnePointFaultNotSwitch;
	//								RecordErrorCoode = 0x1000 |	heartbeatID;                                                //�����л�����˭���Ͷ�����
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//								return ;					
	//							}
	//							
	//							//errCode=0x21xx �����߶Ϲ��ϣ����л�
	//							else if((j>1) && (k==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can2 Tx errCode=0x21xx Trunk line fault, switch!\r\n");
	//								#endif
	//			
	//								Switch_LED_Display();  //�л�led��ʾ
	//								CAN1EmcyState = 	MainLineFaultSwitch;
	//								RecordErrorCoode = 0x2100 |	heartbeatID;
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
	//								return ;						
	//							}
	//							
	//							//errCode=0x20xx �����߶Ϲ��ϣ������л�
	//							else if((j>1) && (k>0) )
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can2 Tx 0x20xx Trunk line fault,Not switch!\r\n");
	//								#endif
	//								
	//								CAN1EmcyState = 	MainControlFaultNotSwitch;
	//								RecordErrorCoode = 0x2000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//								return ;						
	//							}
	//						
	//						}
	//				}
	//			
	//			}
	//			
	//			
	//			else		//ccu1�������أ�ȥ���ccu2 �豸������
	//			{
	//				printf("CCU1 can1 node2 Err, can2_node2=%d\r\n",g_RxCAN2HeartFlag[heartbeatID]);
	//				//1.������ع���,������ccu1 ���ccu2������
	//				//��ǰ����Ϊ���ع��ϣ�����can2��Ҳ���ܲ�������ccu2������
	//				if((CAN1EmcyState == MainControlFaultSwitch) && (g_RxCAN2HeartFlag[heartbeatID] == NoRxHeart))
	//				{
	//					j=0;		//��¼can1�ڵ��������豸�ĸ���
	//					k=0;		//��¼can2�ڵ��������豸�ĸ���
	//					for(i=3;i<10/*12*/;i++)
	//					{
	//						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart ) //�����Ƿ��ܽ��ܵ������豸������
	//						{
	//							j++;  
	//						}
	//						
	//						if(g_RxCAN2HeartFlag[i] == NoRxHeart)
	//						{
	//							k++;
	//						}
	//					}
	//					
	//					//errCode=0x30xx ����2���������л�
	//					if((j!=0 )&&(k!=0))
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x60xx Master2 Bad��Not Switch!\r\n");
	//						#endif 
	//						
	//						CAN1EmcyState = 	MainControlFaultNotSwitch;
	//						RecordErrorCoode = 0x6000 |heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//						
	//						//RecordErrorCoode = 0x3000 |	heartbeatID;					
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//						return ;
	//					}
	//					
	//					//errCode=0x30xx����2�������л�
	//					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x61xx Master2 Bad�� Switch!\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseOperate;
	//						CCU2SwitchState = ChooseNotOperate;
	//						CAN1EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//						
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//						return ;
	//					}
	//					else if((0!=j) && (0==k))	//can2�߷ǳ���
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x31xx Master2 Bad�� Switch!\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseOperate;
	//						CCU2SwitchState = ChooseNotOperate;
	//						CAN1EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x3100 |	heartbeatID;
	//						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//						
	//						//RecordErrorCoode = 0x6100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//						return ;
	//					}
	//					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x61xx Master2 Bad�� Switch!\r\n");
	//						#endif
	//						CCU1SwitchState = ChooseOperate;
	//						CCU2SwitchState = ChooseNotOperate;
	//						CAN1EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//					
	//						//	RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//	sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//						return ;
	//					}
	//				}
	//			}
	//			
	//		}
	//		else		//��ʾʹ�õ�ccu2
	//		{
	//			if(CCU2SwitchState == ChooseOperate)		//ccu2 ����
	//			{
	//				/*����can2���豸���뵽����״̬֮����ܵ�����*/
	//				if (CurrentUseCAN1 == UseState)
	//				{
	//						/*1.������ع���,*/
	//					if(CAN1EmcyState == MainControlFaultSwitch)
	//					{
	//							//errCode=0x11xx ������ϣ����л�		(	// can1�ղ������豸������������can2���ܵ�������,���ҵ�ǰ�����豸Ϊccu1��)
	//							if(g_RxCAN2HeartFlag[heartbeatID] == RxHeart)		
	//							{							
	//								k=0;		//��¼can2�ڵ��������豸�ĸ���	
	//								for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
	//								{
	//									if(g_RxCAN2HeartFlag[i] == NoRxHeart)
	//									{
	//										k++;
	//									}								
	//								}
	//								if(0==k)	//��ʾcan2���������豸������ͨ�š����͵�����Ͽ��л����л���can2���� 
	//								{
	//									#ifdef DEBUG_PRINTF
	//										debugprintf("ccu2 can2 Tx errCode=0x11xx Single point of failure, switchable!\r\n");
	//									#endif 
	//									
	//									Switch_LED_Display();  //�л�led��ʾ
	//									CAN1EmcyState = OnePointFaultSwitch;
	//									RecordErrorCoode = (0x1100  | heartbeatID);
	//									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���									
	//								}
	//								return ;				//ֱ�ӷ���
	//							}
	//														
	//							//��ʾcan2Ҳû�н��յ������������ͷ��͸��ػ������л����������߻���ʹ��
	//							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
	//							else
	//							{							
	//									#ifdef DEBUG_PRINTF	
	//										debugprintf("ccu1 can2 Tx errCode=0x10xx Slave2 Bad��Not Switch! \r\n");
	//									#endif 
	//								
	//									CAN1EmcyState = 	OnePointFaultNotSwitch;
	//									RecordErrorCoode = 0x1000 |	heartbeatID;                                                    	//�����л�����˭���Ͷ�����
	//									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���								
	//									
	//									return ;
	//							}
	//					}
	//			
	//					 /*2.���������ϣ����������߹���*/
	//					if(CAN1EmcyState == OnePointFaultSwitch) 
	//					{
	//							j=0;		//��¼can1�ڵ��������豸�ĸ���
	//							k=0;		//��¼can2�ڵ��������豸�ĸ���
	//							
	//							for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
	//							{
	//								if(g_RxCAN1HeartFlag[i]	==  NoRxHeart ) 
	//								{
	//									j++;  
	//								}
	//								if(g_RxCAN2HeartFlag[i] == NoRxHeart) 
	//								{
	//									k++;
	//								}
	//							}
	//							
	//							//errCode=0x11xx ������ϣ����л�
	//							if((j==1) && (k==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can2 Tx 0x11xx Switch\r\n");
	//								#endif
	//								Switch_LED_Display();  //�л�led��ʾ
	//								CAN1EmcyState = 	OnePointFaultSwitch;
	//								RecordErrorCoode = 0x1100 | heartbeatID;
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
	//								return ;
	//							}
	//							//errCode=0x10xx  ������ϣ������л� 
	//							else if((j==1) && (k>1))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can2 Tx 0x10xx Not Switch!\r\n");
	//								#endif
	//								
	//								CAN1EmcyState = 	OnePointFaultNotSwitch;
	//								RecordErrorCoode = 0x1000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//								return ;					
	//							}
	//							
	//							//errCode=0x21xx �����߶Ϲ��ϣ����л�
	//							else if((j>1) && (k==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can2 Tx 0x21xx Switch!\r\n");
	//								#endif 
	//								
	//								Switch_LED_Display();  //�л�led��ʾ
	//								CAN1EmcyState = 	MainLineFaultSwitch;
	//								RecordErrorCoode = 0x2100 |	heartbeatID;                                                       
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
	//								return ;						
	//							}
	//							//errCode=0x20xx �����߶Ϲ��ϣ������л�
	//							else if((j>1) && (k>0) )
	//							{	
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can2 Tx 0x20xx Switch!\r\n");
	//								#endif
	//								
	//								CAN1EmcyState = 	MainControlFaultNotSwitch;
	//								RecordErrorCoode = 0x2000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
	//								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//								return ;						
	//							}
	//				
	//					}
	//				}
	//			
	//			}
	//			
	//			
	//			else		//ccu2�������أ�ȥ��1 �豸������
	//			{
	//				//printf("whh ccu2 is not master!\r\n");
	//				//1.������ع���,������ccu1 ���ccu2������
	//				//��ǰ����Ϊ���ع��ϣ�����can2��Ҳ���ܲ�������ccu2������
	//				if((CAN1EmcyState == MainControlFaultSwitch) && (g_RxCAN2HeartFlag[heartbeatID] == NoRxHeart) )
	//				{
	//					j=0;		//��¼can1�ڵ��������豸�ĸ���
	//					k=0;		//��¼can2�ڵ��������豸�ĸ���
	//					for(i=3;i<10/*12*/;i++)
	//					{
	//						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )	//�����Ƿ��ܽ��ܵ������豸������
	//						{
	//							j++;  
	//						}
	//						
	//						if(g_RxCAN2HeartFlag[i] == NoRxHeart)
	//						{
	//							k++;
	//						}
	//					}
	//					
	//					//errCode=0x31xx ����1���������л�
	//					if((j!=0 )&&(k!=0))
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x60xx Master1 Not Switch!\r\n");
	//						#endif 
	//						
	//						CAN1EmcyState = 	MainControlFaultNotSwitch;
	//						RecordErrorCoode = 0x6000 |heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���						
	//						
	//						//RecordErrorCoode = 0x3000 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���						                                                                                                                 
	//						return ;
	//					}
	//					
	//					//errCode=0x30xx����1�������л�
	//					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x61xx Master1  Switch!\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseNotOperate;
	//						CCU2SwitchState = ChooseOperate;
	//						CAN1EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//						
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//						return ;
	//					}
	//					else if((0!=j) && (0==k))	//can2�߷ǳ���
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x31xx Master1  Switch!\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseNotOperate;
	//						CCU2SwitchState = ChooseOperate;
	//						CAN1EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x3100 |	heartbeatID;
	//						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
	//						
	//						//RecordErrorCoode = 0x6100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//					}
	//					//errCode=0x30xx����1�������л�
	//					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x61xx Master1  Switch!\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseNotOperate;
	//						CCU2SwitchState = ChooseOperate;
	//						CAN1EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	//						
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���					
	//						return ;
	//					}
	//				}
	//			}
	//		}
}
/********************************************************
 * ConfigureSlaveNode is responsible to
 *  - setup master RPDO 1 to receive TPDO 1 from id 2
 *  - setup master RPDO 2 to receive TPDO 2 from id 2
 ********************************************************/
void CAN1Master_initialisation(CO_Data* d)
{
	eprintf("CAN1Master_initialisation\r\n");		
}

/*Froward declaration*/
static void ConfigureSlaveNode(CO_Data* d, UNS8 nodeId);

///**/
//static void CheckSDOAndContinue(CO_Data* d, UNS8 nodeId)
//{
//	UNS32 abortCode;	
//	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
//		eprintf("Master : Failed in initializing slave %2.2x, step %d, AbortCode :%4.4lx \r\n", nodeId, init_step, abortCode);

//	/* Finalise last SDO transfer with this node */
//	closeSDOtransfer(&CCUCAN1Master_Data, nodeId, SDO_CLIENT);

//	ConfigureSlaveNode(d, nodeId);
//}

/********************************************************
 * ConfigureSlaveNode is responsible to
 *  - setup slave TPDO 1 transmit type
 *  - setup slave TPDO 2 transmit type
 *  - switch to operational mode
 *  - send NMT to slave
 ********************************************************
 * This an example of :
 * Network Dictionary Access (SDO) with Callback 
 * Slave node state change request (NMT) 
 ********************************************************
 * This is called first by CAN1Master_post_SlaveBootup
 * then it called again each time a SDO exchange is
 * finished.
 ********************************************************/
 
static void ConfigureSlaveNode(CO_Data* d, UNS8 nodeId)
{
	/* Master configure heartbeat producer time at 1000 ms 
	 * for slave node-id 0x02 by DCF concise */
	 
//	UNS8 Transmission_Type = 0x01;
//	UNS8 res;
	eprintf("Master : ConfigureSlaveNode %2.2x\r\n", nodeId);

//	switch(++init_step){
//		case 1: /*First step : setup Slave's TPDO 1 to be transmitted on SYNC*/
//			eprintf("Master : set slave %2.2x TPDO 1 transmit type\r\n", nodeId);
//			res = writeNetworkDictCallBack (d, /*CO_Data* d*/
//					nodeId, /*UNS8 nodeId*/
//					0x1800, /*UNS16 index*/
//					0x02, /*UNS8 subindex*/
//					1, /*UNS8 count*/
//					0, /*UNS8 dataType*/
//					&Transmission_Type,/*void *data*/
//					CheckSDOAndContinue, /*SDOCallback_t Callback*/
//                    0); /* use block mode */
//					break;
//		
//		case 2:	/*Second step*/
//			eprintf("Master : set slave %2.2x TPDO 2 transmit type\r\n", nodeId);
//			writeNetworkDictCallBack (d, /*CO_Data* d*/
//					nodeId, /*UNS8 nodeId*/
//					0x1801, /*UNS16 index*/
//					0x02, /*UNS16 index*/
//					1, /*UNS8 count*/
//					0, /*UNS8 dataType*/
//					&Transmission_Type,/*void *data*/
//					CheckSDOAndContinue, /*SDOCallback_t Callback*/
//                    0); /* use block mode */
//					break;
//		case 1: 
		
		/****************************** START *******************************/
		
			/* Put the master in operational mode */
			setState(d, Operational);
		 
			/* Ask slave node to go in operational mode */
			masterSendNMTstateChange (d, nodeId, NMT_Start_Node);
}


void CAN1Master_preOperational(CO_Data* d)
{
	eprintf("CAN1Master_preOperational\r\n");
}

void CAN1Master_operational(CO_Data* d)
{
	eprintf("CAN1Master_operational\r\n");
}

void CAN1Master_stopped(CO_Data* d)
{
	eprintf("CAN1Master_stopped\r\n");
}

void CAN1Master_post_sync(CO_Data* d)
{
	
	eprintf("CAN1Master_post_sync\r\n");
}

void CAN1Master_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg)
{
	UNS8 	CodeBitHig,CodeBitLow;
	//u8 CanChangeFlag = false;

	CodeBitHig = (errCode & 0xF000)>>12;
	CodeBitLow = (UNS8)((errCode & 0x0F00)>>8);

	eprintf("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\r\n", nodeID, errCode, errReg);
	
	/*whh ��can2�ڷ��͵Ľ������ģ�����Ҫ�л���can2��*/
	if(CodeBitHig == 1 || CodeBitHig == 2 )    //��ʾcan2�ڷ��͵Ľ������ģ�Ҫ�л���can2��
	{
		if((1 == CodeBitLow) && (CurrentUseCAN1 == UseState))		//��ǰʹ�õ�can1���ߣ�������Ҫ�л���can2����
		{
			CurrentUseCAN1 = NoUseState;
			CurrentUseCAN2 = UseState;
		}
	}

	/*whh ��can1�ڷ��͵Ľ������ģ�����Ҫ�л���can1��*/
	else if(CodeBitHig == 4 || CodeBitHig == 5 ) //��ʾcan1�ڷ��͵Ľ������ģ�Ҫ�л���can1��
	{
		if((1 == CodeBitLow) && (CurrentUseCAN2 == UseState))		//��ǰʹ�õ�can2���ߣ�������Ҫ�л���can1����
		{
			CurrentUseCAN1 = UseState;
			CurrentUseCAN2 = NoUseState;		
		}		
	}
	
	/*whh ��can2���͵����ػ�*/
	else if(CodeBitHig == 3)       //can2���͵����ػ� ��������
	{
		 if(1 == CodeBitLow)		//���ػ������л�		
		 {
			if(CCU1SwitchState == ChooseOperate)
			{
				CCU1SwitchState = ChooseNotOperate;
				CCU2SwitchState = ChooseOperate;
			}
			else
			{
				CCU1SwitchState = ChooseOperate;
				CCU2SwitchState = ChooseNotOperate;				
			} 

			/*can2���Ľ������� ����ʾ��ǰcan2������ã��л���can2����*/
			CurrentUseCAN1 = NoUseState;
			CurrentUseCAN2 = UseState;			
		 }
	}

	/*whh ��can1���͵����ػ�*/
	else if(CodeBitHig == 6)    //can1���͵����ػ� ��������
	{
		 if(1 == CodeBitLow)		//���ػ������л�		
		 {
			if(CCU1SwitchState == ChooseOperate)
			{
				CCU1SwitchState = ChooseNotOperate;
				CCU2SwitchState = ChooseOperate;
			}
			else
			{
				CCU1SwitchState = ChooseOperate;
				CCU2SwitchState = ChooseNotOperate;				
			} 

			/*can1���Ľ������� ����ʾ��ǰcan1������ã��л���can1����*/
			CurrentUseCAN1 = UseState;
			CurrentUseCAN2 = NoUseState;			
		 }											
	}
	
	/*whh ���������л��Ľ�������*/
	else
	{
		printf("Emcy is not Can Switch! ErrorCode: %4.4x ",errCode);
	}
		
}

//char query_result = 0;
//char waiting_answer = 0;

//static void CheckSDO(CO_Data* d, UNS8 nodeId)
//{
//	UNS32 abortCode;	
//	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
//		eprintf("Master : Failed in changing Slave's transmit type AbortCode :%4.4lx \r\n", abortCode);

//	/* Finalise last SDO transfer with this node */
//	closeSDOtransfer(&CCUCAN1Master_Data, nodeId, SDO_CLIENT);
//}


static int MasterSyncCount = 0;
void CAN1Master_post_TPDO(CO_Data* d)
{
	eprintf("CAN1Master_post_TPDO MasterSyncCount = %d \r\n", MasterSyncCount);
	MasterSyncCount++;
}

void CAN1Master_post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
	if(CCU1 ==1 )						// ��ʾCCU1�豸,��can1ͨ��
	{
		if(CCU1SwitchState == ChooseOperate)	//��ʾʹ��CCU1
		{
			eprintf("CCU1 CAN1Master_post_SlaveBootup %x\r\n", nodeid);
		
			ConfigureSlaveNode(d, nodeid);
		}
		else
		{
			setState(d, Operational);  //ccu1���Լ�����Ϊ����״̬
			eprintf("CCU1 CAN1 Prot is Not Opreate CAN1 BUS !\r\n");
		}
	}
	
	else		//��ʾʹ��ccu2,��can1ͨ��
	{
		if(CCU2SwitchState == ChooseOperate) 	//��ʾʹ��CCU2
		{
			eprintf("CCU2 CAN1Master_post_SlaveBootup %x\r\n", nodeid);
		
			ConfigureSlaveNode(d, nodeid);
		}
		else
		{
			setState(d, Operational);  //ccu2���Լ�����Ϊ����״̬
			eprintf("CCU2 CAN1 Prot is Not Opreate CAN1 BUS !\r\n");
		}
	}
}
