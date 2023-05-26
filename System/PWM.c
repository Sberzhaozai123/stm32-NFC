#include "stm32f10x.h"                  // Device header
#include "PWM.h"

//PWM 频率：1khz 分辨率：1000

void PWM_Init(void)
{
	/*初始化PA0为复用推挽输出PWM*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//GPIOA口RCC使能
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出 GPIO输出只能由寄存器控制
	GPIO_InitStruct.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	GPIO_SetBits(GPIOA, GPIO_Pin_0);
	
	/*初始化时基单元*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//内部时钟 TIM2通用定时器使能
	
	TIM_InternalClockConfig(TIM2);//选择内部时钟模式  默认模式可写可不写
	
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitSturct;
	TIM_TimeBaseInitSturct.TIM_ClockDivision=TIM_CKD_DIV1;//外部时钟滤波采样的频率可以由RCC分频得
	TIM_TimeBaseInitSturct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
	/*     计数频率=72MHz/（预分频+1）/（自动重载+1）    */
	TIM_TimeBaseInitSturct.TIM_Period=PWM_MAX-1;	   //ARR
	TIM_TimeBaseInitSturct.TIM_Prescaler=71; //PSC
	TIM_TimeBaseInitSturct.TIM_RepetitionCounter=0;//重复计数器的值  高级计时器特有
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitSturct);
	
	TIM_OCInitTypeDef TIM_OCInitStruct;			//通用定时器只用到一部分参数赋值
	TIM_OCStructInit(&TIM_OCInitStruct);    	//结构体内所有成员赋初值
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_Pulse=0;				//CCR
	TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	
	TIM_Cmd(TIM2,ENABLE);//开启定时器
}

void TIM_SetCCR(BaseType_t CCR)
{
	TIM_SetCompare1(TIM2,CCR);
}







