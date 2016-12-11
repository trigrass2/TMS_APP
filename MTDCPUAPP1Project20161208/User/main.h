/**
  ******************************************************************************
  * @file    Project/STM32_CPAL_Examples/STM32_CPAL_I2C/Basic_EEPROM/main.h
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    21-December-2012
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/*WHH  ����i2c�ӿڵ�EEPROM ����*/
/*�� c/c++�ļ����Ѿ������� USE_STM324xG_EVAL*/
/* Includes ------------------------------------------------------------------*/
#ifdef USE_STM3210C_EVAL
 #include "stm3210c_eval.h"
#elif defined USE_STM32100E_EVAL
 #include "stm32100e_eval.h"
#elif defined USE_STM322xG_EVAL
 #include "stm322xg_eval.h"
#elif defined USE_STM324xG_EVAL
 #include "stm324xg_eval.h"
#elif defined USE_STM32L152_EVAL
 #include "stm32l152_eval.h"
#elif defined USE_STM32L152D_EVAL
 #include "stm32l152d_eval.h"
#endif

#include "stm32_eval_i2c_ee_cpal.h"
#include <stdio.h>
#include "stdbool.h"
#include "os.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Define EEPROM address, size and page size */
#ifdef USE_STM32L152D_EVAL
 #define EEPROM_ADDRESS        0xA0
 #define EEPROM_PAGE_SIZE      4
#else
 #define EEPROM_ADDRESS        0xA0
 #define EEPROM_PAGE_SIZE      32 
#endif /* USE_STM32L152D_EVAL */ 

#if defined (USE_STM32L152_EVAL) || defined (USE_STM32L152D_EVAL) || defined (USE_STM3210C_EVAL) || defined (USE_STM322xG_EVAL) || defined (USE_STM324xG_EVAL)
 #define sEE_DevStructure sEE2_DevStructure 		//WHH �޸ı�ʾʹ���ĸ�i2c�豸
#elif defined USE_STM32100E_EVAL
 #define sEE_DevStructure sEE2_DevStructure 
#endif /* USE_STM32L152_EVAL || USE_STM3210C_EVAL || USE_STM322xG_EVAL || USE_STM324xG_EVAL */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


/*WHH can1 2 led �ƶ���*/

/*�¼���־�� λ���嶨��*/
#define  CAN1TxLED_FLAG  		0x01
#define  CAN1RxLED_FLAG 		0x02
#define  CAN1LEDFLAG_VALUE 	0x00

#define  CAN2TxLED_FLAG  		0x01
#define  CAN2RxLED_FLAG 		0x02
#define  CAN2LEDFLAG_VALUE 	0x00

/*����*/	
extern u16	CarNumber;
extern u8   CarNo;
extern char cartype[10];

/*������ƿ�*/
extern OS_TCB CAN1SlaveTxTaskTCB;

/*1.VVVF�����ж� ����Ҫ��ȫ�ֱ���*/
/*VVVF��Դ */
extern bool DI_VVVFPowertx_B1_10 ;	 			//VVVF��Դ  MC1
extern bool DI_VVVFPowertx_B1_30 ;	 			//VVVF��Դ  M
extern bool DI_VVVFPowertx_B1_60 ;	 			//VVVF��Դ  MC2

/*VVVF ������ЧλCRD*/
extern bool TCU1_CRDtx_SD_B1_10	; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5��
extern bool TCU2_CRDtx_SD_B1_10	; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 
	
extern bool TCU1_CRDtx_SD_B1_30	; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 
extern bool TCU2_CRDtx_SD_B1_30	; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 
	
extern bool TCU1_CRDtx_SD_B1_60	; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 
extern bool TCU2_CRDtx_SD_B1_60	; 				//VVVF1Э���еĵ�����Чλ ��char No.7 Bit5�� 

