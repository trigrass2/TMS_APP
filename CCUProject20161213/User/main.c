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

#define CAN_SYNC_NETWORK_STA_MAP	0x5FFE

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
#define Main_STK_SIZE 		256
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


/*������ʾcpu,��ջ��Ϣ������*/
	//KEY �����������ȼ�
#define KEY_TASK_PRIO		20
	//�����ջ��С	
#define KEY_STK_SIZE 		128
	//������ƿ�
static OS_TCB KeyTaskTCB;
	//�����ջ	
static CPU_STK KEY_TASK_STK[KEY_STK_SIZE];
	//������
static void key_task(void *p_arg);

/*CAN3��վ���������豸����������*/
	//�������ȼ�
#define CAN3_CHANGE_HEART_TASK_PRIO		5
	//�����ջ��С	
#define CAN3_CHANGE_HEART_STK_SIZE 		512
	//������ƿ�
static OS_TCB Can3ChangeHeartTCB;
	//�����ջ	
static CPU_STK  CAN3_CHANGE_HEART_TASK_STK[CAN3_CHANGE_HEART_STK_SIZE];
	//������
void Can3ChangeHeart_Take(void *p_arg);

/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN1MASTERRX_TASK_PRIO		8
	//�����ջ��С	
#define CAN1MASTERRX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN1MasterRxTaskTCB;
	//�����ջ	
static CPU_STK CAN1MASTERRX_TASK_STK[CAN1MASTERRX_STK_SIZE];
	//������
extern void can1master_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN1MASTERTX_TASK_PRIO		9
	//�����ջ��С	
#define CAN1MASTERTX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN1MasterTxTaskTCB;
	//�����ջ	
static CPU_STK CAN1MASTERTX_TASK_STK[CAN1MASTERTX_STK_SIZE];
	//������
extern void can1master_send_thread(void *p_arg);


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN2MASTERRX_TASK_PRIO		10
	//�����ջ��С	
#define CAN2MASTERRX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN2MasterRxTaskTCB;
	//�����ջ	
static CPU_STK CAN2MASTERRX_TASK_STK[CAN2MASTERRX_STK_SIZE];
	//������
extern void can2master_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN2MASTERTX_TASK_PRIO		11
	//�����ջ��С	
#define CAN2MASTERTX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN2MasterTxTaskTCB;
	//�����ջ	
static CPU_STK CAN2MASTERTX_TASK_STK[CAN2MASTERTX_STK_SIZE];
	//������
extern void can2master_send_thread(void *p_arg);


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN3MASTERRX_TASK_PRIO		6
	//�����ջ��С	
#define CAN3MASTERRX_STK_SIZE 		512
	//������ƿ�
OS_TCB CAN3MasterRxTaskTCB;
	//�����ջ	
static CPU_STK CAN3MASTERRX_TASK_STK[CAN3MASTERRX_STK_SIZE];
	//������
extern void can3master_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN3MASTERTX_TASK_PRIO		 7
	//�����ջ��С	
#define CAN3MASTERTX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN3MasterTxTaskTCB;
	//�����ջ	
static CPU_STK CAN3MASTERTX_TASK_STK[CAN3MASTERTX_STK_SIZE];
	//������
extern void can3master_send_thread(void *p_arg);


/*can1 ����������*/
	//�������ȼ�
#define CAN1NETWORKMONITOR_TASK_PRIO		14
	//�����ջ��С	
#define CAN1NETWORKMONITOR_STK_SIZE 		512
	//������ƿ�
OS_TCB CAN1NetWorkMonitorTaskTCB;
	//�����ջ	
static CPU_STK CAN1NetWorkMonitor_TASK_STK[CAN1NETWORKMONITOR_STK_SIZE];
	//������
void CAN1NetWorkMonitorTask(void *p_arg);

/*can2 ����������*/
	//�������ȼ�
#define CAN2NETWORKMONITOR_TASK_PRIO		15
	//�����ջ��С	
#define CAN2NETWORKMONITOR_STK_SIZE 		512
	//������ƿ�
OS_TCB CAN2NetWorkMonitorTaskTCB;
	//�����ջ	
static CPU_STK CAN2NetWorkMonitor_TASK_STK[CAN2NETWORKMONITOR_STK_SIZE];
	//������
void CAN2NetWorkMonitorTask(void *p_arg);

/*can ��ѯcan1��can2ͬʱ����ͻȻ��һ���кõĴ�����������		*/
	//�������ȼ�
#define CANQUERY_TASK_PRIO		16
	//�����ջ��С	
#define CANQUERY_STK_SIZE 		256
	//������ƿ�
OS_TCB CANQueryTaskTCB;
	//�����ջ	
static CPU_STK CANQuery_TASK_STK[CANQUERY_STK_SIZE];
	//������
void CANQueryTask(void *p_arg);


/*�������Ĵ���*/
static  OS_SEM     	SEM_SYNCH;	   	//����ͬ��
OS_SEM     	Slave_Start_Sem;				//��վ����������ź���
OS_MUTEX	CAN3_MUTEX;			 					//����һ�������ź���,����can3���ͽ�������Ļ���

/*��ʱ���Ķ���*/
OS_TMR Can3ErrResetTmr;	//can3�����жϸ�λ��ʱ��
extern void Can3ErrResetTmrTrmCallBack(void *p_tmr, void *p_arg);

/*canfestival �����������ĵĶ���*/
IsNoReceHeart			 g_RxCAN1HeartFlag[15]={NoRxHeart};					//��Ÿ����豸�������ı�־���±���ǽڵ��ID�ţ����±�Ϊ0������û��ʹ�á�
IsNoReceHeart			 g_RxCAN2HeartFlag[15]={NoRxHeart};

//CurReceHeartState  g_RxCAN1HeartStateFlag[15] = {0};		//��ŵ�ǰ��������ʱ�����豸���ڵ�״̬�����ϵ�ճ�ʼ����ʱ�򣬻��ǽ��������ӵ�ʱ��		
//CurReceHeartState  g_RxCAN2HeartStateFlag[15] = {0};

RxCanMessageState  g_RxCAN1MesStateFlag[15]={NoRxCanMesState};			//��ŵ�ǰ��վ�ӵ���վ��can��Ϣ ������״̬ 
RxCanMessageState	 g_RxCAN2MesStateFlag[15]={NoRxCanMesState};

CCUCANState 	CCU1SwitchState;			//CCU1 ��������CCU2����
CCUCANState   CCU2SwitchState;			//ccu1��ccu2��״̬һ��Ҫ����

CurrentUserState CurrentUseCAN1 = UseState;			/*��ʾ��ǰʹ��can1���ߣ�����can2���ߣ�Ĭ��ʹ��can1����*/
CurrentUserState CurrentUseCAN2 = NoUseState;	

u8 CAN1_Heart;			/*��ʾccu1 ���� ��can1���յ������豸����������ʾ�Լ�������ͨ��*/
u8 CAN2_Heart;			

u8 ChooseBoard=0;			//���ȫ�ֱ�������������ť��ֵ���Ӷ�ѡ�����
extern uint8_t UART1_Rx_Cnt;
//u32 g_SlaveTxCount=0;  //can2�ķ��ͱ�־


/*
	����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
	�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

	ϵͳʱ��ȱʡ����Ϊ168MHz�������Ҫ���ģ������޸� system_stm32f4xx.c �ļ�
*/
int main(void)
{
		OS_ERR  err;

		CPU_SR_ALLOC();																	//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
	
		//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); //����ӳ���ж�������
		SCB->VTOR = FLASH_BASE | 0x10000;	//�����ж�������	
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
		
		//uart1��GPIO��ʼ�� ,��ǰ�ĳ�ʼ������ֹ�м��д�ӡ��ʾ��Ϣ���޷����
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
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),
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

