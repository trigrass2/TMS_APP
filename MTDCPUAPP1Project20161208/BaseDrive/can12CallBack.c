#include "can12CallBack.h"
#include "bsp.h"
#include "main.h"


/*���ñ���ʱ��*/
UNS32 SetMTDTimeCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{

	RTC_TIME time;
	static u8 HMITxDev_Time[7]={0};

	//debugprintf("set time call!\r\n");
	if(d == &MTDApp1CAN1Slave_Data)		//can1
	{
		if(memcmp(HMITxDev_Time,can1_HMITxDev_Time,7) !=0)  // ��ʾ���ݲ����,��ִ��
		{
			if(can1_HMITxDev_Time[0] == 1)
			{
				time._year = can1_HMITxDev_Time[1];
				time._month = can1_HMITxDev_Time[2];
				time._day	= can1_HMITxDev_Time[3];
				time._hour = can1_HMITxDev_Time[4];
				time._min = can1_HMITxDev_Time[5];
				time._sec = can1_HMITxDev_Time[6];
		
				Manual_Set_Time(&time);		//���ñ���ʱ��
				
				can3_TxDoorSpeedTimBit[0] = 1; //����DOOR��ʱ��
				MTDCAN3Master_Data.PDO_status[2].last_message.cob_id = 0;    //0x184
				sendOnePDOevent(&MTDCAN3Master_Data,2);
				can3_TxDoorSpeedTimBit[0] = 0; //���ʱ��������Чλ
				
				can3_TxAccTempTimBit[0] |= 0x01; //����ACC��ʱ��	  (�յ�bit1Ϊʱ������)			
				MTDCAN3Master_Data.PDO_status[3].last_message.cob_id = 0;    //0x185
				sendOnePDOevent(&MTDCAN3Master_Data,3);
				can3_TxAccTempTimBit[0] &= 0xFE; //���ʱ��������Чλ
				
				can3_TxBCUCmd[1] |= 0x02;  //����BCU��ʱ�� (BCU��1�ֽڣ�bit2)				
				MTDCAN3Master_Data.PDO_status[4].last_message.cob_id = 0;    //0x186
				sendOnePDOevent(&MTDCAN3Master_Data,4);
		    can3_TxBCUCmd[1] &= 0xFD;  //���ʱ��������Чλ
				
				can3_TxVVVFCmd[0] |= 0x0A; //����VVVF1,VVVF2��ʱ�� (VVVF ��1�ֽڣ�bit4 ��bit2)
				MTDCAN3Master_Data.PDO_status[5].last_message.cob_id = 0;    //0x187
				sendOnePDOevent(&MTDCAN3Master_Data,5);
		    can3_TxVVVFCmd[0] &= 0xF5;  //���ʱ��������Чλ
				
				can3_TxSIVCmd[0] |= 0x02; //����SIV��ʱ�� (SIV ��1�ֽڣ�bit2)
				MTDCAN3Master_Data.PDO_status[6].last_message.cob_id = 0;    //0x188
				sendOnePDOevent(&MTDCAN3Master_Data,6);
		    can3_TxSIVCmd[0] &= 0xFD;  //���ʱ��������Чλ				
				
			}
			memcpy(HMITxDev_Time,can1_HMITxDev_Time,7);			//�����ݸ��Ƶ���������
		}
  }
	
	else		//can2
	{
		if(memcmp(HMITxDev_Time,can2_HMITxDev_Time,7) !=0)  // ��ʾ���ݲ����,��ִ��
		{
			if(can2_HMITxDev_Time[0] == 1)
			{
				time._year = can2_HMITxDev_Time[1];
				time._month = can2_HMITxDev_Time[2];
				time._day	= can2_HMITxDev_Time[3];
				time._hour = can2_HMITxDev_Time[4];
				time._min = can2_HMITxDev_Time[5];
				time._sec = can2_HMITxDev_Time[6];
				
				Manual_Set_Time(&time);		//���ñ���ʱ��
				
				can3_TxDoorSpeedTimBit[0] = 1; //����DOOR��ʱ��
				MTDCAN3Master_Data.PDO_status[2].last_message.cob_id = 0;    //0x184
				sendOnePDOevent(&MTDCAN3Master_Data,2);
				can3_TxDoorSpeedTimBit[0] = 0;  //���ʱ��������Чλ
				
				can3_TxAccTempTimBit[0] |= 0x01; //����ACC��ʱ��	  (�յ����1bitΪʱ������)			
				MTDCAN3Master_Data.PDO_status[3].last_message.cob_id = 0;    //0x185
				sendOnePDOevent(&MTDCAN3Master_Data,3);	
				can3_TxAccTempTimBit[0] &= 0xFE; //���ʱ��������Чλ
	
				can3_TxBCUCmd[1] |= 0x02;  //����BCU��ʱ�� (BCU��1�ֽڣ�bit2)				
				MTDCAN3Master_Data.PDO_status[4].last_message.cob_id = 0;    //0x186
				sendOnePDOevent(&MTDCAN3Master_Data,4);
		    can3_TxBCUCmd[1] &= 0xFD;  //���ʱ��������Чλ	

				can3_TxVVVFCmd[0] |= 0x0A; //����VVVF1,VVVF2��ʱ�� (VVVF ��1�ֽڣ�bit4 ��bit2)
				MTDCAN3Master_Data.PDO_status[5].last_message.cob_id = 0;    //0x187
				sendOnePDOevent(&MTDCAN3Master_Data,5);
		    can3_TxVVVFCmd[0] &= 0xF5;  //���ʱ��������Чλ
				
				can3_TxSIVCmd[0] |= 0x02; //����SIV��ʱ�� (SIV ��1�ֽڣ�bit2)
				MTDCAN3Master_Data.PDO_status[6].last_message.cob_id = 0;    //0x188
				sendOnePDOevent(&MTDCAN3Master_Data,6);
		    can3_TxSIVCmd[0] &= 0xFD;  //���ʱ��������Чλ		
			}
			memcpy(HMITxDev_Time,can2_HMITxDev_Time,7);			//�����ݸ��Ƶ���������
		}		
	}
	
	return 0;
}

///*���ÿյ��¶�*/
//UNS32 SetAccTempCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
//{
//	static u8 RxAccTemp[1]={0};
//	
//	if(d == &MTDApp1CAN1Slave_Data)		//can1
//	{
//		if(memcmp(RxAccTemp,can1_HMITxAccTemp,1) !=0)  // ��ʾ���ݲ����,��ִ��
//		{
//			can3_TxAccTempTimBit[0] = can1_HMITxAccTemp[0];
//			
//			MTDCAN3Master_Data.PDO_status[3].last_message.cob_id = 0; //ǿ�Ʒ���
//			sendOnePDOevent(&MTDCAN3Master_Data,3);		//can3,0x185,3
//		memcpy(RxAccTemp,can1_HMITxAccTemp,1);			//�����ݸ��Ƶ���������
//		}
//	}
//	
//	else		//can2
//	{
//		if(memcmp(RxAccTemp,can2_HMITxAccTemp,1) !=0)  // ��ʾ���ݲ����,��ִ��
//		{
//			can3_TxAccTempTimBit[0] = can2_HMITxAccTemp[0];
//			
//			MTDCAN3Master_Data.PDO_status[3].last_message.cob_id = 0; //ǿ�Ʒ���
//			sendOnePDOevent(&MTDCAN3Master_Data,3);		//can3,0x185,3	
//		memcpy(RxAccTemp,can2_HMITxAccTemp,1);			//�����ݸ��Ƶ���������		
//		}			
//	}
//	return 0;
//}


