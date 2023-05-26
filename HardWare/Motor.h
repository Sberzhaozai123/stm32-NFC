#ifndef MOTOR_START
	#define MOTOR_START		1
#endif

#ifndef MOTOR_STOP
	#define MOTOR_STOP		0
#endif

#ifndef __MOTOR_H__
#define __MOTOR_H__

void vMotor_Init(void);
void vMotor_SetSpeed(BaseType_t Speed);
uint8_t uMotor_Start(void);
uint8_t uMotor_Stop(void);

#endif

