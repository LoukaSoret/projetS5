/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

struct registers_data {
	uint32_t reg_no[18]; 	//16 registres + cpsr + spsr
	uint8_t mode;					/* 7 modes : 10000 User (0)		10001 FIQ (1)	10010 IRQ (2)		10011 Supervisor (3)		10111 Abort (4)		11011 Undefined (5)		11111 System (6)
	Privileged modes: tous sauf User
	SPSR: tous sauf User et System*/
};

registers registers_create() {
    registers r = malloc(sizeof(uint32_t)*18+sizeof(uint8_t));
    r->mode = 16; 
    return r;
}

void registers_destroy(registers r) {
 	free(r);
}

uint8_t get_mode(registers r) {
    return r->mode;
} 

int current_mode_has_spsr(registers r) {
	if (r->mode == 16 || r->mode == 31){
    return 0;
   }
   else{
   	return 1;
   }
}

int in_a_privileged_mode(registers r) {
    if (r->mode == 16){
    return 0;
   }
   else{
   	return 1;
   }
}

uint32_t read_register(registers r, uint8_t reg) {
	return r->reg_no[reg];
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    if(reg >= 0 && reg <= 12){
		return r->reg_no[reg];
	}
	else{
		return -1;
	}
}

uint32_t read_cpsr(registers r) {
    return r->reg_no[16];
}

uint32_t read_spsr(registers r) {
    return r->reg_no[17];
}

void write_register(registers r, uint8_t reg, uint32_t value) {
	r->reg_no[reg] = value;
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	if(reg >= 0 && reg <= 12){
		r->reg_no[reg] = value;
	}
}

void write_cpsr(registers r, uint32_t value) {
	r->reg_no[16] = value;
}

void write_spsr(registers r, uint32_t value) {
	if(current_mode_has_spsr(r)){
		r->reg_no[17] = value;
	}
}
