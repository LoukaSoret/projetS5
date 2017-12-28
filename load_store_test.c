#include <stdio.h>
#include <stdlib.h>
#include "arm_load_store.h"
#include "arm_core.h"
#include "memory.h"
#include "util.h"


void compare(uint32_t a, uint32_t b) {
    if (a == b){
    	printf("SUCCESS ( %i == %i )",a,b);
    } else {
    	printf("FAILURE ( %i != %i )",a,b);
    }
}

/*********************************************
Auteur: Louka
Date: 28/12/2017
Spec: Programme de test pour arm_load_store.c
**********************************************/
int main(void){
	char *endianess[] = { "little", "big" };
	int returnValue;
	uint8_t reg,registerValueByte,memoryValueByte ;
	uint32_t value, ins,address,registerValue,memoryValue;
	memory mem = memory_create(128,is_big_endian());
    if ((mem == NULL)) {
        fprintf(stderr, "Error when creating simulated memory\n");
        exit(1);
    }

	arm_core p = arm_create(mem);

	value = 0;
	for(reg=0;reg<15;reg++){
		arm_write_register(p,reg,value);
		value ++;
	}

	printf("I'm a %s endian host\n", endianess[is_big_endian()]);

	//STRB R0,R2, #+0 w/ P=1 & W=0
	ins = 0b11110101110000000010000000000000 ;

	returnValue = arm_load_store(p,ins);

	address = arm_read_usr_register(p,2);
	registerValueByte = arm_read_usr_register(p,0);
	arm_read_byte(p,address, &memoryValueByte);
	printf("Ecriture de %i sur 8 bits a l'address %i : ",registerValueByte,address);
	compare(registerValueByte,memoryValueByte);
	printf(" <returnValue %i> \n",returnValue);

	//STR R5,R2,#+2 w/ P=1 & W=0
	ins = 0b11110101100000100101000000000010;

	returnValue = arm_load_store(p,ins);

	address = arm_read_usr_register(p,2);
	registerValue = arm_read_usr_register(p,5);
	arm_read_word(p,address, &memoryValue);
	printf("Ecriture de %i sur 32 bits a l'address %i: ",registerValue,address);
	compare(registerValue,memoryValue);
	printf(" <returnValue %i> \n",returnValue);

	//LDR R12,R2,#+0 w/ P=1 & W=0
	ins = 0b11110101100100101100000000000000;

	returnValue = arm_load_store(p,ins);
	
	address = arm_read_usr_register(p,2);
	registerValue = arm_read_usr_register(p,12);
	printf("Transfere dans R%i du contenu a l'address %i: ",12,address);
	arm_read_word(p,address,&memoryValue);
	compare(registerValue,memoryValue);
	printf(" <returnValue %i> \n",returnValue);


	return 0;
}

