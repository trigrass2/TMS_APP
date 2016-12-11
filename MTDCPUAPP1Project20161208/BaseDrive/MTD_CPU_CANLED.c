#include "MTD_CPU_CANLED.h"

MTD_CPU_CANLED_LedGpio_Info MTD_CPU_CANLED_info[MTDCPU_CANLED_NUMBER]={
    {CAN1_TX_RCC,CAN1_TX_GPIO,CAN1_TX_GPIO_PIN},
    {CAN1_RX_RCC,CAN1_RX_GPIO,CAN1_RX_GPIO_PIN},
    {CAN2_TX_RCC,CAN2_TX_GPIO,CAN2_TX_GPIO_PIN},
    {CAN2_RX_RCC,CAN2_RX_GPIO,CAN2_RX_GPIO_PIN},
	
};



/******************************************************************************************
*�������ƣ�void MTD_CPU_CANLED_LEDGpio_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����LED��GPIO��ʼ��
*******************************************************************************************/
void 	MTD_CPU_CANLED_Gpio_Init(void)
{
    uint8_t i;
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    for(i=0;i<MTDCPU_CANLED_NUMBER;i++)                                    
    {
        RCC_AHB1PeriphClockCmd( MTD_CPU_CANLED_info[i].rcc , ENABLE); 
        GPIO_InitStructure.GPIO_Pin = MTD_CPU_CANLED_info[i].pin;
        GPIO_Init(MTD_CPU_CANLED_info[i].gpio, &GPIO_InitStructure);
			
        GPIO_WriteBit(MTD_CPU_CANLED_info[i].gpio,MTD_CPU_CANLED_info[i].pin,Bit_RESET);	//LED All Off
    }
}

/********************************************************************************************
*�������ƣ�void MTD_CPU_CANLED_LED_On(uint8_t i)
*��ڲ�������
*���ڲ�������
*����˵��������ָ��LED��
*******************************************************************************************/
void  MTD_CPU_CANLED_On(uint8_t i)
{
  //GPIO_SetBits(MTD_CPU_CANLED_info[i].gpio,MTD_CPU_CANLED_info[i].pin);          //����ָ��LED��  
	GPIO_ResetBits(MTD_CPU_CANLED_info[i].gpio,MTD_CPU_CANLED_info[i].pin);    	   //����ָ��LED�� 	 2���忨�͵�ƽ����	
}

/********************************************************************************************
*�������ƣ�void MTD_CPU_CANLED_LED_Off(uint8_t i)
*��ڲ�������
*���ڲ�������
*����˵����Ϩ��ָ��LED��
*******************************************************************************************/
void  MTD_CPU_CANLED_Off(uint8_t i)
{
		//GPIO_ResetBits(MTD_CPU_CANLED_info[i].gpio,MTD_CPU_CANLED_info[i].pin);    	   //�ر�ָ��LED�� 
		GPIO_SetBits(MTD_CPU_CANLED_info[i].gpio,MTD_CPU_CANLED_info[i].pin);          //�ر�ָ��LED��  	 2���忨�ߵ�ƽϨ��	
}

/********************************************************************************************
*�������ƣ�void MTD_CPU_CANLED_LED_Turn(uint16_t GPIO_Pin)
*��ڲ�������
*���ڲ�������
*����˵������תָ��LED��״̬
*******************************************************************************************/
void  MTD_CPU_CANLED_Turn(uint8_t i)
{
		GPIO_WriteBit(MTD_CPU_CANLED_info[i].gpio, MTD_CPU_CANLED_info[i].pin,(BitAction)(1-(GPIO_ReadOutputDataBit(MTD_CPU_CANLED_info[i].gpio, MTD_CPU_CANLED_info[i].pin))));                 
}

