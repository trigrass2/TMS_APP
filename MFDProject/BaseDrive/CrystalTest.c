#include "CrystalTest.h"

void System_Clk_Output_Init(void)				//PC9
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);//ʹ��GPIOC��ʱ��

	  GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_MCO);//��GPIOC_Pin_9��MCO����
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //ѡ��ܽź�
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz; //���ùܽŵ��ٶ�
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF; //���ùܽ�λ���ù���
    GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;//���ùܽ�λ�������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //��������	
    GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ���ܽ�

    RCC_MCO2Config(RCC_MCO2Source_SYSCLK,RCC_MCO2Div_2);//����GPIOC_Pin_9���ϵͳʱ��Ƶ�ʵĶ���Ƶ
}        

void HSE_Clk_Output_Init(void)				//PA8
{
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);//ʹ��GPIOA��ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; 	 //ѡ��ܽź�
    GPIO_InitStructure.GPIO_Speed =GPIO_Speed_100MHz;// ���ùܽŵ��ٶ�Ϊ100M
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF;  //���ùܽ�λ���ù���
    GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;//���ùܽ�λ�������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ���ܽ�

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_MCO);//��GPIOA_Pin_9��MCO����
	
    RCC_MCO1Config(RCC_MCO1Source_HSE,RCC_MCO2Div_1);//����GPIOA_Pin_8����ⲿ���پ���HSE��Ƶ��

}        
