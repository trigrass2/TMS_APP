/**
  ******************************************************************************
  * @file    can_stm32.c
  * @author  Zhenglin R&D Driver Software Team
  * @version V1.0.0
  * @date    26/04/2015
  * @brief   This file is can_stm32 file.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "can_stm32.h"
#include "canfestival.h"


//#include "SEGGER_RTT.h"
//#include "GlobalVar.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#include <applicfg.h>
#include "os.h"
//extern OS_SEM  	recv_sem;		
//extern OS_SEM 	tran_sem;
extern OS_SEM  	slaverecv_sem;		
extern OS_SEM 	slavetran_sem;

u32 g_can2RxCount;
u32 g_can2TxCount;

///*can1 ������*/
//Message   rx_msg_buf[RX_BUF_LEN];
//uint32_t	rx_save, rx_read;
//CanTxMsg 	tx_msg_buf[TX_BUF_LEN];
//uint32_t 	tx_save, tx_read;

/*can2 ������*/
extern Message   	slaverx_msg_buf[RX_BUF_LEN];
extern uint32_t		slaverx_save, slaverx_read;
extern CanTxMsg 	slavetx_msg_buf[TX_BUF_LEN];
extern uint32_t 	slavetx_save, slavetx_read;

/**
  * @brief  canInit
  * @param  CANx:CAN1 or CAN2 bitrate
  * @retval 0��Success
  */
unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate)
{
		CAN_InitTypeDef        CAN_InitStructure;
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		GPIO_InitTypeDef  GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;	         
		/* CAN GPIOs configuration **************************************************/

		if(CANx == CAN1)
		{
				/* Enable GPIO clock */
				RCC_AHB1PeriphClockCmd(CAN1_GPIO_CLK, ENABLE);
			
				/* Connect CAN pins to AF9 */
				GPIO_PinAFConfig(CAN1_GPIO_PORT, CAN1_RX_SOURCE, CAN1_AF_PORT);
				GPIO_PinAFConfig(CAN1_GPIO_PORT, CAN1_TX_SOURCE, CAN1_AF_PORT); 
				
				/* Configure CAN RX and TX pins */
				GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN | CAN1_TX_PIN;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
				GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);
			
				/* CAN configuration ********************************************************/  
				/* Enable CAN clock */
				RCC_APB1PeriphClockCmd(CAN1_CLK, ENABLE);
				
				/* CAN register init */
				CAN_DeInit(CANx);
				CAN_StructInit(&CAN_InitStructure);
			
				/* CAN cell init */
				CAN_InitStructure.CAN_TTCM = DISABLE;   	 //��ֹʱ�䴥��ͨ��ģʽ 												//��ʱ�䴥��ͨ��ģʽ 	0����ֹ 1��ʹ��
				CAN_InitStructure.CAN_ABOM = ENABLE;   		 //�Զ����߹���ʹ��													//ENABLEʹ��Ӳ�����߹���   //DISABLE ����Զ����߹���  �Զ����߹رչ���          
				CAN_InitStructure.CAN_AWUM = ENABLE;			 //�Զ�����ģʽ��ʹ��													// DISABLE ˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)  0��������ѣ�1��Ӳ������
				CAN_InitStructure.CAN_NART = DISABLE;			 //���Զ��ش�ģʽ��ʹ�ܣ����Զ��ش�ֱ���ɹ�			//��ֹ�����Զ����� // 0���Զ��ط�   1��ֻ��һ��       
				CAN_InitStructure.CAN_RFLM = DISABLE;			 //FIFO����ģʽ��ʹ�ܣ����ʱ�ɱ��ĸ����±���		//���Ĳ�����,�µĸ��Ǿɵ� //  0����������1������ 
				CAN_InitStructure.CAN_TXFP = DISABLE;			 //FIFO���ȼ�ȷ�������ȼ��ɱ��ı�ʶ��ȷ��				//���ȼ��ɱ��ı�ʶ������ //0�����ȼ��ɱ��ı�ʶ��������1�����ȼ�������˳��ȷ��    
				CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;		 // ����Ϊ����ģʽ   ģʽ����   
				CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;   //�������������  ����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ CAN_SJW_1tq~CAN_SJW_4tq  			
				 
				CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq; //Tbs1��ΧCAN_BS1_1tq ~CAN_BS1_16tq
				CAN_InitStructure.CAN_BS2 = CAN_BS1_2tq;	//Tbs2��ΧCAN_BS2_1tq ~	CAN_BS2_8tq
				CAN_InitStructure.CAN_Prescaler = bitrate; //��Ƶϵ��(Fdiv)Ϊbrp+1  
				CAN_Init(CANx, &CAN_InitStructure);
			
				CAN_FilterInitStructure.CAN_FilterNumber = 0;	 
				CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
				CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;           
				CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;           
				CAN_FilterInit(&CAN_FilterInitStructure);
						
				/* Enable FIFO 0 message pending Interrupt */
				CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);	     //FIFO0��Ϣ�Һ��ж�����.         
				
				NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);

		}
		else if(CANx == CAN2)
		{
				/* Enable GPIO clock */
				RCC_AHB1PeriphClockCmd(CAN2_GPIO_CLK, ENABLE);
			
				/* Connect CAN pins to AF9 */
				GPIO_PinAFConfig(CAN2_GPIO_PORT, CAN2_RX_SOURCE, CAN2_AF_PORT);
				GPIO_PinAFConfig(CAN2_GPIO_PORT, CAN2_TX_SOURCE, CAN2_AF_PORT); 
				
				/* Configure CAN RX and TX pins */
				GPIO_InitStructure.GPIO_Pin = CAN2_RX_PIN | CAN2_TX_PIN;
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
				GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
				GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
				GPIO_Init(CAN2_GPIO_PORT, &GPIO_InitStructure);
			
				/* CAN configuration ********************************************************/  
				/* Enable CAN clock */
		/*-----------CAN��������ʼ�� ------------------------------------------*/
				/* ʹ��CANʱ��, ʹ��CAN2�����CAN1��ʱ�� */
//				RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);			//W H H  �ص�
//				RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
				RCC_APB1PeriphClockCmd(CAN1_CLK, ENABLE);			//W H H  �ص�
				RCC_APB1PeriphClockCmd(CAN2_CLK, ENABLE);
			
				
				/* CAN register init */
				CAN_DeInit(CANx);
				CAN_StructInit(&CAN_InitStructure);
			
				/* CAN cell init */
				CAN_InitStructure.CAN_TTCM = DISABLE;    
				CAN_InitStructure.CAN_ABOM = ENABLE;    
				CAN_InitStructure.CAN_AWUM = ENABLE;	
				CAN_InitStructure.CAN_NART = DISABLE;	
				CAN_InitStructure.CAN_RFLM = DISABLE;	
				CAN_InitStructure.CAN_TXFP = DISABLE;		
				CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	
				CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; 
					
				CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq;
				CAN_InitStructure.CAN_BS2 = CAN_BS1_2tq;
				CAN_InitStructure.CAN_Prescaler = bitrate;    
				CAN_Init(CANx, &CAN_InitStructure);		

				CAN_SlaveStartBank(14);                             
				CAN_FilterInitStructure.CAN_FilterNumber = 14;	  
				CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   
				CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
				CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
				CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
				CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;           
				CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;         
				CAN_FilterInit(&CAN_FilterInitStructure);
						
				/* Enable FIFO 0 message pending Interrupt */
				CAN_ITConfig(CANx, CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0 , ENABLE);			//W H H ,���������жϣ����������жϣ�����������ж�
				
				NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;	 
				NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
				NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
				NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
				NVIC_Init(&NVIC_InitStructure);                       
		}
		
//		else if(CANx == CAN3)
//		{
//				u8 res;
//				res = MTDREP_CAN3_MCP2515_Init(bitrate);
//				if(res == MCP2515_OK)
//					return 0;
//				else
//					return CAN_FAILINIT;
//		}
		
		return 0;
}



/*********************************************************************************************************
** Function name:       User_CANTransmit
** Descriptions:        CAN1��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
u8 User_CAN1Transmit(uint32_t id,uint8_t* val,uint8_t Len)
{
	CanTxMsg TxMessage;                                                   //����CAN���Ľṹ��          
	uint8_t  TransmitMailbox;
	uint16_t i;

	TxMessage.StdId=id/*&0x7FF*/;                                           //idΪ0x04      
//	TxMessage.ExtId=id;
	TxMessage.RTR=CAN_RTR_DATA;                                           //����֡                      
	TxMessage.IDE=CAN_ID_STD;  //CAN_ID_EXT;                                             //��չ֡                      
	TxMessage.DLC=Len;                                                      //���ݳ�����    
	
	//W H H 
