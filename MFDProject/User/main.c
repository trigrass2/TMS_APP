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

u8 szASC16[200]={	0};

#define BUFFER_SIZE 512

static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  void  AppTaskStart          (void     *p_arg);
static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);

/*���񴴽���������Ĳ���*/

	//MainTask  �������ȼ�
#define Main_TASK_PRIO		4
	//�����ջ��С	
#define Main_STK_SIZE 		512		//512*4 = 2k�Ŀռ�
	//������ƿ�
static OS_TCB MainTaskTCB;
	//�����ջ	
static CPU_STK Main_TASK_STK[Main_STK_SIZE];
	//������
static void Main_task(void *p_arg);


///*�¶ȼ������*/
//	//temper �¶ȼ���������ȼ�
//#define TemperCheck_TASK_PRIO		10
//	//�����ջ��С	
//#define TemperCheck_STK_SIZE 		256
//	//������ƿ�
//static OS_TCB TemperCheckTaskTCB;
//	//�����ջ	
//static CPU_STK TemperCheck_TASK_STK[TemperCheck_STK_SIZE];
//	//������
//static void TemperCheck_task(void *p_arg);


/*��վ������������*/
	//�������ȼ�
#define CAN2SLAVE_STARTUPMANAGER_TASK_PRIO		5
	//�����ջ��С	
#define CAN2SLAVE_STARTUPMANAGER_STK_SIZE 		128
	//������ƿ�
static OS_TCB CAN2SlaveStartupManagerTCB;
	//�����ջ	
static CPU_STK CAN2SLAVE_STARTUPMANAGER_TASK_STK[CAN2SLAVE_STARTUPMANAGER_STK_SIZE];
	//������
void CAN2Slave_StartupManager_Take(void *p_arg);



/*��վ��������,��ջ��Ϣ������*/
	//�������ȼ�
#define CAN2SLAVERX_TASK_PRIO		7
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
#define CAN2SLAVETX_TASK_PRIO		6
	//�����ջ��С	
#define CAN2SLAVETX_STK_SIZE 		512
	//������ƿ�
static OS_TCB CAN2SlaveTxTaskTCB;
	//�����ջ	
static CPU_STK CAN2SLAVETX_TASK_STK[CAN2SLAVETX_STK_SIZE];
	//������
extern void can2slave_send_thread(void *p_arg);


/*DI�ɼ���ѯ����*/
	//�������ȼ�
#define	DI_CHECK_TASK_PRIO					5//12			//DI��������ȼ�Ӧ�÷ŵ���ߵ�
	//�����ջ��С	
#define DI_CHECK_STK_SIZE 					256
	//������ƿ�
static OS_TCB DICheckTCB; 
	//�����ջ	
static CPU_STK DI_CHECK_TASK_STK[DI_CHECK_STK_SIZE];
	//������
void DICheck_Take(void *p_arg);


/*����״̬��ѯ����*/
	//�������ȼ�
#define DOOR_STATE_QUERY_TASK_PRIO		8
	//�����ջ��С	
#define DOOR_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB DoorStateQueryTCB; 
	//�����ջ	
static CPU_STK DOOR_STATE_QUERY_TASK_STK[DOOR_STATE_QUERY_STK_SIZE];
	//������
void DoorStateQuery_Take(void *p_arg);


/*�յ�״̬��ѯ����*/
	//�������ȼ�
#define ACC_STATE_QUERY_TASK_PRIO		10
	//�����ջ��С	
#define ACC_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB AccStateQueryTCB; 
	//�����ջ	
static CPU_STK ACC_STATE_QUERY_TASK_STK[ACC_STATE_QUERY_STK_SIZE];
	//������
void AccStateQuery_Take(void *p_arg);

/*�ƶ�״̬��ѯ����*/
	//�������ȼ�
#define BCU_STATE_QUERY_TASK_PRIO		9//14
	//�����ջ��С	
#define BCU_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB BcuStateQueryTCB; 
	//�����ջ	
static CPU_STK BCU_STATE_QUERY_TASK_STK[BCU_STATE_QUERY_STK_SIZE];
	//������
void BcuStateQuery_Take(void *p_arg);

/*��ѹ��״̬��ѯ����*/
	//�������ȼ�
#define ACP_STATE_QUERY_TASK_PRIO		16
	//�����ջ��С	
#define ACP_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB AcpStateQueryTCB; 
	//�����ջ	
static CPU_STK ACP_STATE_QUERY_TASK_STK[ACP_STATE_QUERY_STK_SIZE];
	//������
void AcpStateQuery_Take(void *p_arg);


/*ATC���źţ�״̬��ѯ����*/
	//�������ȼ�
#define ATC_STATE_QUERY_TASK_PRIO		17
	//�����ջ��С	
#define ATC_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB ATCStateQueryTCB; 
	//�����ջ	
static CPU_STK ATC_STATE_QUERY_TASK_STK[ATC_STATE_QUERY_STK_SIZE];
	//������
void ATCStateQuery_Take(void *p_arg);

/*PIS���㲥��״̬��ѯ����*/
	//�������ȼ�
#define PIS_STATE_QUERY_TASK_PRIO		18
	//�����ջ��С	
#define PIS_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB PISStateQueryTCB; 
	//�����ջ	
static CPU_STK PIS_STATE_QUERY_TASK_STK[PIS_STATE_QUERY_STK_SIZE];
	//������
void PISStateQuery_Take(void *p_arg);

/*VVVF1״̬��ѯ����*/
	//�������ȼ�
#define VVVF1_STATE_QUERY_TASK_PRIO		12
	//�����ջ��С	
#define VVVF1_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB VVVF1StateQueryTCB; 
	//�����ջ	
static CPU_STK VVVF1_STATE_QUERY_TASK_STK[VVVF1_STATE_QUERY_STK_SIZE];
	//������
void VVVF1StateQuery_Take(void *p_arg);

/*VVVF2״̬��ѯ����*/
	//�������ȼ�
#define VVVF2_STATE_QUERY_TASK_PRIO		13
	//�����ջ��С	
#define VVVF2_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB VVVF2StateQueryTCB; 
	//�����ջ	
static CPU_STK VVVF2_STATE_QUERY_TASK_STK[VVVF2_STATE_QUERY_STK_SIZE];
	//������
void VVVF2StateQuery_Take(void *p_arg);


/*SIV״̬��ѯ����*/
	//�������ȼ�
#define SIV_STATE_QUERY_TASK_PRIO		12
	//�����ջ��С	
#define SIV_STATE_QUERY_STK_SIZE 		512
	//������ƿ�
static OS_TCB SIVStateQueryTCB; 
	//�����ջ	
static CPU_STK SIV_STATE_QUERY_TASK_STK[SIV_STATE_QUERY_STK_SIZE];
	//������
void SIVStateQuery_Take(void *p_arg);



///*UART3 ���ܴ�������*/
//	//�������ȼ�
//#define USART3Rx_TASK_PRIO		19
//	//�����ջ��С	
//#define USART3Rx_STK_SIZE 		512
//	//������ƿ�
//OS_TCB  USART3Rx_TCB;
//	//�����ջ	
//static CPU_STK  USART3Rx_TASK_STK[USART3Rx_STK_SIZE];
//	//������
//void USART3RxTake(void *p_arg);


/*can2 ���ܷ���LED����˸����*/
	//�������ȼ�
#define CAN2LED_TASK_PRIO		20
	//�����ջ��С	
#define CAN2LED_STK_SIZE 		128
	//������ƿ�
static OS_TCB  CAN2LED_TCB;
	//�����ջ	
static CPU_STK  CAN2LED_TASK_STK[CAN2LED_STK_SIZE];
	//������
void CAN2LEDTake(void *p_arg);


/*�������Ĵ���*/
//static  OS_SEM     	SEM_SYNCH;	   	//����ͬ��
OS_SEM  Slave_Start_Sem;						//��վ����������ź���
OS_SEM	DI_Check_Sem;								//����DI�ɼ��ź���

/*�����¼���־��*/
OS_FLAG_GRP	CAN2LedEventFlag;		//����can2led��˸�¼���־��

OS_FLAG_GRP	DoorEventFlags;			//������ϵͳЭ���� ��DOOR���Ŵ������� �¼���־��
OS_FLAG_GRP ACCEventFlags;			//�յ��¼���־��
OS_FLAG_GRP BCUEventFlags;			//BCU�ƶ� �¼���־��
OS_FLAG_GRP ACPEventFlags;			//ACP��ѹ�� �¼���־��
OS_FLAG_GRP SIVEventFlags;			//SIV������Դ �¼���־��
OS_FLAG_GRP VVVF1EventFlags;		//VVVF1ǣ��1 �¼���־��
OS_FLAG_GRP VVVF2EventFlags;		//VVVF2ǣ��2 �¼���־��
OS_FLAG_GRP ATCEventFlags;			//ATC�ź� �¼���־��
OS_FLAG_GRP PISEventFlags;			//PIS�㲥 �¼���־��

//��ʱ��1		������ʹ�ã���Ҫ��led��˸
//OS_TMR 	tmr1;		
//void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����


/*��ϵͳЭ������Ҫ�Ķ�ʱ������*/
OS_TMR DoorTmr;	//�Ŷ�ʱ��
void DoorTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR ACCTmr;	//�յ���ʱ��
void ACCTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR BCUTmr;	//�ƶ���ʱ��
void BCUTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR ACPTmr;	//��ѹ����ʱ��
void ACPTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR PISTmr;	//�㲥��ʱ��
void PISTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR ATCTmr;	//�źŶ�ʱ��
void ATCTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR SIVTmr;	//������Դ��ʱ��
void SIVTrmCallBack(void *p_tmr, void *p_arg);

OS_TMR VVVF1Tmr;	//VVVF1��ʱ��
void VVVF1TrmCallBack(void *p_tmr, void *p_arg);

OS_TMR VVVF2Tmr;	//VVVF2��ʱ��
void VVVF2TrmCallBack(void *p_tmr, void *p_arg);

OS_TMR DITmr;			//DI�ɼ���ʱ��
void DITrmCallBack(void *p_tmr, void *p_arg);



/*485��ʹ�õ�ȫ�ֱ����Ķ���*/
u8 g_Uart2RxStartFlag=0;
u8 g_Uart2RxDataFlag=0;
u8 g_Uart2FrameErrorFlag=0;

u8 g_Uart3RxStartFlag=0;
u8 g_Uart3RxDataFlag=0;
u8 g_Uart3FrameErrorFlag=0;

u8 g_Uart4RxStartFlag=0;
u8 g_Uart4RxDataFlag=0;
u8 g_Uart4FrameErrorFlag=0;
u8 g_BCURxSdOrTdFlag=0;
u8 g_Uart4NackFlag=0;

//���ȫ�ֱ�������������ť��ֵ���Ӷ�ѡ�����
u8 ChooseBoard=0;		

extern  uint8_t UART1_Rx_Cnt;

