#include "main.h"
#include "rtc.h"
#include "oled.h"
#include "bmp.h"
#include "delay.h"
//#include "usart.h"
uchar dis_time_buf[16]={0};
uchar xingqi = 0;
u8 i=0,butt;
volatile u8 x=15,y=32,passw[4],j=0,zd=0,zd1=0;
volatile u16 conut=0;

//初始时间定义
u8 time_buf[8] = {0x20,0x24,0x11,0x26,0x18,0x51,0x30,0x02};//初始时间2024年11月21号23点59分55秒 星期二
//u8 time_buf[8];//初始时间2010年6月1号23点59分55秒 星期二
//DS1302引脚定义
#define RTC_RST_Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define RTC_RST_Set() GPIO_SetBits(GPIOA, GPIO_Pin_6)
#define RTC_IO_Clr()  GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define RTC_IO_Set()  GPIO_SetBits(GPIOA, GPIO_Pin_5)
#define RTC_SCK_Clr() GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define RTC_SCK_Set() GPIO_SetBits(GPIOA, GPIO_Pin_4)

//DS1302地址定义
#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

   	//DS1302初始化函数
void ds1302_init(void) 
{
	 GPIO_InitTypeDef GPIO_InitStructure; 
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //Ê¹ÄÜGPIOA¶Ë¿ÚÊ±ÖÓ
	 
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 ; 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	 RTC_RST_Clr();
	 RTC_SCK_Clr();

}

void buzz_init(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure; 
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //Ê¹ÄÜGPIOA¶Ë¿ÚÊ±ÖÓ
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure); 
	 GPIO_ResetBits(GPIOA, GPIO_Pin_7);
}

void ir_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
   
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

}

void mq2_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
   
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
	


}
void butt_init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure; 
    
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE);  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
	

}

u8 getbuttdata(void)
{
	
	while(1)
	{
		
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
		{
			Delay_Ms(20);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
			{
			while(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)));
			return 1;
			}
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
		{
			Delay_Ms(20);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
			{
      while(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)));
			return 2;
			}
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
		{
			Delay_Ms(20);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
			{
			while(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)));
			return 3;
			}
		}
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
		{
			Delay_Ms(20);
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
			{
			while(!(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)));
			return 4;
			}
		}
	
	}
	

}

void RTC_IO_IN()
{
		GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //Ê¹ÄÜGPIOA¶Ë¿ÚÊ±ÖÓ
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

void RTC_IO_OUT()
{
		GPIO_InitTypeDef GPIO_InitStructure; 
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);                                //Ê¹ÄÜGPIOA¶Ë¿ÚÊ±ÖÓ
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}

//向DS1302写入一字节数据
void ds1302_write_byte(u8 addr, u8 d) 
{
	u8 i;
	RTC_RST_Set();				//启动DS1302总线	
	//写入目标地址：addr
	addr = addr & 0xFE;   //最低位置零，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			RTC_IO_Set();
			}
		else {
			RTC_IO_Clr();
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		addr = addr >> 1;
		}	
	//写入数据：d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			RTC_IO_Set();
			}
		else {
			RTC_IO_Clr();
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		d = d >> 1;
		}
	RTC_RST_Clr();		//停止DS1302总线
}

//从DS1302读出一字节数据
u8 ds1302_read_byte(u8 addr) {

	u8 i,temp;	
	RTC_RST_Set();						//启动DS1302总线
	//写入目标地址：addr
	addr = addr | 0x01;    //最低位置高，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			RTC_IO_Set();	
			}
		else {
			RTC_IO_Clr();	
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		addr = addr >> 1;
		}
	RTC_IO_IN();	
	//输出数据：temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5)) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}
		RTC_SCK_Set();      //产生时钟
		RTC_SCK_Clr();
		}	
	RTC_RST_Clr();					//停止DS1302总线
	RTC_IO_OUT();
	return temp;
}

//向DS302写入时钟数据
void ds1302_write_time(void) 
{
	ds1302_write_byte(ds1302_control_add,0x00);			//关闭写保护 
	ds1302_write_byte(ds1302_sec_add,0x80);				//暂停时钟 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //涓流充电 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		//年 
	ds1302_write_byte(ds1302_month_add,time_buf[2]);	//月 
	ds1302_write_byte(ds1302_date_add,time_buf[3]);		//日 
	ds1302_write_byte(ds1302_hr_add,time_buf[4]);		//时 
	ds1302_write_byte(ds1302_min_add,time_buf[5]);		//分
	ds1302_write_byte(ds1302_sec_add,time_buf[6]);		//秒
	ds1302_write_byte(ds1302_day_add,time_buf[7]);		//周 
	ds1302_write_byte(ds1302_control_add,0x80);			//打开写保护     
}
//从DS302读出时钟数据
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//年 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//月 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//日 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//时 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//分 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//秒，屏蔽秒的第7位，避免超出59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//周 	
}

