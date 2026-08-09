#ifndef __STEPPER_H
#define __STEPPER_H

#include "tim.h"


void Stepper_Start(uint8_t dir);
void Stepper_Config(int8_t set_num);
void Stepper_Stop(void);


#endif
