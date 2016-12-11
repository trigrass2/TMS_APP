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
#include "CAN2Master.h"
#include "CAN2MasterSlave.h"
#include "Uart.h"
#include "bsp_os.h"


extern CO_Data CCUCAN1Master_Data;
extern UNS8 sendEMCY(CO_Data* d, UNS16 errCode, UNS8 errRegister, const void *Specific, UNS8 SpecificLength);

extern s_BOARD CAN2MasterBoard;

extern void Switch_LED_Display(void);  //�л�led��ʾ


/*
ע��: CCU1 CCU2�豸�޷����ܵ����豸 (�Լ�) �������϶����ڽ�����������ص�����������Ӧ�ü����ж� ��ע��ע��ע�⣡����
*/

/*****************************************************************************/
void CAN2Master_heartbeatError(CO_Data* d, UNS8 heartbeatID)
{
		/*��������*/
//	CPU_SR_ALLOC();																	//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
	
	EmcyMsgState CAN2EmcyState;			//ʹ�þֲ�����������Ҫʹ��ȫ�ֱ�������ʶ
	UNS8	i, j, k;									//i ��ʾѭ��������j���ڴ洢can1�м������豸û��������k���ڱ�ʾcan2�м������豸û������
	UNS16 RecordErrorCoode;

	g_RxCAN2HeartFlag[heartbeatID] = NoRxHeart;
			
		switch(heartbeatID)
		{
			case CCU1_NODEID:
					if(ChooseBoard == CCU2_NODEID)
					{
						CAN2EmcyState	= MainControlFaultSwitch;
						eprintf("CAN2Master_heartbeatError %d\r\n", heartbeatID);
					}
					break;
		
			case CCU2_NODEID:
					if(CCU2_NODEID == CCU1_NODEID)
					{
						CAN2EmcyState	= MainControlFaultSwitch;	//���ع��Ͽ��л�					
						eprintf("CAN2Master_heartbeatError %d\r\n", heartbeatID);
					}
					break;
			
			case MC1_MTD_NODEID:
				
			case T_MTD_NODEID:
				
			case M_MTD_NODEID:
			
			case T1_MTD_NODEID:
			
			case T2_MTD_NODEID:
			
			case MC2_MTD_NODEID:
				
			case DDU1_NODEID:
			
			case DDU2_NODEID:
			
//				case ERM1_NODEID:

//				case ERM2_NODEID:
					CAN2EmcyState	= OnePointFaultSwitch;		//������Ͽ��л���
					eprintf("CAN1Master_heartbeatError %d\r\n", heartbeatID);
					break;
		default :
				CAN2EmcyState =NoEmcy;
		}
			
//		OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����
	
		if(CCU1 == 1)			//��ʾ��ǰ�豸Ϊccu1
		{
			if(CCU1SwitchState == ChooseOperate)	//�ж�ccu1�Ƿ�Ϊ����
			{
				/*����can2���豸���뵽����״̬֮����ܵ�����*/
				if(CurrentUseCAN2 == UseState) 
				{
						/*1.������ع���,*/
					if(CAN2EmcyState == MainControlFaultSwitch)
					{
							//errCode=0x41xx ������ϣ����л�		(	// can1�ղ������豸������������can2���ܵ�������,���ҵ�ǰ�����豸Ϊccu1��)
							if(g_RxCAN1HeartFlag[heartbeatID] == RxHeart)		
							{	
								j=0;  		//��¼can1�ڵ��������豸�ĸ���	
								for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
								{
									if(g_RxCAN1HeartFlag[i] == NoRxHeart)
									{
										j++;
									}								
								}
								
								if(0 == j)		//��ʾcan1���������豸������ͨ�š����͵�����Ͽ��л����л���can1���� 
								{
									#ifdef DEBUG_PRINTF
										debugprintf("ccu1 can1 ���� errCode=0x41xx (����)������ϣ����л�\r\n");
									#endif 
									
									Switch_LED_Display();  //�л�led��ʾ
									CAN2EmcyState = OnePointFaultSwitch;
									RecordErrorCoode = (0x4100  | heartbeatID);
									sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);			//�������ͨ��can1�ڷ���	
								}
								return ;				//ֱ�ӷ���
							}
														
							//��ʾcan1Ҳû�н��յ������������ͷ��͸��ػ������л����������߻���ʹ��
							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
							else
							{
									//errCode=0x61xx ����1���������л� 	
									#ifdef DEBUG_PRINTF
										debugprintf("ccu1 can1 ���� errCode=0x61xx  ����2���������л� \r\n");
									#endif
								
									CAN2EmcyState = 	OnePointFaultNotSwitch;
									RecordErrorCoode = 0x4000 |	heartbeatID;
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
									return ;
							}
					}
			
					 /*2.���������ϣ����������߹���*/
					if(CAN2EmcyState == OnePointFaultSwitch) 
					{
							j=0;		//��¼can1�ڵ��������豸�ĸ���
							k=0;		//��¼can2�ڵ��������豸�ĸ���
			
							for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
							{
								if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )
								{
									j++; 
								}
								if(g_RxCAN2HeartFlag[i] == NoRxHeart)
								{
									k++;
								}
							}
							
							//errCode=0x41xx ������ϣ����л�
							if((k==1) && (j==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 ���� 0x41xx ���л�\r\n");
								#endif 
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	OnePointFaultSwitch;
								RecordErrorCoode = 0x4100 | heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								return ;
							}
							//errCode=0x40xx  ������ϣ������л� 
							else if((k==1) && (j>1))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 ���� 0x40xx �����л�\r\n");
								#endif 
								
								CAN2EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x4000 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								return ;					
							}
							
							//errCode=0x51xx �����߶Ϲ��ϣ����л�
							else if((k>1) && (j==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 ���� 0x51xx ���л�\r\n");
								#endif
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	MainLineFaultSwitch;
								RecordErrorCoode = 0x5100 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								return ;						
							}
							//errCode=0x50xx �����߶Ϲ��ϣ������л�
							else if((k>1) && (j>0) )
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 ���� 0x50xx �����л�\r\n");
								#endif
								
								CAN2EmcyState = 	MainControlFaultNotSwitch;
								RecordErrorCoode = 0x5000 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								return ;						
							}
						
						}
				}
			
			}
			
			
			else		//ccu1�������أ�ȥ���ccu2 �豸������
			{
				//1.������ع���,������ccu1 ���ccu2������
				//��ǰ����Ϊ���ع��ϣ�����can1��Ҳ���ܲ�������ccu2������
				if((CAN2EmcyState == MainControlFaultSwitch) && (g_RxCAN1HeartFlag[heartbeatID] == NoRxHeart))
				{
					j=0;		//��¼can1�ڵ��������豸�ĸ���
					k=0;		//��¼can2�ڵ��������豸�ĸ���
					for(i=3;i<10/*12*/;i++)
					{
						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )//�����Ƿ��ܽ��ܵ������豸������
						{
							j++; 
						}
						
						if(g_RxCAN2HeartFlag[i] == NoRxHeart) 
						{
							k++;
						}
					}
					
					//errCode=0x60xx ����1���������л�
					if((j!=0 )&&(k!=0))
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu1 can2 0x61xx ����2 �����л�\r\n");
						#endif 
						
						CAN2EmcyState = 	MainControlFaultNotSwitch;
						RecordErrorCoode = 0x6000 |heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						RecordErrorCoode = 0x3000 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���						
						return ;
					}

					//errCode=0x31xx����1�������л�
					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu1 can2 x60xx ����2�������л�\r\n");
						#endif 
						
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),RecordErrorCoode,NULL,0);		//����ͨ��can2�ڷ���
						return ;
					}
					else if((0!=j) && (0==k))	//can2�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu1 can2  x30xx ����2�������л�\r\n");
						#endif 
					
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x3100 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						
						//RecordErrorCoode = 0x6100 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
					}
					
					//errCode=0x61xx����1�������л�
					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu1 can2 0x60xx��2 ���л�\r\n");
						#endif
						
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						return ;
					}
				}
			}
			
		}
		else		//��ʾʹ�õ�ccu2
		{
			if(CCU2SwitchState == ChooseOperate)		//ccu2 ����
			{
				/*����can2���豸���뵽����״̬֮����ܵ�����*/
				if(CurrentUseCAN2 == UseState)
				{
						/*1.������ع���,*/
					if(CAN2EmcyState == MainControlFaultSwitch)
					{
							//errCode=0x41xx ������ϣ����л�		(	// can1�ղ������豸������������can2���ܵ�������,���ҵ�ǰ�����豸Ϊccu1��)
							if(g_RxCAN1HeartFlag[heartbeatID] == RxHeart)		
							{							
								j=0;  		//��¼can1�ڵ��������豸�ĸ���	
								for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
								{
									if(g_RxCAN1HeartFlag[i] == NoRxHeart)
									{
										j++;
									}								
								}
								
								if(0 == j)		//��ʾcan1���������豸������ͨ�š����͵�����Ͽ��л����л���can1���� 
								{
									#ifdef DEBUG_PRINTF
										debugprintf("ccu2 can1 ���� errCode=0x41xx (����)������ϣ����л�\r\n");
									#endif 
									
									Switch_LED_Display();  //�л�led��ʾ
									CAN2EmcyState = OnePointFaultSwitch;
									RecordErrorCoode = (0x4100  | heartbeatID);
									sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);			//�������ͨ��can1�ڷ���	
								}
								
								return ;				//ֱ�ӷ���
							}
														
							//��ʾcan2Ҳû������
							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
							else
							{
								//errCode=0x61xx ����1���������л� 	
									#ifdef DEBUG_PRINTF
										debugprintf("ccu2 can2 ���� errCode=0x40xx  ����2���������л� \r\n");
									#endif
								
									CAN2EmcyState = 	OnePointFaultNotSwitch;
									RecordErrorCoode = 0x4000 |	heartbeatID;
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���								
									return ;
							}
					}
				
					 /*2.���������ϣ����������߹���*/
					if(CAN2EmcyState == OnePointFaultSwitch) 
					{
							j=0;		//��¼can1�ڵ��������豸�ĸ���
							k=0;		//��¼can2�ڵ��������豸�ĸ���					
							for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
							{
								if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )
								{
									j++; 
								}
								if(g_RxCAN2HeartFlag[i] == NoRxHeart)
								{
									k++;
								}
							}
							
							//errCode=0x41xx ������ϣ����л�
							if((k==1) && (j==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 ���� 0x41xx ���л�\r\n");
								#endif 
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	OnePointFaultSwitch;
								RecordErrorCoode = 0x4100 | heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								return ;
							}
							//errCode=0x40xx  ������ϣ������л� 
							else if((k==1) && (j>1))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 ���� 0x40xx �����л�\r\n");
								#endif 
								
								CAN2EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x4000 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								return ;					
							}
							
							//errCode=0x51xx �����߶Ϲ��ϣ����л�
							else if((k>1) && (j==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 ���� 0x51xx ���л�\r\n");
								#endif
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	MainLineFaultSwitch;
								RecordErrorCoode = 0x5100 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								return ;						
							}
							//errCode=0x50xx �����߶Ϲ��ϣ������л�
							else if((k>1) && (j>0) )
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 ����  0x50xx �����л�\r\n");
								#endif 
								
								CAN2EmcyState = 	MainControlFaultNotSwitch;
								RecordErrorCoode = 0x5000 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								return ;						
							}
						
						}
				}
			}
			
			
			else		//ccu2�������أ�ȥ��1 �豸������
			{
				//1.������ع���,������ccu1 ���ccu2������
				if((CAN2EmcyState == MainControlFaultSwitch)&& (g_RxCAN1HeartFlag[heartbeatID] == NoRxHeart))
				{
					j=0;		//��¼can1�ڵ��������豸�ĸ���
					k=0;		//��¼can2�ڵ��������豸�ĸ���
					for(i=3;i<10/*12*/;i++)
					{
						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )		//�����Ƿ��ܽ��ܵ������豸������
						{
							j++; 
						}
						
						if(g_RxCAN2HeartFlag[i] == NoRxHeart)
						{
							k++;
						}
					}
					
					//errCode=0x60xx ����1���������л�
					if((j!=0 )&&(k!=0))
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu2 can2 0x61xx �п�1 �����л�\r\n");
						#endif 
						
						CAN2EmcyState = 	MainControlFaultNotSwitch;
						RecordErrorCoode = 0x6000 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						RecordErrorCoode = 0x3000 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						return ;
					}
					
					//errCode=0x31xx����1�������л�
					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu2 can2 x60xx ����1�������л�\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						return ;
					}
					else if((0!=j) && (0==k))	//can2�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu2 can2 x30xx ����1�������л�\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x3100 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						
						//RecordErrorCoode = 0x6100 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
					}
					
					//errCode=0x60xx����1�������л�
					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
					{
						#ifdef DEBUG_PRINTF
							debugprintf("not main ccu2 can2 0x60xx ��1 ���л�\r\n");
						#endif
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���					
						return ;
					}

				}
			}
		}
		