//	printf("can1uTxRtr=%d\r\n",TxMessage.RTR);
//	printf("can1uTIDE=%d\r\n",TxMessage.IDE);
	
	for(i=0;i<Len;i++,val++)
	{
		TxMessage.Data[i]=*val;                                               //������ݳ�    
	}                                                                                   
	 
	TransmitMailbox=CAN_Transmit(CAN1,&TxMessage);                        //���Ͳ���ȡ�����     
	
//	i=0;  //W H H  ����
//	while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i<0XFFF))  i++;         //�ȴ��������   
//	if(i>0xfff)
//	{
//		printf("can1TX,Error!\r\n");
////		CAN_CancelTransmit(CAN1,TransmitMailbox);
//		return 1;
//	}
//		else
//	{
//		printf("can1TX,Success!\r\n");
//		return 0;
//	}
	return TransmitMailbox;
}

/*********************************************************************************************************
** Function name:       CAN1_RX0_ISR
** Descriptions:        CAN1�����жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
u8 CAN1_RX0_ISR(void)
{
	 CanRxMsg RxMessage; 
	 u8 i;
    if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
    {

        CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);                    //����жϱ�־                
     		
				if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)	
					return 1;		//û�н��յ�����,ֱ���˳�
			
				CAN_Receive(CAN1,CAN_FIFO0,&RxMessage);                     //��ȡ����  
				
				for(i=0;i<8;i++)
				printf("can1Rx1buf[%d]:0x%x\r\n",i,RxMessage.Data[i]);

			  return 0;
//        if(RxMessage.RTR==0) /*((RxMessage.Data[1]==0xaa)&&(RxMessage.Data[0]==0xbb))  */ //�򵥲���,����ȫ�����ݽ���У��
//				{
//					 printf("CAN1���յ���������:0x%x-0x%x-0x%x-0x%x-0x%x\r\n",RxMessage.Data[0],\
//					 RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3],RxMessage.Data[4]);   //��ӡCAN1���յ�������
////					 LED_On(3);                      
//				}
//				 else printf("CAN1_RX_ERROR\r\n");
    }
		
		return 0;
}


/*********************************************************************************************************
** Function name:       User_CANTransmit
** Descriptions:        CAN2��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

u8 User_CAN2Transmit(uint32_t id,uint8_t* val,uint8_t Len)
{
	CanTxMsg TxMessage;                                                   //����CAN���Ľṹ��          
	uint8_t  TransmitMailbox;
	uint16_t i;

		TxMessage.StdId=id;						                                        //idΪ0x04  
//	TxMessage.ExtId=id;
	TxMessage.RTR=CAN_RTR_DATA;                                           //����֡                      
	TxMessage.IDE=CAN_ID_STD   ;//CAN_ID_EXT;                                             //��չ֡                      
	TxMessage.DLC=Len;                                                    //���ݳ�����    
	
		//W H H 
//		printf("uTxExtId=0x%x\r\n",TxMessage.ExtId);
//	printf("uTDLC=%d\r\n",TxMessage.DLC);
//	printf("uTxRtr=%d\r\n",TxMessage.RTR);
//	printf("uTIDE=%d\r\n",TxMessage.IDE);
	
	for(i=0;i<Len;i++,val++)
	{
		TxMessage.Data[i]=*val;                                              //������ݳ�    
	}                                                        
	 

	TransmitMailbox=CAN_Transmit(CAN2,&TxMessage);                        //���Ͳ���ȡ�����  
	
//	i=0; //W H H ����
//	while((CAN_TransmitStatus(CAN2,TransmitMailbox) != CANTXOK) && (i<0XFFF) ) i++;         //�ȴ��������     
//	
////	printf("i=%d\r\n",i);
//	if(i>=0xfff)
//	{
//		printf("can2TX,Error!\r\n");
////		CAN_CancelTransmit(CAN2,TransmitMailbox);
//		return 1;
//	}	
//	else
//	{
//		printf("can2TX,Success!\r\n");
//		return 0;
//	}
		return TransmitMailbox;
}
/*********************************************************************************************************
** Function name:       CAN2_RX0_ISR
** Descriptions:        CAN1�����жϷ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
u8 CAN2_RX0_ISR(void)
{
		CanRxMsg RxMessage;
		u8 i;
	
    if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET)
    {
        			   
        CAN_ClearITPendingBit(CAN2,CAN_IT_FMP0);                        //����жϱ�־   
				
				if( CAN_MessagePending(CAN2,CAN_FIFO0)==0)	
					return 1;		//û�н��յ�����,ֱ���˳�
				
				printf("can2��ʼ��������\r\n");	
        CAN_Receive(CAN2,CAN_FIFO0,&RxMessage);                         //��ȡ����   
				
				for(i=0;i<8;i++)
				printf("can2Rx2buf[%d]:0x%x\r\n",i,RxMessage.Data[i]);			
				
				return 0;
//        if (RxMessage.RTR==0) /*((RxMessage.Data[0]==0xaa)&&(RxMessage.Data[1]==0xbb))*/
//				{
//					printf("CAN2���յ���������:0x%x-0x%x-0x%x-0x%x \r\n",RxMessage.Data[0],\
//					RxMessage.Data[1],RxMessage.Data[2],RxMessage.Data[3]);         //��ӡCAN2���յ�������                               /* ����1ת������                */          
////         			LED_On(1);
//				}
//         else printf("CAN2_RX_ERROR\r\n");				
    }
		
		return 0;
}


