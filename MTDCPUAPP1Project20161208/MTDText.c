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
#define Main_TASK_PRIO		4
	//�����ջ��С	
#define Main_STK_SIZE 		512
	//������ƿ�
static OS_TCB MainTaskTCB;
	//�����ջ	
static CPU_STK Main_TASK_STK[Main_STK_SIZE];
	//������
static void Main_task(void *p_arg);


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


/*��վ������������*/
	//�������ȼ�
#define CAN1SLAVE_STARTUPMANAGER_TASK_PRIO		5
	//�����ջ��С	
#define CAN1SLAVE_STARTUPMANAGER_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN1SlaveStartupManagerTCB;
	//�����ջ	
static CPU_STK CAN1SLAVE_STARTUPMANAGER_TASK_STK[CAN1SLAVE_STARTUPMANAGER_STK_SIZE];
	//������
void CAN1Slave_StartupManager_Take(void *p_arg);


/*��վ������������*/
	//�������ȼ�
#define CAN2SLAVE_STARTUPMANAGER_TASK_PRIO		6
	//�����ջ��С	
#define CAN2SLAVE_STARTUPMANAGER_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN2SlaveStartupManagerTCB;
	//�����ջ	
static CPU_STK CAN2SLAVE_STARTUPMANAGER_TASK_STK[CAN2SLAVE_STARTUPMANAGER_STK_SIZE];
	//������
void CAN2Slave_StartupManager_Take(void *p_arg);


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN1SLAVERX_TASK_PRIO		7
	//�����ջ��С	
#define CAN1SLAVERX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN1SlaveRxTaskTCB;
	//�����ջ	
static CPU_STK CAN1SLAVERX_TASK_STK[CAN1SLAVERX_STK_SIZE];
	//������
extern void can1slave_recv_thread(void *p_arg);


/*��վ��������*/
	//�������ȼ�
#define CAN1SLAVETX_TASK_PRIO		8
	//�����ջ��С	
#define CAN1SLAVETX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN1SlaveTxTaskTCB;
	//�����ջ	
static CPU_STK CAN1SLAVETX_TASK_STK[CAN1SLAVETX_STK_SIZE];
	//������
extern void can1slave_send_thread(void *p_arg);


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN2SLAVERX_TASK_PRIO		9
	//�����ջ��С	
#define CAN2SLAVERX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN2SlaveRxTaskTCB;
	//�����ջ	
static CPU_STK CAN2SLAVERX_TASK_STK[CAN2SLAVERX_STK_SIZE];
	//������
extern void can2slave_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN2SLAVETX_TASK_PRIO		10
	//�����ջ��С	
#define CAN2SLAVETX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN2SlaveTxTaskTCB;
	//�����ջ	
static CPU_STK CAN2SLAVETX_TASK_STK[CAN2SLAVETX_STK_SIZE];
	//������
extern void can2slave_send_thread(void *p_arg);

/*��ʱ��������*/
	//�������ȼ�
#define LOOPSEND_TASK_PRIO		4
	//�����ջ��С	
#define LOOPSEND_STK_SIZE 		512
	//������ƿ�
static OS_TCB LoopSendTaskTCB;
	//�����ջ	
static CPU_STK LOOPSEND_TASK_STK[LOOPSEND_STK_SIZE];
	//������
extern void loop_send_thread(void *p_arg);

/*OLED Key����*/
	//�������ȼ�
#define OLEDKEY_TASK_PRIO		11
	//�����ջ��С	
#define OLEDKEY_STK_SIZE 		512
	//������ƿ�
static OS_TCB OledKeyTaskTCB;
	//�����ջ	
static CPU_STK OLEDKEY_TASK_STK[OLEDKEY_STK_SIZE];
	//������
void Oled_Key_thread(void *p_arg);



	/*�������Ĵ���*/
static  OS_SEM     	SEM_SYNCH;	   //����ͬ��
OS_SEM     	Slave_Start_Sem;			//��վ����������ź���

///*����ʹ�õ�ȫ�ֱ����Ķ���*/

u8 g_RxDataFlag=0;
u8 g_FrameErrorFlag=0;

/*canfestival �����������ĵĶ���*/

//IsNoReceHeart			 g_RxCAN1HeartFlag[15]={0};					//��Ÿ����豸�������ı�־���±���ǽڵ��ID�ţ����±�Ϊ0������û��ʹ�á�
//IsNoReceHeart			 g_RxCAN2HeartFlag[15]={0};

//CurReceHeartState  g_RxCAN1HeartStateFlag[15] = {0};		//��ŵ�ǰ��������ʱ�����豸���ڵ�״̬�����ϵ�ճ�ʼ����ʱ�򣬻��ǽ��������ӵ�ʱ��		
//CurReceHeartState  g_RxCAN2HeartStateFlag[15] = {0};

//RxCanMessageState  g_RxCAN1MesStateFlag[15]={0};						//��ŵ�ǰ��վ�ӵ���վ��can��Ϣ ������״̬ 
//RxCanMessageState	 g_RxCAN2MesStateFlag[15]={0};

CurrentUserState CurrentUseCAN1 = UseState;			/*��ʾ��ǰʹ��can1���ߣ�����can2���ߣ�Ĭ��ʹ��can1����*/
CurrentUserState CurrentUseCAN2 = NoUseState;	

