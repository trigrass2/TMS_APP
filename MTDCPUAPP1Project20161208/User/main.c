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
#include "main.h"

//#define BUFFER_SIZE 512
	
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
#define Main_STK_SIZE 		128
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
//#define TemperCheck_STK_SIZE 		128
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
//#define KEY_STK_SIZE 		128
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
#define CAN1SLAVE_STARTUPMANAGER_STK_SIZE 		96
	//������ƿ�
static OS_TCB CAN1SlaveStartupManagerTCB;
	//�����ջ	
static CPU_STK CAN1SLAVE_STARTUPMANAGER_TASK_STK[CAN1SLAVE_STARTUPMANAGER_STK_SIZE];
	//������
void CAN1Slave_StartupManager_Take(void *p_arg);


///*��վ������������*/
//	//�������ȼ�
//#define CAN2SLAVE_STARTUPMANAGER_TASK_PRIO		6
//	//�����ջ��С	
//#define CAN2SLAVE_STARTUPMANAGER_STK_SIZE 		64
//	//������ƿ�
//static OS_TCB CAN2SlaveStartupManagerTCB;
//	//�����ջ	
//static CPU_STK CAN2SLAVE_STARTUPMANAGER_TASK_STK[CAN2SLAVE_STARTUPMANAGER_STK_SIZE];
//	//������
//void CAN2Slave_StartupManager_Take(void *p_arg);


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN1SLAVERX_TASK_PRIO		10//7
	//�����ջ��С	
#define CAN1SLAVERX_STK_SIZE 		128
	//������ƿ�
static OS_TCB CAN1SlaveRxTaskTCB;
	//�����ջ	
static CPU_STK CAN1SLAVERX_TASK_STK[CAN1SLAVERX_STK_SIZE];
	//������
extern void can1slave_recv_thread(void *p_arg);


/*��վ��������*/
	//�������ȼ�
#define CAN1SLAVETX_TASK_PRIO		9//8
	//�����ջ��С	
#define CAN1SLAVETX_STK_SIZE 		128
	//������ƿ�
/*static*/ OS_TCB CAN1SlaveTxTaskTCB;
	//�����ջ	
static CPU_STK CAN1SLAVETX_TASK_STK[CAN1SLAVETX_STK_SIZE];
	//������
extern void can1slave_send_thread(void *p_arg);


/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN2SLAVERX_TASK_PRIO		13//9
	//�����ջ��С	
#define CAN2SLAVERX_STK_SIZE 		128
	//������ƿ�
static OS_TCB CAN2SlaveRxTaskTCB;
	//�����ջ	
static CPU_STK CAN2SLAVERX_TASK_STK[CAN2SLAVERX_STK_SIZE];
	//������
extern void can2slave_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN2SLAVETX_TASK_PRIO		12//10
	//�����ջ��С	
#define CAN2SLAVETX_STK_SIZE 		128
	//������ƿ�
static OS_TCB CAN2SlaveTxTaskTCB;
	//�����ջ	
static CPU_STK CAN2SLAVETX_TASK_STK[CAN2SLAVETX_STK_SIZE];
	//������
extern void can2slave_send_thread(void *p_arg);



/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN3MASTERRX_TASK_PRIO		6
	//�����ջ��С	
#define CAN3MASTERRX_STK_SIZE 		128
	//������ƿ�
static OS_TCB CAN3MasterRxTaskTCB;
	//�����ջ	
static CPU_STK CAN3MASTERRX_TASK_STK[CAN3MASTERRX_STK_SIZE];
	//������
extern void can3master_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN3MASTERTX_TASK_PRIO		8//12
	//�����ջ��С	
#define CAN3MASTERTX_STK_SIZE 		128
	//������ƿ�
static OS_TCB CAN3MasterTxTaskTCB;
	//�����ջ	
static CPU_STK CAN3MASTERTX_TASK_STK[CAN3MASTERTX_STK_SIZE];
	//������
extern void can3master_send_thread(void *p_arg);


/*can3���ܴ�������*/
	//�������ȼ�
#define CAN3RXPROC_TASK_PRIO			7//13
	//�����ջ��С	
#define CAN3RXPROC_STK_SIZE 	  	128
	//������ƿ�
static OS_TCB CAN3RxProcTaskTCB;
	//�����ջ	
static CPU_STK CAN3RxProc_TASK_STK[CAN3RXPROC_STK_SIZE];
	//������
extern void can3RxProcthread(void *p_arg);


/*OLED Key����*/
	//�������ȼ�
#define OLEDKEY_TASK_PRIO		14
	//�����ջ��С	
#define OLEDKEY_STK_SIZE 		128
	//������ƿ�
static OS_TCB OledKeyTaskTCB;
	//�����ջ	
static CPU_STK OLEDKEY_TASK_STK[OLEDKEY_STK_SIZE];
	//������
void Oled_Key_thread(void *p_arg);


/*can1 ���ܷ���LED����˸����*/
	//�������ȼ�
#define CAN1LED_TASK_PRIO		20
	//�����ջ��С	
#define CAN1LED_STK_SIZE 		64
	//������ƿ�
static OS_TCB  CAN1LED_TCB;
	//�����ջ	
static CPU_STK  CAN1LED_TASK_STK[CAN1LED_STK_SIZE];
	//������
void CAN1LEDTake(void *p_arg);

/*can2 ���ܷ���LED����˸����*/
	//�������ȼ�
#define CAN2LED_TASK_PRIO		21
	//�����ջ��С	
#define CAN2LED_STK_SIZE 		64
	//������ƿ�
static OS_TCB  	CAN2LED_TCB;
	//�����ջ	
static CPU_STK  CAN2LED_TASK_STK[CAN2LED_STK_SIZE];
	//������
void CAN2LEDTake(void *p_arg);


/*can1 2 ��ʱ����PDO���� */
	//�������ȼ�
#define TimTxPDO_TASK_PRIO		19
	//�����ջ��С	
#define TimTxPDO_STK_SIZE 		256
	//������ƿ�
static OS_TCB  	TimTxPDO_TCB;
	//�����ջ	
static CPU_STK  TimTxPDO_TASK_STK[TimTxPDO_STK_SIZE];
	//������
void TimTxPDOTake(void *p_arg);

/*�������Ĵ���*/
static  OS_SEM     	SEM_SYNCH;	   //����ͬ��
//OS_SEM     	Slave_Start_Sem;			//��վ����������ź���

OS_MUTEX	CAN3_MUTEX;		//����һ�������ź���,����can3���ͽ�������Ļ���


/*�����¼���־��*/
OS_FLAG_GRP	CAN1LedEventFlag;		//����can1led��˸ʱ���־��
OS_FLAG_GRP	CAN2LedEventFlag;		//����can2led��˸�¼���־��

/*����ʱ���Ĵ���*/
OS_TMR CAN3TimeTmr;		//can3���緢�͹���ʱ���MFD�� ��ʱ��
void CAN3TimeTrmCallBack(void *p_tmr, void *p_arg);

/*��ʱ����PDO�Ķ�ʱ��*/
OS_TMR TxPDOTmr;
void TxPDOTmrCallBack(void *p_tmr, void *p_arg);

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

extern CO_Data MTDApp1CAN1Slave_Data;
extern CO_Data MTDApp1CAN2Slave_Data;

u8 can1CarNumberFlag = false;
u8 can2CarNumberFlag = false;
u8 can1UpdateTimeFlag = false;
u8 can2UpdateTimeFlag = false;

extern __IO uint16_t PWM2DutyCycle;
extern __IO uint32_t PWM2Frequency ;
extern __IO uint16_t PWM1DutyCycle;
extern __IO uint32_t PWM1Frequency;

