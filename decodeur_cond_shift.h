#ifndef __DECODEUR_COND_SHIFT_H__
#define __DECODEUR_COND_SHIFT_H__
#include <stdint.h>
#include "arm_core.h"
//
int read_N(arm_core p);
int read_Z(arm_core p);
int read_C(arm_core p);
int read_V(arm_core p);
int condition(arm_core p, uint8_t cond);
int shift(arm_core p, uint16_t ins);

#endif