extern CO_Data MTDCAN1Slave_Data;
extern CO_Data MTDCAN2Slave_Data;

u8 can1CarNumberFlag = false;
u8 can2CarNumberFlag = false;
u8 can1UpdateTimeFlag = false;
u8 can2UpdateTimeFlag = false;

extern __IO uint16_t PWM2DutyCycle;
extern __IO uint32_t PWM2Frequency ;
extern __IO uint16_t PWM1DutyCycle;
extern __IO uint32_t PWM1Frequency;

u8 ChooseBoard=0;			//���ȫ�ֱ�������������ť��ֵ���Ӷ�ѡ�����


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
		Uart1_init();//uart1��GPIO��ʼ�� ,��ǰ�ĳ�ʼ������ֹ�м��д�ӡ��ʾ��Ϣ���޷����
	
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
		OS_ERR      err;
   (void)p_arg;
    BSP_Init();                 /*���е�����ĳ�ʼ����������*/     /* Initialize BSP functions                             */
    CPU_Init();                                                	 /* Initialize the uC/CPU services                       */ 
	//  BSP_Tick_Init();                                            /* Start Tick Initialization                            */
	//  Mem_Init();                                                 /* Initialize Memory Management Module                  */
	//  Math_Init();                                                /* Initialize Mathematical Module                       */

	
	///* Configure the NVIC Preemption Priority Bits */		//SD���жϵ�������sd���ļ��Ѿ�д��
	//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//#if defined (SD_DMA_MODE)     //�жϱ������ã����򴮿��޷���ʾ���
	//	NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//	NVIC_Init(&NVIC_InitStructure);
	//#endif

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
                 (OS_TASK_PTR)Main_task, 			//������
                 (void		  *)0,					//���ݸ��������Ĳ���
                 (OS_PRIO	   )Main_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&Main_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)Main_TASK_STK[Main_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)Main_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/,		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ
								 
								 
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
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/,		//����ѡ��
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
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ


/*W H H*/
		//������վ������������
	OSTaskCreate((OS_TCB 		* )&CAN1SlaveStartupManagerTCB,				//������ƿ�
								(CPU_CHAR	* )"CAN1SlaveStartManager_task", 			//��������
                 (OS_TASK_PTR)CAN1Slave_StartupManager_Take, 		//������
                 (void		  *)0,														//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1SLAVE_STARTUPMANAGER_TASK_PRIO,    	//�������ȼ�
                 (CPU_STK   *)&CAN1SLAVE_STARTUPMANAGER_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1SLAVE_STARTUPMANAGER_TASK_STK[CAN1SLAVE_STARTUPMANAGER_STK_SIZE/10],	//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1SLAVE_STARTUPMANAGER_STK_SIZE,			//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
								 
		//������վ������������
	OSTaskCreate((OS_TCB 		* )&CAN2SlaveStartupManagerTCB,				//������ƿ�
								(CPU_CHAR	* )"CAN2SlaveStartManager_task", 			//��������
                 (OS_TASK_PTR)CAN2Slave_StartupManager_Take, 		//������
                 (void		  *)0,														//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2SLAVE_STARTUPMANAGER_TASK_PRIO,    	//�������ȼ�
                 (CPU_STK   *)&CAN2SLAVE_STARTUPMANAGER_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2SLAVE_STARTUPMANAGER_TASK_STK[CAN2SLAVE_STARTUPMANAGER_STK_SIZE/10],	//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2SLAVE_STARTUPMANAGER_STK_SIZE,			//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
				 

		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN1SlaveRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN1SlaveRx_task", 			//��������
                 (OS_TASK_PTR)can1slave_recv_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1SLAVERX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN1SLAVERX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1SLAVERX_TASK_STK[CAN1SLAVERX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1SLAVERX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				


		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN1SlaveTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN1SlaveTx_task", 			//��������
                 (OS_TASK_PTR)can1slave_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1SLAVETX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN1SLAVETX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1SLAVETX_TASK_STK[CAN1SLAVETX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1SLAVETX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		


		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN2SlaveRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN2SlaveRx_task", 			//��������
                 (OS_TASK_PTR)can2slave_recv_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2SLAVERX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN2SLAVERX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2SLAVERX_TASK_STK[CAN2SLAVERX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2SLAVERX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				


		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN2SlaveTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN2SlaveTx_task", 			//��������
                 (OS_TASK_PTR)can2slave_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2SLAVETX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN2SLAVETX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2SLAVETX_TASK_STK[CAN2SLAVETX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2SLAVETX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
								 
		///*��ʱ��������*/
	OSTaskCreate((OS_TCB 		* )&LoopSendTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"LoopSend_task", 			//��������
                 (OS_TASK_PTR)loop_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )LOOPSEND_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&LOOPSEND_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)LOOPSEND_TASK_STK[LOOPSEND_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)LOOPSEND_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ					
								 
								 
		///*��ʱ��������*/
	OSTaskCreate((OS_TCB 		* )&OledKeyTaskTCB,					//������ƿ�
								(CPU_CHAR	* )"OledKey_task", 					//��������
                 (OS_TASK_PTR)Oled_Key_thread, 				//������
                 (void		  *)0,											//���ݸ��������Ĳ���
                 (OS_PRIO	   )OLEDKEY_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&OLEDKEY_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)OLEDKEY_TASK_STK[OLEDKEY_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)OLEDKEY_STK_SIZE,		//�����ջ��С
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


//extern u8 User_CAN1Transmit(uint32_t id,uint8_t* val,uint8_t Len);
//extern u8 User_CAN2Transmit(uint32_t id,uint8_t* val,uint8_t Len);
//extern unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);


///*EEPROM ����*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define sEE_WRITE_ADDRESS1        0xB0
#define sEE_READ_ADDRESS1         0xB0
#define BUFFER_SIZE1             (countof(aTxBuffer1)-1)
#define BUFFER_SIZE2             (countof(aTxBuffer2)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer1[] =   "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
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
#include "can.h"
#include "config.h"
#include "RunLed.h"
#include "MTD_CPU_CAN3_MCP2515.h"
#include "RTC_ISL1208.h" 
#include "OLED_SSD1325.h"

//#include "stm324xg_eval_i2c_ee.h"
#include "main.h"
#include "OLED_Menu.h"
//#define OLED_BASE     	 ((uint32_t)(0x6C000000))
//#define OLED_CMD		*(__IO uint16_t *)(OLED_BASE)								// D/C=0: Command
//#define OLED_DATA		*(__IO uint16_t *)(OLED_BASE + (1 << 18))		/* FSMC_A18��D/C ,D/C=1: Data�����õ�8bitģʽ*/


//#define MC1_CAR_TYPE	"MC1(1)"
//#define T_CAR_TYPE		"T(4)"
//#define M_CAR_TYPE		"M(3)"
//#define T1_CAR_TYPE		"T1(5)"
//#define T2_CAR_TYPE		"T2(6)"
//#define MC2_CAR_TYPE	"MC2(2)"



///*W H H*/
//u8 oled_read_Date(u8 *pBuffer)
//{
//	*pBuffer =  OLED_DATA;
//}

	/*�����*/
	u16		CarNumber=4031;
	u8    CarNo=1;
	char cartype[10];

void Main_task(void *p_arg)
{
//	CPU_SR_ALLOC();	//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//	uint64_t  a;
//	OS_ERR err;
//	static u8 SlaveStartCount;
//	static u8 can1StartCount;
//	static u8 can2StartCount;
//	static u8 can1Flag;
//	static u8 can2Flag=1;
//		RTC_TIME time;

	
	/*spi can*/
//		Frame_TypeDef frame;
//		frame.SID=10;
//		frame.DLC=8;
//		frame.Type=CAN_STD;
//	
	(void)p_arg;
	debugprintf("���뵽Main����\r\n");
	
	
	/*pwm*/
//	MTDCPU_PWM2IN_TIM5_CH1_Init(0xFFFF,84-1);		//pwm�����ʼ����1M��Ƶ�ʲ�׽
//	MTDCPU_PWM1IN_TIM2_CH2_Init(0xFFFF,84-1);
	
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
	
	/*mcp2515*/
//	 MCP2515_REST1_GpioInit();
//	 MTDCPU_MCP2515_Init();
//	 MCP2515_INT1_GpioInit();
	 
	 /*RTC*/
//	 time._year = 16;
//	 time._month=7;
//	 time._day=23;
//	 time._hour=18;
//	 time._min=54;
//	 time._sec=20;
//	 time._week=6;
//	 
//	 ISL1208_Init();
//	 Auto_Time_Set();
//	Manual_Set_Time(&time);
	
	/*can3 ��ʼ��*/
//{
//	u8 res;

//	
//		res = MTDCPU_CAN3_MCP2515_Init(MCP2515_CAN_BAUD_125K);
////		if(res == MCP2515_OK)
////			return 0;
////		else
////			return CAN_FAILINIT;
//}

	/*pwm���*/
//	bsp_SetTIMOutPWM(BUZZER_GPIO,BUZZER_GPIO_PIN,TIM1,1,1000,9000);  //������

//	MTDCPU_PWM2IN_TIM5_CH1_Init(0xFFFF,84-1);		//pwm�����ʼ����1M��Ƶ�ʲ�׽
//	MTDCPU_PWM1IN_TIM2_CH2_Init(0xFFFF,84-1);
	
	
//Buzzer_Func_On();


///*EEPROM_2*/
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



///*EEPROM*/
//		/* Initialize the I2C EEPROM driver ----------------------------------------*/
//		sEE_Init();  
//		delay_ms(10);
//			
//	
//		/*EPPROM*/
//	OS_CRITICAL_ENTER();
//	{
//		u8 i;
//		/*EEPROM ����*/
//		printf("1_EEPROM\r\n");
//		/* First write in the memory followed by a read of the written data --------*/
//		/* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */
//		sEE_WriteBuffer(aTxBuffer1, sEE_WRITE_ADDRESS1, BUFFER_SIZE1); 
//			printf("2_EEPROM\r\n");
//		/* Wait for EEPROM standby state */
//		sEE_WaitEepromStandbyState();  
//		printf("3_EEPROM\r\n");
//		/* Set the Number of data to be read */
//		uhNumDataRead = BUFFER_SIZE1;
//		printf("4_EEPROM\r\n");
//		delay_ms(10);
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
////			for(i=0;i<BUFFER_SIZE1;i++)
////			{
////				printf("r1=%c",aRxBuffer1[i]);
////			}
////			printf("\r\n");
//			printf("Size1=%d\r\n",BUFFER_SIZE1);
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

//		/* Second write in the memory followed by a read of the written data -------*/
//		/* Write on I2C EEPROM from sEE_WRITE_ADDRESS2 */
//		sEE_WriteBuffer(aTxBuffer2, sEE_WRITE_ADDRESS2, BUFFER_SIZE2); 

//		/* Wait for EEPROM standby state */
//		sEE_WaitEepromStandbyState();  
//		
//		/* Set the Number of data to be read */
//		uhNumDataRead = BUFFER_SIZE2;  
//		
//		delay_ms(10);
//		/* Read from I2C EEPROM from sEE_READ_ADDRESS2 */
//		sEE_ReadBuffer(aRxBuffer2, sEE_READ_ADDRESS2, (uint16_t *)(&uhNumDataRead));

//		printf(" Transfer 2 Ongoing \r\n");
//		 /* Wait till DMA transfer is complete (Transfer complete interrupt handler 
//			resets the variable holding the number of data to be read) */
//		 while (uhNumDataRead > 0)
//		{}  
////			for(i=0;i<BUFFER_SIZE1;i++)
////			{
////				printf("r2=%c",aRxBuffer2[i]);
////			}
////			printf("\r\n");
//				 printf("Size2=%d\r\n",BUFFER_SIZE2);
//		 /* Check if the data written to the memory is read correctly */
//		TransferStatus1 = Buffercmp(aTxBuffer2, aRxBuffer2, BUFFER_SIZE1);

//		if (TransferStatus1 == PASSED)
//		{
//			printf(" Transfer 2 PASSED \r\n ");
//		}
//		else
//		{
//			printf(" Transfer 2 FAILED  \r\n");
//		} 
//		  /* Free all used resources */
//	}
//			OS_CRITICAL_EXIT();//�˳��ٽ���
//		
		
	

/*OLED*/

		switch(ChooseBoard)
		{
			case MC1_MTD_NODEID:
				memcpy(cartype,MC1_CAR_TYPE,sizeof(MC1_CAR_TYPE));
				CarNumber = 4011;
				CarNo=1;
				break;
			case T_MTD_NODEID:
				memcpy(cartype,T_CAR_TYPE,sizeof(T_CAR_TYPE));
				CarNumber = 4012;
				CarNo=2;	
				break;
			case M_MTD_NODEID:
				memcpy(cartype,M_CAR_TYPE,sizeof(M_CAR_TYPE));
				CarNumber = 4013;
				CarNo=3;
				break;
			case T1_MTD_NODEID:
				memcpy(cartype,T1_CAR_TYPE,sizeof(T1_CAR_TYPE));
				CarNumber = 4014;
				CarNo=4;
				break;
			case T2_MTD_NODEID:
				memcpy(cartype,T2_CAR_TYPE,sizeof(T2_CAR_TYPE));
				CarNumber = 4015;
				CarNo=5;	
				break;
			case MC2_MTD_NODEID:
				memcpy(cartype,MC2_CAR_TYPE,sizeof(MC2_CAR_TYPE));
				CarNumber = 4016;
				CarNo=6;
				break;
			default :
				break;
		}

		/*��while���Ѿ���ʾ��*/
//	ISL1208_GetTime(&time);		//��ȡʱ��
//	
//	sprintf(pbuf,"%d%s%d%s%d%s%d%s%d",time._year+2000,"-",time._month,"-",time._day,"  ",time._hour,":",time._min);
////OLED��Ļ���ܳ��磬����ͻ�������
//{
////	Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
//	
//	Draw_Component(COMPONENT_LABEL_ENABLED,  1, 0,  0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);                 /* ����Agreement���� */
//	Draw_Component(COMPONENT_LABEL_ENABLED,  2, 16, 0,  16,  0,   0, 0, 0, (unsigned char *)"�������:  4031");    /* ���������ı� */
//	Draw_Component(COMPONENT_LABEL_ENABLED,  3, 32, 0,  16,  0,   0, 0, 0, (unsigned char *)"��    ��:  1 ");
//	Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)"���ͱ��:  MC1(1)");                     /* ����ȷ�ϰ�ť */

////	Change_Index(1);                                                                                    /* ���������һλ������Զ�ѡ��Button */
//	Update_Component(4);                                                                                /* �������״̬ */
//	Next_Frame(); 

//}
		
//			Auto_Time_Set();
		
//			time._year=16;
//			time._month=10;
//			time._day=8;
//			time._sec=50;
//			time._min=45;
//			time._hour=20;		
//		OS_CRITICAL_ENTER();
//		Manual_Set_Time(&time);	//W H H ����
//		OS_CRITICAL_EXIT();//�˳��ٽ���

while(1)
{
	
	RunLed_Turn();
	
//	/*���԰���*/
//	a=MTDCPU_KEY_Scan(0);
//	switch(a)
//	{
//		case 1:
//			printf("����1���£�\r\n");
//			break;
//		case 2:
//			printf("����2���£�\r\n");		
//		break;
//		case 3:
//			printf("����3���£�\r\n");
//		break;
//		case 4:
//			printf("����4���£�\r\n");
//		break;
//	}
	
//	if(can1CarNumberFlag == true )
//	{
//		debugprintf("can1car !\r\n");
//		CarNo = can1_Car_Number_1;
//		CarNumber = can1_Car_Number_2;		//�������
//		can1CarNumberFlag = false;
//	}
//	
//	if(can2CarNumberFlag == true )
//	{
//		debugprintf("can2car !\r\n");
//		CarNo = can2_Car_Number_1;
//		CarNumber = can2_Car_Number_2;		//�������
//		can2CarNumberFlag =false;
//	}
//	
//	if(can1UpdateTimeFlag == true)
//	{
//		debugprintf("can1Data !\r\n");
//		time._year = can1_Car_DateTime[1];
//		time._month = can1_Car_DateTime[2];
//		time._day = can1_Car_DateTime[3];
//		time._hour = can1_Car_DateTime[4];
//		time._min = can1_Car_DateTime[5];
//		time._sec = can1_Car_DateTime[6];
//		Manual_Set_Time(&time);
//		can1UpdateTimeFlag = false;
//	}
//	
//	if(can2UpdateTimeFlag == true)
//	{
//		debugprintf("can2Data !\r\n");
//		time._year = can1_Car_DateTime[1];
//		time._month = can1_Car_DateTime[2];
//		time._day = can1_Car_DateTime[3];
//		time._hour = can1_Car_DateTime[4];
//		time._min = can1_Car_DateTime[5];
//		time._sec = can1_Car_DateTime[6];
//		Manual_Set_Time(&time);
//		can2UpdateTimeFlag =false;
//	}
//	
//	//�����ٽ��� ����Ӧ��д������
//	OS_CRITICAL_ENTER();
//		ISL1208_GetTime(&time);		//��ȡʱ��
//	OS_CRITICAL_EXIT();//�˳��ٽ���
//	
//	Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
//	
//	sprintf(pbuf,"%d%s%02d%s%02d%02d%02d%02d",time._year+2000,"-",time._month,"-",time._day,time._hour,time._min,time._sec);
//		
//	Draw_Component(COMPONENT_LABEL_ENABLED,  1, 0,  0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);     /* �����ı� */

//	memset(pbuf,0,sizeof(pbuf));
//	sprintf(pbuf,"%s%d","�������: ",CarNumber);	
//	Draw_Component(COMPONENT_LABEL_ENABLED,  2, 16, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf); 	  /* �����ı� */
//	
//	memset(pbuf,0,sizeof(pbuf));
//	sprintf(pbuf,"%s%d","��    ��: ",CarNo);		
//	Draw_Component(COMPONENT_LABEL_ENABLED,  3, 32, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf); 	 	/* �����ı� */
//		
//	memset(pbuf,0,sizeof(pbuf));
//	sprintf(pbuf,"%s%s", "���ͱ��: ",cartype);	
//	Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);    /* �����ı� */               
//	
//	Update_Component(0);                                                                              /* �������״̬ */

//	Draw_Reverse(32,0,48,128,0);//��ɫ
//	Next_Frame(); 
//	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
//	/*PWM�����ӡ*/
//	{
//		printf("PWM1ռ�ձ�%d%%\r\n",PWM1DutyCycle);
//		printf("PWM1Ƶ��%dus\r\n",PWM1Frequency);
//		printf("PWM2ռ�ձ�%d%%\r\n",PWM2DutyCycle);
//		printf("PWM2Ƶ��%dus\r\n",PWM2Frequency);
////		printf("PA0=%d\r\n",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0));
////		printf("PA1=%d\r\n",GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1));
//	}
	
	
	
	
	
	
	
	
	
	
	
	
//	if(	bsp_CheckTimer(0))
//	{
//		Buzzer_Func_Off();
//	}
	
//	if(bsp_CheckTimer(1))
//	{
//		RunLed_Turn();
//	}	
	
	/*MTDCPU*/
//	SPICAN_SendData(can2_send,&frame);
//	
//	{
//		
//		Message a;
//		a.cob_id=0x08;
//		a.len =8;
//		memcpy(a.data,can2_send,8);	
//		
//		canSend((void *)CAN3,&a);
//	}
//	

		/*OLED*/

//	Fill_RAM_12864(1);
//	Checkerboard_12864();
//	Grayscale_12864();
//	Show_Frame_12864();
//Draw_Rectangle_12864(10,20,10,20,0xff);
//Show_Pattern_12864((unsigned char *)szASC16,0,10,0,10,0);
//{
//	unsigned char  Name[]={53,78,73,86,73,83,73,79,78,0};
//Show_String_12864(1,Name,0,0);
	
	
//}

//OLED_Draw_16x16String(0,0,0,6);

//{
//		OLED_DATA = 0x08;
//		a=	OLED_DATA;
//		printf("a=%lld\r\n",a);
//	}
//{
//	FONT_T a;
//	a.FontCode=FC_ST_16;
//	a.FrontColor=0;
//	a.BackColor=0;
//	a.Space=0;
////	OLED_DispStr(0,0,"�������",&a);
//		OLED_DispStr(0,0,"ABCD",&a);
//	
//	a.FontCode =FC_ST_12;
//	OLED_DispStr(0,16,"�������",&a);
////	
////	
////	Show_String_12864(1,"ABCDEFG",0,32);
////	Show_String_12864(1,"2",0,40);
////	
////	Draw_String(0,50,1,"2015-05-07  15:24",0xff,0x00);
////	Next_Frame(); 
//}


///*�滭���*/
//{
//Draw_Component(COMPONENT_LABEL_ENABLED,  1, 0,  0,  6,  0,   0, 0, 0, "Agreement");                 /* ����Agreement���� */
//Draw_Component(COMPONENT_LABEL_ENABLED,  2, 10, 0,  4,  0,   0, 0, 0, "COPYRIGHT RIJN, 2014. ");    /* ���������ı� */
//Draw_Component(COMPONENT_LABEL_ENABLED,  3, 16, 0,  4,  0,   0, 0, 0, "PIXELNFINITE.COM ");
//Draw_Component(COMPONENT_BUTTON_ENABLED, 4, 50, 80, 62, 127, 0, 0, 0, "Agree");                     /* ����ȷ�ϰ�ť */

////Change_Index(1);                                                                                    /* ���������һλ������Զ�ѡ��Button */
//Update_Component(4);                                                                                /* �������״̬ */
//Next_Frame();   
//}



////OLED��Ļ���ܳ��磬����ͻ�������
//{
////	Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
//	
//	Draw_Component(COMPONENT_LABEL_ENABLED,  1, 0,  0,  16,  0,   0, 0, 0, pbuf);                 /* ����Agreement���� */
//	Draw_Component(COMPONENT_LABEL_ENABLED,  2, 16, 0,  16,  0,   0, 0, 0, "�������: 4031");    /* ���������ı� */
//	Draw_Component(COMPONENT_LABEL_ENABLED,  3, 32, 0,  16,  0,   0, 0, 0, "��    ��: 1 ");
//	Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, "���ͱ��: MC1(1)");                     /* ����ȷ�ϰ�ť */
////	
////	Change_Index(1);                                                                                    /* ���������һλ������Զ�ѡ��Button */
//	Update_Component(4);                                                                                /* �������״̬ */
//	Next_Frame(); 

//}


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
		
//		{
//			/*RTC*/
//			ISL1208_GetTime(&time);
//			printf("_yera=%d\r\n",time._year);
//			printf("_month=%d\r\n",time._month);
//			printf("_day=%d\r\n",time._day);
//			printf("_hour=%d\r\n",time._hour);
//			printf("_min=%d\r\n",time._min);
//			printf("_sec=%d\r\n",time._sec);
//			printf("_week=%d\r\n",time._week);
//			
//		}
	
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

//void led_task(void *p_arg)
//{
//	printf("���뵽led����\r\n");
//  while (1)
//  {	
//		
//		LED_Turn(0);
//		LED_Turn(1);
//		LED_Turn(2);
//		LED_Turn(3);
////		printf("LedEnd!\r\n");
//		BSP_OS_TimeDlyMs(3000);	 //��ʱ1s
//  }
//}

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
////			LED_Turn (0);
//		}
//		BSP_OS_TimeDlyMs(3000);//��ʱ2s	,���ҽ����������л�
//	}
//}

extern int  can1test_slave(void);
extern int  can2test_slave(void);
void CAN1Slave_StartupManager_Take(void *p_arg)
{
	OS_ERR err;
	static u8 SlaveStateCount =0;

	debugprintf("���뵽��վ������������\r\n");

	while(1)
	{
		debugprintf("can1 slave manager\r\n");
		
		if(SlaveStateCount > 5 )		//��������5�δӽڵ㵽��ʼ��״̬������bootup
		{
			debugprintf("can1 slave manager 5�ε�\r\n");
			//	SlaveStateCount = 0;
			OSTaskDel((OS_TCB*)&CAN1SlaveStartupManagerTCB,&err);	//�����վ����������ִ��5�˺�ɾ�����������
		}						

		{
			e_nodeState slavestate;
			slavestate = getState(&MTDCAN1Slave_Data);
			debugprintf("can1slavestate=0x%x\r\n",slavestate);
			if(slavestate != Operational/* 0x05*/)
			{
//				debugprintf("can1slavestate=0x%x\r\n",slavestate);
//				SlaveStateCount++;
//				switch(ChooseBoard)
//				{
//					case MC1_MTD_NODEID:
//						setNodeId(&MTDCAN1Slave_Data,MC1_MTD_NODEID);
//						break;
//					case T_MTD_NODEID:
//						setNodeId(&MTDCAN1Slave_Data,T_MTD_NODEID);
//						break;
//					case M_MTD_NODEID:
//						setNodeId(&MTDCAN1Slave_Data,M_MTD_NODEID);
//						break;
//					case T1_MTD_NODEID:
//						setNodeId(&MTDCAN1Slave_Data,T1_MTD_NODEID);
//						break;
//					case T2_MTD_NODEID:
//						setNodeId(&MTDCAN1Slave_Data,T2_MTD_NODEID);
//						break;
//					case MC2_MTD_NODEID:
//						setNodeId(&MTDCAN1Slave_Data,MC2_MTD_NODEID);
//						break;
//					default :
//						break;
//				}
//				setState(&MTDCAN1Slave_Data, Initialisation);
	
					/*����һ�������ź� cansend*/
					
					u8 BootMsg =0x00;
					uint32_t id= (0x700 | ChooseBoard);
					SlaveStateCount++;
					User_CAN1Transmit(id,&BootMsg,1);
					
			}

			else
			{
				OSTaskDel((OS_TCB*)&CAN1SlaveStartupManagerTCB,&err);	//�����վ״̬λ����״̬��ɾ�����������
			}

		}

		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
		
	}

}


void CAN2Slave_StartupManager_Take(void *p_arg)
{
	OS_ERR err;
	static u8 SlaveStateCount =0;
	debugprintf("���뵽��վ������������\r\n");
	
	while(1)
	{
		debugprintf("slave manager\r\n");
		
		if(SlaveStateCount > 5)		//��������5�δӽڵ㵽��ʼ��״̬������bootup
		{
			debugprintf("can2slave manager 5�ε�\r\n");
			//	SlaveStateCount = 0;
			OSTaskDel((OS_TCB*)&CAN2SlaveStartupManagerTCB,&err);	//�����վ����������ִ��5�˺�ɾ�����������
		}						

		{
			e_nodeState slavestate;
			slavestate = getState(&MTDCAN2Slave_Data);
			debugprintf("can2slavestate=0x%x\r\n",slavestate);
			if(slavestate != Operational/* 0x05*/)
			{
//				debugprintf("can2slavestate=0x%x\r\n",slavestate);
//				SlaveStateCount++;
//				switch(ChooseBoard)
//				{
//					case MC1_MTD_NODEID:
//						setNodeId(&MTDCAN2Slave_Data,MC1_MTD_NODEID);
//						break;
//					case T_MTD_NODEID:
//						setNodeId(&MTDCAN2Slave_Data,T_MTD_NODEID);
//						break;
//					case M_MTD_NODEID:
//						setNodeId(&MTDCAN2Slave_Data,M_MTD_NODEID);
//						break;
//					case T1_MTD_NODEID:
//						setNodeId(&MTDCAN2Slave_Data,T1_MTD_NODEID);
//						break;
//					case T2_MTD_NODEID:
//						setNodeId(&MTDCAN2Slave_Data,T2_MTD_NODEID);
//						break;
//					case MC2_MTD_NODEID:
//						setNodeId(&MTDCAN2Slave_Data,MC2_MTD_NODEID);
//						break;
//					default :
//						break;
//				}
//				setState(&MTDCAN2Slave_Data, Initialisation);

					/*����һ�������ź� cansend*/
					u8 BootMsg =0x00;
					uint32_t id= (0x700 | ChooseBoard);
					SlaveStateCount++;
					User_CAN2Transmit(id,&BootMsg,1);
			}
	
			else
			{
				OSTaskDel((OS_TCB*)&CAN2SlaveStartupManagerTCB,&err);	//�����վ״̬λ����״̬��ɾ�����������
			}
		}
		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
		
	}

}


void loop_send_thread(void *p_arg)
{
	static u8 sendFlag=0;		//����ʱ��
	e_nodeState slavestate;
	OS_ERR err;
//	static u8 doordirection=0;
	(void)p_arg; /* ������������� */
	
	if(ChooseBoard == MC1_MTD_NODEID)
	{
		can1_car_Direction =1;
		can2_car_Direction =1;
	}
	
	while(1)
	{	
		//MTD cpu�﷢�͵�ʱ������can���綼���ͣ�ֻ�н������ݵ�ʱ��ŷ��Ƿ������can�ڵ�����

			slavestate = getState(&MTDCAN1Slave_Data);
			if(slavestate == Operational/* 0x05*/)
			{			

				if(sendFlag >8)
				{
					sendFlag =1;
				}
//					printf("sed=%d\r\n",sendFlag);
				switch(sendFlag)
				{
					case 1:
						can1_Door_mc1_1 =0x11111111;
						can1_Door_T_1 =	 0x11111111;
						can1_Door_M_1 =  0x11111111;
						can1_Door_T1_1 = 0x11111111;
						can1_Door_T2_1 = 0x11111111;
						can1_Door_mc2_1 =0x11111111;
						break;
					case 2:
						can1_Door_mc1_1 =0x22222222;
						can1_Door_T_1 =	 0x22222222;
						can1_Door_M_1 =  0x22222222;
						can1_Door_T1_1 = 0x22222222;
						can1_Door_T2_1 = 0x22222222;
						can1_Door_mc2_1 = 0x22222222;		
						break;
					case 3:
						can1_Door_mc1_1 =0x33333333;
						can1_Door_T_1 =	 0x33333333;
						can1_Door_M_1 =  0x33333333;
						can1_Door_T1_1 = 0x33333333;
						can1_Door_T2_1 = 0x33333333;
						can1_Door_mc2_1 = 0x33333333;		
						break;
					case 4:
						can1_Door_mc1_1 =0x44444444;
						can1_Door_T_1 =	 0x44444444;
						can1_Door_M_1 =  0x44444444;
						can1_Door_T1_1 = 0x44444444;
						can1_Door_T2_1 = 0x44444444;
						can1_Door_mc2_1 =0x44444444;	
						break;
				  case 5:
						can1_Door_mc1_1 =0x55555555;
						can1_Door_T_1 =	 0x55555555;
						can1_Door_M_1 =  0x55555555;
						can1_Door_T1_1 = 0x55555555;
						can1_Door_T2_1 = 0x55555555;
						can1_Door_mc2_1 =0x55555555;
						break;
					case 6:
						can1_Door_mc1_1 =0x66666666;
						can1_Door_T_1 =	 0x66666666;
						can1_Door_M_1 =  0x66666666;
						can1_Door_T1_1 = 0x66666666;
						can1_Door_T2_1 = 0x66666666;
						can1_Door_mc2_1 = 0x66666666;		
						break;
					case 7:
						can1_Door_mc1_1 =0x77777777;
						can1_Door_T_1 =	 0x77777777;
						can1_Door_M_1 =  0x77777777;
						can1_Door_T1_1 = 0x77777777;
						can1_Door_T2_1 = 0x77777777;
						can1_Door_mc2_1 = 0x77777777;	
						break;
					case 8:
						can1_Door_mc1_1 = 0x88888888;
						can1_Door_T_1 =	  0x88888888;
						can1_Door_M_1 =   0x88888888;
						can1_Door_T1_1 =  0x88888888;
						can1_Door_T2_1 =  0x88888888;
						can1_Door_mc2_1 = 0x88888888;	
						break;
				}

					switch(sendFlag)
				{
					case 1:
						can2_Door_mc1_1 =0x11111111;
						can2_Door_T_1 =	0x11111111;
						can2_Door_M_1 = 0x11111111;
						can2_Door_T1_1 = 0x11111111;
						can2_Door_T2_1 = 0x11111111;
						can2_Door_mc2_1 = 0x11111111;
						break;
					case 2:
						can2_Door_mc1_1 =0x22222222;
						can2_Door_T_1 =	0x22222222;
						can2_Door_M_1 = 0x22222222;
						can2_Door_T1_1 = 0x22222222;
						can2_Door_T2_1 = 0x22222222;
						can2_Door_mc2_1 = 0x22222222;	
						break;
					case 3:
						can2_Door_mc1_1 =0x33333333;
						can2_Door_T_1 =	0x33333333;
						can2_Door_M_1 = 0x33333333;
						can2_Door_T1_1 = 0x33333333;
						can2_Door_T2_1 = 0x33333333;
						can2_Door_mc2_1 = 0x33333333;	
						break;
					case 4:
						can2_Door_mc1_1 =0x44444444;
						can2_Door_T_1 =	0x44444444;
						can2_Door_M_1 = 0x44444444;
						can2_Door_T1_1 = 0x44444444;
						can2_Door_T2_1 = 0x44444444;
						can2_Door_mc2_1 = 0x44444444;		
						break;
				  case 5:
						can2_Door_mc2_1 =0x55555555;
						can2_Door_T_1 =	0x55555555;
						can2_Door_M_1 = 0x55555555;
						can2_Door_T1_1 = 0x55555555;
						can2_Door_T2_1 = 0x55555555;
						can2_Door_mc2_1 = 0x55555555;
						break;
					case 6:
						can2_Door_mc1_1 =0x66666666;
						can2_Door_T_1 =	0x66666666;
						can2_Door_M_1 = 0x66666666;
						can2_Door_T1_1 = 0x66666666;
						can2_Door_T2_1 = 0x66666666;
						can2_Door_mc2_1 = 0x66666666;		
						break;
					case 7:
						can2_Door_mc1_1 = 0x77777777;
						can2_Door_mc1_2 =0x07;
						can2_Door_T_1 =	 0x77777777;
						can2_Door_T_2 = 0x07;
						can2_Door_M_1 =  0x77777777;
						can2_Door_M_2 = 0x07;
						can2_Door_T1_1 = 0x77777777;
						can2_Door_T1_2 = 0x07;
						can2_Door_T2_1 = 0x77777777;
						can2_Door_T2_2 = 0x07;
						can2_Door_mc2_1 = 0x77777777;	
						can2_Door_mc2_2 = 0x07;
						break;
					case 8:
						can2_Door_mc1_1 = 0x88888888;
						can2_Door_mc1_2 =0x08;
						can2_Door_T_1 =	 0x88888888;
						can2_Door_T_2 = 0x08;
						can2_Door_M_1 =  0x88888888;
						can2_Door_M_2 = 0x08;
						can2_Door_T1_1 = 0x88888888;
						can2_Door_T1_2 = 0x08;
						can2_Door_T2_1 = 0x88888888;
						can2_Door_T2_2 = 0x08;
						can2_Door_mc2_1 = 0x88888888;	
						can2_Door_mc2_2 = 0x08;
						break;
				}
				sendFlag ++;
			}
//		BSP_OS_TimeDlyMs(15000);//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
			    OSTimeDlyHMSM((CPU_INT16U) 0u,
                  (CPU_INT16U) 0u,
                  (CPU_INT16U) 26,
                  (CPU_INT32U) 0,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT/*OS_OPT_TIME_PERIODIC*/,
                  (OS_ERR   *)&err);
	}
}


/*OLED Key����*/
extern void Oled_Key_thread(void *p_arg)
{
	(void)p_arg;/* ������������� */
	
	while(1)
	{
		KeySetMenu();
		BSP_OS_TimeDlyMs(50);		//whh������50ms�������ٶȿ��Ժܿ졣
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
	//printf("Size1=%d\r\n",BufferLength);
  while(BufferLength--)
  {
		//printf("a1=0x%x\r\n",*pBuffer1);
		//printf("a2=0x%x\r\n",*pBuffer2);
		//printf("\r\n");
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}





