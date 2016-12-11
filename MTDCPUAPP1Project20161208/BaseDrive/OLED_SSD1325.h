#ifndef  __OLED_SSD1325_H__
#define  __OLED_SSD1325_H__

#include "stm32f4xx.h"


/*whh 600 �п�ʼ*/
extern const unsigned char szASC16[];
extern const unsigned char F6x8[1][1];
extern const unsigned char  F4x6[1][1];

/*��������ȷ���ֿ⣬�����ڴ治���ˣ�Ҳû��ʹ��������壬�����ε�*/
//extern const unsigned char F6x8[][6];
//extern const unsigned char  F4x6[95][3];

#define USE_SMALL_FONT			/* ������б�ʾʹ��С�ֿ� */


/* ������� */
typedef enum
{
	FC_ST_16 = 0,		/* ����15x16���� ����x�ߣ� */
	FC_ST_12 = 1,		/* ����12x12���� ����x�ߣ� */
	FC_ST_8	 = 2,		/* ���� 8*8����   (��x��) */	
}FONT_CODE_E;

/* �������Խṹ, ����LCD_DispStr() */
typedef struct
{
	uint16_t FontCode;		/* ������� 0 ��ʾ16���� */
	uint16_t FrontColor;	/* ������ɫ */
	uint16_t BackColor;		/* ���ֱ�����ɫ��͸�� */
	uint16_t Space;				/* ���ּ�࣬��λ = ���� */
}FONT_T;

/* �ɹ��ⲿģ����õĺ��� */


#define DISPLAY_WIDTH   128
#define DISPLAY_HEIGHT  64


/*W H H*/
#define 		XLevelL										0x02
#define 		XLevelH										0x10
#define 		XLevel										((XLevelH&0x0F)*16+XLevelL)

#define 		Max_Column								0x3F			// 128/2-1 (Total Columns Devided by 2)
#define 		Max_Row										0x3F			// 64-1
#define			Brightness								0xFF


	
#define			COMPONENT_NULL									0x00			//���������������
#define			COMPONENT_LABEL_ENABLED					0x01		/*Label������Ļ�ϻ���һ����ǩ
																											��ʱ index => ������
																											 x0    => ���Ͻ�x����
																											 y0    => ���Ͻ�y����
																											 x1    => 4 (����4*6����) / 6 (����6*10����)
																											 z     => ���Ʋ㣬Ӧ��0
																											 chr[] => Label����*/
#define			COMPONENT_LABEL_DISABLED				0x02		/*���õ�Label����COMPONENT_LABEL_ENABLED����һ�£���ɫ����*/
#define			COMPONENT_LABEL_HIDDEN					0x03		/*��Ч*/
#define			COMPONENT_BUTTON_ENABLED 				0x04		/* Button������Ļ�ϻ���һ����ť
																											��ʱ index => �����ţ���button�ɱ�ѡ�У��μ�Update_Component����
																											 x0    => ���Ͻ�x����
																											 y0    => ���Ͻ�y����
																											 x1    => ���½�x����
																											 y1    => ���½�y����
																											 z     => ���Ʋ㣬Ӧ��0
																											 chr[] => Button���֣����Ĭ�Ͼ���*/
#define			COMPONENT_BUTTON_DISABLED 		 	0x05		/* ���õ�Button����ʱButton���ܱ�Update_Component����ѡ��*/
#define			COMPONENT_BUTTON_HIDDEN  				0x06		/*��Ч*/
#define			COMPONENT_RADIO_TRUE  					0x07		/*adio������Ļ�ϻ���һ���Ѿ���ѡ��ѡ��
																											��ʱRadio����Ϊ6*10��4*6������μ�COMPONENT_SRADIO
																											��ʱ index => �����ţ���Radio�ɱ�ѡ�У��μ�Update_Component����
																											 x0    => ���Ͻ�x����
																											 y0    => ���Ͻ�y����
																											 z     => ���Ʋ㣬Ӧ��0
																											 chr[] => Radio����*/
