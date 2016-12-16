#ifndef	 __ATOCOMPRO_H__
#define  __ATOCOMPRO_H__
	
#include "TMS_ComunDefine.h"

#define  ATO_RX_BUF_SIZE  		20  	//������� ���ݻ������ĳ���
#define  ATO_TX_BUF_SIZE  		20  	//���巢�����ݻ������ĳ���

/*���忪ʼ2�ֽڵ�����*/
#define  ATO_PAD1   						0x7E	//��·PAD1
#define  ATO_PAD2    						0x7E	//��·PAD2
#define  ATO_PAD3    						0x7E	//��·PAD3
#define  ATO_PAD4    						0x7E	//��·PAD4
#define  ATO_STARTFLAG					0x7E	//��ʼ��־
#define  ATO_ADDRESSL8					0x60	//ATO��ַ��8λ
#define  ATO_ADDRESSH8					0x01	//ATO��ַ��8λ
#define  ATO_CONTROLBYTE				0x13	//������
#define  ATO_ENDFLAG						0x7E	//������־

#define  ATO_SDR_COMMAND				0x20	//״̬����Ҫ��SDR �����֣��г����װ�á���������װ�ã�
#define  ATO_SD_COMMAND					0x30	//״̬������ӦSD  �����֣���������װ�á��г����װ�ã�


/*����������ֵ*/
#define  ATO_DataError  				-1			//���ܵ�ǰ4�ֽ��в�һ�µĴ���
#define  ATO_GetTimeoutError  	-2			//��������֮��20ms����û�н��յ�����
#define  ATO_FrameError					-3			//��⵽֡������żУ�����ʱ
#define  ATO_CRCError         	-4   		//BCC ���Ų����ϼƻ�ֵʱ
#define  ATO_GetError						-5			//��ʾ�������ݴ���
#define  ATO_TransException			-6			//��ʾ�����쳣


#endif //__ATOCOMPRO_H__
