/*
*********************************************************************************************************
*
*	ģ������ : I2C��������ģ��
*	�ļ����� : i2c_gpio.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ���
*********************************************************************************************************
*/

#ifndef _I2C_GPIO_H
#define _I2C_GPIO_H

#include <inttypes.h>

#define I2C_WR	0		/* д����bit */
#define I2C_RD	1		/* ������bit */

/* ����I2C������ʼ�ź� */
void i2c_Start(void);
/* ����I2C����ֹͣ�ź� */
void i2c_Stop(void);
/*
  ��I2C���߷���8bit����
*/
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(void);

uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);

void i2c_Configuration(void);
uint8_t i2c_CheckDevice(uint8_t _Address);

#endif
