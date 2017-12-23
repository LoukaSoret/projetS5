#include "arm_load_store.h"
#include "arm_core.h"
#include "memory.h"

void print_test(int result) {
    if (result)
        printf("Test succeded\n");
    else
        printf("TEST FAILED !!\n");
}

int compare(uint32_t a, uint32_t b) {
    return a == b;
}

int main(void){
	uint8_t reg,registerValueByte;
	uint32_t value, ins,address,memoryValue;
	memory mem = memory_create(128,1);
	arm_core p = arm_create(mem);

	value = 0;
	for(reg=0;reg<15;reg++){
		arm_write_register(p,reg,value);
		value ++;
	}

	ins = 0b11110101110000000010000000000000 ; //STRB R0,R2, #+0 w/ P=1 & W=0

	arm_load_store(p,ins);

	address = arm_read_usr_register(p,2);
	registerValueByte = arm_read_usr_register(p,0);
	memoryValue = memory_read_byte(mem,address, &valueByte);
	printf("Ecriture de %i a l'address %i : ",registerValueByte,address);
	print_test(compare(registerValueByte,memoryValue));

	return 0;
}

