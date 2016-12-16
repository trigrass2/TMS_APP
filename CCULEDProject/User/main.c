/****************************************Copyright (c)***************************************
// �̼����д���ɫ��־�ģ���ʾ��ǰû��ʹ�ã�����Ҫʹ������Ҫ���뵽������
**-------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**-------------------------------------------------------------------------------------------
********************************************************************************************/
#include "bsp.h"


#define BUFFER_SIZE 512
	
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];


static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);

/*���񴴽���������Ĳ���*/

/*LED������ʾ����*/
	//MainTask  �������ȼ�
#define Main_TASK_PRIO		6
	//�����ջ��С	
#define Main_STK_SIZE 		512
	//������ƿ�
static OS_TCB MainTaskTCB;
	//�����ջ	
static CPU_STK Main_TASK_STK[Main_STK_SIZE];
	//������
static void Main_task(void *p_arg);

////LED�������ȼ�
//#define LED_TASK_PRIO		5
////�����ջ��С	
//#define LED_STK_SIZE 		256
////������ƿ�
//static OS_TCB LedTaskTCB;
////�����ջ	
//static CPU_STK LED_TASK_STK[LED_STK_SIZE];
////������
//static void led_task(void *p_arg);

///*�¶ȼ������*/
//	//temper �¶ȼ���������ȼ�
//#define TemperCheck_TASK_PRIO		6
//	//�����ջ��С	
//#define TemperCheck_STK_SIZE 		256
//	//������ƿ�
//static OS_TCB TemperCheckTaskTCB;
//	//�����ջ	
//static CPU_STK TemperCheck_TASK_STK[TemperCheck_STK_SIZE];
//	//������
//static void TemperCheck_task(void *p_arg);

///*������ʾcpu,��ջ��Ϣ������*/
//	//KEY �����������ȼ�
//#define KEY_TASK_PRIO		7
//	//�����ջ��С	
//#define KEY_STK_SIZE 		512
//	//������ƿ�
//static OS_TCB KeyTaskTCB;
//	//�����ջ	
//static CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
//	//������
//static void key_task(void *p_arg);


///*��վ��������,��ջ��Ϣ������*/
//	//�������ȼ�
//#define MASTERRX_TASK_PRIO		9
//	//�����ջ��С	
//#define MASTERRX_STK_SIZE 		512
//	//������ƿ�
//static OS_TCB MasterRxTaskTCB;
//	//�����ջ	
//static CPU_STK MASTERRX_TASK_STK[MASTERRX_STK_SIZE];
//	//������
//extern void master_recv_thread(void *p_arg);



///*��վ��������*/
//	//�������ȼ�
//#define MASTERTX_TASK_PRIO		10
//	//�����ջ��С	
//#define MASTERTX_STK_SIZE 		512
//	//������ƿ�
//static OS_TCB MasterTxTaskTCB;
//	//�����ջ	
//static CPU_STK MASTERTX_TASK_STK[MASTERTX_STK_SIZE];
//	//������
//extern void master_send_thread(void *p_arg);

	/*�������Ĵ���*/
static  OS_SEM     	SEM_SYNCH;	   //����ͬ��


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define SLAVERX_TASK_PRIO		7
	//�����ջ��С	
#define SLAVERX_STK_SIZE 		512
	//������ƿ�
static OS_TCB SlaveRxTaskTCB;
	//�����ջ	
static CPU_STK SLAVERX_TASK_STK[SLAVERX_STK_SIZE];
	//������
extern void slave_recv_thread(void *p_arg);



/*��վ��������*/
	//�������ȼ�
#define SLAVETX_TASK_PRIO		8
	//�����ջ��С	
#define SLAVETX_STK_SIZE 		512
	//������ƿ�
static OS_TCB SlaveTxTaskTCB;
	//�����ջ	
static CPU_STK SLAVETX_TASK_STK[SLAVETX_STK_SIZE];
	//������
extern void slave_send_thread(void *p_arg);


/*��վ������������*/
	//�������ȼ�
#define SLAVE_STARTUPMANAGER_TASK_PRIO		5
	//�����ջ��С	
#define SLAVE_STARTUPMANAGER_STK_SIZE 		512
	//������ƿ�
static OS_TCB SlaveStartupManagerTCB;
	//�����ջ	
static CPU_STK SLAVE_STARTUPMANAGER_TASK_STK[SLAVE_STARTUPMANAGER_STK_SIZE];
	//������
void Slave_StartupManager_Take(void *p_arg);


/*CCULED ���������豸��״̬��ʾled����*/
	//�������ȼ�
#define CCULED_DEVICE_STATUS_TASK_PRIO		4
	//�����ջ��С	
#define CCULED_DEVICE_STATUS_STK_SIZE 		512
	//������ƿ�
static OS_TCB CCULEDDeviceStatusTCB;
	//�����ջ	
static CPU_STK CCULED_DEVICE_STATUS_TASK_STK[CCULED_DEVICE_STATUS_STK_SIZE];
	//������
void CCULED_DeviceStatus_Take(void *p_arg);



	/*�������Ĵ���*/
static  OS_SEM     	SEM_SYNCH;	   //����ͬ��
OS_SEM     	Slave_Start_Sem;			//��վ����������ź���

///*����ʹ�õ�ȫ�ֱ����Ķ���*/
////can
uint8_t DS2401P_ID[8]={0x00};			//���붨����bsp��macid�����ʹ����
uint8_t can2_send[]={0x11,0x22,0x33,0x44,0x11,0x66,0x78,0x99,};
u8 g_RxDataFlag=0;
u8 g_FrameErrorFlag=0;

/*canfestival �����������ĵĶ���*/