u8 b_h(u8 a)
{
	u8 b;
  	   switch(a)
	   {
	   		case 0x00 :  b=0;break;
			case 0x01 :  b=1;break;
			case 0x02 :  b=2;break;
			case 0x03 :  b=3;break;
			case 0x04 :  b=4;break;
			case 0x05 :  b=5;break;
			case 0x06 :  b=6;break;
			case 0x07 :  b=7;break;
			case 0x08 :  b=8;break;
			case 0x09 :  b=9;break;
			case 0x10 :  b=10;break;
			case 0x11 :  b=11;break;
			case 0x12 :  b=12;break;
			case 0x13 :  b=13;break;
			case 0x14 :  b=14;break;
			case 0x15 :  b=15;break;
			case 0x16 :  b=16;break;
			case 0x17 :  b=17;break;
			case 0x18 :  b=18;break;
			case 0x19 :  b=19;break;
			case 0x20 :  b=20;break;
			case 0x21 :  b=21;break;
			case 0x22 :  b=22;break;
			case 0x23 :  b=23;break;
			case 0x24 :  b=24;break;
			case 0x25 :  b=25;break;
			case 0x26 :  b=26;break;
			case 0x27 :  b=27;break;
			case 0x28 :  b=28;break;
			case 0x29 :  b=29;break;
			case 0x30 :  b=30;break;
			case 0x31 :  b=31;break;
			case 0x32 :  b=32;break;
			case 0x33 :  b=33;break;
			case 0x34 :  b=34;break;
			case 0x35 :  b=35;break;
			case 0x36 :  b=36;break;
			case 0x37 :  b=37;break;
			case 0x38 :  b=38;break;
			case 0x39 :  b=39;break;
			case 0x40 :  b=40;break;
			case 0x41 :  b=41;break;
			case 0x42 :  b=42;break;
			case 0x43 :  b=43;break;
			case 0x44 :  b=44;break;
			case 0x45 :  b=45;break;
			case 0x46 :  b=46;break;
			case 0x47 :  b=47;break;
			case 0x48 :  b=48;break;
			case 0x49 :  b=49;break;
			case 0x50 :  b=50;break;
			case 0x51 :  b=51;break;
			case 0x52 :  b=52;break;
			case 0x53 :  b=53;break;
			case 0x54 :  b=54;break;
			case 0x55 :  b=55;break;
			case 0x56 :  b=56;break;
			case 0x57 :  b=57;break;
			case 0x58 :  b=58;break;
			case 0x59 :  b=59;break;
			default   : break;
	   }
	   return b;
}
void showbegin(void)//开机显示
{
//  OLED_DrawBMP(0,0,84,8,BMP19);
	OLED_ShowCHinese(16,2,0);
	OLED_ShowCHinese(32,2,1);
	OLED_ShowCHinese(48,2,2);
	OLED_ShowCHinese(64,2,3);
	OLED_ShowCHinese(80,2,4);
	OLED_ShowCHinese(96,2,5);
  Delay_Ms(2000);
  
	OLED_Clear();
	OLED_ShowCHinese(15,6,16);
	OLED_ShowCHinese(40,6,16);
	OLED_ShowCHinese(65,6,16);
	OLED_ShowCHinese(90,6,16);
	
	
	while(1)
	{
				butt=getbuttdata();//获取按键
	
		//显示密码1.5秒后变为*
		passw[j]=butt;
		switch(butt)
		{
			case 0 : OLED_DrawBMP(x,2,y,6,BMP10);zd=1;conut=0;break;
			case 1 : OLED_DrawBMP(x,2,y,6,BMP1);zd=1;conut=0;break;
			case 2 : OLED_DrawBMP(x,2,y,6,BMP2);zd=1;conut=0;break;
			case 3 : OLED_DrawBMP(x,2,y,6,BMP3);zd=1;conut=0;break;
			case 4 : OLED_DrawBMP(x,2,y,6,BMP4);zd=1;conut=0;break;
			case 5 : OLED_DrawBMP(x,2,y,6,BMP5);zd=1;conut=0;break;
			case 6 : OLED_DrawBMP(x,2,y,6,BMP6);zd=1;conut=0;break;
			case 7 : OLED_DrawBMP(x,2,y,6,BMP7);zd=1;conut=0;break;
			case 8 : OLED_DrawBMP(x,2,y,6,BMP8);zd=1;conut=0;break;
			case 9 : OLED_DrawBMP(x,2,y,6,BMP9);zd=1;conut=0;break;
		}
		if(x>25)
			OLED_DrawBMP(x-25,2,y-25,6,BMP11);
		x+=25;
		y+=25;
		j++;
		if(j==4)
		{
		   if(passw[0]==1&&passw[1]==2&&passw[2]==3&&passw[3]==4)
			 {
				 OLED_Clear();
				 break;
			 }
			 else
			 {
				    Delay_Ms(500);
						OLED_Clear();
						OLED_DrawBMP(10,2,42,6,BMP14);
						OLED_DrawBMP(37,2,69,6,BMP15);
						OLED_DrawBMP(67,2,99,6,BMP16);
						OLED_DrawBMP(92,2,124,6,BMP17);
						Delay_Ms(2000);
						OLED_Clear();
				    OLED_ShowCHinese(15,6,16);
						OLED_ShowCHinese(40,6,16);
						OLED_ShowCHinese(65,6,16);
						OLED_ShowCHinese(90,6,16);
						x=15;
						y=32;
				    j=0;
			 }
		}
	
	}
	
  OLED_DrawBMP(0,0,128,8,BMP20);
	

  OLED_Clear();
	OLED_ShowChar(45,0,'W');
	OLED_ShowChar(52,0,'I');
	OLED_ShowChar(59,0,'F');
	OLED_ShowChar(66,0,'I');
	OLED_ShowCHinese(30,2,6);//
	OLED_ShowCHinese(47,2,7);//
	OLED_ShowCHinese(64,2,8);//
  Delay_Ms(2000);
  OLED_Clear();


}