//		OS_CRITICAL_EXIT();	//�˳��ٽ���
//	}
}

/********************************************************
 * ConfigureSlaveNode is responsible to
 *  - setup master RPDO 1 to receive TPDO 1 from id 2
 *  - setup master RPDO 2 to receive TPDO 2 from id 2
 ********************************************************/
void CAN2Master_initialisation(CO_Data* d)
{
	
	eprintf("CAN2Master_initialisation\r\n");
	
				/*W H H ����վ�ϵ��ʼ����ʱ�򣬽���ȡ��վ������״̬��ʶ��ȫ�����ڳ�ʼ*/
	//			{						//�ڶ���ȫ�ֱ�����ʱ���Ѿ���ʼ���ˣ��ʲ���Ҫ�ڳ�ʼ������ʡʱ�䡣
	//				u8 i;

	//				for(i=0;i<sizeof(g_ReceiveHeartStateFlag);i++)
	//				{	
	//					printf("g_re=%d\r\n",sizeof(g_ReceiveHeartStateFlag));
	//					g_ReceiveHeartStateFlag[i] = InPowerInitState; 		
	//					g_RxCAN1MesStateFlag[i] = NoRxCanMesState;					
	//				}
	//			}
					
}

// Step counts number of times ConfigureSlaveNode is called
static int init_step = 0;

