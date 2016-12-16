#include "MTD_MFD_CANLed.h"


MTD_MFD_CANLED_LedGpio_Info MTD_MFD_CANLED_info[MTDMFD_CANLED_NUMBER]={
    {CAN2_TX_RCC,CAN2_TX_GPIO,CAN2_TX_GPIO_PIN},
    {CAN2_RX_RCC,CAN2_RX_GPIO,CAN2_RX_GPIO_PIN},
	
};



/******************************************************************************************
*�������ƣ�void MTD_MFD_CANLED_LEDGpio_Init(void)
*
*��ڲ�������
*
*���ڲ�������
*
*����˵����LED��GPIO��ʼ��
*******************************************************************************************/
void 	MTD_MFD_CANLED_Gpio_Init(void)
{
    uint8_t i;
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

    for(i=0;i<MTDMFD_CANLED_NUMBER;i++)                                    
    {
        RCC_AHB1PeriphClockCmd( MTD_MFD_CANLED_info[i].rcc , ENABLE); 
        GPIO_InitStructure.GPIO_Pin = MTD_MFD_CANLED_info[i].pin;
        GPIO_Init(MTD_MFD_CANLED_info[i].gpio, &GPIO_InitStructure);
			
        GPIO_WriteBit(MTD_MFD_CANLED_info[i].gpio,MTD_MFD_CANLED_info[i].pin,Bit_RESET);	//LED All Off
    }
}

/********************************************************************************************
*�������ƣ�void MTD_MFD_CANLED_LED_On(uint8_t i)
*��ڲ�������
*���ڲ�������
*����˵��������ָ��LED��
*******************************************************************************************/
void  MTD_MFD_CANLED_On(uint8_t i)
{
    GPIO_SetBits(MTD_MFD_CANLED_info[i].gpio,MTD_MFD_CANLED_info[i].pin);          //����ָ��LED��  		
}

/********************************************************************************************
*�������ƣ�void MTD_MFD_CANLED_LED_Off(uint8_t i)
*��ڲ�������
*���ڲ�������
*����˵����Ϩ��ָ��LED��
*******************************************************************************************/
void  MTD_MFD_CANLED_Off(uint8_t i)
{
		GPIO_ResetBits(MTD_MFD_CANLED_info[i].gpio,MTD_MFD_CANLED_info[i].pin);    	   //�ر�ָ��LED��          
}

/********************************************************************************************
*�������ƣ�void MTD_MFD_CANLED_LED_Turn(uint16_t GPIO_Pin)
*��ڲ�������
*���ڲ�������
*����˵������תָ��LED��״̬
*******************************************************************************************/
void  MTD_MFD_CANLED_Turn(uint8_t i)
{
		GPIO_WriteBit(MTD_MFD_CANLED_info[i].gpio, MTD_MFD_CANLED_info[i].pin,(BitAction)(1-(GPIO_ReadOutputDataBit(MTD_MFD_CANLED_info[i].gpio, MTD_MFD_CANLED_info[i].pin))));                 
}

