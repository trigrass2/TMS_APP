#include "can_stm32.h"
#include "canfestival.h"
#include "CAN3Master.h"
#include "CAN3MasterSlave.h"
#include "MTD_CPU_CAN3_MCP2515.h"
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
	CanRxMsg RxMessage;
	Message *pmsg;
	uint32_t i, next;
	
	debugprintf("can3init_500K\r\n");
	canInit(CAN3,MCP2515_CAN_BAUD_500K);			//W H H ,MTDCPU�õ�can3
	
	can3rx_save = 0;
	can3rx_read = 0;

	can3tx_save = 0;
	can3tx_read = 0;
	
	test_can3master();		//������վ���Ժ���

  /* Infinite loop*/
  while(1)
  {	
		//�����ź��� ���ȴ��ź���
		OSSemPend(&can3recv_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
		if(err)
		{
			printf("can3Rx_err=0x%x\r\n",err);
		}
		
		else /*if(err == OS_ERR_NONE)*/
						//if(g_can3RxCount>0 )
						//printf("sem3=%d\r\n",g_can3RxCount);
		//whh 9-13	 �������ݷ��뵽������				
		{
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
				//					return;				//WHH ��ѭ�������� ����
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
				
				//�ͷŽ��ܴ����ź���
				OSSemPost(&can3RxProc_sem,OS_OPT_POST_1,&err);			 /*WHH �ź����ķ��ɱ���ŵ�����������ܴ���������ٶȲſ�*/	
			}
			
			/*WHH*/
			//						printf("id3=0x%x\r\n",can3rx_msg_buf[can3rx_read].cob_id);
			//						for(i=0;i<8;i++)
			//						{
			//							printf("rx3=0x%x\r\n",can3rx_msg_buf[can3rx_read].data[i]);
			//						}
			
			OSMutexPost(&CAN3_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���

		}	
		//OSSched();		//W H  H �����������,��������󣬵���̫�죬�����ȼ��������޷�ִ�е����޷�ʹ�����л��������޵�����
		//BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
  }
}

//can3���ܴ���������
void can3RxProcthread(void *p_arg)
{
	OS_ERR err;
	uint32_t next;
	Message *pmsg;
	
	while(1)
	{
		//�����ź��� �������ȴ��ź���
		OSSemPend(&can3RxProc_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
		
		if(err)
		{
			printf("can3Rx_err=0x%x\r\n",err);
		}
		else /*if(err == OS_ERR_NONE)*/
		{
			next = can3rx_read;
			pmsg = &can3rx_msg_buf[next];
				
			//debugprintf("MC can3 id =0x%x\r\n",pmsg->cob_id);		
			
			//TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);		//�ض�ʱ���ж�
			/* Disable the Interrupt sources */
			TIM4->DIER &= (uint16_t)~TIM_IT_CC1;
				canDispatch(&MTDCAN3Master_Data, pmsg);
			/* Enable the Interrupt sources */
			TIM4->DIER |= TIM_IT_CC1;
			//TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);		//����ʱ���ж�

			next++;
			if(next >= RX_BUF_LEN) next = 0;
			can3rx_read = next;
			
			//debugprintf("can3RxProc ����\r\n");
		}
	}
}


void can3master_send_thread(void *p_arg)
{
	OS_ERR err;
	uint32_t next;
	uint8_t  mailbox_no,TxBufNum;
	CanTxMsg *ptx_msg;	
	u8 timeout=0;
	
	while(1)
	{	
		//printf("can3 sendthread\r\n");
		//�����ź��� ���ȴ��ź���
		//		OSSemPend(&can3tran_sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
		//		if(err)
		//		{
		//			printf("can3Tx_err=%d\r\n",err);
		//			printf("can3post=%d\r\n",err);
		//		}
		
		//W H H  ��ʹ��ȫ�ֱ����������ź���
		//else /*if(err == OS_ERR_NONE)*/
		
		if(g_can3TxCount >0)
		{
			next = can3tx_read;
			ptx_msg = &can3tx_msg_buf[next];
		
			OSMutexPend (&CAN3_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//���󻥳��ź���
			
			do{
					mailbox_no = MTDCPU_CAN3_MCP2515_Get_Free_TxBuf(&TxBufNum); // info = addr.
					timeout++;
					if (timeout == 0xFF)
					{		
						//printf(" 3 no mailbox!\r\n");
						break;
					}
			} while (mailbox_no == MCP2515_ALLTXBUSY);
			MTDCPU_CAN3_MCP2515_Write_CAN_Message(TxBufNum, ptx_msg);
				next++;
				if(next >= TX_BUF_LEN) next = 0;
				can3tx_read = next;	
				
				//W H H  ��ʹ��ȫ�ֱ����������ź���
				g_can3TxCount--;
			if (timeout != 0xFF)			//�������ɹ�,���Է�������
			{	
				
				//debugprintf("can3,ok!\r\n");
			}
			else					//��ʾ��û�п��Է��͵�����
			{
				 printf("can3 no mailbox !\r\n");
				//�����ź���
				//OSSemPost(&can3tran_sem,OS_OPT_POST_1,&err);
			}
		
			OSMutexPost(&CAN3_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���
		}
		BSP_OS_TimeDlyMs(2);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
	}		
}


/*can3�� �ص������Ķ���*/

/*DIӲ�߲ɼ��ص���������*/
UNS32 DICallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
		//			can1_DI_MC1[0] = can3_RxMFD1DI[0];
		//			can1_DI_MC1[1] = (can3_RxMFD1DI[1] | ((can3_RxMFD2DI[0] &0xC0)>>6));		//ȡ����2λ
		//			can1_DI_MC1[2] = ((can3_RxMFD2DI[0] &0x3E)<<2) | ((can3_RxMFD3DI[0] & 0xE0)>>5);
		//			can1_DI_MC1[3] = (can3_RxMFD3DI[0]  & 0x10)>>4;
		//		
		//			can2_DI_MC1[0] = can3_RxMFD1DI[0];
		//			can2_DI_MC1[1] = (can3_RxMFD1DI[1] | ((can3_RxMFD2DI[0] &0xC0)>>6));		//ȡ����2λ
		//			can2_DI_MC1[2] = ((can3_RxMFD2DI[0] &0x3E)<<2) | ((can3_RxMFD3DI[0] & 0xE0)>>5);
	
	
//	can2_DI_MC1[3] = (can3_RxMFD3DI[0]  & 0x10)>>4;		
//	printf("aaa\r\n");
//	printf("indx=0x%x\r\n",unsused_indextable->index);
	
//	u8 i=0;
//  printf("di=0x%x\r\n",unsused_indextable->index);
//if(d== &MTDApp1CAN1Slave_Data)
//{

//}

//else
//{
//		printf("CAN1\r\n");
//	for(i=0;i<6;i++)
//	{
//		printf("0x%x ",can1_DI_MC1[i]);
//	}
//	printf("\r\n");
//	
//	printf("CAN2\r\n");
//	for(i=0;i<6;i++)
//	{
//		printf("0x%x ",can2_DI_MC1[i]);
//	}
//	printf("\r\n");
//	
//	printf("CAN3\r\n");
//	for(i=0;i<2;i++)
//	{
//		printf("0x%x ",can3_RxMFD2DI[i]);
//	}
//	printf("\r\n");
//	
//}
	
	switch(unsused_indextable->index)		//����ӳ���ַ �������Ǹ�MFD�忨������DI����
	{
		case 0x200B:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					can1_DI_MC1[0] = can3_RxMFD1DI[0];
					can1_DI_MC1[1] = can3_RxMFD1DI[1]; 
				
					can2_DI_MC1[0] = can3_RxMFD1DI[0];
					can2_DI_MC1[1] = can3_RxMFD1DI[1]; 
				
					DI_VVVFPowertx_B1_10 = (can3_RxMFD1DI[1]>>4) & 0x01;  //MC1��VVVF��Դ��Ч
				
//					 /* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[0].last_message.cob_id = 0;
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,0);		//pdo 0x183			
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,0);		//pdo��0
				break;
				
				case MC2_MTD_NODEID:
					can1_DI_MC2[0] = can3_RxMFD1DI[0];
					can1_DI_MC2[1] = can3_RxMFD1DI[1]; 
				
					can2_DI_MC2[0] = can3_RxMFD1DI[0];
					can2_DI_MC2[1] = can3_RxMFD1DI[1]; 
				
					DI_VVVFPowertx_B1_60 = (can3_RxMFD1DI[1]>>4) & 0x01;  //MC2��VVVF��Դ��Ч
				
//					 /* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[2].last_message.cob_id = 0;			
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,2);		//pdo 0x188				
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[2].last_message.cob_id = 0;	
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,2);		//pdo��2		

				break;
				
				case M_MTD_NODEID:
					/*���ݸ�ֵ*/
					can1_DI_M = can3_RxMFD1DI[0];
				
					can2_DI_M = can3_RxMFD1DI[0];
		
