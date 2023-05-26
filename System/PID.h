#ifndef __PID_H__
#define __PID_H__

#ifndef PID_LOCATION
	#define PID_LOCATION	1
#endif

#ifndef PID_INCREMENT
	#define PID_INCREMENT	0
#endif


typedef struct{
				float Kp;
				float Ki;
				float Kd;
				float T;
	
				float PreError;  //Ek
				float LastError1;//E(k-1)
				float LastError2;//E(k-2)
				float SumError;	
				float PWM_Out;
}PID;

void PID_Set(PID* pid, float* Kp, float* Ki, float* Kd);
uint16_t PID_OUT(uint16_t TargetVal,uint16_t ActulVal,PID* pid);

#endif
