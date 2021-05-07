#ifndef __CONTROL_H
#define __CONTROL_H

#include "sys.h"
#include "Struct_all.h"

static uint16_t MOT_Compute(uint16_t THROTTLE,int p,int r,int y);
void PID_Control(struct _pid *PID,uint16_t target,float measure,float max,float Integral_max);
void Control_Gyro(uint8_t Lock);//�ڻ�
void Control_Angle(void);//�⻷





#endif /*__CONTROL_H*/