/*Froward declaration*/
static void ConfigureSlaveNode(CO_Data* d, UNS8 nodeId);

/**/
//static void CheckSDOAndContinue(CO_Data* d, UNS8 nodeId)
//{
//	UNS32 abortCode;	
//	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
//		eprintf("Master : Failed in initializing slave %2.2x, step %d, AbortCode :%4.4x \r\n", nodeId, init_step, abortCode);

//	/* Finalise last SDO transfer with this node */
//	closeSDOtransfer(&CCUCAN2Master_Data, nodeId, SDO_CLIENT);

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
 * This is called first by CAN2Master_post_SlaveBootup
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
		
//			g_RxCAN2HeartStateFlag[nodeId] = InOperatState;			//W H H
		
			/* Put the master in operational mode */
			setState(d, Operational);
		 
			/* Ask slave node to go in operational mode */
			masterSendNMTstateChange (d, nodeId, NMT_Start_Node);
			
//	}
}


void CAN2Master_preOperational(CO_Data* d)
{
	eprintf("CAN2Master_preOperational\r\n");
}

void CAN2Master_operational(CO_Data* d)
{
	eprintf("CAN2Master_operational\r\n");
}

void CAN2Master_stopped(CO_Data* d)
{
	eprintf("CAN2Master_stopped\r\n");
}

