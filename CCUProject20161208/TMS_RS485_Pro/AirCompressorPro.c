#include "AirCompressorPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"

u16  crc_table[ 256 ] = { 
		0x0000 ,  0x1021 ,  0x2042 ,  0x3063 ,  0x4084 ,  0x50a5 ,  0x60c6 ,  0x70e7 , 
		0x8108 ,  0x9129 ,  0xa14a ,  0xb16b ,  0xc18c ,  0xd1ad ,  0xe1ce ,  0xf1ef , 
		0x1231 ,  0x0210 ,  0x3273 ,  0x2252 ,  0x52b5 ,  0x4294 ,  0x72f7 ,  0x62d6 , 
		0x9339 ,  0x8318 ,  0xb37b ,  0xa35a ,  0xd3bd ,  0xc39c ,  0xf3ff ,  0xe3de , 
		0x2462 ,  0x3443 ,  0x0420 ,  0x1401 ,  0x64e6 ,  0x74c7 ,  0x44a4 ,  0x5485 , 
		0xa56a ,  0xb54b ,  0x8528 ,  0x9509 ,  0xe5ee ,  0xf5cf ,  0xc5ac ,  0xd58d , 
		0x3653 ,  0x2672 ,  0x1611 ,  0x0630 ,  0x76d7 ,  0x66f6 ,  0x5695 ,  0x46b4 , 
		0xb75b ,  0xa77a ,  0x9719 ,  0x8738 ,  0xf7df ,  0xe7fe ,  0xd79d ,  0xc7bc , 
		0x48c4 ,  0x58e5 ,  0x6886 ,  0x78a7 ,  0x0840 ,  0x1861 ,  0x2802 ,  0x3823 , 
		0xc9cc ,  0xd9ed ,  0xe98e ,  0xf9af ,  0x8948 ,  0x9969 ,  0xa90a ,  0xb92b , 
		0x5af5 ,  0x4ad4 ,  0x7ab7 ,  0x6a96 ,  0x1a71 ,  0x0a50 ,  0x3a33 ,  0x2a12 , 
		0xdbfd ,  0xcbdc ,  0xfbbf ,  0xeb9e ,  0x9b79 ,  0x8b58 ,  0xbb3b ,  0xab1a , 
		0x6ca6 ,  0x7c87 ,  0x4ce4 ,  0x5cc5 ,  0x2c22 ,  0x3c03 ,  0x0c60 ,  0x1c41 , 
		0xedae ,  0xfd8f ,  0xcdec ,  0xddcd ,  0xad2a ,  0xbd0b ,  0x8d68 ,  0x9d49 , 
		0x7e97 ,  0x6eb6 ,  0x5ed5 ,  0x4ef4 ,  0x3e13 ,  0x2e32 ,  0x1e51 ,  0x0e70 , 
		0xff9f ,  0xefbe ,  0xdfdd ,  0xcffc ,  0xbf1b ,  0xaf3a ,  0x9f59 ,  0x8f78 , 
		0x9188 ,  0x81a9 ,  0xb1ca ,  0xa1eb ,  0xd10c ,  0xc12d ,  0xf14e ,  0xe16f , 
		0x1080 ,  0x00a1 ,  0x30c2 ,  0x20e3 ,  0x5004 ,  0x4025 ,  0x7046 ,  0x6067 , 
		0x83b9 ,  0x9398 ,  0xa3fb ,  0xb3da ,  0xc33d ,  0xd31c ,  0xe37f ,  0xf35e , 
		0x02b1 ,  0x1290 ,  0x22f3 ,  0x32d2 ,  0x4235 ,  0x5214 ,  0x6277 ,  0x7256 , 
		0xb5ea ,  0xa5cb ,  0x95a8 ,  0x8589 ,  0xf56e ,  0xe54f ,  0xd52c ,  0xc50d , 
		0x34e2 ,  0x24c3 ,  0x14a0 ,  0x0481 ,  0x7466 ,  0x6447 ,  0x5424 ,  0x4405 , 
		0xa7db ,  0xb7fa ,  0x8799 ,  0x97b8 ,  0xe75f ,  0xf77e ,  0xc71d ,  0xd73c , 
		0x26d3 ,  0x36f2 ,  0x0691 ,  0x16b0 ,  0x6657 ,  0x7676 ,  0x4615 ,  0x5634 , 
		0xd94c ,  0xc96d ,  0xf90e ,  0xe92f ,  0x99c8 ,  0x89e9 ,  0xb98a ,  0xa9ab , 
		0x5844 ,  0x4865 ,  0x7806 ,  0x6827 ,  0x18c0 ,  0x08e1 ,  0x3882 ,  0x28a3 , 
		0xcb7d ,  0xdb5c ,  0xeb3f ,  0xfb1e ,  0x8bf9 ,  0x9bd8 ,  0xabbb ,  0xbb9a , 
		0x4a75 ,  0x5a54 ,  0x6a37 ,  0x7a16 ,  0x0af1 ,  0x1ad0 ,  0x2ab3 ,  0x3a92 , 
		0xfd2e ,  0xed0f ,  0xdd6c ,  0xcd4d ,  0xbdaa ,  0xad8b ,  0x9de8 ,  0x8dc9 , 
		0x7c26 ,  0x6c07 ,  0x5c64 ,  0x4c45 ,  0x3ca2 ,  0x2c83 ,  0x1ce0 ,  0x0cc1 , 
		0xef1f ,  0xff3e ,  0xcf5d ,  0xdf7c ,  0xaf9b ,  0xbfba ,  0x8fd9 ,  0x9ff8 , 
		0x6e17 ,  0x7e36 ,  0x4e55 ,  0x5e74 ,  0x2e93 ,  0x3eb2 ,  0x0ed1 ,  0x1ef0  
	} ; 
 