#include "iap.h"
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

	__disable_irq();   // �ر����ж�
	//����Ĵ���
  AppTaskCreate();                                            /* Create Application tasks                             */
	__enable_irq();    // �������ж�

	/*��ӡ������Ϣ*/
	delay_ms(1000);			//��������ĵ��ȣ�����ĺ����Ǵ�ӡ��ʾ����������Ϣ
	DispTaskInfo();			//��ӡ���AppTaskStart����ͽ����ˣ���Ϊû��while();
	IAP_Init(&CCUCAN2Master_Data,&CCUCAN3Master_Data,MTDCCU_CanAddVal(),0x161213);
	
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
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/,		//����ѡ��
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
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP,		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ
								 				 
	//����KEY����
	OSTaskCreate((OS_TCB 		* )&KeyTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"key_task", 			//��������
                 (OS_TASK_PTR)key_task, 			//������
                 (void		  *)0,					//���ݸ��������Ĳ���
                 (OS_PRIO	   )KEY_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&KEY_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)KEY_TASK_STK[KEY_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)KEY_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ
							 								 
		//����CAN3��վ���������豸����������
	OSTaskCreate((OS_TCB 		* )&Can3ChangeHeartTCB,				//������ƿ�
								(CPU_CHAR	* )"Can3ChangeHeart_task", 			//��������
                 (OS_TASK_PTR)Can3ChangeHeart_Take, 		//������
                 (void		  *)0,														//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN3_CHANGE_HEART_TASK_PRIO,    	//�������ȼ�
                 (CPU_STK   *)&CAN3_CHANGE_HEART_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN3_CHANGE_HEART_TASK_STK[CAN3_CHANGE_HEART_STK_SIZE/10],	//�����ջ�����λ
                 (CPU_STK_SIZE)CAN3_CHANGE_HEART_STK_SIZE,			//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
								 
								 
		//����can1��վ��������
	OSTaskCreate((OS_TCB 		* )&CAN1MasterRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN1MasterRx_task", 			//��������
                 (OS_TASK_PTR)can1master_recv_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1MASTERRX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN1MASTERRX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1MASTERRX_TASK_STK[CAN1MASTERRX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1MASTERRX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				

		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN1MasterTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN1MasterTx_task", 			//��������
                 (OS_TASK_PTR)can1master_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1MASTERTX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN1MASTERTX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1MASTERTX_TASK_STK[CAN1MASTERTX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1MASTERTX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ			


		//����can2��վ��������
	OSTaskCreate((OS_TCB 		* )&CAN2MasterRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN2MasterRx_task", 			//��������
                 (OS_TASK_PTR)can2master_recv_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2MASTERRX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN2MASTERRX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2MASTERRX_TASK_STK[CAN2MASTERRX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2MASTERRX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				

		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN2MasterTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN2MasterTx_task", 			//��������
                 (OS_TASK_PTR)can2master_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2MASTERTX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN2MASTERTX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2MASTERTX_TASK_STK[CAN2MASTERTX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2MASTERTX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ			


		//����can3��վ��������
	OSTaskCreate((OS_TCB 		* )&CAN3MasterRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN3MasterRx_task", 			//��������
                 (OS_TASK_PTR)can3master_recv_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN3MASTERRX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN3MASTERRX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN3MASTERRX_TASK_STK[CAN3MASTERRX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN3MASTERRX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ				

		//������վ��������
	OSTaskCreate((OS_TCB 		* )&CAN3MasterTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN3MasterTx_task", 			//��������
                 (OS_TASK_PTR)can3master_send_thread, 	//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN3MASTERTX_TASK_PRIO,     	//�������ȼ�
                 (CPU_STK   *)&CAN3MASTERTX_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN3MASTERTX_TASK_STK[CAN3MASTERTX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN3MASTERTX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ			

  /*can1 ����������*/
	OSTaskCreate((OS_TCB 		* )&CAN1NetWorkMonitorTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN1NetWorkMonitor_Task", 			//��������
                 (OS_TASK_PTR)CAN1NetWorkMonitorTask, 				//������
                 (void		  *)0,															//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1NETWORKMONITOR_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&CAN1NetWorkMonitor_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1NetWorkMonitor_TASK_STK[CAN1NETWORKMONITOR_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1NETWORKMONITOR_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ										 

  /*can2 ����������*/
	OSTaskCreate((OS_TCB 		* )&CAN2NetWorkMonitorTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN2NetWorkMonitor_Task", 			//��������
                 (OS_TASK_PTR)CAN2NetWorkMonitorTask, 				//������
                 (void		  *)0,															//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2NETWORKMONITOR_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&CAN2NetWorkMonitor_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2NetWorkMonitor_TASK_STK[CAN2NETWORKMONITOR_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2NETWORKMONITOR_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,					//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ			

	/*
	 can ��ѯcan1��can2ͬʱ����ͻȻ��һ���кõĴ�����������										
	*/
	OSTaskCreate((OS_TCB 		* )&CANQueryTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CANQuery_Task", 			//��������
                 (OS_TASK_PTR)CANQueryTask, 				//������
                 (void		  *)0,															//���ݸ��������Ĳ���
                 (OS_PRIO	   )CANQUERY_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&CANQuery_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CANQuery_TASK_STK[CANQUERY_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CANQUERY_STK_SIZE,		//�����ջ��С
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
	OS_ERR err;
	
	/* ����ͬ���ź��� */ 
	BSP_OS_SemCreate(&SEM_SYNCH, 0,(CPU_CHAR *)"SEM_SYNCH");
	
	//����һ�������ź���
	OSMutexCreate((OS_MUTEX*	)&CAN3_MUTEX,
								(CPU_CHAR*	)"CAN3_MUTEX",
                (OS_ERR*		)&err);
	
	//����һ�������ź���
	OSSemCreate ((OS_SEM*	)&can3recv_sem,
                 (CPU_CHAR*	)"can3recvsem",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);

		//������ʱ�� can3�����жϸ�λ��ʱ��
		OSTmrCreate((OS_TMR		*)&Can3ErrResetTmr,		//��ʱ��1
								(CPU_CHAR	*)"Can3ErrResetTmr",	//��ʱ������
								(OS_TICK	 )0,									//��ʼ����ʱ������ʱֵ
								(OS_TICK	 )1,   							  //1*100=100ms  �ظ�����
								(OS_OPT		 )OS_OPT_TMR_ONE_SHOT,	 		//����ģʽ
								(OS_TMR_CALLBACK_PTR)Can3ErrResetTmrTrmCallBack,	//��ʱ��1�ص�����
								(void	    *)0,					//�ص���������Ϊ0
								(OS_ERR	  *)&err);			//���صĴ�����				

}


static  void CCUSyncStatus_SDO_Callback(CO_Data* d, UNS8 nodeId)
{
	UNS32 abortCode;
	UNS32 data=0;
	UNS32 size=sizeof(UNS8);
	
	//����CCU����״̬�ı���
	UNS8  CCUMSStatus = 0;
	UNS8  CCUCanWorkSta = 0;

	UNS32 ret = getReadResultNetworkDict(d, nodeId,&data, &size, &abortCode);
	if( ret != SDO_FINISHED) //ͨѶ�쳣
	{
		printf("Result : Failed in getting information for slave %2.2x, AbortCode :%4.4x \r\n", nodeId, (unsigned int)abortCode);
	}
	else
	{
		printf("Result : %#lx\r\n",data);
		CCUMSStatus = (data>>4) &0x0F;
		CCUCanWorkSta = data&0x0F;
		
		if(ChooseBoard == CCU1_NODEID)   //CCU1
		{
			if/*(CCU1SwitchState == ChooseOperate) &&*/ (CCUMSStatus == 2)  //��CCU2ΪΪ��ʱ����ΪCCU2������ .
			{
				CCU1SwitchState = ChooseNotOperate;
				CCU2SwitchState = ChooseOperate;
				
				if(CCUCanWorkSta == 1)   //��ǰʹ��can1
				{
					CurrentUseCAN1 = UseState;
					CurrentUseCAN2 = NoUseState;
				}	
				else if(CCUCanWorkSta == 2)
				{
					CurrentUseCAN1 = NoUseState;
					CurrentUseCAN2 = UseState;	
				}
				else
				{
					printf("CCU1 can work status Err!\r\n");
				}
			}
		}
		else if(ChooseBoard == CCU2_NODEID)  //CCU2
		{
			if((CCUMSStatus == 1) &&(CCU2SwitchState == ChooseNotOperate)) //���CCU1Ϊ��������CCU2�Լ�Ϊ��
			{
				CCU1SwitchState = ChooseOperate;
				CCU2SwitchState = ChooseNotOperate;
							
				if(CCUCanWorkSta == 1)   //��ǰʹ��can1
				{
					CurrentUseCAN1 = UseState;
					CurrentUseCAN2 = NoUseState;
				}	
				else if(CCUCanWorkSta == 2)
				{
					CurrentUseCAN1 = NoUseState;
					CurrentUseCAN2 = UseState;	
				}
				else
				{
					printf("CCU2 can work status Err!\r\n");
				}
			}
		}
	}
	
	/* Finalize last SDO transfer with this node */
	closeSDOtransfer(d, nodeId, SDO_CLIENT);
}	


void Main_task(void *p_arg)
{
//	OS_ERR err;
	
	/*iap UART1*/
	u16 UartOldCount=0;	//�ϵĴ��ڽ�������ֵ
	//u16 AppLenth=0;	//���յ���app���볤��
		
	/*����ccu�ж��Ƿ���յ�������������*/
	uint8_t	can1Count=0;
	uint8_t can2Count=0;
	static	uint8_t CCUdelayCount=0;
	uint8_t i;
	
	/*���ڽ���������״̬��can1��can2����״̬����¼��ӳ������ֵ�0x5FFE��*/
	u8 CCUMasterSlaveSta = 0;   //1:��ʾCCU1Ϊ��, 2����ʾCCU2Ϊ��
	u8 CCUCanWorkSta = 0;       //1����ʾcan1�����У�2����ʾcan2�����С�
	
	/*����SDO�ϵ�2�����1��*/
	static u8 SDO2sCount = 0;
	
	(void)p_arg;
	debugprintf("Enter Main Task!\r\n");
		
while(1)
{
	/*1.���е�ÿ500ms��תһ��*/
	RunLed_Turn();
	
	/*2.IAP���ڽ�������*/
	if(UART1_Rx_Cnt)
	{
		if(UartOldCount==UART1_Rx_Cnt)//��������,û���յ��κ�����,��Ϊ�������ݽ������.
		{
			UartOldCount=0;
			UART1_Rx_Cnt=0;
			//printf("Rx Uart1 Data End!\r\n");
			
			if((Uart1_Rx_Buff[0]=='u')&&(Uart1_Rx_Buff[1]=='p')&&(Uart1_Rx_Buff[2]=='d')&&(Uart1_Rx_Buff[3]=='a')&&(Uart1_Rx_Buff[4]=='t')&&(Uart1_Rx_Buff[5]=='a'))  //��ʾ���� ��ʹ�������λ
			{
				__set_FAULTMASK(1);		// �ر������ж�
				NVIC_SystemReset();		// ��λ
			}
		}
		else UartOldCount=UART1_Rx_Cnt;	
	}
	
	
	/*3.ccu���豸�����豸 ָʾ�ơ��Լ��ж���·���ݶ����ܲ������Լ���Ϊ���豸*/
	CCUdelayCount++;
	if(CCUdelayCount >=6) //ÿ����ʱ500ms�����ܹ���ʱ��3s
	{
		for(i=3;i<10/*12*/;i++)	//�����Ƿ��ܽ��ܵ������豸������
		{
			if(g_RxCAN1HeartFlag[i]	==  NoRxHeart ) 	
			{
				can1Count++;
			}
			
			if(g_RxCAN2HeartFlag[i] == NoRxHeart) 
			{
				can2Count++;
			}		
		}

		if((7==can1Count) && (7==can2Count))		//can1,can2 ���ղ��������豸������
		{
			if(CCU1 == CCU1_NODEID)		//CCU1 �豸
			{
				if(CCU1SwitchState == ChooseOperate)	//���ccu1��ǰΪ����
				{
					CCU1SwitchState = ChooseNotOperate;
					CCU2SwitchState = ChooseOperate;
				}
			}
			else if(CCU1 == CCU2_NODEID )
			{
				if(CCU2SwitchState == ChooseOperate)		//���ccu2Ϊ����
				{
					CCU1SwitchState = ChooseOperate;
					CCU2SwitchState = ChooseNotOperate;
				}
			}
		}

			
		CCUdelayCount =0;
		can1Count =0;
		can2Count =0;
	}
	
	if(CCU1 == CCU1_NODEID)		//CCU1 �豸
	{
		if(CCU1SwitchState == ChooseOperate)		//CCU1Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_ON);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_OFF);
			printf("C1 Device CCU1 Master!\r\n");
		}
		else			//CCU1 Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_OFF);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_ON);			
			printf("C1 Device CCU2 Master!\r\n");
		}
	}
	
	else		//��ʾΪCCU2�豸
	{
		if(CCU2SwitchState == ChooseOperate)		//CCU2Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_ON);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_OFF);
			printf("C2 Device CCU2 Master!\r\n");
		}
		else			//CCU1 Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_OFF);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_ON);	
			printf("C2 Device CCU1 Master \r\n");
		}		
	}
	
	
	/*4. ��CUU����״̬,�Լ�can1��can2���繤��״̬д�뵽0x5FFE ӳ������ֵ���*/
	/*4.1.�ж�can�����豸ͨ��״̬������״̬��can���繤��״̬*/
		/*��1�豸ͨ��״̬��*/
	for(i=1;i<=12;i++)	//��ǰ�����¼���¼��
	{
		if((ChooseBoard == CCU1_NODEID) && (i==1))    //CCU1 �Լ��ж��Լ�������
		{
			if((CAN1_Heart == RxHeart) || (CAN2_Heart == RxHeart)) 
			{
				can1_CCUTxStatu_SubStatu1 |=(1<<(7-(i-1)));
				can2_CCUTxStatu_SubStatu1 |=(1<<(7-(i-1)));
				
				continue;
			}
		}
		else if((ChooseBoard == CCU2_NODEID ) && (i==2))  //CCU2 �Լ��ж��Լ�������
		{
		 		can1_CCUTxStatu_SubStatu1 |=(1<<(7-(i-1)));
				can2_CCUTxStatu_SubStatu1 |=(1<<(7-(i-1)));
			
				continue;
		}
		else
		{
			can1_CCUTxStatu_SubStatu1 &= (~(1<<(7-(i-1)))); 
			can2_CCUTxStatu_SubStatu1 &= (~(1<<(7-(i-1))));	
		}
		
		
		/*�ж�����id*/
		if((g_RxCAN1HeartFlag[i]	==  RxHeart) || (g_RxCAN2HeartFlag[i] == RxHeart))   //��ʾ�豸ͨ������
		{
			//��1
			if(i<9)   //SubStatu1
			{
			  can1_CCUTxStatu_SubStatu1 |= (1<<(7-(i-1))); 
				can2_CCUTxStatu_SubStatu1 |= (1<<(7-(i-1)));
			}
			else      //SubStatu2
			{
			  can1_CCUTxStatu_SubStatu2 |= (1<<(15-(i-1))); 
				can2_CCUTxStatu_SubStatu2 |= (1<<(15-(i-1))); 
			}
		}
		else  //ͨ���쳣
		{
			//��0
			if(i<9)   //SubStatu1
			{
			  can1_CCUTxStatu_SubStatu1 &= (~(1<<(7-(i-1)))); 
				can2_CCUTxStatu_SubStatu1 &= (~(1<<(7-(i-1)))); 
			}
			else      //SubStatu2
			{
			  can1_CCUTxStatu_SubStatu2 &= (~(1<<(15-(i-1)))); 
				can2_CCUTxStatu_SubStatu2 &= (~(1<<(7-(i-1)))); 
			}			
		}
	}	
		
	if((CCU1SwitchState == ChooseOperate) && (CCU2SwitchState == ChooseNotOperate))  //CCU1����
	{
	  CCUMasterSlaveSta = 1;
		
		can1_CCUTxStatu_CCU1MSStatu = 1;
		can2_CCUTxStatu_CCU1MSStatu = 1;
		
		can1_CCUTxStatu_CCU2MSStatu = 2;
		can2_CCUTxStatu_CCU2MSStatu = 2;			
	}
	else if ((CCU1SwitchState == ChooseNotOperate) && (CCU2SwitchState == ChooseOperate))  //CCU2����
	{
	  CCUMasterSlaveSta = 2;
		
		can1_CCUTxStatu_CCU1MSStatu = 2;
		can2_CCUTxStatu_CCU1MSStatu = 2;	
		
		can1_CCUTxStatu_CCU2MSStatu = 1;
		can2_CCUTxStatu_CCU2MSStatu = 1;				
	}
	else       //bug  //�����ӣ������� ����Ϊ����
	{
	  CCUMasterSlaveSta = 0xF;  

		can1_CCUTxStatu_CCU2MSStatu = 0;
		can2_CCUTxStatu_CCU2MSStatu = 0;
																	
		can1_CCUTxStatu_CCU1MSStatu = 0;
		can2_CCUTxStatu_CCU1MSStatu = 0;	
		
		printf("CCU Master Slave Sta Err��\r\n");
	}
	
	if((CurrentUseCAN1 == UseState) && (CurrentUseCAN2 == NoUseState))  //��ǰʹ��can1
	{
		CCUCanWorkSta = 1;
		
		can1_CCUTxStatu_CANLineWork = 1;
		can2_CCUTxStatu_CANLineWork = 1;		
	}
	else if((CurrentUseCAN1 == NoUseState) && (CurrentUseCAN2 == UseState))   //��ǰʹ��can2
	{
		CCUCanWorkSta = 2;
		
		can1_CCUTxStatu_CANLineWork = 2;
		can2_CCUTxStatu_CANLineWork = 2;		
	}
	else       //bug
	{
	  CCUCanWorkSta = 0xF;  

		can1_CCUTxStatu_CANLineWork = 0;
		can2_CCUTxStatu_CANLineWork = 0;		
		printf("CCU Master Slave Sta Err��\r\n");
	}	
	
	/*��ֵ��0x5FFEӳ��Ķ����ֵ���*/
	Can1_CCUNetworkSta = (CCUMasterSlaveSta <<4) |  CCUCanWorkSta;
	
	Can2_CCUNetworkSta = (CCUMasterSlaveSta <<4) |  CCUCanWorkSta;
	
   /*���ط���PDO��can������*/
	if((ChooseBoard == CCU1_NODEID) && (CCUMasterSlaveSta ==1 ))    //��ǰ�豸��CCU1 ���������� ����PDO
	{
		CCUCAN1Master_Data.PDO_status[0].last_message.cob_id = 0;			
		sendOnePDOevent(&CCUCAN1Master_Data,0);		//pdo 0x1E0		
		
		CCUCAN2Master_Data.PDO_status[0].last_message.cob_id = 0;			
		sendOnePDOevent(&CCUCAN2Master_Data,0);		//pdo 0x1E0			
	}
	else if ((ChooseBoard == CCU2_NODEID) && (CCUMasterSlaveSta ==2 ))    //��ǰ�豸��CCU2 ���������� ����PDO
	{
		CCUCAN1Master_Data.PDO_status[0].last_message.cob_id = 0;			
		sendOnePDOevent(&CCUCAN1Master_Data,0);		//pdo 0x1E0		
		
		CCUCAN2Master_Data.PDO_status[0].last_message.cob_id = 0;			
		sendOnePDOevent(&CCUCAN2Master_Data,0);		//pdo 0x1E0	
	}		
	
	
	/*5.�ϵ��ʼ��2s ֮��ʼ����һ֡SDO����ȥ��� ��һ��CCU��can�����ͨ��״̬*/

	if(SDO2sCount ==4)
	{
		UNS32 val = 0;
		UNS32 size = 0; 
		
		SDO2sCount = 0xFF;  //ֻ����һ��SDO
		
		if(ChooseBoard == CCU1_NODEID)
		{
			//can1 ��ʼ����SDO
			val = 0x600 | CCU2_NODEID ;
			size = sizeof(UNS32); 
			writeLocalDict(&CCUCAN1Master_Data,0x1280,0x01,&val,&size,RW);   //�޸Ŀͻ���TSDO node id
			val = 0x580 | CCU2_NODEID;
			writeLocalDict(&CCUCAN1Master_Data,0x1280,0x02,&val,&size,RW);   //�޸Ŀͻ���RSDO node id
			val = CCU2_NODEID;
			size = sizeof(UNS8);
			writeLocalDict(&CCUCAN1Master_Data,0x1280,0x03,&val,&size,RW);   //�޸Ŀͻ��˵ķ����� node id

			readNetworkDictCallback(&CCUCAN1Master_Data,CCU2_NODEID,CAN_SYNC_NETWORK_STA_MAP,0x00,0,&CCUSyncStatus_SDO_Callback,0);   //��ȡ0x5FFE��0x00 �е�����
			
			//can2 ��ʼ����SDO
			val = 0x600 | CCU2_NODEID ;
			size = sizeof(UNS32); 
			writeLocalDict(&CCUCAN2Master_Data,0x1280,0x01,&val,&size,RW);   //�޸Ŀͻ���TSDO node id
			val = 0x580 | CCU2_NODEID;
			writeLocalDict(&CCUCAN2Master_Data,0x1280,0x02,&val,&size,RW);   //�޸Ŀͻ���RSDO node id
			val = CCU2_NODEID;
			size = sizeof(UNS8);
			writeLocalDict(&CCUCAN2Master_Data,0x1280,0x03,&val,&size,RW);   //�޸Ŀͻ��˵ķ����� node id

			readNetworkDictCallback(&CCUCAN2Master_Data,CCU2_NODEID,CAN_SYNC_NETWORK_STA_MAP,0x00,0,&CCUSyncStatus_SDO_Callback,0);   //��ȡ0x5FFE��0x00 �е�����			
		}
		else if(ChooseBoard == CCU2_NODEID)
		{
			//can1 ��ʼ����SDO
			val = 0x600 | CCU1_NODEID ;
			size = sizeof(UNS32); 
			writeLocalDict(&CCUCAN1Master_Data,0x1280,0x01,&val,&size,RW);   //�޸Ŀͻ���TSDO node id
			val = 0x580 | CCU1_NODEID;
			writeLocalDict(&CCUCAN1Master_Data,0x1280,0x02,&val,&size,RW);   //�޸Ŀͻ���RSDO node id
			val = CCU1_NODEID;
			size = sizeof(UNS8);
			writeLocalDict(&CCUCAN1Master_Data,0x1280,0x03,&val,&size,RW);   //�޸Ŀͻ��˵ķ����� node id

			readNetworkDictCallback(&CCUCAN1Master_Data,CCU1_NODEID,CAN_SYNC_NETWORK_STA_MAP,0x00,0,&CCUSyncStatus_SDO_Callback,0);   //��ȡ0x5FFE��0x00 �е�����
			
			//can2 ��ʼ����SDO
			val = 0x600 | CCU1_NODEID ;
			size = sizeof(UNS32); 
			writeLocalDict(&CCUCAN2Master_Data,0x1280,0x01,&val,&size,RW);   //�޸Ŀͻ���TSDO node id
			val = 0x580 | CCU1_NODEID;
			writeLocalDict(&CCUCAN2Master_Data,0x1280,0x02,&val,&size,RW);   //�޸Ŀͻ���RSDO node id
			val = CCU1_NODEID;
			size = sizeof(UNS8);
			writeLocalDict(&CCUCAN2Master_Data,0x1280,0x03,&val,&size,RW);   //�޸Ŀͻ��˵ķ����� node id

			readNetworkDictCallback(&CCUCAN2Master_Data,CCU1_NODEID,CAN_SYNC_NETWORK_STA_MAP,0x00,0,&CCUSyncStatus_SDO_Callback,0);   //��ȡ0x5FFE��0x00 �е�����			
		}

	}
	
	if(SDO2sCount <5)
	{
		SDO2sCount++;	
	}

	BSP_OS_TimeDlyMs(500);
	}	
}

void key_task(void *p_arg)		 	//������ʾ�����ջʹ��������Ѿ�cpuʹ��
{

	(void)p_arg;	               /* ������������� */
	printf("Enter key Task!\r\n");
	while(1)
	{		
		if(MTDCCU_CanAddVal() == 0xFF )
		{
			DispTaskInfo();		//��ӡ��ʾ��Ϣ	
		}
		BSP_OS_TimeDlyMs(2000);//��ʱ2s
	}
}

//void TemperCheck_task(void *p_arg)			//�¶ȼ�������������������������¶ȣ���ʵʩ����
//{

//	(void )p_arg;			//��ֹ����
//	LEDGpio_Init();
//	ADC1_CH16_Config();
//	printf("Enter Temper Task!\r\n");	

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



void Can3ChangeHeart_Take(void *p_arg)
{
		(void )p_arg;			//��ֹ����
	
	while(1)
	{
	 if(ChooseBoard == CCU1_NODEID)		//��ʾ���豸ΪCCU1�豸
	 {
		if(CurrentUseCAN1 == UseState )		//can1������ʹ��
		{
			HeartMap_CAN1_CANBC = CurrentUseCAN1;
			HeartMap_CAN1_CCU1 =  CAN1_Heart;
			HeartMap_CAN1_CCU2 = g_RxCAN1HeartFlag[2];
			HeartMap_CAN1_MTD1 = g_RxCAN1HeartFlag[3];	//MC1_MTD
			HeartMap_CAN1_MTD2 = g_RxCAN1HeartFlag[4];	//T_MTD
			HeartMap_CAN1_MTD3 = g_RxCAN1HeartFlag[5];	//M_MTD
			HeartMap_CAN1_MTD4 = g_RxCAN1HeartFlag[6];	//T1_MTD
			HeartMap_CAN1_MTD5 = g_RxCAN1HeartFlag[7];	//T2_MTD
			HeartMap_CAN1_MTD6 = g_RxCAN1HeartFlag[8];	//MC2_MTD
			HeartMap_CAN1_DDU1 = g_RxCAN1HeartFlag[9];	//DDU1
			HeartMap_CAN1_DDU2 = g_RxCAN1HeartFlag[10];	//DDU2
			HeartMap_CAN1_EMR1 = g_RxCAN1HeartFlag[11];	//ERM1
			HeartMap_CAN1_EMR2 = g_RxCAN1HeartFlag[12];	//ERM2
			
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
		}
		else if(CurrentUseCAN2 == UseState )		//can2������ʹ��
		{
			HeartMap_CAN2_CANBC = CurrentUseCAN2;
			HeartMap_CAN2_CCU1 =  CAN2_Heart;
			HeartMap_CAN2_CCU2 = g_RxCAN2HeartFlag[2];
			HeartMap_CAN2_MTD1 = g_RxCAN2HeartFlag[3];	//MC1_MTD
			HeartMap_CAN2_MTD2 = g_RxCAN2HeartFlag[4];	//T_MTD
			HeartMap_CAN2_MTD3 = g_RxCAN2HeartFlag[5];	//M_MTD
			HeartMap_CAN2_MTD4 = g_RxCAN2HeartFlag[6];	//T1_MTD
			HeartMap_CAN2_MTD5 = g_RxCAN2HeartFlag[7];	//T2_MTD
			HeartMap_CAN2_MTD6 = g_RxCAN2HeartFlag[8];	//MC2_MTD
			HeartMap_CAN2_DDU1 = g_RxCAN2HeartFlag[9];	//DDU1
			HeartMap_CAN2_DDU2 = g_RxCAN2HeartFlag[10];	//DDU2
			HeartMap_CAN2_EMR1 = g_RxCAN2HeartFlag[11];	//ERM1
			HeartMap_CAN2_EMR2 = g_RxCAN2HeartFlag[12];	//ERM2
			
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
		
		}
	}
	else if(ChooseBoard == CCU2_NODEID)					//��ʾʹ��CCU2�豸
	{
		if(CurrentUseCAN1 == UseState )
		{
			HeartMap_CAN1_CANBC = CurrentUseCAN1;
			HeartMap_CAN1_CCU1 = g_RxCAN1HeartFlag[1];
			HeartMap_CAN1_CCU2 = CAN1_Heart;
			HeartMap_CAN1_MTD1 = g_RxCAN1HeartFlag[3];	//MC1_MTD
			HeartMap_CAN1_MTD2 = g_RxCAN1HeartFlag[4];	//T_MTD
			HeartMap_CAN1_MTD3 = g_RxCAN1HeartFlag[5];	//M_MTD
			HeartMap_CAN1_MTD4 = g_RxCAN1HeartFlag[6];	//T1_MTD
			HeartMap_CAN1_MTD5 = g_RxCAN1HeartFlag[7];	//T2_MTD
			HeartMap_CAN1_MTD6 = g_RxCAN1HeartFlag[8];	//MC2_MTD
			HeartMap_CAN1_DDU1 = g_RxCAN1HeartFlag[9];	//DDU1
			HeartMap_CAN1_DDU2 = g_RxCAN1HeartFlag[10];	//DDU2
			HeartMap_CAN1_EMR1 = g_RxCAN1HeartFlag[11];	//ERM1
			HeartMap_CAN1_EMR2 = g_RxCAN1HeartFlag[12];	//ERM2
			
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
		}
		else if(CurrentUseCAN2 == UseState )
		{
			HeartMap_CAN2_CANBC = CurrentUseCAN2;
			HeartMap_CAN2_CCU1 = g_RxCAN2HeartFlag[1];
			HeartMap_CAN2_CCU2 = CAN2_Heart;
			HeartMap_CAN2_MTD1 = g_RxCAN2HeartFlag[3];	//MC1_MTD
			HeartMap_CAN2_MTD2 = g_RxCAN2HeartFlag[4];	//T_MTD
			HeartMap_CAN2_MTD3 = g_RxCAN2HeartFlag[5];	//M_MTD
			HeartMap_CAN2_MTD4 = g_RxCAN2HeartFlag[6];	//T1_MTD
			HeartMap_CAN2_MTD5 = g_RxCAN2HeartFlag[7];	//T2_MTD
			HeartMap_CAN2_MTD6 = g_RxCAN2HeartFlag[8];	//MC2_MTD
			HeartMap_CAN2_DDU1 = g_RxCAN2HeartFlag[9];	//DDU1
			HeartMap_CAN2_DDU2 = g_RxCAN2HeartFlag[10];	//DDU2
			HeartMap_CAN2_EMR1 = g_RxCAN2HeartFlag[11];	//ERM1
			HeartMap_CAN2_EMR2 = g_RxCAN2HeartFlag[12];	//ERM2
			
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
		
		}

	}	
		BSP_OS_TimeDlyMs(50);			//��ʱ50ms,�ı�һ������ӳ���ֵ�����������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�		
	}
}


/*whh 2016-12-12 ��������������can�����л�������can1Masterc. can2Master.c��ʱ���ж��еĴ�����뵽�����д���*/
extern UNS8 sendEMCY(CO_Data* d, UNS16 errCode, UNS8 errRegister, const void *Specific, UNS8 SpecificLength);
extern void Switch_LED_Display(void);  //�л�led��ʾ

/*can1 ����������*/
void CAN1NetWorkMonitorTask(void *p_arg)
{
	OS_ERR err;
	EmcyMsgState CAN1EmcyState = NoEmcy;		//ʹ�þֲ�����������Ҫʹ��ȫ�ֱ�������ʶ
	UNS8	i=0, j=0, k=0;									 //i ��ʾѭ��������j���ڴ洢can1�м������豸û��������k���ڱ�ʾcan2�м������豸û������
	UNS16 RecordErrorCoode = 0;
	UNS8  heartbeatID = 0;
	
	while(1)
	{
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);  //�����ȴ������ź���
		BSP_OS_TimeDlyMs(50);  //��ʱ50ms�ȴ�can2�����������ɶ�ʱ���ж��ж����
		
		for(i=1;i<=10;i++)   /*CCU1_NODEID ��  DDU2_NODEID*/
		{
		  if(ChooseBoard == CCU1_NODEID) //CCU1 �ӽڵ�2��ʼ���
			{
				if(i==1)
				{
					continue;
				}
				else
				{
					if(	g_RxCAN1HeartFlag[i] == NoRxHeart)
					{
					  heartbeatID = i;  //û�н��ܵ�������id
						break;  //����ѭ��
					}
				}
			}
			
			else if(ChooseBoard == CCU2_NODEID) //CCU2 �ӽڵ�1��ʼ��أ����Ϊ�ڵ�2������
			{
				if(i==2)
				{
					continue;
				}
				else
				{
					if(	g_RxCAN1HeartFlag[i] == NoRxHeart)
					{
					  heartbeatID = i;  //û�н��ܵ�������id
						break;  //����ѭ��
					}
				}
			}
		}
		
		switch(heartbeatID)
		{
			case CCU1_NODEID:
					if(ChooseBoard == CCU2_NODEID)
					{
						CAN1EmcyState	= MainControlFaultSwitch;
						printf("CCU2 CAN1Master_heartbeatError %d\r\n", heartbeatID);
					}
				break;
		
			case CCU2_NODEID:
					if(ChooseBoard == CCU1_NODEID)
					{
						CAN1EmcyState	= MainControlFaultSwitch;	//���ع��Ͽ��л�
						printf("CCU1 CAN1Master_heartbeatError %d\r\n", heartbeatID);
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
		
			//case ERM1_NODEID:

			//case ERM2_NODEID:
					CAN1EmcyState	= OnePointFaultSwitch;		//������Ͽ��л���
					printf("CAN1 Error %d\r\n", heartbeatID);
				break;
			
			default :
					CAN1EmcyState =NoEmcy;
				break;
				
		}		
		
		/*can1 ���� �л���������*/
		if(CCU1 == 1)			//��ʾ��ǰ�豸Ϊccu1
		{
			if(CCU1SwitchState == ChooseOperate)	//�ж�ccu1�Ƿ�Ϊ����
			{
				/*����can2���豸���뵽����״̬֮����ܵ�����*/
				if (CurrentUseCAN1 == UseState)	//�ж�can1���Ƿ�Ϊ����
				{
					/*1.������ع���,*/
					if(CAN1EmcyState == MainControlFaultSwitch) 	//˵�����ܵ�Ϊ������������
					{
							//errCode=0x11xx ������ϣ����л�		(	// can1�ղ������豸������������can2���ܵ�������,���ҵ�ǰ�����豸Ϊccu1��)
							if(g_RxCAN2HeartFlag[heartbeatID] == RxHeart)		
							{
								k=0;		//��¼can2�ڵ��������豸�ĸ���	
								for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
								{
									if(g_RxCAN2HeartFlag[i] == NoRxHeart)
									{
										k++;
									}								
								}
								if(0==k)	//��ʾcan2���������豸������ͨ�š����͵�����Ͽ��л����л���can2���� 
								{
									#ifdef DEBUG_PRINTF
										debugprintf("ccu1 can2 Tx errCode=0x11xx Single point of failure, switchable!\r\n");
									#endif 
									
									Switch_LED_Display();  //�л�led��ʾ
									CAN1EmcyState = OnePointFaultSwitch;
									RecordErrorCoode = (0x1100  | heartbeatID);
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���									
								}
								
								continue ;				//ֱ�ӷ���
							}
														
							//��ʾcan2Ҳû�н��յ������������ͷ��͸��ػ������л����������߻���ʹ��
							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
							else
							{
								#ifdef DEBUG_PRINTF	
									debugprintf("ccu1 can2 Tx errCode=0x40xx  Slave2 Bad��Not Switch!a \r\n");
								#endif 
							
								CAN1EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x4000 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���	

								continue ;				//ֱ�ӷ���
							}
					}
				
					 /*2.���������ϣ����������߹���*/
					if(CAN1EmcyState == OnePointFaultSwitch) 
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
							
							//errCode=0x11xx ������ϣ����л�
							if((j==1) && (k==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can2 Tx errCode=0x11xx Single point of failure, switchable!\r\n");
								#endif 
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN1EmcyState = 	OnePointFaultSwitch;
								RecordErrorCoode = 0x1100 | heartbeatID;
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ���
							}
							
							//errCode=0x10xx  ������ϣ������л� 
							else if((j==1) && (k>1))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can2 Tx errCode=0x10xx  Single point of failure, Not switchable\r\n");
								#endif 
								
								CAN1EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x1000 |	heartbeatID;                                                //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ���					
							}
							
							//errCode=0x21xx �����߶Ϲ��ϣ����л�
							else if((j>1) && (k==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can2 Tx errCode=0x21xx Trunk line fault, switch!\r\n");
								#endif
			
								Switch_LED_Display();  //�л�led��ʾ
								CAN1EmcyState = 	MainLineFaultSwitch;
								RecordErrorCoode = 0x2100 |	heartbeatID;
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ���						
							}
							
							//errCode=0x20xx �����߶Ϲ��ϣ������л�
							else if((j>1) && (k>0) )
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can2 Tx 0x20xx Trunk line fault,Not switch!\r\n");
								#endif
								
								CAN1EmcyState = 	MainControlFaultNotSwitch;
								RecordErrorCoode = 0x2000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ���					
							}
						
						}
				}
			
			}
			
			
			else		//ccu1�������أ�ȥ���ccu2 �豸������
			{
				printf("CCU1 can1 node2 Err, can2_node2=%d\r\n",g_RxCAN2HeartFlag[heartbeatID]);
				//1.������ع���,������ccu1 ���ccu2������
				//��ǰ����Ϊ���ع��ϣ�����can2��Ҳ���ܲ�������ccu2������
				if((CAN1EmcyState == MainControlFaultSwitch) && (g_RxCAN2HeartFlag[heartbeatID] == NoRxHeart))
				{
					j=0;		//��¼can1�ڵ��������豸�ĸ���
					k=0;		//��¼can2�ڵ��������豸�ĸ���
					for(i=3;i<10/*12*/;i++)
					{
						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart ) //�����Ƿ��ܽ��ܵ������豸������
						{
							j++;  
						}
						
						if(g_RxCAN2HeartFlag[i] == NoRxHeart)
						{
							k++;
						}
					}
					
					//errCode=0x30xx ����2���������л�
					if((j!=0 )&&(k!=0))
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x60xx Master2 Bad��Not Switch!\r\n");
						#endif 
						
						CAN1EmcyState = 	MainControlFaultNotSwitch;
						RecordErrorCoode = 0x6000 |heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3000 |	heartbeatID;					
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ���
					}
					
					//errCode=0x30xx����2�������л�
					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x61xx Master2 Bad�� Switch!\r\n");
						#endif 
						
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN1EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					else if((0!=j) && (0==k))	//can2�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x31xx Master2 Bad�� Switch!\r\n");
						#endif 
						
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN1EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x3100 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						
						//RecordErrorCoode = 0x6100 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x61xx Master2 Bad�� Switch!\r\n");
						#endif
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN1EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
					
						//	RecordErrorCoode = 0x3100 |	heartbeatID;
						//	sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ���
					}
				}
			}
			
		}
		else		//��ʾʹ�õ�ccu2
		{
			if(CCU2SwitchState == ChooseOperate)		//ccu2 ����
			{
				/*����can2���豸���뵽����״̬֮����ܵ�����*/
				if (CurrentUseCAN1 == UseState)
				{
						/*1.������ع���,*/
					if(CAN1EmcyState == MainControlFaultSwitch)
					{
							//errCode=0x11xx ������ϣ����л�		(	// can1�ղ������豸������������can2���ܵ�������,���ҵ�ǰ�����豸Ϊccu1��)
							if(g_RxCAN2HeartFlag[heartbeatID] == RxHeart)		
							{							
								k=0;		//��¼can2�ڵ��������豸�ĸ���	
								for(i=3;i<=10/*12*/;i++)	//��ǰ�����¼���¼��
								{
									if(g_RxCAN2HeartFlag[i] == NoRxHeart)
									{
										k++;
									}								
								}
								if(0==k)	//��ʾcan2���������豸������ͨ�š����͵�����Ͽ��л����л���can2���� 
								{
									#ifdef DEBUG_PRINTF
										debugprintf("ccu2 can2 Tx errCode=0x11xx Single point of failure, switchable!\r\n");
									#endif 
									
									Switch_LED_Display();  //�л�led��ʾ
									CAN1EmcyState = OnePointFaultSwitch;
									RecordErrorCoode = (0x1100  | heartbeatID);
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���									
								}
								continue ;				//ֱ�ӷ��� ;				//ֱ�ӷ���
							}
														
							//��ʾcan2Ҳû�н��յ������������ͷ��͸��ػ������л����������߻���ʹ��
							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
							else
							{							
									#ifdef DEBUG_PRINTF	
										debugprintf("ccu1 can2 Tx errCode=0x10xx Slave2 Bad��Not Switch! \r\n");
									#endif 
								
									CAN1EmcyState = 	OnePointFaultNotSwitch;
									RecordErrorCoode = 0x1000 |	heartbeatID;                                                    	//�����л�����˭���Ͷ�����
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���								
									
									continue ;				//ֱ�ӷ��� ;
							}
					}
			
					 /*2.���������ϣ����������߹���*/
					if(CAN1EmcyState == OnePointFaultSwitch) 
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
							
							//errCode=0x11xx ������ϣ����л�
							if((j==1) && (k==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can2 Tx 0x11xx Switch\r\n");
								#endif
								Switch_LED_Display();  //�л�led��ʾ
								CAN1EmcyState = 	OnePointFaultSwitch;
								RecordErrorCoode = 0x1100 | heartbeatID;
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ��� ;
							}
							//errCode=0x10xx  ������ϣ������л� 
							else if((j==1) && (k>1))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can2 Tx 0x10xx Not Switch!\r\n");
								#endif
								
								CAN1EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x1000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ��� ;					
							}
							
							//errCode=0x21xx �����߶Ϲ��ϣ����л�
							else if((j>1) && (k==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can2 Tx 0x21xx Switch!\r\n");
								#endif 
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN1EmcyState = 	MainLineFaultSwitch;
								RecordErrorCoode = 0x2100 |	heartbeatID;                                                       
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//�������ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ��� ;						
							}
							//errCode=0x20xx �����߶Ϲ��ϣ������л�
							else if((j>1) && (k>0) )
							{	
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can2 Tx 0x20xx Switch!\r\n");
								#endif
								
								CAN1EmcyState = 	MainControlFaultNotSwitch;
								RecordErrorCoode = 0x2000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ��� ;						
							}
				
					}
				}
			
			}
			
			
			else		//ccu2�������أ�ȥ��1 �豸������
			{
				//printf("whh ccu2 is not master!\r\n");
				//1.������ع���,������ccu1 ���ccu2������
				//��ǰ����Ϊ���ع��ϣ�����can2��Ҳ���ܲ�������ccu2������
				if((CAN1EmcyState == MainControlFaultSwitch) && (g_RxCAN2HeartFlag[heartbeatID] == NoRxHeart) )
				{
					j=0;		//��¼can1�ڵ��������豸�ĸ���
					k=0;		//��¼can2�ڵ��������豸�ĸ���
					for(i=3;i<10/*12*/;i++)
					{
						if(g_RxCAN1HeartFlag[i]	==  NoRxHeart )	//�����Ƿ��ܽ��ܵ������豸������
						{
							j++;  
						}
						
						if(g_RxCAN2HeartFlag[i] == NoRxHeart)
						{
							k++;
						}
					}
					
					//errCode=0x31xx ����1���������л�
					if((j!=0 )&&(k!=0))
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x60xx Master1 Not Switch!\r\n");
						#endif 
						
						CAN1EmcyState = 	MainControlFaultNotSwitch;
						RecordErrorCoode = 0x6000 |heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���						
						
						//RecordErrorCoode = 0x3000 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���						                                                                                                                 
						continue ;				//ֱ�ӷ��� ;
					}
					
					//errCode=0x30xx����1�������л�
					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x61xx Master1  Switch!\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN1EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					else if((0!=j) && (0==k))	//can2�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x31xx Master1  Switch!\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN1EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x3100 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						
						//RecordErrorCoode = 0x6100 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					//errCode=0x30xx����1�������л�
					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x61xx Master1  Switch!\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN1EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���					
						continue ;				//ֱ�ӷ��� ;
					}
				}
			}
		}
		
		BSP_OS_TimeDlyMs(50);
	}
}


/*can2 ����������*/
void CAN2NetWorkMonitorTask(void *p_arg)
{
	OS_ERR err;
	EmcyMsgState CAN2EmcyState;			//ʹ�þֲ�����������Ҫʹ��ȫ�ֱ�������ʶ
	UNS8	i=0, j=0 , k=0;									//i ��ʾѭ��������j���ڴ洢can1�м������豸û��������k���ڱ�ʾcan2�м������豸û������
	UNS16 RecordErrorCoode =0;
	UNS8  heartbeatID = 0;
	while(1)
	{
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);  //�����ȴ������ź���		
		BSP_OS_TimeDlyMs(50);  //��ʱ50ms�ȴ�can2�����������ɶ�ʱ���ж��ж����
		
		for(i=1;i<=10;i++)   /*CCU1_NODEID ��  DDU2_NODEID*/
		{
		  if(ChooseBoard == CCU1_NODEID) //CCU1 �ӽڵ�2��ʼ���
			{
				if(i==1)
				{
					continue;
				}
				else
				{
					if(	g_RxCAN2HeartFlag[i] == NoRxHeart)
					{
					  heartbeatID = i;  //û�н��ܵ�������id
						break;  //����forѭ��
					}
				}
			}
			
			else	if(ChooseBoard == CCU2_NODEID) //CCU2 �ӽڵ�1��ʼ��أ����Ϊ�ڵ�2������
			{
				if(i==2)
				{
					continue;
				}
				else
				{
					if(	g_RxCAN2HeartFlag[i] == NoRxHeart)
					{
					  heartbeatID = i;  //û�н��ܵ�������id
						break;  //����forѭ��
					}
				}
			}
		}
				
		switch(heartbeatID)
		{
			case CCU1_NODEID:
					if(ChooseBoard == CCU2_NODEID)
					{
						CAN2EmcyState	= MainControlFaultSwitch;
						printf("CCU2 CAN2Master_heartbeatError %d\r\n", heartbeatID);
					}
				break;
		
			case CCU2_NODEID:
					if(ChooseBoard == CCU1_NODEID)
					{
						CAN2EmcyState	= MainControlFaultSwitch;	//���ع��Ͽ��л�					
						printf("CCU1 CAN2Master_heartbeatError %d\r\n", heartbeatID);
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
			
			//case ERM1_NODEID:

			//case ERM2_NODEID:
					CAN2EmcyState	= OnePointFaultSwitch;		//������Ͽ��л���
					printf("CAN2 Error %d\r\n", heartbeatID);
					break;
		default :
				CAN2EmcyState =NoEmcy;
			break;
		}
	
	
		/*can2 ���� �л���������*/	
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
										debugprintf("ccu1 can1 Tx errCode=0x41xx Single point of failure, Switchable!\r\n");
									#endif 
									
									Switch_LED_Display();  //�л�led��ʾ
									CAN2EmcyState = OnePointFaultSwitch;
									RecordErrorCoode = (0x4100  | heartbeatID);
									sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);			//�������ͨ��can1�ڷ���	
								}
								continue ;				//ֱ�ӷ��� ;				//ֱ�ӷ���
							}
														
							//��ʾcan1Ҳû�н��յ������������ͷ��͸��ػ������л����������߻���ʹ��
							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
							else
							{
									//errCode=0x61xx ����1���������л� 	
									#ifdef DEBUG_PRINTF
										debugprintf("ccu1 can1 Tx errCode=0x10xx  Slave 2 bad, Not switchable \r\n");
									#endif
								
									CAN2EmcyState = 	OnePointFaultNotSwitch;
									RecordErrorCoode = 0x1000 |	heartbeatID;                                                      //�����л�����˭���Ͷ�����
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
									continue ;				//ֱ�ӷ��� ;
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
									debugprintf("ccu1 can1 Tx 0x41xx Switchable!\r\n");
								#endif 
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	OnePointFaultSwitch;
								RecordErrorCoode = 0x4100 | heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;
							}
							//errCode=0x40xx  ������ϣ������л� 
							else if((k==1) && (j>1))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 Tx 0x40xx Not Switchable!\r\n");
								#endif 
								
								CAN2EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x4000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;					
							}
							
							//errCode=0x51xx �����߶Ϲ��ϣ����л�
							else if((k>1) && (j==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 Tx 0x51xx Switchable!\r\n");
								#endif
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	MainLineFaultSwitch;
								RecordErrorCoode = 0x5100 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;						
							}
							//errCode=0x50xx �����߶Ϲ��ϣ������л�
							else if((k>1) && (j>0) )
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu1 can1 Tx 0x50xx Not Switchable!\r\n");
								#endif
								
								CAN2EmcyState = 	MainControlFaultNotSwitch;
								RecordErrorCoode = 0x5000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;						
							}
						
						}
				}
			
			}
			
			
			else		//ccu1�������أ�ȥ���ccu2 �豸������
			{
				printf("CCU1 can2 node2 Err, can1_node2=%d\r\n",g_RxCAN1HeartFlag[heartbeatID]);
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
							debugprintf("Tx ErrorCode 0x30xx Master2 Bad Not switchable!\r\n");
						#endif 
						
						CAN2EmcyState = 	MainControlFaultNotSwitch;
						//RecordErrorCoode = 0x6000 |heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						RecordErrorCoode = 0x3000 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���						
						continue ;				//ֱ�ӷ��� ;
					}

					//errCode=0x31xx����1�������л�
					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x61xx Master2 Bad Switchable!\r\n");
						#endif 
						
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),RecordErrorCoode,NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					else if((0!=j) && (0==k))	//can2�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode 0x31xx Master2 Bad Switchable!\r\n");
						#endif 
					
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x3100 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						
						//RecordErrorCoode = 0x6100 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					
					//errCode=0x61xx����1�������л�
					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode  0x61xx Master2 Bad Switchable!\r\n");
						#endif
						
						CCU1SwitchState = ChooseOperate;
						CCU2SwitchState = ChooseNotOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ��� ;
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
										debugprintf("ccu2 can1 Tx errCode=0x41xx Single point of failure, switchable!\r\n");
									#endif 
									
									Switch_LED_Display();  //�л�led��ʾ
									CAN2EmcyState = OnePointFaultSwitch;
									RecordErrorCoode = (0x4100  | heartbeatID);
									sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);			//�������ͨ��can1�ڷ���	
								}
								
								continue ;				//ֱ�ӷ��� ;				//ֱ�ӷ���
							}
														
							//��ʾcan2Ҳû������
							/*�����������ػ���������Ӱ�쵽����ͨ�ţ��ʲ��л���(���߿��Է���һ��������ϲ����л�ָ����߱���)*/
							else
							{
								//errCode=0x61xx ����1���������л� 	
									#ifdef DEBUG_PRINTF
										debugprintf("ccu2 can2 Tx errCode=0x10xx  Slave2  Not switch! \r\n");
									#endif
								
									CAN2EmcyState = 	OnePointFaultNotSwitch;
									RecordErrorCoode = 0x1000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
									sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���								
									continue ;				//ֱ�ӷ��� ;
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
									debugprintf("ccu2 can1 Tx 0x41xx switch!\r\n");
								#endif 
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	OnePointFaultSwitch;
								RecordErrorCoode = 0x4100 | heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;
							}
							//errCode=0x40xx  ������ϣ������л� 
							else if((k==1) && (j>1))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 Tx 0x40xx Not switch!\r\n");
								#endif 
								
								CAN2EmcyState = 	OnePointFaultNotSwitch;
								RecordErrorCoode = 0x4000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;					
							}
							
							//errCode=0x51xx �����߶Ϲ��ϣ����л�
							else if((k>1) && (j==0))
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 Tx 0x51xx switch!\r\n");
								#endif
								
								Switch_LED_Display();  //�л�led��ʾ
								CAN2EmcyState = 	MainLineFaultSwitch;
								RecordErrorCoode = 0x5100 |	heartbeatID;
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//�������ͨ��can1�ڷ���
								continue ;				//ֱ�ӷ��� ;						
							}
							//errCode=0x50xx �����߶Ϲ��ϣ������л�
							else if((k>1) && (j>0) )
							{
								#ifdef DEBUG_PRINTF
									debugprintf("ccu2 can1 Tx  0x50xx Not switch!\r\n");
								#endif 
								
								CAN2EmcyState = 	MainControlFaultNotSwitch;
								RecordErrorCoode = 0x5000 |	heartbeatID;                                                       //�����л�����˭���Ͷ�����
								sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
								continue ;				//ֱ�ӷ��� ;						
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
						debugprintf("Tx ErrorCode :0x30xx Master1 Not switch!\r\n");
						#endif 
						
						CAN2EmcyState = 	MainControlFaultNotSwitch;
						//RecordErrorCoode = 0x6000 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						
						RecordErrorCoode = 0x3000 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					
					//errCode=0x31xx����1�������л�
					else if ((0==j) && (k!=0) )  //can1�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode :0x61xx Master1  switch!\r\n\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					else if((0!=j) && (0==k))	//can2�߷ǳ���
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode :0x31xx Master1  switch!\r\n\r\n");
						#endif 
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x3100 |	heartbeatID;
						sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���
						
						//RecordErrorCoode = 0x6100 |	heartbeatID;
						//sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
						continue ;				//ֱ�ӷ��� ;
					}
					
					//errCode=0x60xx����1�������л�
					else		//��·can���ǳ��� Ĭ�Ͼ�ʹ��can1����
					{
						#ifdef DEBUG_PRINTF
							debugprintf("Tx ErrorCode :0x61xx Master1 switch!\r\n\r\n");
						#endif          
						
						CCU1SwitchState = ChooseNotOperate;
						CCU2SwitchState = ChooseOperate;
						CAN2EmcyState = 	MainControlFaultSwitch;
						RecordErrorCoode = 0x6100 |	heartbeatID;
						sendEMCY(&CCUCAN1Master_Data,RecordErrorCoode,*(CCUCAN1Master_Data.error_register),NULL,0);		//����ͨ��can1�ڷ���
	
						//RecordErrorCoode = 0x3100 |	heartbeatID;
						//sendEMCY(&CCUCAN2Master_Data,RecordErrorCoode,*(CCUCAN2Master_Data.error_register),NULL,0);		//����ͨ��can2�ڷ���					
						continue ;				//ֱ�ӷ��� ;
					}

				}
			}
		}
		
		BSP_OS_TimeDlyMs(50);
	}
}


