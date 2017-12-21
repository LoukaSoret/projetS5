#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"
#include "decodeur_cond_shift.h"
//
int read_N(arm_core p){
	uint32_t cprs = arm_read_cpsr(p);
	
	return get_bit(cprs, 31);
}

int read_Z(arm_core p){
	uint32_t cprs = arm_read_cpsr(p);
	
	return get_bit(cprs, 30);
}

int read_C(arm_core p){
	uint32_t cprs = arm_read_cpsr(p);
	
	return get_bit(cprs, 29);
}

int read_V(arm_core p){
	uint32_t cprs = arm_read_cpsr(p);
	
	return get_bit(cprs, 28);
}

int condition(arm_core p, uint8_t cond){
	
	switch(cond){
		case 0 :
			if(read_Z(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 1 :
			if(!read_Z(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 2 :
			if(read_C(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 3 :
			if(!read_C(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 4 :
			if(read_N(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 5 :
			if(!read_N(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
		
		case 6 :
			if(read_V(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 7 :
			if(!read_V(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;

		case 8 :
			if(read_C(p) && !read_Z(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 9 :
			if(!read_C(p) || read_Z(p)){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 10 :
			if( (read_N(p) && read_V(p)) || (!read_N(p) && !read_V(p)) ){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 11 :
			if( (!read_N(p) && read_V(p)) || (read_N(p) && !read_V(p)) ){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 12 :
			if( !read_Z(p) && ( (read_N(p) && read_V(p)) || (!read_N(p) && !read_V(p)) ) ){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 13 :
			if( read_Z(p) && ( (!read_N(p) && read_V(p)) || (read_N(p) && !read_V(p)) ) ){
				return 1;
			}
			else{
				return 0;
			break;
		
		case 14 :
			return 0;
			break;
			
		case 15 :
			return -1;
			break;
			
		default :
			break;
	
		}
	}
	return 0;
}

int shift(arm_core p, uint16_t ins){
	int shift = get_bits(ins,6,4);
	int shift_imm = get_bits(ins,11,7);
	int8_t reg = get_bits(ins,3,0);
	int32_t value_reg = arm_read_register(p,reg);
	int value = 0;
	switch(shift){
		case LSL: // Shift left immediat
			if(shift_imm > 0){
				value = value_reg<<shift_imm;
			}else{
				value = value_reg;
			}
			return value;
		break;

		case LSLr: // Shift left avec registre
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

		case LSR: //Shit right immediat
			if(shift_imm > 0){
				value = value_reg>>shift_imm;
			}else{
				value = value_reg;
			}
			return value;
		break;

		case LSRr: // Shift right avec registre
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

		case ASR: // Shift arithmétique immediat
			return asr(value_reg,shift_imm);
		break;

		case ASRr: // Shift arithmétique avec registre
			int8_t regs = get_bits(arm_read_register(p,get_bits(ins,11,8)),7,0);
			return ror(value_reg,regs);
		break;

		case ROR: // Rotation immediat
			return ror(value_reg,shift_imm);
		break;

		case RORr: // rotation avec registre
			int8_t regs = get_bits(arm_read_register(p,get_bits(ins,11,8)),7,0);
			return ror(value_reg,regs);
		break;
	}
}


