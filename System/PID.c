#include "stm32f10x.h"                  // Device header
#include "PID.h"
#include "PWM.h"


void PID_Set(PID* pid, float* Kp, float* Ki, float* Kd)
{
	pid->Kp=*Kp;
	pid->Ki=*Ki;
	pid->Kd=*Kd;
}

uint16_t PID_OUT(uint16_t TargetVal,uint16_t ActulVal,PID* pid)
{
#if (PID_LOCATION==1 && PID_INCREMENT==0)
	
	pid->PreError=TargetVal-ActulVal;
	pid->SumError+=pid->PreError;
	pid->PWM_Out= pid->Kp*pid->PreError 
				+ pid->Ki*pid->SumError
				+ pid->Kd*(pid->PreError-pid->LastError1);
	pid->LastError1=pid->PreError;
	
	if(TargetVal==0)
	{
		return 0;
	}
	if(pid->PWM_Out<0)
	{
		return 0;
	}
	if(pid->PWM_Out>PWM_MAX)
	{
		return PWM_MAX;
	}
	
	return (uint16_t)pid->PWM_Out;
	
#endif	
	
	
}


