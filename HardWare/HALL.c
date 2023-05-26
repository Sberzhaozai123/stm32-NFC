#include "stm32f10x.h"                  // Device header
#include "portmacro.h"

volatile UBaseType_t uxHALL_Count;
extern UBaseType_t uxHALL_Flow;


void vHALL_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;//GPIO初始化
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/*配置AFIO*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);//AFIO的数据选择器 选择一个GPIO作为中断源
	
	/*AFIO配置完成后，输入端被拨到GPIOB_0/1端，输出链接EXTI第0/1个中断线路*/
	
	/*配置EXTI*/
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStruct);//中断初始化
	
	/*配置NVIC*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组方式每个芯片只能用一种 确保每个模块都是同一个分组
	/*NVIC的中断优先级由优先级寄存器的4位（0~15）决定，这4位可以进行切分，
	分为高n位的抢占优先级和低4-n位的响应优先级*/
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;//A
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//从优先级
	NVIC_Init(&NVIC_InitStruct);
	
}

UBaseType_t uxHALL_GetCount(void)
{
	UBaseType_t EncoderCount;
	EncoderCount=uxHALL_Count;//在中断服务函数中变化
	uxHALL_Count=0;
	return EncoderCount;
}


void EXTI3_IRQHandler(void)		//下降沿触发
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		uxHALL_Count++;
		EXTI_ClearITPendingBit(EXTI_Line3);//执行函数后清除标志位 否则函数一直执行
	}
}