u16  crc16( u8  d[],  int  len)
{
		u8   hbit  =   0 ;
		u16  crc  =   0xffff ;
		int  i;

		for (i = 0 ; i < len; i ++ )
		{        
				hbit  =  (crc & 0xff00 ) >> 8 ;
				crc <<= 8 ;
				crc  ^=  crc_table[hbit ^ d[i]];
		} 
	 
//	 crc  =   ~ crc;			//���crcֵ�൱�� ���0xff ��λȡ����

	 printf( " crc: 0x%.4X " , crc);     
		return crc;
} 

/*
	����˵����
���ܣ����Ϳ�ѹ����״̬�������ݡ�
����:	
	pInData:ָ��Ӧ�ò�Ҫ�������ݵ�ָ��   
	pOutData:ָ��ײ㴮�ڷ��͵����ݻ���
����ֵ���޷���ֵ
*/
void SendAirCompre_SDR(S_TMS_AirCompre_SDR_Frame * pInData,u8 * pOutData)
{
	
		#if	 DEBUG_RS485==1
			printf("����豸������������\r\n");
		#endif
		pInData->CRCH8 = (crc16((u8 *)pInData,sizeof(S_TMS_AirCompre_SDR_Frame)-2)>>8) & 0x0ff;
		pInData->CRCL8 = (crc16((u8 *)pInData,sizeof(S_TMS_AirCompre_SDR_Frame)-2)) & 0x0ff;
	
		memcpy(pOutData,pInData,sizeof(S_TMS_AirCompre_SDR_Frame));
}