IsNoReceHeart			 g_RxCAN1HeartFlag[15]={NoRxHeart};					//��Ÿ����豸�������ı�־���±���ǽڵ��ID�ţ����±�Ϊ0������û��ʹ�á�
IsNoReceHeart			 g_RxCAN2HeartFlag[15]={NoRxHeart};

CurReceHeartState  g_RxCAN1HeartStateFlag[15] = {InPowerInitState};	//��ŵ�ǰ��������ʱ�����豸���ڵ�״̬�����ϵ�ճ�ʼ����ʱ�򣬻��ǽ��������ӵ�ʱ��		
CurReceHeartState  g_RxCAN2HeartStateFlag[15] = {InPowerInitState};

RxCanMessageState  g_RxCAN1MesStateFlag[15]={NoRxCanMesState};			//��ŵ�ǰ��վ�ӵ���վ��can��Ϣ ������״̬ 
RxCanMessageState	 g_RxCAN2MesStateFlag[15]={NoRxCanMesState};

IsNoReceHeart  		 g_CCU1HeartFlag=NoRxHeart ;                //��վ���ܵ���վ����������ȫ�ֱ�־��

//extern CO_Data TestSlave_Data;
//extern CO_Data TestMaster_Data;

extern uint8_t UART1_Rx_Cnt;

extern CO_Data Ccu1LedObjDict_Data;

//void Ican_Receive_ISR(void);		//ican���жϽ��ܷ������

	/*
		����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
		�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

		ϵͳʱ��ȱʡ����Ϊ168MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
	*/
	
int main(void)
{
		OS_ERR  err;

		CPU_SR_ALLOC();																	//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	
		//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); //����ӳ���ж�������
		SCB->VTOR = FLASH_BASE | 0x10000;	//�����ж�������	
//		uart3_init();		//uart3��GPIO��ʼ�� ,��ǰ�ĳ�ʼ������ֹ�м��д�ӡ��ʾ��Ϣ���޷����
		Uart1_init();
	
		/* ��ʼ��uC/OS-III �ں� */	
    OSInit(&err);                                               /* Init uC/OS-III.                                      */
		
		OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����

		/* ����һ����������Ҳ���������񣩡���������ᴴ�����е�Ӧ�ó������� */
    OSTaskCreate((OS_TCB       *)&AppTaskStartTCB,       /* Create the start task   *//* ������ƿ��ַ */    
                 (CPU_CHAR     *)"App Task Start",			 /* ������ */	
                 (OS_TASK_PTR   )AppTaskStart, 					 /* ������������ַ */
                 (void         *)0,											 /* ���ݸ�����Ĳ��� */
                 (OS_PRIO       )APP_CFG_TASK_START_PRIO,/* �������ȼ� */
                 (CPU_STK      *)&AppTaskStartStk[0],		 /* ��ջ����ַ */
                 (CPU_STK_SIZE  )AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE / 10],	/* ��ջ������������ʾ��10%��Ϊ����� */
                 (CPU_STK_SIZE  )APP_CFG_TASK_START_STK_SIZE,	/* ��ջ�ռ��С */
                 (OS_MSG_QTY    )0,														/* ������֧�ֽ��ܵ������Ϣ�� */
                 (OS_TICK       )0,													 /* ����ʱ��Ƭ */
                 (void         *)0,													 /* ��ջ�ռ��С */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),
                 (OS_ERR       *)&err);
					 
									 /*  �������£�
										OS_OPT_TASK_STK_CHK      ʹ�ܼ������ջ��ͳ������ջ���õĺ�δ�õ�
										OS_OPT_TASK_STK_CLR      �ڴ�������ʱ����������ջ
										OS_OPT_TASK_SAVE_FP      ���CPU�и���Ĵ��������������л�ʱ���渡��Ĵ���������
									 */ 
								 
		OS_CRITICAL_EXIT();	//�˳��ٽ���

		/* ����������ϵͳ������Ȩ����uC/OS-III */
    OSStart(&err);                        /* Start multitasking (i.e. give control to uC/OS-III). */
    
    (void)&err;

		return(0);
}


		/*��ʼ����*/
static  void  AppTaskStart (void *p_arg)
{		

//	iCANID packet_id;
	
		OS_ERR      err;
   (void)p_arg;
    BSP_Init();                 /*���е�����ĳ�ʼ����������*/     /* Initialize BSP functions                             */
    CPU_Init();                                                	 /* Initialize the uC/CPU services                       */ 
	//  BSP_Tick_Init();                                            /* Start Tick Initialization                            */
	//  Mem_Init();                                                 /* Initialize Memory Management Module                  */
	//  Math_Init();                                                /* Initialize Mathematical Module                       */

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

	/* 
	 1. ��OSInit����Ĭ��ÿ�������ʱ��Ƭ��OSCfg_TickRate_Hz / 10u��	�� 1000/10=100ms
	 2. ʹ������ĺ�����������ȫ��ʱ��Ƭ��Ҳ����ÿ������ʹ�õ�ʱ��Ƭ
	 3. ������溯���ĵڶ�����������Ϊ0����ô��ʾ��������Ĭ��ʱ��ƬΪOSCfg_TickRate_Hz / 10u
	 4. ��Ȼ��Ҳ�����ڴ��������ʱ�򵥶�����ʱ��Ƭ��С�� 
	*/
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��	��ÿ��ʱ�ӽ���Ϊ1ms
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ5��ϵͳʱ�ӽ��ģ���5*1=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,5,&err);  
#endif

		//����Ĵ���
  AppObjCreate();                                             /* Create Applicaiton kernel objects                    */

		//����Ĵ���
  AppTaskCreate();                                            /* Create Application tasks                             */

	/*��ӡ������Ϣ*/
	delay_ms(1000);			//��������ĵ��ȣ�����ĺ����Ǵ�ӡ��ʾ����������Ϣ
	DispTaskInfo();			//��ӡ���AppTaskStart����ͽ����ˣ���Ϊû��while();
	OS_TaskSuspend((OS_TCB*)&AppTaskStartTCB,&err);		//����ʼ����		
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
	OS_ERR      err;
	CPU_SR_ALLOC();			//�������壬���ڱ���SR״̬.
	
	OS_CRITICAL_ENTER();	//�����ٽ���
	
		//����MainTask����
	OSTaskCreate((OS_TCB 		  *)&MainTaskTCB,			//������ƿ�
								(CPU_CHAR	  *)"Main_task", 			//��������
                 (OS_TASK_PTR)Main_task, 				//������
                 (void		  *)0,								//���ݸ��������Ĳ���
                 (OS_PRIO	   )Main_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&Main_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)Main_TASK_STK[Main_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)Main_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ
								 