//					 /* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[1].last_message.cob_id = 0;			
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,1);		//pdo 0x185
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[1].last_message.cob_id = 0;					
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,1);		//pdo��1				
				break;
			}
			break;
			
		case 0x200C:
			
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					can1_DI_MC1[2] = can3_RxMFD2DI[0];
				
					can2_DI_MC1[2] = can3_RxMFD2DI[0];
				
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[0].last_message.cob_id = 0;
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,0);		//pdo 0x183
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;						
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,0);		//pdo��0				
				break;
				
				case MC2_MTD_NODEID:
					can1_DI_MC2[2] = can3_RxMFD2DI[0];
				
					can2_DI_MC2[2] = can3_RxMFD2DI[0];	
				
//					 /* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[2].last_message.cob_id = 0;
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,2);		//pdo 0x188
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[2].last_message.cob_id = 0;		
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,2);		//pdo��2				
				break;
							
			}				
			break;
		case 0x200D:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:		
					can1_DI_MC1[3] = can3_RxMFD3DI[0];
				
					can2_DI_MC1[3] = can3_RxMFD3DI[0];
				
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[0].last_message.cob_id = 0;						
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,0);		//pdo��0	
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;						
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,0);		//pdo��0					
					break;
				
				case MC2_MTD_NODEID:				
					can1_DI_MC2[3] = can3_RxMFD3DI[0];
				
					can2_DI_MC2[3] = can3_RxMFD3DI[0];
				
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN1Slave_Data.PDO_status[2].last_message.cob_id = 0;				
//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,2);		//pdo 0x188				
//					/* force emission of PDO by artificially changing last emitted */
//					MTDApp1CAN2Slave_Data.PDO_status[2].last_message.cob_id = 0;	
//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,2);		//pdo��2			
					break;
			}
			break;
	}

	return 0;
}


