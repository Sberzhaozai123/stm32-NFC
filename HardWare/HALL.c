#include "stm32f10x.h"                  // Device header
#include "portmacro.h"

volatile UBaseType_t uxHALL_Count;
extern UBaseType_t uxHALL_Flow;


void vHALL_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;//GPIO��ʼ��
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;//��������
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	/*����AFIO*/
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource3);//AFIO������ѡ���� ѡ��һ��GPIO��Ϊ�ж�Դ
	
	/*AFIO������ɺ�����˱�����GPIOB_0/1�ˣ��������EXTI��0/1���ж���·*/
	
	/*����EXTI*/
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line=EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd=ENABLE;
	EXTI_InitStruct.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
	EXTI_Init(&EXTI_InitStruct);//�жϳ�ʼ��
	
	/*����NVIC*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���鷽ʽÿ��оƬֻ����һ�� ȷ��ÿ��ģ�鶼��ͬһ������
	/*NVIC���ж����ȼ������ȼ��Ĵ�����4λ��0~15����������4λ���Խ����з֣�
	��Ϊ��nλ����ռ���ȼ��͵�4-nλ����Ӧ���ȼ�*/
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=EXTI3_IRQn;//A
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;//�����ȼ�
	NVIC_Init(&NVIC_InitStruct);
	
}

UBaseType_t uxHALL_GetCount(void)
{
	UBaseType_t EncoderCount;
	EncoderCount=uxHALL_Count;//���жϷ������б仯
	uxHALL_Count=0;
	return EncoderCount;
}


void EXTI3_IRQHandler(void)		//�½��ش���
{
	if(EXTI_GetITStatus(EXTI_Line3) == SET)
	{
		uxHALL_Count++;
		EXTI_ClearITPendingBit(EXTI_Line3);//ִ�к����������־λ ������һֱִ��
	}
}