void CAN2Master_post_sync(CO_Data* d)
{
	
	eprintf("CAN2Master_post_sync\r\n");
}

void CAN2Master_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg)
{
	UNS8 	CodeBitHig,CodeBitLow;
	u8 CanChangeFlag = false;

	CodeBitHig = (errCode & 0xF000)>>12;
	CodeBitLow = (UNS8)((errCode & 0x0F00)>>8);

	eprintf("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\r\n", nodeID, errCode, errReg);
	
	/*whh ֮ǰ����*/
	//	if(CodeBitHig == 1 || CodeBitHig == 2 || CodeBitHig == 4 || CodeBitHig == 5 /*|| CodeBitHig == 6  || CodeBitHig == 3 */ )
	//	{
	//		if(0 == CodeBitLow)
	//		{
	//			CanChangeFlag = false;	
	//		}
	//		else
	//		{
	//			CanChangeFlag = true;
	//		}
	//	}
	
	/*whh can2�ڷ��͸��������ģ�����Ҫ�л���can2��*/
	if(CodeBitHig == 1 || CodeBitHig == 2 )    //��ʾcan2�ڷ��͵Ľ������ģ�Ҫ�л���can2��
	{
		if((1 == CodeBitLow) &&  (CurrentUseCAN1 == UseState))		//��ǰʹ�õ�can1���ߣ��л���can2����
		{
			CurrentUseCAN1 = NoUseState;
			CurrentUseCAN2 = UseState;
		}
	}

	/*whh can1�ڷ��͸��������ģ�����Ҫ�л���can1��*/
	else if(CodeBitHig == 4 || CodeBitHig == 5 ) //��ʾcan1�ڷ��͵Ľ������ģ�Ҫ�л���can1��
	{
		if((1 == CodeBitLow) && (CurrentUseCAN2 == UseState))		//��ǰʹ�õ�can2���ߣ��л���can1����
		{
			CurrentUseCAN1 = UseState;
			CurrentUseCAN2 = NoUseState;		
		}		
	}
	
	else
	{
		if(1 == CodeBitLow)		//�����л�
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
		}
		else
		{
			CanChangeFlag = false;
		}
	}
	
	//	if(CanChangeFlag)
	//	{
	//		if(CurrentUseCAN1 == UseState)		//��ǰʹ�õ�can1���ߣ��л���can2����
	//		{
	//			CurrentUseCAN1 = NoUseState;
	//			CurrentUseCAN2 = UseState;
	//		}
	//		else
	//		{
	//			CurrentUseCAN1 = UseState;
	//			CurrentUseCAN2 = NoUseState;		
	//		}
	//	}
	
}

