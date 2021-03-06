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
extern void Switch_LED_Display(void);  //切换led显示


extern OS_TCB CAN2NetWorkMonitorTaskTCB;
extern void ConsumerHeartbeatAlarm(CO_Data* d, UNS32 id);
/*
注意: CCU1 CCU2设备无法接受到本设备 (自己) 心跳，肯定会在进入心跳错误回调函数。这里应该加入判断 ，注意注意注意！！！
*/
/*****************************************************************************/
void CAN2Master_heartbeatError(CO_Data* d, UNS8 heartbeatID)
{	
	OS_ERR err;
	//	EmcyMsgState CAN2EmcyState;			//使用局部变量，不需要使用全局变量来标识
	//	UNS8	i, j, k;									//i 表示循环变量，j用于存储can1有几个从设备没有心跳，k用于表示can2有几个从设备没有心跳
	//	UNS16 RecordErrorCoode;
  
  TIMEVAL time = ( (d->ConsumerHeartbeatEntries[heartbeatID-1]) & (UNS32)0x0000FFFF ) ;

	printf("can2 heartbeatID=%#x,time=%#llx\r\n",heartbeatID,time);
	
	if(ChooseBoard == CCU1_NODEID)
	{
		if(heartbeatID !=1)  //CCU1 不能监控节点1
		{
			g_RxCAN2HeartFlag[heartbeatID] = NoRxHeart;	
			OSTaskSemPost(&CAN2NetWorkMonitorTaskTCB,OS_OPT_POST_NONE,&err);  //释放can2网络监控任务级信号量
			
			//DelAlarm(d->ConsumerHeartBeatTimers[heartbeatID-1]);
			//SetAlarm(d, heartbeatID-1, &ConsumerHeartbeatAlarm, MS_TO_TIMEVAL(time), 0);
			
			//eprintf("CAN2Master_heartbeatError %d\r\n", heartbeatID);	
		}
	}
	
	else 	if(ChooseBoard == CCU2_NODEID)
	{
		if(heartbeatID !=2)  //CCU2 不能监控节点2
		{
			g_RxCAN2HeartFlag[heartbeatID] = NoRxHeart;	
			OSTaskSemPost(&CAN2NetWorkMonitorTaskTCB,OS_OPT_POST_NONE,&err);  //释放can2网络监控任务级信号量
			
			//DelAlarm(d->ConsumerHeartBeatTimers[heartbeatID-1]);
			//SetAlarm(d, heartbeatID-1, &ConsumerHeartbeatAlarm, MS_TO_TIMEVAL(time), 0);
			
			//eprintf("CAN2Master_heartbeatError %d\r\n", heartbeatID);	
		}
	}
	
	/*whh 之前是放在回调函数，相当于在定时器中断中去处理 can网络切换，保留，为以后调试方便*/	
	
	//	switch(heartbeatID)
	//	{
	//		case CCU1_NODEID:
	//				if(ChooseBoard == CCU2_NODEID)
	//				{
	//					CAN2EmcyState	= MainControlFaultSwitch;
	//					eprintf("CCU2 CAN2Master_heartbeatError %d\r\n", heartbeatID);
	//				}
	//			break;
	//	
	//		case CCU2_NODEID:
	//				if(ChooseBoard == CCU1_NODEID)
	//				{
	//					CAN2EmcyState	= MainControlFaultSwitch;	//主控故障可切换					
	//					eprintf("CCU1 CAN2Master_heartbeatError %d\r\n", heartbeatID);
	//				}
	//			break;
	//		
	//		case MC1_MTD_NODEID:
	//			
	//		case T_MTD_NODEID:
	//			
	//		case M_MTD_NODEID:
	//		
	//		case T1_MTD_NODEID:
	//		
	//		case T2_MTD_NODEID:
	//		
	//		case MC2_MTD_NODEID:
	//			
	//		case DDU1_NODEID:
	//		
	//		case DDU2_NODEID:
	//		
	//		//case ERM1_NODEID:

	//		//case ERM2_NODEID:
	//				CAN2EmcyState	= OnePointFaultSwitch;		//单点故障可切换，
	//				eprintf("CAN2Master_heartbeatError %d\r\n", heartbeatID);
	//				break;
	//	default :
	//			CAN2EmcyState =NoEmcy;
	//		break;
	//	}
	//		
	//		/*can2 网络 切换处理流程*/	
	//		if(CCU1 == 1)			//表示当前设备为ccu1
	//		{
	//			if(CCU1SwitchState == ChooseOperate)	//判断ccu1是否为主控
	//			{
	//				/*并且can2口设备进入到操作状态之后接受的心跳*/
	//				if(CurrentUseCAN2 == UseState) 
	//				{
	//						/*1.如果主控故障,*/
	//					if(CAN2EmcyState == MainControlFaultSwitch)
	//					{
	//							//errCode=0x41xx 单点故障，可切换		(	// can1收不到此设备的心跳，但是can2接受到了心跳,并且当前主控设备为ccu1。)
	//							if(g_RxCAN1HeartFlag[heartbeatID] == RxHeart)		
	//							{	
	//								j=0;  		//记录can1口的无心跳设备的个数	
	//								for(i=3;i<=10/*12*/;i++)	//当前屏蔽事件记录仪
	//								{
	//									if(g_RxCAN1HeartFlag[i] == NoRxHeart)
	//									{
	//										j++;
	//									}								
	//								}
	//								
	//								if(0 == j)		//表示can1线上其他设备都正常通信。则发送单点故障可切换，切换到can1线上 
	//								{
	//									#ifdef DEBUG_PRINTF
	//										debugprintf("ccu1 can1 Tx errCode=0x41xx Single point of failure, Switchable!\r\n");
	//									#endif 
	//									
	//									Switch_LED_Display();  //切换led显示
	//									CAN2EmcyState = OnePointFaultSwitch;
	//									RecordErrorCoode = (0x4100  | heartbeatID);
	//									sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);			//这里必须通过can1口发送	
	//								}
	//								return ;				//直接返回
	//							}
	//														
	//							//表示can1也没有接收到副控心跳，就发送副控坏不可切换，这样总线还能使用
	//							/*不做处理，副控坏掉，不能影响到网络通信，故不切换。(或者可以发送一个单点故障不可切换指令，或者报警)*/
	//							else
	//							{
	//									//errCode=0x61xx 主控1坏，不可切换 	
	//									#ifdef DEBUG_PRINTF
	//										debugprintf("ccu1 can1 Tx errCode=0x10xx  Slave 2 bad, Not switchable \r\n");
	//									#endif
	//								
	//									CAN2EmcyState = 	OnePointFaultNotSwitch;
	//									RecordErrorCoode = 0x1000 |	heartbeatID;                                                      //不可切换，有谁发送都可以
	//									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//									return ;
	//							}
	//					}
	//			
	//					 /*2.如果单点故障，或者主干线故障*/
	//					if(CAN2EmcyState == OnePointFaultSwitch) 
	//					{
	//							j=0;		//记录can1口的无心跳设备的个数
	//							k=0;		//记录can2口的无心跳设备的个数
	//			
	//							for(i=3;i<=10/*12*/;i++)	//当前屏蔽事件记录仪
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
	//							//errCode=0x41xx 单点故障，可切换
	//							if((k==1) && (j==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can1 Tx 0x41xx Switchable!\r\n");
	//								#endif 
	//								
	//								Switch_LED_Display();  //切换led显示
	//								CAN2EmcyState = 	OnePointFaultSwitch;
	//								RecordErrorCoode = 0x4100 | heartbeatID;
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里必须通过can1口发送
	//								return ;
	//							}
	//							//errCode=0x40xx  单点故障，不可切换 
	//							else if((k==1) && (j>1))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can1 Tx 0x40xx Not Switchable!\r\n");
	//								#endif 
	//								
	//								CAN2EmcyState = 	OnePointFaultNotSwitch;
	//								RecordErrorCoode = 0x4000 |	heartbeatID;                                                       //不可切换，有谁发送都可以
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//								return ;					
	//							}
	//							
	//							//errCode=0x51xx 主干线断故障，可切换
	//							else if((k>1) && (j==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can1 Tx 0x51xx Switchable!\r\n");
	//								#endif
	//								
	//								Switch_LED_Display();  //切换led显示
	//								CAN2EmcyState = 	MainLineFaultSwitch;
	//								RecordErrorCoode = 0x5100 |	heartbeatID;
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里必须通过can1口发送
	//								return ;						
	//							}
	//							//errCode=0x50xx 主干线断故障，不可切换
	//							else if((k>1) && (j>0) )
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu1 can1 Tx 0x50xx Not Switchable!\r\n");
	//								#endif
	//								
	//								CAN2EmcyState = 	MainControlFaultNotSwitch;
	//								RecordErrorCoode = 0x5000 |	heartbeatID;                                                       //不可切换，有谁发送都可以
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//								return ;						
	//							}
	//						
	//						}
	//				}
	//			
	//			}
	//			
	//			
	//			else		//ccu1不是主控，去监控ccu2 设备的心跳
	//			{
	//				printf("CCU1 can2 node2 Err, can1_node2=%d\r\n",g_RxCAN1HeartFlag[heartbeatID]);
	//				//1.如果主控故障,这里有ccu1 监控ccu2的心跳
	//				//当前故障为主控故障，并且can1线也接受不到主控ccu2的心跳
	//				if((CAN2EmcyState == MainControlFaultSwitch) && (g_RxCAN1HeartFlag[heartbeatID] == NoRxHeart))
	//				{
	//					j=0;		//记录can1口的无心跳设备的个数
	//					k=0;		//记录can2口的无心跳设备的个数
	//					for(i=3;i<10/*12*/;i++)
	//					{
	//						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )//计算是否能接受到其他设备的心跳
	//						{
	//							j++; 
	//						}
	//						
	//						if(g_RxCAN2HeartFlag[i] == NoRxHeart) 
	//						{
	//							k++;
	//						}
	//					}

	//					//errCode=0x60xx 主控1坏，不可切换
	//					if((j!=0 )&&(k!=0))
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x30xx Master2 Bad Not switchable!\r\n");
	//						#endif 
	//						
	//						CAN2EmcyState = 	MainControlFaultNotSwitch;
	//						//RecordErrorCoode = 0x6000 |heartbeatID;
	//						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//						
	//						RecordErrorCoode = 0x3000 |	heartbeatID;
	//						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送						
	//						return ;
	//					}

	//					//errCode=0x31xx主控1坏，可切换
	//					else if ((0==j) && (k!=0) )  //can1线非常好
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x61xx Master2 Bad Switchable!\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseOperate;
	//						CCU2SwitchState = ChooseNotOperate;
	//						CAN2EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//						
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),RecordErrorCoode,NULL,0);		//这里通过can2口发送
	//						return ;
	//					}
	//					else if((0!=j) && (0==k))	//can2线非常好
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode 0x31xx Master2 Bad Switchable!\r\n");
	//						#endif 
	//					
	//						CCU1SwitchState = ChooseOperate;
	//						CCU2SwitchState = ChooseNotOperate;
	//						CAN2EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x3100 |	heartbeatID;
	//						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//						
	//						//RecordErrorCoode = 0x6100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//					}
	//					
	//					//errCode=0x61xx主控1坏，可切换
	//					else		//两路can都非常好 默认就使用can1发送
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode  0x61xx Master2 Bad Switchable!\r\n");
	//						#endif
	//						
	//						CCU1SwitchState = ChooseOperate;
	//						CCU2SwitchState = ChooseNotOperate;
	//						CAN2EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//						
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//						return ;
	//					}
	//				}
	//			}
	//			
	//		}
	//		else		//表示使用的ccu2
	//		{
	//			if(CCU2SwitchState == ChooseOperate)		//ccu2 主控
	//			{
	//				/*并且can2口设备进入到操作状态之后接受的心跳*/
	//				if(CurrentUseCAN2 == UseState)
	//				{
	//						/*1.如果主控故障,*/
	//					if(CAN2EmcyState == MainControlFaultSwitch)
	//					{
	//							//errCode=0x41xx 单点故障，可切换		(	// can1收不到此设备的心跳，但是can2接受到了心跳,并且当前主控设备为ccu1。)
	//							if(g_RxCAN1HeartFlag[heartbeatID] == RxHeart)		
	//							{							
	//								j=0;  		//记录can1口的无心跳设备的个数	
	//								for(i=3;i<=10/*12*/;i++)	//当前屏蔽事件记录仪
	//								{
	//									if(g_RxCAN1HeartFlag[i] == NoRxHeart)
	//									{
	//										j++;
	//									}								
	//								}
	//								
	//								if(0 == j)		//表示can1线上其他设备都正常通信。则发送单点故障可切换，切换到can1线上 
	//								{
	//									#ifdef DEBUG_PRINTF
	//										debugprintf("ccu2 can1 Tx errCode=0x41xx Single point of failure, switchable!\r\n");
	//									#endif 
	//									
	//									Switch_LED_Display();  //切换led显示
	//									CAN2EmcyState = OnePointFaultSwitch;
	//									RecordErrorCoode = (0x4100  | heartbeatID);
	//									sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);			//这里必须通过can1口发送	
	//								}
	//								
	//								return ;				//直接返回
	//							}
	//														
	//							//表示can2也没有心跳
	//							/*不做处理，副控坏掉，不能影响到网络通信，故不切换。(或者可以发送一个单点故障不可切换指令，或者报警)*/
	//							else
	//							{
	//								//errCode=0x61xx 主控1坏，不可切换 	
	//									#ifdef DEBUG_PRINTF
	//										debugprintf("ccu2 can2 Tx errCode=0x10xx  Slave2  Not switch! \r\n");
	//									#endif
	//								
	//									CAN2EmcyState = 	OnePointFaultNotSwitch;
	//									RecordErrorCoode = 0x1000 |	heartbeatID;                                                       //不可切换，有谁发送都可以
	//									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送								
	//									return ;
	//							}
	//					}
	//				
	//					 /*2.如果单点故障，或者主干线故障*/
	//					if(CAN2EmcyState == OnePointFaultSwitch) 
	//					{
	//							j=0;		//记录can1口的无心跳设备的个数
	//							k=0;		//记录can2口的无心跳设备的个数					
	//							for(i=3;i<=10/*12*/;i++)	//当前屏蔽事件记录仪
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
	//							//errCode=0x41xx 单点故障，可切换
	//							if((k==1) && (j==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can1 Tx 0x41xx switch!\r\n");
	//								#endif 
	//								Switch_LED_Display();  //切换led显示
	//								CAN2EmcyState = 	OnePointFaultSwitch;
	//								RecordErrorCoode = 0x4100 | heartbeatID;
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里必须通过can1口发送
	//								return ;
	//							}
	//							//errCode=0x40xx  单点故障，不可切换 
	//							else if((k==1) && (j>1))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can1 Tx 0x40xx Not switch!\r\n");
	//								#endif 
	//								
	//								CAN2EmcyState = 	OnePointFaultNotSwitch;
	//								RecordErrorCoode = 0x4000 |	heartbeatID;                                                       //不可切换，有谁发送都可以
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//								return ;					
	//							}
	//							
	//							//errCode=0x51xx 主干线断故障，可切换
	//							else if((k>1) && (j==0))
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can1 Tx 0x51xx switch!\r\n");
	//								#endif
	//								
	//								Switch_LED_Display();  //切换led显示
	//								CAN2EmcyState = 	MainLineFaultSwitch;
	//								RecordErrorCoode = 0x5100 |	heartbeatID;
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里必须通过can1口发送
	//								return ;						
	//							}
	//							//errCode=0x50xx 主干线断故障，不可切换
	//							else if((k>1) && (j>0) )
	//							{
	//								#ifdef DEBUG_PRINTF
	//									debugprintf("ccu2 can1 Tx  0x50xx Not switch!\r\n");
	//								#endif 
	//								
	//								CAN2EmcyState = 	MainControlFaultNotSwitch;
	//								RecordErrorCoode = 0x5000 |	heartbeatID;                                                       //不可切换，有谁发送都可以
	//								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//								return ;						
	//							}
	//						
	//						}
	//				}
	//			}
	//			
	//			
	//			else		//ccu2不是主控，去监1 设备的心跳
	//			{
	//				//1.如果主控故障,这里有ccu1 监控ccu2的心跳
	//				if((CAN2EmcyState == MainControlFaultSwitch)&& (g_RxCAN1HeartFlag[heartbeatID] == NoRxHeart))
	//				{
	//					j=0;		//记录can1口的无心跳设备的个数
	//					k=0;		//记录can2口的无心跳设备的个数
	//					for(i=3;i<10/*12*/;i++)
	//					{
	//						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )		//计算是否能接受到其他设备的心跳
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
	//					//errCode=0x60xx 主控1坏，不可切换
	//					if((j!=0 )&&(k!=0))
	//					{
	//						#ifdef DEBUG_PRINTF
	//						debugprintf("Tx ErrorCode :0x30xx Master1 Not switch!\r\n");
	//						#endif 
	//						
	//						CAN2EmcyState = 	MainControlFaultNotSwitch;
	//						//RecordErrorCoode = 0x6000 |	heartbeatID;
	//						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//						
	//						RecordErrorCoode = 0x3000 |	heartbeatID;
	//						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//						return ;
	//					}
	//					
	//					//errCode=0x31xx主控1坏，可切换
	//					else if ((0==j) && (k!=0) )  //can1线非常好
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode :0x61xx Master1  switch!\r\n\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseNotOperate;
	//						CCU2SwitchState = ChooseOperate;
	//						CAN2EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//	
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//						return ;
	//					}
	//					else if((0!=j) && (0==k))	//can2线非常好
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode :0x31xx Master1  switch!\r\n\r\n");
	//						#endif 
	//						
	//						CCU1SwitchState = ChooseNotOperate;
	//						CCU2SwitchState = ChooseOperate;
	//						CAN2EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x3100 |	heartbeatID;
	//						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送
	//						
	//						//RecordErrorCoode = 0x6100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//					}
	//					
	//					//errCode=0x60xx主控1坏，可切换
	//					else		//两路can都非常好 默认就使用can1发送
	//					{
	//						#ifdef DEBUG_PRINTF
	//							debugprintf("Tx ErrorCode :0x61xx Master1 switch!\r\n\r\n");
	//						#endif
	//						
	//						CCU1SwitchState = ChooseNotOperate;
	//						CCU2SwitchState = ChooseOperate;
	//						CAN2EmcyState = 	MainControlFaultSwitch;
	//						RecordErrorCoode = 0x6100 |	heartbeatID;
	//						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//这里通过can1口发送
	//	
	//						//RecordErrorCoode = 0x3100 |	heartbeatID;
	//						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//这里通过can2口发送					
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
void CAN2Master_initialisation(CO_Data* d)
{	
	eprintf("CAN2Master_initialisation\r\n");					
}

