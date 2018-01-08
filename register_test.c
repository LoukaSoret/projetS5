#include <stdio.h>
#include <stdlib.h>
#include "registers.h"
#include "arm_constants.h"

int main (){
	registers r;
	uint32_t t;
	int test;
	uint8_t mode;
	
	r = registers_create();
	printf("R initialise\n");
	
	write_register(r, 1, 2);
	printf("R1 plein\n");
	
	write_usr_register(r, 2, 4);
	printf("R2 plein\n");
	
	write_register(r, 27, 50);
	printf("Test faux\n");
	
	mode = get_mode(r);
	printf("Mode : %d\n", mode);
	
	test = current_mode_has_spsr(r);
	printf("SPSR? : %d\n", test);
	
	test = in_a_privileged_mode(r);
	printf("Privileged mode : %d\n", test);
	
	t = read_register(r, 1);
	printf("R1 : %d\n",t);
	
	t = read_register(r, 2);
	printf("R2 : %d\n",t);
	
	t = read_register(r, 5);
	printf("R5 : %d - Ce test ne doit pas marcher \n",t);
	
	t = read_register(r, 27);
	printf("R27 : %d - Ce test ne doit pas marcher \n",t);
	
	write_cpsr(r, 20);
	
	t = read_cpsr(r);
	printf("CPSR : %d\n",t);
	
	write_spsr(r, 40);
	
	t = read_spsr(r);
	printf("SPSR : %d\n",t);
	
	registers_destroy(r);
	printf("Registers destroyed\n");
	
	t = read_register(r, 1);
	printf("R1 : %d - Ce test ne doit pas marcher \n",t);
	
	return 0;
}
