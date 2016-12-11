#ifndef __OLED_MENU_H__
#define __OLED_MENU_H__

#include "stm32f4xx.h"

#define MC1_CAR_TYPE	"MC1(1)"
#define T_CAR_TYPE		"T(4)"
#define M_CAR_TYPE		"M(3)"
#define T1_CAR_TYPE		"T1(5)"
#define T2_CAR_TYPE		"T2(6)"
#define MC2_CAR_TYPE	"MC2(2)"


typedef struct 
{ 
 u8 CurrentIndex;//��ǰ״̬������ 
 u8 Up; 		//����(UP) ��ʱת���״̬������
 u8 Down; 	//����(Down)��ʱת���״̬������
 u8 Enter; 	//ȷ��(OK����)��ʱת���״̬������
 u8 Exit; 	//�˳�(set����) ��ʱת���״̬������
 void (*CurrentOperation)(void); //��ǰ״̬Ӧ��ִ�еĲ��� 
} Menu_table; 

extern Menu_table  MenuTable[5];
extern void (*CurrentOperationIndex)(void);  //���庯��ָ��
extern const char CarTypeArr[6][10];

void  MainMenu(void);
void  ChooseSetMeun(void);
void  SetCarMenu(void);

void  KeySetMenu(void);

#endif //__OLED_MENU_H__
