#include "MTD_CCU_CanAddres.h"

void MTDCCU_CanAdd_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ����GPIO ��ʱ��  */
	RCC_AHB1PeriphClockCmd(MTDCCU_CANADD0_RCC ,ENABLE);		//��Ϊ���е����Ŷ�ʹ�õ�CPIOF
			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	//GPIO_PuPd_UP;				//W H H 

	GPIO_InitStructure.GPIO_Pin= MTDCCU_CANADD0_GPIO_PIN | MTDCCU_CANADD1_GPIO_PIN | MTDCCU_CANADD2_GPIO_PIN | MTDCCU_CANADD3_GPIO_PIN 
					| MTDCCU_CANADD4_GPIO_PIN |MTDCCU_CANADD5_GPIO_PIN | MTDCCU_CANADD6_GPIO_PIN | MTDCCU_CANADD7_GPIO_PIN ;					// ����Ϊ0x303f
	
	GPIO_Init(MTDCCU_CANADD0_GPIO, &GPIO_InitStructure);		//������GPIOF
}


/*��·ͼ�� s2��ʾ��λ���ݣ�s1��ʾ��λ����*/
uint8_t MTDCCU_CanAddVal(void)
{
	uint8_t  vale=0;
	vale = (MTDCCU_CANADD4_READ() <<7) |(MTDCCU_CANADD5_READ() <<6) |(MTDCCU_CANADD6_READ() <<5) | (MTDCCU_CANADD7_READ() <<4)
				|(MTDCCU_CANADD0_READ() <<3) | (MTDCCU_CANADD1_READ() <<2)| (MTDCCU_CANADD2_READ() <<1)|(MTDCCU_CANADD3_READ() <<0)	  ;
	
	return ~vale;		//�����������ݰ�λȡ��
	/*
	����
	{
		uint16_t ReadValue;
		ReadValue = GPIO_ReadInputData(GPIOF);
		vale = (uint8_t)( (ReadValue >> 6 ) | (ReadValue & 0x3f) ) & 0xff ;
	}
	return vale;
	*/

}

