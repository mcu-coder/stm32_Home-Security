#include "dhtll.h"
#include "stm32f10x.h"  //°üº¬ÐèÒªµÄÍ·ÎÄ¼þ
#include "delay.h"      //°üº¬ÐèÒªµÄÍ·ÎÄ¼þ

//DHT11³õÊ¼»¯ 
//·µ»Ø0£º³õÊ¼»¯³É¹¦£¬1£ºÊ§°Ü
u8 DHT11_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);

	GPIO_InitStructure.GPIO_Pin=DHT11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
	GPIO_SetBits(GPIO_DHT11,DHT11);	   //À­¸ß

	DHT11_Rst();	  
	return DHT11_Check();	
}

//¸´Î»DHT11
void DHT11_Rst()	   
{                 
	  DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT=0; 	//À­µÍDQ
    Delay_Ms(20);    	//À­µÍÖÁÉÙ18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	  Delay_Us(30);     	//Ö÷»úÀ­¸ß20~40us
}

//µÈ´ýDHT11µÄ»ØÓ¦
//·µ»Ø1:Î´¼ì²âµ½DHT11µÄ´æÔÚ
//·µ»Ø0:´æÔÚ
u8 DHT11_Check() 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 
    while (DHT11_DQ_IN&&retry<100)//DHT11»áÀ­µÍ40~50us
	{
		retry++;
		Delay_Us(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11À­µÍºó»áÔÙ´ÎÀ­¸ß40~50us
	{
		retry++;
		Delay_Us(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//´ÓDHT11¶ÁÈ¡Ò»¸öÎ»
//·µ»ØÖµ£º1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//µÈ´ý±äÎªµÍµçÆ½ 12-14us ¿ªÊ¼
	{
		retry++;
		Delay_Us(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//µÈ´ý±ä¸ßµçÆ½	 26-28us±íÊ¾0,116-118us±íÊ¾1
	{
		retry++;
		Delay_Us(1);
	}
	Delay_Us(40);//µÈ´ý40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}

//´ÓDHT11¶ÁÈ¡Ò»¸ö×Ö½Ú
//·µ»ØÖµ£º¶Áµ½µÄÊý¾Ý
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//´ÓDHT11¶ÁÈ¡Ò»´ÎÊý¾Ý
//temp:ÎÂ¶ÈÖµ(·¶Î§:0~50¡ã)
//humi:Êª¶ÈÖµ(·¶Î§:20%~90%)
//·µ»ØÖµ£º0,Õý³£;1,¶ÁÈ¡Ê§°Ü
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//¶ÁÈ¡40Î»Êý¾Ý
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
		
	}else return 1;
	return 0;	    
}

//DHT11Êä³öÄ£Ê½ÅäÖÃ
void DHT11_IO_OUT()	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;	 //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);	
}

//DHT11ÊäÈëÄ£Ê½ÅäÖÃ
void DHT11_IO_IN()	
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=DHT11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	 //ÉÏÀ­ÊäÈëÄ£Ê½
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);	
}







