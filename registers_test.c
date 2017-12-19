#include <stdio.h>
#include <stdlib.h>
#include "registers.h"
#include "arm_constants.h"

void print_test(int result) {
    if (result)
        printf("Test succeded\n");
    else
        printf("TEST FAILED !!\n");
}

int compare(uint32_t a, uint32_t b) {
    return a == b;
}

int compare_with_sim(uint32_t a, registers r, uint8_t select) {
    uint32_t value;
	value = read_register(r, select);
        if (a != value)
            return 0;
    return 1;
}

int main (){
	registers r;
	uint32_t t;
	int test;
	uint8_t mode;
	int a = 2;
	int b = 75;
	int c = 56;
	int d = 20;
	
	r = registers_create();
	printf("registers init : ");
	print_test(r!=NULL);
	
	write_register(r, 1, a);
	printf("Ecriture de %i dans R1 : ",a);
	print_test(compare_with_sim(a,r,1));
	
	write_usr_register(r, 2, b);
	printf("Ecriture de %i dans R2 : ",b);
	print_test(compare_with_sim(b,r,2));
	
	write_register(r, 7, c);
	printf("Ecriture de %i dans R7 : ",c);
	print_test(compare_with_sim(c,r,7));
	
	t = read_register(r, 1);
	printf("Lecture de R1 : ");
	print_test(compare(t,a));
	
	t = read_register(r, 2);
	printf("Lecture de R2 : ");
	print_test(compare(t,b));
	
	t = read_register(r, 7);
	printf("Lecture de R7 : ");
	print_test(compare(t,c));
	
	write_cpsr(r, d);
	printf("Ecriture de %i dans CPSR : ",d);
	print_test(compare_with_sim(d,r,16));
	
	t = read_cpsr(r);
	printf("Lecture de CPSR : ");
	print_test(compare(t,d));
	
	printf("----------- Droits insufisants -----------\n");

	write_spsr(r, b);
	printf("Ecriture de %i dans SPSR : ",b);
	print_test(compare_with_sim(b,r,17));

	t = read_spsr(r);
	printf("Lecture de SPSR : ");
	print_test(compare(t,b));

	printf("------------------------------------------\n");
	
	mode = get_mode(r);
	printf("Mode courant des registres: %d\n", mode);
	
	test = current_mode_has_spsr(r);
	printf("SPSR? : %d\n", test);
	
	test = in_a_privileged_mode(r);
	printf("Privileged mode : %d\n", test);

	registers_destroy(r);
	printf("Destroy registers.");
	
	return 0;
}
