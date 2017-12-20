#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

int condition(arm_core p, uint32_t ins){
	int cond = get_bits(ins, 31, 28);
	
}
