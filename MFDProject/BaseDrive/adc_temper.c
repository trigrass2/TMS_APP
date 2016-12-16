#include "adc_temper.h"
//#include "Gpio.h"

__IO uint16_t ADC3ConvertedValue = 0;
__IO uint32_t ADC3ConvertedVoltage = 0;

/**
  * @brief  ADC3 channel06 with DMA configuration
  * @param  None
  * @retval None
  */
void ADC1_CH16_Config(void)
{
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
//    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
    
    /* Enable ADC1,  and GPIO clocks ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    
	  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);  
	  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);  
	  ADC_TempSensorVrefintCmd(ENABLE);
	

    /* Configure ADC3 Channel6 pin as analog input ******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    /* ADC Common Init **********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;			 //����ģʽ
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;		 //ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		//��ʹ��DMA
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);			//ADCͨ�ó�ʼ��
    
    /* ADC1 Init ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;			//12λ�ֱ���
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;								//��ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;					//��ʹ������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//����ͨ���ⲿ��������ѡ��,�˴�δ�õ�
    //ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;			//�����Ҷ���
    ADC_InitStructure.ADC_NbrOfConversion = 1;									//1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC_Init(ADC1, &ADC_InitStructure);													//ADC1��ʼ��
    
    /* ADC1 regular channel6 configuration *************************************/
   // ADC_RegularChannelConfig(ADC3, ADC_Channel_6, 1, ADC_SampleTime_3Cycles);
    
    /* Enable DMA request after last transfer (Single-ADC mode) */
   // ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);
    
    /* Enable ADC3 DMA */
   // ADC_DMACmd(ADC3, ENABLE);
  //  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles );	
	
		ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_480Cycles );		//����ͨ�������� ͨ��1  480������,��߲���ʱ�������߾�ȷ��	
		ADC_TempSensorVrefintCmd(ENABLE);  //�����¶ȴ�����
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);
}

//���ADCֵ
//ch:ͨ��ֵ 0~16
//����ֵ:ת�����
u16 Get_AdcValue(void)   
{
	 //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	//ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	
  
	ADC_SoftwareStartConv(ADC1);		//ʹ�����ת������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	          //�������һ��ADC1�������ת�����
}

//��ȡͨ��ch��ת��ֵ��ȡnumber��,Ȼ��ƽ�� 
//ch:ͨ�����
//times:��ȡ����
//����ֵ:ͨ��ch��times��ת�����ƽ��ֵ
u32 Get_Adc_Average(u8 number)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<number;t++)
	{
		temp_val+=Get_AdcValue();                     //��ADCƽ��ֵ
		delay_ms(5);
	}
	return temp_val/number;
} 

//�õ��¶�ֵ
//����ֵ:�¶�ֵ(������100��,��λ:��.)
short Get_Temprate(void)
{
  u32 AdcValue;
  double temperate;
	short result;
  AdcValue=Get_Adc_Average(20);  
	printf("\r\nADCֵ��:%d\r\n",AdcValue);   
  temperate=(float)AdcValue*(3.3/4096);      
  temperate=(temperate-0.76)/0.0025+25;    
  printf("\r\nоƬ�ڲ��¶���:%f C\r\n",temperate);	
	result = temperate*100; //����100��
	return result;
}

void ADC_Temper_Test(void)
{
 // LEDGpio_Init();
	ADC1_CH16_Config();
   
    while (1)
    {
			Get_Temprate();	
		//	LED_Turn (0);
			delay_ms(1000);

    }
}

