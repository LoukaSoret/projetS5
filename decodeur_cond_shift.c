#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "arm_core.h"
#include "util.h"
#include "debug.h"

int condition(arm_core p, uint32_t ins){
	int cond = get_bits(ins, 31, 28);
	
}

int shift(arm_core p, uint16_t ins){
	int shift = get_bits(ins,6,4);
	int shift_imm = get_bits(ins,11,7);
	int8_t reg = get_bits(ins,3,0);
	int32_t value_reg = arm_read_register(p,reg);
	int value = 0;
	switch(shift){
		case LSL:
			if(shift_imm > 0){
				value = value_reg<<shift_imm;
			}else{
				value = value_reg;
			}
			return value;
		break;

		case LSLr:
			int8_t regs = get_bits(arm_read_register(p,get_bits(ins,11,8)),7,0);
			if(regs < 32){
				value = value_reg<<regs;
			}else if(regs == 0){
				value = value_reg;
			}else{
				value = 0;
			}
			return value;
		break;

		case LSR:
			if(shift_imm > 0){
				value = value_reg>>shift_imm;
			}else{
				value = value_reg;
			}
			return value;
		break;

		case LSRr:
			int8_t regs = get_bits(arm_read_register(p,get_bits(ins,11,8)),7,0);
			if(regs < 32){
				value = value_reg>>regs;
			}else if(regs == 0){
				value = value_reg;
			}else{
				value = 0;
			}
			return value;
		break;

		case ASR:
			return asr(value_reg,shift_imm);
		break;

		case ASRr:
			int8_t regs = get_bits(arm_read_register(p,get_bits(ins,11,8)),7,0);
			return ror(value_reg,regs);
		break;

		case ROR:
			return ror(value_reg,shift_imm);
		break;

		case RORr:
			int8_t regs = get_bits(arm_read_register(p,get_bits(ins,11,8)),7,0);
			return ror(value_reg,regs);
		break;
	}
}