extern uint8_t UART1_Rx_Cnt;
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
	
		//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8000); //����ӳ���ж�������
		SCB->VTOR = FLASH_BASE | 0x10000;	//�����ж�������
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
		Uart1_init();//uart1��GPIO��ʼ�� ,��ǰ�ĳ�ʼ������ֹ�м��д�ӡ��ʾ��Ϣ���޷����
	
		printf("MTDCPU_APP1 �忨����!\r\n");
			
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

	/*Whh 2016-11-9 �޸��ڴ����ں˶��� �Լ� �����ʱ�� ��ֹ�жϴ�ϣ��ر������жϡ�*/
	CPU_IntDis(); //�ر�ȫ���ж�  
	//����Ĵ���
	AppObjCreate();                                          /* Create Applicaiton kernel objects                    */

	//����Ĵ���
  AppTaskCreate();                                            /* Create Application tasks                             */

	CPU_IntEn(); //ʹ��ȫ���ж�
	
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
								 
//		//������վ������������
//	OSTaskCreate((OS_TCB 		* )&CAN2SlaveStartupManagerTCB,				//������ƿ�
//								(CPU_CHAR	* )"CAN2SlaveStartManager_task", 			//��������
//                 (OS_TASK_PTR)CAN2Slave_StartupManager_Take, 		//������
//                 (void		  *)0,														//���ݸ��������Ĳ���
//                 (OS_PRIO	   )CAN2SLAVE_STARTUPMANAGER_TASK_PRIO,    	//�������ȼ�
//                 (CPU_STK   *)&CAN2SLAVE_STARTUPMANAGER_TASK_STK[0],	//�����ջ����ַ
//                 (CPU_STK_SIZE)CAN2SLAVE_STARTUPMANAGER_TASK_STK[CAN2SLAVE_STARTUPMANAGER_STK_SIZE/10],	//�����ջ�����λ
//                 (CPU_STK_SIZE)CAN2SLAVE_STARTUPMANAGER_STK_SIZE,			//�����ջ��С
//                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//                 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//                 (void   	   *)0,					//�û�����Ĵ洢��
//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
//                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
				 

		//����can1��վ��������
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


		//����can1��վ��������
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


		//����can2��վ��������
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


		//����can2��վ��������
	OSTaskCreate((OS_TCB 		* )&CAN2SlaveTxTaskTCB,			 //������ƿ�
								(CPU_CHAR	* )"CAN2SlaveTx_task", 			 //��������
                 (OS_TASK_PTR)can2slave_send_thread, 	 //������
                 (void		  *)0,											 //���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2SLAVETX_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&CAN2SLAVETX_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2SLAVETX_TASK_STK[CAN2SLAVETX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2SLAVETX_STK_SIZE,	//�����ջ��С
                 (OS_MSG_QTY  )0,											//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,											//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,											//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		

		//����can3��վ��������
	OSTaskCreate((OS_TCB 		* )&CAN3MasterRxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN3MasterRx_task", 			//��������
                 (OS_TASK_PTR)can3master_recv_thread, 	//������
                 (void		  *)0,												//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN3MASTERRX_TASK_PRIO,  	//�������ȼ�
                 (CPU_STK   *)&CAN3MASTERRX_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CAN3MASTERRX_TASK_STK[CAN3MASTERRX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN3MASTERRX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,												//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,												//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,												//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);										//��Ÿú�������ʱ�ķ���ֵ				

		//����can3��վ��������
	OSTaskCreate((OS_TCB 		* )&CAN3MasterTxTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN3MasterTx_task", 			//��������
                 (OS_TASK_PTR)can3master_send_thread, 	//������
                 (void		  *)0,												//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN3MASTERTX_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&CAN3MASTERTX_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CAN3MASTERTX_TASK_STK[CAN3MASTERTX_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN3MASTERTX_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,												//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,												//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,												//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);										//��Ÿú�������ʱ�ķ���ֵ			
								 								 
	//����can3��վ���ܴ�������
	OSTaskCreate((OS_TCB 		* )&CAN3RxProcTaskTCB,			//������ƿ�
								(CPU_CHAR	* )"CAN3RxProc_task", 			//��������
                 (OS_TASK_PTR)can3RxProcthread, 			//������
                 (void		  *)0,											//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN3RXPROC_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&CAN3RxProc_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)CAN3RxProc_TASK_STK[CAN3RXPROC_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN3RXPROC_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,											//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,											//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,											//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);									//��Ÿú�������ʱ�ķ���ֵ		
									 
		///*OLE��ʾ�Լ���������*/
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

		//����can1 led ��������
	OSTaskCreate((OS_TCB 		* )&CAN1LED_TCB,					//������ƿ�
								(CPU_CHAR	* )"CAN1LEDTask", 				//��������
                 (OS_TASK_PTR)CAN1LEDTake, 					//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN1LED_TASK_PRIO,    //�������ȼ�
                 (CPU_STK   *)&CAN1LED_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN1LED_TASK_STK[CAN1LED_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN1LED_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,										//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,										//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);								//��Ÿú�������ʱ�ķ���ֵ		

		//����can2 led ��������
	OSTaskCreate((OS_TCB 		* )&CAN2LED_TCB,					//������ƿ�
								(CPU_CHAR	* )"CAN2LEDTask", 				//��������
                 (OS_TASK_PTR)CAN2LEDTake, 					//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )CAN2LED_TASK_PRIO,    //�������ȼ�
                 (CPU_STK   *)&CAN2LED_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)CAN2LED_TASK_STK[CAN2LED_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)CAN2LED_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,										//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,										//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);								//��Ÿú�������ʱ�ķ���ֵ				
						 
		//����can ��ʱ����PDO ��������
	OSTaskCreate((OS_TCB 		* )&TimTxPDO_TCB,						//������ƿ�
								(CPU_CHAR	* )"TimTxPDOTask", 					//��������
                 (OS_TASK_PTR)TimTxPDOTake, 					//������
                 (void		  *)0,											//���ݸ��������Ĳ���
                 (OS_PRIO	   )TimTxPDO_TASK_PRIO,   	//�������ȼ�
                 (CPU_STK   *)&TimTxPDO_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)TimTxPDO_TASK_STK[TimTxPDO_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)TimTxPDO_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,										//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,										//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);								//��Ÿú�������ʱ�ķ���ֵ											 
								 
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
	CPU_SR_ALLOC();			//�������壬���ڱ���SR״̬.
	OS_CRITICAL_ENTER();	//�����ٽ���	
	
	/* ����ͬ���ź��� */ 
	BSP_OS_SemCreate(&SEM_SYNCH, 0,(CPU_CHAR *)"SEM_SYNCH");

		//����һ�������ź���
	OSMutexCreate((OS_MUTEX*	)&CAN3_MUTEX,
								(CPU_CHAR*	)"CAN3_MUTEX",
                (OS_ERR*		)&err);
	
	//����һ��can3�����ź���
	OSSemCreate ((OS_SEM*	)&can3recv_sem,
                 (CPU_CHAR*	)"can3recvsem",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
	
	//����һ��can3���ܴ������ź���
	OSSemCreate ((OS_SEM*	)&can3RxProc_sem,
                 (CPU_CHAR*	)"can3RxProSem",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
	
	//	//����һ��can3�����ź���
	//	OSSemCreate ((OS_SEM*	)&can3tran_sem,
	//                 (CPU_CHAR*	)"can3transem",
	//                 (OS_SEM_CTR)0,		
	//                 (OS_ERR*	)&err);
	

	/*�����¼���־��*/
	OSFlagCreate((OS_FLAG_GRP*)&CAN1LedEventFlag,				//ָ���¼���־��
                 (CPU_CHAR*	  )"CAN1 Led Event Flag",	//����
                 (OS_FLAGS	  )0,											//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);									//������
								 
	OSFlagCreate((OS_FLAG_GRP*)&CAN2LedEventFlag,				//ָ���¼���־��
                 (CPU_CHAR*	  )"CAN2 Led Event Flag",	//����
                 (OS_FLAGS	  )0,											//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);									//������	
	
	//����can3����ʱ��
	OSTmrCreate((OS_TMR		*)&CAN3TimeTmr,		//��ʱ��1
							(CPU_CHAR	*)"CAN3TimeTmr",	//��ʱ������
							(OS_TICK	 )0,							//��ʼ����ʱ������ʱֵ
							(OS_TICK	 )100,   		  		//100*10(Ƶ��)=1000ms						//1000*1=1000ms  �ظ�����
							(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
							(OS_TMR_CALLBACK_PTR)CAN3TimeTrmCallBack,	//��ʱ��1�ص�����
							(void	    *)0,							//�ص���������Ϊ0
							(OS_ERR	  *)&err);					//���صĴ�����		
	
	//����can3����ʱ��
	OSTmrCreate((OS_TMR		*)&TxPDOTmr,			//��ʱ��1
							(CPU_CHAR	*)"TxPDOTmr",			//��ʱ������
							(OS_TICK	 )0,							//��ʼ����ʱ������ʱֵ
							(OS_TICK	 )1,   		  			//1*10(Ƶ��)=10ms							//1000*1=1000ms  �ظ�����
							(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
							(OS_TMR_CALLBACK_PTR)TxPDOTmrCallBack,	//��ʱ��1�ص�����
							(void	    *)0,							//�ص���������Ϊ0
							(OS_ERR	  *)&err);					//���صĴ�����		

	OS_CRITICAL_EXIT();	//�˳��ٽ���		

}


/////*EEPROM ����*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
//#define sEE_WRITE_ADDRESS1        0xB0
//#define sEE_READ_ADDRESS1         0xB0
//#define BUFFER_SIZE1             (countof(aTxBuffer1)-1)
//#define BUFFER_SIZE2             (countof(aTxBuffer2)-1)
//#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
//#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)
///* Private macro -------------------------------------------------------------*/
//#define countof(a) (sizeof(a) / sizeof(*(a)))

///* Private variables ---------------------------------------------------------*/
//uint8_t aTxBuffer1[] =   "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
//                        buffer 1 transfer into address sEE_WRITE_ADDRESS1 */ \
//                        Example Description \
//                        This firmware provides a basic example of how to use the I2C firmware library and\
//                        an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the\
//                        example is interfacing with M24C64 EEPROM)\
//                          \
//                        I2C peripheral is configured in Master transmitter during write operation and in\
//                        Master receiver during read operation from I2C EEPROM. \
//                          \
//                        The peripheral used is I2C1 but can be configured by modifying the defines values\
//                        in stm324xg_eval.h file. The speed is set to 100kHz and can be configured by \
//                        modifying the relative define in stm32_eval_i2c_ee.h file.\
//                         \
//                        For M24C64 devices all the memory is accessible through the two-bytes \
//                        addressing mode and need to define block addresses. In this case, only the physical \
//                        address has to be defined (according to the address pins (E0,E1 and E2) connection).\
//                        This address is defined in i2c_ee.h (default is 0xA0: E0, E1 and E2 tied to ground).\
//                        The EEPROM addresses where the program start the write and the read operations \
//                        is defined in the main.c file. \
//                         \
//                        First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the\
//                        written data are read. The written and the read buffers data are then compared.\
//                        Following the read operation, the program waits that the EEPROM reverts to its \
//                        Standby state. A second write operation is, then, performed and this time, Tx2_Buffer\
//                        is written to EEPROM_WriteAddress2, which represents the address just after the last \
//                        written one in the first write. After completion of the second write operation, the \
//                        written data are read. The contents of the written and the read buffers are compared.\
//                         \
//                        All transfers are managed in DMA mode (except when 1-byte read/write operation is\
//                        required). Once sEE_ReadBuffer() or sEE_WriteBuffer() function is called, the \
//                        use application may perform other tasks in parallel while Read/Write operation is\
//                        managed by DMA.\
//                          \
//                        This example provides the possibility to use the STM324xG-EVAL LCD screen for\
//                        messages display (transfer status: Ongoing, PASSED, FAILED).\
//                        To enable this option uncomment the define ENABLE_LCD_MSG_DISPLAY in the main.c\
//                        file.                                                                              ";
//uint8_t aTxBuffer2[] =  "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
//                        buffer 2 transfer into address sEE_WRITE_ADDRESS2 */";
//uint8_t aRxBuffer1[BUFFER_SIZE1];
//uint8_t aRxBuffer2[BUFFER_SIZE2];

//volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
//volatile uint16_t uhNumDataRead = 0;

///* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

//extern void can_recv_thread(void);
//extern	void can_slave_init(void);



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
	OS_ERR err;
//	static u8 SlaveStartCount;
//	static u8 can1StartCount;
//	static u8 can2StartCount;
//	static u8 can1Flag;
//	static u8 can2Flag=1;
//		RTC_TIME time;

	/*iap UART1*/
	static u16 UartOldCount=0;	//�ϵĴ��ڽ�������ֵ
	//u16 AppLenth=0;	//���յ���app���볤��
	
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
		
	
/*main������ ��ʼ��һЩ����*/
/*OLED*/

		switch(ChooseBoard)
		{
			case MC1_MTD_NODEID:
				memcpy(cartype,MC1_CAR_TYPE,sizeof(MC1_CAR_TYPE));
				CarNumber = 4011;
				CarNo=1;
	
				can1_MC1TxCarNo[0] =  CarNumber/100;
				can1_MC1TxCarNo[1] =  CarNumber%100;
			
				can2_MC1TxCarNo[0] =  CarNumber/100; 
				can2_MC1TxCarNo[1] =  CarNumber%100;					
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
			
				can1_MTxCarNo[0] =  CarNumber/100;
				can1_MTxCarNo[1] =  CarNumber%100;	

				can2_MTxCarNo[0] =  CarNumber/100;
				can2_MTxCarNo[1] =  CarNumber%100;			
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
			
				can1_MC2TxCarNo[0] =  CarNumber/100;
				can1_MC2TxCarNo[1] =  CarNumber%100;			
				
				can2_MC2TxCarNo[0] =  CarNumber/100;
				can2_MC2TxCarNo[1] =  CarNumber%100;				
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
//			time._min=42;
//			time._hour=10;		
//		//OS_CRITICAL_ENTER();
//		Manual_Set_Time(&time);	//W H H ����
//		//OS_CRITICAL_EXIT();//�˳��ٽ���


/*���������ʱ��*/
	OSTmrStart(&CAN3TimeTmr,&err);	//������ʱ��can3Time
	OSTmrStart(&TxPDOTmr,&err);

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


//extern int  can1test_slave(void);
//extern int  can2test_slave(void);
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
			slavestate = getState(&MTDApp1CAN1Slave_Data);
			debugprintf("can1slavestate=0x%x\r\n",slavestate);
			if(slavestate != Operational/* 0x05*/)
			{	
					/*����һ�������ź� cansend*/
					
					//					u8 BootMsg =0x00;
					//					uint32_t id= (0x700 | ChooseBoard);
					//					SlaveStateCount++;
					//					User_CAN1Transmit(id,&BootMsg,1);
					
					Message BootMessage;
					BootMessage.cob_id = (0x700 | ChooseBoard);
					BootMessage.data[0] = 0x00;
					BootMessage.len =1;
					BootMessage.rtr = CAN_RTR_DATA;
					canSend(CAN1,&BootMessage);
				
					canSend(CAN2,&BootMessage);			//WHH  �޸�can1 can2 ͬʱ������������
					
					SlaveStateCount++;					
			}
			else
			{
				OSTaskDel((OS_TCB*)&CAN1SlaveStartupManagerTCB,&err);	//�����վ״̬λ����״̬��ɾ�����������
			}
		}
		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
	}
}


//void CAN2Slave_StartupManager_Take(void *p_arg)
//{
//	OS_ERR err;
//	static u8 SlaveStateCount =0;
//	debugprintf("���뵽��վ������������\r\n");
//	
//	while(1)
//	{
//		debugprintf("slave manager\r\n");
//		
//		if(SlaveStateCount > 5)		//��������5�δӽڵ㵽��ʼ��״̬������bootup
//		{
//			debugprintf("can2slave manager 5�ε�\r\n");
//			//	SlaveStateCount = 0;
//			OSTaskDel((OS_TCB*)&CAN2SlaveStartupManagerTCB,&err);	//�����վ����������ִ��5�˺�ɾ�����������
//		}						
//		{
//			e_nodeState slavestate;
//			slavestate = getState(&MTDApp1CAN2Slave_Data);
//			debugprintf("can2slavestate=0x%x\r\n",slavestate);
//			if(slavestate != Operational/* 0x05*/)
//			{
//					//					/*����һ�������ź� cansend*/
//					//					u8 BootMsg =0x00;
//					//					uint32_t id= (0x700 | ChooseBoard);
//					//					SlaveStateCount++;
//					//					User_CAN2Transmit(id,&BootMsg,1);
//					Message BootMessage;
//					BootMessage.cob_id = (0x700 | ChooseBoard);
//					BootMessage.data[0] = 0x00;
//					BootMessage.len =1;
//					BootMessage.rtr = CAN_RTR_DATA;
//					canSend(CAN2,&BootMessage);
//					
//					SlaveStateCount++;				
//			}
//			else
//			{
//				OSTaskDel((OS_TCB*)&CAN2SlaveStartupManagerTCB,&err);	//�����վ״̬λ����״̬��ɾ�����������
//			}
//		}
//		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�
//	}
//}

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
					


/*
can1LED��˸���������¼���־�����ж�����
*/
void CAN1LEDTake(void *p_arg)
{
	OS_ERR err;

	while(1)
	{	
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&CAN1LedEventFlag,
							 (OS_FLAGS	  )CAN1TxLED_FLAG,		//���ͱ�־λ
							 (OS_TICK     )20,									//20ms��ʱ
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_TIMEOUT)		//��ʱ
		{
			MTD_CPU_CANLED_Off(CAN1_TX_LED);
		}
		else if(err == OS_ERR_NONE)
		{
			MTD_CPU_CANLED_On(CAN1_TX_LED);
			BSP_OS_TimeDlyMs(150);
			MTD_CPU_CANLED_Off(CAN1_TX_LED);
		}
		
				//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&CAN1LedEventFlag,
							 (OS_FLAGS	  )CAN1RxLED_FLAG,		//���ͱ�־λ
							 (OS_TICK     )20,									//20ms��ʱ
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_TIMEOUT)		//��ʱ
		{
			MTD_CPU_CANLED_Off(CAN1_RX_LED);
		}
		else if(err == OS_ERR_NONE)
		{
			MTD_CPU_CANLED_On(CAN1_RX_LED);
			BSP_OS_TimeDlyMs(150);
			MTD_CPU_CANLED_Off(CAN1_RX_LED);
		}
		//BSP_OS_TimeDlyMs(10);
	}
}


/*
can2LED��˸���������¼���־�����ж�����
*/
void CAN2LEDTake(void *p_arg)
{
	OS_ERR err;

	while(1)
	{	
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&CAN2LedEventFlag,
							 (OS_FLAGS	  )CAN2TxLED_FLAG,		//���ͱ�־λ
							 (OS_TICK     )20,								//20ms��ʱ
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_TIMEOUT)		//��ʱ
		{
			MTD_CPU_CANLED_Off(CAN2_TX_LED);
		}
		else if(err == OS_ERR_NONE)
		{
			MTD_CPU_CANLED_On(CAN2_TX_LED);
			BSP_OS_TimeDlyMs(150);
			MTD_CPU_CANLED_Off(CAN2_TX_LED);
		}
		
				//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&CAN2LedEventFlag,
							 (OS_FLAGS	  )CAN2RxLED_FLAG,		//���ͱ�־λ
							 (OS_TICK     )20,								//20ms��ʱ
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_TIMEOUT)		//��ʱ
		{
			MTD_CPU_CANLED_Off(CAN2_RX_LED);
		}
		else if(err == OS_ERR_NONE)
		{
			MTD_CPU_CANLED_On(CAN2_RX_LED);
			BSP_OS_TimeDlyMs(150);
			MTD_CPU_CANLED_Off(CAN2_RX_LED);
		}
		//BSP_OS_TimeDlyMs(10);
	}
}

								 
/*
	can1 2 ��ʱ����PDO���� 
	���õ�Ƶ����10ms����ÿ10ms����һ���������
*/
void TimTxPDOTake(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	//u8 i;
	static u8 Tim100msCount=0;
	static u8 Tim200msCount=0;
	while(1)
	{
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);		//�����ȴ������ź���
		if(err !=OS_ERR_NONE)
		{
			printf("TimTxPDO Timer Err!\r\n");
		}
		
		else
		{
			
			if(Tim100msCount>10)       //����5�ξ���50ms
			{
				Tim100msCount = 0; //����
				
				switch(ChooseBoard)
				{
					case MC1_MTD_NODEID:
						/*MC1*/
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[0].last_message.cob_id = 0;
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,0);		//pdo 0x183			
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,0);		//pdo��0
					break;
					
					case M_MTD_NODEID:
						/*M*/
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[1].last_message.cob_id = 0;			
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,1);		//pdo 0x185
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[1].last_message.cob_id = 0;					
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,1);		//pdo��1	
					break;

					case MC2_MTD_NODEID:
						/*MC2*/
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[2].last_message.cob_id = 0;			
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,2);		//pdo 0x188				
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[2].last_message.cob_id = 0;	
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,2);		//pdo��		
					break;
					
					default:
						break;
				}				
				
			}
			Tim100msCount++;	
			
			if(Tim200msCount>20)
			{
				Tim200msCount =0;
				
				switch(ChooseBoard)
				{
					case MC1_MTD_NODEID:
						/*MC1 ATC_Sdr*/
						MTDCPUProATCSdrLogicalData();
					
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[80].last_message.cob_id = 0;	
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,80);		//pdo��0x20B , 80			

						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[80].last_message.cob_id = 0;	
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,80);		//pdo��0x20B , 80							
					
						/*can3 ����ATC sdr1*/
						MTDCAN3Master_Data.PDO_status[7].last_message.cob_id = 0;
					  sendOnePDOevent(&MTDCAN3Master_Data,7);		//pdo��0x189 , 7		
						break;
					
					case MC2_MTD_NODEID:
						/*MC2 ATC_Sdr*/
						MTDCPUProATCSdrLogicalData();
					
						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN1Slave_Data.PDO_status[81].last_message.cob_id = 0;	
						sendOnePDOevent(&MTDApp1CAN1Slave_Data,81);		//pdo��0x20B , 81			

						/* force emission of PDO by artificially changing last emitted */
						MTDApp1CAN2Slave_Data.PDO_status[81].last_message.cob_id = 0;	
						sendOnePDOevent(&MTDApp1CAN2Slave_Data,81);		//pdo��0x20B , 81							
					
						/*can3 ����ATC sdr1*/
						MTDCAN3Master_Data.PDO_status[7].last_message.cob_id = 0;
					  sendOnePDOevent(&MTDCAN3Master_Data,7);		//pdo��0x189 ,					
						break;
					
					default :
						break;
				}
			}
			Tim200msCount++;
			
			
			//		for(i=0;i<80;i++)
			//		{
			//			/*0x209*/
			//			MTDApp1CAN1Slave_Data.PDO_status[/*79*/i].last_message.cob_id = 0;	
			//			sendOnePDOevent(&MTDApp1CAN1Slave_Data,i/*79*/);		//pdo 0x209
			//			MTDApp1CAN2Slave_Data.PDO_status[i/*79*/].last_message.cob_id = 0;
			//			sendOnePDOevent(&MTDApp1CAN2Slave_Data,i/*79*/);		//pdo��0x209	
			//		}
		
		}
		//BSP_OS_TimeDlyMs(10);
	}
}