/**
  * @brief  canSend
	* @param  CANx:CAN1 or CAN2   m:can message
  * @retval 0��Success
  */
unsigned char canSend(CAN_PORT CANx, Message *m)	                
{
		u16 i;
		uint32_t  next;
//		OS_ERR err;
	
//	if(CANx == CAN1)
//	{
//		next = tx_save + 1;
//		if(next >= TX_BUF_LEN) next = 0;
//		if(next != tx_read)
//		{
//			CanTxMsg *ptx_msg;

//			ptx_msg = &tx_msg_buf[tx_save];
//			ptx_msg->StdId = m->cob_id;
//	
//			if(m->rtr)
//	  		ptx_msg->RTR = CAN_RTR_REMOTE;
//			else
//				ptx_msg->RTR = CAN_RTR_DATA;

//	  	ptx_msg->IDE = CAN_ID_STD;
//  		ptx_msg->DLC = m->len;
//			for(i = 0; i < m->len; i++)
//				ptx_msg->Data[i] = m->data[i];
//	
//			tx_save = next;

//			//�����ź���
//			OSSemPost(&tran_sem,OS_OPT_POST_1,&err);
//			printf("os1=%d\r\n",tran_sem.Ctr);
//		
//		}		
//		
//		else 
//			printf("can1 err!\r\n");
//		
//			printf("can1_next=%d\r\n",next);		

//		return 0x00;			//W H H		�����ﷸ��
//	}
		
	/*else*/ if(CANx == CAN2)
	{
		next = slavetx_save + 1;
		if(next >= TX_BUF_LEN) next = 0;
		if(next != slavetx_read)
		{
			CanTxMsg *ptx_msg;

			ptx_msg = &slavetx_msg_buf[slavetx_save];
			ptx_msg->StdId = m->cob_id;
	
			if(m->rtr)
	  		ptx_msg->RTR = CAN_RTR_REMOTE;
			else
				ptx_msg->RTR = CAN_RTR_DATA;

			ptx_msg->IDE = CAN_ID_STD;
  		ptx_msg->DLC = m->len;
			for(i = 0; i < m->len; i++)
				ptx_msg->Data[i] = m->data[i];
	
			slavetx_save = next;

			//�����ź���
			//	OSSemPost(&slavetran_sem,OS_OPT_POST_1,&err);
			//	printf("os2=%d\r\n",slavetran_sem.Ctr);
					
			g_can2TxCount++;
			printf("os2=%d\r\n",g_can2TxCount);
		}
		
		else 
			printf("can2 err!\r\n");

		return 0x00;
	}
	
//	else if(CANx == CAN3 )			//MCP2515_CAN3
//	{ 
//		u8 res, TxBufNum;
//		u8 i;
//		u8 timeout=0;
//		CanTxMsg ptx_msg;
//		memset(&ptx_msg,0,sizeof(CanTxMsg));	//��ʼ�����͵Ŀռ�
//		
//		ptx_msg.StdId = m->cob_id;
//		
//		if(m->rtr)
//			ptx_msg.RTR = CAN_RTR_REMOTE;
//		else
//			ptx_msg.RTR = CAN_RTR_DATA;
//		
//		ptx_msg.IDE = CAN_ID_STD;
//  	ptx_msg.DLC = m->len;
//		
//		for(i = 0; i < m->len; i++)
//				ptx_msg.Data[i] = m->data[i];
//		
//		do {
//			res = MTDREP_CAN3_MCP2515_Get_Free_TxBuf(&TxBufNum); // info = addr.
//			timeout++;
//					if (timeout == 0xFF) break;
//		} while (res == MCP2515_ALLTXBUSY);
//		
//		if (timeout != 0xFF)
//		{
//			MTDREP_CAN3_MCP2515_Write_CAN_Message(TxBufNum, (const CanTxMsg*)&ptx_msg);
//			MTDREP_CAN3_MCP2515_Start_Transimt(TxBufNum);
//			return CAN_OK;
//		}
//		else
//		{
//			return CAN_FAILTX;
//		}
//				
//	}
	 
	else
		printf("no canx chanle!\r\n");
	return 0xff;
}

