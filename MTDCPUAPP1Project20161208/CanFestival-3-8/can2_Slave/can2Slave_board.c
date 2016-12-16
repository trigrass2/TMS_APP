#include "can_stm32.h"
#include "canfestival.h"
#include "can2Slave.h"
#include "can1MasterSlave.h"
#include "objdictdef.h"		//W H H  ������ͷ�ļ�������ӣ������޷���ȡ�ڵ��id
#include "sysdep.h"
#include "os.h"
#include "delay.h"
#include "MTD_CPU_CANLED.h"
#include "stdbool.h"
#include "bsp.h"
#include "can12CallBack.h"

/*can2 led��˸��*/
extern OS_FLAG_GRP CAN2LedEventFlag;

extern void TIM4_start(void);
extern void TIM4_stop(void);
int  can2test_slave(void);

extern u8 can1CarNumberFlag;
extern u8 can2CarNumberFlag;
extern u8 can1UpdateTimeFlag ;
extern u8 can2UpdateTimeFlag;

void can2slave_recv_thread(void* p_arg)
{
	OS_ERR err;
	uint32_t next;
	Message *pmsg;
	
//	printf("slave test!\r\n");
//	//����һ�������ź���
//	OSSemCreate ((OS_SEM*	)&can2slaverecv_sem,
//                 (CPU_CHAR*	)"can2slaverecvsem",
//                 (OS_SEM_CTR)0,		
//                 (OS_ERR*	)&err);

	printf("CAN2int\r\n");
	canInit(CAN2,CAN_BAUD_125K);			//W H H ,���豸�õ�can2

	/* transmit queue init */
	can2slavetx_save = 0;
	can2slavetx_read = 0;
	/* receive  queue init */
	can2slaverx_save = 0;
	can2slaverx_read = 0;
	
	CurrentUseCAN1 = UseState;					 //ʹ��can1������ʾ״̬
	CurrentUseCAN2 = NoUseState;				//��ʹ��ccu2��ʾ����״̬
		
	printf("can2slave,start\r\n");
	//	delay_ms(100);			//��ֹ�ӽڵ�����ڵ�������
	can2test_slave();
	printf("can2slave,end\r\n");

  /* Infinite loop*/
  while(1)
  {	
		//�����ź��� ���ȴ��ź���
//		OSSemPend(&can2slaverecv_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
//		
//		if(err)
//		{
//			printf("sla_err=%d\r\n",err);
//		}
		if(g_can2RxCount>0)
		{
			next = can2slaverx_read;
			pmsg = &can2slaverx_msg_buf[next];

//			/*W H H can2����״̬�ĸı�*/
//			{
//				UNS8 nodeId = (UNS8) GET_NODE_ID((*pmsg));
//				g_RxCAN2MesStateFlag[nodeId] = RxCanMesState;
//			}
			
			/* Disable the Interrupt sources */
			TIM4->DIER &= (uint16_t)~TIM_IT_CC1;
				canDispatch(&MTDApp1CAN2Slave_Data, pmsg);		//W H H 
			/* Enable the Interrupt sources */
			TIM4->DIER |= TIM_IT_CC1;

			next++;
			if(next >= RX_BUF_LEN) next = 0;
			can2slaverx_read = next;
				
			g_can2RxCount--;	
				//				printf("can2,rece,end\r\n");
				
			//MTD_CPU_CANLED_Turn(CAN2_RX_LED);			//can2����ledָʾ��
				
			//���¼���־��CAN2LedEventFlag���ͱ�־
			OSFlagPost((OS_FLAG_GRP*)&CAN2LedEventFlag,
								 (OS_FLAGS	  )CAN2RxLED_FLAG,
								 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
					       (OS_ERR*	    )&err);
		}
		//OSSched();		//W H  H �����������,�����������,��������󣬵���̫�죬�����ȼ��������޷�ִ�е����޷�ʹ�����л��������޵�����
		BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
  }
}

