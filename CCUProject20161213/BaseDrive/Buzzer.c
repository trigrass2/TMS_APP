#include "Buzzer.h"

void Buzzer_Func_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(BUZZER_RCC, ENABLE);//ʹ��GPIOAʱ��
  
  //��ʼ����������Ӧ����GPIOA8
  GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(BUZZER_GPIO, &GPIO_InitStructure);//��ʼ��GPIOA
	
//  GPIO_ResetBits(BUZZER_GPIO,BUZZER_GPIO_PIN);  //��������Ӧ����GPIOA8���ͣ� 
		Buzzer_Func_Off();
}

void Buzzer_Func_On(void)   //�������ߵ�ƽ��ͨ
{
	BUZZER_ONOFF(1);
}


void Buzzer_Func_Off(void)   //�������ߵ�ƽ��ͨ
{
	BUZZER_ONOFF(0);
}

