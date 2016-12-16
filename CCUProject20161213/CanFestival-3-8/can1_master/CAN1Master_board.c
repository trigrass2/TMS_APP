#include "can_stm32.h"
#include "canfestival.h"
#include "CAN1Master.h"
#include "CAN1MasterSlave.h"

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

extern void TIM4_start(void);
static int  test_master(void);

void can1master_recv_thread(void* p_arg)
{
	//	OS_ERR err;
	//	printf("can1 test!\r\n");
	//	//����һ�������ź���
	//	OSSemCreate ((OS_SEM*	)&can1recv_sem,
	//                 (CPU_CHAR*	)"can1recvsem",
	//                 (OS_SEM_CTR)0,		
	//                 (OS_ERR*	)&err);

	printf("can1init\r\n");
	canInit(CAN1,CAN_BAUD_125K);			//W H H ,���豸�õ�can1

	can1rx_save = 0;
	can1rx_read = 0;

	can1tx_save = 0;
	can1tx_read = 0;
	
	/*���� Ĭ�� ʹ��ccu1�豸,ͨ��1���������ܣ����򲻿��������� */

	CCU1SwitchState = ChooseOperate;			//ccu1ʹ��
	CCU2SwitchState = ChooseNotOperate;
	CurrentUseCAN1 = UseState;					 //ʹ��can1������ʾ״̬
	CurrentUseCAN2 = NoUseState;				//��ʹ��ccu2��ʾ����״̬

	
	printf("tes can1tmaster,start\r\n");
	test_master();		//������վ���Ժ���
	printf("test can1 master,end\r\n");

  /* Infinite loop*/
  while(1)
  {	
	//			//�����ź��� ���ȴ��ź���
	//		OSSemPend(&can1recv_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
	//		if(err != OS_ERR_NONE)
	//		{
	//			printf("can1Rx_err=%d\r\n",err);
	//		}
	
		if(g_can1RxCount>0)
		{
			uint32_t next;
			Message *pmsg;
			
			next = can1rx_read;
			pmsg = &can1rx_msg_buf[next];
			
			/*W H H can1����״̬�ĸı�*/
			{
				UNS8 nodeId = (UNS8) GET_NODE_ID((*pmsg));
				g_RxCAN1MesStateFlag[nodeId] = RxCanMesState;
				
				CAN1_Heart = RxHeart; //��ʾcan1���ܽ��ܵ������豸����Ϣ�����ʾcan1��������������
			}

			//TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);		//�ض�ʱ���ж�
			/* Disable the Interrupt sources */
			TIM4->DIER &= (uint16_t)~TIM_IT_CC1;
				canDispatch(&CCUCAN1Master_Data, pmsg);
			/* Enable the Interrupt sources */
			TIM4->DIER |= TIM_IT_CC1;
			//TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);		//����ʱ���ж�

			next++;
			if(next >= RX_BUF_LEN) next = 0;
			can1rx_read = next;
			
			//W H H  ��ʹ��ȫ�ֱ����������ź���
			g_can1RxCount--;
			//printf("can1,rece,end\r\n");
		}
		BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�		
  }
}

void can1master_send_thread(void *p_arg)
{
	//	OS_ERR err;
	//	
	//	//����һ�������ź���
	//	OSSemCreate ((OS_SEM*	)&can1tran_sem,
	//                 (CPU_CHAR*	)"can1transem",
	//                 (OS_SEM_CTR)0,		
	//                 (OS_ERR*	)&err);

	while(1)
	{
			//�����ź��� ���ȴ��ź���
			//		OSSemPend(&can1tran_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
			//		if(err != OS_ERR_NONE)
			//		{
			//			printf("can1Tx_err=%d\r\n",err);
			//		}
		
			//	printf("can1 send\r\n");

			if(g_can1TxCount>0)
			{
				uint32_t next;
				uint8_t  mailbox_no;
				CanTxMsg *ptx_msg;
				
				next = can1tx_read;
				ptx_msg = &can1tx_msg_buf[next];

				mailbox_no = CAN_Transmit(CAN1, ptx_msg);
				
				if(mailbox_no != CAN_NO_MB)
				{
					next++;
					if(next >= TX_BUF_LEN) next = 0;
					can1tx_read = next;			
					
					//W H H  ��ʹ��ȫ�ֱ����������ź���
					g_can1TxCount--;
					//printf("can1,ok!\r\n");					
				}
				else
				{
					//printf("can1 no mailbox !\r\n");
					//�����ź���
					//	OSSemPost(&can1tran_sem,OS_OPT_POST_1,&err);
				}	
			}
		BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
	}		
}



s_BOARD CAN1MasterBoard = {"1", "1M"};

/***************************  INIT  *****************************************/
static void InitNodes(CO_Data* d, UNS32 id)
{
	/****************************** INITIALISATION MASTER *******************************/
	if(strcmp(CAN1MasterBoard.baudrate, "none")){
	//		RegisterSetODentryCallBack(&CCUCAN1Master_Data, 0x2001, 0, &can1MasterGetHerat);
	// 		RegisterSetODentryCallBack(&CCUCAN1Master_Data, 0x1016, 0x01, &OnMasterGetHerat);

		/* Defining the node Id */	
		if( CCU1 == 1)		//CCU1 �豸
			setNodeId(&CCUCAN1Master_Data, 0x01);
		else						//CCU2�豸
			setNodeId(&CCUCAN1Master_Data, 0x02);
		
		/* init */
		setState(&CCUCAN1Master_Data, Initialisation);
		
			
		setState(d, Operational);  //ccu1,can2���Լ�����Ϊ����״̬	
	}
}

static TimerCallback_t init_callback;

static void StartTimerLoop(TimerCallback_t _init_callback) 
{
	init_callback = _init_callback;
	TIM4_start();
	SetAlarm(&CCUCAN1Master_Data, 0, init_callback, 0, 0);
}

///***************************  EXIT  *****************************************/
//static void Exit(CO_Data* d, UNS32 id)
//{
//	if(strcmp(CAN1MasterBoard.baudrate, "none")){
//		
//		masterSendNMTstateChange(&CCUCAN1Master_Data, 0x00, NMT_Reset_Node);    	//�����еĽڵ㸴λ
//		
//   	//Stop master
//		setState(&CCUCAN1Master_Data, Stopped);
//	}
//}

/****************************************************************************/
/****************************  test_master  *********************************/
/****************************************************************************/
static int test_master(void)
{
	if(strcmp(CAN1MasterBoard.baudrate, "none")){
		
		CCUCAN1Master_Data.canHandle = CAN1;		//W H H
		
		CCUCAN1Master_Data.heartbeatError = CAN1Master_heartbeatError;
		CCUCAN1Master_Data.initialisation = CAN1Master_initialisation;
		CCUCAN1Master_Data.preOperational = CAN1Master_preOperational;
		CCUCAN1Master_Data.operational = CAN1Master_operational;
		CCUCAN1Master_Data.stopped = CAN1Master_stopped;
		CCUCAN1Master_Data.post_sync = CAN1Master_post_sync;
		CCUCAN1Master_Data.post_TPDO = CAN1Master_post_TPDO;
		CCUCAN1Master_Data.post_emcy = CAN1Master_post_emcy;
		CCUCAN1Master_Data.post_SlaveBootup=CAN1Master_post_SlaveBootup;
	}

	// Start timer thread
	StartTimerLoop(&InitNodes);

	return 0;
}