void can2slave_send_thread(void *p_arg)
{
	OS_ERR err;
	uint32_t next;
	uint8_t  mailbox_no;
	CanTxMsg *ptx_msg;
				
//	//����һ�������ź���
//	OSSemCreate ((OS_SEM*	)&can2slavetran_sem,
//                 (CPU_CHAR*	)"can2slavetransem",
//                 (OS_SEM_CTR)0,		
//                 (OS_ERR*	)&err);
	
	while(1)
	{
//		//�����ź��� ���ȴ��ź���
//		OSSemPend(&can2slavetran_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
		
		if(g_can2TxCount>0)
		{
			next = can2slavetx_read;
			ptx_msg = &can2slavetx_msg_buf[next];

			mailbox_no = CAN_Transmit(CAN2, ptx_msg);		//can2����
			next++;
				if(next >= TX_BUF_LEN) next = 0;
					can2slavetx_read = next;		

				g_can2TxCount--;
			if(mailbox_no != CAN_NO_MB)
			{
				
				//	printf("can2,ok!\r\n");			
				//	printf("MC can2 Tx id=0x%x\r\n",ptx_msg->StdId);	
				
				//���¼���־��CAN2LedEventFlag���ͱ�־
				OSFlagPost((OS_FLAG_GRP*)&CAN2LedEventFlag,
									 (OS_FLAGS	  )CAN2TxLED_FLAG,
									 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
									 (OS_ERR*	    )&err);				
			}
			else
			{
				//	printf("can2 no mailbox !\r\n");
				//�����ź���
				//OSSemPost(&can2slavetran_sem,OS_OPT_POST_1,&err);
			} 

		//			printf("slave send ok,end!\r\n");
		}
		BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
	}		
}



UNS32 can2SlaveNumberUpdate(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
//	printf("bbbbbbbbbbbbbbbbbbbbb%d  %d\r\n",can2_Car_Number_1,can2_Car_Number_2);
//	if(CurrentUseCAN2 == UseState)
//	{
//		switch(can2_Car_Number_1)
//		{
//			case 0x01:
//				if(ChooseBoard == MC1_MTD_NODEID)
//					can2CarNumberFlag=true;
//				break;
//			case 0x02:
//				if(ChooseBoard == T_MTD_NODEID)
//					can2CarNumberFlag=true;
//				break;		
//			case 0x03:
//				if(ChooseBoard == M_MTD_NODEID)
//					can2CarNumberFlag=true;
//				break;	
//			case 0x04:
//				if(ChooseBoard == T1_MTD_NODEID)
//					can2CarNumberFlag=true;
//				break;	
//			case 0x05:
//				if(ChooseBoard == T2_MTD_NODEID)
//					can2CarNumberFlag=true;
//				break;	
//			case 0x06:
//				if(ChooseBoard == MC2_MTD_NODEID)
//					can2CarNumberFlag=true;
//				break;				
//		}
//	}
	return 0;
}


UNS32 can2SlaveDataUpdate(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
//	printf("can2 ddddddddddddd %d %d %d %d %d   %d\r\n",can2_Car_DataTime[0],can2_Car_DataTime[1],can2_Car_DataTime[2],can2_Car_DataTime[3],can2_Car_DataTime[4],can2_Car_DataTime[5]);
//	if((CurrentUseCAN2 == UseState) && (can2_Car_DataTime[0]!=0) )
//	{
//		can2UpdateTimeFlag=true;
//	}
	return 0 ;
}


s_BOARD CAN2SlaveBoard  = {"0", "1M"};