/*
	can3 �Ž��ܵ����ݵĻص�����
*/
UNS32 DoorCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case	0x200E:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_Door_MC1_Odd,can3_RxDoorSta1,8);		//1,3,5,7 �ŵ�����
					memcpy(can2_Door_MC1_Odd,can3_RxDoorSta1,8);	
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[4].last_message.cob_id = 0;	
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,4);		//pdo 0x18C
					/* force emission of PDO by artificially changing last emitted */	
					MTDApp1CAN2Slave_Data.PDO_status[4].last_message.cob_id = 0;	
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,4);		//pdo��4
				
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_Door_M_Odd,can3_RxDoorSta1,8);		//1,3,5,7 �ŵ�����
					memcpy(can2_Door_M_Odd,can3_RxDoorSta1,8);	
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[5].last_message.cob_id = 0;
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,5);		//pdo 0x18E
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[5].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,5);		//pdo��5
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_Door_MC2_Odd,can3_RxDoorSta1,8);		//1,3,5,7 �ŵ�����
					memcpy(can2_Door_MC2_Odd,can3_RxDoorSta1,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[6].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,6);		//pdo 0x191
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[6].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,6);		//pdo��6
					break;
			}
		break;
			
		case 0x200F:	
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_Door_MC1_Even,can3_RxDoorSta2,8);		//2,4,6,8 �ŵ�����
					memcpy(can2_Door_MC1_Even,can3_RxDoorSta2,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[67].last_message.cob_id = 0;	
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,67);		//pdo 0x1F2
					/* force emission of PDO by artificially changing last emitted */	
					MTDApp1CAN2Slave_Data.PDO_status[67].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,67);		//pdo��67
				
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_Door_M_Even,can3_RxDoorSta2,8);		//2,4,6,8  �ŵ�����
					memcpy(can2_Door_M_Even,can3_RxDoorSta2,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[68].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,68);		//pdo 0x1F4
					/* force emission of PDO by artificially changing last emitted */	
					MTDApp1CAN2Slave_Data.PDO_status[68].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,68);		//pdo��68
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_Door_MC2_Even,can3_RxDoorSta2,8);		//2,4,6,8  �ŵ�����
					memcpy(can2_Door_MC2_Even,can3_RxDoorSta2,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[69].last_message.cob_id = 0;						
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,69);		//pdo 0x1F7
					/* force emission of PDO by artificially changing last emitted */	
					MTDApp1CAN2Slave_Data.PDO_status[69].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,69);		//pdo��69
					break;
			}			
			break;
			
		default :
			printf("Door APP1 Call Board Err!\r\n");
			break;
	}
	return 0;
}


/*can3 ���ܿռ����ݵĻص�����*/
UNS32 ACCCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case	0x2010:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ACC_MC1,can3_RxAccSta,8);		
					memcpy(can2_ACC_MC1,can3_RxAccSta,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[7].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,7);		//pdo 0x192
					/* force emission of PDO by artificially changing last emitted */	
					MTDApp1CAN2Slave_Data.PDO_status[7].last_message.cob_id = 0;						
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,7);		//pdo��7
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_ACC_M,can3_RxAccSta,8);		
					memcpy(can2_ACC_M,can3_RxAccSta,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[8].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,8);		//pdo 0x194
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[8].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,8);		//pdo��8
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ACC_MC2,can3_RxAccSta,8);		
					memcpy(can2_ACC_MC2,can3_RxAccSta,8);	

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[9].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,9);		//pdo 0x197
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[9].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,9);		//pdo��9
					break;
			
				default:
					printf("ACC APP1 Err!\r\n");
					break;
			}			
			break;
			
		case 0x2028:
				switch(ChooseBoard)
				{
					case MC1_MTD_NODEID:
						memcpy(&can1_ACC_MC1_2,can3_RxAccSta2,1);		
						memcpy(&can2_ACC_MC1_2,can3_RxAccSta2,1);	

						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[64].last_message.cob_id = 0;				
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,64);		//pdo 0x1E6
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[64].last_message.cob_id = 0;				
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,64);		//pdo��64
						break;
					
					case M_MTD_NODEID:
						memcpy(&can1_ACC_M_2,can3_RxAccSta2,1);		
						memcpy(&can2_ACC_M_2,can3_RxAccSta2,1);	

						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[65].last_message.cob_id = 0;				
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,65);		//pdo 0x1E8
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[65].last_message.cob_id = 0;				
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,65);		//pdo��65
						break;
					
					case MC2_MTD_NODEID:
						memcpy(&can1_ACC_MC2_2,can3_RxAccSta2,1);		
						memcpy(&can2_ACC_MC2_2,can3_RxAccSta2,1);	
					
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[66].last_message.cob_id = 0;				
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,66);		//pdo 0x1EB
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[66].last_message.cob_id = 0;				
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,66);		//pdo��66
						break;			

					default:
						printf("ACC APP1 Err!\r\n");
						break;
				}
				break;
				
		default:
				printf("ACC Err %#x ӳ���!\r\n",unsused_indextable->index);
				break;
	}
	return 0;
}

/*can3 ����BCU���ݵĻص�����*/
UNS32 BCUCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case 0x2011:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_BCU_MC1_1,can3_RxBCUSta1,8);
					memcpy(can2_BCU_MC1_1,can3_RxBCUSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[10].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,10);		//pdo 0x198		11
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[10].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,10);		//pdo��10
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_BCU_M_1,can3_RxBCUSta1,8);
					memcpy(can2_BCU_M_1,can3_RxBCUSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[13].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,13);		//pdo 0x19E		14
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[13].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,13);		//pdo��13					
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_BCU_MC2_1,can3_RxBCUSta1,8);
					memcpy(can2_BCU_MC2_1,can3_RxBCUSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[16].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,16);		//pdo 0x1A7		17
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[16].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,16);		//pdo��16					
					break;
			}
			break;
		
		case 0x2012:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_BCU_MC1_2,can3_RxBCUSta2,8);
					memcpy(can2_BCU_MC1_2,can3_RxBCUSta2,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[11].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,11);		//pdo 0x199		12
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[11].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,11);		//pdo��11
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_BCU_M_2,can3_RxBCUSta2,8);
					memcpy(can2_BCU_M_2,can3_RxBCUSta2,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[14].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,14);		//pdo 0x19F		15
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[14].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,14);		//pdo��14					
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_BCU_MC2_2,can3_RxBCUSta2,8);
					memcpy(can2_BCU_MC2_2,can3_RxBCUSta2,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[17].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,17);		//pdo 0x1A8		18
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[17].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,17);		//pdo��17					
					break;
			}			
			break;
			
		case 0x2013:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_BCU_MC1_3,can3_RxBCUSta3,8);
					memcpy(can2_BCU_MC1_3,can3_RxBCUSta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[12].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,12);		//pdo 0x19A		13
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[12].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,12);		//pdo��12
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_BCU_M_3,can3_RxBCUSta3,8);
					memcpy(can2_BCU_M_3,can3_RxBCUSta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[15].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,15);		//pdo 0x1A0		16
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[15].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,15);		//pdo��15					
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_BCU_MC2_3,can3_RxBCUSta3,8);
					memcpy(can2_BCU_MC2_3,can3_RxBCUSta3,8);
		
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[18].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,18);		//pdo 0x1A9		19
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[18].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,18);		//pdo��18					
					break;
			}				
			break;
			
		default:
				printf("BCU APP1 ӳ��Err!\r\n");
			break;
	}
	return 0;
}


