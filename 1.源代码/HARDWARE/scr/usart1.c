

#include "stm32f10x.h"  //包含需要的头文件
#include "usart1.h"     //包含需要的头文件

#if  USART1_RX_ENABLE                   //如果使能接收功能
char Usart1_RxCompleted = 0;            //定义一个变量 0：表示接收未完成 1：表示接收完成 
unsigned int Usart1_RxCounter = 0;      //定义一个变量，记录串口1总共接收了多少字节的数据
char Usart1_RxBuff[USART1_RXBUFF_SIZE]; //定义一个数组，用于保存串口1接收到的数据   	
#endif

/*-------------------------------------------------*/
/*函数名：初始化串口1发送功能                      */
/*参  数：bound：波特率                            */
/*返回值：无                                       */
/*-------------------------------------------------*/


void Usart2_Init(unsigned int bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能GPIOA时钟
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.2
   
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.3 

  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口2
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART2, ENABLE);                    //使能串口2
		
}



__align(8) char Usart2_TxBuff[USART2_TXBUFF_SIZE];  

void u2_printf(char* fmt,...) 
{  
	unsigned int i,length;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(Usart2_TxBuff,fmt,ap);
	va_end(ap);	
	
	length=strlen((const char*)Usart2_TxBuff);		
	while((USART2->SR&0X40)==0);
	for(i = 0;i < length;i ++)
	{			
		USART2->DR = Usart2_TxBuff[i];
		while((USART2->SR&0X40)==0);	
	}	
}

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
				USART_SendData(pUSARTx,ch);        /*   ?????????USART   */        
				while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);        /*   ???????????   */
}

//void Usart1_SendArray( USART_TypeDef * pUSART1, uint8_t *array, uint16_t num) //发送8位数组
//{
//				uint8_t i;        
//				for(i=0; i<num; i++)
//				{
//								Usart1_SendByte(pUSART1,array[i]);          /*  ?????USART   */
//				}                        
//				while(USART_GetFlagStatus(pUSART1,USART_FLAG_TC)==RESET); /*  ??????   */
//}


//void Usart_SendHalfWord( USART_TypeDef * pUSART1, uint16_t ch) //发送一个16进制数
//{
//				uint8_t temp_h, temp_l;        
//				temp_h = (ch&0XFF00)>>8;          /*     ???8?      */
//				temp_l = ch&0XFF;                              /*     ???8?      */
//				USART_SendData(pUSART1,temp_h);         /*   ???8?   */
//				while (USART_GetFlagStatus(pUSART1, USART_FLAG_TXE) == RESET);        
//				USART_SendData(pUSART1,temp_l);         /*     ???8?     */
//				while (USART_GetFlagStatus(pUSART1, USART_FLAG_TXE) == RESET);        
//}
    /*
    * ???:Usart_SendString
    * ??   :?????
    */
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
				unsigned int k=0;
				do 
				{
								Usart_SendByte( USART2, *(str + k) );
								k++;
				} while(*(str + k)!='\0');
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); /*    ??????      */              
}
