#ifndef __NFC_CONFIG_H__
#define __NFC_CONFIG_H__

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "Encoder.h"
#include "OLED.h"
#include "HALL.h"
#include "PWM.h"
#include "Motor.h"
#include "Key.h"
#include "PID.h"

#define Xsec_Amin		60		//60s=1min
#define Xcnt_Al			5400 	//number of pulse each L water 
#define ONE_COMPENSATE	0.05    //in order to round up and down the one decimal place
#define INT_COMPENSATE	0.5     //in order to round up and down the integer

/**********************************************/
				  /*Task config*/


#define START_TASK_PRIO		1
#define START_STK_SIZE 		128  
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);


#define KEY_TASK_PRIO		2	
#define KEY_STK_SIZE 		30  
TaskHandle_t KeyTask_Handler;
void Key_task(void *pvParameters);


#define SYSINIT_TASK_PRIO		2	
#define SYSINIT_STK_SIZE 		100  
TaskHandle_t SysInitTask_Handler;
void SysInit_task(void *pvParameters);


#define OLED_TASK_PRIO		2	
#define OLED_STK_SIZE 		100  
TaskHandle_t OLEDTask_Handler;
void OLED_task(void *pvParameters);


#define ENCODER_TASK_PRIO		2	
#define ENCODER_STK_SIZE 		50  
TaskHandle_t EncoderTask_Handler;
void Encoder_task(void *pvParameters);


#define MOTOR_TASK_PRIO		2	
#define MOTOR_STK_SIZE 		100  
TaskHandle_t MotorTask_Handler;
void Motor_task(void *pvParameters);

/**********************************************/


/**********************************************/
				/*Software Timer*/
#define TIMER1_PERIOD_TICKS   100		//1Tick==1ms
#define TIMER1_ID             1
#define TIMER1_TICKSTOWAIT    10
TimerHandle_t Timer1_Sample;            //定时器句柄
void Timer1_PeriodicHALLSample_Function(TimerHandle_t xTimer);//定时器回调函数


#define TIMER2_PERIOD_TICKS   20		//1Tick==1ms
#define TIMER2_ID             2
#define TIMER2_TICKSTOWAIT    10
TimerHandle_t Timer2_Sample;            //定时器句柄
void Timer2_PeriodicScan_Function(TimerHandle_t xTimer);//定时器回调函数
/**********************************************/


/**********************************************/
				/*Queue1*/
#define QUEUE_ENCODER_LENGTH  5
#define QUEUE_ENCODER_TICKSTOWAIT	  5
QueueHandle_t Queue_Encoder;
			
/**********************************************/


#endif
