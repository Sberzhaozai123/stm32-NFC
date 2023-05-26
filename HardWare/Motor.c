#include "stm32f10x.h"                  // Device header
#include "portmacro.h"
#include "PWM.h"

#define MOTOR_START		1
#define MOTOR_STOP		0

uint8_t uMotor_Start(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	return MOTOR_START;
}

uint8_t uMotor_Stop(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	return MOTOR_STOP;
}

void vMotor_SetSpeed(BaseType_t Speed)
{
		vPWM_SetDuty(Speed);
}


void vMotor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	PWM_Init();
}



