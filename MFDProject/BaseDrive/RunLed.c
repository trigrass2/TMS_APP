#include "RunLed.h"


void RunLedGpio_Init(void)
{
//    uint8_t i;
    GPIO_InitTypeDef  GPIO_InitStructure;
     
		RCC_AHB1PeriphClockCmd( RUNLED_RCC , ENABLE); 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

		GPIO_InitStructure.GPIO_Pin = RUNLED_GPIO_PIN;
		GPIO_Init(RUNLED_GPIO, &GPIO_InitStructure);
			
		RUNLED_ONOFF(0);				//�м���
//		RUNLED_ONOFF(1);			//  W H H ccuΪ�ߵ�ƽϨ�𣬵͵�ƽ����

}


void  RunLed_On (void)
{	/*ccu Ϊ�͵�ƽ����*/
//    GPIO_ResetBits(RUNLED_GPIO,RUNLED_GPIO_PIN);                                     //����ָ��LED��       

		/*�м���Ϊ�ߵ�ƽ����*/
	    GPIO_SetBits(RUNLED_GPIO,RUNLED_GPIO_PIN);                 //Ϩ��ָ��LED��  
}

void  RunLed_Off (void )
{
		/*ccuΪ�ߵ�ƽϨ��*/
//    GPIO_SetBits(RUNLED_GPIO,RUNLED_GPIO_PIN);                 //Ϩ��ָ��LED��       

			/*�м���Ϊ�͵�ƽ����*/
	    GPIO_ResetBits(RUNLED_GPIO,RUNLED_GPIO_PIN);                 //Ϩ��ָ��LED��  
}


void  RunLed_Turn (void)
{
   
	GPIO_WriteBit(RUNLED_GPIO, RUNLED_GPIO_PIN,(BitAction)(1-(GPIO_ReadOutputDataBit(RUNLED_GPIO, RUNLED_GPIO_PIN))));                                            //��ϲ�д��                  
}