//	//����LedTaskTCB����
//	OSTaskCreate((OS_TCB 	 *)&LedTaskTCB,			//������ƿ�
//							(CPU_CHAR	 *)"led_task", 			//��������
//							 (OS_TASK_PTR)led_task, 			//������
//							 (void		  *)0,							//���ݸ��������Ĳ���
//							 (OS_PRIO	   )LED_TASK_PRIO,   //�������ȼ�
//							 (CPU_STK   *)&LED_TASK_STK[0],//�����ջ����ַ
//							 (CPU_STK_SIZE)LED_TASK_STK[LED_STK_SIZE/10],//�����ջ�����λ
//							 (CPU_STK_SIZE)LED_STK_SIZE,	 //�����ջ��С
//							 (OS_MSG_QTY  )0,							 //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//							 (OS_TICK	    )0,							 //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//							 (void   	   *)0,							 //�û�����Ĵ洢��
//							 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),		//����ѡ��
//							 (OS_ERR 	   *)&err);					 //��Ÿú�������ʱ�ķ���ֵ
								 
//	 //����TemperCheckTask����
//	OSTaskCreate((OS_TCB 		  *)&TemperCheckTaskTCB,			//������ƿ�
//								(CPU_CHAR	  *)"TemperCheck_task", 			//��������
//                 (OS_TASK_PTR)TemperCheck_task, 			//������
//                 (void		  *)0,											//���ݸ��������Ĳ���
//                 (OS_PRIO	   )TemperCheck_TASK_PRIO,     	//�������ȼ�
//                 (CPU_STK   *)&TemperCheck_TASK_STK[0],	//�����ջ����ַ
//                 (CPU_STK_SIZE)TemperCheck_TASK_STK[TemperCheck_STK_SIZE/10],		//�����ջ�����λ
//                 (CPU_STK_SIZE)TemperCheck_STK_SIZE,		//�����ջ��С
//                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//                 (void   	   *)0,					//�û�����Ĵ洢��
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ
//								 				 
//	//����KEY����
//	OSTaskCreate((OS_TCB 		* )&KeyTaskTCB,			//������ƿ�
//								(CPU_CHAR	* )"key_task", 			//��������
//                 (OS_TASK_PTR)key_task, 			//������
//                 (void		  *)0,					//���ݸ��������Ĳ���
//                 (OS_PRIO	   )KEY_TASK_PRIO,     	//�������ȼ�
//                 (CPU_STK   *)&KEY_TASK_STK[0],	//�����ջ����ַ
//                 (CPU_STK_SIZE)KEY_TASK_STK[KEY_STK_SIZE/10],		//�����ջ�����λ
//                 (CPU_STK_SIZE)KEY_STK_SIZE,		//�����ջ��С
//                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//                 (void   	   *)0,					//�û�����Ĵ洢��
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ
//								 
//		//������վ��������
//	OSTaskCreate((OS_TCB 		* )&MasterRxTaskTCB,			//������ƿ�
//								(CPU_CHAR	* )"MasterRx_task", 			//��������
//                 (OS_TASK_PTR)master_recv_thread, 	//������
//                 (void		  *)0,										//���ݸ��������Ĳ���
//                 (OS_PRIO	   )MASTERRX_TASK_PRIO,     	//�������ȼ�
//                 (CPU_STK   *)&MASTERRX_TASK_STK[0],	//�����ջ����ַ
//                 (CPU_STK_SIZE)MASTERRX_TASK_STK[MASTERRX_STK_SIZE/10],		//�����ջ�����λ
//                 (CPU_STK_SIZE)MASTERRX_STK_SIZE,		//�����ջ��С
//                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//                 (void   	   *)0,					//�û�����Ĵ洢��
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				

//		//������վ��������
//	OSTaskCreate((OS_TCB 		* )&MasterTxTaskTCB,			//������ƿ�
//								(CPU_CHAR	* )"MasterTx_task", 			//��������
//                 (OS_TASK_PTR)master_send_thread, 	//������
//                 (void		  *)0,										//���ݸ��������Ĳ���
//                 (OS_PRIO	   )MASTERTX_TASK_PRIO,     	//�������ȼ�
//                 (CPU_STK   *)&MASTERTX_TASK_STK[0],	//�����ջ����ַ
//                 (CPU_STK_SIZE)MASTERTX_TASK_STK[MASTERTX_STK_SIZE/10],		//�����ջ�����λ
//                 (CPU_STK_SIZE)MASTERTX_STK_SIZE,		//�����ջ��С
//                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//                 (void   	   *)0,					//�û�����Ĵ洢��
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ			