/*���ó������*/
UNS32 SetCarNoCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static u8 RxCarNo[4]={0};
	//uint8_t i=0;	//i,��ʾ0x205E ��0�ĸ���
		
	debugprintf("set car No!\r\n");
	//	if(d == &MTDApp1CAN1Slave_Data)	//can1
	//	{
	//		//����0�ĸ���
	//		for(i=0;i<4;i++)
	//		{
	//			if(can1_HMITxCarNoPar[i] !=0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//	
	//	else		//can2
	//	{
	//		//����0�ĸ���
	//		for(i=0;i<4;i++)
	//		{
	//			if(can2_HMITxCarNoPar[i] !=0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//	
	//	if(i>=4)	//������ܵ����ݶ���0 ��ֱ���˳�
	//	{
	//		return 0;
	//	}

	//debugprintf("index=0x%x\r\n",unsused_indextable->index);
	if(d == &MTDApp1CAN1Slave_Data)		//can1
	{
		switch(unsused_indextable->index)
		{
			case 0x205E:
				if(can1_HMITxCarNoPar[1] == 1)		//�ڶ��ֽ�Ϊ���õ����� ��1��ʾ�������
				{
					if(memcmp(RxCarNo,can1_HMITxCarNoPar,4)!=0)// ��ʾ���ݲ����,��ִ��
					{
						//debugprintf("can1 car 123 !\r\n");
						switch(ChooseBoard)
						{
							case MC1_MTD_NODEID:
								if(can1_HMITxCarNoPar[0] ==1 ) //��һ���ֽڱ�ʾ����������
								{
									CarNumber = (uint16_t)can1_HMITxCarNoPar[2]*100+can1_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);
									
									can1_MC1TxCarNo[0] =  can1_HMITxCarNoPar[2];
									can1_MC1TxCarNo[1] =  can1_HMITxCarNoPar[3];
								
									can2_MC1TxCarNo[0] =  can1_HMITxCarNoPar[2];
									can2_MC1TxCarNo[1] =  can1_HMITxCarNoPar[3];
								}
								break;
							case T_MTD_NODEID:
								if(can1_HMITxCarNoPar[0] ==2 ) //��һ���ֽڱ�ʾ����������
								{
									CarNumber = (uint16_t)can1_HMITxCarNoPar[2]*100+can1_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);
								}
								break;
							case M_MTD_NODEID:
								if(can1_HMITxCarNoPar[0] ==3 ) //��һ���ֽڱ�ʾ����������
								{
									CarNumber = (uint16_t)can1_HMITxCarNoPar[2]*100+can1_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);									
									
									can1_MTxCarNo[0] =  can1_HMITxCarNoPar[2];
									can1_MTxCarNo[1] =  can1_HMITxCarNoPar[3];
								
									can2_MTxCarNo[0] =  can1_HMITxCarNoPar[2];
									can2_MTxCarNo[1] =  can1_HMITxCarNoPar[3];
								}
							case T1_MTD_NODEID:
								if(can1_HMITxCarNoPar[0] ==4 ) //��һ���ֽڱ�ʾ����������
								{
									CarNumber = (uint16_t)can1_HMITxCarNoPar[2]*100+can1_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);									
								}
								break;
							case T2_MTD_NODEID:
								if(can1_HMITxCarNoPar[0] ==5 ) //��һ���ֽڱ�ʾ����������
								{
									CarNumber = (uint16_t)can1_HMITxCarNoPar[2]*100+can1_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);									
								}
								break;
							case MC2_MTD_NODEID:
								if(can1_HMITxCarNoPar[0] ==6 ) //��һ���ֽڱ�ʾ����������
								{
									CarNumber = (uint16_t)can1_HMITxCarNoPar[2]*100+can1_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);
									
									can1_MC2TxCarNo[0] =  can1_HMITxCarNoPar[2];
									can1_MC2TxCarNo[1] =  can1_HMITxCarNoPar[3];
								
									can2_MC2TxCarNo[0] =  can1_HMITxCarNoPar[2];
									can2_MC2TxCarNo[1] =  can1_HMITxCarNoPar[3];
								}								
								break;

							default :
								break;					
						}
						memcpy(RxCarNo,can1_HMITxCarNoPar,4);//�����ݸ��Ƶ���������	
					}
				}
				break;

			default:
				printf("can1 carNO�����ֵ�ӳ�䲻��!\r\n");
				break;
		}
	}

	else		//can2
	{
		switch(unsused_indextable->index)
		{
			case 0x205E:
				if(can2_HMITxCarNoPar[1] == 1)		//�ڶ��ֽ�Ϊ���õ����� ��1��ʾ�������
				{
					if(memcmp(RxCarNo,can2_HMITxCarNoPar,4)!=0)// ��ʾ���ݲ����,��ִ��
					{
						//debugprintf("can2 car 123 !\r\n");
						switch(ChooseBoard)
						{
							case MC1_MTD_NODEID:
								if(can2_HMITxCarNoPar[0] == 1)
								{
									CarNumber = (uint16_t)can2_HMITxCarNoPar[2]*100+can2_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);
									
									can1_MC1TxCarNo[0] =  can2_HMITxCarNoPar[2];
									can1_MC1TxCarNo[1] =  can2_HMITxCarNoPar[3];
								
									can2_MC1TxCarNo[0] =  can2_HMITxCarNoPar[2];
									can2_MC1TxCarNo[1] =  can2_HMITxCarNoPar[3];									
								}
								break;
							case T_MTD_NODEID:
								if(can2_HMITxCarNoPar[0] == 2)
								{
									CarNumber = (uint16_t)can2_HMITxCarNoPar[2]*100+can2_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);									
								}
								break;
							case M_MTD_NODEID:
								if(can2_HMITxCarNoPar[0] == 3)
								{
									CarNumber = (uint16_t)can2_HMITxCarNoPar[2]*100+can2_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);
									
									can1_MTxCarNo[0] =  can2_HMITxCarNoPar[2];
									can1_MTxCarNo[1] =  can2_HMITxCarNoPar[3];
								
									can2_MTxCarNo[0] =  can2_HMITxCarNoPar[2];
									can2_MTxCarNo[1] =  can2_HMITxCarNoPar[3];									
								}
								break;
							case T1_MTD_NODEID:
								if(can2_HMITxCarNoPar[0] == 4)
								{
									CarNumber = (uint16_t)can2_HMITxCarNoPar[2]*100+can2_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);									
								}
								break;
							case T2_MTD_NODEID:
								if(can2_HMITxCarNoPar[0] == 5)
								{
									CarNumber = (uint16_t)can2_HMITxCarNoPar[2]*100+can2_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);									
								}
								break;
							case MC2_MTD_NODEID:
								if(can2_HMITxCarNoPar[0] == 6)
								{
									CarNumber = (uint16_t)can2_HMITxCarNoPar[2]*100+can2_HMITxCarNoPar[3];
									CarNo = CarNumber%10;
									strcpy(cartype,CarTypeArr[CarNo-1]);
									
									can1_MC2TxCarNo[0] =  can2_HMITxCarNoPar[2];
									can1_MC2TxCarNo[1] =  can2_HMITxCarNoPar[3];
								
									can2_MC2TxCarNo[0] =  can2_HMITxCarNoPar[2];
									can2_MC2TxCarNo[1] =  can2_HMITxCarNoPar[3];									
								}
								break;								
							default :
								break;					
						}
						memcpy(RxCarNo,can2_HMITxCarNoPar,4);//�����ݸ��Ƶ���������						
					}					
				}
				break;

			default:
				printf("can2 carNO�����ֵ�ӳ�䲻��!\r\n");
				break;
		}		
	}
	
	return  0;
}

