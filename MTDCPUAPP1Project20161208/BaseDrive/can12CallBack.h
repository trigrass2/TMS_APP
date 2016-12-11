#ifndef  __CAN12CALLBACK_H__
#define  __CAN12CALLBACK_H__

#include "stm32f4xx.h"
#include "canfestival.h"
#include "can1Slave.h"
#include "can2Slave.h"

UNS32 SetMTDTimeCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);			//���ñ���ʱ��
//UNS32 SetAccTempCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);		//���ÿյ��¶�
UNS32 SetCarNoCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);				/*���ó������*/
UNS32 SetWhellCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);				/*�����־��Ļص�����*/
UNS32 Can3SnedProDataCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex); /*����HMI���͵�ATOģʽ �Ȳ���  �ص�����*/
UNS32 Can3SnedACCSdrCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);	/*ͨ��can3���Ϳյ��¶�*/
UNS32 Can3SnedDelCmdCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);  /*can3���� ɾ�����ϼ�¼ ����*/
UNS32 Can3SendPISSdrCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);  /*can3���ͣ�PIS��sdr����*/
UNS32 MDCURxATCDICallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);  		/*����MDCU���͵�ATC��Ҫ��DI����*/ 
UNS32 MDCURxATCSbuFaultCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);   /*����MDCU���͵�ATC��Ҫ�����豸 ��������*/
UNS32 MDCURxATCVVVFProCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex); /*����MDCU ���͵�VVVFЭ������*/
UNS32 MDCURxATCBCUProCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex);	/*����MDCU ���͵�BCUЭ������*/

#endif 	//__CAN12CALLBACK_H__