//	extern u8 HSE_AND_HSI;

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
	
		Uart1_init(); /* uart1 ��ʼ������,��ǰ�ĳ�ʼ������ֹ�м��д�ӡ��ʾ��Ϣ���޷���� */
		
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
  AppObjCreate();                                             /* Create Applicaiton kernel objects                    */
	
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
                 (OS_TASK_PTR)Main_task, 				//������
                 (void		  *)0,								//���ݸ��������Ĳ���
                 (OS_PRIO	   )Main_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&Main_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)Main_TASK_STK[Main_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)Main_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,								//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,								//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,								//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/,		//����ѡ��
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

		//������վ��������
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
								 
	//	/*UART3 ���ܴ�������*/
	//	OSTaskCreate((OS_TCB 		* )&USART3Rx_TCB,					//������ƿ�
	//								(CPU_CHAR	* )"Usart3RxTask", 				//��������
	//                 (OS_TASK_PTR)USART3RxTake, 				//������
	//                 (void		  *)0,										//���ݸ��������Ĳ���
	//                 (OS_PRIO	   )USART3Rx_TASK_PRIO,   //�������ȼ�
	//                 (CPU_STK   *)&USART3Rx_TASK_STK[0],//�����ջ����ַ
	//                 (CPU_STK_SIZE)USART3Rx_TASK_STK[USART3Rx_STK_SIZE/10],		//�����ջ�����λ
	//                 (CPU_STK_SIZE)USART3Rx_STK_SIZE,		//�����ջ��С
	//                 (OS_MSG_QTY  )0,										//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
	//                 (OS_TICK	    )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
	//                 (void   	   *)0,										//�û�����Ĵ洢��
	//                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
	//                 (OS_ERR 	   *)&err);								//��Ÿú�������ʱ�ķ���ֵ		
	
								 
	/*WHH*/							 
									
	/*DI�ɼ���ѯ����*/
	OSTaskCreate((OS_TCB 		* )&DICheckTCB,						//������ƿ�
								(CPU_CHAR	* )"DICheckTask", 				//��������
                 (OS_TASK_PTR)DICheck_Take, 				//������
                 (void		  *)0,										//���ݸ��������Ĳ���
                 (OS_PRIO	   )DI_CHECK_TASK_PRIO,   //�������ȼ�
                 (CPU_STK   *)&DI_CHECK_TASK_STK[0],//�����ջ����ַ
                 (CPU_STK_SIZE)DI_CHECK_TASK_STK[DI_CHECK_STK_SIZE/10],		//�����ջ�����λ
                 (CPU_STK_SIZE)DI_CHECK_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,										//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	    )0,										//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	   *)0,										//�û�����Ĵ洢��
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR/* | OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
                 (OS_ERR 	   *)&err);								//��Ÿú�������ʱ�ķ���ֵ											 

	switch(ChooseBoard)
	{
		case MC1_MFD1_NODEID:
		case T_MFD1_NODEID:	
		case M_MFD1_NODEID:
		case T1_MFD1_NODEID:
		case T2_MFD1_NODEID:
		case MC2_MFD1_NODEID:		
								 
		/*����״̬��ѯ����*/
		OSTaskCreate((OS_TCB 		* )&DoorStateQueryTCB,						//������ƿ�
									(CPU_CHAR	* )"DoorStateQuery_task", 				//��������
									 (OS_TASK_PTR)DoorStateQuery_Take, 					//������
									 (void		  *)0,														//���ݸ��������Ĳ���
									 (OS_PRIO	   )DOOR_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
									 (CPU_STK   *)&DOOR_STATE_QUERY_TASK_STK[0],	//�����ջ����ַ
									 (CPU_STK_SIZE)DOOR_STATE_QUERY_TASK_STK[DOOR_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
									 (CPU_STK_SIZE)DOOR_STATE_QUERY_STK_SIZE,			//�����ջ��С
									 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
									 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
									 (void   	   *)0,					//�û�����Ĵ洢��
									 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
									 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
							 							 
		/*�յ�״̬��ѯ����*/						 
			OSTaskCreate((OS_TCB 		* )&AccStateQueryTCB,						//������ƿ�
								(CPU_CHAR	* )"AccStateQueryTask", 						//��������
								 (OS_TASK_PTR)AccStateQuery_Take, 						//������
								 (void		  *)0,															//���ݸ��������Ĳ���
								 (OS_PRIO	   )ACC_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
								 (CPU_STK   *)&ACC_STATE_QUERY_TASK_STK[0],		//�����ջ����ַ
								 (CPU_STK_SIZE)ACC_STATE_QUERY_TASK_STK[ACC_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
								 (CPU_STK_SIZE)ACC_STATE_QUERY_STK_SIZE,			//�����ջ��С
								 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
								 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
								 (void   	   *)0,					//�û�����Ĵ洢��
								 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
								 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ								 									 
							 
		/*�ƶ�״̬��ѯ����*/				 
			OSTaskCreate((OS_TCB 		* )&BcuStateQueryTCB,						//������ƿ�
								(CPU_CHAR	* )"BcuStateQueryTask", 						//��������
								 (OS_TASK_PTR)BcuStateQuery_Take, 						//������
								 (void		  *)0,															//���ݸ��������Ĳ���
								 (OS_PRIO	   )BCU_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
								 (CPU_STK   *)&BCU_STATE_QUERY_TASK_STK[0],		//�����ջ����ַ
								 (CPU_STK_SIZE)BCU_STATE_QUERY_TASK_STK[BCU_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
								 (CPU_STK_SIZE)BCU_STATE_QUERY_STK_SIZE,			//�����ջ��С
								 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
								 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
								 (void   	   *)0,					//�û�����Ĵ洢��
								 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
								 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ			
	
			break;
		default:
			printf("MFD Board Choose Err��\r\n");
			break;
	}
	
	switch(ChooseBoard)
	{
		case MC1_MFD1_NODEID:
		case MC2_MFD1_NODEID:	
		case M_MFD1_NODEID:	
				/*VVVF1,VVVF2*/
				/*VVVF1״̬��ѯ����*/						 
				OSTaskCreate((OS_TCB 		* )&VVVF1StateQueryTCB,						//������ƿ�
									(CPU_CHAR	* )"VVVF1StateQueryTask", 						//��������
									 (OS_TASK_PTR)VVVF1StateQuery_Take, 						//������
									 (void		  *)0,																//���ݸ��������Ĳ���
									 (OS_PRIO	   )VVVF1_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
									 (CPU_STK   *)&VVVF1_STATE_QUERY_TASK_STK[0],		//�����ջ����ַ
									 (CPU_STK_SIZE)VVVF1_STATE_QUERY_TASK_STK[VVVF1_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
									 (CPU_STK_SIZE)VVVF1_STATE_QUERY_STK_SIZE,			//�����ջ��С
									 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
									 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
									 (void   	   *)0,					//�û�����Ĵ洢��
									 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
									 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ	
				if(err == OS_ERR_NONE)
				{
					printf("VVVF1Task�����ɹ�!\r\n");
				}
				else
				{
					printf("VVVF1Task����ʧ��!\r\n");
				}			
				
				/*VVVF1״̬��ѯ����*/						 
				OSTaskCreate((OS_TCB 		* )&VVVF2StateQueryTCB,						//������ƿ�
									(CPU_CHAR	* )"VVVF2StateQueryTask", 						//��������
									 (OS_TASK_PTR)VVVF2StateQuery_Take, 						//������
									 (void		  *)0,																//���ݸ��������Ĳ���
									 (OS_PRIO	   )VVVF2_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
									 (CPU_STK   *)&VVVF2_STATE_QUERY_TASK_STK[0],		//�����ջ����ַ
									 (CPU_STK_SIZE)VVVF2_STATE_QUERY_TASK_STK[VVVF2_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
									 (CPU_STK_SIZE)VVVF2_STATE_QUERY_STK_SIZE,			//�����ջ��С
									 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
									 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
									 (void   	   *)0,					//�û�����Ĵ洢��
									 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
									 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ	
				if(err == OS_ERR_NONE)
				{
					printf("VVVF2Task�����ɹ�!\r\n");
				}
				else
				{
					printf("VVVF2Task����ʧ��!\r\n");
				}						
		
			break;
		
		case T_MFD1_NODEID:	
		case T1_MFD1_NODEID:	
			/*SIV*/
			/*SIV״̬��ѯ����*/					 
			OSTaskCreate((OS_TCB 		* )&SIVStateQueryTCB,						//������ƿ�
								(CPU_CHAR	* )"SIVStateQueryTask", 						//��������
								 (OS_TASK_PTR)SIVStateQuery_Take, 						//������
								 (void		  *)0,															//���ݸ��������Ĳ���
								 (OS_PRIO	   )SIV_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
								 (CPU_STK   *)&SIV_STATE_QUERY_TASK_STK[0],		//�����ջ����ַ
								 (CPU_STK_SIZE)SIV_STATE_QUERY_TASK_STK[SIV_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
								 (CPU_STK_SIZE)SIV_STATE_QUERY_STK_SIZE,			//�����ջ��С
								 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
								 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
								 (void   	   *)0,					//�û�����Ĵ洢��
								 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
								 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ	
			if(err == OS_ERR_NONE)
			{
				printf("SIVTask�����ɹ�!\r\n");
			}
			else
			{
				printf("SIVTask����ʧ��!\r\n");
			}				
			break;
		
		case MC1_MFD2_NODEID:
		case MC2_MFD2_NODEID:	
			/*ATC ,PIS*/
			/*ATC���źţ�״̬��ѯ����*/
		OSTaskCreate((OS_TCB 		* )&ATCStateQueryTCB,						//������ƿ�
									(CPU_CHAR	* )"ATCStateQuery_task", 				//��������
									 (OS_TASK_PTR)ATCStateQuery_Take, 					//������
									 (void		  *)0,														//���ݸ��������Ĳ���
									 (OS_PRIO	   )ATC_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
									 (CPU_STK   *)&ATC_STATE_QUERY_TASK_STK[0],	//�����ջ����ַ
									 (CPU_STK_SIZE)ATC_STATE_QUERY_TASK_STK[ATC_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
									 (CPU_STK_SIZE)ATC_STATE_QUERY_STK_SIZE,			//�����ջ��С
									 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
									 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
									 (void   	   *)0,					//�û�����Ĵ洢��
									 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
									 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
									 
		/*PIS���㲥��״̬��ѯ����*/							 
		OSTaskCreate((OS_TCB 		* )&PISStateQueryTCB,						//������ƿ�
									(CPU_CHAR	* )"PISStateQuery_task", 				//��������
									 (OS_TASK_PTR)PISStateQuery_Take, 					//������
									 (void		  *)0,														//���ݸ��������Ĳ���
									 (OS_PRIO	   )PIS_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
									 (CPU_STK   *)&PIS_STATE_QUERY_TASK_STK[0],	//�����ջ����ַ
									 (CPU_STK_SIZE)PIS_STATE_QUERY_TASK_STK[PIS_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
									 (CPU_STK_SIZE)PIS_STATE_QUERY_STK_SIZE,			//�����ջ��С
									 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
									 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
									 (void   	   *)0,					//�û�����Ĵ洢��
									 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
									 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ						
			break;
		
		case MC1_MFD3_NODEID:
		case MC2_MFD3_NODEID:
			/*ACP*/
			/*��ѹ��״̬��ѯ����*/						 
				OSTaskCreate((OS_TCB 		* )&AcpStateQueryTCB,						//������ƿ�
									(CPU_CHAR	* )"AcpStateQueryTask", 						//��������
									 (OS_TASK_PTR)AcpStateQuery_Take, 						//������
									 (void		  *)0,															//���ݸ��������Ĳ���
									 (OS_PRIO	   )ACP_STATE_QUERY_TASK_PRIO,    	//�������ȼ�
									 (CPU_STK   *)&ACP_STATE_QUERY_TASK_STK[0],		//�����ջ����ַ
									 (CPU_STK_SIZE)ACP_STATE_QUERY_TASK_STK[ACP_STATE_QUERY_STK_SIZE/10],	//�����ջ�����λ
									 (CPU_STK_SIZE)ACP_STATE_QUERY_STK_SIZE,			//�����ջ��С
									 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
									 (OS_TICK	    )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
									 (void   	   *)0,					//�û�����Ĵ洢��
									 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR /*| OS_OPT_TASK_SAVE_FP*/),		//����ѡ��
									 (OS_ERR 	   *)&err);				//��Ÿú�������ʱ�ķ���ֵ		
			break;
									 
		default :
			break;
	}
	
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
	//BSP_OS_SemCreate(&SEM_SYNCH, 0,(CPU_CHAR *)"SEM_SYNCH");


	//����һ��can2�����ź���
	OSSemCreate ((OS_SEM*	)&can2slaverecv_sem,
                 (CPU_CHAR*	)"can2slaverecvsem",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
	if(err == OS_ERR_NONE)
	{
		printf("Creat can2 Rx Sem Success!\r\n");
	}
	else
	{
		printf("Creat can2 Rx Sem False!\r\n");
	}

	//����һ��can2�����ź���
	OSSemCreate ((OS_SEM*	)&can2slavetran_sem,
                 (CPU_CHAR*	)"can2slavetransem",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
	if(err == OS_ERR_NONE)
	{
		printf("Creat can2 Tx Sem Success!\r\n");
	}
	else
	{
		printf("Creat can2 Tx Sem False!\r\n");
	}

	/*����DI�ɼ��ź���	*/
	OSSemCreate ((OS_SEM*	)&DI_Check_Sem,
                 (CPU_CHAR*	)"DiCheckSem",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
	if(err == OS_ERR_NONE)
	{
		printf("Creat DI Sem Success!\r\n");
	}
	else
	{
		printf("Creat DI Sem False!\r\n");
	}
	
	/*�����¼���־��*/
	OSFlagCreate((OS_FLAG_GRP*)&CAN2LedEventFlag,				//ָ���¼���־��
                 (CPU_CHAR*	  )"CAN2 Led Event Flag",	//����
                 (OS_FLAGS	  )0,											//�¼���־���ʼֵ
                 (OS_ERR*  	  )&err);									//������	
	if(err == OS_ERR_NONE)
	{
		printf("Creat can2Led EvenFlag Success!\r\n");
	}
	else
	{
		printf("Creat can2Led EvenFlag False!\r\n");
	}	

	//	//������ʱ��1
	//	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
	//                (CPU_CHAR	*)"tmr1",	//��ʱ������
	//                (OS_TICK	 )0,			//��ʼ����ʱ������ʱֵ
	//                (OS_TICK	 )100,    //100*1=100ms  �ظ�����
	//                (OS_OPT		 )OS_OPT_TMR_PERIODIC,	 //����ģʽ
	//                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
	//                (void	    *)0,			//�ص纯������Ϊ0
	//                (OS_ERR	  *)&err);	//���صĴ�����			
	//	if(err == OS_ERR_NONE)
	//	{
	//		printf("����Trm1�ɹ�!\r\n");
	//	}
	//	else
	//	{
	//		printf("����Trm1ʧ��!\r\n");
	//	}			
								
	/*6�������MFD1 �๦�ܰ忨���� �ţ��յ� ���ƶ�,Ӳ�߲ɼ�*/							
	switch(ChooseBoard)
	{
		case MC1_MFD1_NODEID:
		case T_MFD1_NODEID:	
		case M_MFD1_NODEID:
		case T1_MFD1_NODEID:
		case T2_MFD1_NODEID:
		case MC2_MFD1_NODEID:
			
			//������ʱ�� ��(Door)
			OSTmrCreate((OS_TMR		*)&DoorTmr,		//��ʱ��1
									(CPU_CHAR	*)"DoorTmr",	//��ʱ������
									(OS_TICK	 )10,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )25,   		  //25*1=25ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)DoorTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����			
			if(err == OS_ERR_NONE)
			{
				printf("����DoorTrm�ɹ�!\r\n");
			}
			else
			{
				printf("����DoorTrmʧ��!\r\n");
			}	
									
			//������ʱ�� �յ�(ACC)
			OSTmrCreate((OS_TMR		*)&ACCTmr,		//��ʱ��1
									(CPU_CHAR	*)"ACCTmr",		//��ʱ������
									(OS_TICK	 )20,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )50,   		  //50*1=50ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)ACCTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����	
			if(err == OS_ERR_NONE)
			{
				printf("����ACCTrm�ɹ�!\r\n");
			}
			else
			{
				printf("����ACCTrmʧ��!\r\n");
			}										
			//������ʱ�� �ƶ�(BCU)
			OSTmrCreate((OS_TMR		*)&BCUTmr,		//��ʱ��1
									(CPU_CHAR	*)"BCUTmr",		//��ʱ������
									(OS_TICK	 )30,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )200,   		  //200*1=200ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)BCUTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����		
			if(err == OS_ERR_NONE)
			{
				printf("����BCUTrm�ɹ�!\r\n");
			}
			else
			{
				printf("����BCUTrmʧ��!\r\n");
			}	
														
			//������ʱ�� Ӳ��(DI)
			OSTmrCreate((OS_TMR		*)&DITmr,			//��ʱ��1
									(CPU_CHAR	*)"DITmr",		//��ʱ������
									(OS_TICK	 )0,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )20,   		  //20*1=20ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 	//����ģʽ
									(OS_TMR_CALLBACK_PTR)DITrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����		
			if(err == OS_ERR_NONE)
			{
				printf("����DITrm�ɹ�!\r\n");
			}
			else
			{
				printf("����DITrmʧ��!\r\n");
			}	
														
			/*������ϵͳЭ���е��¼���־��*/
			/*Door*/
			OSFlagCreate((OS_FLAG_GRP*)&DoorEventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"Door Event Flags",			//����
									 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);										//������	
			if(err == OS_ERR_NONE)
			{
				printf("����DoorEvenFlag�ɹ�!\r\n");
			}
			else
			{
				printf("����DoorEvenFlagʧ��!\r\n");
			}	
			
			/*ACC*/							
			OSFlagCreate((OS_FLAG_GRP*)&ACCEventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"ACC Event Flags",				//����
									 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);										//������							 
				if(err == OS_ERR_NONE)
			{
				printf("����ACCEvenFlag�ɹ�!\r\n");
			}
			else
			{
				printf("����ACCEvenFlagʧ��!\r\n");
			}	
			
			/*BCU*/
			OSFlagCreate((OS_FLAG_GRP*)&BCUEventFlags,					//ָ���¼���־��
								 (CPU_CHAR*	  )"BCU Event Flags",				//����
								 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
								 (OS_ERR*  	  )&err);										//������							 
			if(err == OS_ERR_NONE)
			{
				printf("����BCUEvenFlag�ɹ�!\r\n");
			}
			else
			{
				printf("����BCUEvenFlagʧ��!\r\n");
			}	
			
			break;
		default :
			break;
	}								
								
	switch(ChooseBoard)
	{
		case MC1_MFD1_NODEID:
		case MC2_MFD1_NODEID:	
		case M_MFD1_NODEID:	
			//������ʱ�� ǣ�� ��VVVF1��/*100ms ��һ��sdr��100ms��һ��tdr���ܵĶ�ʱ������Ϊ200ms���ڻص������о�̬��������*/
			OSTmrCreate((OS_TMR		*)&VVVF1Tmr,		//��ʱ��1
									(CPU_CHAR	*)"VVVF1Tmr",		//��ʱ������
									(OS_TICK	 )2000,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )100,   		  //100*1=100ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)VVVF1TrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����	
			if(err == OS_ERR_NONE)
			{
				printf("����VVVF1Trm�ɹ�!\r\n");
			}
			else
			{
				printf("����VVVF1Trmʧ��!\r\n");
			}	
			
			//������ʱ�� ǣ�� ��VVVF2��
			OSTmrCreate((OS_TMR		*)&VVVF2Tmr,		//��ʱ��1
									(CPU_CHAR	*)"VVVF2Tmr",		//��ʱ������
									(OS_TICK	 )2020,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )100,   		  //100*1=100ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)VVVF2TrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����			
			if(err == OS_ERR_NONE)
			{
				printf("����VVVF2Trm�ɹ�!\r\n");
			}
			else
			{
				printf("����VVVF2Trmʧ��!\r\n");
			}	
			/*VVVF1*/							
			OSFlagCreate((OS_FLAG_GRP*)&VVVF1EventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"VVVF1 Event Flags",				//����
									 (OS_FLAGS	  )0,													//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);											//������		
			if(err == OS_ERR_NONE)
			{
				printf("����VVVF1EvenFlag�ɹ�!\r\n");
			}
			else
			{
				printf("����VVVF1EvenFlagʧ��!\r\n");
			}	
			
			/*VVVF2*/							
			OSFlagCreate((OS_FLAG_GRP*)&VVVF2EventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"VVVF2 Event Flags",				//����
									 (OS_FLAGS	  )0,													//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);											//������			
			if(err == OS_ERR_NONE)
			{
				printf("����VVVF2EvenFlag�ɹ�!\r\n");
			}
			else
			{
				printf("����VVVF2EvenFlagʧ��!\r\n");
			}	
			break;
									
		case T_MFD1_NODEID:	
		case T1_MFD1_NODEID:	
			//������ʱ�� ������Դ(SIV)
			OSTmrCreate((OS_TMR		*)&SIVTmr,		//��ʱ��1
									(CPU_CHAR	*)"SIVTmr",		//��ʱ������
									(OS_TICK	 )5000,				//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )200,   		  //200*1=200ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)SIVTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����		
			if(err == OS_ERR_NONE)
			{
				printf("����SIVTrm�ɹ�!\r\n");
			}
			else
			{
				printf("����SIVTrmʧ��!\r\n");
			}		
			
			/*SIV*/							
			OSFlagCreate((OS_FLAG_GRP*)&SIVEventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"SIV Event Flags",				//����
									 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);										//������		
			if(err == OS_ERR_NONE)
			{
				printf("����SIVEvenFlag�ɹ�!\r\n");
			}
			else
			{
				printf("����SIVEvenFlagʧ��!\r\n");
			}			
			
			break;
												
		case MC1_MFD2_NODEID:
		case MC2_MFD2_NODEID:	
			//������ʱ�� �ź�(ATC)
			OSTmrCreate((OS_TMR		*)&ATCTmr,		//��ʱ��1
									(CPU_CHAR	*)"ATCTmr",		//��ʱ������
									(OS_TICK	 )20,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )200,   		  //200*1=200ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)ATCTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����	
									
			//������ʱ�� �㲥(PIS)
			OSTmrCreate((OS_TMR		*)&PISTmr,		//��ʱ��1
									(CPU_CHAR	*)"PISTmr",		//��ʱ������
									(OS_TICK	 )40,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )200,   		  //200*1=200ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)PISTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����		
									
			//������ʱ�� Ӳ��(DI)
			OSTmrCreate((OS_TMR		*)&DITmr,		//��ʱ��1
									(CPU_CHAR	*)"DITmr",		//��ʱ������
									(OS_TICK	 )0,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )20,   		  //20*1=20ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)DITrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����					

			/*ATC*/							
			OSFlagCreate((OS_FLAG_GRP*)&ATCEventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"ATC Event Flags",				//����
									 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);										//������			
			/*PIS*/							
			OSFlagCreate((OS_FLAG_GRP*)&PISEventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"PIS Event Flags",				//����
									 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);										//������																						
			break;
									
		case MC1_MFD3_NODEID:
		case MC2_MFD3_NODEID:
			//������ʱ�� ��ѹ��(ACP)
			OSTmrCreate((OS_TMR		*)&ACPTmr,		//��ʱ��1
									(CPU_CHAR	*)"ACPTmr",		//��ʱ������
									(OS_TICK	 )0,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )1000,   		//1000*1=1000ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)ACPTrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����		
									
			//������ʱ�� Ӳ��(DI)
			OSTmrCreate((OS_TMR		*)&DITmr,		//��ʱ��1
									(CPU_CHAR	*)"DITmr",		//��ʱ������
									(OS_TICK	 )0,					//��ʼ����ʱ������ʱֵ
									(OS_TICK	 )20,   		  //20*1=20ms  �ظ�����
									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
									(OS_TMR_CALLBACK_PTR)DITrmCallBack,	//��ʱ��1�ص�����
									(void	    *)0,					//�ص���������Ϊ0
									(OS_ERR	  *)&err);			//���صĴ�����				

			/*ACP�¼���־��*/							
			OSFlagCreate((OS_FLAG_GRP*)&ACPEventFlags,					//ָ���¼���־��
									 (CPU_CHAR*	  )"ACP Event Flags",				//����
									 (OS_FLAGS	  )0,												//�¼���־���ʼֵ
									 (OS_ERR*  	  )&err);										//������							 				
			break;
									
		//		case T_MFD2_NODEID:
		//		case M_MFD2_NODEID:
		//		case T1_MFD2_NODEID:
		//		case T2_MFD2_NODEID:
		//		case MC1_MFD4_NODEID:
		//		case MC2_MFD4_NODEID:	
		//						//������ʱ�� Ӳ��(DI)
		//			OSTmrCreate((OS_TMR		*)&DITmr,			//��ʱ��1
		//									(CPU_CHAR	*)"DITmr",		//��ʱ������
		//									(OS_TICK	 )0,					//��ʼ����ʱ������ʱֵ
		//									(OS_TICK	 )20,   		  //20*1=20ms  �ظ�����
		//									(OS_OPT		 )OS_OPT_TMR_PERIODIC,	 		//����ģʽ
		//									(OS_TMR_CALLBACK_PTR)DITrmCallBack,	//��ʱ��1�ص�����
		//									(void	    *)0,					//�ص���������Ϊ0
		//									(OS_ERR	  *)&err);			//���صĴ�����					
		//			break;

	}								
							
	OS_CRITICAL_EXIT();	//�˳��ٽ���									
}


	//extern u8 User_CAN1Transmit(uint32_t id,uint8_t* val,uint8_t Len);
	//extern u8 User_CAN2Transmit(uint32_t id,uint8_t* val,uint8_t Len);
	//extern unsigned char canInit(CAN_TypeDef* CANx,unsigned int bitrate);

/*EEPROM ����*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
#define sEE_WRITE_ADDRESS1        0x50
#define sEE_READ_ADDRESS1         0x50
#define BUFFER_SIZE1            (countof(aTxBuffer1)-1)
#define BUFFER_SIZE2            (countof(aTxBuffer2)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t aTxBuffer1[] = "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
															";
uint8_t aTxBuffer2[] = "/* STM32F40xx I2C Firmware  */";
uint8_t aRxBuffer1[BUFFER_SIZE1];
uint8_t aRxBuffer2[BUFFER_SIZE2];

volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
volatile uint16_t uhNumDataRead = 0;

/* Private functions ---------------------------------------------------------*/
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);


//extern void can_recv_thread(void);
//extern	void can_slave_init(void);


void Main_task(void *p_arg)
{
	OS_ERR err;

	/*iap UART1*/
	u16 UartOldCount=0;	//�ϵĴ��ڽ�������ֵ
	//u16 AppLenth=0;	//���յ���app���볤��
	
	//	u8 CLTxBuf[10]={9,8,7,6,5,4,3,2,1,0};
	
	/*HDLC*/
		u8 ATxBuf[200]={0,1,2,3,4,5,6,7,8,9,};
		u8	BTxBuf[200]={9,8,7,6,5,4,3,2,1,0};
	//		u8 Rxlen;
		u8 i;
		u8 aa=0;
					
		for(i=0;i<128;i++)
		{
			szASC16[i]=i;
		}
		memcpy(ATxBuf,szASC16,sizeof(ATxBuf));
		memcpy(BTxBuf,szASC16,sizeof(BTxBuf));
		
	(void)p_arg;
	printf("���뵽Main����\r\n");
	


	/*110 24*/
	//Vol_110VGpio_Init();
	//Vol_24VGpio_Init();

	RunLedGpio_Init();			//��ʼ�����е�
	
	SAF82525_Init(SAF_BAUD_RATE_125000);//HDLC��ʼ��

	//	SAF82525_INT_GPIO_Init();
	//	SAF82525_Rest_CPIO_Init();
	//	SAF82525_GPIO_Init();
	//	
	//	//step1: SAF82525оƬ��λ
	//	SAF82525_REST_ON();		//������λ
	//	delay_ms(10);
	//	SAF82525_REST_OFF();  //�رո�λ

		/*ʱ��Ƶ�ʲ���*/
	//	System_Clk_Output_Init();
	//	HSE_Clk_Output_Init();
		
		/*CL*/
	//	MTDMFD_CL_Send_Data(&CLTxBuf[0],10);
	
	
	
	

			//		/* Initialize the I2C EEPROM driver ----------------------------------------*/
			//		sEE_Init();  

			//	/*EPPROM*/
			//	i=0;
			////	for(i=0;i<64;i++)
			//	{
			//		printf("w1=%d\r\n",sEE_WRITE_ADDRESS1+i*BUFFER_SIZE1);	
			//		printf("w2=%d\r\n",sEE_WRITE_ADDRESS2+(i*BUFFER_SIZE1+1));
			//		
			//		/*EEPROM ����*/
			//		//printf("1_EEPROM\r\n");
			//		/* First write in the memory followed by a read of the written data --------*/
			//		/* Write on I2C EEPROM from sEE_WRITE_ADDRESS1 */
			//		sEE_WriteBuffer(aTxBuffer1, sEE_WRITE_ADDRESS1/*+i*BUFFER_SIZE1*/, BUFFER_SIZE1); 
			//		//printf("2_EEPROM\r\n");
			//		/* Wait for EEPROM standby state */
			//		sEE_WaitEepromStandbyState();  
			//		//printf("3_EEPROM\r\n");
			//		/* Set the Number of data to be read */
			//		uhNumDataRead = BUFFER_SIZE1;
			//		//printf("4_EEPROM\r\n");
			//		/* Read from I2C EEPROM from sEE_READ_ADDRESS1 */
			//		sEE_ReadBuffer(aRxBuffer1, sEE_READ_ADDRESS1/*+i*BUFFER_SIZE1*/, (uint16_t *)(&uhNumDataRead)); 

			////printf("5_EEPROM\r\n");
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
			//			//printf("a1=%s\r\n",aTxBuffer1);
			//			//printf("a2=%s\r\n",aRxBuffer1);
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
			//		uhNumDataRead =10;
			//	 	sEE_ReadBuffer(aRxBuffer1, sEE_READ_ADDRESS1+10,(uint16_t *)(&uhNumDataRead)); 

			//			printf("test=%s\r\n",aRxBuffer1);
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
			////			printf("a1=%s\r\n",aTxBuffer2);
			////			printf("a2=%s\r\n",aRxBuffer2);			
			//		 /* Check if the data written to the memory is read correctly */
			//		TransferStatus1 = Buffercmp(aTxBuffer2, aRxBuffer2, BUFFER_SIZE2);

			//		if (TransferStatus1 == PASSED)
			//		{
			//			printf(" Transfer 2 PASSED \r\n ");
			//		}
			//		else
			//		{
			//			printf(" Transfer 2 FAILED  \r\n");
			//		} 
			//		
			//		uhNumDataRead = 10;
			//	 	sEE_ReadBuffer(aRxBuffer2, sEE_READ_ADDRESS2+10,(uint16_t *)(&uhNumDataRead)); 

			//			printf("test=%s\r\n",aRxBuffer1);

			//	}		



	
	//OSTmrStart(&tmr1,&err);	//������ʱ��1
	//			printf("������ʱ��1\r\n");
				
	/*6�������MFD1 �๦�ܰ忨���� �ţ��յ� ���ƶ�,Ӳ�߲ɼ�*/							
	switch(ChooseBoard)
	{
		case MC1_MFD1_NODEID:
		case T_MFD1_NODEID:	
		case M_MFD1_NODEID:
		case T1_MFD1_NODEID:
		case T2_MFD1_NODEID:
		case MC2_MFD1_NODEID:			
			OSTmrStart(&DoorTmr,&err);	//������ʱ��Door
						printf("������ʱ��Door\r\n");			
			OSTmrStart(&ACCTmr,&err);	//������ʱ��ACC
						printf("������ʱ��ACC\r\n");					
			OSTmrStart(&BCUTmr,&err);	//������ʱ��BCU
						printf("������ʱ��BCU\r\n");					
			OSTmrStart(&DITmr,&err);	//������ʱ��DI
						printf("������ʱ��DI\r\n");
			break;
	}	
		
	switch(ChooseBoard)
	{
		case MC1_MFD1_NODEID:
		case MC2_MFD1_NODEID:	
		case M_MFD1_NODEID:	
			OSTmrStart(&VVVF1Tmr,&err);	//������ʱ��VVVF1
						printf("������ʱ��VVVF1\r\n");					
			OSTmrStart(&VVVF2Tmr,&err);	//������ʱ��VVVF2
						printf("������ʱ��VVVF2\r\n");
			break;
		
		case T_MFD1_NODEID:	
		case T1_MFD1_NODEID:	
			OSTmrStart(&SIVTmr,&err);	//������ʱ��SIV
						printf("������ʱ��SIV\r\n");			
			break;
		
		case MC1_MFD2_NODEID:
		case MC2_MFD2_NODEID:
			OSTmrStart(&ATCTmr,&err);	//������ʱ��ATC
						printf("������ʱ��ATC\r\n");					
			OSTmrStart(&PISTmr,&err);	//������ʱ��PIS
						printf("������ʱ��PIS\r\n");					
			OSTmrStart(&DITmr,&err);	//������ʱ��DI
						printf("������ʱ��DI\r\n");
			break;
		
		case MC1_MFD3_NODEID:
		case MC2_MFD3_NODEID:
			OSTmrStart(&ACPTmr,&err);	//������ʱ��ACP
						printf("������ʱ��ACP\r\n");					
			OSTmrStart(&DITmr,&err);	//������ʱ��DI
						printf("������ʱ��DI\r\n");	
			break;
		
		//		case T_MTD2_MFD2_NODEID:
		//		case M_MTD3_MFD2_NODEID:
		//		case T1_MTD4_MFD2_NODEID:
		//		case T2_MTD5_MFD2_NODEID:
		//		case MC1_MTD1_MFD4_NODEID:
		//		case MC2_MTD6_MFD4_NODEID:	
		//					OSTmrStart(&DITmr,&err);	//������ʱ��DI
		//						printf("������ʱ��DI\r\n");
	}
		
				
				
	while(1)
	{
		//i++;
		RunLed_Turn();

//		/*��ť���*/
//		 aa=	MTDMFD_CanAddVal();
//			printf("MFD��ťֵ0x%x\r\n",aa);

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
		BSP_OS_TimeDlyMs(500);
	}	
	
}



//		void key_task(void *p_arg)		 	//������ʾ�����ջʹ��������Ѿ�cpuʹ��
//		{
//		//	OS_ERR err;
//		//	uint8_t ucKeyCode;
//		//	int64_t runtime; 
//		//	ucKeyCode=0;
//			(void)p_arg;	               /* ������������� */
//			printf("���뵽key����\r\n");
//			while(1)
//			{		
//		//		if(KEY_S1_UPDOWM() == Bit_RESET)
//		//		{
//		//			printf("���뵽��������\r\n");
//		//			OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);//��ʱ1s	//��������ʱȥ��
//		//			ucKeyCode	= KEY_Scan(0);	//�ٴ�ȷ�ϰ���״̬
//		//			if(KEY_S1_UPDOWM() == Bit_RESET)	
//		//			{
//		//				ucKeyCode=1;
//		//			}
//		//		}
//		//		
//		//		if(ucKeyCode==1)
//		//		{
//		//			ucKeyCode=0;
//		//			DispTaskInfo();		//��ӡ��ʾ��Ϣ			
//		//			runtime=OSTimeGet(&err);
//		//			printf("runtime=%lld\r\n",runtime);
//		//			printf("KeyEnd!\r\n");
//		//		}
//				
//				if(MTDMFD_CanAddVal() == 0xFF )
//				{
//					DispTaskInfo();		//��ӡ��ʾ��Ϣ	
//				}
//				
//				BSP_OS_TimeDlyMs(2000);//��ʱ2s
//			}
//		}

//		void TemperCheck_task(void *p_arg)			//�¶ȼ�������������������������¶ȣ���ʵʩ����
//		{

//			(void )p_arg;			//��ֹ����
//		//	LEDGpio_Init();
//			ADC1_CH16_Config();
//			printf("���뵽Temper����\r\n");	

//			while(1)
//			{
//		//		if(KEY_S1_UPDOWM() == Bit_RESET)
//		//		{
//					Get_Temprate();		//stm32�ڲ��ļ���¶Ȳ��� ������ADCת���¶�
//		//			LED_Turn (0);
//		//		}
//				BSP_OS_TimeDlyMs(3000);//��ʱ2s	,���ҽ����������л�
//			}
//		}



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
			//SlaveStateCount = 0;
			OSTaskDel((OS_TCB*)&CAN2SlaveStartupManagerTCB,&err);	//�����վ����������ִ��5�˺�ɾ�����������
		}						

		{
			e_nodeState slavestate;
			slavestate = getState(&MFDCAN2Slave_Data);
			debugprintf("can2slavestate=0x%x\r\n",slavestate);
			if(slavestate != Operational/* 0x05*/)
			{
					/*����һ�������ź� cansend*/
				
					Message BootMessage;
					BootMessage.cob_id = (0x700 | (ChooseBoard & 0x0F));
					BootMessage.data[0] = 0x00;
					BootMessage.len =1;
					BootMessage.rtr = CAN_RTR_DATA;
					canSend(CAN2,&BootMessage);
					
					SlaveStateCount++;
			}
	
			else
			{
				OSTaskDel((OS_TCB*)&CAN2SlaveStartupManagerTCB,&err);	//�����վ״̬λ����״̬��ɾ�����������
			}
		}
		BSP_OS_TimeDlyMs(2000);			//���������ʱ�ǽ��������Ⱥ�������������ĵ��ȡ�	
	}
}

/*����״̬��������*/
void DoorStateQuery_Take(void *p_arg)
{
	OS_ERR err;
	/*1.������SDR���� ��������*/
	static u8 DoorAdd=0;
	DoorSdrFrame DoorSDRFrame;
	static u8 TimeFlagCount=0;
	
	/*2.����SD���ݱ�������*/
	DoorSdFrame SdFrmaeData;
	//static u8 TxErrFlagCount=0;
	static u8 DoorErrFlagCount[8]={0};		//8���Ŵ�����������־
	s8 resvalue;
	
	memset(DoorErrFlagCount,0,8);
	
	while(1)
	{
		/*�ȴ�����SDR�¼���־*/
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&DoorEventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		
		if(err == OS_ERR_NONE)
		{
			if(can2_RxDoorSpeedTimBit[0] == 1)		//��ʾ����ʱ��
			{	
				TimeFlagCount++;
				if(TimeFlagCount >5)	//��ʾ5��
				{
					TimeFlagCount = 0;
					can2_RxDoorSpeedTimBit[0] = 0;
				}
			}
			
			DoorSDRFrame.DoorSdrData.Address =0x00;
			DoorSDRFrame.DoorSdrData.Sub_Address =0x00;
			DoorSDRFrame.DoorSdrData.Command = 0x20;
			DoorSDRFrame.DoorSdrData.SetClock = can2_RxDoorSpeedTimBit[0];
			DoorSDRFrame.DoorSdrData.Year = can2_RxTime[0];
			DoorSDRFrame.DoorSdrData.Month = can2_RxTime[1];
			DoorSDRFrame.DoorSdrData.Day = can2_RxTime[2];
			DoorSDRFrame.DoorSdrData.Hour = can2_RxTime[3];
			DoorSDRFrame.DoorSdrData.Minute = can2_RxTime[4];
			DoorSDRFrame.DoorSdrData.Second = can2_RxTime[5];
			DoorSDRFrame.DoorSdrData.Speed = can2_RxDoorSpeedTimBit[1];
			DoorSDRFrame.DoorSdrData.Dummy2 = 0x00;	

			/*����ŵĵ�ַ*/
			DoorAdd = DoorAdd+1;
			if(DoorAdd>8)
			{
				DoorAdd =1;
			}
			DoorSDRFrame.DoorSdrData.Address = DoorAdd;
			
			/*����Sdr*/
			SendDoorSdr(&DoorSDRFrame.DoorSdrData);	//�����ݷ��뵽����3�ķ��ͻ�����������

			
			
			/*2.����SD����*/
			{
				resvalue = GetDoorSd(USART3_RS485_RxBuffer,&SdFrmaeData);
				
				if(resvalue !=0)	//����ֵ�д���
				{
						//					TxErrFlagCount++;
						//					
						//					if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
						//					{
						//						can2_TxSubDevFault[0] = 0xFF;
						//						
						//						/* force emission of PDO by artificially changing last emitted */
						//						MFDCAN2Slave_Data.PDO_status[TPDO_1A9_SubDecFault].last_message.cob_id = 0;
						//						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1A9_SubDecFault);		//����PDO
						//						//debugprintf("Door2�������,va=%d!\r\n",resvalue);
						//						//printf("door2�����������ݳ�ʱ!\r\n");
						//						TxErrFlagCount =0;	//���
						//						//can2_TxSubDevFault_Door =0 ;//����Ź���
						//					}					

							DoorErrFlagCount[DoorAdd-1]++;		//��Ӧ�Ĵ������++
						
							if(DoorErrFlagCount[DoorAdd-1] >=5)//��ֵ�������ֵ� ��ϵͳ�����ʾλ
							{
								can2_TxMFD1SubFault[0] |= (1<<(7-(DoorAdd-1)));	  //�Ŷ�Ӧ��λ ��1  ��1,2,3,4,5,6,7,8 �ŷֱ��Ӧ��λ�ôӸߵ��ͣ�
								
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
								
								DoorErrFlagCount[DoorAdd-1] = 0;
								
								printf("Door�����쳣,%d!\r\n",resvalue);
							}
				}
				
				else		//������copy�������ֵ䣬�����͵�can������
				{
					debugprintf("door ok!\r\n");
					
					/*���� ��ͨ������*/
					DoorErrFlagCount[DoorAdd-1] = 0;
					can2_TxMFD1SubFault[0] &= (~(1<<(7-(DoorAdd-1))));	  //�Ŷ�Ӧ��λ ��0
					
					//MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0; //whh 2016-11-23
					//sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
					
					switch(SdFrmaeData.DoorSdData.Address)		//�����ŵĵ�ַ�ŵ���ͬ��λ��
					{
						case 0x01:		//�ӵ�ַ0x1
							memcpy(&can2_TxDoorSta1[0],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3		//WHH ,ע����ҪǿתΪ(U8 *)��ָ��ӵĵ�ַһ�βż�1
							memcpy(&can2_TxDoorSta1[1],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4
						
							//printf("st1=0x%02x,st2=0x%02x\r\n",can2_TxDoorSta1[0],can2_TxDoorSta1[1]);
							//sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_190_DoorSta1);		//����PDO,��ַ����8ʱ�ڷ��͵�can������
							break;
						
						case 0x02:
							memcpy(&can2_TxDoorSta2[0],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta2[1],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4
						
							//printf("st1=0x%02x,st2=0x%02x\r\n",can2_TxDoorSta2[0],can2_TxDoorSta2[1]);
							//sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_191_DoorSta2);	//����PDO
							break;
						
						case 0x03:
							memcpy(&can2_TxDoorSta1[2],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta1[3],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4
						
						  //sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_190_DoorSta1);	//����PDO
							break;
						
						case 0x04:
							memcpy(&can2_TxDoorSta2[2],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta2[3],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4
						
							//sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_191_DoorSta2);	//����PDO
							break;
						
						case 0x05:
							memcpy(&can2_TxDoorSta1[4],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta1[5],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4
						
							//sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_190_DoorSta1);	//����PDO
							break;
						case 0x06:
							memcpy(&can2_TxDoorSta2[4],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta2[5],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4
						
							//sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_191_DoorSta2);	//����PDO
							break;
						
						case 0x07:
							memcpy(&can2_TxDoorSta1[6],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta1[7],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4								
						
							//sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_190_DoorSta1);	//����PDO
							break;
						
						case 0x08:
							memcpy(&can2_TxDoorSta2[6],(u8*)&SdFrmaeData.DoorSdData+3,1);	//��ַ+3
							memcpy(&can2_TxDoorSta2[7],(u8*)&SdFrmaeData.DoorSdData+4,1);	//�׵�ַ+4								

							break;
					}
					
					if(SdFrmaeData.DoorSdData.Address == 0x08)    //8���Ŷ�ͨ�����ڷ���
					{
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TPDO_190_DoorSta1].last_message.cob_id = 0;	
						sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_190_DoorSta1);		//����PDO				
						/* force emission of PDO by artificially changing last emitted */	
						MFDCAN2Slave_Data.PDO_status[TPDO_191_DoorSta2].last_message.cob_id = 0;						
						sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_191_DoorSta2);

						MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0; //whh 2016-11-23
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO						
						printf("send DOOR DATA!\r\n");						
					}
				}
			}			
		}
	}
}


/*�յ�״̬��ѯ����*/
void AccStateQuery_Take(void *p_arg)			//�յ�Э��SDR����������Ԥ���ֽ�û�м���  WHH
{
	OS_ERR err;
	 	
	/*1. ���Ϳյ���SDR���� ��������*/
	static uint8_t TimeFlagCount=0;			//����ʱ�� ����������
	ACCSdrText	Sdrtext;
	
	/*2.�յ�����SD���� ��������*/
	ACCSdText SdTextData;
	static u8 TxErrFlagCount=0;
	s8 resvalue; //���ܺ�������ֵ
	
	while(1)
	{
		/*�ȴ�����SDR�¼���־*/
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&ACCEventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		
		if(err == OS_ERR_NONE)	//��ʾû�д���
		{
			if(can2_RxAccTempTimBit[0] &0x01)	//��ʾ����ʱ��
			{	
				TimeFlagCount++;
				if(TimeFlagCount >5)	//��ʾ5��
				{
					TimeFlagCount = 0;	//��0
					can2_RxAccTempTimBit[0] &= 0xFE;	//���һλ��0
				}
			}			
			
			memset(&Sdrtext,0,sizeof(ACCSdrText));	//��0
			
			Sdrtext.Address = ACC_ADDRES;
			Sdrtext.Sub_Address = ACC_SUB_ADDRES;
			Sdrtext.Command = ACC_COMMAND;
			
			Sdrtext.SetClock = can2_RxAccTempTimBit[0]&0x01;		//����ʱ��
			Sdrtext.TargetTempVaild = (can2_RxAccTempTimBit[0]>>1)&0x01;  //Ŀ���¶���Ч
			
			Sdrtext.Year = can2_RxTime[0];
			Sdrtext.Month = can2_RxTime[1];
			Sdrtext.Day = can2_RxTime[2];
			Sdrtext.Hour = can2_RxTime[3];
			Sdrtext.Minute = can2_RxTime[4];
			Sdrtext.Second =can2_RxTime[5];

			Sdrtext.NetTargetTemp = can2_RxAccTempTimBit[1]; //����Ŀ���¶�Ϊ10����
			
			SendACCSdr(&Sdrtext);	//�����ݷ��뵽����2�ķ��ͻ�����������
			
			
			/*2.���ܿյ�״̬����*/
			{	
				resvalue = GetACCSd(USART2_RS485_RxBuffer,&SdTextData);
				
				if(resvalue !=0)	//����ֵ�д���
				{
					TxErrFlagCount++;
					
					if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
					{
						can2_TxMFD1SubFault[1] |= 0x80;		//�յ�����λ��1
						
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
						
						debugprintf("ACC	Err!,va=%d!\r\n",resvalue);
						TxErrFlagCount =0;	//���
					}					
				}
				
				else		//������copy�������ֵ䣬�����͵�can������
				{
					debugprintf("Acc ok!\r\n");
					
					/*���Խ��ܵ���������*/
					//					{
					//						int i;
					//							printf("u2=%d\r\n",USART2_RS485_RxLength);
					//						printf("ԭʼ���ݣ�");		
					//						for ( i = 0; i < USART2_RS485_RxLength; i++)
					//						{	
					//							printf("%02x",USART2_RS485_RxBuffer[i]);
					//						}
					//					}					
										
					TxErrFlagCount =0;	//���			

					can2_TxMFD1SubFault[1] &= 0x7F;		//�յ�����λ ��0
					/* force emission of PDO by artificially changing last emitted */
					MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
					sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
					
					memcpy(&can2_TxAccSta,(u8*)&SdTextData+3,8);		//��ַ+3
					memcpy(&can2_TxAccSta2,(u8*)&SdTextData+11,1);	//�׵�ַ+11							

					/* force emission of PDO by artificially changing last emitted */
					MFDCAN2Slave_Data.PDO_status[TPDO_192_ACCSta1].last_message.cob_id = 0;
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_192_ACCSta1);		//����PDO
					/* force emission of PDO by artificially changing last emitted */
					MFDCAN2Slave_Data.PDO_status[TPDO_1AA_ACCSta2].last_message.cob_id = 0;				
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_1AA_ACCSta2);
				}
			}
				
		}
		
		else
		{
			printf("ACC EventFlags Err!\r\n");
		}
	}		
}


/*
����һ��ȫ����ţ������ڶ����ֵ���   ����ʾ��ǰ�Ƿ���Ҫ����TDR
*/
bool 	BCUTxTdrFlag = false; //�Ƿ���Ҫ����TDR 0,����Ҫ�� 1 ��Ҫ
bool 	BCUClearStatDataFlag	= false;
bool	BCUSetTimeFlag = false;
	
/*�ƶ�״̬��ѯ����*/
void BcuStateQuery_Take(void *p_arg)
{
	OS_ERR err;
	static u8  TimeFlagCount=0;			//ʱ�����ñ�־����
	static u8  ClearStaFlagCount=0;	//���״̬���ݱ�־ ����λ
	//static u8  CallCount = 0;			//���ü���ֵ
	static bool	SendSdrOrTdrFlag =true;	//����SDR ���� TDR ��־λ
	//u8 CallOddEvenCount=0; 				//�������õ���ż���� (ż����ʱ��һֱ����SDR,ֻ�������Ż��ж��Ƿ���TDR��TCR�����˳�)
	
	BcuSdrText 	SdrText ;
	BcuTdrText 	TdrText ;
	static u8 BlockNo=0;
	
	//���徲̬����������Ҫ�ط�һ�ε�����
	//�鿴����ͼ Ӧ��Ϊ�����ܵ�NACKʱ��������һ��Sdr ����Tdr
	/*static*/ 	BcuSdrText 	OldSdrText;
	/*static*/ 	BcuTdrText 	OldTdrText;
	
	/*�������ݱ���*/
	static u8 TxErrFlagCount=0;
	//static bool GetNackFlag = false; 	//false ��ʾû�л��Nack��Ture ��ʾ���Nack���´μ��������ϴε����ݣ����޷���һ�Σ�
	s8 resvalue;
	u8 IsNoACK = 0;
	
	BcuSdFrame SdDataFrame;	//����sd����	
	BcuTdFrame TdDataFrame;	//����Td����	
	
	memset(&SdrText,0,sizeof(BcuSdrText));	
	memset(&TdrText,0,sizeof(BcuTdrText));
	
	while(1)
	{	
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&BCUEventFlags,
							 (OS_FLAGS	  )TxSDROrTDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,												//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		
		if(err == OS_ERR_NONE)	//��ʾû�д���
		{
			if(BCUTxTdrFlag == true)		//��ʾ��Ҫ����Tdr
			{
				if(SendSdrOrTdrFlag ==true)		//�ȷ���SDR
				{
					/*1. ����BCU��SDR����*/
					//memset(&SdrText,0,sizeof(BcuSdrText));
					
					//SendSdrOrTdrFlag = false;		//�´ν�����TDR
					
					SdrText.Command = BCU_SDR_COMMAND;
					
					/*BCD��*/
					SdrText.Year = DecToBcd(can2_RxTime[0]);
					SdrText.Month = DecToBcd(can2_RxTime[1]);
					SdrText.Day = DecToBcd(can2_RxTime[2]);
					SdrText.Hour = DecToBcd(can2_RxTime[3]);
					SdrText.Minute = DecToBcd(can2_RxTime[4]);
					SdrText.Second = DecToBcd(can2_RxTime[5]);
					
					SdrText.BrakeRise = can2_RxBCUCmd[0];		//�ƶ�������ָ�0xFF��Ч
					SdrText.Reserve2 = 0;
					SdrText.Reserve3 = 0;
					
					//�������λ
					if(BCUClearStatDataFlag == true)
					{
						ClearStaFlagCount++;			//���״̬ ��־��1
						SdrText.BCU_TraceDataClea = 1;	
						
						if(ClearStaFlagCount>=5)
						{
							ClearStaFlagCount = 0;  //����
							BCUClearStatDataFlag = false ; 	//���ڷ����������λ
						}
					}
					else
					{
						SdrText.BCU_TraceDataClea = 0;
					}
					
					//ʱ����Чλ
					if(BCUSetTimeFlag == true)
					{
						TimeFlagCount++;		//ʱ�����ü����־λ
						SdrText.BCU_TimeSet = 1;	
						
						if(TimeFlagCount>=5)		
						{
							TimeFlagCount = 0;	//ʱ�����ñ�־��������
							BCUSetTimeFlag = false;
						}
					}
					else
					{
						SdrText.BCU_TimeSet = 0;
					}
					
					//1.���Ƚ�����copy����̬������
					memcpy(&OldSdrText,&SdrText,sizeof(BcuSdrText));
					
					SendBcuSdrOrTdr(&SdrText,SEND_SDR);				//����SDR
				}
				
				else //�ڷ���TDR
				{
					//memset(&TdrText,0,sizeof(BcuTdrText));
					//SendSdrOrTdrFlag =true;  //�´ν�����SDR
					
					TdrText.Command = BCU_TDR_COMMAND;
					TdrText.BCU_TraceDataNoRq = can2_RxBCUCmd[3];
					TdrText.BCU_TraceDataBlackNoRq = BlockNo;
					BlockNo ++;
					
					//1.���Ƚ�����copy����̬������
					memcpy(&OldTdrText,&TdrText,sizeof(BcuTdrText));
					
					SendBcuSdrOrTdr(&TdrText,SEND_TDR);		//����TDR
					
					//					if(BlockNo >=40)
					//					{
					//						BlockNo = 0;		//���
					//						BCUTxTdrFlag = false;  //��� �����ڷ���TDR
					//					}
				}
			}
			
			else		//����SDR
			{
					/*1. ����BCU��SDR����*/
					//memset(&SdrText,0,sizeof(BcuSdrText));
					
					SendSdrOrTdrFlag = true;		//��������Ҫ����TDR��ʱ�����ȷ���SDR
				
					SdrText.Command = BCU_SDR_COMMAND;
					
					/*BCD��*/
					SdrText.Year = DecToBcd(can2_RxTime[0]);
					SdrText.Month = DecToBcd(can2_RxTime[1]);
					SdrText.Day = DecToBcd(can2_RxTime[2]);
					SdrText.Hour = DecToBcd(can2_RxTime[3]);
					SdrText.Minute = DecToBcd(can2_RxTime[4]);
					SdrText.Second = DecToBcd(can2_RxTime[5]);
					
					SdrText.BrakeRise = can2_RxBCUCmd[0];		//�ƶ�������ָ�0xFF��Ч
	        SdrText.Reserve2 = 0;
					SdrText.Reserve3 = 0;
				
					//�������λ
					if(BCUClearStatDataFlag == true)
					{
						ClearStaFlagCount++;			//���״̬ ��־��1
						SdrText.BCU_TraceDataClea = 1;	
						
						if(ClearStaFlagCount>=5)
						{
							ClearStaFlagCount = 0;  //����
							BCUClearStatDataFlag = false ; 	//���ڷ����������λ
						}
					}
					else
					{
						SdrText.BCU_TraceDataClea = 0;
					}
					
					//ʱ����Чλ
					if(BCUSetTimeFlag == true)
					{
						TimeFlagCount++;		//ʱ�����ü����־λ
						SdrText.BCU_TimeSet = 1;	
						
						if(TimeFlagCount>=5)		
						{
							TimeFlagCount = 0;	//ʱ�����ñ�־��������
							BCUSetTimeFlag = false;
						}
					}
					else
					{
						SdrText.BCU_TimeSet = 0;
					}
					
					//1.���Ƚ�����copy����̬������
					memcpy(&OldSdrText,&SdrText,sizeof(BcuSdrText));
					
					SendBcuSdrOrTdr(&SdrText,SEND_SDR);				//����SDR
					
					printf("buc 11\r\n");
			}
	
			
			/*2.��ʼ��������*/		//WHH 2016-10-27 ��������Ҫ�޸ģ���鿴BCUЭ���ֲ��޸�
			{
				if(BCUTxTdrFlag == true)		//��ʾ��Ҫ����Td
				{
					if(SendSdrOrTdrFlag ==true)		//�Ƚ���Sd
					{
						SendSdrOrTdrFlag  = false; //�´ν�����Tdr
						
						resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM1);			//��һ�λ�ȡ����
			
						if(resvalue !=0)	//��ʾ�������ݴ���
						{
							TxErrFlagCount++;
							
							//��ʾ����5�ζ�ͨ���쳣 ������BCUͨ���쳣	
							if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
							{
								can2_TxMFD1SubFault[1] |= 0x40;  //��1��ʾ���� 
								
								/* force emission of PDO by artificially changing last emitted */
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
								
								debugprintf("BCU�������,va=%d!\r\n",resvalue);
								TxErrFlagCount =0;	//���
							}	

							/*WHH 2016-11-3 ���ʣ�����*/
							if(resvalue == BCU_NACKError)	//��ʾ���NACK���������·���һ������
							{
								SendBcuSdrOrTdr(&OldSdrText,SEND_SDR);				//���·���SDR
								
								resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM1);			//��һ�λ�ȡ����
								
								if(resvalue !=0)	//��ʾ�д�����
								{
									resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM2);  //�ڶ��λ�ȡ����
									if(resvalue !=0)	//������
									{
									
									}
									else
									{
										//0.���ͨ���쳣������־
										TxErrFlagCount = 0 ; 	
										
										can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������
	
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
										sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
										
										//1.����ACK,��ʾ����Bcu���ݳɹ�
										IsNoACK = BCU_ACK;
										MTDMFD_CL_Send_Data(&IsNoACK,1);

										//2.������copy�������ֵ��� ���͵�can����
										memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
										memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
										memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);
										
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;	
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;											
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);									
									}
								}
								
								else	//��ʾû�д���
								{
									//0.���ͨ���쳣������־
									TxErrFlagCount = 0 ; 	
									
									can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������

									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
									sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
									
									//1.����ACK,��ʾ����Bcu���ݳɹ�
									IsNoACK = BCU_ACK;
									MTDMFD_CL_Send_Data(&IsNoACK,1);

									//2.������copy�������ֵ��� ���͵�can����
									memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
									memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
									memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);

									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;									
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;																		
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
								}
									
							}
							
							else if( (resvalue == BCU_FrameError) ||(resvalue ==BCU_BCCError))		//Э��ͼ5��.��ʾ������Ϣ����
							{
								resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM2);  //�ڶ��λ�ȡ����
								
								if(resvalue!=0)		//��ʾ�д�����
								{
									//�д��������һ������
								}
								else		//��ʾ������Ϣ��ȷ
								{
									//0.���ͨ���쳣������־
									TxErrFlagCount = 0 ; 	
									
									can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������

									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
									sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
									
									//1.����ACK,��ʾ����Bcu���ݳɹ�
									IsNoACK = BCU_ACK;
									MTDMFD_CL_Send_Data(&IsNoACK,1);

									//2.������copy�������ֵ��� ���͵�can����
									memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
									memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
									memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);

									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
								}
							}

							else		//��ʾ�������� ����ֱ�ӷ��أ�ִ���´εķ�������
							{
								
							}
						}
						
						else		//��ʾ����������ȷ
						{
							//0.���ͨ���쳣������־
							TxErrFlagCount = 0 ; 	
							
							can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������

							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
							
							//1.����ACK,��ʾ����Bcu���ݳɹ�
							IsNoACK = BCU_ACK;
							MTDMFD_CL_Send_Data(&IsNoACK,1);

							//2.������copy�������ֵ��� ���͵�can����
							memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
							memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
							memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);
							
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);			
						}

						
					}
					
					else		//����Td
					{
						SendSdrOrTdrFlag  = true; //�´ν�����Sdr	

						if(BlockNo >=40)
						{
							BlockNo = 0;		//���
							BCUTxTdrFlag = false;  //��� �����ڷ���TDR
						}						
						
						resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&TdDataFrame,GET_TD,GET_NUM1);			//��һ�λ�ȡ����
			
						if(resvalue !=0)	//��ʾ�������ݴ���
						{
							TxErrFlagCount++;
							
							//��ʾ����5�ζ�ͨ���쳣 ������BCUͨ���쳣	
							if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
							{
								can2_TxMFD1SubFault[1] |= 0x40;  //��1��ʾ���� 
								
								/* force emission of PDO by artificially changing last emitted */
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
								
								debugprintf("BCU�������,va=%d!\r\n",resvalue);
								TxErrFlagCount =0;	//���
							}	

							if(resvalue == BCU_NACKError)	//��ʾ���NACK����
							{
								//GetNackFlag = true;
								SendBcuSdrOrTdr(&OldTdrText,SEND_TDR);				//����TDR
								
								resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&TdDataFrame,GET_TD,GET_NUM1);			//��һ�λ�ȡ����
								
								if(resvalue !=0)	//��ʾ�д�����
								{
									resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&TdDataFrame,GET_TD,GET_NUM2);  //�ڶ��λ�ȡ����
									if(resvalue !=0)	//������
									{
									
									}
									else
									{
										//0.���ͨ���쳣������־
										TxErrFlagCount = 0 ; 	
										
										can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������
				
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
										sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
										
										//1.����ACK,��ʾ����Bcu���ݳɹ�
										IsNoACK = BCU_ACK;
										MTDMFD_CL_Send_Data(&IsNoACK,1);

										//2.������copy�������ֵ��� ���͵�can����
										memcpy(can2_TxBCUSta1,(u8*)&TdDataFrame.BcuTdData+1,8);		//����8�ֽ�����
										memcpy(can2_TxBCUSta2,(u8*)&TdDataFrame.BcuTdData+9,8);
										memcpy(can2_TxBCUSta3,(u8*)&TdDataFrame.BcuTdData+17,8);
										
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);							
									}
								}
								
								else	//��ʾû�д���
								{
									//0.���ͨ���쳣������־
									TxErrFlagCount = 0 ; 	
									
									can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������
			
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
									sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
									
									//1.����ACK,��ʾ����Bcu���ݳɹ�
									IsNoACK = BCU_ACK;
									MTDMFD_CL_Send_Data(&IsNoACK,1);

									//2.������copy�������ֵ��� ���͵�can����
									memcpy(can2_TxBCUSta1,(u8*)&TdDataFrame.BcuTdData+1,8);		//����8�ֽ�����
									memcpy(can2_TxBCUSta2,(u8*)&TdDataFrame.BcuTdData+9,8);
									memcpy(can2_TxBCUSta3,(u8*)&TdDataFrame.BcuTdData+17,8);
									
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
								}								
							}
							
							else if( (resvalue == BCU_FrameError) ||(resvalue ==BCU_BCCError))		//Э��ͼ5��.��ʾ������Ϣ����
							{
								resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&TdDataFrame,GET_TD,GET_NUM2);  //�ڶ��λ�ȡ����
								
								if(resvalue!=0)		//��ʾ�д�����
								{
									//�д��������һ������
								}
								else		//��ʾ������Ϣ��ȷ
								{
									//0.���ͨ���쳣������־
									TxErrFlagCount = 0 ; 	
									
									can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������

									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
									sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
									
									//1.����ACK,��ʾ����Bcu���ݳɹ�
									IsNoACK = BCU_ACK;
									MTDMFD_CL_Send_Data(&IsNoACK,1);

									//2.������copy�������ֵ��� ���͵�can����
									memcpy(can2_TxBCUSta1,(u8*)&TdDataFrame.BcuTdData+1,8);		//����8�ֽ�����
									memcpy(can2_TxBCUSta2,(u8*)&TdDataFrame.BcuTdData+9,8);
									memcpy(can2_TxBCUSta3,(u8*)&TdDataFrame.BcuTdData+17,8);
									
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
								}
							}

							else		//��ʾ�������� ����ֱ�ӷ��أ�ִ���´εķ�������
							{
								
							}
						}
						
						else		//��ʾ����������ȷ
						{
							//0.���ͨ���쳣������־
							TxErrFlagCount = 0 ; 	
							
							can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������

							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
							
							//1.����ACK,��ʾ����Bcu���ݳɹ�
							IsNoACK = BCU_ACK;
							MTDMFD_CL_Send_Data(&IsNoACK,1);

							//2.������copy�������ֵ��� ���͵�can����
							memcpy(can2_TxBCUSta1,(u8*)&TdDataFrame.BcuTdData+1,8);		//����8�ֽ�����
							memcpy(can2_TxBCUSta2,(u8*)&TdDataFrame.BcuTdData+9,8);
							memcpy(can2_TxBCUSta3,(u8*)&TdDataFrame.BcuTdData+17,8);
							
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);		
						}

					}
				}
				
				
				else	//��ʾֱ�ӽ���Sd
				{
					printf("buc 22\r\n");
					//					{
					//						int i;
					//						printf("u4=%d\r\n",USART4_CL_RxLength);
					//						printf("ԭʼ���ݣ�");		
					//						for (i = 0; i < USART4_CL_RxLength; i++)
					//						{	
					//							printf("%02x",USART4_CL_RxBuffer[i]);
					//						}
					//						printf("\r\n\r\n");
					//					}
					
						resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM1);			//��һ�λ�ȡ����
			
						if(resvalue !=0)	//��ʾ�������ݴ���
						{
							printf("buc 33,res=%d\r\n",resvalue);
							TxErrFlagCount++;
							
							//��ʾ����5�ζ�ͨ���쳣 ������BCUͨ���쳣	
							if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
							{
								can2_TxMFD1SubFault[1] |= 0x40;  //��1��ʾ���� 
								
								/* force emission of PDO by artificially changing last emitted */
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
								
								debugprintf("BCU Sdr�������,va=%d!\r\n",resvalue);
								TxErrFlagCount =0;	//���
							}	

							/*WHH 2016-11-3 ���ʣ�����*/
							if(resvalue == BCU_NACKError)	//��ʾ���NACK���������·���һ������
							{
								SendBcuSdrOrTdr(&OldSdrText,SEND_SDR);				//���·���SDR
								
								resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM1);			//��һ�λ�ȡ����
								
								if(resvalue !=0)	//��ʾ�д�����
								{
									resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM2);  //�ڶ��λ�ȡ����
									if(resvalue !=0)	//������
									{
									
									}
									else
									{
										//0.���ͨ���쳣������־
										TxErrFlagCount = 0 ; 	
										
										can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������

										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
										sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO										
										
										//1.����ACK,��ʾ����Bcu���ݳɹ�
										IsNoACK = BCU_ACK;
										MTDMFD_CL_Send_Data(&IsNoACK,1);

										//2.������copy�������ֵ��� ���͵�can����
										memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
										memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
										memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);
										
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
										/* force emission of PDO by artificially changing last emitted */
										MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
										sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);							
									}
								}
								
								else	//��ʾû�д���
								{
									//0.���ͨ���쳣������־
									TxErrFlagCount = 0 ; 	
									
									can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������
									
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
									sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO		
									
									//1.����ACK,��ʾ����Bcu���ݳɹ�
									IsNoACK = BCU_ACK;
									MTDMFD_CL_Send_Data(&IsNoACK,1);

									//2.������copy�������ֵ��� ���͵�can����
									memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
									memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
									memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);
									
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
								}
									
							}
							
							else if( (resvalue == BCU_FrameError) ||(resvalue ==BCU_BCCError))		//Э��ͼ5��.��ʾ������Ϣ����
							{
								resvalue = GetBcuSdOrTd(USART4_CL_RxBuffer,&SdDataFrame,GET_SD,GET_NUM2);  //�ڶ��λ�ȡ����
								
								if(resvalue!=0)		//��ʾ�д�����
								{
									//�д��������һ������
								}
								else		//��ʾ������Ϣ��ȷ
								{
									//0.���ͨ���쳣������־
									TxErrFlagCount = 0 ; 	
									
									can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������
									
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
									sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
									
									//1.����ACK,��ʾ����Bcu���ݳɹ�
									IsNoACK = BCU_ACK;
									MTDMFD_CL_Send_Data(&IsNoACK,1);

									//2.������copy�������ֵ��� ���͵�can����
									memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
									memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
									memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);
									
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
									/* force emission of PDO by artificially changing last emitted */
									MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
									sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
								}
							}

							else		//��ʾ�������� ����ֱ�ӷ��أ�ִ���´εķ�������
							{
								
							}
						}
						
						else		//��ʾ����������ȷ
						{
							printf("buc 44\r\n");
							
							{
									int i;
										printf("u4=%d\r\n",USART4_CL_RxLength);
									printf("ԭʼ���ݣ�");		
									for ( i = 0; i < USART4_CL_RxLength; i++)
									{	
										printf("%02x",USART4_CL_RxBuffer[i]);
									}
							}
							//0.���ͨ���쳣������־
							TxErrFlagCount = 0 ; 	
							can2_TxMFD1SubFault[1] &= 0xBF;		//�յ�����λ ��0 ��ʾͨ������
							
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO
							
							//1.����ACK,��ʾ����Bcu���ݳɹ�
							IsNoACK = BCU_ACK;
							MTDMFD_CL_Send_Data(&IsNoACK,1);

							//2.������copy�������ֵ��� ���͵�can����
							memcpy(can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//����8�ֽ�����
							memcpy(can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);
							memcpy(can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);
							
							/*whh 216-11-12 sendOnePdoevent ռ�ò���ʱ�䣬������˶�ʱ���;Ͳ�����������÷���*/
	//							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
	//							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
	//							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);		

							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_193_BCUSta1].last_message.cob_id = 0;										
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_194_BCUSta2].last_message.cob_id = 0;										
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_195_BCUSta3].last_message.cob_id = 0;									
							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);							
						}
					
				}
				
				
				//				if(g_BCURxSdOrTdFlag == BCU_RxSD)		//��ʾΪSD����
				//				{
				//						
				//					
				//						g_BCURxSdOrTdFlag =0; //������ܱ�־
				//						resvalue = GetBcuSdOrTd(USART2_RS485_RxBuffer,&SdDataFrame);
				//						
				//						if(resvalue !=0)	//����ֵ�д���
				//						{
				//							TxErrFlagCount++;
				//		//							//����NACK,��ʾ����Bcu����ʧ��
				//		//							IsNoACK = BCU_NACK;
				//		//							MTDMFD_CL_Send_Data(&IsNoACK,1);		
				//							
				//							if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
				//							{
				//								//can2_TxSubDevFault_BCU = 1;
				//								can2_TxSubDevFault[1] = can2_TxSubDevFault[1]|0x40;		//1
				//								
				//								/* force emission of PDO by artificially changing last emitted */
				//								MFDCAN2Slave_Data.PDO_status[TPDO_1A9_SubDecFault].last_message.cob_id = 0;
				//								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1A9_SubDecFault);		//����PDO
				//								//debugprintf("BCU2�������,va=%d!\r\n",resvalue);
				//								TxErrFlagCount =0;	//���
				//								//can2_TxSubDevFault_BCU = 0;//���BCU����
				//							}					
				//						}
				//						
				//						else		//������copy�������ֵ䣬�����͵�can������
				//						{
				//							debugprintf("Bcu sd ok!\r\n");
				//							can2_TxSubDevFault[1] = can2_TxSubDevFault[1]&0xBF;		//����
				//							/* force emission of PDO by artificially changing last emitted */
				//							MFDCAN2Slave_Data.PDO_status[TPDO_1A9_SubDecFault].last_message.cob_id = 0;
				//							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1A9_SubDecFault);		//����PDO
				//							
				//							memcpy(&can2_TxBCUSta1,(u8*)&SdDataFrame.BcuSdData+1,8);		//��ַ+1
				//							memcpy(&can2_TxBCUSta2,(u8*)&SdDataFrame.BcuSdData+9,8);	//�׵�ַ+9		
				//							memcpy(&can2_TxBCUSta3,(u8*)&SdDataFrame.BcuSdData+17,8);	//�׵�ַ+17	
				//						
				//							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_193_BCUSta1);		//����PDO
				//							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_194_BCUSta2);
				//							sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_195_BCUSta3);
				//						}
				//				}
				//				else if(g_BCURxSdOrTdFlag == BCU_RxTD)		//��ʾΪTD����
				//				{
				//				
				//				}
				//			}	
				//		}
				//		
				//		else
				//		{
				//			printf("BCUerr=%d\r\n",err);
				//		}
			}
		}
	}
}



