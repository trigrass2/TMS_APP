#include "OLED_Menu.h"
#include "MTD_CPU_Key.h"
#include "OLED_SSD1325.h"
#include "RTC_ISL1208.h"
#include "applicfg.h"
#include "MTDApp1CAN1Slave.h"
#include "MTDApp1CAN2Slave.h"
#include "Uart.h"
#include "stdbool.h"
#include "os.h"

extern u8 ChooseBoard;
extern u8 can1CarNumberFlag ;
extern u8 can2CarNumberFlag ;
extern u8 can1UpdateTimeFlag;
extern u8 can2UpdateTimeFlag;

	/*�����*/
extern	u16		CarNumber;
extern	u8    CarNo;
extern	char 	cartype[10];

u16	LocalCarNumber;		//���ڱ��水������ʱ��ֵ
u8 	LocalCarNo;
char LocalCartype[10];
const char CarTypeArr[6][10] = {"MC1(1)","T(4)","M(3)","T1(5)","T2(6)","MC2(2)"};

void (*CurrentOperationIndex)(void);  //���庯��ָ��
u8 KeyFuncIndex=0;				//ȫ�ֱ�������¼������������,Ĭ��Ϊ0
s8 ChooseUpDownCount=1;		//ѡ�����ý����up������
u8 CarSetUpFlag=false;		//������ŵ�����	��up��־
u8 CarSetDownFlag=false;	//������ŵ�����	��Down��־
s8 CarNumMenuOkKeyCount=0;//����������ý���OK�����ļ���
u8 CarSetOk1Flag=false;		//�������ñ�־λ
u8 CarSetOk2Flag=false;		//�������ñ�־λ
u8 CarSetOk3Flag=false;		//�������ñ�־λ

Menu_table  MenuTable[5]=
{
  {0,0,0,0,1,(*MainMenu)}, 			//0	��1�� ���˵�
	{1,1,1,2,0,(*ChooseSetMeun)},	//1 ��2��	ѡ�����ò˵�(��1�����Ӳ˵�)
	{2,2,2,2,1,(*SetCarMenu)},		//2 ��3��	������ţ����ţ����͵�����			
};

/*
�������ò˵�
*/
void  KeySetMenu(void)
{
	u8 KeyValue=0;		
	KeyValue=MTDCPU_KEY_Scan(0);	//��ȡ����ֵ
	switch(KeyValue)
	{
		case MTDCPU_UP:					//�ϰ���
			switch(KeyFuncIndex)	//�жϵ�ǰ�����ڵĽ���
			{
				case 1:
					ChooseUpDownCount--;		//���Ϸ�ҳ����ʹ��--
					if(ChooseUpDownCount<1)
					{
						ChooseUpDownCount=3;
					}
					break;					
				case 2:
					CarSetUpFlag = true;
					break;
			}
			KeyFuncIndex = MenuTable[KeyFuncIndex].Up;
			break;
		case MTDCPU_DOWN:			//�°���
			switch(KeyFuncIndex)//�жϵ�ǰ�����ڵĽ���
			{
				case 1:
					ChooseUpDownCount++;	//���·�ҳ����ʹ��--
					if(ChooseUpDownCount>3)
					{
						ChooseUpDownCount=1;
					}
					break;
				case 2:
					CarSetDownFlag=true;
					break;
			}			
			KeyFuncIndex = MenuTable[KeyFuncIndex].Down;
			break;
		case MTDCPU_SET:
			KeyFuncIndex = MenuTable[KeyFuncIndex].Exit;
			break;
		case MTDCPU_OK:
			switch(KeyFuncIndex)
			{
				case 2:
					switch(ChooseUpDownCount)		//�ж���ѡ�����
					{
						case 1:		//�����������

							CarNumMenuOkKeyCount++;
		//							if(CarNumMenuOkKeyCount>3)
		//							{
		//								//CarNumMenuOkKeyCount=0;	//��λ
		//								MenuTable[KeyFuncIndex].Enter = 0;	//���ص�������
		//							}
							if(CarNumMenuOkKeyCount>3)
							{
								CarSetOk1Flag = true;	
							}
							break;
						case 2:		//��������
							CarSetOk2Flag = true;	
							break;
						case 3: 	//��������
							CarSetOk3Flag = true;	
							break;
					};
					break;
			}
			KeyFuncIndex = MenuTable[KeyFuncIndex].Enter;
			break;
	}
	CurrentOperationIndex = MenuTable[KeyFuncIndex].CurrentOperation;
	(*CurrentOperationIndex)();		//ִ�е�ǰ��������
}