/*
	����˵����
���ܣ����ܿ�ѹ����״̬��Ӧ�����ݡ�
����: 
	pInData:ָ��Ҫ�ײ�Ӳ�����ܵ������ݻ����ָ�� 
	pOutData:����豸���õ�ÿ��״̬��ʶ(�ŵ�S_AirCompre_SD_StateBit�ṹ��)��
����ֵ��0,�޴�  �����д�
*/
s8 GetAirCompre_SD(u8 *pInData,S_AirCompre_SD_StateBit *pOutData)
{
	s8 returnvale=0;
	S_TMS_AirCompre_SD_Frame DataFrame;
	u8 CRCH8,CRCL8;
	returnvale=CheckAirCompreGetError(pInData);		//�������Ƿ����
	if(returnvale !=0)
	{
		#if DEBUG_RS485==1
			printf("�д�������\r\n");
		#endif	
		return AIRCOMP_GetError;
	}
	
	memcpy(&DataFrame,pInData,sizeof(DataFrame));	//�����ڽ��ܵ����ݿ������ṹ����

	CRCH8 = (crc16(&DataFrame.AirCompressAddress,sizeof(S_TMS_AirCompre_SD_Frame)-2)>>8)& 0xff;	//�õ����ݵ�CRCУ��ֵ
	CRCL8 = (crc16(&DataFrame.AirCompressAddress,sizeof(S_TMS_AirCompre_SD_Frame)-2))& 0xff;		//�õ����ݵ�CRCУ��ֵ
	
	if(CRCH8 != DataFrame.CRCH8 && CRCL8 != DataFrame.CRCL8)
	{
		#if DEBUG_RS485==1
			printf("BCCУ��ֵ������\r\n");
		#endif		
		return AIRCOMP_CRCError;
	}
	
	if(returnvale==0)	//��ʾû�д���
	{
		memcpy(&pOutData->AirCompressAddress,&DataFrame.AirCompressAddress,(size_t)3);
		
		/*����1״̬*/
		pOutData->NetVoltLowBit = GetDataBit(&DataFrame.FaultCode1,AIRCOMP_NETVOLTLOW_POSIT);
		pOutData->NetVoltHigBit = GetDataBit(&DataFrame.FaultCode1,AIRCOMP_NETVOLTHIG_POSIT);
		pOutData->FCLowBit = GetDataBit(&DataFrame.FaultCode1,AIRCOMP_FCLOW_POSIT);
		pOutData->OutputOver1 = GetDataBit(&DataFrame.FaultCode1,AIRCOMP_OUTOVER1_POSIT);
		pOutData->OutputOver2 =	GetDataBit(&DataFrame.FaultCode1,AIRCOMP_OUTOVER2_POSIT);
		pOutData->OutputOver3 = GetDataBit(&DataFrame.FaultCode1,AIRCOMP_OUTOVER3_POSIT);
		pOutData->OutputShort = GetDataBit(&DataFrame.FaultCode1,AIRCOMP_OUTSHORT_POSIT);
		pOutData->OutputDefPhase =GetDataBit(&DataFrame.FaultCode1,AIRCOMP_OUTDEFPHASE_POSIT);
		
		/*����2״̬*/
		pOutData->IGBT_Or_RaOverHeat = GetDataBit(&DataFrame.FaultCode2,AIRCOMP_IGBT_OR_RAHeat_POSIT);
		pOutData->SelfFault =	GetDataBit(&DataFrame.FaultCode2,AIRCOMP_SELFFAULT_POSIT);
		pOutData->KM2CloseFalut = GetDataBit(&DataFrame.FaultCode2,AIRCOMP_KM2CLOSEFAULT_POSIT);
		pOutData->IGBTFault = GetDataBit(&DataFrame.FaultCode2,AIRCOMP_IGBTFAULT_POSIT);
		pOutData->OtherFault = GetDataBit(&DataFrame.FaultCode2,AIRCOMP_OTHERFAULT_POSIT);
		
		memcpy(&pOutData->InputVoltageH8,&DataFrame.InputVoltageH8,sizeof(S_TMS_AirCompre_SD_Frame)-7);
		pOutData->CRCH8 = CRCH8;
		pOutData->CRCL8 = CRCL8;
	}
		//û�д��󷵻�0
		return 0;

}