void showhuo(void)
{

	OLED_DrawBMP(0,1,64,8,BMP26);
}

void showmei(void)
{
	OLED_DrawBMP(0,1,64,8,BMP24);
}

void showman(void)
{
	OLED_DrawBMP(8,1,56,8,BMP25);
}

void showBMP11(void)
{
	OLED_DrawBMP(x-25,2,y-25,6,BMP11);
}


void showtime(void)
{

	ds1302_read_time();  //读取时间 

	switch((b_h(time_buf[2])/10))
	{
		case 0 :  	OLED_ShowChar(0,0,'0'); break;
		case 1 :  	OLED_ShowChar(0,0,'1'); break;
	}
	switch((b_h(time_buf[2])%10))
	{
		case 0 :  	OLED_ShowChar(8,0,'0'); break;
		case 1 :  	OLED_ShowChar(8,0,'1'); break;
		case 2 :  	OLED_ShowChar(8,0,'2'); break;
		case 3 :  	OLED_ShowChar(8,0,'3'); break;
		case 4 :  	OLED_ShowChar(8,0,'4'); break;
		case 5 :  	OLED_ShowChar(8,0,'5'); break;
		case 6 :  	OLED_ShowChar(8,0,'6'); break;
		case 7 :  	OLED_ShowChar(8,0,'7'); break;
		case 8 :  	OLED_ShowChar(8,0,'8'); break;
		case 9 :  	OLED_ShowChar(8,0,'9'); break;
	}
	OLED_ShowChar(16,0,'-');
	
	switch((b_h(time_buf[3])/10))
	{
		case 0 :  	OLED_ShowChar(24,0,'0'); break;
		case 1 :  	OLED_ShowChar(24,0,'1'); break;
		case 2 :  	OLED_ShowChar(24,0,'2'); break;
		case 3 :  	OLED_ShowChar(24,0,'3'); break;
	}
	switch((b_h(time_buf[3])%10))
	{
		case 0 :  	OLED_ShowChar(32,0,'0'); break;
		case 1 :  	OLED_ShowChar(32,0,'1'); break;
		case 2 :  	OLED_ShowChar(32,0,'2'); break;
		case 3 :  	OLED_ShowChar(32,0,'3'); break;
		case 4 :  	OLED_ShowChar(32,0,'4'); break;
		case 5 :  	OLED_ShowChar(32,0,'5'); break;
		case 6 :  	OLED_ShowChar(32,0,'6'); break;
		case 7 :  	OLED_ShowChar(32,0,'7'); break;
		case 8 :  	OLED_ShowChar(32,0,'8'); break;
		case 9 :  	OLED_ShowChar(32,0,'9'); break;
	}
	
////	OLED_ShowChar(40,0,'-');
	
	switch((b_h(time_buf[4])/10))
	{
		case 0 :  	OLED_ShowChar(64,0,'0'); break;
		case 1 :  	OLED_ShowChar(64,0,'1'); break;
		case 2 :  	OLED_ShowChar(64,0,'2'); break;
	}
	switch((b_h(time_buf[4])%10))
	{
		case 0 :  	OLED_ShowChar(72,0,'0'); break;
		case 1 :  	OLED_ShowChar(72,0,'1'); break;
		case 2 :  	OLED_ShowChar(72,0,'2'); break;
		case 3 :  	OLED_ShowChar(72,0,'3'); break;
		case 4 :  	OLED_ShowChar(72,0,'4'); break;
		case 5 :  	OLED_ShowChar(72,0,'5'); break;
		case 6 :  	OLED_ShowChar(72,0,'6'); break;
		case 7 :  	OLED_ShowChar(72,0,'7'); break;
		case 8 :  	OLED_ShowChar(72,0,'8'); break;
		case 9 :  	OLED_ShowChar(72,0,'9'); break;
	}
	
		OLED_ShowChar(80,0,'-');
	
	switch((b_h(time_buf[5])/10))
	{
		case 0 :  	OLED_ShowChar(88,0,'0'); break;
		case 1 :  	OLED_ShowChar(88,0,'1'); break;
		case 2 :  	OLED_ShowChar(88,0,'2'); break;
		case 3 :  	OLED_ShowChar(88,0,'3'); break;
		case 4 :  	OLED_ShowChar(88,0,'4'); break;
		case 5 :  	OLED_ShowChar(88,0,'5'); break;
		case 6 :  	OLED_ShowChar(88,0,'6'); break;
		case 7 :  	OLED_ShowChar(88,0,'7'); break;
		case 8 :  	OLED_ShowChar(88,0,'8'); break;
		case 9 :  	OLED_ShowChar(88,0,'9'); break;
	}
	switch((b_h(time_buf[5])%10))
	{
		case 0 :  	OLED_ShowChar(96,0,'0'); break;
		case 1 :  	OLED_ShowChar(96,0,'1'); break;
		case 2 :  	OLED_ShowChar(96,0,'2'); break;
		case 3 :  	OLED_ShowChar(96,0,'3'); break;
		case 4 :  	OLED_ShowChar(96,0,'4'); break;
		case 5 :  	OLED_ShowChar(96,0,'5'); break;
		case 6 :  	OLED_ShowChar(96,0,'6'); break;
		case 7 :  	OLED_ShowChar(96,0,'7'); break;
		case 8 :  	OLED_ShowChar(96,0,'8'); break;
		case 9 :  	OLED_ShowChar(96,0,'9'); break;
	}
	OLED_ShowChar(104,0,'-');
	
	switch((b_h(time_buf[6])/10))
	{
		case 0 :  	OLED_ShowChar(112,0,'0'); break;
		case 1 :  	OLED_ShowChar(112,0,'1'); break;
		case 2 :  	OLED_ShowChar(112,0,'2'); break;
		case 3 :  	OLED_ShowChar(112,0,'3'); break;
		case 4 :  	OLED_ShowChar(112,0,'4'); break;
		case 5 :  	OLED_ShowChar(112,0,'5'); break;
		case 6 :  	OLED_ShowChar(112,0,'6'); break;
		case 7 :  	OLED_ShowChar(112,0,'7'); break;
		case 8 :  	OLED_ShowChar(112,0,'8'); break;
		case 9 :  	OLED_ShowChar(112,0,'9'); break;
	}
	switch((b_h(time_buf[6])%10))
	{
		case 0 :  	OLED_ShowChar(120,0,'0'); break;
		case 1 :  	OLED_ShowChar(120,0,'1'); break;
		case 2 :  	OLED_ShowChar(120,0,'2'); break;
		case 3 :  	OLED_ShowChar(120,0,'3'); break;
		case 4 :  	OLED_ShowChar(120,0,'4'); break;
		case 5 :  	OLED_ShowChar(120,0,'5'); break;
		case 6 :  	OLED_ShowChar(120,0,'6'); break;
		case 7 :  	OLED_ShowChar(120,0,'7'); break;
		case 8 :  	OLED_ShowChar(120,0,'8'); break;
		case 9 :  	OLED_ShowChar(120,0,'9'); break;
	}
	


}
