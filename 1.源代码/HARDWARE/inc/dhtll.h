#ifndef _dht11_H
#define _dht11_H

#include "system.h"
#include "SysTick.h"


#define DHT11 (GPIO_Pin_10) //PG11
#define GPIO_DHT11 GPIOB

#define DHT11_DQ_IN   PBin(10)	  // ‰»Î
#define DHT11_DQ_OUT  PBout(10)  // ‰≥ˆ

void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
u8 DHT11_Init(void);
void DHT11_Rst(void);
u8 DHT11_Check(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Data(u8 *temp,u8 *humi);


#endif