/*������*/
void  MainMenu(void)
{
	/*RTC ����*/
	static 	RTC_TIME time;
	char pbuf[20];
	
	CPU_SR_ALLOC();
								
	if(CarSetOk1Flag==true)		//����ֵ
	{					
		debugprintf("111!\r\n");
		CarSetOk1Flag = false;
		
		if(CarNumber != LocalCarNumber)
		{
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					can1_MC1TxCarNo[0] = LocalCarNumber/100;
					can1_MC1TxCarNo[1] = LocalCarNumber%100;
					can2_MC1TxCarNo[0] = LocalCarNumber/100;
					can2_MC1TxCarNo[1] = LocalCarNumber%100;				
				
				/*WHH ���ﲻ��Ҫֱ�ӷ��ͣ��ڶ�ʱ���ж��лᷢ�����������ŵ�*/
				//					/* force emission of PDO by artificially changing last emitted */
				//					MTDApp1CAN1Slave_Data.PDO_status[75].last_message.cob_id = 0;	
				//					MTDApp1CAN2Slave_Data.PDO_status[75].last_message.cob_id = 0;	
				//					
				//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,75);		//pdo 0x200  75
				//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,75);				
					break;
				
				case T_MTD_NODEID:
					break;
				
				case M_MTD_NODEID:
					can1_MTxCarNo[0] = LocalCarNumber/100;
					can1_MTxCarNo[1] = LocalCarNumber%100;		
					can2_MTxCarNo[0] = LocalCarNumber/100;
					can2_MTxCarNo[1] = LocalCarNumber%100;					
					break;
				
				case T1_MTD_NODEID:
					break;
				
				case T2_MTD_NODEID:
					break;
				
				case MC2_MTD_NODEID:
					can1_MC2TxCarNo[0] = LocalCarNumber/100;
					can1_MC2TxCarNo[1] = LocalCarNumber%100;	
					can2_MC2TxCarNo[0] = LocalCarNumber/100;
					can2_MC2TxCarNo[1] = LocalCarNumber%100;					
					break;
			}
		}
		
		CarNumber =  LocalCarNumber;
		CarNo = LocalCarNo;
		strcpy(cartype,LocalCartype);		//whh ���ӹ��� ��������
	}


	else if(CarSetOk2Flag == true)		//����ֵ
	{
		debugprintf("222!\r\n");
		CarSetOk2Flag = false;
		
		if(CarNumber != LocalCarNumber)
		{
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					can1_MC1TxCarNo[0] = LocalCarNumber/100;
					can1_MC1TxCarNo[1] = LocalCarNumber%100;
					can2_MC1TxCarNo[0] = LocalCarNumber/100;
					can2_MC1TxCarNo[1] = LocalCarNumber%100;				
				/*WHH ���ﲻ��Ҫֱ�ӷ��ͣ��ڶ�ʱ���ж��лᷢ�����������ŵ�*/
				//					/* force emission of PDO by artificially changing last emitted */
				//					MTDApp1CAN1Slave_Data.PDO_status[75].last_message.cob_id = 0;	
				//					MTDApp1CAN2Slave_Data.PDO_status[75].last_message.cob_id = 0;	
				//					
				//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,75);		//pdo 0x200  75
				//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,75);				
					break;
				
				case T_MTD_NODEID:
					break;
				
				case M_MTD_NODEID:
					can1_MTxCarNo[0] = LocalCarNumber/100;
					can1_MTxCarNo[1] = LocalCarNumber%100;		
					can2_MTxCarNo[0] = LocalCarNumber/100;
					can2_MTxCarNo[1] = LocalCarNumber%100;					
					break;
				
				case T1_MTD_NODEID:
					break;
				
				case T2_MTD_NODEID:
					break;
				
				case MC2_MTD_NODEID:
					can1_MC2TxCarNo[0] = LocalCarNumber/100;
					can1_MC2TxCarNo[1] = LocalCarNumber%100;	
					can2_MC2TxCarNo[0] = LocalCarNumber/100;
					can2_MC2TxCarNo[1] = LocalCarNumber%100;					
					break;
			}
		}
		
		CarNumber =  LocalCarNumber;
		CarNo = LocalCarNo;
		strcpy(cartype,LocalCartype);		//whh ���ӹ��� ��������
	}	
				
	else if(CarSetOk3Flag == true)		//����ֵ
	{
		debugprintf("333!\r\n");
		CarSetOk3Flag=false;
		
		if(CarNumber != LocalCarNumber)
		{
			switch(ChooseBoard)
			{
				case MC1_MTD_NODEID:
					can1_MC1TxCarNo[0] = LocalCarNumber/100;
					can1_MC1TxCarNo[1] = LocalCarNumber%100;
					can2_MC1TxCarNo[0] = LocalCarNumber/100;
					can2_MC1TxCarNo[1] = LocalCarNumber%100;				
				/*WHH ���ﲻ��Ҫֱ�ӷ��ͣ��ڶ�ʱ���ж��лᷢ�����������ŵ�*/
				//					/* force emission of PDO by artificially changing last emitted */
				//					MTDApp1CAN1Slave_Data.PDO_status[75].last_message.cob_id = 0;	
				//					MTDApp1CAN2Slave_Data.PDO_status[75].last_message.cob_id = 0;	
				//					
				//					sendOnePDOevent(&MTDApp1CAN1Slave_Data,75);		//pdo 0x200  75
				//					sendOnePDOevent(&MTDApp1CAN2Slave_Data,75);				
					break;
				
				case T_MTD_NODEID:
					break;
				
				case M_MTD_NODEID:
					can1_MTxCarNo[0] = LocalCarNumber/100;
					can1_MTxCarNo[1] = LocalCarNumber%100;		
					can2_MTxCarNo[0] = LocalCarNumber/100;
					can2_MTxCarNo[1] = LocalCarNumber%100;				
					break;
				
				case T1_MTD_NODEID:
					break;
				
				case T2_MTD_NODEID:
					break;
				
				case MC2_MTD_NODEID:
					can1_MC2TxCarNo[0] = LocalCarNumber/100;
					can1_MC2TxCarNo[1] = LocalCarNumber%100;		
					can2_MC2TxCarNo[0] = LocalCarNumber/100;
					can2_MC2TxCarNo[1] = LocalCarNumber%100;					
					break;
			}
		}
		
		CarNumber =  LocalCarNumber;
		CarNo = LocalCarNo;
		strcpy(cartype,LocalCartype);		//whh ���ӹ��� ��������
	}
				
	/*�����˵���ʱ��Ҫ�����м��������λ*/
	ChooseUpDownCount=1;			//ѡ�����ý����up������
	CarSetUpFlag=false;		//������ŵ�����	��up��־
	CarSetDownFlag=false;	//������ŵ�����	��Down��־
	CarNumMenuOkKeyCount=0;		//����������ý���OK�����ļ���
	CarSetOk1Flag=false;	
	CarSetOk2Flag=false;	
	CarSetOk3Flag=false;	
	
	/*��ԭʼ���ݸ��Ƶ��˵�������Ҫ�Ĳ���������*/  //�������if����
	strcpy(LocalCartype,cartype);
	LocalCarNumber = CarNumber;
	LocalCarNo = CarNo;
		
	//	if(can1CarNumberFlag == true )
	//	{
	//		debugprintf("can1car !\r\n");
	//		CarNo = can1_Car_Number_1;
	//		CarNumber = can1_Car_Number_2;		//�������
	//		can1CarNumberFlag = false;
	//	}
	//	
	//	if(can2CarNumberFlag == true )
	//	{
	//		debugprintf("can2car !\r\n");
	//		CarNo = can2_Car_Number_1;
	//		CarNumber = can2_Car_Number_2;		//�������
	//		can2CarNumberFlag =false;
	//	}
	//	
	//	if(can1UpdateTimeFlag == true)
	//	{
	//		debugprintf("can1Data !\r\n");
	//		time._year = can1_Car_DateTime[1];
	//		time._month = can1_Car_DateTime[2];
	//		time._day = can1_Car_DateTime[3];
	//		time._hour = can1_Car_DateTime[4];
	//		time._min = can1_Car_DateTime[5];
	//		time._sec = can1_Car_DateTime[6];
	//		Manual_Set_Time(&time);
	//		can1UpdateTimeFlag = false;
	//	}
	//	
	//	if(can2UpdateTimeFlag == true)
	//	{
	//		debugprintf("can2Data !\r\n");
	//		time._year = can1_Car_DateTime[1];
	//		time._month = can1_Car_DateTime[2];
	//		time._day = can1_Car_DateTime[3];
	//		time._hour = can1_Car_DateTime[4];
	//		time._min = can1_Car_DateTime[5];
	//		time._sec = can1_Car_DateTime[6];
	//		Manual_Set_Time(&time);
	//		can2UpdateTimeFlag =false;
	//	}
	
	//�����ٽ��� ����Ӧ��д������
	OS_CRITICAL_ENTER();
		ISL1208_GetTime(&time);		//��ȡʱ��
	OS_CRITICAL_EXIT();//�˳��ٽ���
	
	Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
	
	//sprintf(pbuf,"%d%s%02d%s%02d%02d%02d%02d",time._year+2000,"-",time._month,"-",time._day,time._hour,time._min,time._sec);
	sprintf(pbuf,"%d%s%02d%s%02d%s%02d%s%02d",time._year+2000,"-",time._month,"-",time._day," ",time._hour,":",time._min);
		
	Draw_Component(COMPONENT_LABEL_ENABLED,  1, 0,  0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);     /* �����ı� */

	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,"%s%04d","�������: ",CarNumber);	
	Draw_Component(COMPONENT_LABEL_ENABLED,  2, 16, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf); 	  /* �����ı� */
	
	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,"%s%01d","��    ��: ",CarNo);		
	Draw_Component(COMPONENT_LABEL_ENABLED,  3, 32, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf); 	 	/* �����ı� */
		
	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,"%s%s", "���ͱ��: ",cartype);	
	Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);    /* �����ı� */               
	
	Update_Component(4);                                                                              /* �������״̬ */
	Next_Frame(); 

}
/*
�����˵���ѡ�����ò˵�
*/
void  ChooseSetMeun(void)
{
	char pbuf[20];
	
	/*���³�ʼ������*/
	CarSetUpFlag=false;		//������ŵ�����	��up��־
	CarSetDownFlag=false;	//������ŵ�����	��Down��־
	CarNumMenuOkKeyCount=0;		//����������ý���OK�����ļ���
	CarSetOk1Flag=false;	
	CarSetOk2Flag=false;	
	CarSetOk3Flag=false;	
	
	/*��ԭʼ���ݸ��Ƶ��˵�������Ҫ�Ĳ���������*/  //�������if����
	strcpy(LocalCartype,cartype);
	LocalCarNumber = CarNumber;
	LocalCarNo = CarNo;
	
	Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
	
	sprintf(pbuf,"%s","ѡ������");
	Draw_Component(COMPONENT_LABEL_ENABLED,  1, 0,  0,  15,  0,   0, 0, 0, (unsigned char *)pbuf);     /* �����ı�,������� */

	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,"%s%04d","�������: ",CarNumber);	
	Draw_Component(COMPONENT_LABEL_ENABLED,  2, 16, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf); 	  /* �����ı� */
	
	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,"%s%01d","��    ��: ",CarNo);		
	Draw_Component(COMPONENT_LABEL_ENABLED,  3, 32, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf); 	 	/* �����ı� */
		
	memset(pbuf,0,sizeof(pbuf));
	sprintf(pbuf,"%s%s", "���ͱ��: ",cartype);	
	Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);    /* �����ı� */               
	
	Update_Component(4);                                                                              /* �������״̬ */
	
	Draw_Reverse(ChooseUpDownCount*16,0,(ChooseUpDownCount+1)*16,128,0);	//������ŷ�ɫ
	Next_Frame(); 
}