/*��ѹ��״̬��ѯ����*/
void AcpStateQuery_Take(void *p_arg)
{
	OS_ERR err;
	
	//static uint8_t TimeFlagCount=0;
	/*1. ���Ϳյ���SDR����*/
	ACPSdrFrame SdrFrame;
	
	/*2.SD*/
	ACPSdFrame  SdFrame;
	static u8 TxErrFlagCount=0;
	s8 resvalue;
	
	while(1)
	{
		/*1.�ȴ�����SDR�¼���־*/
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&ACPEventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_NONE)	//��ʾû�д���
		{
			SdrFrame.StartAddressH8 = 0x00;
			SdrFrame.StartAddressL8 = 0x01;
			SdrFrame.RegisterNumberH8 = 0x00;
			SdrFrame.RegisterNumberL8 = 0x03;
			
			SendACPSdr(&SdrFrame);
	
			//	/*2 ��������¼���־λ*/
			//			/*2.���� ��ѹ�� ������������¼���־*/
			//			OSFlagPost((OS_FLAG_GRP*)&ACPEventFlags,
			//						 (OS_FLAGS	  )TxSDREndFlag,				//����SDR��ɱ�־
			//						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
			//						 (OS_ERR*	    )&err);			
			
			//			/*�ȴ�����SDR����¼���־*/
			//			//�ȴ��¼���־��
			//			OSFlagPend((OS_FLAG_GRP*)&ACPEventFlags,
			//								 (OS_FLAGS	  )TxSDREndFlag,			//�ȴ� ����SDR��ɱ�־λ
			//								 (OS_TICK     )0,								
			//								 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
			//								 (CPU_TS*     )0,
			//								 (OS_ERR*	    )&err);	
		
			//			if(err == OS_ERR_NONE)	//��ʾû�д���
			//			{
			//				OSFlagPend((OS_FLAG_GRP*)&ACPEventFlags,
			//									 (OS_FLAGS	  )RxStartFlag,			//�ȴ����ܿ�ʼ��־λ
			//									 (OS_TICK     )20,							//�ȴ�20ms
			//									 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
			//									 (CPU_TS*     )0,
			//									 (OS_ERR*	    )&err);				
				
			//				if(err == OS_ERR_TIMEOUT)		//��ʾ�ȴ���ʱ
			//				{
			//					//û�н��յ����ݣ����ʾ֡�������
			//					TxErrFlagCount++;
			//					
			//					if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
			//					{
			//						
			//		//					/* force emission of PDO by artificially changing last emitted */
			//		//					MFDCAN2Slave_Data.PDO_status[TPDO_1A9_SubDecFault].last_message.cob_id = 0;
			//		//					sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1A9_SubDecFault);		//����PDO
			//		//					debugprintf("Acp1�������!\r\n");
			//						TxErrFlagCount =0;	//���
			//					}
			//				}
			//				
			//				else if(err == OS_ERR_NONE)
			//				{
			//					OSFlagPend((OS_FLAG_GRP*)&ACPEventFlags,
			//										 (OS_FLAGS	  )RxEndFlag,			//�ȴ� ������ɱ�־λ
			//										 (OS_TICK     )0,						
			//										 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
			//										 (CPU_TS*     )0,
			//										 (OS_ERR*	    )&err);		
			
			/*2.����SD����*/
			{			
				resvalue = GetACPSd(USART3_RS485_RxBuffer,&SdFrame);
				
				if(resvalue !=0)	//����ֵ�д���
				{
					TxErrFlagCount++;
					
					if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
					{											
						can2_TxMFD3SubFault[0] |= 0x04;  //��ѹ����Ӧ��λ����1
						
						MFDCAN2Slave_Data.PDO_status[TPDO_1AE_SubDecFault].last_message.cob_id = 0;
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AE_SubDecFault);		//����PDO
						debugprintf("ACP �����쳣\r\n");
						
						TxErrFlagCount =0;	//���
					}					
				}
				
				else		//������copy�������ֵ䣬�����͵�can������
				{
					debugprintf("Acp ok!\r\n");
					
					can2_TxMFD3SubFault[0] &= 0xFB;  //��ѹ����Ӧ��λ��  ����
					MFDCAN2Slave_Data.PDO_status[TPDO_1AE_SubDecFault].last_message.cob_id = 0;
					sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AE_SubDecFault);		//����PDO 
					
					/**/  //WHH ��Ҫ�����ö����ֵ䣬�����ݷ�����ACP��Ȼ���ڷ��͵�����
					
					//2016-10-27  δ���
				}
			}			
		}
		
		else
		{
			printf("ACP EventFlags Err!\r\n");
		}
	}		
}