/*
	����ʱ����
		�ص�������ִ��ʱ����������������������ûص����������ܵض�
*/
/*��ʱ���ص�����*/
void CAN3TimeTrmCallBack(void *p_tmr, void *p_arg)
{
	/*��ȡ����ʱ�� ,����ʱ�临�Ƹ�can3�����ֵ�*/
	static 	RTC_TIME time;			
	
	ISL1208_GetTime(&time);		//��ȡʱ��
	
	can3_TxTime[0] = time._year;
	can3_TxTime[1] = time._month;
	can3_TxTime[2] = time._day;
	can3_TxTime[3] = time._hour;
	can3_TxTime[4] = time._min;
	can3_TxTime[5] = time._sec;
}


/*
	��ʱ����PDO�Ķ�ʱ���ص�����
*/
void TxPDOTmrCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	
	OSTaskSemPost(&TimTxPDO_TCB,OS_OPT_POST_NONE,&err);//����can1�����ź���			
}



/*1.VVVF�����ж� ����Ҫ��ȫ�ֱ���*/
/*VVVF��Դ */
bool DI_VVVFPowertx_B1_10 = 0;	 			//VVVF��Դ  MC1
bool DI_VVVFPowertx_B1_30 = 0;	 			//VVVF��Դ  M
bool DI_VVVFPowertx_B1_60 = 0;	 			//VVVF��Դ  MC2