#define			COMPONENT_RADIO_FALSE  					0x08		/*Radio������Ļ�ϻ���һ���Ѿ�δ��ѡ��ѡ��*/
#define			COMPONENT_RADIO_DISABLED  			0x09		/*���õ�Radio����ʱRadio���ܱ�Update_Component����ѡ��*/
#define			COMPONENT_RADIO_HIDDEN  				0x0A		/*��Ч*/
#define			COMPONENT_TEXT_ENABLED					0x0B		/*Text������Ļ�ϻ����ı���
																											�ı������ǩ��ͬ���ı������������Ӧ�������ָ���
																											��ʱ index => �����ţ���Text�ɱ�ѡ�У��μ�Update_Component����
																										 x0    => ���Ͻ�x����
																										 y0    => ���Ͻ�y����
																										 x1    => ���½�x����
																										 y1    => ���½�y����
																										 z     => ���Ʋ㣬Ӧ��0
																										 c1    => 4 (����4*6����) / 6 (����6*10����)
																										 chr[] => Text����*/
#define			COMPONENT_TEXT_DISABLED					0x0C		/* ���õ�Text����ʱText���ܱ�Update_Component����ѡ��*/																										 
#define			COMPONENT_TEXT_HIDDEN						0x0D		/*��Ч*/
#define			COMPONENT_PBAR_ENABLED					0x0E		/*PBar������Ļ�ϻ��ƽ�����
																											��������������ɫ�ᱻ�Զ�����Ϊ��ɫ
																											��ʱ index => ������
																											 x0    => ���Ͻ�x����
																											 y0    => ���Ͻ�y����
																											 x1    => ���½�x����
																											 y1    => ���½�y����
																											 z     => ���Ʋ㣬Ӧ��0
																											 c1    => ��������߾࣬��ʾ�ٷֱ�ʱ����ֵӦΪ0
																											 c2    => ���������ȣ�ӦС�� y1-y0
																											 chr[] => PBar����*/
#define			COMPONENT_PBAR_DISABLED					0x0F		/*���õ�PBar*/
#define			COMPONENT_WINDOW_ENABLED				0x10		/*��Ч*/
#define			COMPONENT_WINDOW_HIDDEN					0x11		/*��Ч*/
#define			COMPONENT_CHART_ENABLED					0x12		/*��Ч*/
#define			COMPONENT_CHART_HIDDEN					0x13		/*��Ч*/
#define			COMPONENT_LISTITEM_ENABLED		  0x14		/*ListItem������Ļ�ϻ��ƶ����˵��б�
																											��������������ɫ�ᱻ�Զ�����Ϊ��ɫ
																											��ʱ index => �����ţ���ListItem�ɱ�ѡ�У��μ�Update_Component����
																											 x0    => ���Ͻ�x����
																											 z     => ���Ʋ㣬Ӧ��0
																											 chr[] => ListItem����*/
#define			COMPONENT_LISTITEM_DISABLED			0x15		/*���õ�ListItem����ʱListItem���ܱ�Update_Component����ѡ��*/
#define			COMPONENT_LISTITEM_HIDDEN				0x16		/*��Ч*/
#define			COMPONENT_SRADIO_TRUE  					0x17		/*sRadio������Ļ�ϻ���һ���Ѿ���ѡ��ѡ��
																											��ʱsRadio����Ϊ4*6��6*10������μ�COMPONENT_RADIO
																									��ʱ index => �����ţ���sRadio�ɱ�ѡ�У��μ�Update_Component����
																											 x0    => ���Ͻ�x����
																											 y0    => ���Ͻ�y����
																											 z     => ���Ʋ㣬Ӧ��0
																											 chr[] => sRadio����*/
#define			COMPONENT_SRADIO_FALSE          0x18		/* sRadio������Ļ�ϻ���һ���Ѿ�δ��ѡ��ѡ��*/
#define			COMPONENT_SRADIO_DISABLED  		  0x19		/*���õ�sRadio����ʱsRadio���ܱ�Update_Component����ѡ��*/
#define			COMPONENT_SRADIO_HIDDEN  				0x1A		/* ��Ч*/
#define			COMPONENT_BLANK                 0x1B		/* ��Ч*/

#define			COMPONENT_MAX_INDEX 						0x15


