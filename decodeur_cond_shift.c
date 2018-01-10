#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "arm_core.h"
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
			return 1;
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

/*************************************************************************
Auteur : Kirill (+Louka modifications)
Date : 28/12/2017
Spec : Prends en argument le code op shift la valeur immediate shift_imm
le registre d'offset Rm re registre de shift Rs et le bit I qui determine
si le shift est execute avec shift_imm (0) ou avec Rs (1). Renvois la
valeur de Rm shiftee.
**************************************************************************/
uint32_t shift(arm_core p, uint8_t shift,uint8_t shift_imm, uint8_t Rm,uint8_t Rs,uint8_t I)
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
				return Rm_value;
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
				return Rm_value;
			break;
		}
	}
	return -1;
}
