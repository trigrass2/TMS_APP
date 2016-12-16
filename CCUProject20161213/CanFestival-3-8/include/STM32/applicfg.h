/*
This file is part of CanFestival, a library implementing CanOpen Stack.

Copyright (C): Edouard TISSERANT and Francis DUPIN
AT91 Port: Peter CHRISTEN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __APPLICFG_AVR__
#define __APPLICFG_AVR__

#include <string.h>
#include <stdio.h>

// Integers
#define INTEGER8 	signed char
#define INTEGER16 signed short
#define INTEGER24 signed long
#define INTEGER32 signed long
#define INTEGER40 signed long long
#define INTEGER48 signed long long
#define INTEGER56 signed long long
#define INTEGER64 signed long long

// Unsigned integers
#define UNS8   unsigned char
#define UNS16  unsigned short
#define UNS32  unsigned long
#define UNS24  unsigned long
#define UNS40  unsigned long long
#define UNS48  unsigned long long
#define UNS56  unsigned long long
#define UNS64  unsigned long long

// Reals
#define REAL32 float
#define REAL64 double

// Reals
#define REAL32 	float
#define REAL64 double

#include "can.h"

//W H H ����״̬�������ж��Ƿ���ܵ�������
/*canfestival �����������ĵĶ���*/

#define CCU1_NODEID  				1		//���߿�����1���ڵ�� 1
#define CCU2_NODEID	 				2		//���߿�����2���ڵ�� 2	
#define MC1_MTD_NODEID	 		3		//MC1�����װ�ã��ڵ�� 3
#define T_MTD_NODEID   	 		4		//T�����װ�ã��ڵ��4
#define M_MTD_NODEID		 		5		//M�����װ�ã� �ڵ��5
#define T1_MTD_NODEID				6		//T1�����װ�ã��ڵ��6
#define T2_MTD_NODEID   		7		//T2�����װ�ã��ڵ��7
#define MC2_MTD_NODEID   		8		//MC2�����װ�ã��ڵ��8
#define DDU1_NODEID					9   //�����ʾ��1���ڵ��9   (˾����ʾ��Ԫ)
#define DDU2_NODEID   			10	//�����ʾ��2���ڵ��10
#define ERM1_NODEID					11	//�¼���¼��1: �ڵ��11
#define ERM2_NODEID					12  //�¼���¼��2���ڵ��12


typedef  enum  { 
	NoRxHeart = 0,
	RxHeart = 1, 
}IsNoReceHeart;					//��ʾ��ǰ�����������ڵ�״̬

typedef  enum  { 
	InPowerInitState = 0,
	InOperatState = 1, 
}CurReceHeartState;			//��ʾ��ǰ�����������ڵ�״̬

typedef enum{
	NoRxCanMesState = 0,
	RxCanMesState =1,
	RxCanNodeGuardMesState =2,
	RxCanHeartMesState =3,
}RxCanMessageState;		//��ʾ��ǰ���ܵ�can��Ϣ���ڵ�״̬


typedef enum{ 						//������ʾcan1 �ں�can2�ڽ����״̬�������л���can1��can2��״̬ һ��Ҫ����
	ChooseOperate=1,
	ChooseNotOperate=2,
}CCUCANState;

typedef enum{
	NoUseState=0,
	UseState =1,
}CurrentUserState;

typedef enum{
	NoEmcy=0,											//û�н�����Ϣ
	OnePointFaultNotSwitch =1, 		//������ϲ����л�
	OnePointFaultSwitch=2,				//������Ͽ��л�
	MainLineFaultNotSwitch =3,		//�����߹��ϲ����л�
	MainLineFaultSwitch =4,	  		//�����߹��Ͽ��л�
	MainControlFaultNotSwitch=5, //����1���������л�
	MainControlFaultSwitch=6, 	 //����1�������л�
}EmcyMsgState;

extern CCUCANState   CCU1SwitchState;		//CCU1 ��������CCU2����		,��ʾ�豸
extern CCUCANState   CCU2SwitchState;

extern CurrentUserState CurrentUseCAN1;	//��ʾ��ǰʹ��can1������can2���磬��ʾͨ��
extern CurrentUserState CurrentUseCAN2;	

extern UNS8 CAN1_Heart;									//��ʾccu1 ����ccu2 ��can1���յ������豸����������ʾ�Լ�������ͨ�ţ�can1�ڵ�����
extern UNS8 CAN2_Heart;			

extern  UNS8  ChooseBoard;			//���ȫ�ֱ�������������ť��ֵ���Ӷ�ѡ�����
#define CCU1  ChooseBoard				//��ʾΪccu1 
#define CCU2  ChooseBoard				//��ʾΪccu2


extern IsNoReceHeart  g_RxCAN1HeartFlag[15];								  //��Ÿ����豸�������ı�־���±���ǽڵ��ID�ţ����±�Ϊ0������û��ʹ�á�
extern IsNoReceHeart	g_RxCAN2HeartFlag[15];

//extern CurReceHeartState  g_RxCAN1HeartStateFlag[15];					//��ŵ�ǰ��������ʱ�����豸���ڵ�״̬�����ϵ�ճ�ʼ����ʱ�򣬻��ǽ��������ӵ�ʱ��	
//extern CurReceHeartState  g_RxCAN2HeartStateFlag[15];

extern RxCanMessageState  g_RxCAN1MesStateFlag[15];						//��ŵ�ǰ��վ�ӵ���վ��can��Ϣ ������״̬ 
extern RxCanMessageState	g_RxCAN2MesStateFlag[15];

//extern EmcyMsgState 	CAN1EmcyState;		//��can2���߹��ϣ���can1���߷��ͽ�����Ϣ
//extern EmcyMsgState 	CAN2EmcyState;		//��can1���߹��ϣ���can2���߷��ͽ�����Ϣ					


#define CANFESTIVAL_DEBUG_MSG(num, str, val) 

//#define CANFESTIVAL_DEBUG_MSG(num, str, val) \
//  {unsigned long value = val;\
//	 printf("  %s 0x%lX\r\n", str, value); \
//	}
  // printf(" 0x%X %s 0x%lX\r\n",num, str, value); \



/*
#define CANFESTIVAL_DEBUG_MSG(num, str, val)\
  {unsigned long value = val;\
   printf("%s(%d) : 0x%X %s 0x%lX\r\n",__FILE__, __LINE__,num, str, value); \
  }
*/

/* Definition of MSG_ERR */
#define MSG_ERR(num, str, val) CANFESTIVAL_DEBUG_MSG(num, str, val)

/* Definition of MSG_WAR */
// ---------------------
#define MSG_WAR(num, str, val) CANFESTIVAL_DEBUG_MSG(num, str, val)

typedef void* CAN_HANDLE;

typedef void* CAN_PORT;

#endif