/*VVVF ������ЧλCRD*/
bool TCU1_CRDtx_SD_B1_10 = 0; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5��
bool TCU2_CRDtx_SD_B1_10 = 0; 				//VVVF2Э���еĵ�����Чλ ��char No.7 Bit5�� 

bool TCU1_CRDtx_SD_B1_30 = 0; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 
bool TCU2_CRDtx_SD_B1_30 = 0; 				//VVVF2Э���еĵ�����Чλ ��char No.7 Bit5�� 

bool TCU1_CRDtx_SD_B1_60 = 0; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 
bool TCU2_CRDtx_SD_B1_60 = 0; 				//VVVF2Э���еĵ�����Чλ ��char No.7 Bit5�� 

/*VVVF��Ⱥ����UCORM*/
bool TCU1_UCORtx_SD_B1_10 = 0;    		//VVVF1Э���еģ�char No.17 Bit1��
bool TCU2_UCORtx_SD_B1_10 = 0;		 		//VVVF2Э���еģ�char No.17 Bit1��

bool TCU1_UCORtx_SD_B1_30 = 0;    		//VVVF1Э���еģ�char No.17 Bit1��
bool TCU2_UCORtx_SD_B1_30 = 0;		 		//VVVF2Э���еģ�char No.17 Bit1��

bool TCU1_UCORtx_SD_B1_60 = 0;    		//VVVF1Э���еģ�char No.17 Bit1��
bool TCU2_UCORtx_SD_B1_60 = 0;		 		//VVVF2Э���еģ�char No.17 Bit1��

/*VVVF BSLB_F������Чλ*/
bool TCU1_BSLBFtx_SD_B1_10 = 0; 			//VVVF1Э���еĵ�����Чλ ��char No.18 Bit1��
bool TCU2_BSLBFtx_SD_B1_10 = 0; 			//VVVF2Э���еĵ�����Чλ ��char No.18 Bit1��

bool TCU1_BSLBFtx_SD_B1_30 = 0; 			//VVVF1Э���еĵ�����Чλ ��char No.18 Bit1��
bool TCU2_BSLBFtx_SD_B1_30 = 0; 			//VVVF2Э���еĵ�����Чλ ��char No.18 Bit1��

bool TCU1_BSLBFtx_SD_B1_60 = 0; 			//VVVF1Э���еĵ�����Чλ ��char No.18 Bit1��
bool TCU2_BSLBFtx_SD_B1_60 = 0; 			//VVVF2Э���еĵ�����Чλ ��char No.18 Bit1��

/*VVVFͨ��״̬*/
bool MDCU_VVVF1ComFaultx_B1_10 = 0;   //VVVF1ͨ��״̬
bool MDCU_VVVF2ComFaultx_B1_10 = 0;   //VVVF2ͨ��״̬

bool MDCU_VVVF1ComFaultx_B1_30 = 0;   //VVVF1ͨ��״̬
bool MDCU_VVVF2ComFaultx_B1_30 = 0;   //VVVF2ͨ��״̬

bool MDCU_VVVF1ComFaultx_B1_60 = 0;   //VVVF1ͨ��״̬
bool MDCU_VVVF2ComFaultx_B1_60 = 0;   //VVVF2ͨ��״̬

/*BCU ͨ��״̬*/
bool MDCU_BCUComFaultrx_B1_10 = 0;  	//MC1 BCUͨ��״̬ 
bool MDCU_BCUComFaultrx_B1_20 = 0;  	//T BCUͨ��״̬ 
bool MDCU_BCUComFaultrx_B1_30 = 0;  	//M BCUͨ��״̬ 
bool MDCU_BCUComFaultrx_B1_40 = 0;  	//T1 BCUͨ��״̬ 
bool MDCU_BCUComFaultrx_B1_50 = 0;  	//T2 BCUͨ��״̬ 
bool MDCU_BCUComFaultrx_B1_60 = 0;  	//MC2 BCUͨ��״̬ 

/*BCU ���б�־λ*/
bool BCU_Axle1Skidtx_SD_B1_10 = 0 ;   //MC1 BCU����
bool BCU_Axle2Skidtx_SD_B1_10 = 0 ;
bool BCU_Axle3Skidtx_SD_B1_10 = 0 ;
bool BCU_Axle4Skidtx_SD_B1_10 = 0 ;

bool BCU_Axle1Skidtx_SD_B1_60 = 0 ;   //MC2 BCU����
bool BCU_Axle2Skidtx_SD_B1_60 = 0 ;
bool BCU_Axle3Skidtx_SD_B1_60 = 0 ;
bool BCU_Axle4Skidtx_SD_B1_60 = 0 ;

