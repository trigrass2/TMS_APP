#include "VVVFComPro.h"
#include "string.h"
#include "Uart.h"
#include "malloc.h"	
#include "delay.h"
#include "CommunPro.h"
#include "CRC_CCITT.h"
#include "SAF82525.h"
#include "bsp.h"

/*
	函数说明：
功能：发送制动器的状态请求数据。
参数:	
	pInData:指向应用层要发送数据的指针 (指向的类型为，VVVFSdrText,或者 VVVFSdText,VVVFTcrText)   
	pOutData:指向底层串口发送的数据
	u8 mode:发送的数据为SDR，还是TDR	TCR; 
返回值：无返回值
*/
void SendVVVFSdrOrTdrOrTcr(void * pInData,/*u8 * pOutData,*/u8 mode)
{	
	u16	CrcVale;
	u8 PADFlag = VVVF_PAD;
	
	switch(mode)
	{
		case	VVVF_SEND_SDR:
		{
			VVVFSdrFrame SdrDataFrame;	//定义变量
			
			memset(SdrDataFrame.StartPAD,PADFlag,sizeof(SdrDataFrame.StartPAD));	//写入PAD
			SdrDataFrame.StartFlag = VVVF_STARTFLAG;
			SdrDataFrame.Address1 = VVVF_ADDRESS1;
			SdrDataFrame.Address2 = VVVF_ADDRESS2;
			SdrDataFrame.Control = VVVF_CONTROLBYTE;
			memcpy(&SdrDataFrame.VVVFSdrData,pInData,sizeof(VVVFSdrText));		//复制文本数据
			SdrDataFrame.VVVFSdrData.TextNumber = VVVF_SDR_TEXTNUM;
			SdrDataFrame.VVVFSdrData.Command	= VVVF_SDR_COMMAND;
			SdrDataFrame.VVVFSdrData.State = VVVF_SDR_STATE;
			SdrDataFrame.VVVFSdrData.Sender = VVVF_SDR_SENDER;
			SdrDataFrame.VVVFSdrData.OrderNo = VVVF_SDR_NO;
			
			CrcVale=crcCompute((u8*)&SdrDataFrame+5,sizeof(VVVFSdrText)+3); //生成CRC校验值,校验的是地址位，控制位和文本数据帧的数据
			SdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			SdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			SdrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy数据到输出缓冲区*/
			//memcpy(pOutData,&SdrDataFrame,sizeof(VVVFSdFrame));
			if(SdrDataFrame.VVVFSdrData.Receiver == VVVF_SDR_RECEIVER1)		//VVVF1发送
			{
				printf("VVVF1 Send!\r\n");
				AChanleTxData((u8 *)&SdrDataFrame,sizeof(VVVFSdrFrame));
			}
			else		//VVVF2发送
			{
				printf("VVF2 Send!\r\n");
				BChanleTxData((u8 *)&SdrDataFrame,sizeof(VVVFSdrFrame));
			}
			
		}
		break;
		
		case VVVF_SEND_TDR:
		{
			VVVFTdrFrame TdrDataFrame;
			
			memset(TdrDataFrame.StartPAD,PADFlag,sizeof(TdrDataFrame.StartPAD));
			TdrDataFrame.StartFlag = VVVF_STARTFLAG;
			TdrDataFrame.Address1 = VVVF_ADDRESS1;
			TdrDataFrame.Address2 = VVVF_ADDRESS2;
			TdrDataFrame.Control = VVVF_CONTROLBYTE;
			memcpy(&TdrDataFrame.VVVFTdrData,pInData,sizeof(VVVFTdrText));//复制文本数据
			TdrDataFrame.VVVFTdrData.TextNumber = VVVF_TDR_TEXTNUM;
			TdrDataFrame.VVVFTdrData.Command = VVVF_TDR_COMMAND;
			TdrDataFrame.VVVFTdrData.State = VVVF_TDR_STATE;
			TdrDataFrame.VVVFTdrData.Sender = VVVF_TDR_SENDER;
			TdrDataFrame.VVVFTdrData.OrderNo = VVVF_TDR_NO;
			
			CrcVale = crcCompute((u8*)&TdrDataFrame+5,sizeof(VVVFTdrText)+3);//生成CRC校验值,校验的是地址位，控制位和文本数据帧的数据
			TdrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TdrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TdrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy数据到输出缓冲区*/
			//memcpy(pOutData,&TdrDataFrame,sizeof(VVVFTdrFrame));
			if(TdrDataFrame.VVVFTdrData.Receiver == VVVF_SDR_RECEIVER1)		//VVVF1发送
			{
				AChanleTxData((u8 *)&TdrDataFrame,sizeof(VVVFTdrFrame));
			}
			else		//VVVF2发送
			{
				BChanleTxData((u8 *)&TdrDataFrame,sizeof(VVVFTdrFrame));
			}
		}
		break;
		
		case VVVF_SEND_TCR:
		{
			VVVFTcrFrame TcrDataFrame;
			
			memset(TcrDataFrame.StartPAD,PADFlag,sizeof(TcrDataFrame.StartPAD));
			TcrDataFrame.StartFlag = VVVF_STARTFLAG;
			TcrDataFrame.Address1 = VVVF_ADDRESS1;
			TcrDataFrame.Address2 = VVVF_ADDRESS2;
			TcrDataFrame.Control = VVVF_CONTROLBYTE;
			memcpy(&TcrDataFrame.VVVFTcrData,pInData,sizeof(VVVFTdrText));//复制文本数据
			TcrDataFrame.VVVFTcrData.TextNumber = VVVF_TCR_TEXTNUM;
			TcrDataFrame.VVVFTcrData.Command = VVVF_TCR_COMMAND;
			TcrDataFrame.VVVFTcrData.State = VVVF_TCR_STATE;
			TcrDataFrame.VVVFTcrData.Sender = VVVF_TCR_SENDER;
			TcrDataFrame.VVVFTcrData.OrderNo = VVVF_TCR_NO;
			
			CrcVale = crcCompute((u8*)&TcrDataFrame+5,sizeof(VVVFTcrText)+3);//生成CRC校验值,校验的是地址位，控制位和文本数据帧的数据
			TcrDataFrame.CRCH8 = (u8)(CrcVale>>8)&0xFF; 
			TcrDataFrame.CRCL8 = (u8)(CrcVale)&0xFF;
			TcrDataFrame.EndFlag = VVVF_ENDFLAG;
			
			/*copy数据到输出缓冲区*/
			//memcpy(pOutData,&TcrDataFrame,sizeof(VVVFTcrFrame));
			if(TcrDataFrame.VVVFTcrData.Receiver == VVVF_SDR_RECEIVER1)		//VVVF1发送
			{
				AChanleTxData((u8 *)&TcrDataFrame,sizeof(VVVFTcrFrame));
			}
			else		//VVVF2发送
			{
				BChanleTxData((u8 *)&TcrDataFrame,sizeof(VVVFTcrFrame));
			}			
		}
		break;
		
		default: 
			#if	 DEBUG_RS485==1
				printf("VVVF Frame Format Error！\r\n");
			#endif
			break;
	}
}  