/*Wh h*/
		//������վ��������
	OSTaskCreate((OS_TCB 		* )&SlaveRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"SlaveRx_task", 			//��������
                 (OS_TASK_PTR)slave_recv_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )SLAVERX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&SLAVERX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)SLAVERX_TASK_STK[SLAVERX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)SLAVERX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				

		//������վ��������
	OSTaskCreate((OS_TCB 		* )&SlaveTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"SlaveTx_task", 			//��������
                 (OS_TASK_PTR)slave_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )SLAVETX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&SLAVETX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)SLAVETX_TASK_STK[SLAVETX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)SLAVETX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ										 
								 
								 
		//������վ������������
	OSTaskCreate((OS_TCB 		* )&SlaveStartupManagerTCB,				//������ƿ�
								(CPU_CHAR	* )"SlaveStartManager_task", 			//��������
                 (OS_TASK_PTR)Slave_StartupManager_Take, 		//������
                 (void		  *)0,														//���ݸ��������Ĳ���
                 (OS_PRIO	   )SLAVE_STARTUPMANAGER_TASK_PRIO,    	//�������ȼ�
                 (CPU_STK   *)&SLAVE_STARTUPMANAGER_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)SLAVE_STARTUPMANAGER_TASK_STK[SLAVE_STARTUPMANAGER_STK_SIZE/10],	//�����ջ�����λ
                 (CPU_STK_SIZE)SLAVE_STARTUPMANAGER_STK_SIZE,			//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ						 

		//������վ���������豸��״̬��ʾled����
	OSTaskCreate((OS_TCB 		* )&CCULEDDeviceStatusTCB,				//������ƿ�
								(CPU_CHAR	* )"CCULEDDeviceStatus_Take", 			//��������
                 (OS_TASK_PTR)CCULED_DeviceStatus_Take, 		//������
                 (void		  *)0,														//���ݸ��������Ĳ���
                 (OS_PRIO	   )CCULED_DEVICE_STATUS_TASK_PRIO,    	//�������ȼ�
                 (CPU_STK   *)&CCULED_DEVICE_STATUS_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CCULED_DEVICE_STATUS_TASK_STK[CCULED_DEVICE_STATUS_STK_SIZE/10],	//�����ջ�����λ
                 (CPU_STK_SIZE)CCULED_DEVICE_STATUS_STK_SIZE,			//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ								 
								 

	OS_CRITICAL_EXIT();	//�˳��ٽ���							 
}


/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{

	/* ����ͬ���ź��� */ 
		BSP_OS_SemCreate(&SEM_SYNCH, 0,(CPU_CHAR *)"SEM_SYNCH");

}


extern u8 User_CAN1Transmit(uint32_t id,uint8_t* val,uint8_t Len);
extern u8 User_CAN2Transmit(uint32_t id,uint8_t* val,uint8_t Len);
extern unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);

///*EEPROM ����*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define sEE_WRITE_ADDRESS1        0xA0
#define sEE_READ_ADDRESS1         0xA0
#define BUFFER_SIZE1             (countof(aTxBuffer1)-1)
#define BUFFER_SIZE2             (countof(aTxBuffer2)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer1[] =  "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
                        buffer 1 transfer into address sEE_WRITE_ADDRESS1 */ \
                        Example Description \
                        This firmware provides a basic example of how to use the I2C firmware library and\
                        an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the\
                        example is interfacing with M24C64 EEPROM)\
                          \
                        I2C peripheral is configured in Master transmitter during write operation and in\
                        Master receiver during read operation from I2C EEPROM. \
                          \
                        The peripheral used is I2C1 but can be configured by modifying the defines values\
                        in stm324xg_eval.h file. The speed is set to 100kHz and can be configured by \
                        modifying the relative define in stm32_eval_i2c_ee.h file.\
                         \
                        For M24C64 devices all the memory is accessible through the two-bytes \
                        addressing mode and need to define block addresses. In this case, only the physical \
                        address has to be defined (according to the address pins (E0,E1 and E2) connection).\
                        This address is defined in i2c_ee.h (default is 0xA0: E0, E1 and E2 tied to ground).\
                        The EEPROM addresses where the program start the write and the read operations \
                        is defined in the main.c file. \
                         \
                        First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the\
                        written data are read. The written and the read buffers data are then compared.\
                        Following the read operation, the program waits that the EEPROM reverts to its \
                        Standby state. A second write operation is, then, performed and this time, Tx2_Buffer\
                        is written to EEPROM_WriteAddress2, which represents the address just after the last \
                        written one in the first write. After completion of the second write operation, the \
                        written data are read. The contents of the written and the read buffers are compared.\
                         \
                        All transfers are managed in DMA mode (except when 1-byte read/write operation is\
                        required). Once sEE_ReadBuffer() or sEE_WriteBuffer() function is called, the \
                        use application may perform other tasks in parallel while Read/Write operation is\
                        managed by DMA.\
                          \
                        This example provides the possibility to use the STM324xG-EVAL LCD screen for\
                        messages display (transfer status: Ongoing, PASSED, FAILED).\
                        To enable this option uncomment the define ENABLE_LCD_MSG_DISPLAY in the main.c\
                        file.                                                                              ";
uint8_t aTxBuffer2[] =  "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
                        buffer 2 transfer into address sEE_WRITE_ADDRESS2 */";
uint8_t aRxBuffer1[BUFFER_SIZE1];
uint8_t aRxBuffer2[BUFFER_SIZE2];

volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
//volatile uint16_t uhNumDataRead = 0;

/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

//extern void can_recv_thread(void);
//extern	void can_slave_init(void);

//#include "MTD_CPU_CAN3_MCP2515.h"
//#include "MTD_REP_CAN4_MCP2515.h"
//#include "MTD_REP_CAN3_MCP2515.h"

//#include "stm324xg_eval_i2c_ee.h"  //whh ��ͻ���ε�


void Main_task(void *p_arg)
{
	//CPU_SR_ALLOC();	//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//	uint64_t  a;
//	OS_ERR err;
//	static u8 SlaveStartCount;
	
//	static u8 can1StartCount;
//	static u8 can2StartCount;
//	static u8 can1Flag;
//	static u8 can2Flag=1;

	/*iap UART1*/
	u16 UartOldCount=0;	//�ϵĴ��ڽ�������ֵ
	//u16 AppLenth=0;	//���յ���app���볤��	
	
	/*spi can*/
//		Frame_TypeDef frame;

//		frame.SID=3;
//		frame.DLC=8;
//		frame.Type=CAN_STD;
//	
//	(void)p_arg;
//	debugprintf("���뵽Main����\r\n");
	
	/*��ʼ��spi can*/
//	MTDCPU_MCP2515_Init();

	/*������*/
//	bsp_StartTimer(0,2000);
//	Buzzer_Func_On();
//	delay_ms(2000);

	/*	RUNLED */	
//	bsp_StartAutoTimer(1,500);
	
	/*can test*/
//	canInit(CAN1,CAN_BAUD_125K);
//	canInit(CAN2,CAN_BAUD_125K);
	
	/*MTD CPU mcp2515*/
	
//	 MCP2515_REST1_GpioInit();
//	 MTDCPU_MCP2515_Init();
//	 MCP2515_INT1_GpioInit();

///*can3 ��ʼ��*/
//{
//	u8 res;

//	
//		res = MTDREP_CAN3_MCP2515_Init(MCP2515_CAN_BAUD_125K);
////		if(res == MCP2515_OK)
////			return 0;
////		else
////			return CAN_FAILINIT;
//}


/*can4 ��ʼ��*/
//	MCP2515_REST2_GpioInit();
//	MTDREP_CAN4_MCP2515_Init();
//	MCP2515_INT2_GpioInit();


/*CCULED ��ʼ��*/
//CCULED_LEDGpio_Init();


/*EEPROM_2*/
//{
//  /*------------- Initialize sEE_DevStructure -------------*/
//  sEE_DevStructure.sEEAddress = EEPROM_ADDRESS;
//  sEE_DevStructure.sEEPageSize = EEPROM_PAGE_SIZE;
//  sEE_DevStructure.sEEMemoryAddrMode = sEE_OPT_16BIT_REG;

//  /*-------------- Initialize sEE peripheral -------------*/
//  sEE_StructInit(&sEE_DevStructure);
//  sEE_Init(&sEE_DevStructure); 
//	
//	  /*---------------------  Phase1 : Write data from Tx1_Buffer
//  then Read data in Rx1_Buffer -----------------------------*/
//  
//  /* Write Data in EEPROM */
//  sEE_WriteBuffer(&sEE_DevStructure, (uint8_t*)aTxBuffer1, sEE_WRITE_ADDRESS1, BUFFER_SIZE1);
//  
//  /* Wail until communication is complete */
//  while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
//        (sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR))
//  { 
//    /* Application may perform other tasks while CPAL write operation is ongoing */
//  }	 
//  
//  /* Read Data from EEPROM */
//  sEE_ReadBuffer(&sEE_DevStructure,aRxBuffer1 , sEE_READ_ADDRESS1, BUFFER_SIZE1);
//  
//  /* Wail until communication is complete */
//  while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
//        (sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR))
//  {
//    /* Application may perform other tasks while CPAL read operation is ongoing */
//  }
//  
//  /* if Error Occurred reset sEE state */
//  if (sEE_DevStructure.sEEState == sEE_STATE_ERROR)
//  {
//    sEE_DevStructure.sEEState = sEE_STATE_IDLE;
//  }
//  
//		TransferStatus1 = Buffercmp(aTxBuffer1, aRxBuffer1, BUFFER_SIZE1);

//		if (TransferStatus1 == PASSED)
//		{
//			printf(" Transfer 1 PASSED \r\n ");
//		}
//		else
//		{
//			printf(" Transfer 1 FAILED  \r\n");
//		} 
//  /*---------------------  Phase2 : Write data from Tx2_Buffer
//  then Read data in Rx2_Buffer -----------------------------*/
//  
//  /* Write Data in EEPROM */
//  sEE_WriteBuffer(&sEE_DevStructure, aTxBuffer2, sEE_WRITE_ADDRESS2, BUFFER_SIZE2);
//  
//  /* Wail until communication is complete */
//  while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
//        (sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR))
//  { 
//    /* Application may perform other tasks while CPAL write operation is ongoing */	   
//  }
//  
//  /* Read Data from EEPROM */
//  sEE_ReadBuffer(&sEE_DevStructure, aRxBuffer2, sEE_READ_ADDRESS2, BUFFER_SIZE2);
//  
//  /* Wail until communication is complete */
//  while((sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE) && 
//        (sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_ERROR))
//  {
//    /* Application may perform other tasks while CPAL read operation is ongoing */      
//  }
//  
//  /* if Error Occurred reset sEE state */
//  if (sEE_DevStructure.sEEState == sEE_STATE_ERROR)
//  {
//    sEE_DevStructure.sEEState = sEE_STATE_IDLE;
//  }
//  /*----------------- Test transfer phase 1 -----------------*/
//  
//  if (Buffercmp ((uint8_t*)aTxBuffer2,aRxBuffer2,BUFFER_SIZE2) == PASSED )
//  {
//    /* Turn on LED1 if Transfer1 Passed */
//   printf(" Transfer 2 PASSED \r\n ");
//  }
//  else
//  {
//    /* Turn on LED3 if Transfer1 Failed */
//    printf(" Transfer 2 FAILED  \r\n");
//  }
//  
//  /* Deinitialize peripheral */
//  //sEE_DeInit(&sEE_DevStructure);
//}



//		/* Initialize the I2C EEPROM driver ----------------------------------------*/
//		sEE_Init();  
//		delay_ms(10);
//				/*EPPROM*/
//				OS_CRITICAL_ENTER();
//	{
//		u8 i;
//		/*EEPROM ����*/
//		printf("1_EEPROM\r\n");
//		/* First write in the memory followed by a read of the written data --------*/
//		/* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */
//		sEE_WriteBuffer(aTxBuffer1, sEE_WRITE_ADDRESS1, BUFFER_SIZE1); 
//printf("2_EEPROM\r\n");
//		/* Wait for EEPROM standby state */
//		sEE_WaitEepromStandbyState();  
//		printf("3_EEPROM\r\n");
//		/* Set the Number of data to be read */
//		uhNumDataRead = BUFFER_SIZE1;
//		printf("4_EEPROM\r\n");
//		/* Read from I2C EEPROM from sEE_READ_ADDRESS1 */
//		sEE_ReadBuffer(aRxBuffer1, sEE_READ_ADDRESS1, (uint16_t *)(&uhNumDataRead)); 

//printf("5_EEPROM\r\n");
//		/* Starting from this point, if the requested number of data is higher than 1, 
//			 then only the DMA is managing the data transfer. Meanwhile, CPU is free to 
//			 perform other tasks:
//		
//			// Add your code here: 
//			//...
//			//...

//			 For simplicity reasons, this example is just waiting till the end of the 
//			 transfer. */
//	 
//		printf(" Transfer 1 Ongoing \r\n");
//		 /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
//			resets the variable holding the number of data to be read) */
//		 while (uhNumDataRead > 0)
//		{}  
//			
//						for(i=0;i<BUFFER_SIZE1;i++)
//			{
//				printf("r1=%c",aRxBuffer1[i]);
//			}
//			printf("\r\n");
//			
//		 /* Check if the data written to the memory is read correctly */
//		TransferStatus1 = Buffercmp(aTxBuffer1, aRxBuffer1, BUFFER_SIZE1);

//		if (TransferStatus1 == PASSED)
//		{
//			printf(" Transfer 1 PASSED \r\n ");
//		}
//		else
//		{
//			printf(" Transfer 1 FAILED  \r\n");
//		} 		
//	 
//		/* Second write in the memory followed by a read of the written data -------*/
//		/* Write on I2C EEPROM from sEE_WRITE_ADDRESS2 */
//		sEE_WriteBuffer(aTxBuffer2, sEE_WRITE_ADDRESS2, BUFFER_SIZE2); 

//		/* Wait for EEPROM standby state */
//		sEE_WaitEepromStandbyState();  
//		
//		/* Set the Number of data to be read */
//		uhNumDataRead = BUFFER_SIZE2;  
//		
//		/* Read from I2C EEPROM from sEE_READ_ADDRESS2 */
//		sEE_ReadBuffer(aRxBuffer2, sEE_READ_ADDRESS2, (uint16_t *)(&uhNumDataRead));

//		printf(" Transfer 2 Ongoing \r\n");
//		 /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
//			resets the variable holding the number of data to be read) */
//		 while (uhNumDataRead > 0)
//		{}  
//			for(i=0;i<BUFFER_SIZE1;i++)
//			{
//				printf("r2=%c",aRxBuffer2[i]);
//			}
//			printf("\r\n");
//		 /* Check if the data written to the memory is read correctly */
//		TransferStatus1 = Buffercmp(aTxBuffer1, aRxBuffer1, BUFFER_SIZE1);

//		if (TransferStatus1 == PASSED)
//		{
//			printf(" Transfer 2 PASSED \r\n ");
//		}
//		else
//		{
//			printf(" Transfer 2 FAILED  \r\n");
//		} 
//	}
//			OS_CRITICAL_EXIT();//�˳��ٽ���


//��ʼ����ť
CCULED_CanAdd_Gpio_Init();

while(1)
{
		RunLed_Turn();
	
	if(UART1_Rx_Cnt)
	{
		if(UartOldCount==UART1_Rx_Cnt)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
		{
			//AppLenth=UART1_Rx_Cnt;
			UartOldCount=0;
			UART1_Rx_Cnt=0;
			printf("�����������!\r\n");
			
			if((Uart1_Rx_Buff[0]=='u')&&(Uart1_Rx_Buff[1]=='p')&&(Uart1_Rx_Buff[2]=='d')&&(Uart1_Rx_Buff[3]=='a')&&(Uart1_Rx_Buff[4]=='t')&&(Uart1_Rx_Buff[5]=='a'))  //��ʾ���� ��ʹ�������λ
			{
				__set_FAULTMASK(1);		// �ر������ж�
				NVIC_SystemReset();		// ��λ
			}
		}
		else UartOldCount=UART1_Rx_Cnt;	
	}	
	
//	/*��ť����*/
//	{
//		u8 ChooseBoard;
//		ChooseBoard =  CCULED_CanAddVal();
//		printf("LED_BO=0x%x\r\n",ChooseBoard);
//	}
	
	
//	if(	bsp_CheckTimer(0))
//	{
////		Buzzer_Func_Off();
//	}
	
//	if(bsp_CheckTimer(1))
//	{
////		RunLed_Turn();
//	}	
	
	
//		CCULED_LED_Turn(25);
//		CCULED_LED_Turn(24);
	
	/*CCU LED ��˸*/
//	for(a=0;a<26;a++)
////	{
//		CCULED_LED_Turn(a);
//	}
	
	
	//	spi mcp2515 test
	
	/*MTDCPU*/
//	SPICAN_SendData(can2_send,&frame);
	
//	{
//		
//		Message a;
//		a.cob_id=0x08;
//		a.len =8;
//		memcpy(a.data,can2_send,8);	
//		
//		canSend((void *)CAN3,&a);
//	}
	
	/*can4*/
//	MTDREP_CAN4_SPICAN_SendData(can2_send,&frame);
	
//	
//		if(can2Flag ==1 )
//		{
//			can1StartCount++;
//			printf("can1���͵�%d��\r\n",can1StartCount);
//			User_CAN1Transmit(0x01,can2_send,8);
//					
//			if(can1StartCount>=10)
//			{
//				can2Flag=0;
//				can1Flag=1;
//				can1StartCount =0;
//			}
//			delay_ms(100);
//		}
//		
//		if(can1Flag==1)
//		{
//			can2StartCount++;
//			printf("can2���͵�%d��\r\n",can2StartCount);
//			User_CAN2Transmit(0x02,can2_send,8);
//			
//			
//			if(can2StartCount>=10)
//			{
//				can2Flag=1;
//				can1Flag=0;
//				can2StartCount=0;
//			}
//			delay_ms(100);
//		}
//	
	
	BSP_OS_TimeDlyMs(500);
}	
	
	
	
	
//	{

//		debugprintf("������ʱ��3\r\n");
//		debugprintf("����һ���ź���\r\n");
//		
//			//����һ����վ����������ź���
//		OSSemCreate ((OS_SEM*	)&Slave_Start_Sem,
//                 (CPU_CHAR*	)"Slave_Start_Sem",
//                 (OS_SEM_CTR)0,		
//                 (OS_ERR*	)&err);
//		
//			TIM3_Int_Init(20000,8400-1);
//	}

//TIM4_start();
//	{
//		
//		Message m;
//		m.cob_id=1;
//		m.rtr=0;
//		m.len=1;
//		m.data[0]=0xaa;
//		canInit(CAN1,CAN_BAUD_1M);
//		canInit(CAN2,CAN_BAUD_1M);
//		
//		canSend(CAN1,&m);
//		
//		m.cob_id=2;
//		canSend(CAN2,&m);
//	}




//	while(1)
//	{

////		if(KEY_S2_UPDOWM() == Bit_RESET)
////		{
////			LED_Turn(2);
////			debugprintf("MainEnd!\r\n");
////			{
////				door_T_1 +=1;
////				door_T_2 +=1;
////				door_M_1 +=1;
////				door_M_2 +=1;
////				door_T1_1 +=1;
////				door_T1_2 +=1;
////				door_T2_1 +=1;
////				door_T2_2 +=1;
////				door_mc2_1 +=1;
////				door_mc2_2 +=1;
////				door_mc1_1 +=1;
////				door_mc1_2 +=1;
////				sendPDOevent(&TestSlave_Data);
//////				sendOnePDOevent(&TestSlave_Data,10);
////				a=0x12345678912345;
////				debugprintf("64bita=%llx\r\n",a);
////				debugprintf("boolsize=%d\r\n",sizeof(_Bool));
////				debugprintf("g_Recei=%d\r\n",sizeof(g_RxCAN1HeartFlag));
////				
//////				printf("Master: %llx %llx %llx %llx %llx %llx %llx %llx %llx %llx %llx %llx \r\n",
//////				Masterdoor_mc1_1,Masterdoor_mc1_2,Masterdoor_T_1,Masterdoor_T_2,Masterdoor_M_1,Masterdoor_M_2,Masterdoor_T1_1,
//////				Masterdoor_T1_2,Masterdoor_T2_1,Masterdoor_T2_2,Masterdoor_mc2_1,Masterdoor_mc2_2);
////	
////			}
////		}
		
////				//WH��
////		{
////			if(g_RxCAN1HeartFlag[2] == NoRxHeart)
////			{
////				printf("master no rx slave herat!\r\n");
////			}
////			
////			if(g_RxCAN2HeartFlag[1] == NoRxHeart)
////			{
////				printf("slave no rx master herat!\r\n");
////			}
////		}
		
		
//			OSSemPend(&Slave_Start_Sem,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
//			debugprintf("���󵽴�վ���������ź���\r\n");
//			{
//				e_nodeState slavestate;
//				slavestate = getState(&TestSlave_Data);
//				debugprintf("slavestate=0x%x\r\n",slavestate);
//				
//				if(SlaveStartCount >=5)
//				{
//					TIM_Cmd(TIM3,DISABLE);
//				}
//				
//				if(slavestate != 0x05)
//				{
//					debugprintf("slavestate=0x%x\r\n",slavestate);
//					SlaveStartCount++;
//					setNodeId(&TestSlave_Data, 0x02);
//					setState(&TestSlave_Data, Initialisation);
//				}
//				else
//				{
//					TIM_Cmd(TIM3,DISABLE);
//				}
//			}
		
		
		
//	
//	{
//		
//		Message m;
//		m.cob_id=1;
//		m.rtr=0;
//		m.len=1;
//		m.data[0]=0xaa;

//		
//		canSend(CAN1,&m);
//		
//		m.cob_id=2;
//		canSend(CAN2,&m);
//	}

//		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
//	}

}


