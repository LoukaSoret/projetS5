#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "arm_core.h"
#include "util.h"
#include "debug.h"

int condition(arm_core p, uint8_t ins){
	return UNDEFINED_INSTRUCTION;
	
}


/*************************************************************************
Auteur : Kirill (+Louka modifications)
Date : 28/12/2017

Spec : Prends en argument le code op shift la valeur immediate shift_imm
le registre d'offset Rm re registre de shift Rs et le bit I qui determine
si le shift est execute avec shift_imm (0) ou avec Rs (1). Renvois la
valeur de Rm shiftee.
**************************************************************************/
int shift(arm_core p, uint8_t shift,uint8_t shift_imm, uint8_t Rm,uint8_t Rs,uint8_t I)
{
	int32_t Rm_value = arm_read_register(p,Rm);
	uint32_t Rs_value;

	// mem[Rm] shift mem[Rs]  
	if(I){
		Rs_value = arm_read_register(p,Rs);
		switch(shift){
			case LSL: // Shift left immediat
				return Rm_value<<Rs_value;
			break;

			case LSR: //Shit right immediat
				if(Rs_value > 0){
					return Rm_value>>Rs_value;
				}else{
					return Rm_value;
				}
			break;

			case ASR: // Shift arithmétique immediat
				return asr(Rm_value,Rs_value);
			break;

			case ROR: // Rotation immediat
				return ror(Rm_value,Rs_value);
			break;

			default:
				return 0;
			break;
		}
	// mem[Rm] shift imm_shift
	}else{
		switch(shift){
			case LSL: // Shift left immediat
				return Rm_value<<shift_imm;
			break;

			case LSR: //Shit right immediat
				if(shift_imm > 0){
					return Rm_value>>shift_imm;
				}else{
					return Rm_value;
				}
			break;

			case ASR: // Shift arithmétique immediat
				return asr(Rm_value,shift_imm);
			break;

			case ROR: // Rotation immediat
				return ror(Rm_value,shift_imm);
			break;

			default:
				return 0;
			break;
		}
	}
	return -1;
}