/*
	函数说明：
功能：接受车门控制的状态响应的数据。
参数: 
	pInData:指向要底层硬件接受到的数据的指针 
	pOutData:监控设备，得到每个状态标识(放到S_TMS_BRAKE_SD_Text结构体)。 复制数据 去除前面的0x7E
	VVVF_No: 表示获取VVVF1，或者VVVF2
//	u8 mode :接受的数据为SD，或者 TD
返回值：0,无错，  其他有错。
*/
s8 GetVVVFSdOrTd(u8 *pInData,void *pOutData,u8 VVVF_No)
{
	s8 returnvale=0;			//记录返回值
	u8 StartVale=0,ComandVale=0;
	u16 SourCrcVale,ComputeCrcVale;
	u8 i=0;		//记录开始数据有多少个0x7E
	
	returnvale =	CheckVVVFGetError(pInData,VVVF_No);
	
	if(returnvale !=0)
	{
		#if DEBUG_RS485==0
		switch(VVVF_No)
		{
			case  VVVF1:
				printf("VVVF11 Get SDorTD Error！\r\n");
				break;
			case VVVF2:
				printf("VVVF22Get SDorTD Error！\r\n");
				break;
		}
		#endif	
		return VVVF_GetError;
	}
	
	/*进行CRC检验*/
	i=0;		//计算出有多少个0x7E
	while(1)
	{
		StartVale = *(pInData+i);
		if(StartVale == VVVF_PAD)
		{
			i++;
		}
		else
		{
			break; //跳出while循环
		}
	}
		//printf("0x7e-%d\r\n",i);
	ComandVale = *(pInData+i+3+1);		//得到命令字节  i+3+1 = 0x7E个数+地址2字节，控制字1字节+1字节文本数 
	
	switch(ComandVale)
	{
		case VVVF_SD_COMMAND:
		{
			ComputeCrcVale = crcCompute(pInData+i,sizeof(VVVFSdText)+3);
			SourCrcVale = (u16)(*(pInData+i+3+sizeof(VVVFSdText)) <<8) | *(pInData+i+3+sizeof(VVVFSdText)+1);
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData+i+3,sizeof(VVVFSdText));		//去除前面的0x7E	和地址 控制字节，只要文本数据
			}
			else
			{
				#if DEBUG_RS485==0
				printf("VVVF Sd校验值不符合\r\n");
				#endif		
				return VVVF_CRCError;
			}
		}
		break;		//sd
		
		case VVVF_TD_COMMAND:
		{
			ComputeCrcVale = crcCompute(pInData+i,sizeof(VVVFTdText)+3);
			SourCrcVale = (u16)(*(pInData+i+3+sizeof(VVVFTdText)) <<8) | *(pInData+i+3+sizeof(VVVFTdText)+1) ;
			
			if(SourCrcVale == ComputeCrcVale)
			{
				memcpy(pOutData,pInData+i+3,sizeof(VVVFTdText));//去除前面的0x7E	和地址 控制字节，只要文本数据
			}
			else
			{
				#if DEBUG_RS485==1
				printf("VVVF Td校验值不符合\r\n");
				#endif		
				return VVVF_CRCError;				
			}			
		}
		break;	//td
		
		default:
			break;	//错误
	}
	return 0;
}