/*
	 can ��ѯcan1��can2ͬʱ����ͻȻ��һ���кõĴ�����������							
*/
								 	//������
void CANQueryTask(void *p_arg)
{
  OS_ERR err;
	UNS8	i=0, j=0, k=0;									 //i ��ʾѭ��������j���ڴ洢can1�м������豸û��������k���ڱ�ʾcan2�м������豸û������
	while(1)
	{
		/*1.��ǰʹ��can������ϣ�����һ���Ƿ������Ĳ�ѯ*/
		if(CurrentUseCAN1 == UseState)  //��ʾ��ǰʹ��can1����
		{
			/*(1),��can1������ϵ�ʱ�򣬲�ѯcan2�����Ƿ�����������������can1������ϵ������ź���*/
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
			if((j!=0) && (k==0))    //can1���ϣ�can2����
			{
			 OSTaskSemPost(&CAN1NetWorkMonitorTaskTCB,OS_OPT_POST_NONE,&err);   //�ͷ�can1�����������ź���
			}	
		}
		else if (CurrentUseCAN2 == UseState) //��ʾ��ǰʹ��can2���� 
		{
			/*(1),��can1������ϵ�ʱ�򣬲�ѯcan2�����Ƿ�����������������can1������ϵ������ź���*/
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
			if((k!=0) && (j==0))    //can2���ϣ�can1����
			{
			 OSTaskSemPost(&CAN2NetWorkMonitorTaskTCB,OS_OPT_POST_NONE,&err);  //�ͷ�can2�����������ź���
			}			
		}
		else
		{
			printf("can NetWork Choose Error��\r\n");
		}
		
		BSP_OS_TimeDlyMs(400);     //������Ƶ��Ϊ��׼
	}
}

void assert_failed(u8* file, u32 line)
{
    debug("fail");
		while (1) ;
}




