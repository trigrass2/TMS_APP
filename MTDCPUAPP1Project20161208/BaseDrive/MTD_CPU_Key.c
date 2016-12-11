#include "MTD_CPU_Key.h"
#include "delay.h"

MTDCPU_KeyGpio_Info MTDCPU_Key_info[MTDCPU_KEY_NUMBER] = {
    {MTDCPU_KEY_S1_RCC,MTDCPU_KEY_S1_GPIO,MTDCPU_KEY_S1_GPIO_PIN},
    {MTDCPU_KEY_S2_RCC,MTDCPU_KEY_S2_GPIO,MTDCPU_KEY_S2_GPIO_PIN},
    {MTDCPU_KEY_S3_RCC,MTDCPU_KEY_S3_GPIO,MTDCPU_KEY_S3_GPIO_PIN},
    {MTDCPU_KEY_S4_RCC,MTDCPU_KEY_S4_GPIO,MTDCPU_KEY_S4_GPIO_PIN},
};

void 	MTDCPU_KEYGpio_Init(void)
{
	uint8_t i;
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	for(i=0;i<MTDCPU_KEY_NUMBER;i++)
	{
		RCC_AHB1PeriphClockCmd( MTDCPU_Key_info[i].rcc , ENABLE); 
		GPIO_InitStructure.GPIO_Pin = MTDCPU_Key_info[i].pin;
		GPIO_Init(MTDCPU_Key_info[i].gpio, &GPIO_InitStructure);
	}
}


//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��K1����
//2��K2����
//3��K3���� 
//4��k4���� 

//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3>KEY4!!
uint8_t		MTDCPU_KEY_Scan(uint8_t mode)  		//����ɨ�躯��	
{
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(MTDCPU_KEY_S1_UPDOWM()==0||MTDCPU_KEY_S2_UPDOWM()==0||MTDCPU_KEY_S3_UPDOWM()==0||MTDCPU_KEY_S4_UPDOWM()==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(MTDCPU_KEY_S1_UPDOWM() == 0)					return 1;
		else if(MTDCPU_KEY_S2_UPDOWM() == 0)		return 2;
		else if(MTDCPU_KEY_S3_UPDOWM() == 0)		return 3;
		else if(MTDCPU_KEY_S4_UPDOWM() == 0)		return 4;
	}
	else if(MTDCPU_KEY_S1_UPDOWM()==1&&MTDCPU_KEY_S2_UPDOWM()==1&&MTDCPU_KEY_S3_UPDOWM()==1&&MTDCPU_KEY_S4_UPDOWM()==1)
		key_up=1; 	    
 	return 0;// �ް�������
}