/*ATC���źţ�״̬��ѯ����*/
void ATCStateQuery_Take(void *p_arg)
{
	OS_ERR err;
	/*1.������SDR����*/
	ATCSdrText ATCSdrData;
	static u8 TimeFlagCount=0;
	
	/*SD*/
	ATCSdFrame SdFrmaeData;
	static u8 TxErrFlagCount=0;
	s8 resvalue;
	
	while(1)
	{
		/*�ȴ�����SDR�¼���־*/
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&ATCEventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		
		if(err == OS_ERR_NONE)
		{
			
			//ʱ����Чλ
			if(can2_RxTime[1] >0) //�·ݴ���0 ����ʾʱ����Ч
			{
				ATCSdrData.ATC_TimeVaild = 1;	
			}
			else
			{
				ATCSdrData.ATC_TimeVaild = 0;			
			}		
				
			//			if(can2_RxATCSdr1[0] == 1)		//��ʾ����ʱ��
			//			{	
			//				TimeFlagCount++;
			//				if(TimeFlagCount >5)	//��ʾ5��
			//				{
			//					TimeFlagCount = 0;
			//					can2_RxATCSdr1[0] = 0;
			//				}
			//			}
			
			ATCSdrData.ATC_TextCodeH8 = ATC_SDR_TEXTCODEH8;
			ATCSdrData.ATC_TextCodeL8 = ATC_SDR_TEXTCODEL8;
			ATCSdrData.ATC_TotalweightH8 = can2_RxATCSdr2[0];
			ATCSdrData.ATC_TotalweightL8 = can2_RxATCSdr2[1];
			
			/*VVVF1����*/ 
			ATCSdrData.ATC_No1VVVF1Vaild = (can2_RxATCSdr1[0]>>7)&0x01;
			ATCSdrData.ATC_No3VVVF1Vaild = (can2_RxATCSdr1[0]>>6)&0x01;
			ATCSdrData.ATC_No6VVVF1Vaild = (can2_RxATCSdr1[0]>>5)&0x01;
			
			/*VVVF1������Ч*/
			ATCSdrData.ATC_No1VVVF1EleBrake = (can2_RxATCSdr1[0]>>4)&0x01;
			ATCSdrData.ATC_No3VVVF1EleBrake = (can2_RxATCSdr1[0]>>3)&0x01;
			ATCSdrData.ATC_No6VVVF1EleBrake = (can2_RxATCSdr1[0]>>2)&0x01;			
			
			/*BCU ����*/
			ATCSdrData.ATC_BCU1vaild = (can2_RxATCSdr1[1]>>7)&0x01;
			ATCSdrData.ATC_BCU2vaild = (can2_RxATCSdr1[1]>>6)&0x01;
			ATCSdrData.ATC_BCU3vaild = (can2_RxATCSdr1[1]>>5)&0x01;					
			ATCSdrData.ATC_BCU4vaild = (can2_RxATCSdr1[1]>>4)&0x01;
			ATCSdrData.ATC_BCU5vaild = (can2_RxATCSdr1[1]>>3)&0x01;
			ATCSdrData.ATC_BCU6vaild = (can2_RxATCSdr1[1]>>2)&0x01;	

			/*����*/
			ATCSdrData.ATC_Slide1 = (can2_RxATCSdr1[2]>>7)&0x01;
			ATCSdrData.ATC_Slide6 = (can2_RxATCSdr1[2]>>6)&0x01;	
			
			ATCSdrData.ATC_No1VVVF1SlideStatu = (can2_RxATCSdr1[2]>>5)&0x01;
			ATCSdrData.ATC_No6VVVF1SlideStatu = (can2_RxATCSdr1[2]>>4)&0x01;			

			ATCSdrData.ATC_TrainNumber	= can2_RxATCSdr1[3]; //BCD��
			//ATCSdrData.ATC_TimeVaild = 0;	
			
			/*ʱ��*/
			ATCSdrData.ATC_Year = DecToBcd(0);
			ATCSdrData.ATC_Month = DecToBcd(0);
			ATCSdrData.ATC_Day = DecToBcd(0);					
			ATCSdrData.ATC_Hour = DecToBcd(0);
			ATCSdrData.ATC_Minute = DecToBcd(0);
			ATCSdrData.ATC_Second = DecToBcd(0);				
			
			ATCSdrData.ATC_TBFroce1 = can2_RxATCSdr1[4];	
			ATCSdrData.ATC_TBFroce2 = can2_RxATCSdr1[5];	
			
			ATCSdrData.ATC_DoorStatu = can2_RxATCSdr2[3];	
			ATCSdrData.ATC_LoadRate = can2_RxATCSdr2[4];	
			
			/*����Sdr*/
			SendATCSdr(&ATCSdrData);	//�����ݷ��뵽����3�ķ��ͻ�����������
			
			
			/*2.��������*/
			{
				resvalue = GetATCSd(USART3_RS485_RxBuffer,&SdFrmaeData);
				
				if(resvalue !=0)	//����ֵ�д���
				{
					TxErrFlagCount++;
					
					if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
					{						
						can2_TxMFD2SubFault[0] = can2_TxMFD2SubFault[0]|0x02;		//1
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TPDO_1AD_SubDecFault].last_message.cob_id = 0;
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AD_SubDecFault);		//����PDO  MFD2�忨
						
						printf("ATC ERR res=%d!\r\n",resvalue);
						TxErrFlagCount =0;	//���
					}					
				}
				
				else		//������copy�������ֵ䣬�����͵�can������
				{
					debugprintf("ATC ok!\r\n");
					
					/*���� ATCͨ������*/
					TxErrFlagCount =0;	//���
				
					can2_TxMFD2SubFault[0] = can2_TxMFD2SubFault[0]&0xFD;	//����
					MFDCAN2Slave_Data.PDO_status[TPDO_1AD_SubDecFault].last_message.cob_id = 0;
					sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AD_SubDecFault);		//����PDO  MFD2�忨
					
					/*����ATCʱ��*/
					can2_TxATCTime[0] = SdFrmaeData.ATCSdData.ATC_Year;
					can2_TxATCTime[1] = SdFrmaeData.ATCSdData.ATC_Month;
					can2_TxATCTime[2] = SdFrmaeData.ATCSdData.ATC_Day;
					can2_TxATCTime[3] = SdFrmaeData.ATCSdData.ATC_Hour;
					can2_TxATCTime[4] = SdFrmaeData.ATCSdData.ATC_Minute;
					can2_TxATCTime[5] = SdFrmaeData.ATCSdData.ATC_Second;
					MFDCAN2Slave_Data.PDO_status[TPDO_1AB_ATCTime].last_message.cob_id = 0;
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_1AB_ATCTime);
					
					memcpy(can2_TxATCSta1,(u8*)&SdFrmaeData.ATCSdData+2,7);		//�׵�ַ+2	
					memcpy(can2_TxATCSta2,(u8*)&SdFrmaeData.ATCSdData+9,8);
					memcpy(can2_TxATCSta3,(u8*)&SdFrmaeData.ATCSdData+17,6);	//״̬3��ǰ���ֽ�
					memcpy(&can2_TxATCSta3[6],(u8*)&SdFrmaeData.ATCSdData+33,2);  //״̬3�ĺ����ֽ�
					memcpy(can2_TxATCSta4,(u8*)&SdFrmaeData.ATCSdData+38,4);	
					
					/* force emission of PDO by artificially changing last emitted */
					MFDCAN2Slave_Data.PDO_status[TPDO_196_ATCSta1].last_message.cob_id = 0;		
					MFDCAN2Slave_Data.PDO_status[TPDO_197_ATCSta2].last_message.cob_id = 0;
					MFDCAN2Slave_Data.PDO_status[TPDO_198_ATCSta3].last_message.cob_id = 0;
					MFDCAN2Slave_Data.PDO_status[TPDO_199_ATCSta4].last_message.cob_id = 0;

					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_196_ATCSta1);
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_197_ATCSta2);
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_198_ATCSta3);
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_199_ATCSta4);	
				}
			}			
		}
		else
		{
			printf("PIS EventFlags Err!\r\n");
		}		
	}
}