extern 	u8 g_RxDataFlag;
extern	u8 g_FrameErrorFlag;
/*
	����˵����
���ܣ�����������ʱ��Ĵ���
����: 
����ֵ������0��û�д��󡣷�������ֵ������ֵ
*/
s8 CheckAirCompreGetError(u8 *pInData)
{
	//1.����SDR �󣬾���20ms ������û�������յ�SD ʱ,����
	{
		u8 i=0;
		for(i=0;i<10;i++)		//��������ĺô��ǣ����Լ�ʱ�ж������Ƿ�������Ӧ�ἰʱЩ
		{
			delay_ms(100);//��ʱ100ms���жϽ����жϱ�־λ��ֵ				//W H H ��������ø������ʱ�� ����� ��ʱʹ����ʱ���� 
			if(g_RxDataFlag == 1)
			{
				break;
			}				
		}
		if(i>=10)
		{
			if(g_RxDataFlag == 0)	//˵��û�н��ܵ�����
			{
				#if DEBUG_RS485==1
					printf("1s��û�н��ܵ�����\r\n");
				#endif
				return AIRCOMP_GetTimeoutError;
			}
		}
	}
	
	//2.��⵽֡������żУ�����ʱ,�Ĵ���
	if(g_FrameErrorFlag == 1)	////��⵽֡������żУ�����ʱ
	{
		#if DEBUG_RS485==1
			printf("��⵽֡������żУ�����\r\n");
		#endif
		return AIRCOMP_FrameError;
	}
	
	//3.�����ܵ�ǰ�ֽ������Ƿ�ƥ��
//	if(pInData[0]!=AIR_STX || pInData[1]!=AIR_ADDRES || pInData[2]!=AIR_SUB_ADDRES ||pInData[3]!=AIR_COMMAND)
//	{
//		#if DEBUG_RS485==1
//			printf("���ܵĿ�ʼ������ַ��������һ��������\r\n");
//		#endif		
//			return AIRCOMP_DataError;
//	}
	
	//û�д������
	return 0;
}



void test_aircomper(void)
{
		S_TMS_AirCompre_SD_Frame	dafr;
		S_AirCompre_SD_StateBit bit;
		u8 buffer[20];
		memset(buffer,0xac,sizeof(buffer));
		memset(&dafr,0xff,sizeof(S_TMS_AirCompre_SD_Frame));
		dafr.AirCompressAddress=1;
		dafr.Command=2;
		dafr.Datalen=3;
		dafr.FaultCode1=4;
		dafr.FaultCode2=5;
		dafr.InputVoltageH8=6;
		dafr.InputVoltageL8=7;
	
		printf("bit=%d\r\n",sizeof(bit));
		printf("AirCompre=%d\r\n",sizeof(S_AirCompre_SD_StateBit));
		memcpy(&bit.AirCompressAddress,&dafr.AirCompressAddress,(size_t)3);
		memcpy(&bit.InputVoltageH8,&dafr.InputVoltageH8,sizeof(S_TMS_AirCompre_SD_Frame)-5);
		printf("Ad=%d\r\n",bit.AirCompressAddress);
		printf("cd=%d\r\n",bit.Command);
		printf("da=%d\r\n",bit.Datalen);
		printf("1=%d\r\n",bit.InputVoltageH8);
		printf("2=%d\r\n",bit.InputVoltageL8);
		printf("3=%d\r\n",bit.OutputHzH8);
		printf("4=%d\r\n",bit.CRCH8);
		printf("5=%d\r\n",bit.CRCL8);
		
		GetAirCompre_SD(buffer,&bit);
		printf("Ad=%d\r\n",bit.AirCompressAddress);
		printf("cd=%d\r\n",bit.Command);
		printf("da=%d\r\n",bit.Datalen);
		printf("1=%d\r\n",bit.InputVoltageH8);
		printf("2=%d\r\n",bit.InputVoltageL8);
		printf("3=%d\r\n",bit.OutputHzH8);
		printf("4=%d\r\n",bit.CRCH8);
		printf("5=%d\r\n",bit.CRCL8);
		printf("bit6=%d\r\n",bit.NetVoltHigBit);
		printf("bit7=%d\r\n",bit.NetVoltLowBit);
		printf("bit1=%d\r\n",bit.OutputShort);
		printf("bit0=%d\r\n",bit.OutputDefPhase);
		
}