/***************************  INIT  *****************************************/
static void InitNodes(CO_Data* d, UNS32 id)
{
	/****************************** INITIALISATION SLAVE *******************************/
	if(strcmp(CAN2SlaveBoard.baudrate, "none")) {

		/*ע��ص�����*/
		
		/*���ñ���ʱ��*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2008, 7, &SetMTDTimeCallBack);
		
		/*���ܿյ��¶�*/
		//RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2060, 1, &SetAccTempCallBack);
		
		/*����HMI���͵Ĳ�������*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x205E, 4, &SetCarNoCallBack);		//���ó������		

		/*����HMI���͵�ATOģʽ �Ȳ�����PDO��0x189*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2006, 8, &Can3SnedProDataCallBack);
		
		/*����HMI���͵�PDO 0x1E3,ӳ��0x2060 �յ��¶�����*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2060, 1, &Can3SnedACCSdrCallBack);
		
		/*����HMI����ɾ����¼ָ�PDO 0x1ED,ӳ��0x206A */
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x206A, 3, &Can3SnedDelCmdCallBack);		
		
		/*����HMI���͵ĵ������ϼ�¼ָ��*/
		
		/*����HIM����PIS��SdrЭ�����ݣ�PDO 0x1FE,0x1FF,ӳ��0x207D,0x207E */
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x207D, 2, &Can3SendPISSdrCallBack);
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x207E, 8, &Can3SendPISSdrCallBack);		
		
		/*����MCDU ���͵�DIӲ������*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2000, 2, &MDCURxATCDICallBack);
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2002, 2, &MDCURxATCDICallBack);
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2005, 2, &MDCURxATCDICallBack);
		
		/*����MDCU ���͵�ͨ��״̬����*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2057, 2, &MDCURxATCSbuFaultCallBack);  
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2058, 2, &MDCURxATCSbuFaultCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2059, 2, &MDCURxATCSbuFaultCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x205A, 2, &MDCURxATCSbuFaultCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x205B, 2, &MDCURxATCSbuFaultCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x205C, 2, &MDCURxATCSbuFaultCallBack);   

		/*����MDCU ���͵�VVVFЭ������*/ 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2033, 2, &MDCURxATCVVVFProCallBack);   //MC1 VVVF1
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2034, 5, &MDCURxATCVVVFProCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2038, 2, &MDCURxATCVVVFProCallBack);   //MC1 VVVF2
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2039, 5, &MDCURxATCVVVFProCallBack); 		
		                                      
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x203D, 2, &MDCURxATCVVVFProCallBack);   //M VVVF1
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x203E, 5, &MDCURxATCVVVFProCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2042, 2, &MDCURxATCVVVFProCallBack);   //M VVVF2
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2043, 5, &MDCURxATCVVVFProCallBack); 	
                                          
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2047, 2, &MDCURxATCVVVFProCallBack);   //MC2 VVVF1
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2048, 5, &MDCURxATCVVVFProCallBack); 
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x204C, 2, &MDCURxATCVVVFProCallBack);   //MC2 VVVF2
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x204D, 5, &MDCURxATCVVVFProCallBack); 	

		/*����MDCU ���͵�BCUЭ������*/
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2015, 3, &MDCURxATCBCUProCallBack);   	//MC1 BCU
		RegisterSetODentryCallBack(&MTDApp1CAN2Slave_Data, 0x2024, 3, &MDCURxATCBCUProCallBack); 		//MC2 BCU	
		
		switch(ChooseBoard)
		{
			case MC1_MTD_NODEID:
				setNodeId(&MTDApp1CAN2Slave_Data,MC1_MTD_NODEID);
				break;
			case T_MTD_NODEID:
				setNodeId(&MTDApp1CAN2Slave_Data,T_MTD_NODEID);
				break;
			case M_MTD_NODEID:
				setNodeId(&MTDApp1CAN2Slave_Data,M_MTD_NODEID);
				break;
			case T1_MTD_NODEID:
				setNodeId(&MTDApp1CAN2Slave_Data,T1_MTD_NODEID);
				break;
			case T2_MTD_NODEID:
				setNodeId(&MTDApp1CAN2Slave_Data,T2_MTD_NODEID);
				break;
			case MC2_MTD_NODEID:
				setNodeId(&MTDApp1CAN2Slave_Data,MC2_MTD_NODEID);
				break;
			default :
				break;
		}
		/* init */
		setState(&MTDApp1CAN2Slave_Data, Initialisation);
	}
}

static TimerCallback_t init_callback;

static void  StartTimerLoop(TimerCallback_t _init_callback) 
{
	init_callback = _init_callback;
	TIM4_start();
	SetAlarm(NULL, 0, init_callback, 0, 0);	
}


///***************************  EXIT  *****************************************/
//static void Exit(CO_Data* d, UNS32 id)
//{
//	if(strcmp(CAN2SlaveBoard.baudrate, "none")){
//		 
//   //Stop slave
//		setState(&MTDApp1CAN2Slave_Data, Stopped);
//	}
//}


//#include "Tim.h"
/****************************************************************************/
/*****************************  test_salve  *********************************/
/****************************************************************************/
 int can2test_slave(void)
{
	if(strcmp(CAN2SlaveBoard.baudrate, "none")){
		
		MTDApp1CAN2Slave_Data.canHandle = CAN2;		//W H H
		
		MTDApp1CAN2Slave_Data.heartbeatError = CAN2Slave_heartbeatError;
		MTDApp1CAN2Slave_Data.initialisation = CAN2Slave_initialisation;
		MTDApp1CAN2Slave_Data.preOperational = CAN2Slave_preOperational;
		MTDApp1CAN2Slave_Data.operational = CAN2Slave_operational;
		MTDApp1CAN2Slave_Data.stopped   = CAN2Slave_stopped;
		MTDApp1CAN2Slave_Data.post_sync = CAN2Slave_post_sync;
		MTDApp1CAN2Slave_Data.post_TPDO = CAN2Slave_post_TPDO;
		MTDApp1CAN2Slave_Data.storeODSubIndex = CAN2Slave_storeODSubIndex;
		MTDApp1CAN2Slave_Data.post_emcy = CAN2Slave_post_emcy;
	}
	// Start timer thread
	StartTimerLoop(&InitNodes);

	/*������ʱ��3����վ�ĵ�ǰ״̬�����û�н������״̬�������������ڵ�*/
	//	TIM3_Int_Init(20000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����10000��Ϊ1000ms
		
	return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/