char query_result = 0;
char waiting_answer = 0;

static void CheckSDO(CO_Data* d, UNS8 nodeId)
{
	UNS32 abortCode;	
	if(getWriteResultNetworkDict (d, nodeId, &abortCode) != SDO_FINISHED)
		eprintf("Master : Failed in changing Slave's transmit type AbortCode :%4.4x \r\n", abortCode);

	/* Finalise last SDO transfer with this node */
	closeSDOtransfer(&CCUCAN2Master_Data, nodeId, SDO_CLIENT);
}


static int MasterSyncCount = 0;
void CAN2Master_post_TPDO(CO_Data* d)
{
	eprintf("CAN2Master_post_TPDO MasterSyncCount = %d \r\n", MasterSyncCount);
	MasterSyncCount++;
}

void CAN2Master_post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
	if(CCU1 == 1)			//ʹ�õ�ccu1,����can2
	{
		if(CCU1SwitchState == ChooseOperate) 	//ʹ�õ�can2 ͨ��
		{
			eprintf("CCU1 CAN2Master_post_SlaveBootup %x\r\n", nodeid);
		
			ConfigureSlaveNode(d, nodeid);
		}
		else
		{
			setState(d, Operational);  //ccu2���Լ�����Ϊ����״̬
			eprintf("CCU1 CAN2 Prot is Not Opreate CAN2 BUS !\r\n");
		}
	}
	else				//ʹ�õ�ccu2 ��can2����
	{
		if( CCU2SwitchState == ChooseOperate )	//ʹ�õ�can2 ͨ��
		{
			eprintf("CCU2 CAN2Master_post_SlaveBootup %x\r\n", nodeid);
		
			ConfigureSlaveNode(d, nodeid);
		}
		else
		{
			setState(d, Operational);  //ccu2���Լ�����Ϊ����״̬
			eprintf("CCU2 CAN2 Prot is Not Opreate CAN2 BUS !\r\n");
			printf("whh can2 aaa\r\n");
		}	
	}
}