/*����ն˶�ATC��Sdr�߼�Э���ĵ��� һЩ���� ���жϺ���*/
void  MTDCPUProATCSdrLogicalData(void)
{
	u8 MC1SlideSta=0;		//�ֲ�����
	u8 MC2SlideSta=0;
	
	switch(ChooseBoard)
	{
		case MC1_MTD_NODEID:		 //MC1��	
			
/*1.VVVF�����ж�*/
			/*VVVF��Ԫͨ��������VVVF���Ƶ�Դ��Ч����VVVF��Ԫ ��Ⱥ����UCORM���ź���Ч*/		
			/*VVVF1 �� VVVF2 ֻҪһ��������Ϊ���Ƕ�����*/
			
			/*1�ų�VVVF����*/
			if((DI_VVVFPowertx_B1_10 && TCU1_UCORtx_SD_B1_10 && (!MDCU_VVVF1ComFaultx_B1_10))  ||    /*VVVF1*/
				 (DI_VVVFPowertx_B1_10 && TCU2_UCORtx_SD_B1_10 && (!MDCU_VVVF2ComFaultx_B1_10)))       /*VVVF2*/
			{
				can1_TxMC1ATCSdr[0] |=0x80;  //1��VVVF����    
				can2_TxMC1ATCSdr[0] |=0x80;

				can3_TxATCSdr1[0] |=0x80;
			}
			else
			{
				can1_TxMC1ATCSdr[0] &=0x7F;  //1��VVVF���� ��Ч    
				can2_TxMC1ATCSdr[0] &=0x7F;
			
				can3_TxATCSdr1[0] &=0x7F;				
			}
			
			/*3�ų�VVVF����*/
			if((DI_VVVFPowertx_B1_30 && TCU1_UCORtx_SD_B1_30 && (!MDCU_VVVF1ComFaultx_B1_30))  ||    /*VVVF1*/
				 (DI_VVVFPowertx_B1_30 && TCU2_UCORtx_SD_B1_30 && (!MDCU_VVVF2ComFaultx_B1_30)))       /*VVVF2*/
			{
				can1_TxMC1ATCSdr[0] |=0x40;  //3��VVVF����    
				can2_TxMC1ATCSdr[0] |=0x40;		
				
				can3_TxATCSdr1[0] |=0x40;				     
			}		
			else
			{
				can1_TxMC1ATCSdr[0] &=0xBF;  //3��VVVF����  ��Ч   
				can2_TxMC1ATCSdr[0] &=0xBF;
				
				can3_TxATCSdr1[0]  &=0xBF;					
			}
			
			/*6�ų�VVVF����*/
			if((DI_VVVFPowertx_B1_60 && TCU1_UCORtx_SD_B1_60 && (!MDCU_VVVF1ComFaultx_B1_60))  ||    /*VVVF1*/
				 (DI_VVVFPowertx_B1_60 && TCU2_UCORtx_SD_B1_60 && (!MDCU_VVVF2ComFaultx_B1_60)))       /*VVVF2*/
			{
				can1_TxMC1ATCSdr[0] |=0x20;  //6��VVVF����    
				can2_TxMC1ATCSdr[0] |=0x20;
				
				can3_TxATCSdr1[0] |=0x20;						     
			}
			else
			{
				can1_TxMC1ATCSdr[0] &=0xDF;  //6��VVVF���� ��Ч   
				can2_TxMC1ATCSdr[0] &=0xDF;;
				
				can3_TxATCSdr1[0] &=0xDF;					
			}
			
/*2.��������Ч�ж�*/
			/*VVVF ͨ���������ҿ��Ƶ�Դ��Ч����VVVF ͨ��������յ�CRD(������Ч)��Ч��*/
			if((DI_VVVFPowertx_B1_10 && TCU1_CRDtx_SD_B1_10 && (!MDCU_VVVF1ComFaultx_B1_10)) ||   	 /*VVVF1*/
				 (DI_VVVFPowertx_B1_10 && TCU2_CRDtx_SD_B1_10 && (!MDCU_VVVF2ComFaultx_B1_10)))     	 /*VVVF2*/
			{
				can1_TxMC1ATCSdr[0] |=0x10;  //1��������Ч   
				can2_TxMC1ATCSdr[0] |=0x10;

				can3_TxATCSdr1[0] |=0x10;				
			}
			else
			{
				can1_TxMC1ATCSdr[0] &=0xEF;  //1��������Ч  ��Ч    
				can2_TxMC1ATCSdr[0] &=0xEF;
			
				can3_TxATCSdr1[0] &=0xEF;				
			}
			
			if((DI_VVVFPowertx_B1_30 && TCU1_CRDtx_SD_B1_30 && (!MDCU_VVVF1ComFaultx_B1_30)) ||   	 /*VVVF1*/
				 (DI_VVVFPowertx_B1_30 && TCU2_CRDtx_SD_B1_30 && (!MDCU_VVVF2ComFaultx_B1_30)))     	 /*VVVF2*/
			{
				can1_TxMC1ATCSdr[0] |=0x08;  //3��������Ч   
				can2_TxMC1ATCSdr[0] |=0x08;

				can3_TxATCSdr1[0] |=0x08;				
			}
			else
			{
				can1_TxMC1ATCSdr[0] &=0xF7;  //3��������Ч  ��Ч    
				can2_TxMC1ATCSdr[0] &=0xF7;
			
				can3_TxATCSdr1[0] &=0xF7;				
			}		

			if((DI_VVVFPowertx_B1_60 && TCU1_CRDtx_SD_B1_60 && (!MDCU_VVVF1ComFaultx_B1_60)) ||   	 /*VVVF1*/
				 (DI_VVVFPowertx_B1_60 && TCU2_CRDtx_SD_B1_60 && (!MDCU_VVVF2ComFaultx_B1_60)))     	 /*VVVF2*/
			{
				can1_TxMC1ATCSdr[0] |=0x04;  //6��������Ч   
				can2_TxMC1ATCSdr[0] |=0x04;

				can3_TxATCSdr1[0] |=0x04;				
			}
			else
			{
				can1_TxMC1ATCSdr[0] &=0xFB;  //1��������Ч  ��Ч    
				can2_TxMC1ATCSdr[0] &=0xFB;
			
				can3_TxATCSdr1[0] &=0xFB;				
			}	

/*3.BCU����*/
			can1_TxMC1ATCSdr[1] =  (MDCU_BCUComFaultrx_B1_10<<7) |(MDCU_BCUComFaultrx_B1_20<<6) |(MDCU_BCUComFaultrx_B1_30<<5) |
														 (MDCU_BCUComFaultrx_B1_40<<4) |(MDCU_BCUComFaultrx_B1_50<<3) |(MDCU_BCUComFaultrx_B1_60<<2);
			can2_TxMC1ATCSdr[1] = can1_TxMC1ATCSdr[1];
			
			can3_TxATCSdr1[1] = can1_TxMC1ATCSdr[1];
			
/*4.���е��ж�,4���жϱ�����5ǰ�棬MC1SlideSta ��5��ʹ��*/		
		if((!MDCU_BCUComFaultrx_B1_10) &&(BCU_Axle1Skidtx_SD_B1_10 || BCU_Axle2Skidtx_SD_B1_10 || BCU_Axle3Skidtx_SD_B1_10 ||BCU_Axle4Skidtx_SD_B1_10) )
		{
			can1_TxMC1ATCSdr[2] |=0x80;  //���� 
			can2_TxMC1ATCSdr[2] |=0x80;  //����
			MC1SlideSta =1;             //�ֲ����� ���ڻ���״̬���ж�
			
			can3_TxATCSdr1[2]  |=0x80;
		}
		else
		{
			can1_TxMC1ATCSdr[2] &=0x7F;  //����     ��Ч
			can2_TxMC1ATCSdr[2] &=0x7F;  //����     ��Ч
			MC1SlideSta =0;             //�ֲ����� ���ڻ���״̬���ж� ��Ч
			
			can3_TxATCSdr1[2]  &=0x7F;		
		}
		
		if((!MDCU_BCUComFaultrx_B1_60) &&(BCU_Axle1Skidtx_SD_B1_60 || BCU_Axle2Skidtx_SD_B1_60 || BCU_Axle3Skidtx_SD_B1_60 ||BCU_Axle4Skidtx_SD_B1_60) )
		{
			can1_TxMC1ATCSdr[2] |=0x40;  //���� 
			can2_TxMC1ATCSdr[2] |=0x40;  //����
			MC2SlideSta =1;             //�ֲ����� ���ڻ���״̬���ж�
			
			can3_TxATCSdr1[2]  |=0x40;
		}
		else
		{
			can1_TxMC1ATCSdr[2] &=0xBF;  //����    ��Ч
			can2_TxMC1ATCSdr[2] &=0xBF;  //����    ��Ч
			MC2SlideSta =0;             //�ֲ����� ���ڻ���״̬���ж�   ��Ч
			
			can3_TxATCSdr1[2]  &=0xBF;		
		}		

/*5.����״̬���ж�*/		
		if((MC1SlideSta ||((!MDCU_VVVF1ComFaultx_B1_10) && TCU1_BSLBFtx_SD_B1_10)) || (MC1SlideSta || ((!MDCU_VVVF2ComFaultx_B1_10) && TCU2_BSLBFtx_SD_B1_10)))     //1������״̬
		{
			can1_TxMC1ATCSdr[2] |=0x20;  //����״̬ 
			can2_TxMC1ATCSdr[2] |=0x20;  //����״̬ 
			
			can3_TxATCSdr1[2]  |=0x20;
		}
		else
		{
			can1_TxMC1ATCSdr[2] &=0xDF;  //����״̬  ��Ч 
			can2_TxMC1ATCSdr[2] &=0xDF;  //����״̬  ��Ч 
			
			can3_TxATCSdr1[2]  &=0xDF;			
		}
		
		if((MC2SlideSta ||((!MDCU_VVVF1ComFaultx_B1_60) && TCU1_BSLBFtx_SD_B1_60)) || (MC2SlideSta || ((!MDCU_VVVF2ComFaultx_B1_60) && TCU2_BSLBFtx_SD_B1_60)))     //6������״̬
		{
			can1_TxMC1ATCSdr[2] |=0x10;  //����״̬ 
			can2_TxMC1ATCSdr[2] |=0x10;  //����״̬
			
			can3_TxATCSdr1[2]  |=0x10;		
		}
		else
		{
			can1_TxMC1ATCSdr[2] &=0xEF;  //����״̬    ��Ч
			can2_TxMC1ATCSdr[2] &=0xEF;  //����״̬    ��Ч

			can3_TxATCSdr1[2]  &=0xEF;				
		}

/*6. �г���ŵļ��㣬����can3ATCSdr2��*/
			can1_TxMC1ATCSdr[3] = can3_TxATCSdr2[2];
			can2_TxMC1ATCSdr[3] = can3_TxATCSdr2[2];
		
			can3_TxATCSdr1[3] = can3_TxATCSdr2[2];

/*7.ǣ���ƶ���*/		
		can1_TxMC1ATCSdr[4] = can1_DI_MC1[4]; //1��ǣ���ƶ���
		can2_TxMC1ATCSdr[4] = can1_DI_MC1[4];
		
		can1_TxMC1ATCSdr[5] = can1_DI_MC2[4]; //6��ǣ���ƶ���
		can2_TxMC1ATCSdr[5] = can1_DI_MC2[4];
		
		can3_TxATCSdr1[4] = can1_DI_MC1[4];
		can3_TxATCSdr1[5] = can1_DI_MC2[4];
		break;

		
		case MC2_MTD_NODEID:  	 //MC2��
/*1.VVVF�����ж�*/
			/*VVVF��Ԫͨ��������VVVF���Ƶ�Դ��Ч����VVVF��Ԫ ��Ⱥ����UCORM���ź���Ч*/		
			/*VVVF1 �� VVVF2 ֻҪһ��������Ϊ���Ƕ�����*/
			
			/*1�ų�VVVF����*/
			if((DI_VVVFPowertx_B1_10 && TCU1_UCORtx_SD_B1_10 && (!MDCU_VVVF1ComFaultx_B1_10))  ||    /*VVVF1*/
				 (DI_VVVFPowertx_B1_10 && TCU2_UCORtx_SD_B1_10 && (!MDCU_VVVF2ComFaultx_B1_10)))       /*VVVF2*/
			{
				can1_TxMC2ATCSdr[0] |=0x80;  //1��VVVF����    
				can2_TxMC2ATCSdr[0] |=0x80;

				can3_TxATCSdr1[0] |=0x80;
			}
			else
			{
				can1_TxMC2ATCSdr[0] &=0x7F;  //1��VVVF���� ��Ч    
				can2_TxMC2ATCSdr[0] &=0x7F;
			
				can3_TxATCSdr1[0] &=0x7F;				
			}
			
			/*3�ų�VVVF����*/
			if((DI_VVVFPowertx_B1_30 && TCU1_UCORtx_SD_B1_30 && (!MDCU_VVVF1ComFaultx_B1_30))  ||    /*VVVF1*/
				 (DI_VVVFPowertx_B1_30 && TCU2_UCORtx_SD_B1_30 && (!MDCU_VVVF2ComFaultx_B1_30)))       /*VVVF2*/
			{
				can1_TxMC2ATCSdr[0] |=0x40;  //3��VVVF����    
				can2_TxMC2ATCSdr[0] |=0x40;		
				
				can3_TxATCSdr1[0] |=0x40;				     
			}		
			else
			{
				can1_TxMC2ATCSdr[0] &=0xBF;  //3��VVVF����  ��Ч   
				can2_TxMC2ATCSdr[0] &=0xBF;
				
				can3_TxATCSdr1[0]  &=0xBF;					
			}
			
			/*6�ų�VVVF����*/
			if((DI_VVVFPowertx_B1_60 && TCU1_UCORtx_SD_B1_60 && (!MDCU_VVVF1ComFaultx_B1_60))  ||    /*VVVF1*/
				 (DI_VVVFPowertx_B1_60 && TCU2_UCORtx_SD_B1_60 && (!MDCU_VVVF2ComFaultx_B1_60)))       /*VVVF2*/
			{
				can1_TxMC2ATCSdr[0] |=0x20;  //6��VVVF����    
				can2_TxMC2ATCSdr[0] |=0x20;
				
				can3_TxATCSdr1[0] |=0x20;						     
			}
			else
			{
				can1_TxMC2ATCSdr[0] &=0xDF;  //6��VVVF���� ��Ч   
				can2_TxMC2ATCSdr[0] &=0xDF;;
				
				can3_TxATCSdr1[0] &=0xDF;					
			}	

/*2.��������Ч�ж�*/
			/*VVVF ͨ���������ҿ��Ƶ�Դ��Ч����VVVF ͨ��������յ�CRD(������Ч)��Ч��*/
			if((DI_VVVFPowertx_B1_10 && TCU1_CRDtx_SD_B1_10 && (!MDCU_VVVF1ComFaultx_B1_10)) ||   	 /*VVVF1*/
				 (DI_VVVFPowertx_B1_10 && TCU2_CRDtx_SD_B1_10 && (!MDCU_VVVF2ComFaultx_B1_10)))     	 /*VVVF2*/
			{
				can1_TxMC2ATCSdr[0] |=0x10;  //1��������Ч   
				can2_TxMC2ATCSdr[0] |=0x10;

				can3_TxATCSdr1[0] |=0x10;				
			}
			else
			{
				can1_TxMC2ATCSdr[0] &=0xEF;  //1��������Ч  ��Ч    
				can2_TxMC2ATCSdr[0] &=0xEF;
			
				can3_TxATCSdr1[0] &=0xEF;				
			}
			
			if((DI_VVVFPowertx_B1_30 && TCU1_CRDtx_SD_B1_30 && (!MDCU_VVVF1ComFaultx_B1_30)) ||   	 /*VVVF1*/
				 (DI_VVVFPowertx_B1_30 && TCU2_CRDtx_SD_B1_30 && (!MDCU_VVVF2ComFaultx_B1_30)))     	 /*VVVF2*/
			{
				can1_TxMC2ATCSdr[0] |=0x08;  //3��������Ч   
				can2_TxMC2ATCSdr[0] |=0x08;

				can3_TxATCSdr1[0] |=0x08;				
			}
			else
			{
				can1_TxMC1ATCSdr[0] &=0xF7;  //3��������Ч  ��Ч    
				can2_TxMC1ATCSdr[0] &=0xF7;
			
				can3_TxATCSdr1[0] &=0xF7;				
			}		

			if((DI_VVVFPowertx_B1_60 && TCU1_CRDtx_SD_B1_60 && (!MDCU_VVVF1ComFaultx_B1_60)) ||   	 /*VVVF1*/
				 (DI_VVVFPowertx_B1_60 && TCU2_CRDtx_SD_B1_60 && (!MDCU_VVVF2ComFaultx_B1_60)))     	 /*VVVF2*/
			{
				can1_TxMC2ATCSdr[0] |=0x04;  //6��������Ч   
				can2_TxMC2ATCSdr[0] |=0x04;

				can3_TxATCSdr1[0] |=0x04;				
			}
			else
			{
				can1_TxMC2ATCSdr[0] &=0xFB;  //1��������Ч  ��Ч    
				can2_TxMC2ATCSdr[0] &=0xFB;
			
				can3_TxATCSdr1[0] &=0xFB;				
			}	

/*3.BCU����*/
			can1_TxMC2ATCSdr[1] =  (MDCU_BCUComFaultrx_B1_10<<7) |(MDCU_BCUComFaultrx_B1_20<<6) |(MDCU_BCUComFaultrx_B1_30<<5) |
														 (MDCU_BCUComFaultrx_B1_40<<4) |(MDCU_BCUComFaultrx_B1_50<<3) |(MDCU_BCUComFaultrx_B1_60<<2);
			can2_TxMC2ATCSdr[1] = can1_TxMC2ATCSdr[1];
			
			can3_TxATCSdr1[1] = can1_TxMC2ATCSdr[1];		
			
/*4.���е��ж�,4���жϱ�����5ǰ�棬MC1SlideSta ��5��ʹ��*/		
		if((!MDCU_BCUComFaultrx_B1_10) &&(BCU_Axle1Skidtx_SD_B1_10 || BCU_Axle2Skidtx_SD_B1_10 || BCU_Axle3Skidtx_SD_B1_10 ||BCU_Axle4Skidtx_SD_B1_10) )
		{
			can1_TxMC2ATCSdr[2] |=0x80;  //���� 
			can2_TxMC2ATCSdr[2] |=0x80;  //����
			MC1SlideSta =1;             //�ֲ����� ���ڻ���״̬���ж�
			
			can3_TxATCSdr1[2]  |=0x80;
		}
		else
		{
			can1_TxMC2ATCSdr[2] &=0x7F;  //���� 
			can2_TxMC2ATCSdr[2] &=0x7F;  //����
			MC1SlideSta =0;             //�ֲ����� ���ڻ���״̬���ж�
			
			can3_TxATCSdr1[2]  &=0x7F;		
		}
		
		if((!MDCU_BCUComFaultrx_B1_60) &&(BCU_Axle1Skidtx_SD_B1_60 || BCU_Axle2Skidtx_SD_B1_60 || BCU_Axle3Skidtx_SD_B1_60 ||BCU_Axle4Skidtx_SD_B1_60) )
		{
			can1_TxMC2ATCSdr[2] |=0x40;  //���� 
			can2_TxMC2ATCSdr[2] |=0x40;  //����
			MC2SlideSta =1;             //�ֲ����� ���ڻ���״̬���ж�
			
			can3_TxATCSdr1[2]  |=0x40;
		}
		else
		{
			can1_TxMC2ATCSdr[2] &=0xBF;  //���� 
			can2_TxMC2ATCSdr[2] &=0xBF;  //����
			MC2SlideSta =0;             //�ֲ����� ���ڻ���״̬���ж�
			
			can3_TxATCSdr1[2]  &=0xBF;		
		}	
		
/*5.����״̬���ж�*/		
		if((MC1SlideSta ||((!MDCU_VVVF1ComFaultx_B1_10) && TCU1_BSLBFtx_SD_B1_10)) || (MC1SlideSta || ((!MDCU_VVVF2ComFaultx_B1_10) && TCU2_BSLBFtx_SD_B1_10)))     //1������״̬
		{
			can1_TxMC2ATCSdr[2] |=0x20;  //����״̬ 
			can2_TxMC2ATCSdr[2] |=0x20;  //����״̬ 
			
			can3_TxATCSdr1[2]  |=0x20;
		}
		else
		{
			can1_TxMC2ATCSdr[2] &=0xDF;  //����״̬  ��Ч 
			can2_TxMC2ATCSdr[2] &=0xDF;  //����״̬  ��Ч 
			
			can3_TxATCSdr1[2]  &=0xDF;			
		}
		
		if((MC2SlideSta ||((!MDCU_VVVF1ComFaultx_B1_60) && TCU1_BSLBFtx_SD_B1_60)) || (MC2SlideSta || ((!MDCU_VVVF2ComFaultx_B1_60) && TCU2_BSLBFtx_SD_B1_60)))     //6������״̬
		{
			can1_TxMC2ATCSdr[2] |=0x10;  //����״̬ 
			can2_TxMC2ATCSdr[2] |=0x10;  //����״̬
			
			can3_TxATCSdr1[2]  |=0x10;		
		}
		else
		{
			can1_TxMC2ATCSdr[2] &=0xEF;  //����״̬    ��Ч
			can2_TxMC2ATCSdr[2] &=0xEF;  //����״̬    ��Ч

			can3_TxATCSdr1[2]  &=0xEF;				
		}

/*6. �г���ŵļ��㣬����can3ATCSdr2��*/
			can1_TxMC2ATCSdr[3] = can3_TxATCSdr2[2];
			can2_TxMC2ATCSdr[3] = can3_TxATCSdr2[2];
		
			can3_TxATCSdr1[3] = can3_TxATCSdr2[2];
		
/*7.ǣ���ƶ���*/		
		can1_TxMC2ATCSdr[4] = can1_DI_MC1[4]; //1��ǣ���ƶ���
		can2_TxMC2ATCSdr[4] = can1_DI_MC1[4];
		
		can1_TxMC2ATCSdr[5] = can1_DI_MC2[4]; //6��ǣ���ƶ���
		can2_TxMC2ATCSdr[5] = can1_DI_MC2[4];
		
		can3_TxATCSdr1[4] = can1_DI_MC1[4];
		can3_TxATCSdr1[5] = can1_DI_MC2[4];		
		
		break;
		
		default:
			break;
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





