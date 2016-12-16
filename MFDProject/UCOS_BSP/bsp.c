/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                         STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*                 DC
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include  <bsp.h>
#include  <bsp_os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_BIT_RCC_PLLCFGR_PLLM               25u
#define  BSP_BIT_RCC_PLLCFGR_PLLN              336u
#define  BSP_BIT_RCC_PLLCFGR_PLLP                2u
#define  BSP_BIT_RCC_PLLCFGR_PLLQ                7u



/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)

/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/
extern	void initTimer(void);
void  BSP_Init (void)
{

/* ϵͳʱ�ӵ����ã��������ļ����Ѿ�������SystemInit()��������main����֮ǰִ�С����Ѿ����ú���ʱ�� */
	/*ʱ�ӵĳ�ʼ��,������Ϊ�ο�*/
	/*SystemInit()Ϊ����HSI��Ĭ���ڲ����ٵ�16M  RCΪʱ��Դ,��C/C++ �����ж����� STM32F40_41xxx�ͻ�ʹ���ⲿ�ģ�
		һ�º���RCC_HSEConfig()�����ⲿ��25M����Ϊʱ��Դ
	*/

//    RCC_DeInit();																								/*����ǰ������RCC����Ϊ��ʼֵ*/			
//    RCC_HSEConfig(RCC_HSE_ON);                                  /* HSE = 25MHz ext. crystal.����ѡ�� �ⲿ����HSE����Ϊ ʱ��Դ��������ȴ��ⲿ����*/
//    while(RCC_WaitForHSEStartUp()!=SUCCESS);										/*�ȴ��ⲿ��������ȶ�״̬*/
//    
//    RCC_HCLKConfig(RCC_SYSCLK_Div1);                            /* HCLK    = AHBCLK  = PLL / AHBPRES(1) = 168MHz.       */
//    RCC_PCLK2Config(RCC_HCLK_Div2);                             /* APB2CLK = AHBCLK  / APB2DIV(2)       = 84MHz.        */
//    RCC_PCLK1Config(RCC_HCLK_Div4);                             /* APB1CLK = AHBCLK  / APB1DIV(4)       = 42MHz (max).  */
//    
//                                                                /* PLLCLK    = HSE * (PLLN / PLLM)      = 336MHz.       */
//                                                                /* SYSCLK    = PLLCLK / PLLP            = 168MHz.       */
//                                                                /* OTG_FSCLK = PLLCLK / PLLQ            =  48MHz.       */ 
//    RCC_PLLConfig(RCC_PLLCFGR_PLLSRC_HSE,
//                  BSP_BIT_RCC_PLLCFGR_PLLM,
//                  BSP_BIT_RCC_PLLCFGR_PLLN, 
//                  BSP_BIT_RCC_PLLCFGR_PLLP, 
//                  BSP_BIT_RCC_PLLCFGR_PLLQ);
//    
//    RCC_PLLCmd(ENABLE);
//    
//    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {       /* Wait for PLL to lock.                                */
//        ;
//    }
//    
//    FLASH_SetLatency(FLASH_Latency_5);                          /* 5 Flash wait states when HCLK > 120MHz.              */
//    FLASH_PrefetchBufferCmd(ENABLE);
//    
//    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                  /* HCLK = SYSCLK = PLL = 168MHz.   ѡ��PLLʱ����Ϊϵͳʱ��Դ*/
//    while (RCC_GetSYSCLKSource() != RCC_CFGR_SWS_PLL) {
//        ;
//    }


/*�������һЩ�����ʼ������*/	 
	/*1.��ʼ��NVIC �ж����ȼ���������*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϷ������� /* ����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ� */
	
	/*2.��ʼ������,���Դ�ӡ��ʾ��Ϣ���жϵ����ȼ��Ѿ�����Ϊ��͡�*/
	Uart1_init();
	
	/*3.����ϵͳʱ�ӣ���ʱ����,�����ʱ���ĳ�ʼ��*/
	delay_init(168);				//delay_init()����ʱ�����ĳ�ʼ����������ϵͳʱ�ӳ�ʼ������֮��
	bsp_InitTimer();      	//�����ʱ���ĳ�ʼ������������������delay() (��ϵͳʱ�ӵĳ�ʼ��֮��)

	/*4.LED����ʱ�������������GPIO��ʼ��*/	
	RunLedGpio_Init();			//��ʼ�����е�
	MTD_MFD_CANLED_Gpio_Init(); //canledָʾ�Ƶĳ�ʼ��

		//TIM3_Int_Init(5000-1,8400-1);			//��ʱ��3 Ҫ���������ȵĶ�ʱ���Ƿ�ʹ��
		//TIM4_Int_Init(5000-1,8400-1);
		//TIM5_Int_Init(5000-1,8400-1);			//��ʱ��5��Ҫ����RUNLED�ƵĿ��ƣ���Ҫ����led�Ƴ�ʼ����ǰ�档��Ϊ�ֿ���µ���ʾ�ơ�
	
	my_mem_init(SRAMIN); 		//�ڲ�RAM��ʼ��	
	
	
	/*7.���Ź��ĳ�ʼ��,��ʼ���ֿ⣬��ʱ���ʼ��nand flash*/
	bsp_InitIwdg(1000);		//��ʼ���������Ź�Ϊ1s��װ��ֵ

	/*10.���CPU ID ,����ʾ������*/
	Check_CPU_Id();	

	/*17 ��ʼ��can��ַ��ȡ��ť*/
	MTDMFD_CanAdd_Gpio_Init();
	ChooseBoard = MTDMFD_CanAddVal();
	
	printf("Choose=0x%x\r\n",ChooseBoard);
	//	/*18 ������GPIO���ŵĳ�ʼ��*/
	//	#ifdef MTD_MFD_1_0_ENABLE
	//		MTDMFD_CL_Gpio_Init();
	//	#endif 
	//	
	//	#ifdef MTD_MFD_2_0_ENABLE
	//		MTDMFD_CL_UART_Gpio_Init(9600);
	//	#endif
	

	/*19.RS485��ʼ��*/
		switch(ChooseBoard)
		{
			case MC1_MFD1_NODEID:
			case T_MFD1_NODEID:	
			case M_MFD1_NODEID:
			case T1_MFD1_NODEID:
			case MC2_MFD1_NODEID:
				SAF82525_Init(SAF_BAUD_RATE_125000);							//HDLC��ʼ��(VVVF1,VVVF2) /T,T1ΪSIV
				MTDMFD_USART2_RS485_Init(19200);									//�յ�
				MTDMFD_USART3_RS485_Init(19200,USART_Parity_Odd);	//��	
				MTDMFD_CL_UART_Gpio_Init(9600);										//�ƶ�
				/*��ѹ�����ʼ��*/
				Vol_110VGpio_Init();
				Vol_24VGpio_Init();
			printf("11\r\n");
				break;
			
			case MC1_MFD2_NODEID:
			case MC2_MFD2_NODEID:
				MTDMFD_USART2_RS485_Init(19200);									//PIS �㲥
				MTDMFD_USART3_RS485_Init(19200,USART_Parity_Odd);	//ATC �ź�
				/*��ѹ�����ʼ��*/
				Vol_110VGpio_Init();
				Vol_24VGpio_Init();
				break;
			
			case MC1_MFD3_NODEID:
			case MC2_MFD3_NODEID:
				MTDMFD_USART3_RS485_Init(19200,USART_Parity_Even);	//ACP ��ѹ��������ż����
				/*��ѹ�����ʼ��*/
				Vol_110VGpio_Init();
				Vol_24VGpio_Init();
				break;
			
			case MC1_MFD4_NODEID:
			case T_MFD2_NODEID:
			case M_MFD2_NODEID:
			case T1_MFD2_NODEID:
			case T2_MFD2_NODEID:
			case MC2_MFD4_NODEID:
				/*��ѹ�����ʼ��*/
				Vol_110VGpio_Init();
				Vol_24VGpio_Init();
				break;			
						
			case T2_MFD1_NODEID:
				MTDMFD_USART2_RS485_Init(19200);									//�յ�
				MTDMFD_USART3_RS485_Init(19200,USART_Parity_Odd);	//��	
				MTDMFD_CL_UART_Gpio_Init(9600);										//�ƶ�
				/*��ѹ�����ʼ��*/
				Vol_110VGpio_Init();
				Vol_24VGpio_Init();
				break;	

			default:
				SAF82525_Init(SAF_BAUD_RATE_125000);							//HDLC��ʼ��(VVVF1,VVVF2) /T,T1ΪSIV
				MTDMFD_USART2_RS485_Init(19200);									//�յ�
				MTDMFD_USART3_RS485_Init(19200,USART_Parity_Odd);	//��	
				MTDMFD_CL_UART_Gpio_Init(9600);										//�ƶ�
				/*��ѹ�����ʼ��*/
				Vol_110VGpio_Init();
				Vol_24VGpio_Init();
				break;
		}
	
#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_IOEN_MASK;    /* Enable tracing (see Note #2).                        */
    BSP_CPU_REG_DBGMCU_CR &= ~BSP_DBGMCU_CR_TRACE_MODE_MASK;    /* Clr trace mode sel bits.                             */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04; /* Cfg trace mode to synch 4-bit.                       */
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: BSP_DelayMs
*	����˵��: Ϊ���õײ������ڴ�RTOS�����������и��õļ�����
*             ר������һ������ʽ���ӳٺ������ڵײ�������ms�����ӳ���Ҫ���ڳ�ʼ����������Ӱ��ʵʱ�ԡ�
*	��    ��: n �ӳٳ��ȣ���λ1 ms
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BSP_DelayMs(uint32_t _ulDelayTime)
{
	BSP_DelayUs(1000*_ulDelayTime);
}

/*
*********************************************************************************************************
*	�� �� ��: BSP_DelayUs
*	����˵��: �������ʱ����CPU���ڲ�����ʵ�֣�32λ������
*       OSSchedLock(&err);
*				bsp_DelayUS(5);
*				OSSchedUnlock(&err); ����ʵ����������Ƿ���Ҫ�ӵ�������ѡ����ж�
*	��    ��: _ulDelayTime  �ӳٳ��ȣ���λ1 us
*	�� �� ֵ: ��
*   ˵    ��: 1. ��Ƶ168MHz������£�32λ������������2^32/168000000 = 25.565��
*                ����ʹ�ñ��������ӳٵĻ����ӳ���1�����¡�  
*             2. ʵ��ͨ��ʾ�������ԣ�΢���ӳٺ�����ʵ������ʵ�ʶ�����0.25us���ҵ�ʱ�䡣
*             �������ݲ���������
*             ��1��. MDK5.15���Ż��ȼ�0, ��ͬ��MDK�Ż��ȼ�����û��Ӱ�졣
*             ��2��. STM32F407IGT6
*             ��3��. ���Է�����
*				 GPIOI->BSRRL = GPIO_Pin_8;
*				 bsp_DelayUS(10);
*				 GPIOI->BSRRH = GPIO_Pin_8;
*             -------------------------------------------
*                ����                 ʵ��ִ��
*             bsp_DelayUS(1)          1.2360us
*             bsp_DelayUS(2)          2.256us
*             bsp_DelayUS(3)          3.256us
*             bsp_DelayUS(4)          4.256us
*             bsp_DelayUS(5)          5.276us
*             bsp_DelayUS(6)          6.276us
*             bsp_DelayUS(7)          7.276us
*             bsp_DelayUS(8)          8.276us
*             bsp_DelayUS(9)          9.276us
*             bsp_DelayUS(10)         10.28us
*            3. ����32λ�޷������������ȡ�Ľ���ٸ�ֵ��32λ�޷�������Ȼ������ȷ�Ļ�ȡ��ֵ��
*              ����A,B,C����32λ�޷�������
*              ���A > B  ��ôA - B = C������ܺ���⣬��ȫû������
*              ���A < B  ��ôA - B = C�� C����ֵ����0xFFFFFFFF - B + A + 1����һ��Ҫ�ر�ע�⣬�������ڱ�������
*********************************************************************************************************
*/
void BSP_DelayUs(uint32_t _ulDelayTime)
{
  uint32_t tCnt, tDelayCnt;
	uint32_t tStart;
		
	tStart = (uint32_t)CPU_TS_TmrRd();                       /* �ս���ʱ�ļ�����ֵ */
	tCnt = 0;
	tDelayCnt = _ulDelayTime * (SystemCoreClock / 1000000);	 /* ��Ҫ�Ľ����� */ 		      

	while(tCnt < tDelayCnt)
	{
		tCnt = (uint32_t)CPU_TS_TmrRd() - tStart; /* ��������У����������һ��32λ���������¼�������Ȼ������ȷ���� */	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: BSP_Idle
*	����˵��: ����ʱִ�еĺ�����һ����������for��whileѭ������������Ҫ���� CPU_IDLE() �������ñ�������
*			 ������ȱʡΪ�ղ������û��������ι��������CPU��������ģʽ�Ĺ��ܡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void BSP_Idle(void)			//���Է��뵽�δ�ʱ�����жϺ�����ȥ
{
	/* --- ι�� */
	IWDG_Feed();
	/* --- ��CPU�������ߣ���Systick��ʱ�жϻ��ѻ��������жϻ��� */

	/* ���� emWin ͼ�ο⣬���Բ���ͼ�ο���Ҫ����ѯ���� */


	/* ���� uIP Э��ʵ�֣����Բ���uip��ѯ���� */
}

/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);
    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}


/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    
    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */
	
#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif
    
    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
																																	/* ����Ĭ�ϵ���������ȼ�������ʵ������޸�             */
//		SysTick_Config(cnts);                                      /* ����Ĭ�ϵ���������ȼ�                               */
}