//void key_task(void *p_arg)		 	//������ʾ�����ջʹ��������Ѿ�cpuʹ��
//{
//	OS_ERR err;
//	uint8_t ucKeyCode;
//	int64_t runtime; 
//	ucKeyCode=0;
//	(void)p_arg;	               /* ������������� */
//	printf("���뵽key����\r\n");
//	while(1)
//	{		
//		if(KEY_S1_UPDOWM() == Bit_RESET)
//		{
//			printf("���뵽��������\r\n");
//			OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);//��ʱ1s	//��������ʱȥ��
//			ucKeyCode	= KEY_Scan(0);	//�ٴ�ȷ�ϰ���״̬
//			if(KEY_S1_UPDOWM() == Bit_RESET)	
//			{
//				ucKeyCode=1;
//			}
//		}
//		
//		if(ucKeyCode==1)
//		{
//			ucKeyCode=0;
//			DispTaskInfo();		//��ӡ��ʾ��Ϣ			
//			runtime=OSTimeGet(&err);
//			printf("runtime=%lld\r\n",runtime);
//			printf("KeyEnd!\r\n");
//		}
//		BSP_OS_TimeDlyMs(2000);//��ʱ2s
//	}
//}

//void TemperCheck_task(void *p_arg)			//�¶ȼ�������������������������¶ȣ���ʵʩ����
//{

