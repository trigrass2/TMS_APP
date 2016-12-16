#include "stm32I2C.h"
#include "stm32f4xx.h"
#include "os.h"
#include "Uart.h"

/*
STM32 I2C�ӿ�����ϵ�һЩȱ�ݺ���αܿ���Щȱ�ݵ��Ƽ�����ģ�ͣ�
��1����I2C���ж����ȼ����������
��2���ѷ��Ͷ���2���ֽڵķ�������շ�װ������DMA�շ��ĺ��������Ѷ�ĳI2C�豸���պͷ���һ���ֽڵĺ���������װΪһ��POLLING ����ѯ��������
��3����ѰַĳһI2C DEVICEʱҪ��CHECK I2C BUS �Ƿ�BUSY�����æ����ȴ�ָ��ʱ�䣬�������æ��˵��I2C BUS ����
��ԭ��99.9%���������ǵ�I2Cͨ��ʱ�򲢲�ʮ������I2C��Լ��������������װ��I2Cͨ��ģ��û�м��Ϸ��ش��루����ָ����룩����
��ʱҪ����һ��ר�ŵ�����֪ͨ I2C BUS�ϵ�����device�������ǽ�����ǰ�ڲ��Ĺ���������׼���ã������ˣ����·���������������ҵ�I2Cģ���FUN ��Ƭ��

*/

__IO uint32_t I2C_Timeout = I2C_TIMEOUT;

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
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
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

I2C_Status STM32_I2C_WriteByte(uint8_t* pBuffer, uint8_t WriteAddr)
{
		
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����
	/*����������*/
		I2C_Timeout = I2C_TIMEOUT;
		while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY))		//W H H
		{
			if((I2C_Timeout--) ==0)
			{
				#ifdef DEBUG_PRINTF
					debugprintf("Wirte I2C Bus Busy!\r\n");
				#endif
				return I2C_FAIL;
		  }
		}	

		I2C_AcknowledgeConfig(RTC_I2C, ENABLE);		/*����1�ֽ�1Ӧ��ģʽ*/		//W H H
	
		I2C_GenerateSTART(RTC_I2C,ENABLE);				//������ʼ����
	
		I2C_Timeout = I2C_TIMEOUT;
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT))	//�ȴ�ACK/*EV5,��ģʽ*/
		{
			if((I2C_Timeout--) ==0)
			{
				#ifdef DEBUG_PRINTF
					debugprintf("Write I2C master mode error!\r\n");
				#endif				
				return I2C_FAIL;
		  }
		}
			
		I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7,I2C_Direction_Transmitter);	//���豸�����豸��ַ
		
		I2C_Timeout = I2C_TIMEOUT;
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))//�ȴ�ACK			//W H H ���û��i2c�豸��Ӧ������Ῠ��������
		{
			if((I2C_Timeout--) ==0)
			{
				#ifdef DEBUG_PRINTF
					debugprintf("Write I2C master transmit mode error!\r\n");
				#endif					
				return I2C_FAIL;
		  }		
		}
		
		I2C_SendData(RTC_I2C, WriteAddr);//����д��ļĴ�����ַ
		
		I2C_Timeout = I2C_TIMEOUT;
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	//�ȴ�ACK	/*�����ѷ���*/
		{
			if((I2C_Timeout--) ==0)
			{
				#ifdef DEBUG_PRINTF
					debugprintf("Write I2C Master Transmit Error!\r\n");
				#endif					
				return I2C_FAIL;
		  }				
		}
	
		I2C_SendData(RTC_I2C, *pBuffer);	//��������
		
		I2C_Timeout = I2C_TIMEOUT;
		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))	//�������
		{
			if((I2C_Timeout--) ==0)
			{
				return I2C_FAIL;
		  }				
		}
	
		I2C_GenerateSTOP(RTC_I2C, ENABLE);	//���������ź�
		
		//	OS_CRITICAL_EXIT();	//�˳��ٽ���		 /*����������*/

		return I2C_OK;
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
	
  	I2C_Cmd(RTC_I2C, ENABLE);			/*W H H  ���ﲻһ��  ע��*/ /* Clear EV6 by setting again the PE bit */
	
  	I2C_SendData(RTC_I2C, ReadAddr); 	//���Ͷ�ȡ�ļĴ�����ַ
	
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));	/*�����ѷ���*/
	
  	I2C_GenerateSTART(RTC_I2C, ENABLE);
	
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT));
		
  	I2C_Send7bitAddress(RTC_I2C, I2C1_SLAVE_ADDRESS7, I2C_Direction_Receiver);
		
  	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
		
    I2C_AcknowledgeConfig(RTC_I2C, DISABLE);	// ���һλ��Ҫ�ر�Ӧ���
		
    I2C_GenerateSTOP(RTC_I2C, ENABLE);			//����ֹͣλ
		
    while(!(I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED))); 
		
    temp = I2C_ReceiveData(RTC_I2C);
		
  	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);	//�ٴ�����Ӧ��ģʽ
		