/*can3 ����ATC���ݵĻص�����*/
UNS32 ATCCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case 0x2014:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ATC_MC1_1,can3_RxATCSta1,7);
					memcpy(can2_ATC_MC1_1,can3_RxATCSta1,7);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[19].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,19);		//pdo 0x1AA
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[19].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,19);		//pdo��19				
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ATC_MC2_1,can3_RxATCSta1,7);
					memcpy(can2_ATC_MC2_1,can3_RxATCSta1,7);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[23].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,23);		//pdo 0x1AE
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[23].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,23);		//pdo��23						
					break;
						
				default:
							printf("ATC APP1 Call �忨����!\r\n");
						break;		
			}
			break;

		case 0x2015:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ATC_MC1_2,can3_RxATCSta2,8);
					memcpy(can2_ATC_MC1_2,can3_RxATCSta2,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[20].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,20);		//pdo 0x1AB
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[20].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,20);		//pdo��20		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ATC_MC2_2,can3_RxATCSta2,8);
					memcpy(can2_ATC_MC2_2,can3_RxATCSta2,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[24].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,24);		//pdo 0x1AF
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[24].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,24);		//pdo��24							
					break;
						
				default:
							//printf("ATC APP1 Call �忨����!\r\n");
						break;		
			}
			break;

		case 0x2016:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ATC_MC1_3,can3_RxATCSta3,8);
					memcpy(can2_ATC_MC1_3,can3_RxATCSta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[21].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,21);		//pdo 0x1AC
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[21].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,21);		//pdo��21		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ATC_MC2_3,can3_RxATCSta3,8);
					memcpy(can2_ATC_MC2_3,can3_RxATCSta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[25].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,25);		//pdo 0x1B0
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[25].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,25);		//pdo��25						
					break;
						
				default:
							//printf("ATC APP1 Call �忨����!\r\n");
						break;		
			}
			break;

		case 0x2017:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ATC_MC1_4,can3_RxATCSta4,4);
					memcpy(can2_ATC_MC1_4,can3_RxATCSta4,4);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[22].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,22);		//pdo 0x1AD
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[22].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,22);		//pdo��22	
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ATC_MC2_4,can3_RxATCSta4,4);
					memcpy(can2_ATC_MC2_4,can3_RxATCSta4,4);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[26].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,26);		//pdo 0x1B0
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[26].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,26);		//pdo��26								
					break;
						
				default:
							//printf("ATC APP1 Call �忨����!\r\n");
						break;		
			}
			break;
			
		case 0x2029:		//ATCʱ��	
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_TxMC1ATO_Time,can3_RxATCTime,6);
					memcpy(can2_TxMC1ATO_Time,can3_RxATCTime,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[3].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,3);		//pdo 0x18A  3
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[3].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,3);		//pdo��26	
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_TxMC2ATO_Time,can3_RxATCTime,6);
					memcpy(can2_TxMC2ATO_Time,can3_RxATCTime,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[70].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,70);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[70].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,70);		//pdo��26					
					break;
						
				default:
							//printf("ATC APP1 Call �忨����!\r\n");
						break;				
			}
			break;
			
		default:
				printf("ATC Err ӳ���!\r\n");
				break;
					
	}
	
	return 0;
}


/*ATCʱ��ص�����*/
UNS32 ATCTimeCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(ChooseBoard)
	{
		case MC1_MTD_NODEID:
			memcpy(can1_TxMC1ATO_Time,can3_RxATCTime,6);
			memcpy(can2_TxMC1ATO_Time,can3_RxATCTime,6);
		
			MTDApp1CAN1Slave_Data.PDO_status[3].last_message.cob_id = 0;
			sendOnePDOevent(&MTDApp1CAN1Slave_Data,3);		//pdo 0x18a
			MTDApp1CAN2Slave_Data.PDO_status[3].last_message.cob_id = 0;
			sendOnePDOevent(&MTDApp1CAN2Slave_Data,3);		//pdo 0x18a
		break;
		
		case MC2_MTD_NODEID:
			memcpy(can1_TxMC2ATO_Time,can3_RxATCTime,6);
			memcpy(can2_TxMC2ATO_Time,can3_RxATCTime,6);
		
			MTDApp1CAN1Slave_Data.PDO_status[70].last_message.cob_id = 0;
			sendOnePDOevent(&MTDApp1CAN1Slave_Data,70);		//pdo 0x1F8
			MTDApp1CAN2Slave_Data.PDO_status[70].last_message.cob_id = 0;
			sendOnePDOevent(&MTDApp1CAN2Slave_Data,70);		//pdo 0x1F8
		break;
		
		default :
			printf("ATCTime APP1 ChooseBoard Err!\r\n");
			break;
		
	}
	return 0;
}