UNS32 Can3SnedProDataCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static uint8_t RxHMITxProData[8] = {0};
	
	if(d == &MTDApp1CAN1Slave_Data)		//can1
	{
		if(memcmp(RxHMITxProData,can1_HMITxProData,8) !=0)  // ��ʾ���ݲ����,��ִ��
		{

			/*ATCЭ������*/
			can3_TxATCSdr2[0] = can1_HMITxProData[3]; //�г�������  16����
			can3_TxATCSdr2[1] = can1_HMITxProData[4];
			
			can3_TxATCSdr2[3] = can1_HMITxProData[2]; //����״̬   
			can3_TxATCSdr2[4] = can1_HMITxProData[5]; //ƽ���˳���	
			MTDCAN3Master_Data.PDO_status[8].last_message.cob_id = 0; //ǿ�Ʒ���
			sendOnePDOevent(&MTDCAN3Master_Data,8);		//can3,0x18A,8

			/*�ƶ�������ָ��ת��*/
			can3_TxBCUCmd[0] = can1_HMITxProData[6];  // ����BCU�ƶ�������
			MTDCAN3Master_Data.PDO_status[4].last_message.cob_id = 0;    	//0x186
			sendOnePDOevent(&MTDCAN3Master_Data,4);				
			
			/*�����ٶ�����*/
			can3_TxDoorSpeedTimBit[1] = can1_HMITxProData[7]; 						//����DOOR���ٶ�
			MTDCAN3Master_Data.PDO_status[2].last_message.cob_id = 0;    	//0x184
			sendOnePDOevent(&MTDCAN3Master_Data,2);	

			memcpy(RxHMITxProData,can1_HMITxProData,8);			//�����ݸ��Ƶ���������			
		}
	}
	
	else  //can2
	{
		if(memcmp(RxHMITxProData,can2_HMITxProData,8) !=0)  // ��ʾ���ݲ����,��ִ��	
		{
			/*ATCЭ������*/
			can3_TxATCSdr2[0] = can2_HMITxProData[3]; //�г�������  16����
			can3_TxATCSdr2[1] = can2_HMITxProData[4];
			
			can3_TxATCSdr2[3] = can2_HMITxProData[2]; //����״̬   
			can3_TxATCSdr2[4] = can2_HMITxProData[5]; //ƽ���˳���	
			MTDCAN3Master_Data.PDO_status[8].last_message.cob_id = 0; //ǿ�Ʒ���
			sendOnePDOevent(&MTDCAN3Master_Data,8);		//can3,0x18A,8
			
 			/*�ƶ�������ָ��ת��*/
			can3_TxBCUCmd[0] = can2_HMITxProData[6];  // ����BCU�ƶ�������
			MTDCAN3Master_Data.PDO_status[4].last_message.cob_id = 0;    	//0x186
			sendOnePDOevent(&MTDCAN3Master_Data,4);			
			
			/*�����ٶ�����*/
			can3_TxDoorSpeedTimBit[1] = can2_HMITxProData[7]; 						//����DOOR���ٶ�
			MTDCAN3Master_Data.PDO_status[2].last_message.cob_id = 0;    	//0x184
			sendOnePDOevent(&MTDCAN3Master_Data,2);		

			memcpy(RxHMITxProData,can2_HMITxProData,8);			//�����ݸ��Ƶ���������			
		}
	}
	
	return 0;
}

/*���Ϳյ��¶�*/
UNS32 Can3SnedACCSdrCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static u8 RxAccTemp[1]={0};
	
	if(d == &MTDApp1CAN1Slave_Data)		//can1
	{
		if(memcmp(RxAccTemp,can1_HMITxAccTemp,1) !=0)  // ��ʾ���ݲ����,��ִ��
		{
			if((10 <= can1_HMITxAccTemp[0]) && (can1_HMITxAccTemp[0]<=30))  //����Ŀ���¶���Ч
			{
				can3_TxAccTempTimBit[0] |= 0x02;	//Ŀ���¶���Ч ��1 
			}		
			else
			{
			  can3_TxAccTempTimBit[0] &= 0xFD;	//Ŀ���¶���Ч ��0
			}
			
			can3_TxAccTempTimBit[1] = can1_HMITxAccTemp[0];	//��������¶�ֵ

			MTDCAN3Master_Data.PDO_status[3].last_message.cob_id = 0; //ǿ�Ʒ���
			sendOnePDOevent(&MTDCAN3Master_Data,3);		//can3,0x185,3
			
			can3_TxAccTempTimBit[0] &= 0xFD;	//Ŀ���¶���Ч ��0
			
			memcpy(RxAccTemp,can1_HMITxAccTemp,1);			//�����ݸ��Ƶ���������			
		}
	}
	
	else		//can2
	{
		if(memcmp(RxAccTemp,can2_HMITxAccTemp,1) !=0)  // ��ʾ���ݲ����,��ִ��
		{
			if((10 <= can2_HMITxAccTemp[0]) && (can2_HMITxAccTemp[0]<=30))  //����Ŀ���¶���Ч
			{
				can3_TxAccTempTimBit[0] |= 0x02;	//Ŀ���¶���Ч ��1 
			}		
			else
			{
			  can3_TxAccTempTimBit[0] &= 0xFD;	//Ŀ���¶���Ч ��0
			}
			
			can3_TxAccTempTimBit[1] = can2_HMITxAccTemp[0];	//��������¶�ֵ

			MTDCAN3Master_Data.PDO_status[3].last_message.cob_id = 0; //ǿ�Ʒ���
			sendOnePDOevent(&MTDCAN3Master_Data,3);		//can3,0x185,3	

			can3_TxAccTempTimBit[0] &= 0xFD;	//Ŀ���¶���Ч ��0		

			memcpy(RxAccTemp,can2_HMITxAccTemp,1);			//�����ݸ��Ƶ���������					
		}	
	}
	return 0;
}