// Step counts number of times ConfigureSlaveNode is called
//static int init_step = 0;

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
			/* Put the master in operational mode */
			setState(d, Operational);
		 
			/* Ask slave node to go in operational mode */
			masterSendNMTstateChange (d, nodeId, NMT_Start_Node);
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
	//u8 CanChangeFlag = false;

	CodeBitHig = (errCode & 0xF000)>>12;
	CodeBitLow = (UNS8)((errCode & 0x0F00)>>8);

	eprintf("Master received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\r\n", nodeID, errCode, errReg);
	
	/*whh can2口发送个紧急报文，则需要切换到can2口*/
	if(CodeBitHig == 1 || CodeBitHig == 2 )    //表示can2口发送的紧急报文，要切换到can2线
	{
		if((1 == CodeBitLow) &&  (CurrentUseCAN1 == UseState))		//当前使用的can1总线，切换到can2总线
		{
			CurrentUseCAN1 = NoUseState;
			CurrentUseCAN2 = UseState;
		}
	}

	/*whh can1口发送个紧急报文，则需要切换到can1口*/
	else if(CodeBitHig == 4 || CodeBitHig == 5 ) //表示can1口发送的紧急报文，要切换到can1线
	{
		if((1 == CodeBitLow) && (CurrentUseCAN2 == UseState))		//当前使用的can2总线，切换到can1总线
		{
			CurrentUseCAN1 = UseState;
			CurrentUseCAN2 = NoUseState;		
		}		
	}

	
	/*whh 由can2发送的主控坏*/
	else if(CodeBitHig == 3)       //can2发送的主控坏 紧急报文
	{
		 if(1 == CodeBitLow)		//主控坏，可切换		
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

			/*can2发的紧急报文 ，表示当前can2网络最好，切换到can2网络*/
			CurrentUseCAN1 = NoUseState;
			CurrentUseCAN2 = UseState;			
		 }
	}

	/*whh 由can1发送的主控坏*/
	else if(CodeBitHig == 6)    //can1发送的主控坏 紧急报文
	{
		 if(1 == CodeBitLow)		//主控坏，可切换		
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

			/*can1发的紧急报文 ，表示当前can1网络最好，切换到can1网络*/
			CurrentUseCAN1 = UseState;
			CurrentUseCAN2 = NoUseState;			
		 }											
	}
	
	/*whh 不是网络切换的紧急报文*/
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
//		eprintf("Master : Failed in changing Slave's transmit type AbortCode :%4.4x \r\n", abortCode);