/*PIS�ص�����*/
UNS32 PISCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case 0x2018:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_PIS_MC1_1,can3_RxPISSta1,8);
					memcpy(can2_PIS_MC1_1,can3_RxPISSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[27].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,27);		//pdo 0x1B2  28
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[27].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,27);		//pdo��27	
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_PIS_MC2_1,can3_RxPISSta1,8);
					memcpy(can2_PIS_MC2_1,can3_RxPISSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[29].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,29);		//pdo 0x1B4  30
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[29].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,29);		//pdo��29					
					break;
						
				default:
							printf("PIS APP1 Call �忨����!\r\n");
						break;				
			}
			break;
		
		case 0x2019:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_PIS_MC1_2,can3_RxPISSta2,3);
					memcpy(can2_PIS_MC1_2,can3_RxPISSta2,3);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[28].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,28);		//pdo 0x1B3  29
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[28].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,28);		//pdo��28	
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_PIS_MC2_2,can3_RxPISSta2,3);
					memcpy(can2_PIS_MC2_2,can3_RxPISSta2,3);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[30].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,30);		//pdo 0x1B4  31
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[30].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,30);		//pdo��30				
					break;
						
				default:
							printf("PIS APP1 Call �忨����!\r\n");
						break;				
			}			
			break;
		
		default:
			printf("PIS APP1 ӳ��Err!\r\n");
			break;
	}		
	return 0;
}

/*ACP �ص�����*/
UNS32 ACPCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case 0x202D:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ACP_MC1_1,can3_RxACPSta1,8);
					memcpy(can2_ACP_MC1_1,can3_RxACPSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[72].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,72);		//PDO 0x1FA   73
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[72].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,72);		//pdo��72
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ACP_MC2_1,can3_RxACPSta1,8);
					memcpy(can2_ACP_MC2_1,can3_RxACPSta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[74].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,74);		//PDO 0x1FC   75
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[74].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,74);		//pdo��74					
					break;
				
				default:
					break;
			}
			break;
		
		case 0x202E:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_ACP_MC1_2,can3_RxACPSta2,3);
					memcpy(can2_ACP_MC1_2,can3_RxACPSta2,3);
	
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[73].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,73);		//PDO 0x1FB   74
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[73].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,73);		//pdo��73
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_ACP_MC2_2,can3_RxACPSta2,3);
					memcpy(can2_ACP_MC2_2,can3_RxACPSta2,3);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[75].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,75);		//PDO 0x1FD   76
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[75].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,75);		//pdo��75					
					break;
				
				default:
					break;
			}			
			break;
		
		default:
			printf("ACP APP1 ӳ��Err!\r\n");
			break;
	}
	return 0;
}

/*VVVF�ص�����*/
UNS32 VVVF1CallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	//printf("VVVF call bcak =0x%x!\r\n",unsused_indextable->index);
	switch(unsused_indextable->index)
	{
		case 0x201A:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF1_MC1_1,can3_RxVVVF1Sta1,8);
					memcpy(can2_VVVF1_MC1_1,can3_RxVVVF1Sta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[31].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,31);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[31].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,31);		//pdo��26		
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF1_M_1,can3_RxVVVF1Sta1,8);
					memcpy(can2_VVVF1_M_1,can3_RxVVVF1Sta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[41].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,41);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[41].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,41);		//pdo��26		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF1_MC2_1,can3_RxVVVF1Sta1,8);
					memcpy(can2_VVVF1_MC2_1,can3_RxVVVF1Sta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[51].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,51);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[51].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,51);		//pdo��26							
					break;
				
				default:
					printf("VVVF Board Err!\r\n");
					break;
			}
			break;
		
		case 0x201B:
			//printf("VHb=%d\r\n",can3_RxVVVF1Sta2[0]);
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF1_MC1_2,can3_RxVVVF1Sta2,6);
					memcpy(can2_VVVF1_MC1_2,can3_RxVVVF1Sta2,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[32].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,32);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[32].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,32);		//pdo��26								
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF1_M_2,can3_RxVVVF1Sta2,6);
					memcpy(can2_VVVF1_M_2,can3_RxVVVF1Sta2,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[42].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,42);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[42].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,42);		//pdo��26								
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF1_MC2_2,can3_RxVVVF1Sta2,6);
					memcpy(can2_VVVF1_MC2_2,can3_RxVVVF1Sta2,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[52].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,52);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[52].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,52);		//pdo��26								
					break;
				
				default:
					printf("VVVF Board Err!\r\n");
					break;
			}
			break;
		
		case 0x201C:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF1_MC1_3,can3_RxVVVF1Sta3,8);
					memcpy(can2_VVVF1_MC1_3,can3_RxVVVF1Sta3,8);
	
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[33].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,33);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[33].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,33);		//pdo��26		
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF1_M_3,can3_RxVVVF1Sta3,8);
					memcpy(can2_VVVF1_M_3,can3_RxVVVF1Sta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[43].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,43);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[43].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,43);		//pdo��26		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF1_MC2_3,can3_RxVVVF1Sta3,8);
					memcpy(can2_VVVF1_MC2_3,can3_RxVVVF1Sta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[53].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,53);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[53].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,53);		//pdo��26							
					break;
				
				default:
					printf("VVVF Board Err!\r\n");
					break;
			}			
			break;
		
		case 0x201D:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF1_MC1_4,can3_RxVVVF1Sta4,8);
					memcpy(can2_VVVF1_MC1_4,can3_RxVVVF1Sta4,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[34].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,34);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[34].last_message.cob_id = 0;
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,34);		//pdo��26		
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF1_M_4,can3_RxVVVF1Sta4,8);
					memcpy(can2_VVVF1_M_4,can3_RxVVVF1Sta4,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[44].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,44);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[44].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,44);		//pdo��26		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF1_MC2_4,can3_RxVVVF1Sta4,8);
					memcpy(can2_VVVF1_MC2_4,can3_RxVVVF1Sta4,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[54].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,54);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[54].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,54);		//pdo��26							
					break;
				
				default:
					printf("VVVF Board Err!\r\n");
					break;
			}			
			break;
		
		case 0x201E:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF1_MC1_5,can3_RxVVVF1Sta5,6);
					memcpy(can2_VVVF1_MC1_5,can3_RxVVVF1Sta5,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[35].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,35);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[35].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,35);		//pdo��26								
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF1_M_5,can3_RxVVVF1Sta5,6);
					memcpy(can2_VVVF1_M_5,can3_RxVVVF1Sta5,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[45].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,45);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[45].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,45);		//pdo��26								
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF1_MC2_5,can3_RxVVVF1Sta5,6);
					memcpy(can2_VVVF1_MC2_5,can3_RxVVVF1Sta5,6);
	
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[55].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,55);		//pdo��1F8  	70
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[55].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,55);		//pdo��26								
					break;
				
				default:
					printf("VVVF Board Err!\r\n");
					break;
			}			
			break;
		
		default:
			printf("VVVF1 ӳ�� Err��\r\n");
			break;
	}
	return 0;
}