/*���� ɾ�����ϼ�¼����*/
UNS32 Can3SnedDelCmdCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static u8 RxDelCmd[3]={0};

	if(d == &MTDApp1CAN1Slave_Data)   //can1
	{
		if(memcmp(RxDelCmd,can1_HMITxDelRecordCmd,3) !=0)  // ��ʾ���ݲ����,��ִ��
		{
			switch(can1_HMITxDelRecordCmd[0])
			{
				case 0:
					break;
				
				case 1:     //ɾ�����ϼ�¼
					switch(can1_HMITxDelRecordCmd[1])
					{
						case 1:      //��������
							break;
						
						case 2:      //VVVF1
							break;
						
						case 3:      //VVVF2
							break;
						
						case 4:			 //SIV
							break;
						
						case 5:      //�ƶ�װ��
							if(can1_HMITxDelRecordCmd[2] == 1)
							{
								can3_TxBCUCmd[1] |=0x01;  // ���״̬����   �� 1
								MTDCAN3Master_Data.PDO_status[4].last_message.cob_id = 0; //ǿ�Ʒ���
								sendOnePDOevent(&MTDCAN3Master_Data,4);		//can3,0x186,4		
								can3_TxBCUCmd[1] &=0xFE;  // ���״̬����   �� 0 
							}								
							break;
						
						default:
							break;
					}
					break;
				
				case 2:    //ɾ�����м�¼
					switch(can1_HMITxDelRecordCmd[1])
					{
						case 1:
							break;
						
						case 2:
							break;
						
						case 3:
							break;
						
						case 4:
							break;
						
						case 5:
							break;
						
						case 6:
							break;
						
						case 7:
							break;
						
						default:
							break;
					}
					break;
				
				case 3:		 //ɾ���Ӽ��ٶ�
					break;
				
				default:  
					printf("Del Cmd Err!\r\n");
					break;
			}
			memcpy(RxDelCmd,can1_HMITxDelRecordCmd,3);			//�����ݸ��Ƶ���������
		}	
	}
	
	else  //can2
	{
		if(memcmp(RxDelCmd,can2_HMITxDelRecordCmd,3) !=0)  // ��ʾ���ݲ����,��ִ��
		{
			switch(can2_HMITxDelRecordCmd[0])
			{
				case 0:
					break;
				
				case 1:     //ɾ�����ϼ�¼
					switch(can2_HMITxDelRecordCmd[1])
					{
						case 1:      //��������
							break;
						
						case 2:      //VVVF1
							break;
						
						case 3:      //VVVF2
							break;
						
						case 4:			 //SIV
							break;
						
						case 5:      //�ƶ�װ��
							if(can2_HMITxDelRecordCmd[2] == 1)
							{
								can3_TxBCUCmd[1] |=0x01;  // ���״̬����   �� 1
								MTDCAN3Master_Data.PDO_status[4].last_message.cob_id = 0; //ǿ�Ʒ���
								sendOnePDOevent(&MTDCAN3Master_Data,4);		//can3,0x186,4		
								can3_TxBCUCmd[1] &=0xFE;  // ���״̬����   �� 0
							}								
							break;
						
						default:
							break;
					}
					break;
				
				case 2:    //ɾ�����м�¼
					switch(can2_HMITxDelRecordCmd[1])
					{
						case 1:
							break;
						
						case 2:
							break;
						
						case 3:
							break;
						
						case 4:
							break;
						
						case 5:
							break;
						
						case 6:
							break;
						
						case 7:
							break;
						
						default:
							break;
					}
					break;
				
				case 3:		 //ɾ���Ӽ��ٶ�
					break;
				
				default:  
					printf("Del Cmd Err!\r\n");
					break;			
			}
			memcpy(RxDelCmd,can2_HMITxDelRecordCmd,3);			//�����ݸ��Ƶ���������
		}	
	}
	
	return 0;
}

/*����HMI��PIS sdr����Э��ص�����*/
UNS32 Can3SendPISSdrCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static u8 RxPISSdr1[2] = {0};
	static u8 RxPISSdr2[8] = {0};
	
	if(d == &MTDApp1CAN1Slave_Data) //can1
	{
		switch(unsused_indextable->index) //��ʾ�ĸ������� 
		{
			case 0x207D:    //sdr1
				if(memcmp(RxPISSdr1,can1_HMITxStationInfrom1,2)!=0)   // ��ʾ���ݲ����,��ִ��
				{
					memcpy(can3_TxPISSdr1,can1_HMITxStationInfrom1,2);
					MTDCAN3Master_Data.PDO_status[9].last_message.cob_id = 0; //ǿ�Ʒ���
					sendOnePDOevent(&MTDCAN3Master_Data,9);		//can3,0x18B,9	

					memcpy(RxPISSdr1,can1_HMITxStationInfrom1,2);					
				}	
				break;
					
			case 0x207E:    //sdr2
				if(memcmp(RxPISSdr2,can1_HMITxStationInfrom2,8)!=0)   // ��ʾ���ݲ����,��ִ��
				{
					/*����PIS sdr2 ����*/
					memcpy(can3_TxPISSdr2,can1_HMITxStationInfrom2,8);
					MTDCAN3Master_Data.PDO_status[10].last_message.cob_id = 0; //ǿ�Ʒ���
					sendOnePDOevent(&MTDCAN3Master_Data,10);		//can3,0x18C,10		

					/*����ATC�� �г����*/
					can3_TxATCSdr2[2] = ((((u16)can1_HMITxStationInfrom2[0]<<8) + (u16)can1_HMITxStationInfrom2[1])>>4)&0xFF;  //����BCD����ӣ�  �õ�����BCD��
					MTDCAN3Master_Data.PDO_status[8].last_message.cob_id = 0; //ǿ�Ʒ���
					sendOnePDOevent(&MTDCAN3Master_Data,8);		//can3,0x18A,8	
					
					memcpy(RxPISSdr2,can1_HMITxStationInfrom2,8);
				}
				break;
			
			default:
				break;
		}
	}
	
	else   //can2
	{
		switch(unsused_indextable->index) //��ʾ�ĸ������� 
		{
			case 0x207D:    //sdr1
				if(memcmp(RxPISSdr1,can2_HMITxStationInfrom1,2)!=0)   // ��ʾ���ݲ����,��ִ��
				{
					memcpy(can3_TxPISSdr1,can2_HMITxStationInfrom1,2);
					MTDCAN3Master_Data.PDO_status[9].last_message.cob_id = 0; //ǿ�Ʒ���
					sendOnePDOevent(&MTDCAN3Master_Data,9);		//can3,0x18B,9	
						
					memcpy(RxPISSdr1,can2_HMITxStationInfrom1,2);
				}				
				break;
					
			case 0x207E:    //sdr2
				if(memcmp(RxPISSdr2,can2_HMITxStationInfrom2,8)!=0)   // ��ʾ���ݲ����,��ִ��
				{
					memcpy(can3_TxPISSdr2,can2_HMITxStationInfrom2,8);
					MTDCAN3Master_Data.PDO_status[10].last_message.cob_id = 0; //ǿ�Ʒ���
					sendOnePDOevent(&MTDCAN3Master_Data,10);		//can3,0x18C,10		

					/*����ATC�� �г����*/
					can3_TxATCSdr2[2] = ((((u16)can2_HMITxStationInfrom2[0]<<8) + (u16)can2_HMITxStationInfrom2[1])>>4)&0xFF;  //����BCD����ӣ�  �õ�����BCD��
					MTDCAN3Master_Data.PDO_status[8].last_message.cob_id = 0; //ǿ�Ʒ���
					sendOnePDOevent(&MTDCAN3Master_Data,8);		//can3,0x18A,8		

					memcpy(RxPISSdr2,can2_HMITxStationInfrom2,8);
				}
				break;
			
			default:
				break;
		}	
	}
	
	return 0;
}

 /*����MDCU���͵�ATC��Ҫ��DI����*/ 
