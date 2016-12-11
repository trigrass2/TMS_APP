#ifndef __SPI_H
#define __SPI_H

#include "stm32f4xx.h" 	
#define  MTDCPU_SPI1_CS_GPIO   				GPIOA
#define  MTDCPU_SPI1_CS_CPIO_PIN			GPIO_Pin_4

/* Ƭѡ�����õ�ѡ��  */
#define MTDCPU_SPI1_CS_LOW()      		MTDCPU_SPI1_CS_GPIO->BSRRH = MTDCPU_SPI1_CS_CPIO_PIN

/* Ƭѡ�����ø߲�ѡ�� */
#define MTDCPU_SPI1_CS_HIGH()      		MTDCPU_SPI1_CS_GPIO->BSRRL = MTDCPU_SPI1_CS_CPIO_PIN


#define  MTDCPU_SPI1 				SPI1
#define  MTDCPU_SPI1_CLK			RCC_APB2Periph_SPI1

void MTDCPU_SPI1_Init(void);			 //��ʼ��SPI1��
void MTDCPU_SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 	 MTDCPU_SPI1_SendByte(u8 TxData);		//SPI1���߷���һ���ֽ�
u8 	 MTDCPU_SPI1_ReadByte(void);    		//SPI1���߶�ȡһ���ֽڵ�����
#endif

