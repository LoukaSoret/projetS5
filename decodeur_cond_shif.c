#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

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
			if( read_Z(p) && ( (read_N(p) && read_V(p)) || (!read_N(p) && !read_V(p)) ) ){
				return 1;
			}
			else{
				return 0;
			}
			break;
			
		case 13 :
			if( ( (!read_N(p) && read_V(p)) || (read_N(p) && !read_V(p)) ) ){
				return 1;
			}
			else{
				return 0;
			break;
		
		case 14 :
			break
			
		case 15 :
			break
			
		default :
			break;
	
	
	}
}