/*VVVF2 �ص�����*/
UNS32 VVVF2CallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	//printf("VVVF2 call bcak =0x%x!\r\n",unsused_indextable->index);
	switch(unsused_indextable->index)
	{
		case 0x201F:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF2_MC1_1,can3_RxVVVF2Sta1,8);
					memcpy(can2_VVVF2_MC1_1,can3_RxVVVF2Sta1,8);
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[36].last_message.cob_id = 0;
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,36);		//pdo��1BB  	37
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[36].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,36);		//pdo��36		
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF2_M_1,can3_RxVVVF2Sta1,8);
					memcpy(can2_VVVF2_M_1,can3_RxVVVF2Sta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[46].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,46);		//pdo��1C5 	 47
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[46].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,46);		//pdo��46		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF2_MC2_1,can3_RxVVVF2Sta1,8);
					memcpy(can2_VVVF2_MC2_1,can3_RxVVVF2Sta1,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[56].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,56);		//pdo��1CF  	57
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[56].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,56);		//pdo��56							
					break;
				
				default:
					printf("VVVF2 Board Err!\r\n");
					break;
			}
			break;
		
		case 0x2020:
			//printf("VHb=%d\r\n",can3_RxVVVF2Sta2[0]);
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF2_MC1_2,can3_RxVVVF2Sta2,6);
					memcpy(can2_VVVF2_MC1_2,can3_RxVVVF2Sta2,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[37].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,37);		//pdo��1BC  	38
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[37].last_message.cob_id = 0;
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,37);		//pdo��37								
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF2_M_2,can3_RxVVVF2Sta2,6);
					memcpy(can2_VVVF2_M_2,can3_RxVVVF2Sta2,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[47].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,47);		//pdo��1C6  	48
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[47].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,47);		//pdo��47								
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF2_MC2_2,can3_RxVVVF2Sta2,6);
					memcpy(can2_VVVF2_MC2_2,can3_RxVVVF2Sta2,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[57].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,57);		//pdo��1D0  	58
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[57].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,57);		//pdo��57								
					break;
				
				default:
					printf("VVVF Board Err!\r\n");
					break;
			}
			break;
		
		case 0x2021:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF2_MC1_3,can3_RxVVVF2Sta3,8);
					memcpy(can2_VVVF2_MC1_3,can3_RxVVVF2Sta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[38].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,38);		//pdo��1BD  	39
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[38].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,38);		//pdo��38		
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF2_M_3,can3_RxVVVF2Sta3,8);
					memcpy(can2_VVVF2_M_3,can3_RxVVVF2Sta3,8);
	
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[48].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,48);		//pdo��1C7  	49
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[48].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,48);		//pdo��48		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF2_MC2_3,can3_RxVVVF2Sta3,8);
					memcpy(can2_VVVF2_MC2_3,can3_RxVVVF2Sta3,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[58].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,58);		//pdo��1D1  	59
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[58].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,58);		//pdo��58						
					break;
				
				default:
					printf("VVVF2 Board Err!\r\n");
					break;
			}			
			break;
		
		case 0x2022:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF2_MC1_4,can3_RxVVVF2Sta4,8);
					memcpy(can2_VVVF2_MC1_4,can3_RxVVVF2Sta4,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[38].last_message.cob_id = 0;				
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,39);		//pdo��1BE  	40
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[39].last_message.cob_id = 0;						
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,39);		//pdo��39		
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF2_M_4,can3_RxVVVF2Sta4,8);
					memcpy(can2_VVVF2_M_4,can3_RxVVVF2Sta4,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[49].last_message.cob_id = 0;						
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,49);		//pdo��1C8  	50
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[49].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,49);		//pdo��49		
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF2_MC2_4,can3_RxVVVF2Sta4,8);
					memcpy(can2_VVVF2_MC2_4,can3_RxVVVF2Sta4,8);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[59].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,59);		//pdo��1D2  	60
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[59].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,59);		//pdo��59							
					break;
				
				default:
					printf("VVVF2 Board Err!\r\n");
					break;
			}			
			break;
		
		case 0x2023:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					memcpy(can1_VVVF2_MC1_5,can3_RxVVVF2Sta5,6);
					memcpy(can2_VVVF2_MC1_5,can3_RxVVVF2Sta5,6);
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[40].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,40);		//pdo��1BF  	41
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[40].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,40);		//pdo��40								
					break;
				
				case M_MTD_NODEID:
					memcpy(can1_VVVF2_M_5,can3_RxVVVF2Sta5,6);
					memcpy(can2_VVVF2_M_5,can3_RxVVVF2Sta5,6);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[50].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,50);		//pdo��1C9  	51
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[50].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,50);		//pdo��50								
					break;
				
				case MC2_MTD_NODEID:
					memcpy(can1_VVVF2_MC2_5,can3_RxVVVF2Sta5,6);
					memcpy(can2_VVVF2_MC2_5,can3_RxVVVF2Sta5,6);
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[60].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,60);		//pdo��1D3  	61
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[60].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,60);		//pdo��60								
					break;
				
				default:
					printf("VVVF2 Board Err!\r\n");
					break;
			}			
			break;
		
		default:
			printf("VVVF2 ӳ�� Err��\r\n");
			break;
	}
	return 0;
}