/*PIS���㲥��״̬��ѯ����*/
void PISStateQuery_Take(void *p_arg)
{
	OS_ERR err;
	/*1.������SDR���� ��������*/
	PISSdrText SdrText;
	static u8 TimeFlagCount=0;
	
	/*2.����SD���� ��������*/
	PISSdFrame SdFrmaeData;
	static u8 TxErrFlagCount=0;
	s8 resvalue;
	
	while(1)
	{
		/*�ȴ�����SDR�¼���־*/
		//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&PISEventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		
		if(err == OS_ERR_NONE)
		{
				//ʱ����Чλ
				if(can2_RxTime[1] >0) //�·ݴ���0 ����ʾʱ����Ч
				{
					can2_RxPISSdr1[0] |= 0x01;  //PIS,ʱ����Чλ
				}
				else
				{
					can2_RxPISSdr1[0] &= 0xFE;  //���
				}			
			
				//			//��ʾ����ʱ��
				//			if(can2_RxPISSdr1[0] == 1)		
				//			{	
				//				TimeFlagCount++;
				//				if(TimeFlagCount >5)	//��ʾ5��
				//				{
				//					TimeFlagCount = 0;
				//					can2_RxPISSdr1[0] = 0;
				//				}
				//			}
			
				
				//λ�õļ���
				switch(ChooseBoard)
				{
					case MC1_MFD2_NODEID:  //1��  λ��=0
						can2_RxPISSdr1[1] &= 0xEF; //���λ��λ
						break;
					     
					case MC2_MFD2_NODEID:  //6��   λ��=1
						can2_RxPISSdr2[1] |= 0x10; //����λ��λ
						break;
					
					default:
						break;
				}
				
			//memset(&SdrText,0,sizeof(PISSdrText));
	
			SdrText.PIS_DataLongth = 0x1A;
			SdrText.PIS_TimeVaild = can2_RxPISSdr1[0];  //ʱ����Чλ
	
			memcpy((u8 *)&SdrText.Byte3Undef+1,&can2_RxPISSdr1[1],1);	 /*byte4*/
	
			/*BCD��*/
			SdrText.Year = DecToBcd(16);
			SdrText.Month = DecToBcd(8);
			SdrText.Day = DecToBcd(11);
			SdrText.Hour = DecToBcd(10);
			SdrText.Minute = DecToBcd(11);
			SdrText.Second = DecToBcd(12);	

			/*�г����*/
			memcpy((u8 *)&SdrText.PIS_TrainNumberH8,can2_RxPISSdr2,2);
				
			/*�г�Ŀ�ĵ����*/	
			memcpy((u8 *)&SdrText.PIS_TrainDesIDH8,&can2_RxPISSdr2[2],6);	
	
			/*����PIS_SDR����*/
			SendPISSdr(&SdrText);
			
			/*2.��������*/
			{
				resvalue = GetPISSd(USART2_RS485_RxBuffer,&SdFrmaeData);
				
				if(resvalue !=0)	//����ֵ�д���
				{
					TxErrFlagCount++;
					printf("res=%d\r\n",resvalue);
					
					if(TxErrFlagCount>=5)		//��ֵ�������ֵ� ��ϵͳ�����ʾλ
					{
						can2_TxMFD2SubFault[0] = can2_TxMFD2SubFault[0]|0x08;		//1
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TPDO_1AD_SubDecFault].last_message.cob_id = 0;
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AD_SubDecFault);		//����PDO  MFD2�忨
						
						printf("PIS�����쳣!\r\n");
						TxErrFlagCount =0;	//���
					}					
				}
				
				else		//������copy�������ֵ䣬�����͵�can������
				{
					debugprintf("PIS ok!\r\n");
					
					/*���� ATCͨ������*/
					TxErrFlagCount =0;	//���
					
					can2_TxMFD2SubFault[0] = can2_TxMFD2SubFault[0]&0xF7;	//����
					MFDCAN2Slave_Data.PDO_status[TPDO_1AD_SubDecFault].last_message.cob_id = 0;
					sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AD_SubDecFault);		//����PDO  MFD2�忨
						
					/*WHH ��Ҫ�����ö����ֵ䣬�����ݷ�����PIS��Ȼ���ڷ��͵�����*/ 
					memcpy(can2_TxPISSta1,(u8*)&SdFrmaeData.PISSdData.PIS_DataLongth+2,8);	//�׵�ַ+2	
					memcpy(can2_TxPISSta2,(u8*)&SdFrmaeData.PISSdData.PIS_DataLongth+10,3);
					
					/* force emission of PDO by artificially changing last emitted */
					MFDCAN2Slave_Data.PDO_status[TPDO_19A_PISSta1].last_message.cob_id = 0;		
					MFDCAN2Slave_Data.PDO_status[TPDO_19B_PISSta2].last_message.cob_id = 0;

					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_19A_PISSta1);
					sendOnePDOevent(&MFDCAN2Slave_Data,TPDO_19B_PISSta2);
				}
			}			
		}
		
		else
		{
			printf("PIS EventFlags Err!\r\n");
		}
	}
}