//	/* Finalise last SDO transfer with this node */
//	closeSDOtransfer(&CCUCAN2Master_Data, nodeId, SDO_CLIENT);
//}


static int MasterSyncCount = 0;
void CAN2Master_post_TPDO(CO_Data* d)
{
	eprintf("CAN2Master_post_TPDO MasterSyncCount = %d \r\n", MasterSyncCount);
	MasterSyncCount++;
}

void CAN2Master_post_SlaveBootup(CO_Data* d, UNS8 nodeid)
{
	if(CCU1 == 1)			//使用的ccu1,总线can2
	{
		if(CCU1SwitchState == ChooseOperate) 	//使用的can2 通道
		{
			eprintf("CCU1 CAN2Master_post_SlaveBootup %x\r\n", nodeid);
		
			ConfigureSlaveNode(d, nodeid);
		}
		else
		{
			setState(d, Operational);  //ccu2将自己启动为操作状态
			eprintf("CCU1 CAN2 Prot is Not Opreate CAN2 BUS !\r\n");
		}
	}
	else				//使用的ccu2 ，can2总线
	{
		if( CCU2SwitchState == ChooseOperate )	//使用的can2 通道
		{
			eprintf("CCU2 CAN2Master_post_SlaveBootup %x\r\n", nodeid);
		
			ConfigureSlaveNode(d, nodeid);
		}
		else
		{
			setState(d, Operational);  //ccu2将自己启动为操作状态
			eprintf("CCU2 CAN2 Prot is Not Opreate CAN2 BUS !\r\n");
			printf("whh can2 aaa\r\n");
		}	
	}
}