//	(void )p_arg;			//��ֹ����
//	LEDGpio_Init();
//	ADC1_CH16_Config();
//	printf("���뵽Temper����\r\n");	

//	while(1)
//	{
//		if(KEY_S1_UPDOWM() == Bit_RESET)
//		{
//			Get_Temprate();		//stm32�ڲ��ļ���¶Ȳ��� ������ADCת���¶�
//			LED_Turn (0);
//		}
//		BSP_OS_TimeDlyMs(3000);//��ʱ2s	,���ҽ����������л�
//	}
//}


void Slave_StartupManager_Take(void *p_arg)
{
	OS_ERR err;
	static u8 SlaveStateCount =0;
	debugprintf("���뵽��վ������������\r\n");
	(void )p_arg;			//��ֹ����
	while(1)
	{
		debugprintf("slave manager\r\n");
		
		if(SlaveStateCount > 5)		//��������5�δӽڵ㵽��ʼ��״̬������bootup
		{
			debugprintf("slave manager 5�ε�\r\n");
			//	SlaveStateCount = 0;
			OSTaskDel((OS_TCB*)&SlaveStartupManagerTCB,&err);	//�����վ����������ִ��5�˺�ɾ�����������
		}						

		{
			e_nodeState slavestate;
			slavestate = getState(&Ccu1LedObjDict_Data);
			debugprintf("slavestate=0x%x\r\n",slavestate);
			if(slavestate != Operational/* 0x05*/)
			{
				debugprintf("slavestate=0x%x\r\n",slavestate);
				SlaveStateCount++;
				setNodeId(&Ccu1LedObjDict_Data, 0x01);
				setState(&Ccu1LedObjDict_Data, Initialisation);
			}
	
			else
			{
				OSTaskDel((OS_TCB*)&SlaveStartupManagerTCB,&err);	//�����վ״̬λ����״̬��ɾ�����������
			}
		}
		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
		
	}

}