UNS32 MDCURxATCDICallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static u8 RxMC1DI[1] ={0};
	static u8 RxMDI[1] ={0};	
	static u8 RxMC2DI[1] ={0};
	
	if(d == &MTDApp1CAN1Slave_Data)   //can1
	{
		switch(unsused_indextable->index)
		{
			case 0x2000:
				if(RxMC1DI[0] != ((can1_DI_MC1[1]>>4)&0x01))   // ��ʾ���ݲ����,��ִ��
				{
					DI_VVVFPowertx_B1_10 = (can1_DI_MC1[1]>>4)&0x01;	//MC1,VVVF��Դ��Ч
					
					RxMC1DI[0] = DI_VVVFPowertx_B1_10;
				}				
				break;
			
			case 0x2002:
				if(RxMDI[0] !=((can1_DI_M>>2)&0x01))   // ��ʾ���ݲ����,��ִ��
				{
					DI_VVVFPowertx_B1_30 = (can1_DI_M>>2)&0x01;	//M,VVVF��Դ��Ч 
					
					RxMDI[0]=  DI_VVVFPowertx_B1_30;
				}
				break;
			
			case 0x2005:
				if(RxMC2DI[0] != ((can1_DI_MC2[1]>>4)&0x01))    // ��ʾ���ݲ����,��ִ��
				{
					DI_VVVFPowertx_B1_60 = (can1_DI_MC2[1]>>4)&0x01;	//MC2,VVVF��Դ��Ч
					
					RxMC2DI[0] = DI_VVVFPowertx_B1_60;
				}
				break;
			
			default:
				break;
		}
	}
	
	else  //can2
	{
		switch(unsused_indextable->index)
		{
			case 0x2000:
				if(RxMC1DI[0] != ((can2_DI_MC1[1]>>4)&0x01))   // ��ʾ���ݲ����,��ִ��
				{
					DI_VVVFPowertx_B1_10 = (can2_DI_MC1[1]>>4)&0x01;	//MC1,VVVF��Դ��Ч
					
					RxMC1DI[0] = DI_VVVFPowertx_B1_10;
				}				
				break;
			
			case 0x2002:
				if(RxMDI[0] !=((can2_DI_M>>2)&0x01))   // ��ʾ���ݲ����,��ִ��
				{
					DI_VVVFPowertx_B1_30 = (can2_DI_M>>2)&0x01;	//M,VVVF��Դ��Ч 
					
					RxMDI[0]=  DI_VVVFPowertx_B1_30;
				}
				break;
			
			case 0x2005:
				if(RxMC2DI[0] != ((can2_DI_MC2[1]>>4)&0x01))    // ��ʾ���ݲ����,��ִ��
				{
					DI_VVVFPowertx_B1_60 = (can2_DI_MC2[1]>>4)&0x01;	//MC2,VVVF��Դ��Ч
					
					RxMC2DI[0] = DI_VVVFPowertx_B1_60;
				}
				break;
			
			default:
				break;
		}
	}
	
	return 0;
}	

/*����MDCU���͵�ATC��Ҫ�����豸 ��������*/
UNS32 MDCURxATCSbuFaultCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
  static u8 RxMC1SubFault[1]={0};
	static u8 RxTSubFault[1]={0};
	static u8 RxMSubFault[1]={0};
	static u8 RxT1SubFault[1]={0};
	static u8 RxT2SubFault[1]={0};
	static u8 RxMC2SubFault[1]={0};
	
	if(d == &MTDApp1CAN1Slave_Data)  //can1
	{
		switch(unsused_indextable->index)
		{
			case 0x2057:
				if(RxMC1SubFault[0] != can1_MC1SubDevFault[1])  // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_10 = (can1_MC1SubDevFault[1]>>6)&0x01;	//MC1 BCUͨ��״̬
					MDCU_VVVF1ComFaultx_B1_10 = (can1_MC1SubDevFault[1]>>5)&0x01; //MC1 VVVF1ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_10 = (can1_MC1SubDevFault[1]>>4)&0x01; //MC1 VVVF2ͨ��״̬
					
					RxMC1SubFault[0] = can1_MC1SubDevFault[1];  //�����ݸ��Ƶ���������
				}                                                 			
				break;
			
			case 0x2058:
				if(RxTSubFault[0] != can1_TSubDevFault[1])   // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_20 = (can1_TSubDevFault[1]>>6)&0x01;	//T BCUͨ��״̬
					
					RxTSubFault[0] = can1_TSubDevFault[1];  //�����ݸ��Ƶ���������				
				}
				break;
			
			case 0x2059:
				if(RxMSubFault[0] != can1_MSubDevFault[1])  // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_30 = (can1_MSubDevFault[1]>>6)&0x01;	//M BCUͨ��״̬
					MDCU_VVVF1ComFaultx_B1_30 = (can1_MSubDevFault[1]>>5)&0x01; //M VVVF1ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_30 = (can1_MSubDevFault[1]>>4)&0x01; //M VVVF2ͨ��״̬
					
					RxMSubFault[0] = can1_MSubDevFault[1];  //�����ݸ��Ƶ���������
				} 
				break;
			
			case 0x205A:
				if(RxT1SubFault[0] != can1_T1SubDevFault[1])   // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_40 = (can1_T1SubDevFault[1]>>6)&0x01;	//T1 BCUͨ��״̬
					
					RxT1SubFault[0] = can1_T1SubDevFault[1];  //�����ݸ��Ƶ���������				
				}				
				break;
			
			case 0x205B:
				if(RxT2SubFault[0] != can1_T2SubDevFault[1])   // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_50 = (can1_T2SubDevFault[1]>>6)&0x01;	//T2 BCUͨ��״̬
					
					RxT2SubFault[0] = can1_T2SubDevFault[1];  //�����ݸ��Ƶ���������				
				}						
				break;
			
			case 0x205C:
				if(RxMC2SubFault[0] != can1_MC2SubDevFault[1])  // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_60 = (can1_MC2SubDevFault[1]>>6)&0x01;	//MC2 BCUͨ��״̬
					
					MDCU_VVVF1ComFaultx_B1_60 = (can1_MC2SubDevFault[1]>>5)&0x01; //MC2 VVVF1ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_60 = (can1_MC2SubDevFault[1]>>4)&0x01; //MC2 VVVF2ͨ��״̬
					
					RxMC2SubFault[0] = can1_MC2SubDevFault[1];  //�����ݸ��Ƶ���������
				} 				
				break;
			
			default:
				break;
		}
	}
	
	else  //can2
	{
		switch(unsused_indextable->index)
		{
			case 0x2057:
				if(RxMC1SubFault[0] != can2_MC1SubDevFault[1])  // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_10 = (can2_MC1SubDevFault[1]>>6)&0x01;	//MC1 BCUͨ��״̬
					MDCU_VVVF1ComFaultx_B1_10 = (can2_MC1SubDevFault[1]>>5)&0x01; //MC1 VVVF1ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_10 = (can2_MC1SubDevFault[1]>>4)&0x01; //MC1 VVVF2ͨ��״̬
					
					RxMC1SubFault[0] = can2_MC1SubDevFault[1];  //�����ݸ��Ƶ���������
				}                                                 			
				break;
			
			case 0x2058:
				if(RxTSubFault[0] != can2_TSubDevFault[1])   // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_20 = (can2_TSubDevFault[1]>>6)&0x01;	//T BCUͨ��״̬
					
					RxTSubFault[0] = can2_TSubDevFault[1];  //�����ݸ��Ƶ���������				
				}
				break;
			
			case 0x2059:
				if(RxMSubFault[0] != can2_MSubDevFault[1])  // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_30 = (can2_MSubDevFault[1]>>6)&0x01;	//M BCUͨ��״̬
					MDCU_VVVF1ComFaultx_B1_30 = (can2_MSubDevFault[1]>>5)&0x01; //M VVVF1ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_30 = (can2_MSubDevFault[1]>>4)&0x01; //M VVVF2ͨ��״̬
					
					RxMSubFault[0] = can2_MSubDevFault[1];  //�����ݸ��Ƶ���������
				} 
				break;
			
			case 0x205A:
				if(RxT1SubFault[0] != can2_T1SubDevFault[1])   // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_40 = (can2_T1SubDevFault[1]>>6)&0x01;	//T1 BCUͨ��״̬
					
					RxT1SubFault[0] = can2_T1SubDevFault[1];  //�����ݸ��Ƶ���������				
				}				
				break;
			
			case 0x205B:
				if(RxT2SubFault[0] != can2_T2SubDevFault[1])   // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_50 = (can2_T2SubDevFault[1]>>6)&0x01;	//T2 BCUͨ��״̬
					
					RxT2SubFault[0] = can2_T2SubDevFault[1];  //�����ݸ��Ƶ���������				
				}						
				break;
			
			case 0x205C:
				if(RxMC2SubFault[0] != can2_MC2SubDevFault[1])  // ��ʾ���ݲ����,��ִ��
				{
					MDCU_BCUComFaultrx_B1_60 = (can2_MC2SubDevFault[1]>>6)&0x01;	//MC2 BCUͨ��״̬
					
					MDCU_VVVF1ComFaultx_B1_60 = (can2_MC2SubDevFault[1]>>5)&0x01; //MC2 VVVF1ͨ��״̬
					MDCU_VVVF2ComFaultx_B1_60 = (can2_MC2SubDevFault[1]>>4)&0x01; //MC2 VVVF2ͨ��״̬
					
					RxMC2SubFault[0] = can2_MC2SubDevFault[1];  //�����ݸ��Ƶ���������
				} 				
				break;
			
			default:
				break;
		}	
	}
	
	return 0;
}  