/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer 
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR' 
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater 
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR' 
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be 
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets 
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple 
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the 
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time 
*                       but MUST be less than the maximum measured time; otherwise, timer resolution 
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit (void)
{
    CPU_INT32U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();

    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;    /* Enable Cortex-M4's DWT CYCCNT reg.                   */
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer 
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR' 
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater 
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR' 
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be 
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets 
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple 
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the 
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is 
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured 
*                                       Timer period            Timer's period in some units of 
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same 
*                                                                   units of (fractional) seconds 
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units 
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less 
*                           than the maximum measured time; otherwise, timer resolution inadequate to 
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd (void)
{
    CPU_TS_TMR  ts_tmr_cnts;

                                                                
    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application programming interface (API) 
*               function which MAY be implemented by application/BSP function(s) [see Note #1] & 
*               MAY be called by application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be 
*                   optionally defined by the developer when either of the following CPU features is 
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of 
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units 
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of 
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured, 
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less 
*                       than the maximum measured time; otherwise, timer resolution inadequate to 
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits 
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec (CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;

    
    fclk_freq = BSP_CPU_ClkFreq();    
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec (CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;
    

    fclk_freq = BSP_CPU_ClkFreq();    
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


void Check_CPU_Id(void)
{
	/* ���CPU ID */

		/* �ο��ֲ᣺
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("CPU : STM32F407ZGT6, LQFP144, UID = %08X %08X %08X \r\n"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);		//96λ��id
}

/*
*********************************************************************************************************
*	�� �� ��: DispTaskInfo
*	����˵��: ��uCOS-III������Ϣ��ӡ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DispTaskInfo(void)
{
	OS_TCB      *p_tcb;	        /* ����һ��������ƿ�ָ��, TCB = TASK CONTROL BLOCK */
	float CPU;
	CPU_SR_ALLOC();

	CPU_CRITICAL_ENTER();
		p_tcb = OSTaskDbgListPtr;
	CPU_CRITICAL_EXIT();
	
	/* ��ӡ���� */
		printf("  Prio   Used    Free  Per     CPU    Taskname\r\n");

	/* ����������ƿ��б�(TCB list)����ӡ���е���������ȼ������� */
	while (p_tcb != (OS_TCB *)0) 
	{
		CPU = (float)p_tcb->CPUUsage / 100;
		printf("   %2d  %5d  %5d    %02d%%   %5.2f%%   %s\r\n", 
		p_tcb->Prio, 
		p_tcb->StkUsed, 
		p_tcb->StkFree, 
		(p_tcb->StkUsed * 100) / (p_tcb->StkUsed + p_tcb->StkFree),
		CPU,
		p_tcb->NamePtr);		
	 	
		CPU_CRITICAL_ENTER();
			p_tcb = p_tcb->DbgNextPtr;
    CPU_CRITICAL_EXIT();
	}
	
}

