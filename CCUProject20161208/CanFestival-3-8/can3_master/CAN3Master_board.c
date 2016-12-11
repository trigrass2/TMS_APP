#include "can_stm32.h"
#include "canfestival.h"
#include "CAN3Master.h"
#include "CAN2Master.h"
#include "CAN3MasterSlave.h"
#include "MTD_CCU_CAN3_MCP2515.h"
#include "objdictdef.h"			//W H H  ������ͷ�ļ�������ӣ������޷���ȡ�ڵ��id
#include "sysdep.h"
#include "os.h"
#include "stdbool.h"
#include "bsp.h"

/**
  * @brief  Main program
  * @param  None
  * @retval : None
  */
extern OS_MUTEX	CAN3_MUTEX;		//����һ�������ź���,����can3���ͽ�������Ļ���

extern void TIM4_start(void);
int  test_can3master(void);

void can3master_recv_thread(void* p_arg)
{
		OS_ERR err;
	
	printf("can3init\r\n");
	canInit(CAN3,MCP2515_CAN_BAUD_500K);			//W H H ,���豸�õ�can3
	
	can3rx_save = 0;
	can3rx_read = 0;

	can3tx_save = 0;
	can3tx_read = 0;
	
	printf("test can3 master,start\r\n");
	test_can3master();		//������վ���Ժ���
	printf("test can3 master,end\r\n");

  /* Infinite loop*/
  while(1)
  {	
			//�����ź��� ���ȴ��ź���
		OSSemPend(&can3recv_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
		if(err)
		{
			printf("can3Rx_err=0x%x\r\n",err);
		}
			else
				//if(g_can3RxCount>0 )
			{
				uint32_t next;
				Message *pmsg;

				//printf("sem3=%d\r\n",g_can3RxCount);
				
				//whh 9-13	 �������ݷ��뵽������				
				{
					CanRxMsg RxMessage;
					Message *pmsg;
					uint32_t i, next;

					OSMutexPend (&CAN3_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//���󻥳��ź���
					
					memset(&RxMessage,0,sizeof(CanRxMsg));		
					MCP2515_CAN3_Receive_Message(&RxMessage);	//W H H,���޸�Ϊ�������
					if(RxMessage.DLC >0)
					{
						next = (can3rx_save + 1) ;
						if(next >= RX_BUF_LEN) next = 0;
						//				if(next == can3rx_read)
						//				{
						//					printf("can3 err!\r\n");
						//					#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
						//						OSIntExit();    	//�˳��ж�
						//					#endif
						//					return;
						//				}
						pmsg = &can3rx_msg_buf[can3rx_save];
						pmsg->cob_id = (UNS16)RxMessage.StdId;		/**< message's ID */
						if(RxMessage.RTR == CAN_RTR_REMOTE)				/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
							pmsg->rtr = 1;		
						else
							pmsg->rtr = 0;

						pmsg->len  = (UNS8)RxMessage.DLC;					/**< message's length (0 to 8) */
						for(i = 0; i < pmsg->len; i++)
							pmsg->data[i] = RxMessage.Data[i];

						can3rx_save = next;
					}
					
					OSMutexPost(&CAN3_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���
				}
				
				
				
				next = can3rx_read;
				pmsg = &can3rx_msg_buf[next];
			
				/*W H H can1����״̬�ĸı�*/  //can3,����Ҫ����������õ�����
	//			{
	//				UNS8 nodeId = (UNS8) GET_NODE_ID((*pmsg));
	//				g_RxCAN1MesStateFlag[nodeId] = RxCanMesState;
	//			}
				
				
				//TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);		//�ض�ʱ���ж�
				/* Disable the Interrupt sources */
				TIM4->DIER &= (uint16_t)~TIM_IT_CC1;
					canDispatch(&CCUCAN3Master_Data, pmsg);
				/* Enable the Interrupt sources */
				TIM4->DIER |= TIM_IT_CC1;
				//TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);		//����ʱ���ж�

				next++;
				if(next >= RX_BUF_LEN) next = 0;
				can3rx_read = next;
				
				//g_can3RxCount--;
				//printf("can3,rece,end\r\n");

			}
		//	 OSSched();		//W H  H �����������,��������󣬵���̫�죬�����ȼ��������޷�ִ�е����޷�ʹ�����л��������޵�����
		//BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
  }
}

void can3master_send_thread(void *p_arg)
{
	OS_ERR err;
//	
//	//����һ�������ź���
//	OSSemCreate ((OS_SEM*	)&can3tran_sem,
//                 (CPU_CHAR*	)"can3transem",
//                 (OS_SEM_CTR)0,		
//                 (OS_ERR*	)&err);
	
	while(1)
	{	
//		printf("can3 sendthread\r\n");
		//�����ź��� ���ȴ��ź���
//		OSSemPend(&can3tran_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
//		if(err)
//		{
//			printf("can3Tx_err=%d\r\n",err);
//		}
	
		//W H H  ��ʹ��ȫ�ֱ����������ź���
			if(g_can3TxCount >0)
			{
				uint32_t next;
				uint8_t  mailbox_no,TxBufNum;
				CanTxMsg *ptx_msg;
				
				u8 timeout=0;
				next = can3tx_read;
				ptx_msg = &can3tx_msg_buf[next];
			
				//	debugprintf("can3 whh11\r\n");
				OSMutexPend (&CAN3_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//���󻥳��ź���
				do{
							mailbox_no = MTDCCU_CAN3_MCP2515_Get_Free_TxBuf(&TxBufNum); // info = addr.
							timeout++;
							if (timeout == 0xFF)
							{		
//								printf(" 3 no mailbox!\r\n");//ZXR
								break;
							}
				} while (mailbox_no == MCP2515_ALLTXBUSY);
				
				if (timeout != 0xFF)			//�������ɹ�,���Է�������
				{	
					//debugprintf("can3 whh22\r\n");
					MTDCCU_CAN3_MCP2515_Write_CAN_Message(TxBufNum, ptx_msg);
					//debugprintf("can3 whh33\r\n");
					next++;
					if(next >= TX_BUF_LEN) next = 0;
					can3tx_read = next;	
					
					//W H H  ��ʹ��ȫ�ֱ����������ź���
					g_can3TxCount--;
					//debugprintf("can3,ok!\r\n");
				}
				else					//��ʾ��û�п��Է��͵�����
				{
					 //printf("can3 no mailbox !\r\n");//ZXR
					//�����ź���
					//OSSemPost(&can3tran_sem,OS_OPT_POST_1,&err);
				}
			
				OSMutexPost(&CAN3_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���
			}
		BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
	}		
}


s_BOARD CAN3MasterBoard = {"1", "500K"};
/***************************  INIT  *****************************************/
static void InitNodes(CO_Data* d, UNS32 id)
{
	/****************************** INITIALISATION MASTER *******************************/
	if(strcmp(CAN3MasterBoard.baudrate, "none")){
// 		RegisterSetODentryCallBack(&CCUCAN3Master_Data, 0x2000, 0, &OnMasterMap1Update);
// 		RegisterSetODentryCallBack(&CCUCAN3Master_Data, 0x1016, 0x01, &OnMasterGetHerat);
		
		/* Defining the node Id */
		setNodeId(&CCUCAN3Master_Data, 0x08);
		/* init */
		setState(&CCUCAN3Master_Data, Initialisation);
		
		setState(&CCUCAN3Master_Data, Operational);  //can3���Լ�����Ϊ����״̬
		IAP_Init(&CCUCAN2Master_Data,&CCUCAN3Master_Data,MTDCCU_CanAddVal(),0x161207);
	}
}

static TimerCallback_t init_callback;

static void StartTimerLoop(TimerCallback_t _init_callback) 
{
	init_callback = _init_callback;
	TIM4_start();
	SetAlarm(&CCUCAN3Master_Data, 0, init_callback, 0, 0);
}

///***************************  EXIT  *****************************************/
//static void Exit(CO_Data* d, UNS32 id)
//{
//	if(strcmp(CAN3MasterBoard.baudrate, "none")){
//		
//		masterSendNMTstateChange(&CCUCAN3Master_Data, 0x02, NMT_Reset_Node);    
//    
//   	//Stop master
//		setState(&CCUCAN3Master_Data, Stopped);
//	}
//}

/****************************************************************************/
/****************************  test_master  *********************************/
/****************************************************************************/
/*static*/ int test_can3master(void)
{
	if(strcmp(CAN3MasterBoard.baudrate, "none")){
		
		CCUCAN3Master_Data.canHandle = CAN3;			//W H H	,ʹ��ͨ��3
		
		CCUCAN3Master_Data.heartbeatError = CAN3Master_heartbeatError;
		CCUCAN3Master_Data.initialisation = CAN3Master_initialisation;
		CCUCAN3Master_Data.preOperational = CAN3Master_preOperational;
		CCUCAN3Master_Data.operational = CAN3Master_operational;
		CCUCAN3Master_Data.stopped = CAN3Master_stopped;
		CCUCAN3Master_Data.post_sync = CAN3Master_post_sync;
		CCUCAN3Master_Data.post_TPDO = CAN3Master_post_TPDO;
		CCUCAN3Master_Data.post_emcy = CAN3Master_post_emcy;
		CCUCAN3Master_Data.post_SlaveBootup=CAN3Master_post_SlaveBootup;
	}
	
	// Start timer thread
	StartTimerLoop(&InitNodes);

	return 0;
}