/*
����һ��ȫ����ţ������ڶ����ֵ���   ����ʾ��ǰ�Ƿ���Ҫ����TDR
*/
bool 	VVVF1TxTdrFlag = false; //�Ƿ���Ҫ����TDR 0,����Ҫ�� 1 ��Ҫ
bool 	VVVF1TxTcrFlag	= false;
//bool	VVVF1SetTimeFlag = false;
//bool  VVVF1TimeEffFlag = false;

/*VVVF1״̬��ѯ����*/
void VVVF1StateQuery_Take(void *p_arg)
{
	OS_ERR err;
	static u8  TimeFlagCount=0;
	static u8  CallCount = 0;							//���ü���ֵ
	u8 CallOddEvenCount=0; 								//�������õ���ż���� (ż����ʱ��һֱ����SDR,ֻ�������Ż��ж��Ƿ���TDR��TCR�����˳�)
	
	VVVFSdrText 	SdrText;
	VVVFTdrText 	TdrText;
	VVVFTcrText   TcrText;
	
	VVVFSdFrame   SdFrameData;
	VVVFTdFrame   TdFrameData;
	
	static u16 BlockNo=0;	
	static u8 RxSdErrFlagCount=0;
	static u8 RxTdErrFlagCount=0;
	s8 resvalue;
	
	//	memset(&SdrText,0,sizeof(VVVFSdrText));	
	//	memset(&TdrText,0,sizeof(VVVFTdrText));

	while(1)
	{
	/*�ȴ�����SDR�¼���־*/
	//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&VVVF1EventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);

		if(err !=OS_ERR_NONE)
			printf("VVFErr=%d\r\n",err);
		
		/*1.��������*/
		if(err == OS_ERR_NONE)	//��ʾû�д���
		{
			CallOddEvenCount = CallCount%2;				//�õ���ż��
			
			if(CallOddEvenCount ==0) 							//ż����ֻ�ܷ���SDR
			{
				memset(&SdrText,0,sizeof(VVVFSdrText));
				
				SdrText.TextNumber = VVVF_SDR_TEXTNUM;
				SdrText.Command = VVVF_SDR_COMMAND;
				SdrText.State = VVVF_SDR_STATE;
				SdrText.Sender = VVVF_SDR_SENDER;
				SdrText.Receiver = VVVF_SDR_RECEIVER1;
				SdrText.OrderNo = VVVF_SDR_NO;
				
				/*BCD��*/
				SdrText.Year = DecToBcd(can2_RxTime[0]);
				SdrText.Month = DecToBcd(can2_RxTime[1]);
				SdrText.Day = DecToBcd(can2_RxTime[2]);
				SdrText.Hour = DecToBcd(can2_RxTime[3]);
				SdrText.Minute = DecToBcd(can2_RxTime[4]);
				SdrText.Second = DecToBcd(can2_RxTime[5]);
				
				//ʱ����Чλ
				if(can2_RxTime[1] >0) //�·ݴ���0 ����ʾʱ����Ч
				{
					can2_RxVVVFCmd[0] |= 0x04;  //VVVF1,ʱ����Чλ
				}
				else
				{
					can2_RxVVVFCmd[0] &= 0xFB;  //���
				}
				SdrText.TCU_TimeEffect = (can2_RxVVVFCmd[0]>>2)&0x01;
				
				//				//ʱ����Чλ
				//				if(VVVF1TimeEffFlag == true)
				//				{
				//					VVVF1TimeEffFlag = false;
				//					SdrText.TCU_TimeEffect =1;
				//				}
				//				else
				//				{
				//					SdrText.TCU_TimeEffect =0;
				//				}

				
				//ʱ������
				if(((can2_RxVVVFCmd[0]>>3)&0x01) ==1 )
				{
					TimeFlagCount++;		//ʱ�����ü����־λ
					SdrText.TCU_TimeSet = 1;	
					
					if(TimeFlagCount>=5)	
					{
						TimeFlagCount = 0;  //����ʱ�����ü���ֵ
						can2_RxVVVFCmd[0] &= 0xF7;
					}					
				}
				else
				{
					SdrText.TCU_TimeSet = 0;
				}
				
				//				//ʱ������λ
				//				if(VVVF1SetTimeFlag == true)
				//				{
				//					TimeFlagCount++;		//ʱ�����ü����־λ
				//					SdrText.TCU_TimeSet = 1;	
				//					
				//					if(TimeFlagCount>=5)	
				//					{
				//						TimeFlagCount = 0;  //����ʱ�����ü���ֵ
				//						VVVF1SetTimeFlag = false;
				//					}
				//				}
				//				else
				//				{
				//					SdrText.TCU_TimeSet = 0;
				//				}

				SendVVVFSdrOrTdrOrTcr((u8*)&SdrText,VVVF_SEND_SDR);				//����SDR	

			}
			
			else	//��ʾΪ���� ��ֻ�ܷ���TDR��TCR�����߲�����
			{			
				if(VVVF1TxTdrFlag == true)						//��ʾ��Ҫ����Tdr
				{			
					memset(&TdrText,0,sizeof(VVVFTdrText));
					
					TdrText.TextNumber = VVVF_TDR_TEXTNUM;
					TdrText.Command = VVVF_TDR_COMMAND;
					TdrText.State = VVVF_TDR_STATE;
					TdrText.Sender = VVVF_TDR_SENDER;
					TdrText.Receiver = VVVF_TDR_RECEIVER1;
					TdrText.OrderNo = VVVF_TDR_NO;
					TdrText.CategoryCode = can2_RxVVVFCmd[2];		//VVVF1���������ֽ�
					TdrText.BlockNumH8 = (BlockNo>>8)&0xFF;
					TdrText.BlockNumL8 = (BlockNo)&0xFF;
					
					SendVVVFSdrOrTdrOrTcr((u8*)&TdrText,VVVF_SEND_TDR);		//����TDR
					
					BlockNo ++;
					//					if(BlockNo >140)
					//					{
					//						BlockNo = 0;							//���
					//						VVVF1TxTdrFlag = false;  //��� �����ڷ���TDR
					//					}
				}
						
				else if(VVVF1TxTcrFlag == true)	//����TCR
				{
					memset(&TcrText,0,sizeof(VVVFTcrText));
					
					TcrText.TextNumber = VVVF_TCR_TEXTNUM;
					TcrText.Command = VVVF_TCR_COMMAND;
					TcrText.State = VVVF_TCR_STATE;
					TcrText.Sender = VVVF_TCR_SENDER;
					TcrText.Receiver = VVVF_TCR_RECEIVER1;
					TcrText.OrderNo = VVVF_TCR_NO;
					TcrText.CategoryCode = 0x02;
					
					SendVVVFSdrOrTdrOrTcr((u8*)&TcrText,VVVF_SEND_TCR);
					
				}
						
				else		//�������κ�����
				{
					//printf("VVVF Not Send data!\r\n");
				}
			}

					
			if(CallCount>=10)		//��Ϊ��TDR�����͵Ĵ���Ϊ10��ʱΪż���������������1����������
			{
				CallCount =0;
			}			
			CallCount ++;		//ÿ����һ�μ�1		
		
			
			/*2.�ȴ���������*/
			{
				if(CallOddEvenCount == 0)  //��ʾΪż�� �����Sd
				{
					resvalue = GetVVVFSdOrTd((u8 *)HDLCARxBuf,(u8*)&SdFrameData.VVVFSdData,VVVF1);
					
					if(resvalue !=0)	//����ֵ�д���
					{
						RxSdErrFlagCount++;
						if(RxSdErrFlagCount >=25 )  //����25��û�н��ܵ�����Ϊ�����쳣
						{
							//can2_TxSubDevFault_VVVF1_Or_SIV =1;
							//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]|0x20;		//1
							
							can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]|0x20;
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
							
							//printf("VVVF1 �����쳣!\r\n");
							RxSdErrFlagCount =0;	//���
						}
					}
					else
					{
						RxSdErrFlagCount =0;	//���
						//can2_TxSubDevFault_VVVF1_Or_SIV =0;
						//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]&0xDF;		//����
						
						can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]&0xDF;
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
						
						/*copy-VVVF�����ݣ����͵�can2������*/
						memcpy(can2_TxVVVF1Sta1,(u8*)&SdFrameData.VVVFSdData+6,8);  //�׵�ƫ��6�ֽ�  //WHH ,ע����ҪǿתΪ(U8 *)��ָ��ӵĵ�ַһ�βż�1
						memcpy(can2_TxVVVF1Sta2,(u8*)&SdFrameData.VVVFSdData+14,6);
						memcpy(can2_TxVVVF1Sta3,(u8*)&SdFrameData.VVVFSdData+20,8);
						memcpy(can2_TxVVVF1Sta4,(u8*)&SdFrameData.VVVFSdData+28,8);
						memcpy(can2_TxVVVF1Sta5,(u8*)&SdFrameData.VVVFSdData+36,6);
						
						//						{
						//							u8 buf[sizeof(VVVFSdText)];
						//							u8 i;
						//							printf("sta1\r\n");
						//							
						//							memcpy(buf,(u8*)&SdFrameData.VVVFSdData,sizeof(VVVFSdText));
						//							
						//							for(i=0;i<sizeof(VVVFSdText);i++)
						//							{
						//								printf("%02x",buf[i]);
						//							}
						//							printf("\r\n");
						//							
						//							printf("sta2=0x%x",can2_TxVVVF1Sta2[0]);
						//						}
						
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TDPO_19C_VVVFSta1].last_message.cob_id = 0;	
						/* force emission of PDO by artificially changing last emitted */	
						MFDCAN2Slave_Data.PDO_status[TDPO_19D_VVVFSta2].last_message.cob_id = 0;		
						MFDCAN2Slave_Data.PDO_status[TDPO_19E_VVVFSta3].last_message.cob_id = 0;	
						/* force emission of PDO by artificially changing last emitted */	
						MFDCAN2Slave_Data.PDO_status[TDPO_19F_VVVFSta4].last_message.cob_id = 0;	
						MFDCAN2Slave_Data.PDO_status[TDPO_1A0_VVVFSta5].last_message.cob_id = 0;	
						
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_19C_VVVFSta1);		//����PDO
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_19D_VVVFSta2);
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_19E_VVVFSta3);		//����PDO
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_19F_VVVFSta4);
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A0_VVVFSta5);		//����PDO
					}
				}
				
				else		//��ʾΪ�����������Td
				{
					if(VVVF1TxTcrFlag == true)  //Tcr ����������
					{
						VVVF1TxTcrFlag =false;
					}
					
					else if(VVVF1TxTdrFlag == true)	//Tdr��������
					{
						if(BlockNo >140)
						{
							BlockNo = 0;							//���
							VVVF1TxTdrFlag = false;  //��� �����ڷ���TDR
						}

						resvalue = GetVVVFSdOrTd((u8 *)HDLCARxBuf,(u8*)&TdFrameData.VVVFTdData,VVVF1);
						
						if(resvalue !=0)	//����ֵ�д���
						{
							RxTdErrFlagCount++;
							if(RxTdErrFlagCount >=25 )  //����25��û�н��ܵ�����Ϊ�����쳣
							{
								//can2_TxSubDevFault_VVVF1_Or_SIV =1;
								//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]|0x20;		//1
								
								can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]|0x20;
								/* force emission of PDO by artificially changing last emitted */
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
								
								printf("VVVF1_Rx TD ���ݳ�ʱ!\r\n");
								RxTdErrFlagCount =0;	//���
							}
						}
						
						else
						{
							RxTdErrFlagCount =0;	//���
							//can2_TxSubDevFault_VVVF1_Or_SIV =0;
							//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]&0xDF;		//����
							
							can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]&0xDF;
							
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO		  MFD1

							/*copy-VVVF�����ݣ����͵�can2������*/	
							
							/*WHH TDR�����ֵ����ݻ�û�ж���*/
						}
					}
					
					else  //�������κ�����
					{
					}
				}
			}	
			
		}
	}
}

/*
����һ��ȫ����ţ������ڶ����ֵ���   ����ʾ��ǰ�Ƿ���Ҫ����TDR
*/
bool 	VVVF2TxTdrFlag = false; //�Ƿ���Ҫ����TDR 0,����Ҫ�� 1 ��Ҫ
bool 	VVVF2TxTcrFlag	= false;
//bool	VVVF2SetTimeFlag = false;
//bool  VVVF2TimeEffFlag = false;

