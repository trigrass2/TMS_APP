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
//#define TemperCheck_STK_SIZE 		256
//	//������ƿ�
//static OS_TCB TemperCheckTaskTCB;
//	//�����ջ	
//static CPU_STK TemperCheck_TASK_STK[TemperCheck_STK_SIZE];
//	//������
//static void TemperCheck_task(void *p_arg);

/*������ʾcpu,��ջ��Ϣ������*/
	//KEY �����������ȼ�
#define KEY_TASK_PRIO		12
	//�����ջ��С	
#define KEY_STK_SIZE 		512
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
#define CAN1MASTERRX_TASK_PRIO		6
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
#define CAN1MASTERTX_TASK_PRIO		7
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
#define CAN2MASTERRX_TASK_PRIO		8
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
#define CAN2MASTERTX_TASK_PRIO		9
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
#define CAN3MASTERRX_TASK_PRIO		10
	//�����ջ��С	
#define CAN3MASTERRX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN3MasterRxTaskTCB;
	//�����ջ	
static CPU_STK CAN3MASTERRX_TASK_STK[CAN3MASTERRX_STK_SIZE];
	//������
extern void can3master_recv_thread(void *p_arg);

/*��վ��������*/
	//�������ȼ�
#define CAN3MASTERTX_TASK_PRIO		11
	//�����ջ��С	
#define CAN3MASTERTX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN3MasterTxTaskTCB;
	//�����ջ	
static CPU_STK CAN3MASTERTX_TASK_STK[CAN3MASTERTX_STK_SIZE];
	//������
extern void can3master_send_thread(void *p_arg);


	/*�������Ĵ���*/
static  OS_SEM     	SEM_SYNCH;	   //����ͬ��
OS_SEM     	Slave_Start_Sem;			//��վ����������ź���

OS_MUTEX	CAN3_MUTEX;		//����һ�������ź���,����can3���ͽ�������Ļ���

///*����ʹ�õ�ȫ�ֱ����Ķ���*/
////can
uint8_t DS2401P_ID[8]={0x00};			//���붨����bsp��macid�����ʹ����
uint8_t can2_send[]={0x11,0x22,0x33,0x44,0x11,0x66,0x78,0x99,0x23,0xac,0x89,0xaa,0xbb,0xcc,0xdd,0xaa,0xbb,0x00,0x33};
u8 g_RxDataFlag=0;
u8 g_FrameErrorFlag=0;

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

//EmcyMsgState 	CAN1EmcyState;
//EmcyMsgState 	CAN2EmcyState;

extern int  test_can3master(void);

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
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR | OS_OPT_TASK_SAVE_FP),		//����ѡ��
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

}

extern u8 User_CAN1Transmit(uint32_t id,uint8_t* val,uint8_t Len);
extern u8 User_CAN2Transmit(uint32_t id,uint8_t* val,uint8_t Len);
extern unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);

extern CO_Data CCUCAN1Master_Data;
extern CO_Data CCUCAN2Master_Data;
extern u8 CCUcan1TxFlag;
extern u8 CCUcan2TxFlag;

u8 aaa;  
#define a1 aaa
#define a2 aaa


void Main_task(void *p_arg)
{
//	uint64_t  a;
//	OS_ERR err;
//	static u8 SlaveStartCount;
//	
//	static u8 can1StartCount;
//	static u8 can2StartCount;
//	static u8 can1Flag;
//	static u8 can2Flag=1;

	/*iap UART1*/
	u16 UartOldCount=0;	//�ϵĴ��ڽ�������ֵ
	//u16 AppLenth=0;	//���յ���app���볤��
	
	/*spi can*/
//		Frame_TypeDef frame;
//		frame.SID=10;
//		frame.DLC=8;
//		frame.Type=CAN_STD;
	
/*ccu1  ccu2 ״̬���ж�*/
//		e_nodeState MasterState;
		
	/*����ccu�ж��Ƿ���յ�������������*/
	uint8_t	can1Count=0;
	uint8_t can2Count=0;
	static	uint8_t CCUdelayCount=0;
	uint8_t i;
	
	u8 k=0;
	
	(void)p_arg;
	debugprintf("���뵽Main����\r\n");
	
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


//			/*18.can3���ӵĳ�ʼ��*/
//		MTDCCU_CAN3_MCP2515_Init(MCP2515_CAN_BAUD_125K);
//		canInit(CAN3,MCP2515_CAN_BAUD_125K);			//W H H ,���豸�õ�can3
//		test_can3master();
		
//	/*can3 ��ʼ��*/
//{
//	u8 res;

//	
//		res = MTDCCU_CAN3_MCP2515_Init(MCP2515_CAN_BAUD_125K);
////		if(res == MCP2515_OK)
////			return 0;
////		else
////			return CAN_FAILINIT;
//}

/*can adder*/
//MTDCCU_CanAdd_Gpio_Init();
	
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
	
	
	/*ccu���豸�����豸 ָʾ��*/
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
			printf("C1�豸 CCU1 ����!\r\n");
		}
		else			//CCU1 Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_OFF);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_ON);			
			printf("C1�豸 CCU2 ����!\r\n");
		}
	}
	
	else		//��ʾΪCCU2�豸
	{
		if(CCU2SwitchState == ChooseOperate)		//CCU2Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_ON);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_OFF);
			printf("C2�豸 CCU2 ����!\r\n");
		}
		else			//CCU1 Ϊ����
		{
			MASTER_LED3_ONOFF(MASTER_SLAVE_LED_OFF);
			SLAVE_LED4_ONOFF(MASTER_SLAVE_LED_ON);	
			printf("C2�豸 CCU1 ���� \r\n");
		}		
	}
	
	
	
	
	/*���÷���pdo������ccu1����ccu2������*/
//	if(CCU1 == CCU1_NODEID)		//CCU1
//	{
//			MasterState= getState(&CCUCAN1Master_Data);
//			if(MasterState == Operational)
//			{
//				CCUcan1TxFlag =1;
//				sendPDOevent(&CCUCAN1Master_Data);
//			}
//	}
//		
//	else					//CCU2
//	{			
//			MasterState= getState(&CCUCAN2Master_Data);
//			if(MasterState == Operational)
//			{
//				CCUcan2TxFlag =1;
//				sendPDOevent(&CCUCAN2Master_Data);
//			}
//	}
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
	
	
		BSP_OS_TimeDlyMs(500);
	}	
}

void key_task(void *p_arg)		 	//������ʾ�����ջʹ��������Ѿ�cpuʹ��
{

	(void)p_arg;	               /* ������������� */
	printf("���뵽key����\r\n");
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


void assert_failed(u8* file, u32 line)
{
    debug("fail");
		while (1) ;
}




