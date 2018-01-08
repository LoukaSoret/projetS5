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


/*************************************************************************
Auteur : Kirill (+Louka modifications)
Date : 28/12/2017

Spec : Prends en argument le code op shift la valeur immediate shift_imm
le registre d'offset Rm re registre de shift Rs et le bit I qui determine
si le shift est execute avec shift_imm (0) ou avec Rs (1). Renvois la
valeur de Rm shiftee.
**************************************************************************/
int shift(arm_core p, uint8_t shift,uint8_t shift_imm, uint8_t Rm,uint8_t Rs,uint8_t I);

#endif