//extern 	u8 g_RxDataFlag;
//extern	u8 g_FrameErrorFlag;

/*
	函数说明：
功能：检测接受数据时候的错误
参数: 
	VVVF_No: 表示获取VVVF1，或者VVVF2
返回值：返回0，没有错误。返回其他值，错误值
*/
s8 CheckVVVFGetError(u8 *pInData,u8 VVVF_No)
{
	//1.发送SDR 后，经过80ms 以上则没有正常收到SD 时,错误
	u8 i=0;
	
	switch(VVVF_No)
	{
		case VVVF1:
			for(i=0;i<8;i++)		//这样处理的好处是，可以及时判断数据是否到来，响应会及时些
			{
				if(g_HDLCARxEndFlag == 1)
				{
					g_HDLCARxEndFlag = 0; //清除结束标志
					break;
				}		
				
				else
					BSP_OS_TimeDlyMs(10);//延时10ms后，判断接受中断标志位的值				//W H H 这里可以用个软件定时器 替代， 暂时使用延时代表 		
			}
			if(i>=8)
			{
				if(g_HDLCARxEndFlag == 0)	//说明没有接受到数据
				{
					#if DEBUG_RS485==0
						printf("VVVF1 80ms内没有接受到数据\r\n");
					#endif
					return VVVF_GetTimeoutError;
				}
			}
		break;
			
		case VVVF2:
			for(i=0;i<8;i++)		//这样处理的好处是，可以及时判断数据是否到来，响应会及时些
			{
				if(g_HDLCBRxEndFlag == 1)
				{
					g_HDLCBRxEndFlag = 0; //清除结束标志
					break;
				}		
				
				else
					BSP_OS_TimeDlyMs(10);//延时10ms后，判断接受中断标志位的值				//W H H 这里可以用个软件定时器 替代， 暂时使用延时代表 		
			}
			if(i>=8)
			{
				if(g_HDLCBRxEndFlag == 0)	//说明没有接受到数据
				{
					#if DEBUG_RS485==0
						printf("VVVF2 80ms内没有接受到数据\r\n");
					#endif
					return VVVF_GetTimeoutError;
				}
			}			
		break;
	}

	return 0;
}