/*VVVF2״̬��ѯ����*/
void VVVF2StateQuery_Take(void *p_arg)
{
	OS_ERR err;
	static u8  TimeFlagCount=0;
	static u8  CallCount = 0;							//���ü���ֵ
	u8 CallOddEvenCount=0; 								//�������õ���ż���� (ż����ʱ��һֱ����SDR,ֻ�������Ż��ж��Ƿ���TDR��TCR�����˳�)
	
	VVVFSdrText 	SdrText;
	VVVFTdrText 	TdrText;
	VVVFTcrText   TcrText;
	
	VVVFSdFrame   SdFrameData;
	VVVFTdFrame   TdFrameData;
	
	static u16 BlockNo=0;	
	static u8 RxSdErrFlagCount=0;
	static u8 RxTdErrFlagCount=0;
	s8 resvalue;
	
	//	memset(&SdrText,0,sizeof(VVVFSdrText));	
	//	memset(&TdrText,0,sizeof(VVVFTdrText));

	while(1)
	{
	/*�ȴ�����SDR�¼���־*/
	//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&VVVF2EventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);

		if(err !=OS_ERR_NONE)
			printf("VVF2Err=%d\r\n",err);
		
		/*1.��������*/
		if(err == OS_ERR_NONE)	//��ʾû�д���
		{
			CallOddEvenCount = CallCount%2;				//�õ���ż��
			
			if(CallOddEvenCount ==0) 							//ż����ֻ�ܷ���SDR
			{
				memset(&SdrText,0,sizeof(VVVFSdrText));
				
				SdrText.TextNumber = VVVF_SDR_TEXTNUM;
				SdrText.Command = VVVF_SDR_COMMAND;
				SdrText.State = VVVF_SDR_STATE;
				SdrText.Sender = VVVF_SDR_SENDER;
				SdrText.Receiver = VVVF_SDR_RECEIVER2;
				SdrText.OrderNo = VVVF_SDR_NO;
				
				/*BCD��*/
				SdrText.Year = DecToBcd(can2_RxTime[0]);
				SdrText.Month = DecToBcd(can2_RxTime[1]);
				SdrText.Day = DecToBcd(can2_RxTime[2]);
				SdrText.Hour = DecToBcd(can2_RxTime[3]);
				SdrText.Minute = DecToBcd(can2_RxTime[4]);
				SdrText.Second = DecToBcd(can2_RxTime[5]);
				
				//ʱ����Чλ
				if(can2_RxTime[1] >0) //�·ݴ���0 ����ʾʱ����Ч
				{
					can2_RxVVVFCmd[0] |= 0x01;  //VVVF2,ʱ����Чλ
				}
				else
				{
					can2_RxVVVFCmd[0] &= 0xFE;
				}
				SdrText.TCU_TimeEffect = can2_RxVVVFCmd[0]&0x01;				
				
				//				//ʱ����Чλ
				//				if(VVVF2TimeEffFlag == true)
				//				{
				//					VVVF2TimeEffFlag = false;
				//					SdrText.TCU_TimeEffect =1;
				//				}
				//				else
				//				{
				//					SdrText.TCU_TimeEffect =0;
				//				}

				//ʱ������
				if(((can2_RxVVVFCmd[0]>>1)&0x01) ==1 )  //VVVF2,ʱ��������Чλ
				{
					TimeFlagCount++;		//ʱ�����ü����־λ
					SdrText.TCU_TimeSet = 1;	
					
					if(TimeFlagCount>=5)	
					{
						TimeFlagCount = 0;  //����ʱ�����ü���ֵ
						can2_RxVVVFCmd[0] &= 0xFD;
					}					
				}
				else
				{
					SdrText.TCU_TimeSet = 0;
				}
				
				//				//ʱ������λ
				//				if(VVVF2SetTimeFlag == true)
				//				{
				//					TimeFlagCount++;		//ʱ�����ü����־λ
				//					SdrText.TCU_TimeSet = 1;	
				//					
				//					if(TimeFlagCount>=5)	
				//					{
				//						 TimeFlagCount = 0;  //����ʱ�����ü���ֵ
				//						 VVVF2SetTimeFlag = false;
				//					}
				//				}
				//				else
				//				{
				//					SdrText.TCU_TimeSet = 0;
				//				}

				SendVVVFSdrOrTdrOrTcr((u8*)&SdrText,VVVF_SEND_SDR);				//����SDR	

			}
			
			else	//��ʾΪ���� ��ֻ�ܷ���TDR��TCR�����߲�����
			{			
				if(VVVF2TxTdrFlag == true)						//��ʾ��Ҫ����Tdr
				{			
					memset(&TdrText,0,sizeof(VVVFTdrText));
					
					TdrText.TextNumber = VVVF_TDR_TEXTNUM;
					TdrText.Command = VVVF_TDR_COMMAND;
					TdrText.State = VVVF_TDR_STATE;
					TdrText.Sender = VVVF_TDR_SENDER;
					TdrText.Receiver = VVVF_TDR_RECEIVER2;
					TdrText.OrderNo = VVVF_TDR_NO;
					TdrText.CategoryCode = can2_RxVVVFCmd[2];		//VVVF2���������ֽ�
					TdrText.BlockNumH8 = (BlockNo>>8)&0xFF;
					TdrText.BlockNumL8 = (BlockNo)&0xFF;
					
					SendVVVFSdrOrTdrOrTcr((u8*)&TdrText,VVVF_SEND_TDR);		//����TDR
					
					BlockNo ++;
					//					if(BlockNo >140)
					//					{
					//						BlockNo = 0;							//���
					//						VVVF2TxTdrFlag = false;  //��� �����ڷ���TDR
					//					}
				}
						
				else if(VVVF2TxTcrFlag == true)	//����TCR
				{
					memset(&TcrText,0,sizeof(VVVFTcrText));
					
					TcrText.TextNumber = VVVF_TCR_TEXTNUM;
					TcrText.Command = VVVF_TCR_COMMAND;
					TcrText.State = VVVF_TCR_STATE;
					TcrText.Sender = VVVF_TCR_SENDER;
					TcrText.Receiver = VVVF_TCR_RECEIVER2;
					TcrText.OrderNo = VVVF_TCR_NO;
					TcrText.CategoryCode = 0x02;
					
					SendVVVFSdrOrTdrOrTcr((u8*)&TcrText,VVVF_SEND_TCR);
					
				}
						
				else		//�������κ�����
				{
					//printf("VVVF2 Not Send data!\r\n");
				}
			}

					
			if(CallCount>=10)		//��Ϊ��TDR�����͵Ĵ���Ϊ10��ʱΪż���������������1����������
			{
				CallCount =0;
			}			
			CallCount ++;		//ÿ����һ�μ�1		
		
			/*2.�ȴ���������*/
			{
				if(CallOddEvenCount == 0)  //��ʾΪż�� �����Sd
				{
					resvalue = GetVVVFSdOrTd((u8 *)HDLCBRxBuf,(u8*)&SdFrameData.VVVFSdData,VVVF2);
					
					if(resvalue !=0)	//����ֵ�д���
					{
						RxSdErrFlagCount++;
						printf("VVVF2res=%d\r\n",resvalue);
						
						if(RxSdErrFlagCount >=25 )  //����25��û�н��ܵ�����Ϊ�����쳣
						{
							//can2_TxSubDevFault_VVVF2 =1;
							//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]|0x10;		//��1
							
							can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]|0x10;		//��1
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO		MFD1
							
							//printf("VVVF2 �����쳣!\r\n");
							RxSdErrFlagCount =0;	//���
						}
					}
					else
					{
						RxSdErrFlagCount =0;	//���
						//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]&0xEF;		//����
						
						can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]&0xEF;
												
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
						
						/*copy-VVVF�����ݣ����͵�can2������*/
						memcpy(can2_TxVVVF2Sta1,(u8*)&SdFrameData.VVVFSdData+6,8);  //�׵�ƫ��6�ֽ�  //WHH ,ע����ҪǿתΪ(U8 *)��ָ��ӵĵ�ַһ�βż�1
						memcpy(can2_TxVVVF2Sta2,(u8*)&SdFrameData.VVVFSdData+14,6);
						memcpy(can2_TxVVVF2Sta3,(u8*)&SdFrameData.VVVFSdData+20,8);
						memcpy(can2_TxVVVF2Sta4,(u8*)&SdFrameData.VVVFSdData+28,8);
						memcpy(can2_TxVVVF2Sta5,(u8*)&SdFrameData.VVVFSdData+36,6);
						
						
				//						{
				//							u8 buf[sizeof(VVVFSdText)];
				//							u8 i;
				//							printf("sta1\r\n");
				//							
				//							memcpy(buf,(u8*)&SdFrameData.VVVFSdData,sizeof(VVVFSdText));
				//							
				//							for(i=0;i<sizeof(VVVFSdText);i++)
				//							{
				//								printf("%02x",buf[i]);
				//							}
				//							printf("\r\n");
				//							
				//							printf("sta2=0x%x",can2_TxVVVF1Sta2[0]);
											
				//							
				//														for(i=0;i<8;i++)
				//								printf("sta1=%02x\r\n",can2_TxVVVF2Sta1[i]);
				//														for(i=0;i<6;i++)
				//								printf("sta2=%02x\r\n",can2_TxVVVF2Sta2[i]);
				//														for(i=0;i<8;i++)
				//								printf("sta3=%02x\r\n",can2_TxVVVF2Sta3[i]);
				//							for(i=0;i<8;i++)
				//								printf("sta4=%02x\r\n",can2_TxVVVF2Sta4[i]);
				//														for(i=0;i<6;i++)
				//								printf("sta5=%02x\r\n",can2_TxVVVF2Sta5[i]);
				//						}						
										
						
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TDPO_1A1_VVVFSta1].last_message.cob_id = 0;	
						/* force emission of PDO by artificially changing last emitted */	
						MFDCAN2Slave_Data.PDO_status[TDPO_1A2_VVVFSta2].last_message.cob_id = 0;		
						MFDCAN2Slave_Data.PDO_status[TDPO_1A3_VVVFSta3].last_message.cob_id = 0;	
						/* force emission of PDO by artificially changing last emitted */	
						MFDCAN2Slave_Data.PDO_status[TDPO_1A4_VVVFSta4].last_message.cob_id = 0;	
						MFDCAN2Slave_Data.PDO_status[TDPO_1A5_VVVFSta5].last_message.cob_id = 0;	
						
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A1_VVVFSta1);		//����PDO
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A2_VVVFSta2);
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A3_VVVFSta3);		//����PDO
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A4_VVVFSta4);
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A5_VVVFSta5);		//����PDO
					}
				}
				
				else		//��ʾΪ�����������Td
				{
					if(VVVF2TxTcrFlag == true)  //Tcr ����������
					{
						VVVF2TxTcrFlag =false;
					}
					
					else if(VVVF2TxTdrFlag == true)	//Tdr��������
					{
						if(BlockNo >140)
						{
							BlockNo = 0;							//���
							VVVF2TxTdrFlag = false;  //��� �����ڷ���TDR
						}

						resvalue = GetVVVFSdOrTd((u8 *)HDLCBRxBuf,(u8*)&TdFrameData.VVVFTdData,VVVF2);
						
						if(resvalue !=0)	//����ֵ�д���
						{
							RxTdErrFlagCount++;
							if(RxTdErrFlagCount >=25 )  //����25��û�н��ܵ�����Ϊ�����쳣
							{
								//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]|0x10;		//��1
								
								can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]|0x10;		//��1
								/* force emission of PDO by artificially changing last emitted */
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
								
								printf("VVVF2_Rx TD ���ݳ�ʱ!\r\n");
								RxTdErrFlagCount =0;	//���
							}
						}
						
						else
						{
							RxTdErrFlagCount =0;	//���
							//can2_TxSubDevFault[1] = can2_TxSubDevFault[1]&0xEF;		//����
							can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]&0xEF;
							
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;							
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO		MFD1  

							/*copy-VVVF�����ݣ����͵�can2������*/	
							
							/*WHH TDR�����ֵ����ݻ�û�ж���*/
						}
					}
					
					else  //�������κ�����
					{
						
					}
				}
			}	
			
		}
	}
}


/*
����һ��ȫ����ţ������ڶ����ֵ���   ����ʾ��ǰ�Ƿ���Ҫ����TDR
*/
bool 	SIVTxTdrFlag = false; //�Ƿ���Ҫ����TDR 0,����Ҫ�� 1 ��Ҫ
bool 	SIVTxTcrFlag	= false;
//bool	SIVSetTimeFlag = false;
//bool  SIVTimeEffFlag = false;
/*SIV״̬��ѯ����*/
void SIVStateQuery_Take(void *p_arg)
{
	OS_ERR err;
	static u8  TimeFlagCount=0;
	static u8  CallCount = 0;							//���ü���ֵ
	u8 CallOddEvenCount=0; 								//�������õ���ż���� (ż����ʱ��һֱ����SDR,ֻ�������Ż��ж��Ƿ���TDR��TCR�����˳�)
	
	SIVSdrText 	SdrText;
	SIVTdrText 	TdrText;
	SIVTcrText  TcrText;
	
	SIVSdFrame   SdFrameData;
	SIVTdFrame   TdFrameData;
	
	static u16 BlockNo=0;	
	static u8 RxSdErrFlagCount=0;
	static u8 RxTdErrFlagCount=0;
	s8 resvalue;
	
	//	memset(&SdrText,0,sizeof(SIVSdrText));	
	//	memset(&TdrText,0,sizeof(SIVTdrText));

	while(1)
	{
	/*�ȴ�����SDR�¼���־*/
	//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&SIVEventFlags,
							 (OS_FLAGS	  )TxSDRFlag,					//����SDR��־λ
							 (OS_TICK     )0,									//�����ȴ�
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ALL+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);

		/*1.��������*/
		if(err == OS_ERR_NONE)	//��ʾû�д���
		{
			CallOddEvenCount = CallCount%2;				//�õ���ż��
			
			if(CallOddEvenCount ==0) 							//ż����ֻ�ܷ���SDR
			{
				memset(&SdrText,0,sizeof(SIVSdrText));
				
				SdrText.TextNumber = SIV_SDR_TEXTNUM;
				SdrText.Command = SIV_SDR_COMMAND;
				SdrText.State = SIV_SDR_STATE;
				SdrText.Sender = SIV_SDR_SENDER;
				SdrText.Receiver = SIV_SDR_RECEIVER;
				SdrText.OrderNo = SIV_SDR_NO;
				
				/*BCD��*/
				SdrText.Year = DecToBcd(can2_RxTime[0]);
				SdrText.Month = DecToBcd(can2_RxTime[1]);
				SdrText.Day = DecToBcd(can2_RxTime[2]);
				SdrText.Hour = DecToBcd(can2_RxTime[3]);
				SdrText.Minute = DecToBcd(can2_RxTime[4]);
				SdrText.Second = DecToBcd(can2_RxTime[5]);
				
				//ʱ����Чλ
				if(can2_RxTime[1] >0) //�·ݴ���0 ����ʾʱ����Ч
				{
					can2_RxSIVCmd[0] |= 0x01;  //SIV,ʱ����Чλ
				}
				else
				{
					can2_RxSIVCmd[0] &= 0xFE;
				}
				SdrText.SIV_TimeEffect = can2_RxSIVCmd[0]&0x01;		
				
				//				//ʱ����Чλ
				//				if(SIVTimeEffFlag == true)
				//				{
				//					SIVTimeEffFlag = false;
				//					SdrText.SIV_TimeEffect =1;
				//				}
				//				else
				//				{
				//					SdrText.SIV_TimeEffect =0;
				//				}
	
				//ʱ������
				if(((can2_RxSIVCmd[0]>>1)&0x01) ==1 )  //SIV,ʱ��������Чλ
				{
					TimeFlagCount++;		//ʱ�����ü����־λ
					SdrText.SIV_TimeSet = 1;	
					
					if(TimeFlagCount>=5)	
					{
						TimeFlagCount = 0;  //����ʱ�����ü���ֵ
						can2_RxSIVCmd[0] &= 0xFD;
					}					
				}
				else
				{
					SdrText.SIV_TimeSet = 0;
				}
				
				//				//ʱ������λ
				//				if(SIVSetTimeFlag == true)
				//				{
				//					TimeFlagCount++;		//ʱ�����ü����־λ
				//					SdrText.SIV_TimeSet = 1;	
				//					
				//					if(TimeFlagCount>=5)	
				//					{
				//						TimeFlagCount = 0;  //����ʱ�����ü���ֵ
				//						SIVSetTimeFlag = false;
				//					}
				//				}
				//				else
				//				{
				//					SdrText.SIV_TimeSet = 0;
				//				}

				SendSIVSdrOrTdrOrTcr((u8*)&SdrText,SIV_SEND_SDR);				//����SDR	

			}
			
			else	//��ʾΪ���� ��ֻ�ܷ���TDR��TCR�����߲�����
			{			
				if(SIVTxTdrFlag == true)						//��ʾ��Ҫ����Tdr
				{			
					memset(&TdrText,0,sizeof(SIVTdrText));
					
					TdrText.TextNumber = SIV_TDR_TEXTNUM;
					TdrText.Command = SIV_TDR_COMMAND;
					TdrText.State = SIV_TDR_STATE;
					TdrText.Sender = SIV_TDR_SENDER;
					TdrText.Receiver = SIV_TDR_RECEIVER;
					TdrText.OrderNo = SIV_TDR_NO;
					TdrText.SIV_CategoryCode = can2_RxSIVCmd[0];		//SIV������ֽ�
					TdrText.SIV_TDBlackNoRqH8 = (BlockNo>>8)&0xFF;
					TdrText.SIV_TDBlackNoRqL8 = (BlockNo)&0xFF;
					
					SendSIVSdrOrTdrOrTcr((u8*)&TdrText,SIV_SEND_TDR);		//����TDR
					
					BlockNo ++;
					//					if(BlockNo >140)
					//					{
					//						BlockNo = 0;							//���
					//						SIVTxTdrFlag = false;  //��� �����ڷ���TDR
					//					}
				}
						
				else if(SIVTxTcrFlag == true)	//����TCR
				{
					memset(&TcrText,0,sizeof(SIVTcrText));
					
					TcrText.TextNumber = SIV_TCR_TEXTNUM;
					TcrText.Command = SIV_TCR_COMMAND;
					TcrText.State = SIV_TCR_STATE;
					TcrText.Sender = SIV_TCR_SENDER;
					TcrText.Receiver = SIV_TCR_RECEIVER;
					TcrText.OrderNo = SIV_TCR_NO;
					TcrText.KindOfRecord = 0x02;
					
					SendSIVSdrOrTdrOrTcr((u8*)&TcrText,SIV_SEND_TCR);
					
				}
						
				else		//�������κ�����
				{
					printf("SIV Not Send data!\r\n");
				}
			}

					
			if(CallCount>=10)		//��Ϊ��TDR�����͵Ĵ���Ϊ10��ʱΪż���������������1����������
			{
				CallCount =0;
			}			
			CallCount ++;		//ÿ����һ�μ�1		
		
			
			/*2.�ȴ���������*/
			{
				if(CallOddEvenCount == 0)  //��ʾΪż�� �����Sd
				{
					resvalue = GetSIVSdOrTd((u8 *)HDLCARxBuf,(u8*)&SdFrameData.SIVSdData);
					
					if(resvalue !=0)	//����ֵ�д���
					{
						RxSdErrFlagCount++;
						if(RxSdErrFlagCount >=25 )  //����25��û�н��ܵ�����Ϊ�����쳣
						{
							can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]|0x20;
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
							
							printf("SIV �����쳣!\r\n");
							RxSdErrFlagCount =0;	//���
						}
					}
					else
					{
						RxSdErrFlagCount =0;	//���
						
						can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]&0xDF;
						
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;						
						sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
						
						/*copy-SIV�����ݣ����͵�can2������*/
						memcpy(can2_TxSIVSd1,(u8*)&SdFrameData.SIVSdData+6,1);	//�׵�ƫ��6�ֽ�  //WHH ,ע����ҪǿתΪ(U8 *)��ָ��ӵĵ�ַһ�βż�1
						memcpy(&can2_TxSIVSd1[1],(u8*)&SdFrameData.SIVSdData+13,6);  
						memcpy(can2_TxSIVSd2,(u8*)&SdFrameData.SIVSdData+19,1);	//copy���ߵ�ѹ,������һ���ֽ�Ϊ��
						memcpy(&can2_TxSIVSd2[1],(u8*)&SdFrameData.SIVSdData+21,7);	
						memcpy(can2_TxSIVSd3,(u8*)&SdFrameData.SIVSdData+34,3); ////���ʵ�λ(V ��) �¶� ,����6�ֽ�Ϊ��
						
						{
							u8 buf[sizeof(SIVSdText)];
							u8 i;
							printf("sta1\r\n");
							
							memcpy(buf,(u8*)&SdFrameData.SIVSdData,sizeof(SIVSdText));
							
							for(i=0;i<sizeof(SIVSdText);i++)
							{
								printf("%02x",buf[i]);
							}
							printf("\r\n");
						}
						
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[TDPO_1A6_SIVSta1].last_message.cob_id = 0;	
						/* force emission of PDO by artificially changing last emitted */	
						MFDCAN2Slave_Data.PDO_status[TDPO_1A7_SIVSta2].last_message.cob_id = 0;		
						MFDCAN2Slave_Data.PDO_status[TDPO_1A8_SIVSta3].last_message.cob_id = 0;	

						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A6_SIVSta1);		//����PDO
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A7_SIVSta2);
						sendOnePDOevent(&MFDCAN2Slave_Data,TDPO_1A8_SIVSta3);		//����PDO
					}
				}
				
				else		//��ʾΪ�����������Td
				{
					if(SIVTxTcrFlag == true)  //Tcr ����������
					{
						SIVTxTcrFlag =false;
					}
					
					else if(SIVTxTdrFlag == true)	//Tdr��������
					{
						if(BlockNo >140)
						{
							BlockNo = 0;							//���
							SIVTxTdrFlag = false;  //��� �����ڷ���TDR
						}

						resvalue = GetSIVSdOrTd((u8 *)HDLCARxBuf,(u8*)&TdFrameData.SIVTdData);
						
						if(resvalue !=0)	//����ֵ�д���
						{
							RxTdErrFlagCount++;
							if(RxTdErrFlagCount >=25 )  //����25��û�н��ܵ�����Ϊ�����쳣
							{
								can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]|0x20;
								/* force emission of PDO by artificially changing last emitted */
								MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
								sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO  MFD1
								
								printf("SIV_Rx TD ���ݳ�ʱ!\r\n");
								RxTdErrFlagCount =0;	//���
							}
						}
						
						else
						{
							RxTdErrFlagCount =0;	//���
							
							can2_TxMFD1SubFault[1] = can2_TxMFD1SubFault[1]&0xDF;
							
							/* force emission of PDO by artificially changing last emitted */
							MFDCAN2Slave_Data.PDO_status[TPDO_1AC_SubDecFault].last_message.cob_id = 0;
							sendOnePDOevent(&MFDCAN2Slave_Data,(UNS8)TPDO_1AC_SubDecFault);		//����PDO		  MFD1

							/*copy-SIV�����ݣ����͵�can2������*/	
							
							/*WHH TDR�����ֵ����ݻ�û�ж���*/
						}
					}
					
					else  //�������κ�����
					{
					}
				}
			}	
			
		}
		
		else
		{
			printf("SIV EventFlags Err!\r\n");
		}
	}

}