//	OS_CRITICAL_EXIT();	//�˳��ٽ���		 /*����������*/
		
    return temp;
}

/*
	��i2c��д������
*/
I2C_Status STM32_I2C_BufferWrite(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
//	CPU_SR_ALLOC();			//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����

	I2C_Timeout = I2C_TIMEOUT;
	/*�ȴ�IIC���߿���*/
	while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY))
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Writing to the I2C bus is busy!\r\n");
			#endif			
			return I2C_FAIL;
		}		
	}		

	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);		//W H H
	
	/* ������ʼλ */
  I2C_GenerateSTART(RTC_I2C, ENABLE);

	I2C_Timeout = I2C_TIMEOUT;	
  while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT))		/*EV5,��ģʽ*/
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Write I2C master mode error!\r\n");
			#endif			
			return I2C_FAIL;
		}				
	}

	/*����������ַ(д)*/
  I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
 
	I2C_Timeout = I2C_TIMEOUT;
	while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	 /*!< Test on EV6 and clear it */
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Write I2C master transmit mode error!\r\n");
			#endif				
			return I2C_FAIL;
		}			
	}

	I2C_SendData(RTC_I2C, WriteAddr);		//����д��ַ

	I2C_Timeout = I2C_TIMEOUT;  
	while (!I2C_CheckEvent(RTC_I2C, /*I2C_EVENT_MASTER_BYTE_TRANSMITTED*/I2C_EVENT_MASTER_BYTE_TRANSMITTING))	/*W H H �޸�ΪEV8ģʽ�������ѷ���*/
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Write I2C send address error!\r\n");
			#endif				
			return I2C_FAIL;
		}	
	}
	
	while(NumByteToWrite--)
	{
		/* дһ���ֽ�*/
	  I2C_SendData(RTC_I2C, *pBuffer); 
		
		I2C_Timeout = I2C_TIMEOUT;
	  while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))  //������ģʽֻ��дһ�ֽ�  
		{
			if((I2C_Timeout--) ==0)
			{
				#ifdef DEBUG_PRINTF
					debugprintf("Write I2C data error!\r\n");
				#endif					
				return I2C_FAIL;
			}				
		}
		
		pBuffer++;
	}
	/* ֹͣλ*/
	I2C_GenerateSTOP(RTC_I2C, ENABLE);

//	OS_CRITICAL_EXIT();	//�˳��ٽ���
	return I2C_OK;
}


