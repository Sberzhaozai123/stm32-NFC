#include "stm32f10x.h"                  // Device header
#include "portmacro.h"
//#include "task.h"

#define ENCODER_ADJ_MAX				12
#define ENCODER_EDGE_PERSTEP		4
#define ENCODER_EDGE_FILTER			0xF

void vEncoder_Init(void)    //TIM3
{
	/*输入引脚初始化*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//输入时钟使能
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;		//上拉输入
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	
	/*定时器初始化*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	//TIM_InternalClockConfig(TIM3);		//编码器作为外部时钟
	/*时基单元配置*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=ENCODER_ADJ_MAX-1;			//ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler=ENCODER_EDGE_PERSTEP-1;	//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;					//重复计数:无
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	/*输入通道初始化*/
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_1;//通道1 CCR1 PA6
	TIM_ICInitStruct.TIM_ICFilter=ENCODER_EDGE_FILTER;//滤波器
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;//高低电平极性不反转
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	TIM_ICInitStruct.TIM_Channel=TIM_Channel_2;//通道2 CCR2 PA7
	TIM_ICInitStruct.TIM_ICFilter=ENCODER_EDGE_FILTER;//滤波器
	TIM_ICInitStruct.TIM_ICPolarity=TIM_ICPolarity_Rising;//高低电平极性不反转
	TIM_ICInit(TIM3,&TIM_ICInitStruct);
	
	/*Encoder接口初始化*/
	TIM_EncoderInterfaceConfig(TIM3,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3,ENABLE);
}

BaseType_t xEncoder_Get(void)
{
	BaseType_t xTemp;
	xTemp=TIM_GetCounter(TIM3);
	//TIM_SetCounter(TIM3,0);
	return xTemp;
}

/*读取ccr寄存器获得旋转编码器的值*/
BaseType_t xEncoder_Count(void)
{
	BaseType_t Encoder_Count=0;
	Encoder_Count+=TIM_GetCounter(TIM3);
	//TIM_SetCounter(TIM3,0);
	return Encoder_Count;
}

