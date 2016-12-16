#ifndef _UART_H_
#define _UART_H_

#include "stdio.h"

/* ������Դ�ӡ��䣬�����Ŵ� */		//W H H  ���ֵĶ��嶼����
#define printf_info	printf
#define printf_ok(...)

#define DEBUG_PRINTF  

#ifdef DEBUG_PRINTF
#define debugprintf(...) printf (__VA_ARGS__)
#else	
#define debugprintf(...)  ((void)0)
#endif  //DEBUG_PRINTF

void User_Uart3SendChar(unsigned char ch);
void User_Uart3SendString(unsigned char *s);
void uart3_init(void);
void Uart3_IRQ(void);
void Uart3_NVIC_Config(void);

//void debug(const char* fmt,...);

#endif