void Set_Start_Column(unsigned char d);
void Set_Addressing_Mode(unsigned char d);
void Set_Column_Address(unsigned char a, unsigned char b);
void Set_Page_Address(unsigned char a, unsigned char b);
void Set_Start_Line(unsigned char d);
void Set_Contrast_Control(unsigned char d);
void Set_Area_Brightness(unsigned char d);
void Set_Segment_Remap(unsigned char d);
void Set_Entire_Display(unsigned char d);
void Set_Inverse_Display(unsigned char d);
void Set_Multiplex_Ratio(unsigned char d);
void Set_Dim_Mode(unsigned char a, unsigned char b);
void Set_Master_Config(unsigned char d);
void Set_Display_On_Off(unsigned char d);
void Set_Start_Page(unsigned char d);
void Set_Common_Remap(unsigned char d);
void Set_Display_Offset(unsigned char d);
void Set_Display_Clock(unsigned char d);
void Set_Area_Color(unsigned char d);
void Set_Precharge_Period(unsigned char d);
void Set_Common_Config(unsigned char d);
void Set_VCOMH(unsigned char d);
void Set_Read_Modify_Write(unsigned char d);
void Set_NOP(void);

void Delay_ns(__IO uint32_t del);
void ClearDisplay(int xStart, int yStart, int xEnd, int yEnd);
void Checkerboard_12864(void);void Fill_RAM_12864(unsigned char a);void Set_Start_Line_12864(unsigned char d);
void Set_Display_On_Off_12864(unsigned char d);
void LCD_ClearBar(uint8_t x1_Pos, uint8_t x2_Pos);
void OLED_12864_Init(void);
/*������*/
void Draw_Rectangle_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
//void LCD_TestHorizontal(uint8_t shift);
//void LCD_TestVertical(uint8_t shift);
void Show_Frame_12864(void);			//W H H
void Set_Window(int xStart, int yStart, int xEnd, int yEnd);
void ClearDisplay(int xStart, int yStart, int xEnd, int yEnd);
/*�����е�ַ*/
void Set_Column_Address_12864(unsigned char a, unsigned char b);
/*�����е�ַ*/
void Set_Row_Address_12864(unsigned char a, unsigned char b);
/*���õ�ǰ�Աȶ�*/
void Set_Contrast_Current_12864(unsigned char d);
/*���õ�ǰ��Χ*/
void Set_Current_Range_12864(unsigned char d);
/*����ӳ���ʽ*/
void Set_Remap_Format_12864(unsigned char d);
/*������ʼ��*/
void Set_Start_Line(unsigned char d);
/*������ʾƫ��λ��*/
void Set_Display_Offset_12864(unsigned char d);
/*������ʾģʽ*/
void Set_Display_Mode_12864(unsigned char d);
/*���ö�·������*/
void Set_Multiplex_Ratio_12864(unsigned char d);
/*����������*/
void Set_Master_Config_12864(unsigned char d);
/*������ʾ����*/
void Set_Display_On_Off_12864(unsigned char d);
/*������λ����*/
void Set_Phase_Length_12864(unsigned char d);
/*����֡Ƶ��*/
void Set_Frame_Frequency_12864(unsigned char d);
/*������ʾ��ʱ��*/
void Set_Display_Clock_12864(unsigned char d);
/*����Ԥ�ȳ�粹��*/
void Set_Precharge_Compensation_12864(unsigned char a, unsigned char b);
/*���ó���ѹ*/
void Set_Precharge_Voltage_12864(unsigned char d);
/*�������com��ѹ*/
void Set_VCOMH_12864(unsigned char d);
/*���öεĵ͵�ѹ*/
void Set_VSL_12864(unsigned char d);
/*������*/
void Set_NOP_12864(void);
/*ͼ�μ����������*/
void GA_Option_12864(unsigned char d);
/*������*/
void Draw_Rectangle_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);
/*��һ�����򣬸��Ƶ���һ������*/
void Copy_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Full Screen)	�����Ļ
//
//    a: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_RAM_12864(unsigned char a);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Regular Pattern (Partial or Full Screen)	��䱳��
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Devided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//    e: Two Pixels Data
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fill_Block_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Checkboard (Full Screen)	���12864
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Checkerboard_12864(void);
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Gray Scale Bar (Full Screen)	��ʾ�Ҷ���
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Grayscale_12864(void);
void Show_Font57_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
void Show_String_12864(unsigned char a, unsigned char *Data_Pointer, unsigned char b, unsigned char c);
void OLED_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);			//W H H
/*5*7 ������*/
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Frame (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Frame_12864(void);			//W H H

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Show Pattern (Partial or Full Screen)
//
//    a: Column Address of Start
//    b: Column Address of End (Total Columns Divided by 2)
//    c: Row Address of Start
//    d: Row Address of End
//		k: 0,������ʾ��1���෴����ʾ
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Show_Pattern_12864(unsigned char *Data_Pointer, unsigned char a, unsigned char b, unsigned char c, unsigned char d,unsigned char k);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Vertical Scrolling (Full Screen)
//
//    a: Scrolling Direction
//       "0x00" (Upward)
//       "0x01" (Downward)
//    b: Set Numbers of Row Scroll per Step
//    c: Set Time Interval between Each Scroll Step
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Vertical_Scroll_12864(unsigned char a, unsigned char b, unsigned char c);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Continuous Horizontal Scrolling (Partial or Full Screen)
//
//    a: Set Numbers of Column Scroll per Step
//    b: Set Numbers of Row to Be Scrolled
//    c: Set Time Interval between Each Scroll Step in Terms of Frame Frequency
//    d: Delay Time
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Horizontal_Scroll_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

void Fade_Scroll_12864(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Deactivate Scrolling (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Deactivate_Scroll_12864(void);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade In (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_In_12864(void);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Fade Out (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Fade_Out_12864(void);


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x01" Enter Sleep Mode
//    "0x00" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep_12864(unsigned char a);

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Gray Scale Table Setting (Full Screen)
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Set_Gray_Scale_Table_12864(void);

void con_4_byte(unsigned char DATA);

void con_8_byte(unsigned char DATA);

void Show_dot(unsigned char x, unsigned char y, unsigned char color);

/****************************************************
   д��һ��16*16����
******************************************************/
void OLED_Draw_16x16Char(unsigned char x,unsigned char y,unsigned char coder);	//write chinese word of1616
void OLED_Draw_16x16String(unsigned char x,unsigned char y,unsigned char m,unsigned char endm);//write chinese word of1616;


void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 mode);
void Show_Font57(unsigned char b, unsigned char c, unsigned char d,unsigned char k);
void Show_Font68(unsigned char x, unsigned char y, unsigned char c,unsigned char k);
void Next_Frame(void);
void Show_XGS(void);
void Draw_Dot(u8 x, u8 y, u8 z, u8 color);
void Clean_Screen(u8 x0, u8 y0, u8 x1, u8 y1, u8 z);
void Draw_Char(u8 x,u8 y,u8 z,u8 chr,u8 font_color,u8 back_color);
void Draw_String(u8 x,u8 y,u8 z,u8 chr[],u8 font_color,u8 back_color);
void Draw_5x7Char(u8 x,u8 y,u8 z,u8 chr,u8 font_color,u8 back_color);
void Draw_5x7String(u8 x,u8 y,u8 z,u8 chr[],u8 font_color,u8 back_color);
void Draw_4x6Char(u8 x,u8 y,u8 z,u8 chr,u8 font_color,u8 back_color);
void Draw_4x6String(u8 x,u8 y,u8 z,u8 chr[],u8 font_color,u8 back_color);
void Draw_Icon(u8 x,u8 y,u8 z,u8 chr,u8 font_color,u8 back_color);
void Draw_Notification(u8 title[], u8 chr[]);
void Draw_Reverse(u8 x0, u8 y0, u8 x1, u8 y1, u8 z);
u8   Draw_Menu(u8 menu[][12], u8 x0, u8 y0, u8 x1, u8 y1, u8 default_select);
void Draw_Component(u8 type, u8 index, u8 x0, u8 y0, u8 x1, u8 y1, u8 z, u8 c0, u8 c1, u8 chr[]);
void Draw_Logo(u8 z, u8 chr[]);
void Update_Component(u8 select_index);
void Change_Index(s8 dx);
void Click_Component(void);
void Recycle_Component(u8 indexA, u8 indexB);
u8 Return_Index_Available(u8 type);


/*W H H*/

#endif 	 //__OLED_SSD1325_H__
