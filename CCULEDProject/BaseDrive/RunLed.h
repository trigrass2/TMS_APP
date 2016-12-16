#ifndef __RUNLED_H__
#define __RUNLED_H__
#include "stm32f4xx.h"


/************LED�ƽӿ�******************/

///*ccu ����*/
//#define RUNLED_RCC           RCC_AHB1Periph_GPIOF
//#define RUNLED_GPIO          GPIOF
//#define RUNLED_GPIO_PIN      GPIO_Pin_11
//#define RUNLED_ONOFF(x)      GPIO_WriteBit(RUNLED_GPIO,RUNLED_GPIO_PIN,x)

/*CCU LED����*/
#define RUNLED_RCC           RCC_AHB1Periph_GPIOE
#define RUNLED_GPIO          GPIOE
#define RUNLED_GPIO_PIN      GPIO_Pin_7
#define RUNLED_ONOFF(x)      GPIO_WriteBit(RUNLED_GPIO,RUNLED_GPIO_PIN,(BitAction)x)

void  RunLedGpio_Init(void);
void  RunLed_On (void);
void  RunLed_Off (void);
void  RunLed_Turn (void);

#endif //__RUNLED_H__
