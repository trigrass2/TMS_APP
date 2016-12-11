#include "stm32I2C.h"
#include "stm32f4xx.h"

void STM32_I2C_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	I2C_InitTypeDef  	I2C_InitStructure;
	
	RCC_APB1PeriphClockCmd(RTC_I2C_RCC, ENABLE);			//ʹ��I2Cʱ����
	RCC_AHB1PeriphClockCmd(RTC_I2C_GPIO_RCC, ENABLE);	//ʹ��I2C_SCL_GPIO_CLK and I2C_SDA_GPIO_CLK GPIOʱ����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//ʹ��ϵͳʱ��
	
	RCC_APB1PeriphResetCmd(RTC_I2C_RCC, ENABLE);	//��λ I2C IP 
		/* Release reset signal of I2C IP */
	RCC_APB1PeriphResetCmd(RTC_I2C_RCC, DISABLE);
	
	/*!< GPIO configuration */
	/* Connect PXx to I2C_SCL*/
	GPIO_PinAFConfig(RTC_I2C_PROT, RTC_I2C_SCL_GPIOSource, RTC_I2C_SCL_AF);
	/* Connect PXx to I2C_SDA*/
	GPIO_PinAFConfig(RTC_I2C_PROT, RTC_I2C_SDA_GPIOSource, RTC_I2C_SDA_AF);  
	
	/*!< Configure sEE_I2C pins: SCL */   
	GPIO_InitStructure.GPIO_Pin = RTC_I2C_SCL_GPIO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(RTC_I2C_PROT, &GPIO_InitStructure);
	
	/*!< Configure sEE_I2C pins: SDA */
	GPIO_InitStructure.GPIO_Pin = RTC_I2C_SDA_GPIO;
	GPIO_Init(RTC_I2C_PROT, &GPIO_InitStructure);
	
	
		/*!< I2C configuration */
	/* I2C configuration */
//	I2C_DeInit(RTC_I2C);	//��λʹ��i2c���ر�i2c��λ
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;		//ack enable
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
	
	/* I2C Peripheral Enable */
	I2C_Cmd(RTC_I2C, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(RTC_I2C, &I2C_InitStructure);
    
	/* ����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);		//W H H
}

void STM32_I2C_WriteByte(uint8_t* pBuffer, uint8_t WriteAddr)
{
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����
	/*����������*/
	
		while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY));		//W H H

		I2C_AcknowledgeConfig(RTC_I2C, ENABLE);		/*����1�ֽ�1Ӧ��ģʽ*/		//W H H
	
		I2C_GenerateSTART(RTC_I2C,ENABLE);		//������ʼ����
	
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));	//�ȴ�ACK/*EV5,��ģʽ*/
		
		I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7,I2C_Direction_Transmitter);	//���豸�����豸��ַ
		
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));//�ȴ�ACK
		
		I2C_SendData(RTC_I2C, WriteAddr);//����д��ļĴ�����ַ
	
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//�ȴ�ACK	/*�����ѷ���*/
	
		I2C_SendData(RTC_I2C, *pBuffer);	//��������
		
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));//�������
	
		I2C_GenerateSTOP(RTC_I2C, ENABLE);	//���������ź�
		
//	OS_CRITICAL_EXIT();	//�˳��ٽ���		 /*����������*/

}

uint8_t STM32_I2C_ReadByte(uint8_t ReadAddr)
{  
		unsigned char temp; 	
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����
	/*����������*/
	
		while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY));	 /*�ȴ�IIC���߿���*/
	
		/*����1�ֽ�1Ӧ��ģʽ*/
		I2C_AcknowledgeConfig(RTC_I2C, ENABLE);	  //W H H
	
  	I2C_GenerateSTART(RTC_I2C, ENABLE);	/* ������ʼλ */
	
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));  /*EV5,��ģʽ*/
	
  	I2C_Send7bitAddress(RTC_I2C, I2C1_SLAVE_ADDRESS7, I2C_Direction_Transmitter);  /*����������ַ(д)*/
	
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));/* Test on EV6 and clear it */
	
  	I2C_Cmd(RTC_I2C, ENABLE);			/*W H H  ���ﲻһ��  ע��*/
	
  	I2C_SendData(RTC_I2C, ReadAddr); 	//���Ͷ�ȡ�ļĴ�����ַ
	
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	/*�����ѷ���*/
	
  	I2C_GenerateSTART(RTC_I2C, ENABLE);
	
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));
		
  	I2C_Send7bitAddress(RTC_I2C, I2C1_SLAVE_ADDRESS7, I2C_Direction_Receiver);
		
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) ;
		
    I2C_AcknowledgeConfig(RTC_I2C, DISABLE);	// ���һλ��Ҫ�ر�Ӧ���
		
    I2C_GenerateSTOP(RTC_I2C, ENABLE);			//����ֹͣλ
		
    while(!(I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))); 
		
    temp = I2C_ReceiveData(RTC_I2C);
		
  	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);	//�ٴ�����Ӧ��ģʽ
		
//	OS_CRITICAL_EXIT();	//�˳��ٽ���		 /*����������*/
		
    return temp;
}

void STM32_I2C_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY));		

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);		//W H H
	
	/* ������ʼλ */
  I2C_GenerateSTART(RTC_I2C, ENABLE);
	
  while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
  I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
 
	while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(RTC_I2C, WriteAddr);	//����д��ַ
  
	while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/

	while(NumByteToWrite--)
	{
		/* дһ���ֽ�*/
	  I2C_SendData(RTC_I2C, *pBuffer); 
		
	  while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
		
		pBuffer++;
	}
	/* ֹͣλ*/
	I2C_GenerateSTOP(RTC_I2C, ENABLE);

//	OS_CRITICAL_EXIT();	//�˳��ٽ���
}


void STM32_I2C_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint8_t NumByteToWrite)
{
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����

	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY));		
	
	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);/*����1�ֽ�1Ӧ��ģʽ*/	//W H H 
	
  I2C_GenerateSTART(RTC_I2C, ENABLE);/* ������ʼλ */
  
  while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

	/*����������ַ(д)*/
  I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
  
  while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_Cmd(RTC_I2C, ENABLE);	//i2cʹ��
	
	I2C_SendData(RTC_I2C, ReadAddr);	//���Ͷ��ĵ�ַ
	
  while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
    
	/*��ʼλ*/
	I2C_GenerateSTART(RTC_I2C, ENABLE);
	
	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));

	I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7, I2C_Direction_Receiver);	/*����������ַ(��)24C01*/
	
	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	while(NumByteToWrite)
	{
		if(1==NumByteToWrite)
		{
			I2C_AcknowledgeConfig(RTC_I2C, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
			
    	I2C_GenerateSTOP(RTC_I2C, ENABLE);			//����ֹͣλ
		}

		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)); /*W H H ��һ���ĵط� */
		
		*pBuffer = I2C_ReceiveData(RTC_I2C);
		pBuffer++;
		NumByteToWrite--;
	}
 
	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);  //�ٴ�����Ӧ��ģʽ
//	OS_CRITICAL_EXIT();	//�˳��ٽ���
}






