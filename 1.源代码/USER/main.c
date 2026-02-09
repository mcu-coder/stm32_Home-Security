#include "stm32f10x.h"  //包含需要的头文件
#include "main.h"       //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "usart1.h"     //包含需要的头文件
#include "timer2.h"     //包含需要的头文件

#include "dhtll.h"
#include "oled.h"
#include "rtc.h"
#include "adc.h"

#include "key.h"      //包含需要的头文件
u8 rexflag = 0;
u8 rexnum = 0;
u8 xinxi = 0;
u8 sendflag = 0;
u8 YW=0;
u8 MC=0;
u8 WD=0;
u8 SD=0;
u8 HY=0;
u8 tempbai,tempshi,tempge;  	 
u8 humibai,humishi,humige;
u8 temp;
u8 humi;
int YWADC;
u16 YWADCbai,YWADCshi,YWADCge;
uint8_t jingbao = 0;
u8 keyflag = 0;
u8 settemp = 40;  	    
u8 sethumi = 85;
u16 setyw = 40;
u8 tempKG=11,humiKG=11,ywKG=11,hyKG=11,mcKG=11;
void wifiset(void);
void setzhi(void)
{
	u8 t=0;	
  t=KEY_Scan(0);		//得到键值
	 
}

void wifiinit(void)
{
  u2_printf("AT\r\n");
	Delay_Ms(500);

 

	u2_printf("AT+CWSAP=\"ESP826601\",\"1234567890\",11,4\r\n");
	Delay_Ms(500);

	u2_printf("AT+CIPMUX=1\r\n");
	Delay_Ms(500);

	u2_printf("AT+CIPSERVER=1,5000\r\n");
	Delay_Ms(500);

	u2_printf("AT+CIPSTO=0\r\n");
}	


int main(void) 
{	
    SystemInit();  //将主频调整到72M

	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	  Delay_Init();                   //延时功能初始化 
    Delay_Ms(500);	
    OLED_Init();//oled初始化  
		OLED_Clear(); 
    while(DHT11_Init())	{Delay_Ms(500);}//检测DS18B20是否纯在
   
		butt_init();
    Adc_Init(); 
    KEY_Init(); 

	  Usart2_Init(115200);              //串口1功能初始化，波特率9600
		TIM2_Init();
    Isr_Init();
	  wifiinit();
		showbegin();
    ds1302_write_time();
		ds1302_read_time();
	while(1)                       //主循环
	{	
    
		setzhi();  //设置报警值
    if(keyflag==0)
    {
				DHT11_Read_Data(&temp,&humi);
				YWADC = (Get_Adc_Average(9,10));     //获取计算后的带小数的实际电压值
				YWADC = (YWADC -600) ;
				if(YWADC<6) YWADC = 0;
			 
				if(((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)&&(hyKG==11)) || ((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) != 0)&&(mcKG==11)) 
          ||	((YWADC>setyw)&&(ywKG==11))		)
				  
					{
						jingbao =1;
						GPIO_SetBits(GPIOA, GPIO_Pin_7);
					 
						if((YWADC>setyw)&&(ywKG==11))//烟雾报警
						{
							YW=11;
							OLED_Clear();
							OLED_ShowCHinese(80,4,14);
							OLED_ShowCHinese(96,4,15);
							OLED_ShowCHinese(80,2,17);
							OLED_ShowCHinese(96,2,18);//
							showmei();
            }
           
				  }
					else
					{
						MC=22;YW=22;HY=22;
						if(jingbao == 1)
						{
							 OLED_Clear();
							 jingbao = 0;
						}
					  OLED_ShowCHinese(0,2,9);//温度
						OLED_ShowCHinese(16,2,10);
						OLED_ShowChar(32,2,':');
						OLED_ShowNum(40,2,temp,2,16);
						OLED_ShowChar(56,2,'C');

						OLED_ShowCHinese(0,4,11);//湿度
						OLED_ShowCHinese(16,4,10);
						OLED_ShowChar(32,4,':');
						OLED_ShowNum(40,4,humi,2,16);
						OLED_ShowChar(56,4,'%');
					 
					}
			    wifiset();
		}	
    		
   
		else if(keyflag==1)
		{
		   OLED_ShowCHinese(16,0,9);//温度
			 OLED_ShowCHinese(32,0,10);
			 OLED_ShowCHinese(48,0,28);//报警
			 OLED_ShowCHinese(64,0,14);
			 OLED_ShowCHinese(80,0,29);//开关
			 OLED_ShowCHinese(96,0,30);
			 if(tempKG==11) {OLED_ShowCHinese(48,4,31); OLED_ShowCHinese(64,4,29);}
       if(tempKG==22) {OLED_ShowCHinese(48,4,30); OLED_ShowCHinese(64,4,32);}
		}
	 
		
			 
	
  }
}

void TIM2_IRQHandler(void) //1ms 1次
{					  

	static u8 st;

	static u16 sendnum;
	st=TIM_GetFlagStatus(TIM2, TIM_IT_Update);
	if(st!=0)
	{  
      sendnum++;
      if(sendnum==1000)  //串口5020ms发送一次数据
			{
			  sendnum = 0;
				sendflag = 1;
			}
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	}
}

void wifiset(void)
{
  if(rexflag)
	{
	  rexflag = 0;
		switch(rexnum)
		{
		  case 0x31 :
				if(tempKG==11) {tempKG=22;} 
				else tempKG=11; break;
			case 0x32 :
				if(humiKG==11) {humiKG=22;} 
				else humiKG=11; break;
			case 0x33 :
				if(ywKG==11) {ywKG=22;} 
				else ywKG=11; break;
			case 0x34 :
				if(hyKG==11) {hyKG=22;} 
				else hyKG=11; break;
			case 0x35 :
				if(mcKG==11) {mcKG=22;} 
				else mcKG=11; break;

   }
	}
}
