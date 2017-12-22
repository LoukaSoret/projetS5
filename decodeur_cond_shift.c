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

int shift(arm_core p, uint16_t ins){
	int shift = get_bits(ins,6,5);
	int shift_imm = get_bits(ins,11,7);
	int8_t reg = get_bits(ins,3,0);
	int32_t value_reg = arm_read_register(p,reg);
	if(get_bit(ins,4)){
		int regs = get_bits(ins,11,8);
		uint32_t regs_value = arm_read_register(p,regs);
		switch(shift){
			case LSL: // Shift left immediat
				return value_reg<<regs_value;
			break;

			case LSR: //Shit right immediat
				if(regs_value > 0){
					return value_reg>>regs_value;
				}else{
					return value_reg;
				}
			break;

			case ASR: // Shift arithmétique immediat
				return asr(value_reg,regs_value);
			break;

			case ROR: // Rotation immediat
				return ror(value_reg,regs_value);
			break;

			default:
				return 0;
			break;
		}
	}else{
		switch(shift){
			case LSL: // Shift left immediat
				return value_reg<<shift_imm;
			break;

			case LSR: //Shit right immediat
				if(shift_imm > 0){
					return value_reg>>shift_imm;
				}else{
					return value_reg;
				}
			break;

			case ASR: // Shift arithmétique immediat
				return asr(value_reg,shift_imm);
			break;

			case ROR: // Rotation immediat
				return ror(value_reg,shift_imm);
			break;

			default:
				return 0;
			break;
		}
	}
	
}