/*���豸ͨ�Ź��ϻص�����*/
UNS32 SubDevFaultCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	switch(unsused_indextable->index)
	{
		case 0x202A:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������,���ݶ����ֵ�Ķ��壬��Execl�ĵ�*/
					can1_MC1SubDevFault[0] = 0;
					can1_MC1SubDevFault[1] &= 0x0F;		// ���ǰ4λ
					can2_MC1SubDevFault[0] = 0;
					can2_MC1SubDevFault[1] &= 0x0F;		//���ǰ4λ

					can1_MC1SubDevFault[0] = can3_RxMFD1SubFault[0];	
					can1_MC1SubDevFault[1] |= (can3_RxMFD1SubFault[1]&0xF0);

					/*can2*/
					can2_MC1SubDevFault[0] = can3_RxMFD1SubFault[0];	//��
					can2_MC1SubDevFault[1] |= (can3_RxMFD1SubFault[1]&0xF0);
				
					MDCU_VVVF1ComFaultx_B1_10 = (can3_RxMFD1SubFault[1]>>5)&0x01; //VVVF1 ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_10 = (can3_RxMFD1SubFault[1]>>4)&0x01; //VVVF2 ͨ��״̬
				
					MDCU_BCUComFaultrx_B1_10 = (can3_RxMFD1SubFault[1]>>6)&0x01; //BCUͨ��״̬

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[61].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,61);		//pdo 0x1DA
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[61].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,61);		//pdo��61
					break;
				
				case M_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������*/
					can1_MSubDevFault[0] = 0;
					can1_MSubDevFault[1] &= 0x0F;		// ���ǰ4λ
					can2_MSubDevFault[0] = 0;
					can2_MSubDevFault[1] &= 0x0F;		// ���ǰ4λ
					
					can1_MSubDevFault[0] = can3_RxMFD1SubFault[0];	//��
					can1_MSubDevFault[1] |= (can3_RxMFD1SubFault[1]&0xF0);

					/*can2*/
					can2_MSubDevFault[0] = can3_RxMFD1SubFault[0];	//��
					can2_MSubDevFault[1] |= (can3_RxMFD1SubFault[1]&0xF0);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[62].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,62);		//pdo 0x1DC
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[62].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,62);		//pdo��62
					break;
				
				case MC2_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������*/
					can1_MC2SubDevFault[0] = 0;
					can1_MC2SubDevFault[1] &= 0x0F;
					can2_MC2SubDevFault[0] = 0;
					can2_MC2SubDevFault[1] &= 0x0F;
			
					can1_MC2SubDevFault[0] =can3_RxMFD1SubFault[0];	//��
					can1_MC2SubDevFault[1] |=(can3_RxMFD1SubFault[1]&0xF0);
				
					/*can2*/
					can2_MC2SubDevFault[0] = can3_RxMFD1SubFault[0];	//��
					can2_MC2SubDevFault[1] |=(can3_RxMFD1SubFault[1]&0xF0);

					MDCU_VVVF1ComFaultx_B1_60 = (can3_RxMFD1SubFault[1]>>5)&0x01; //VVVF1 ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_60 = (can3_RxMFD1SubFault[1]>>4)&0x01; //VVVF2 ͨ��״̬
				
					MDCU_BCUComFaultrx_B1_60 = (can3_RxMFD1SubFault[1]>>6)&0x01; //BCUͨ��״̬
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[63].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,63);		//pdo 0x1DF
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[63].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,63);		//pdo��63
					break;
				
				default:
					printf("Sub APP1 Call �忨����!\r\n");
					break;
			}
			break;
		
		case 0x202B:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������*/
					can1_MC1SubDevFault[1] &= 0xF5;		// ���ATC  PIS
					can2_MC1SubDevFault[1] &= 0xF5;

					can1_MC1SubDevFault[1] |= (can3_RxMFD2SubFault[0]&0x0A);

					/*can2*/
					can2_MC1SubDevFault[1] |= (can3_RxMFD2SubFault[0]&0x0A);
				
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[61].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,61);		//pdo 0x1DA
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[61].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,61);		//pdo��61
					break;

				
				case MC2_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������*/
					can1_MC2SubDevFault[1] &= 0xF5;		// ���ATC  PIS
					can2_MC2SubDevFault[1] &= 0xF5;

					can1_MC2SubDevFault[1] |= (can3_RxMFD2SubFault[0]&0x0A);

					/*can2*/
					can2_MC2SubDevFault[1] |= (can3_RxMFD2SubFault[0]&0x0A);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[63].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,63);		//pdo 0x1DF
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[63].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,63);		//pdo��63
					break;
				
				default:
					printf("Sub APP1 Call �忨����!\r\n");
					break;			
			}
			break;
		
		case 0x202C:
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������*/
					can1_MC1SubDevFault[1] &= 0xFB;		// ���ACP
					can2_MC1SubDevFault[1] &= 0xFB;

					can1_MC1SubDevFault[1] |= (can3_RxMFD3SubFault[0]&0x04);

					/*can2*/
					can2_MC1SubDevFault[1] |= (can3_RxMFD3SubFault[0]&0x04);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[61].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,61);		//pdo 0x1DA
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[61].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,61);		//pdo��61
					break;

				
				case MC2_MTD_NODEID:
					/*can1*/
					/*���֮ǰ��������*/
					can1_MC2SubDevFault[1] &= 0xFB;		// ���ACP
					can2_MC2SubDevFault[1] &= 0xFB;

					can1_MC2SubDevFault[1] |= (can3_RxMFD3SubFault[0]&0x04);

					/*can2*/
					can2_MC2SubDevFault[1] |= (can3_RxMFD3SubFault[0]&0x04);

					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN1Slave_Data.PDO_status[63].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN1Slave_Data,63);		//pdo 0x1DF
					/* force emission of PDO by artificially changing last emitted */
					MTDApp1CAN2Slave_Data.PDO_status[63].last_message.cob_id = 0;					
					sendOnePDOevent(&MTDApp1CAN2Slave_Data,63);		//pdo��63
					break;
				
				default:
					printf("Sub APP1 Call �忨����!\r\n");
					break;						
			}
			break;
	}

	return 0;
}