/*
�����Ӳ˵������ó�����ţ����ţ��������͡�
*/
void  SetCarMenu(void)
{
	char pbuf[20];
	s8 ThouBit;	//ǧλ
	s8 HundBit;	//��λ
	s8 TenBit;	//ʮλ��
	s8 UnitBit;	//��λ��

	switch(ChooseUpDownCount)		//�ж���ѡ�����
	{
		case 1:		//����������ý���		
			ThouBit = LocalCarNumber /1000 %10;
			HundBit	= LocalCarNumber /100 %10;
			TenBit = LocalCarNumber  /10 %10;
			UnitBit = LocalCarNumber %10;
			
			MenuTable[KeyFuncIndex].Enter = 2;	//�����汣���ڳ����������
			switch(CarNumMenuOkKeyCount)
			{
				case 0:		//ǧλ
					if(CarSetUpFlag == true)
					{
						CarSetUpFlag = false;
						ThouBit += 1;
						if(ThouBit>9)
						{
							ThouBit=0;
						}
					}
					if(CarSetDownFlag == true)
					{
						CarSetDownFlag = false;
						ThouBit -=1;
						if(ThouBit<0)
						{
							ThouBit=9;
						}
					}
					break;
				case 1:		//��λ
					if(CarSetUpFlag == true)
					{
						CarSetUpFlag = false;
						HundBit += 1;
						if(HundBit>9)
						{
							HundBit=0;
						}
					}
					if(CarSetDownFlag == true)
					{
						CarSetDownFlag = false;
						HundBit -=1;
						if(HundBit<0)
						{
							HundBit=9;
						}				
					}
					break;
				case 2:		//ʮλ
					if(CarSetUpFlag == true)
					{
						CarSetUpFlag = false;
						TenBit += 1;
						if(TenBit>9)
						{
							TenBit=0;
						}
					}
					if(CarSetDownFlag == true)
					{
						CarSetDownFlag = false;
						TenBit -=1;
						if(TenBit<0)
						{
							TenBit=9;
						}				
					}
					break;
				case 3:		//��λ,��ʾΪ����
					if(CarSetUpFlag == true)
					{
						CarSetUpFlag = false;
						UnitBit += 1;
						if(UnitBit>6)
						{
							UnitBit=1;
						}
					}
					if(CarSetDownFlag == true)
					{
						CarSetDownFlag = false;
						UnitBit -=1;
						if(UnitBit<1)
						{
							UnitBit=6;
						}				
					}				
				
					MenuTable[KeyFuncIndex].Enter = 0;	//���һ��Ok����Ҫ���ص�������					
					break;

			}
			
			LocalCarNumber = ThouBit*1000 + HundBit*100+ TenBit*10 +UnitBit;  //���㳵�����
			LocalCarNo = UnitBit;	//���Ÿ��ų����һ����
			strcpy(LocalCartype,CarTypeArr[UnitBit-1]);		//������� �� �������Ͱ�

			Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
			memset(pbuf,0,sizeof(pbuf));
			sprintf(pbuf,"%s","�������: ");	
			Draw_Component(COMPONENT_LABEL_ENABLED,  2, /*16-4*/12, 0,  15,  0,   0, 0, 0, (unsigned char *)pbuf); 	  /* �����ı� */
			
			memset(pbuf,0,sizeof(pbuf));
			sprintf(pbuf,"%04d",LocalCarNumber);		//������ֵ	
			Draw_Component(COMPONENT_LABEL_ENABLED,  3, /*32-4*/28, 0,  15,  0,   0, 0, 0, (unsigned char *)pbuf); 	 	/* �����ı� */
	
		
			memset(pbuf,0,sizeof(pbuf));
			sprintf(pbuf,"%s%s", "SETȡ��   ","OKȷ��");	
			Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);    /* �����ı� */  
		
			Recycle_Component(1,1);//�������1
			Update_Component(4);                                                                              /* �������״̬ */
			Draw_Reverse(48,0,48,128,0);		//��һ����	
			Draw_Reverse(/*32-3*/29,48+CarNumMenuOkKeyCount*8,/*48-5*/43,48+CarNumMenuOkKeyCount*8+6,0);			//ĳ������ȡ��	
			Next_Frame(); 	
			break;
		
		case 2:		//�������ý���	

			if(CarSetUpFlag == true)
			{
				CarSetUpFlag = false;
				LocalCarNo++;
				if(LocalCarNo>6)
				{
					LocalCarNo=1;
				}
			}
			if(CarSetDownFlag == true)
			{
				CarSetDownFlag = false;
				LocalCarNo--;
				if(LocalCarNo<1)
				{
					LocalCarNo=6;
				}
			}
			LocalCarNumber = (LocalCarNumber/10)*10+LocalCarNo;  //������ �복�Ű�
			strcpy(LocalCartype,CarTypeArr[LocalCarNo-1]);		//���� �� �������Ͱ�
			
			Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
			memset(pbuf,0,sizeof(pbuf));
			sprintf(pbuf,"%s%01d","����: ",LocalCarNo);	
			Draw_Component(COMPONENT_LABEL_ENABLED,  2, 16, 0,  15,  0,   0, 0, 0, (unsigned char *)pbuf); 	  /* �����ı� */
			
			memset(pbuf,0,sizeof(pbuf));
			sprintf(pbuf,"%s%s", "SETȡ��   ","OKȷ��");	
			Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);    /* �����ı� */  
		
			Recycle_Component(1,1);//�������1
			Recycle_Component(3,3);//�������1
			Update_Component(4);                                                                              /* �������״̬ */
			Draw_Reverse(48,0,48,128,0);		//��һ����	
			Draw_Reverse(/*16+2*/18,/*16*5+4*/84,/*32-2*/30,/*16*5+10*/90,0);	//����ȡ��ɫ
			Next_Frame(); 
			MenuTable[KeyFuncIndex].Enter = 0;	//���ص�������
			break;
		
		case 3:		//���ͱ�����ý���
			{
				//char CarTypeArr[6][10] = {"MC1(1)","MC2(2)","M(3)","T(4)","T1(5)","T2(6)"};		//WHH ���ֲ������޸�Ϊȫ�ֱ���
				s8 i=0;
		
				while(1)		//�������ǰ�������ڵ�λ��
				{
					if(i>5)
					{
						debugprintf("û��ƥ��ĳ���!\r\n");
						break;
					}
					if(strcmp(LocalCartype,CarTypeArr[i]) == 0)
					{
						break;
					}
					else
					{
						i++;
					}
				}
				
				if(CarSetUpFlag == true)		//�ϰ�������
				{
					CarSetUpFlag = false;
					i++;
					if(i>5)
					{
						i=0;
					}
				}
				if(CarSetDownFlag == true)	//�°�������
				{
					CarSetDownFlag = false;
					i--;
					if(i<0)
					{
						i=5;
					}
				}
				
				LocalCarNumber = (LocalCarNumber/10)*10+(i+1);  //���㳵�����
				LocalCarNo = i+1;	//���Ÿ��ų�������һ����				
				strcpy(LocalCartype,CarTypeArr[i]);
				//memcpy(cartype,CarTypeArr[i],10);  //������ copy��ȫ�ֱ���
				
				Clean_Screen(0, 0, 64, 128, 0);                                                                     /* ���� */
				memset(pbuf,0,sizeof(pbuf));
				sprintf(pbuf,"%s","���ͱ��: ");	
				Draw_Component(COMPONENT_LABEL_ENABLED,  2, /*16-4*/12, 0,  15,  0,   0, 0, 0, (unsigned char *)pbuf); 	  /* �����ı� */
			
				memset(pbuf,0,sizeof(pbuf));
				sprintf(pbuf,"%s",LocalCartype);		//���ͱ��ֵ
				Draw_Component(COMPONENT_LABEL_ENABLED,  3, /*32-4*/28, 0,  15,  0,   0, 0, 0, (unsigned char *)pbuf); 	 	/* �����ı� */
				
				memset(pbuf,0,sizeof(pbuf));
				sprintf(pbuf,"%s%s", "SETȡ��   ","OKȷ��");	
				Draw_Component(COMPONENT_LABEL_ENABLED,  4, 48, 0,  16,  0,   0, 0, 0, (unsigned char *)pbuf);    /* �����ı� */  
			
				Recycle_Component(1,1);//�������1
				Update_Component(4);                                                                              /* �������״̬ */
				Draw_Reverse(48,0,48,128,0);		//��һ����	
				Draw_Reverse(/*32-3*/29,16*3-10,/*48-5*/43,16*6-10,0);	//����ȡ��ɫ
				Next_Frame(); 
				MenuTable[KeyFuncIndex].Enter = 0;	//���ص�������
			}

			break;
	}
}