/*
	��ȡi2c����
*/
I2C_Status STM32_I2C_BufferRead(uint8_t* pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead)
{
//	CPU_SR_ALLOC();			//�������壬���ڱ���SR״̬.��CPU״̬�Ĵ�����ֵ��������
//	OS_CRITICAL_ENTER();//�����ٽ���				�����������ķ�ʽ����֤���´��벻�����

	I2C_Timeout = I2C_TIMEOUT;
	while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BUSY))		/*�ȴ�IIC���߿���*/
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Read I2C Bus Busy!\r\n");
			#endif					
			return I2C_FAIL;
		}	
	}		
	
  I2C_GenerateSTART(RTC_I2C, ENABLE);					/* ������ʼλ */
  
	I2C_Timeout = I2C_TIMEOUT;
  while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT))		/*EV5,ѡ����ģʽ*/
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Read I2C master mode error!\r\n");
			#endif				
			return I2C_FAIL;
		}		
	}
		
	/*����i2c������ַ(д)*/
  I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
  
	I2C_Timeout = I2C_TIMEOUT;
  while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))	/*EV6*/
	{
		if((I2C_Timeout--) ==0)
		{
			return I2C_FAIL;
		}			
	}
	
	I2C_SendData(RTC_I2C, ReadAddr);	//���Ͷ��ĵ�ַ
	
	/*
		W H H �Լ�������
	*/
	//	I2C_Timeout = I2C_TIMEOUT;
	//  while (!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED))		/*�����ѷ���*/
	//  {
	//		if((I2C_Timeout--) ==0)
	//		{
	//			#ifdef DEBUG_PRINTF
	//				debugprintf("��I2C������ģʽ����!\r\n");
	//			#endif				
	//			return I2C_FAIL;
	//		}		
	//	}
	
	
	/*
		���ʣ���  �ٷ�������
	*/
	 /*!< Test on EV8 and clear it */
	I2C_Timeout = I2C_TIMEOUT;
  while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((I2C_Timeout--) == 0) return I2C_FAIL;
  }
	
	I2C_GenerateSTART(RTC_I2C, ENABLE);  	/*��ʼλ*/
	
	I2C_Timeout = I2C_TIMEOUT;
	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_MODE_SELECT))		/*EV5*/
	{
		if((I2C_Timeout--) ==0)
		{
			#ifdef DEBUG_PRINTF
				debugprintf("Read I2C master selection error!\r\n");
			#endif				
			return I2C_FAIL;
		}			
	}

	I2C_Send7bitAddress(RTC_I2C,I2C1_SLAVE_ADDRESS7, I2C_Direction_Receiver);	/*����������ַ(��)*/
	
	//	/*
	//		W H H �Լ�������,���ʹ�ùٷ����ã���仰һ������ʹ�á�
	//	*/	
	//	I2C_Timeout = I2C_TIMEOUT;
	//	while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))	/*EV6*/
	//	{
	//		if((I2C_Timeout--) ==0)
	//		{
	//			#ifdef DEBUG_PRINTF
	//				debugprintf("��I2C���ܴ���!\r\n");
	//			#endif				
	//			return I2C_FAIL;
	//		}				
	//	}

	/* If number of data to be read is 1, then DMA couldn't be used */
  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
	while(NumByteToRead)
	{
		/*
			W H H �Լ�������
		*/
	//		if(1==NumByteToRead)
	//		{
	//			I2C_AcknowledgeConfig(RTC_I2C, DISABLE);	//���һλ��Ҫ�ر�Ӧ���
	//			
	//    	I2C_GenerateSTOP(RTC_I2C, ENABLE);			//����ֹͣλ
	//		}
	//		
	//		I2C_Timeout = I2C_TIMEOUT;
	//		while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) /*W H H ��һ���ĵط� *//* EV7 */
	//		{
	//			if((I2C_Timeout--) ==0)
	//			{
	//				#ifdef DEBUG_PRINTF
	//					debugprintf("��I2C���ݴ���!\r\n");
	//				#endif					
	//				return I2C_FAIL;
	//			}			
	//		}
	//		
	//		*pBuffer = I2C_ReceiveData(RTC_I2C);
	//		pBuffer++;
	//		NumByteToRead--;
		
		
	/*
		 �ٷ�������
	*/
		/*�������Ķ�ȡһ���ֽڵĺ���*/
		if ((uint16_t)(NumByteToRead) < 2)
		{
			I2C_Timeout = I2C_TIMEOUT;
			while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_ADDR) == RESET)
			{
				if((I2C_Timeout--) == 0)
				{
					return I2C_FAIL;
				}
			}

			/*!< Disable Acknowledgement */
			I2C_AcknowledgeConfig(RTC_I2C, DISABLE);   
			
			/* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
			(void)RTC_I2C->SR2;
			
			/*!< Send STOP Condition */
			I2C_GenerateSTOP(RTC_I2C, ENABLE);
			
			/* Wait for the byte to be received */
			I2C_Timeout = I2C_TIMEOUT;
			while(I2C_GetFlagStatus(RTC_I2C, I2C_FLAG_RXNE) == RESET)
			{
				if((I2C_Timeout--) == 0) 
				{
					return I2C_FAIL;
				}
			}
			
			/*!< Read the byte received from the EEPROM */
			*pBuffer = I2C_ReceiveData(RTC_I2C);
			pBuffer++;
			/*!< Decrement the read bytes counter */
			(NumByteToRead)--;        
			
			/* Wait to make sure that STOP control bit has been cleared */
			I2C_Timeout = I2C_TIMEOUT;
			while(RTC_I2C->CR1 & I2C_CR1_STOP)
			{
				if((I2C_Timeout--) == 0) 
				{
					return I2C_FAIL;
				}
			}  
			
			/*!< Re-Enable Acknowledgement to be ready for another reception */
			I2C_AcknowledgeConfig(RTC_I2C, ENABLE); 				
		}
		
		else
		{
			I2C_Timeout = I2C_TIMEOUT;
			while(!I2C_CheckEvent(RTC_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)) /*W H H ��һ���ĵط� *//* EV7 */
			{
				if((I2C_Timeout--) ==0)
				{
					#ifdef DEBUG_PRINTF
						debugprintf("Read I2C data error!\r\n");
					#endif					
					return I2C_FAIL;
				}			
			}
			
			*pBuffer = I2C_ReceiveData(RTC_I2C);
			pBuffer++;
			NumByteToRead--;
		}
	}
 
	I2C_AcknowledgeConfig(RTC_I2C, ENABLE);  //�ٴ�����Ӧ��ģʽ
//	OS_CRITICAL_EXIT();	//�˳��ٽ���
	return I2C_OK;
}
