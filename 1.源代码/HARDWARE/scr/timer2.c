

#include "stm32f10x.h"  //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：定时器2使能1s定时                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void TIM2_Init(void)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

RCC_APB1PeriphClockCmd (RCC_APB1Periph_TIM2,ENABLE);

 

TIM_TimeBaseStructure.TIM_Period = 1000-1; //计数个数     //100us*10=1000us=10ms
TIM_TimeBaseStructure.TIM_Prescaler =72-1;//分频值   	    
TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; 	//分割时钟			
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
 TIM_DeInit(TIM2);
TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure); 
TIM_Cmd(TIM2, ENABLE); 	 //使能定时器2

 /*以下定时器4中断初始化*/
TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //向上计数溢出产生中断                             //设置中断
}
void Isr_Init(void)
{
NVIC_InitTypeDef NVIC_InitStructure; 
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;// TIM2_IRQChannel; 
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
NVIC_Init (&NVIC_InitStructure); 

}