/**
  * @brief  This function handles CAN1 RX0 request.
  * @param  None
  * @retval None
  */
extern  CO_Data TestSlave_Data;
extern 	CO_Data TestMaster_Data;

//void CAN1_RX0_IRQHandler(void)		//W H H ��can1����Ϊ��վ
//{  
//	#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
//	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//  CPU_CRITICAL_ENTER();	//���ж�
//		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
//	CPU_CRITICAL_EXIT();	//���ж�
//#endif
//	
//	{
//		OS_ERR err;
//		Message *pmsg;
//		uint32_t i, next;
//		
//		CanRxMsg RxMessage;		//W H H
//		
//		if( CAN_GetITStatus( CAN1, CAN_IT_FMP0 ) == SET )		//FIFO�����ж�
//		{  
//			CAN_ClearITPendingBit( CAN1, CAN_IT_FMP0 );

//			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

//			next = (rx_save + 1) ;
//			if(next >= RX_BUF_LEN) next = 0;
//			if(next == rx_read) return;

//			pmsg = &rx_msg_buf[rx_save];
//			pmsg->cob_id = (UNS16)RxMessage.StdId;		/**< message's ID */
//			if(RxMessage.RTR == CAN_RTR_REMOTE)				/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
//				pmsg->rtr = 1;		
//			else
//				pmsg->rtr = 0;

//			pmsg->len  = (UNS8)RxMessage.DLC;					/**< message's length (0 to 8) */
//			for(i = 0; i < pmsg->len; i++)
//				pmsg->data[i] = RxMessage.Data[i];

//			rx_save = next;
//		
//		/*W H H */
//		printf("id=%d\r\n",rx_msg_buf[rx_read].cob_id);
//		for(i=0;i<8;i++)
//		{
//			printf("rx=0x%x\r\n",rx_msg_buf[rx_read].data[i]);
//		}
//		//�����ź���
//			OSSemPost(&recv_sem,OS_OPT_POST_1,&err);
//		}
//		
//		if(CAN_GetITStatus( CAN1, CAN_IT_FF0) )		//FIFO ���ж�
//		{
//				
//			CAN_ClearITPendingBit(CAN1, CAN_IT_FF0 );
//		}
//		
//		if(CAN_GetITStatus( CAN1, CAN_IT_FOV0) )	//FIF0 ����ж�
//		{
//				
//			CAN_ClearITPendingBit(CAN1, CAN_IT_FOV0 );
//		}
//		
//	}
//		
////    unsigned int i = 0;
//				
////	Message RxMSG = Message_Initializer;
//////	CANOpen_Message CAN1_Rx_m;
////	
////	if( CAN_GetITStatus( CAN1, CAN_IT_FMP0 ) == SET )		//FIFO�����ж�
////	{  
////		CAN_ClearITPendingBit( CAN1, CAN_IT_FMP0 );
////		
////		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	        //��CAN1 FIFO0��������
////		RxMSG.cob_id = (uint16_t)(RxMessage.StdId);
////		RxMSG.rtr = RxMessage.RTR;
////		RxMSG.len = RxMessage.DLC;
////		for(i=0;i<RxMSG.len;i++)
////		{
////			RxMSG.data[i] = RxMessage.Data[i];
////		}
////	}
////	
////	if(CAN_GetITStatus( CAN1, CAN_IT_FF0) )		//FIFO ���ж�
////	{
////			
////		CAN_ClearITPendingBit(CAN1, CAN_IT_FF0 );
////	}
////	
////	if(CAN_GetITStatus( CAN1, CAN_IT_FOV0) )	//FIF0 ����ж�
////	{
////			
////		CAN_ClearITPendingBit(CAN1, CAN_IT_FOV0 );
////	}
//////	canDispatch(CANOpenMasterObject, &(RxMSG));
////	TIM_ITConfig(TIM4,TIM_IT_CC1,DISABLE);		//�ض�ʱ���ж�
////		canDispatch(&TestSlave_Data, &(RxMSG));
////		TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);		//����ʱ���ж�
////	printf("can Master revcive data!!!!!!!!!!!!\r\n");
//	
//#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
//	OSIntExit();    	//�˳��ж�
//#endif
//}