extern u32  CCULED_DataCombin(void);
void CCULED_DeviceStatus_Take(void *p_arg)
{
	u32 StateData;
	u8 i;
	(void )p_arg;			//��ֹ����
	
	debugprintf("cculed device take! \r\n");
	while(1)
	{	
		/*W h h*/
//		CCULED_LED_Turn(0);
//		CCULED_LED_Turn(1);
//		HeartMap_CAN1_EMR1 =1;
//		HeartMap_CAN2_EMR1 =1;
		
			debugprintf("cculed!\r\n");
		
		StateData = CCULED_DataCombin();
		
		for(i=0;i<CCULED_LED_NUMBER;i++)		//ѭ���ж�
		{
			if(StateData & 0x01)
			{
		//				CCULED_LED_Turn(i);
				CCULED_LED_On(i);//�޸�Ϊ����
			}
			else
			{
		//				CCULED_LED_Off(i);
				CCULED_LED_Off(i);//�޸�Ϊ����
			}
			StateData >>= 1;
		}
		
		// ������ܵ�������
			HeartMap_CAN1_CANBC = 0;
			HeartMap_CAN1_CCU1 = 0;
			HeartMap_CAN1_CCU2 = 0;
			HeartMap_CAN1_MTD1 = 0;	//MC1_MTD
			HeartMap_CAN1_MTD2 = 0;	//T_MTD
			HeartMap_CAN1_MTD3 = 0;	//M_MTD
			HeartMap_CAN1_MTD4 = 0;	//T1_MTD
			HeartMap_CAN1_MTD5 = 0;	//T2_MTD
			HeartMap_CAN1_MTD6 = 0;	//MC2_MTD
			HeartMap_CAN1_DDU1 = 0;	//DDU1
			HeartMap_CAN1_DDU2 = 0;	//DDU2
			HeartMap_CAN1_EMR1 = 0;	//ERM1
			HeartMap_CAN1_EMR2 = 0;	//ERM2
		
			HeartMap_CAN2_CANBC = 0;
			HeartMap_CAN2_CCU1 = 0;
			HeartMap_CAN2_CCU2 = 0;
			HeartMap_CAN2_MTD1 = 0;	//MC1_MTD
			HeartMap_CAN2_MTD2 = 0;	//T_MTD
			HeartMap_CAN2_MTD3 = 0;	//M_MTD
			HeartMap_CAN2_MTD4 = 0;	//T1_MTD
			HeartMap_CAN2_MTD5 = 0;	//T2_MTD
			HeartMap_CAN2_MTD6 = 0;	//MC2_MTD
			HeartMap_CAN2_DDU1 = 0;	//DDU1
			HeartMap_CAN2_DDU2 = 0;	//DDU2
			HeartMap_CAN2_EMR1 = 0;	//ERM1
			HeartMap_CAN2_EMR2 = 0;	//ERM2
		
		BSP_OS_TimeDlyMs(500);//��ʱ500ms	,���ҽ����������л�,��led����˸
	}
	
}

void assert_failed(u8* file, u32 line)
{
    debug("fail");
		while (1) ;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *         FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}