/*VVVF��Ⱥ����UCORM*/
extern bool TCU1_UCORtx_SD_B1_10 ;    		//VVVF1Э���еģ�char No.17 Bit1��
extern bool TCU2_UCORtx_SD_B1_10 ;		 		//VVVF2Э���еģ�char No.17 Bit1��

extern bool TCU1_UCORtx_SD_B1_30 ;    		//VVVF1Э���еģ�char No.17 Bit1��
extern bool TCU2_UCORtx_SD_B1_30 ;		 		//VVVF2Э���еģ�char No.17 Bit1��

extern bool TCU1_UCORtx_SD_B1_60 ;    		//VVVF1Э���еģ�char No.17 Bit1��
extern bool TCU2_UCORtx_SD_B1_60 ;		 		//VVVF2Э���еģ�char No.17 Bit1��

/*VVVF BSLB_F������Чλ*/
extern bool TCU1_BSLBFtx_SD_B1_10 ; 			//VVVF1Э���еĵ�����Чλ ��char No.18 Bit1��
extern bool TCU2_BSLBFtx_SD_B1_10 ; 			//VVVF2Э���еĵ�����Чλ ��char No.18 Bit1��
                                  
extern bool TCU1_BSLBFtx_SD_B1_30 ; 			//VVVF1Э���еĵ�����Чλ ��char No.18 Bit1��
extern bool TCU2_BSLBFtx_SD_B1_30 ; 			//VVVF2Э���еĵ�����Чλ ��char No.18 Bit1��
                                  
extern bool TCU1_BSLBFtx_SD_B1_60 ; 			//VVVF1Э���еĵ�����Чλ ��char No.18 Bit1��
extern bool TCU2_BSLBFtx_SD_B1_60 ; 			//VVVF2Э���еĵ�����Чλ ��char No.18 Bit1��

/*VVVFͨ��״̬*/
extern bool MDCU_VVVF1ComFaultx_B1_10 ;   //VVVF1ͨ��״̬
extern bool MDCU_VVVF2ComFaultx_B1_10 ;   //VVVF2ͨ��״̬

extern bool MDCU_VVVF1ComFaultx_B1_30 ;   //VVVF1ͨ��״̬
extern bool MDCU_VVVF2ComFaultx_B1_30 ;   //VVVF2ͨ��״̬

extern bool MDCU_VVVF1ComFaultx_B1_60 ;   //VVVF1ͨ��״̬
extern bool MDCU_VVVF2ComFaultx_B1_60 ;   //VVVF2ͨ��״̬

/*BCU ͨ��״̬*/
extern bool MDCU_BCUComFaultrx_B1_10 ;  	//MC1 BCUͨ��״̬ 
extern bool MDCU_BCUComFaultrx_B1_20 ;  	//T BCUͨ��״̬ 
extern bool MDCU_BCUComFaultrx_B1_30 ;  	//M BCUͨ��״̬ 
extern bool MDCU_BCUComFaultrx_B1_40 ;  	//T1 BCUͨ��״̬ 
extern bool MDCU_BCUComFaultrx_B1_50 ;  	//T2 BCUͨ��״̬ 
extern bool MDCU_BCUComFaultrx_B1_60 ;  	//MC2 BCUͨ��״̬ 

/*BCU ���б�־λ*/
extern bool BCU_Axle1Skidtx_SD_B1_10  ;   //MC1 BCU����
extern bool BCU_Axle2Skidtx_SD_B1_10  ;
extern bool BCU_Axle3Skidtx_SD_B1_10  ;
extern bool BCU_Axle4Skidtx_SD_B1_10  ;

extern bool BCU_Axle1Skidtx_SD_B1_60  ;   //MC2 BCU����
extern bool BCU_Axle2Skidtx_SD_B1_60  ;
extern bool BCU_Axle3Skidtx_SD_B1_60  ;
extern bool BCU_Axle4Skidtx_SD_B1_60  ;

/*MTDCPU ����ATC_Sdr�߼�*/
void  MTDCPUProATCSdrLogicalData(void);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
