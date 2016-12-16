#ifndef __COMMUNPRO_H__
#define	__COMMUNPRO_H__

#include "TMS_ComunDefine.h"
/*
����˵����
���ܣ�Ҫ���ĳλ������ֵ
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
u8 GetDataBit(u8 * Data,u8 GetPosit);

/*
	����˵����
���ܣ���10������ת��ΪBCD��
����: 
			���룺u8 Dec   ��ת����ʮ��������
����ֵ��  ת�����BCD��
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 DecToBcd(u8 Dec);

/*
	����˵����
���ܣ���BCD��ת��Ϊ10������
����: 
			���룺u8 Bcd   ��ת��BCD��
����ֵ��  ת�����10������
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 BcdToDec(u8 Bcd);

/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
����ֵ���������ɵ�BCC��
*/
u8 GenerateBccChcek(void * pData,u8 len);

#endif //__COMMUNPRO_H__