/**
  * @}
  */ 
/**
  * @brief  This function handles CAN2 RX0 request.
  * @param  None
  * @retval None
  */
void CAN2_RX0_IRQHandler(void)
{  
#if SYSTEM_SUPPORT_OS  	//ʹ��UCOS����ϵͳ
	CPU_SR_ALLOC();				//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
  CPU_CRITICAL_ENTER();	//���ж�
		OSIntEnter();     	//�����ж�	 �ж�Ƕ���������1
	CPU_CRITICAL_EXIT();	//���ж�
#endif
	
	{
//		OS_ERR err;
		Message *pmsg;
		uint32_t i, next;
		CanRxMsg RxMessage;
		
//		printf("can2 int\r\n");
//		printf("CAN_RF0R=%d\r\n",CAN2->RF0R);
		
		if(CAN_GetITStatus( CAN2, CAN_IT_FF0) )		//FIFO ���ж�
		{
			//printf("1\r\n");	
			CAN_ClearITPendingBit(CAN2, CAN_IT_FF0 );
		}
		
		if(CAN_GetITStatus( CAN2, CAN_IT_FOV0) )	//FIF0 ����ж�
		{
			//printf("2\r\n");	
			CAN_ClearITPendingBit(CAN2, CAN_IT_FOV0 );
		}

		if( CAN_GetITStatus( CAN2, CAN_IT_FMP0 ) != RESET )		//FIFO�����ж�
		{  
		//	printf("3\r\n");
//			CAN_ClearITPendingBit( CAN2, CAN_IT_FMP0 );		
			
			CAN_Receive(CAN2, CAN_FIFO0, &RxMessage);

		//	printf("4\r\n");
			
			next = (slaverx_save + 1) ;
			if(next >= RX_BUF_LEN) next = 0;
//			if(next == slaverx_read)
//			{		
//				printf("5\r\n");
//				printf("save=%d\r\n",next);
//				printf("read=%d\r\n",slaverx_read);
//				#if SYSTEM_SUPPORT_OS 	 	//ʹ��UCOS����ϵͳ
//					OSIntExit();    				//�˳��ж�
//				#endif	
//				return;
//			}

			pmsg = &slaverx_msg_buf[slaverx_save];
			pmsg->cob_id = (UNS16)RxMessage.StdId;		/**< message's ID */
			if(RxMessage.RTR == CAN_RTR_REMOTE)				/**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
				pmsg->rtr = 1;		
			else
				pmsg->rtr = 0;

			pmsg->len  = (UNS8)RxMessage.DLC;					/**< message's length (0 to 8) */
			for(i = 0; i < pmsg->len; i++)
				pmsg->data[i] = RxMessage.Data[i];

			slaverx_save = next;
			
			CAN_ClearITPendingBit( CAN2, CAN_IT_FMP0 );		
	
			/*W H H */
			//printf("id2=%d\r\n",pmsg->cob_id/*slaverx_msg_buf[slaverx_read].cob_id*/);
		//			for(i=0;i<8;i++)
		//			{
		//				printf("rx2=0x%x\r\n",slaverx_msg_buf[slaverx_read].data[i]);
		//			}
		//				//�����ź���
		//			OSSemPost(&slaverecv_sem,OS_OPT_POST_1,&err);
			
			g_can2RxCount++;
			printf("RxOs2=%d\r\n",g_can2RxCount);
		}
			
	}
	
#if SYSTEM_SUPPORT_OS  //ʹ��UCOS����ϵͳ
	OSIntExit();    	//�˳��ж�
#endif	
}