/*
//DI �ɼ�������
DI�ɼ���Ӧ�ü�����ʱ�˲�
*/
void DICheck_Take(void *p_arg)
{	
	OS_ERR err;
	
	u8 i=0;
	u8 DI110Buf[VOL110V_NUMBER+1] = {0};		//��һ���±�0��ʹ�ã����±�1��ʼ�洢
	u8 DI24Buf[VOL24V_NUMBER+1] = {0};
	u8 sendreval;
	static u8 CallCount=0;
	
		//	u8 OldDI110Buf[VOL110V_NUMBER+1] = {0};		//��һ���±�0��ʹ�ã����±�1��ʼ�洢
		//	u8 OldDI24Buf[VOL24V_NUMBER+1] = {0};
	while(1)
	{
		//�����ź��� �������ȴ��ź���
		OSSemPend(&DI_Check_Sem,0,OS_OPT_PEND_BLOCKING,0,&err); 
		if(err)
		{
			printf("DI_Check_Sem=%d\r\n",err);
		}
		else	
		{
			CallCount++;
			//��ͬ�İ忨DI�ĺ��岻ͬ
			switch(ChooseBoard)
			{
				case MC1_MFD1_NODEID:
				case MC2_MFD1_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					Vol_24VRead_Scan(DI24Buf);
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD1DI[0] = 0;
					can2_TxMFD1DI[1] = 0;

					for(i=0;i<sizeof(MCMFD1Di110VIndex);i++)
					{
						if(i<=7)
						{
							can2_TxMFD1DI[0] |= (DI110Buf[MCMFD1Di110VIndex[i]]<<(7-i));
						}
						else
						{
							can2_TxMFD1DI[1] |= (DI110Buf[MCMFD1Di110VIndex[i]]<<(15-i));
						}
					}
					
					for(i=0;i<sizeof(MCMFD1Di24VIndex);i++)
					{
						can2_TxMFD1DI[1] |= (DI24Buf[MCMFD1Di24VIndex[i]<<(3-i)]);		//(7-4-i)
					}

					//printf("MFD1di1=%#x,di2=%#x\r\n",can2_TxMFD1DI[0],can2_TxMFD1DI[1]);
					
					if(CallCount >=2)
					{
						CallCount = 0;
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,0);		//pdo��0  0x18d
					}
					break;
				
				case MC1_MFD2_NODEID:
				case MC2_MFD2_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD2DI[0] = 0;
					can2_TxMFD2DI[1] = 0;
					for(i=0;i<sizeof(MCMFD2DiIndex);i++)
					{
						can2_TxMFD2DI[0] |= (DI110Buf[MCMFD2DiIndex[i]]<<(7-i));		//����ռ7λ�����ֽ����λ��ʼ����
					}
					//			debugprintf("id=0x%x\r\n",can2_TxMFD2DI[0]);
					//			debugprintf("\r\n");
					
					if(CallCount >=2)
					{
						CallCount = 0;
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[1].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,1);		//pdo��1  0x18E		
					}
					break;
				
				case MC1_MFD3_NODEID:
				case MC2_MFD3_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD3DI[0] = 0;
					for(i=0;i<sizeof(MCMFD3DiIndex);i++)
					{
						can2_TxMFD3DI[0] |= (DI110Buf[MCMFD3DiIndex[i]]<<(7-i));	//����ռ4λ�����ֽ����λ��ʼ����
					}
					
					if(CallCount >=2)
					{
						CallCount = 0;
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[2].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,2);		//pdo��2  0x18F		
					}						
					break;		
					
				case T_MFD1_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD1DI[0] = 0;
				
					for(i=0;i<sizeof(TDiIndex);i++)
					{
						can2_TxMFD1DI[0] |= (DI110Buf[TDiIndex[i]]<<(7-i));
					}  
	
					if(CallCount >=2)
					{
						CallCount = 0;	

						//						/*�Ƚϵ�1�� �� ��2�βɼ��������Ƿ���1*/
						//						for(i=0;i<VOL110V_NUMBER+1;i++)
						//						{
						//							if((DI110Buf[i] ==1)&&(OldDI110Buf[i] ==1))
						//							{
						//								DI110Buf[i]= 1;
						//							}
						//							else
						//							{
						//								DI110Buf[i]= 0;	
						//							}						
						//						}
						//						
						//						/*��ÿ��ֵ��ֵ�������ֵ�*/
						//						can2_TxMFD1DI[0] = 0;
						//					
						//						for(i=0;i<sizeof(TDiIndex);i++)
						//						{
						//							can2_TxMFD1DI[0] |= (DI110Buf[TDiIndex[i]]<<(7-i));
						//						}
					
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,0);		//pdo��0  0x18d
						
						//printf("T_DI=%#x\r\n",can2_TxMFD1DI[0]);
					}
					//					memcpy(OldDI110Buf,DI110Buf,VOL110V_NUMBER+1);
					break;			
				
				case M_MFD1_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD1DI[0]=0;

					for(i=0;i<sizeof(MDiIndex);i++)
					{
						can2_TxMFD1DI[0] |=(DI110Buf[MDiIndex[i]]<<(7-i));
					}
					
					if(CallCount >=2)
					{
						CallCount = 0;
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,0);		//pdo��0  0x18d			
					}
					break;

				case T1_MFD1_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD1DI[0]=0;

					for(i=0;i<sizeof(T1DiIndex);i++)
					{
						can2_TxMFD1DI[0] |=(DI110Buf[T1DiIndex[i]]<<(7-i)); 
					}
					
					if(CallCount >=2)
					{
						CallCount = 0;	
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,0);		//pdo��0  0x18d			
					}
					break;
				
				case T2_MFD1_NODEID:
					/*ɨ��DIӲ��״̬*/
					Vol_110VRead_Scan(DI110Buf);
					//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
					
					/*��ÿ��ֵ��ֵ�������ֵ�*/
					can2_TxMFD1DI[0]=0;

					for(i=0;i<sizeof(T2DiIndex);i++)
					{
						can2_TxMFD1DI[0] |=(DI110Buf[T2DiIndex[i]]<<(7-i));
					}
	
					if(CallCount >=2)
					{
						CallCount = 0;				
						/* force emission of PDO by artificially changing last emitted */
						MFDCAN2Slave_Data.PDO_status[0].last_message.cob_id = 0;							
						sendOnePDOevent(&MFDCAN2Slave_Data,0);		//pdo��0  0x18d	
					}						
					break;
				
				//		case MC2_MFD1_NODEID:
				//			/*ɨ��DIӲ��״̬*/
				//			Vol_110VRead_Scan(DI110Buf);
				//			Vol_24VRead_Scan(DI24Buf);
				//			
				//			/*��ÿ��ֵ��ֵ�������ֵ�*/
				//			can2_TxMFD1DI[0] = 0;
				//			can2_TxMFD1DI[1] = 0;

				//			for(i=0;i<sizeof(MCMFD1Di110VIndex);i++)
				//			{
				//				if(i<7)
				//				{
				//					can2_TxMFD1DI[0] |= (DI110Buf[MCMFD1Di110VIndex[i]]<<(7-i));
				//				}
				//				else
				//				{
				//					can2_TxMFD1DI[1] |= (DI110Buf[MCMFD1Di110VIndex[i]]<<(15-i));
				//				}
				//			}
				//			
				//			for(i=0;i<sizeof(MCMFD1Di24VIndex);i++)
				//			{
				//				can2_TxMFD1DI[1] |= (DI24Buf[MCMFD1Di24VIndex[i]<<(3-i)]);		//(7-4-i)
				//			}

				//			break;
				//		
				//		case MC2_MFD2_NODEID:
				//			/*ɨ��DIӲ��״̬*/
				//			Vol_110VRead_Scan(DI110Buf);
				//			//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
				//			
				//			/*��ÿ��ֵ��ֵ�������ֵ�*/
				//			can2_TxMFD2DI[0] = 0;
				//			for(i=0;i<sizeof(MCMFD2DiIndex);i++)
				//			{
				//				can2_TxMFD2DI[0] |= (DI110Buf[MCMFD2DiIndex[i]]<<(7-i));		//����ռ7λ�����ֽ����λ��ʼ����
				//			}
				//			break;
				//		
				//		case MC2_MFD3_NODEID:
				//			/*ɨ��DIӲ��״̬*/
				//			Vol_110VRead_Scan(DI110Buf);
				//			//Vol_24VRead_Scan(DI24Buf);  ֻ��110v
				//			
				//			/*��ÿ��ֵ��ֵ�������ֵ�*/
				//			can2_TxMFD3DI[0] = 0;
				//			for(i=0;i<sizeof(MCMFD3DiIndex);i++)
				//			{
				//				can2_TxMFD3DI[0] |= (DI110Buf[MCMFD3DiIndex[i]]<<(7-i));	//����ռ4λ�����ֽ����λ��ʼ����
				//			}
				//			break;	

				default:
					break;
			}		
		}
	}
}



	/*USART3���ܴ���������*/
void USART3RxTake(void *p_arg)
{
	OS_ERR err;
	CPU_TS ts;
	static unsigned char StartFlag = 0;	
	//uint8_t RxChar;
	while(1)
	{
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,&ts,&err);		//�����ȴ������ź���
		if(err == OS_ERR_NONE)	//û�д���
		{
			switch(ChooseBoard)
			{	
				case MC1_MFD1_NODEID:			//�Ž����ж�
				case T_MFD1_NODEID:	
				case M_MFD1_NODEID:
				case T1_MFD1_NODEID:
				case T2_MFD1_NODEID:
				case MC2_MFD1_NODEID:	
					//if(RxChar == 0x02)  //��ʾ���ܵ�Ϊ��ʼ����
					if((USART3_RS485_RxBuffer[0] ==  0x02 ) && (USART3_RS485_RxLength ==1 )) //��ʾ��һ�ֽ� Ϊ0x02 ��Ϊ��ʼ
					{
						StartFlag = 1;

						g_Uart3RxStartFlag =1;	
						//						/*���ͽ��ܿ�ʼ�¼���־*/
						//						OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
						//							 (OS_FLAGS	  )RxStartFlag,				//��������ܿ�ʼ �¼���־
						//							 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						//							 (OS_ERR*	    )&err);
												
						//printf("U3 Rx start!\r\n");
					}
					if((StartFlag == 1) && (USART3_RS485_RxLength ==8))		//����Ӧ���ݳ���Ϊ8����ʾ���ݽ������
					{
						g_Uart3RxDataFlag = 1;
						StartFlag = 0;

						//						/*���ͽ��ܽ����¼���־*/
						//						OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
						//							 (OS_FLAGS	  )RxEndFlag,				//��������ܽ��� �¼���־
						//							 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						//							 (OS_ERR*	    )&err);			
						
												
						//	printf("U3 Rx end!\r\n");
						//						{
						//							u8 i=0;
						//							for(i=0;i<8;i++)
						//							{
						//								printf("u3[%d]=0x%x\r\n",i,USART3_RS485_RxBuffer[i]);
						//							}
						//						}
					}
				break;
					
				case	MC1_MFD2_NODEID:		//ATC�����ж�
				case	MC2_MFD2_NODEID:
					break;
				
				case  MC1_MFD3_NODEID:		//ACP�����ж�
				case 	MC2_MFD3_NODEID:
					break;
					
				default:
					printf("uart3 Rx ChooseBoard Error !\r\n");
					break;
			}			
		}
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
							 (OS_TICK     )5,								//20ms��ʱ
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_TIMEOUT)		//��ʱ
		{
			MTD_MFD_CANLED_Off(CAN2_TX_LED);
		}
		else if(err == OS_ERR_NONE)
		{
			MTD_MFD_CANLED_On(CAN2_TX_LED);
			BSP_OS_TimeDlyMs(150);
			MTD_MFD_CANLED_Off(CAN2_TX_LED);
		}
		
				//�ȴ��¼���־��
		OSFlagPend((OS_FLAG_GRP*)&CAN2LedEventFlag,
							 (OS_FLAGS	  )CAN2RxLED_FLAG,		//���ͱ�־λ
							 (OS_TICK     )5,								//20ms��ʱ
							 (OS_OPT	    )OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							 (CPU_TS*     )0,
							 (OS_ERR*	    )&err);
		if(err == OS_ERR_TIMEOUT)		//��ʱ
		{
			MTD_MFD_CANLED_Off(CAN2_RX_LED);
		}
		else if(err == OS_ERR_NONE)
		{
			MTD_MFD_CANLED_On(CAN2_RX_LED);
			BSP_OS_TimeDlyMs(150);
			MTD_MFD_CANLED_Off(CAN2_RX_LED);
		}
		//BSP_OS_TimeDlyMs(10);
	}
}




//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr, void *p_arg)
{
	RunLed_Turn();	//���еƷ�ת
}


/*
��ʱ�����ص������Ķ���
*/
void DoorTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	OSFlagPost((OS_FLAG_GRP*)&DoorEventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);
}


void ACCTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	OSFlagPost((OS_FLAG_GRP*)&ACCEventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);	
}


void BCUTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	OSFlagPost((OS_FLAG_GRP*)&BCUEventFlags,
						 (OS_FLAGS	  )TxSDROrTDRFlag,						//���Է���SDR��־,����TDR
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);		
}

void ACPTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	OSFlagPost((OS_FLAG_GRP*)&ACPEventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);			
}

void ATCTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	OSFlagPost((OS_FLAG_GRP*)&ATCEventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);	
}

void PISTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	OSFlagPost((OS_FLAG_GRP*)&PISEventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);	
}

void VVVF1TrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	
	OSFlagPost((OS_FLAG_GRP*)&VVVF1EventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);		
}

void VVVF2TrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	
	OSFlagPost((OS_FLAG_GRP*)&VVVF2EventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);		
}

void SIVTrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	
	OSFlagPost((OS_FLAG_GRP*)&SIVEventFlags,
						 (OS_FLAGS	  )TxSDRFlag,						//���Է���SDR��־
						 (OS_OPT	 	  )OS_OPT_POST_FLAG_SET,
						 (OS_ERR*	    )&err);		
}

void DITrmCallBack(void *p_tmr, void *p_arg)
{
	OS_ERR err;
	//�����ź���
	OSSemPost(&DI_Check_Sem,OS_OPT_POST_1,&err);
}

void assert_failed(u8* file, u32 line)
{
    debugprintf("fail");
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


