/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

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
#ifdef USE_XENO
//#define eprintf(...) if(0){}
#define eprintf(...)
#elif defined USE_RTAI
#define eprintf(...)
#else
#define eprintf(...) printf (__VA_ARGS__)
#endif

#include "canfestival.h"
#

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


/*
#define CAN_FIFO_LENGTH 100

#define DECLARE_A_CAN_FIFO \
static Message FIFO[CAN_FIFO_LENGTH];\
static int FIFO_First = 0;\
static int FIFO_Last = 0;\
\
static void PutInFIFO(Message *m)\
{\
	FIFO[FIFO_Last++] = *m;\
	FIFO_Last %= CAN_FIFO_LENGTH;\
}\
\
static void GetFromFIFO(Message *m)\
{\
	*m = FIFO[FIFO_First++];\
	FIFO_First %= CAN_FIFO_LENGTH;\
}\
\
static void TransmitMessage(CO_Data* d, UNS32 id)\
{\
	Message m;\
	GetFromFIFO(&m);\
	canDispatch(d, &m);\
}
*/
