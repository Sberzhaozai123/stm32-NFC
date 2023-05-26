#include "portmacro.h"

#ifndef __PWM_H__
#define __PWM_H__

#define PWM_MAX			1000

void PWM_Init(void);
void TIM_SetCCR(BaseType_t CCR);

#endif

#define vPWM_SetDuty(Duty)        TIM_SetCCR(Duty)