s_BOARD CAN3MasterBoard = {"1", "500K"};
#include "iap.h"
/***************************  INIT  *****************************************/
static void InitNodes(CO_Data* d, UNS32 id)
{
	/****************************** INITIALISATION MASTER *******************************/
	if(strcmp(CAN3MasterBoard.baudrate, "none")){
		
		/*DI�ɼ���Ļص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x200B,0x02,&DICallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x200C,0x01,&DICallBack);		//whh �ص纯������ע��ע��ע��
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x200D,0x01,&DICallBack);

		/*���豸ͨ�Ź��ϻص�����*/
			//RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2027,0x02,&SubDevFaultCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x202A,0x02,&SubDevFaultCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x202B,0x01,&SubDevFaultCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x202C,0x01,&SubDevFaultCallBack);
		
		/*�����ݵĻص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x200E,0x08,&DoorCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x200F,0x08,&DoorCallBack);
		
		/*�յ��Ļص纯��*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2010,0x08,&ACCCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2028,0x01,&ACCCallBack);
		
		/*BCU�Ļص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2011,0x08,&BCUCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2012,0x08,&BCUCallBack);			
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2013,0x08,&BCUCallBack);
			
		/*ATC�ص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2014,0x07,&ATCCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2015,0x08,&ATCCallBack);			
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2016,0x08,&ATCCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2017,0x04,&ATCCallBack);
			//RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2029,0x06,&ATCCallBack);
	
			/*ATC ʱ��ص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2029,0x06,&ATCTimeCallBack);
						
			/*PIS �Ļص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2018,0x08,&PISCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2019,0x03,&PISCallBack);
			
			/*ACP �ص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x202D,0x08,&ACPCallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x202E,0x03,&ACPCallBack);		
			
			/*VVVF1�ص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x201A,0x08,&VVVF1CallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x201B,0x06,&VVVF1CallBack);			
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x201C,0x08,&VVVF1CallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x201D,0x08,&VVVF1CallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x201E,0x06,&VVVF1CallBack);
			
			/*VVVF2�ص�����*/
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x201F,0x08,&VVVF2CallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2020,0x06,&VVVF2CallBack);			
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2021,0x08,&VVVF2CallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2022,0x08,&VVVF2CallBack);
			RegisterSetODentryCallBack(&MTDCAN3Master_Data,0x2023,0x06,&VVVF2CallBack);			
			
		/* Defining the node Id */
		setNodeId(&MTDCAN3Master_Data, 0x08);			 	//�����Լ��Ľڵ�idΪ0x8
		/* init */
		setState(&MTDCAN3Master_Data, Initialisation);
		
		setState(&MTDCAN3Master_Data, Operational);  //can3���Լ�����Ϊ����״̬
		IAP_Init(&MTDApp1CAN2Slave_Data,&MTDCAN3Master_Data,MTDCPU_CanAddVal(),0x2161207);
	}
}

static TimerCallback_t init_callback;

static void StartTimerLoop(TimerCallback_t _init_callback) 
{
	init_callback = _init_callback;
	TIM4_start();
	SetAlarm(NULL, 0, init_callback, 0, 0);
}

///***************************  EXIT  *****************************************/
//static void Exit(CO_Data* d, UNS32 id)
//{
//	if(strcmp(CAN3MasterBoard.baudrate, "none")){
//		
//		masterSendNMTstateChange(&MTDCAN3Master_Data, 0x02, NMT_Reset_Node);    
//    
//   	//Stop master
//		setState(&MTDCAN3Master_Data, Stopped);
//	}
//}

/****************************************************************************/
/****************************  test_master  *********************************/
/****************************************************************************/
/*static*/ int test_can3master(void)
{
	if(strcmp(CAN3MasterBoard.baudrate, "none")){
		
		MTDCAN3Master_Data.canHandle = CAN3;			//W H H	,ʹ��ͨ��3
		
		MTDCAN3Master_Data.heartbeatError = CAN3Master_heartbeatError;
		MTDCAN3Master_Data.initialisation = CAN3Master_initialisation;
		MTDCAN3Master_Data.preOperational = CAN3Master_preOperational;
		MTDCAN3Master_Data.operational = CAN3Master_operational;
		MTDCAN3Master_Data.stopped = CAN3Master_stopped;
		MTDCAN3Master_Data.post_sync = CAN3Master_post_sync;
		MTDCAN3Master_Data.post_TPDO = CAN3Master_post_TPDO;
		MTDCAN3Master_Data.post_emcy = CAN3Master_post_emcy;
		MTDCAN3Master_Data.post_SlaveBootup=CAN3Master_post_SlaveBootup;
	}
	
	// Start timer thread
	StartTimerLoop(&InitNodes);

	return 0;
}
