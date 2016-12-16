#include "CommunPro.h"
#include "string.h"
#include "malloc.h"

/*
����˵����
���ܣ�Ҫ���ĳλ������
����: 
	Data:��ڲ���,����ָ�롣
	GetPosit�� Ҫ�õ����ݵ�λ��
����ֵ�����صõ�ĳλ��ֵ
*/
u8 GetDataBit(u8 * Data,u8 GetPosit)
{
	u8 vale;
	vale=*Data <<(7-GetPosit);
	return vale>>7;
}



/*
	����˵����
���ܣ���10������ת��ΪBCD��
����: 
			���룺u8 Dec   ��ת����ʮ�������� ���������ֻ����0~99
����ֵ��  ת�����BCD��
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 DecToBcd(u8 Dec)
{
	if(Dec>=99)
	{
		Dec %=100;
	}
	return ((Dec/10)<<4) | ((Dec%10) & 0x0F);
}

/*
	����˵����
���ܣ���BCD��ת��Ϊ10������
����: 
			���룺u8 Bcd   ��ת��BCD��		ֻ��һ���ֽ�
����ֵ��  ת�����10������
˼·��ѹ��BCD��һ���ַ�����ʾ��ʮ�������ݷ�ΧΪ0 ~ 99,����Ϊ100
*/
u8 BcdToDec(u8 Bcd)
{
	return (Bcd>>4)*10 + (Bcd & 0x0f);
}


/*
����˵����
���ܣ������������������bcc��
����: 
	pData:��ڲ�������Ҫ����bcc�����ݡ�
	len�� pData�����ݳ��� (�ֽڵ�λ )
����ֵ���������ɵ�BCC��
*/
u8 GenerateBccChcek(void * pData,u8 len)
{
	u8 i=0;
	u8 BccCheckCode=0;
	u8 *BccData;
	BccData = mymalloc(SRAMIN,len);		//��̬�����ڴ� ,��keil�ﲻ��ʹ��BccData[len],����������ʾ���鳤��δ����
	memcpy(BccData,pData,len);
	for(i=0;i<len;i++)
	{
		BccCheckCode=BccCheckCode^BccData[i];
	};
	myfree(SRAMIN,BccData);			//�ͷ��ڴ�
	
	return BccCheckCode;
}