/*����MDCU ���͵�VVVFЭ������*/
UNS32 MDCURxATCVVVFProCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	/*MC1*/
  static u8 RxMC1VVVF1PDO1[1] = {0};
	static u8 RxMC1VVVF1PDO2[2] = {0};
	
  static u8 RxMC1VVVF2PDO1[1] = {0};
	static u8 RxMC1VVVF2PDO2[2] = {0};	
	
	/*M*/
  static u8 RxMVVVF1PDO1[1] = {0};
	static u8 RxMVVVF1PDO2[2] = {0};
	
  static u8 RxMVVVF2PDO1[1] = {0};
	static u8 RxMVVVF2PDO2[2] = {0};
	
	/*MC2*/
  static u8 RxMC2VVVF1PDO1[1] = {0};
	static u8 RxMC2VVVF1PDO2[2] = {0};
	
  static u8 RxMC2VVVF2PDO1[1] = {0};
	static u8 RxMC2VVVF2PDO2[2] = {0};	
	
	if(d == &MTDApp1CAN1Slave_Data)     //can1
	{
		switch(unsused_indextable->index)
		{
			case 0x2033:        //MC1_VVVF1_PDO1
				if(RxMC1VVVF1PDO1[0] != ((can1_VVVF1_MC1_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ�� 
				{
					TCU1_CRDtx_SD_B1_10 = (can1_VVVF1_MC1_1[1]>>5)&0x01;
					
					RxMC1VVVF1PDO1[0] = TCU1_CRDtx_SD_B1_10;   //�����ݸ��Ƶ�������
				}
				break;
			
			case 0x2034:        //MC1_VVVF1_PDO2
				if((RxMC1VVVF1PDO2[0] != ((can1_VVVF1_MC1_2[3]>>1)&0x01) ) || (RxMC1VVVF1PDO2[0] !=((can1_VVVF1_MC1_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU1_UCORtx_SD_B1_10 = (can1_VVVF1_MC1_2[3]>>1)&0x01;
					TCU1_BSLBFtx_SD_B1_10 = (can1_VVVF1_MC1_2[4]>>1)&0x01;
					
					RxMC1VVVF1PDO2[0] = TCU1_UCORtx_SD_B1_10;     //�����ݸ��Ƶ�������
					RxMC1VVVF1PDO2[1] = TCU1_BSLBFtx_SD_B1_10;
				}
				break;
			
			case 0x2038:        //MC1_VVVF2_PDO1
				if(RxMC1VVVF2PDO1[0] != ((can1_VVVF2_MC1_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ��  
				{
					TCU2_CRDtx_SD_B1_10 = (can1_VVVF2_MC1_1[1]>>5)&0x01;
					
					RxMC1VVVF2PDO1[0] = TCU2_CRDtx_SD_B1_10;   //�����ݸ��Ƶ�������
				}
				break;

			case 0x2039:        //MC1_VVVF2_PDO2
				if((RxMC1VVVF2PDO2[0] != ((can1_VVVF2_MC1_2[3]>>1)&0x01) ) || (RxMC1VVVF2PDO2[0] !=((can1_VVVF2_MC1_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU2_UCORtx_SD_B1_10 = (can1_VVVF2_MC1_2[3]>>1)&0x01;
					TCU2_BSLBFtx_SD_B1_10 = (can1_VVVF2_MC1_2[4]>>1)&0x01;
					
					RxMC1VVVF2PDO2[0] = TCU2_UCORtx_SD_B1_10;     //�����ݸ��Ƶ�������
					RxMC1VVVF2PDO2[1] = TCU2_BSLBFtx_SD_B1_10;
				}				
				break;

			
			case 0x203D:        //M_VVVF1_PDO1
				if(RxMVVVF1PDO1[0] != ((can1_VVVF1_M_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ�� 
				{
					TCU1_CRDtx_SD_B1_30 = (can1_VVVF1_M_1[1]>>5)&0x01;
					
					RxMVVVF1PDO1[0] = TCU1_CRDtx_SD_B1_30;   //�����ݸ��Ƶ�������
				}				
				break;
			
			case 0x203E:        //M_VVVF1_PDO2
				if((RxMVVVF1PDO2[0] != ((can1_VVVF1_M_2[3]>>1)&0x01) ) || (RxMVVVF1PDO2[0] !=((can1_VVVF1_M_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU1_UCORtx_SD_B1_30 = (can1_VVVF1_M_2[3]>>1)&0x01;
					TCU1_BSLBFtx_SD_B1_30 = (can1_VVVF1_M_2[4]>>1)&0x01;
					
					RxMVVVF1PDO2[0] = TCU1_UCORtx_SD_B1_30;     //�����ݸ��Ƶ�������
					RxMVVVF1PDO2[1] = TCU1_BSLBFtx_SD_B1_30;
				}				
				break;
			
			case 0x2042:        //M_VVVF2_PDO1
				if(RxMVVVF2PDO1[0] != ((can1_VVVF2_M_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ��  
				{
					TCU2_CRDtx_SD_B1_30 = (can1_VVVF2_M_1[1]>>5)&0x01;
					
					RxMVVVF2PDO1[0] = TCU2_CRDtx_SD_B1_30;   //�����ݸ��Ƶ�������
				}				
				break;

			case 0x2043:        //M_VVVF2_PDO2
				if((RxMVVVF2PDO2[0] != ((can1_VVVF2_M_2[3]>>1)&0x01) ) || (RxMVVVF2PDO2[0] !=((can1_VVVF2_M_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU2_UCORtx_SD_B1_30 = (can1_VVVF2_M_2[3]>>1)&0x01;
					TCU2_BSLBFtx_SD_B1_30 = (can1_VVVF2_M_2[4]>>1)&0x01;
					
					RxMVVVF2PDO2[0] = TCU2_UCORtx_SD_B1_30;     //�����ݸ��Ƶ�������
					RxMVVVF2PDO2[1] = TCU2_BSLBFtx_SD_B1_30;
				}					
				break;			
			
			
			case 0x2047:        //MC2_VVVF1_PDO1
				if(RxMC2VVVF1PDO1[0] != ((can1_VVVF1_MC2_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ�� 
				{
					TCU1_CRDtx_SD_B1_60 = (can1_VVVF1_MC2_1[1]>>5)&0x01;
					
					RxMC2VVVF1PDO1[0] = TCU1_CRDtx_SD_B1_60;   //�����ݸ��Ƶ�������
				}				
				break;
			
			case 0x2048:        //MC2_VVVF1_PDO2
				if((RxMC2VVVF1PDO2[0] != ((can1_VVVF1_MC2_2[3]>>1)&0x01) ) || (RxMC2VVVF1PDO2[0] !=((can1_VVVF1_MC2_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU1_UCORtx_SD_B1_60 = (can1_VVVF1_MC2_2[3]>>1)&0x01;
					TCU1_BSLBFtx_SD_B1_60 = (can1_VVVF1_MC2_2[4]>>1)&0x01;
					
					RxMC2VVVF1PDO2[0] = TCU1_UCORtx_SD_B1_60;     //�����ݸ��Ƶ�������
					RxMC2VVVF1PDO2[1] = TCU1_BSLBFtx_SD_B1_60;
				}				
				break;
			
			case 0x204C:        //MC2_VVVF2_PDO1
				if(RxMC2VVVF2PDO1[0] != ((can1_VVVF2_MC2_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ��  
				{
					TCU2_CRDtx_SD_B1_60 = (can1_VVVF2_MC2_1[1]>>5)&0x01;
					
					RxMC2VVVF2PDO1[0] = TCU2_CRDtx_SD_B1_60;   //�����ݸ��Ƶ�������
				}				
				break;
			
			case 0x204D:        //MC2_VVVF2_PDO2
				if((RxMC2VVVF2PDO2[0] != ((can1_VVVF2_MC2_2[3]>>1)&0x01) ) || (RxMC2VVVF2PDO2[0] !=((can1_VVVF2_MC2_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU2_UCORtx_SD_B1_60 = (can1_VVVF2_MC2_2[3]>>1)&0x01;
					TCU2_BSLBFtx_SD_B1_60 = (can1_VVVF2_MC2_2[4]>>1)&0x01;
					
					RxMC2VVVF2PDO2[0] = TCU2_UCORtx_SD_B1_60;     //�����ݸ��Ƶ�������
					RxMC2VVVF2PDO2[1] = TCU2_BSLBFtx_SD_B1_60;
				}						
				break;			
			
			default :
				break;
		}
	
	}
	
	else	//can2
	{
		switch(unsused_indextable->index)
		{
			case 0x2033:        //MC1_VVVF1_PDO1
				if(RxMC1VVVF1PDO1[0] != ((can2_VVVF1_MC1_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ�� 
				{
					TCU1_CRDtx_SD_B1_10 = (can2_VVVF1_MC1_1[1]>>5)&0x01;
					
					RxMC1VVVF1PDO1[0] = TCU1_CRDtx_SD_B1_10;   //�����ݸ��Ƶ�������
				}
				break;
			
			case 0x2034:        //MC1_VVVF1_PDO2
				if((RxMC1VVVF1PDO2[0] != ((can2_VVVF1_MC1_2[3]>>1)&0x01) ) || (RxMC1VVVF1PDO2[0] !=((can2_VVVF1_MC1_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU1_UCORtx_SD_B1_10 = (can2_VVVF1_MC1_2[3]>>1)&0x01;
					TCU1_BSLBFtx_SD_B1_10 = (can2_VVVF1_MC1_2[4]>>1)&0x01;
					
					RxMC1VVVF1PDO2[0] = TCU1_UCORtx_SD_B1_10;     //�����ݸ��Ƶ�������
					RxMC1VVVF1PDO2[1] = TCU1_BSLBFtx_SD_B1_10;
				}
				break;
			
			case 0x2038:        //MC1_VVVF2_PDO1
				if(RxMC1VVVF2PDO1[0] != ((can2_VVVF2_MC1_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ��  
				{
					TCU2_CRDtx_SD_B1_10 = (can2_VVVF2_MC1_1[1]>>5)&0x01;
					
					RxMC1VVVF2PDO1[0] = TCU2_CRDtx_SD_B1_10;   //�����ݸ��Ƶ�������
				}
				break;

			case 0x2039:        //MC1_VVVF2_PDO2
				if((RxMC1VVVF2PDO2[0] != ((can2_VVVF2_MC1_2[3]>>1)&0x01) ) || (RxMC1VVVF2PDO2[0] !=((can2_VVVF2_MC1_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU2_UCORtx_SD_B1_10 = (can2_VVVF2_MC1_2[3]>>1)&0x01;
					TCU2_BSLBFtx_SD_B1_10 = (can2_VVVF2_MC1_2[4]>>1)&0x01;
					
					RxMC1VVVF2PDO2[0] = TCU2_UCORtx_SD_B1_10;     //�����ݸ��Ƶ�������
					RxMC1VVVF2PDO2[1] = TCU2_BSLBFtx_SD_B1_10;
				}				
				break;

			
			case 0x203D:        //M_VVVF1_PDO1
				if(RxMVVVF1PDO1[0] != ((can2_VVVF1_M_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ�� 
				{
					TCU1_CRDtx_SD_B1_30 = (can2_VVVF1_M_1[1]>>5)&0x01;
					
					RxMVVVF1PDO1[0] = TCU1_CRDtx_SD_B1_30;   //�����ݸ��Ƶ�������
				}				
				break;
			
			case 0x203E:        //M_VVVF1_PDO2
				if((RxMVVVF1PDO2[0] != ((can2_VVVF1_M_2[3]>>1)&0x01) ) || (RxMVVVF1PDO2[0] !=((can2_VVVF1_M_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU1_UCORtx_SD_B1_30 = (can2_VVVF1_M_2[3]>>1)&0x01;
					TCU1_BSLBFtx_SD_B1_30 = (can2_VVVF1_M_2[4]>>1)&0x01;
					
					RxMVVVF1PDO2[0] = TCU1_UCORtx_SD_B1_30;     //�����ݸ��Ƶ�������
					RxMVVVF1PDO2[1] = TCU1_BSLBFtx_SD_B1_30;
				}				
				break;
			
			case 0x2042:        //M_VVVF2_PDO1
				if(RxMVVVF2PDO1[0] != ((can2_VVVF2_M_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ��  
				{
					TCU2_CRDtx_SD_B1_30 = (can2_VVVF2_M_1[1]>>5)&0x01;
					
					RxMVVVF2PDO1[0] = TCU2_CRDtx_SD_B1_30;   //�����ݸ��Ƶ�������
				}				
				break;

			case 0x2043:        //M_VVVF2_PDO2
				if((RxMVVVF2PDO2[0] != ((can2_VVVF2_M_2[3]>>1)&0x01) ) || (RxMVVVF2PDO2[0] !=((can2_VVVF2_M_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU2_UCORtx_SD_B1_30 = (can2_VVVF2_M_2[3]>>1)&0x01;
					TCU2_BSLBFtx_SD_B1_30 = (can2_VVVF2_M_2[4]>>1)&0x01;
					
					RxMVVVF2PDO2[0] = TCU2_UCORtx_SD_B1_30;     //�����ݸ��Ƶ�������
					RxMVVVF2PDO2[1] = TCU2_BSLBFtx_SD_B1_30;
				}					
				break;			
			
			
			case 0x2047:        //MC2_VVVF1_PDO1
				if(RxMC2VVVF1PDO1[0] != ((can2_VVVF1_MC2_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ�� 
				{
					TCU1_CRDtx_SD_B1_60 = (can2_VVVF1_MC2_1[1]>>5)&0x01;
					
					RxMC2VVVF1PDO1[0] = TCU1_CRDtx_SD_B1_60;   //�����ݸ��Ƶ�������
				}				
				break;
			
			case 0x2048:        //MC2_VVVF1_PDO2
				if((RxMC2VVVF1PDO2[0] != ((can2_VVVF1_MC2_2[3]>>1)&0x01) ) || (RxMC2VVVF1PDO2[0] !=((can2_VVVF1_MC2_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU1_UCORtx_SD_B1_60 = (can2_VVVF1_MC2_2[3]>>1)&0x01;
					TCU1_BSLBFtx_SD_B1_60 = (can2_VVVF1_MC2_2[4]>>1)&0x01;
					
					RxMC2VVVF1PDO2[0] = TCU1_UCORtx_SD_B1_60;     //�����ݸ��Ƶ�������
					RxMC2VVVF1PDO2[1] = TCU1_BSLBFtx_SD_B1_60;
				}				
				break;
			
			case 0x204C:        //MC2_VVVF2_PDO1
				if(RxMC2VVVF2PDO1[0] != ((can2_VVVF2_MC2_1[1]>>5)&0x01))    //CR1D λ   // ��ʾ���ݲ����,��ִ��  
				{
					TCU2_CRDtx_SD_B1_60 = (can2_VVVF2_MC2_1[1]>>5)&0x01;
					
					RxMC2VVVF2PDO1[0] = TCU2_CRDtx_SD_B1_60;   //�����ݸ��Ƶ�������
				}				
				break;
			
			case 0x204D:        //MC2_VVVF2_PDO2
				if((RxMC2VVVF2PDO2[0] != ((can2_VVVF2_MC2_2[3]>>1)&0x01) ) || (RxMC2VVVF2PDO2[0] !=((can2_VVVF2_MC2_2[4]>>1)&0x01) ))          // UCOR��C λ��BSLB_Fλ
				{
					TCU2_UCORtx_SD_B1_60 = (can2_VVVF2_MC2_2[3]>>1)&0x01;
					TCU2_BSLBFtx_SD_B1_60 = (can2_VVVF2_MC2_2[4]>>1)&0x01;
					
					RxMC2VVVF2PDO2[0] = TCU2_UCORtx_SD_B1_60;     //�����ݸ��Ƶ�������
					RxMC2VVVF2PDO2[1] = TCU2_BSLBFtx_SD_B1_60;
				}						
				break;			
			
			default :
				break;
		}	
	}
	
	return 0;
}  

/*����MDCU ���͵�BCUЭ������*/
UNS32 MDCURxATCBCUProCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
	static u8 RxMC1BCUData[1]={0};
	static u8 RxMC2BCUData[1]={0};		
	
	if(d == &MTDApp1CAN1Slave_Data)		//can1
	{
	   switch(unsused_indextable->index)
		 {
			 case 0x2015:        //MC1_BCU
				 if(RxMC1BCUData[0] != can1_BCU_MC1_1[2])    //��ʾ���ݲ����,��ִ��
				 {
					 BCU_Axle1Skidtx_SD_B1_10 = (can1_BCU_MC1_1[2]>>6)&0x01;
					 BCU_Axle2Skidtx_SD_B1_10 = (can1_BCU_MC1_1[2]>>5)&0x01;
					 BCU_Axle3Skidtx_SD_B1_10 = (can1_BCU_MC1_1[2]>>4)&0x01;
					 BCU_Axle4Skidtx_SD_B1_10 = (can1_BCU_MC1_1[2]>>3)&0x01;
					 
					 RxMC1BCUData[0] = can1_BCU_MC1_1[2];      //�����ݸ��Ƶ�������
				 }
				 break ;
					 
			 case 0x2024:       //MC2_BCU
				 if(RxMC2BCUData[0] != can1_BCU_MC2_1[2])  //��ʾ���ݲ����,��ִ��
				 {
					 BCU_Axle1Skidtx_SD_B1_60 = (can1_BCU_MC2_1[2]>>6)&0x01;
					 BCU_Axle2Skidtx_SD_B1_60 = (can1_BCU_MC2_1[2]>>5)&0x01;
					 BCU_Axle3Skidtx_SD_B1_60 = (can1_BCU_MC2_1[2]>>4)&0x01;
					 BCU_Axle4Skidtx_SD_B1_60 = (can1_BCU_MC2_1[2]>>3)&0x01;
					 
					 RxMC2BCUData[0] = can1_BCU_MC2_1[2];      //�����ݸ��Ƶ�������				 
				 }
				 break;
			 
			 default:
				 break;
		 }
	}
	
	else  //can2
	{
	   switch(unsused_indextable->index)
		 {
			 case 0x2015:        //MC1_BCU
				 if(RxMC1BCUData[0] != can2_BCU_MC1_1[2])    //��ʾ���ݲ����,��ִ��
				 {
					 BCU_Axle1Skidtx_SD_B1_10 = (can2_BCU_MC1_1[2]>>6)&0x01;
					 BCU_Axle2Skidtx_SD_B1_10 = (can2_BCU_MC1_1[2]>>5)&0x01;
					 BCU_Axle3Skidtx_SD_B1_10 = (can2_BCU_MC1_1[2]>>4)&0x01;
					 BCU_Axle4Skidtx_SD_B1_10 = (can2_BCU_MC1_1[2]>>3)&0x01;
					 
					 RxMC1BCUData[0] = can2_BCU_MC1_1[2];      //�����ݸ��Ƶ�������
				 }
				 break ;
					 
			 case 0x2024:       //MC2_BCU
				 if(RxMC2BCUData[0] != can2_BCU_MC2_1[2])  //��ʾ���ݲ����,��ִ��
				 {
					 BCU_Axle1Skidtx_SD_B1_60 = (can2_BCU_MC2_1[2]>>6)&0x01;
					 BCU_Axle2Skidtx_SD_B1_60 = (can2_BCU_MC2_1[2]>>5)&0x01;
					 BCU_Axle3Skidtx_SD_B1_60 = (can2_BCU_MC2_1[2]>>4)&0x01;
					 BCU_Axle4Skidtx_SD_B1_60 = (can2_BCU_MC2_1[2]>>3)&0x01;
					 
					 RxMC2BCUData[0] = can2_BCU_MC2_1[2];      //�����ݸ��Ƶ�������				 
				 }
				 break;
			 
			 default:
				 break;
		 }	
	}
	
	return 0;
}

/*�����־��Ļص�����*/
UNS32 SetWhellCallBack(CO_Data* d, const indextable * unsused_indextable, UNS8 unsused_bSubindex)
{
//	static u8 RxWhell[6] ={0};
//	u8 i=0;
//	if(d == &MTDApp1CAN1Slave_Data)		//can1
//	{
//		for(i=0;i<6;i++)
//		{
//			if(can1_HMITxWhell[i] !=0)
//			{
//				break;
//			}
//		}	
//	}
//	else
//	{
//		for(i=0;i<6;i++)
//		{
//			if(can2_HMITxWhell[i] !=0)
//			{
//				break;
//			}
//		}		
//	}
//	
//	if(i>=6)	//���ݶ���0����ֱ���˳�
//	{
//		return 0;
//	}
//	
//	if(d == &MTDApp1CAN1Slave_Data)		//can1
//	{
//		if(memcmp(RxWhell,can1_HMITxWhell,6) !=0)  // ��ʾ���ݲ����,��ִ��
//		{
//			for(i=0;i<6;i++)
//			{
//				printf("can1 Rx %d Car Whell %d\r\n",i+1,can1_HMITxWhell[i]);
//			}
//		memcpy(RxWhell,can1_HMITxWhell,6);			//�����ݸ��Ƶ���������	
//		}
//	}
//	
//	else		//can2
//	{
//		if(memcmp(RxWhell,can2_HMITxWhell,6) !=0)  // ��ʾ���ݲ����,��ִ��
//		{
//			for(i=0;i<6;i++)
//			{
//				printf("can2 Rx %d Car Whell %d\r\n",i+1,can2_HMITxWhell[i]);
//			}
//		memcpy(RxWhell,can2_HMITxWhell,6);			//�����ݸ��Ƶ���������	
//		}	
//	}
	return 0;
}




