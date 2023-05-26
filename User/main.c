#include "NFC_Config.h"

static UBaseType_t HALL_CNTActul=0,HALL_CNTTarget=0;		//1L Water==5880CNT
static float HALL_Flow=0;
static uint8_t Motor_Sts=MOTOR_STOP;
static float kp=58.0,ki=8.0,kd=24.0;
uint16_t PID_PWM_Duty=0;
PID pid;

int main()
{
	
    xTaskCreate((TaskFunction_t )start_task,            //task function
                (const char*    )"start_task",          //task name
                (uint16_t       )START_STK_SIZE,        //stack size of task
                (void*          )NULL,                  //parameter to the task function
                (UBaseType_t    )START_TASK_PRIO,       //task priority
                (TaskHandle_t*  )&StartTask_Handler);   //a pointer point to the task              
    vTaskStartScheduler();      
}


//Create a start task
static void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //close the interrupt
      
    //Create a initialize task
    xTaskCreate((TaskFunction_t )SysInit_task,    
                (const char*    )"SysInit_task",   
                (uint16_t       )SYSINIT_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )SYSINIT_TASK_PRIO,
                (TaskHandle_t*  )&SysInitTask_Handler); 
	
 
	//create a sw timer
	Timer1_Sample=xTimerCreate((const char *) "TIMER1_SAMPLE",/*use to sample the hall value*/
				 TIMER1_PERIOD_TICKS,
				 pdTRUE,					/*periodic timer*/
				 (void *) TIMER1_ID,
				 Timer1_PeriodicHALLSample_Function); 
				 
	Timer2_Sample=xTimerCreate((const char *) "TIMER2_SAMPLE",/**/
				 TIMER2_PERIOD_TICKS,
				 pdTRUE,					
				 (void *) TIMER2_ID,
				 Timer2_PeriodicScan_Function); 
				 
	//Create a Queue to transmit Encoder value
	Queue_Encoder=xQueueCreate(QUEUE_ENCODER_LENGTH,
				 sizeof(uint32_t));
				 
				 
				 
	//Create a Key task
    xTaskCreate((TaskFunction_t )Key_task,     
                (const char*    )"Key_task",   
                (uint16_t       )KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KeyTask_Handler);		 
	
    //Create a OLED display task				
	xTaskCreate((TaskFunction_t )OLED_task,     
                (const char*    )"OLED_task",   
                (uint16_t       )OLED_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )OLED_TASK_PRIO,
                (TaskHandle_t*  )&OLEDTask_Handler); 
				
	//Create a encoder task
	xTaskCreate((TaskFunction_t )Encoder_task,     
                (const char*    )"Encoder_task",   
                (uint16_t       )ENCODER_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )ENCODER_TASK_PRIO,
                (TaskHandle_t*  )&EncoderTask_Handler); 
				
	//Create a Motro task
	xTaskCreate((TaskFunction_t )Motor_task,     
                (const char*    )"Motor_task",   
                (uint16_t       )MOTOR_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR_TASK_PRIO,
                (TaskHandle_t*  )&MotorTask_Handler); 
				
	xTimerStart(Timer1_Sample,TIMER1_TICKSTOWAIT);//enable the sw timer1
    vTaskDelete(StartTask_Handler); 			//Delete the start task and free stacks
    taskEXIT_CRITICAL();            			//open interrupt
} 



//Timer call back function.Use to sample hall flow and pid calculate
static void Timer1_PeriodicHALLSample_Function(TimerHandle_t xTimer)
{	
	taskENTER_CRITICAL();
	HALL_Flow=((HALL_CNTActul*(1000.0/TIMER1_PERIOD_TICKS)*Xsec_Amin)/Xcnt_Al)+ONE_COMPENSATE;//use to display on the OLED
	HALL_CNTActul=uxHALL_GetCount();//use to fuzzy pid
	PID_PWM_Duty=PID_OUT(HALL_CNTTarget,HALL_CNTActul,&pid);
	taskEXIT_CRITICAL();
}

static void Timer2_PeriodicScan_Function(TimerHandle_t xTimer)
{
	/*nothing to do*/
}


//System initialize function
void SysInit_task(void *pvParameters)//BLUE
{
    while(1)
    {
		taskENTER_CRITICAL();
		
        SysTick_Init(72);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
		LED_Init();
		USART1_Init(115200);
		OLED_Init();
		vEncoder_Init();
		vHALL_Init();
		vMotor_Init();
		vKey_Init();
		OLED_ShowString(1,1,"ADJ:");
		OLED_ShowString(1,11," L/Min");
		OLED_ShowString(2,1,"FLOW:");
		OLED_ShowString(2,11," L/Min");
		uMotor_Start();
		vTaskSuspend(MotorTask_Handler);
		Motor_Sts=MOTOR_STOP;
		PID_Set(&pid,&kp,&ki,&kd);
		vTaskDelete(NULL);
		taskEXIT_CRITICAL();
    }
}


void Key_task(void *pvParameters)
{
	uint8_t KeyNum;
    while(1)
    {
//        KEY_Sts=uKey_Scan();
//		delay_ms(20);	
		KeyNum=uKey_Scan();
		if(KeyNum==KEY1_NUM)
		{
			vTaskResume(MotorTask_Handler);
			uMotor_Start();
			Motor_Sts=MOTOR_START;
			
		}
		if(KeyNum==KEY2_NUM)
		{
			vTaskSuspend(MotorTask_Handler);
			uMotor_Stop();
			Motor_Sts=MOTOR_STOP;
		}
    }
}

/*OLED receive data from Queue1/2 and display encoder value and hall value*/
static void OLED_task(void *pvParameters)
{
	BaseType_t Encoder_AdjVal , Queue_EncoderReceiveSts;
	while(1)
	{
		Queue_EncoderReceiveSts=xQueueReceive(Queue_Encoder,&Encoder_AdjVal,QUEUE_ENCODER_TICKSTOWAIT);
		OLED_ShowFloatNum(1,6,(Encoder_AdjVal/10.0),2,1);
		OLED_ShowFloatNum(2,6,HALL_Flow,2,1);
		//OLED_ShowFloatNum(3,6,HALL_CNTActul,2,1);
		//OLED_ShowFloatNum(4,6,HALL_CNTTarget,2,1);
	}
}

//Encoder get, send data to Queue1 and set expect hall count 
static void Encoder_task(void *pvParameters)
{
	BaseType_t Encoder_Adj,Queue_EncoderSendSts;
	
	while(1)
	{
		Encoder_Adj=xEncoder_Count();
		Queue_EncoderSendSts=xQueueSendToBack(Queue_Encoder,&Encoder_Adj,QUEUE_ENCODER_TICKSTOWAIT);
		HALL_CNTTarget=(Encoder_Adj*Xcnt_Al/(1000.0/TIMER1_PERIOD_TICKS)/Xsec_Amin/10+INT_COMPENSATE);
	}
}


void Motor_task(void *pvParameters)
{
	while(1)
	{
		vMotor_SetSpeed(PID_PWM_Duty);
		//Motor_Sts=MOTOR_START;
	}

}


