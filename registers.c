/*
Armator - simulateur de jeu d'instruction ARMv5T à but pédagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique Générale GNU publiée par la Free Software
Foundation (version 2 ou bien toute autre version ultérieure choisie par vous).

Ce programme est distribué car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but spécifique. Reportez-vous à la
Licence Publique Générale GNU pour plus de détails.

Vous devez avoir reçu une copie de la Licence Publique Générale GNU en même
temps que ce programme ; si ce n'est pas le cas, écrivez à la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
États-Unis.

Contact: Guillaume.Huard@imag.fr
	 Bâtiment IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'Hères
*/
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

struct registers_data {
	//13 registres general + 5 registres FIQ + 6*SP + 6*LR + PC + CPSR + 5*spsr
	uint32_t reg_no[36];

	/*7 modes : 
				10000 User (USR 0x10)
				10001 FIQ (FIQ 0x11)
				10010 IRQ (IRQ 0x12)
				10011 Supervisor (SVC 0x13)
				10111 Abort (ABT 0x17)
				11011 Undefined (UND 0x1b)
				11111 System (SYS 0x1f)

	Privileged modes: tous sauf User
	SPSR: tous sauf User et System*/
};

registers registers_create() {
	registers r = malloc(sizeof(struct registers_data));
	return r;
}

void registers_destroy(registers r) {
 	free(r->reg_no);
 	free(r);
}

uint8_t get_mode(registers r) {
	return r->reg_no[CPSR] & (-1 << 5);
}

int current_mode_has_spsr(registers r) {
	if (get_mode(r) == 16 || get_mode(r) == 31){
		return 0;
	}
	else{
		return 1;
	}
}

int in_a_privileged_mode(registers r) {
	if (get_mode(r) == 16){
		return 0;
	}
	else{
		return 1;
	}
}

uint32_t read_register(registers r, uint8_t reg) {
	uint8_t current_mode = get_mode(r);
	if (reg>=0 && reg<=7){
		return r->reg_no[reg];
	}
	if (reg>=8 && reg<=12){
		if(current_mode == FIQ){
			return r->reg_no[reg + FIQ_OFFSET];
		} else {
			return r->reg_no[reg];
		}
	}
	if (reg==13){
		switch(current_mode){
			case USR:
			case SYS:
				return r->reg_no[USR_SYS_SP];
			case FIQ:
				return r->reg_no[FIQ_SP];
			case IRQ:
				return r->reg_no[IRQ_SP];
			case SVC:
				return r->reg_no[SVC_SP];
			case ABT:
				return r->reg_no[ABT_SP];
			case UND:
				return r->reg_no[UND_SP];
			default:
				return -1;
		}
	}
	if (reg==14){
		switch(current_mode){
			case USR:
			case SYS:
				return r->reg_no[USR_SYS_LR];
			case FIQ:
				return r->reg_no[FIQ_LR];
			case IRQ:
				return r->reg_no[IRQ_LR];
			case SVC:
				return r->reg_no[SVC_LR];
			case ABT:
				return r->reg_no[ABT_LR];
			case UND:
				return r->reg_no[UND_LR];
			default:
				return -1;
		}
	}
	if (reg==17){
		return read_spsr(r);
	}
	if (reg==15){
		return r->reg_no[PC];
	}
	if(reg==16){
		return r->reg_no[CPSR_REG];
	}
	return 1;
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    if(reg >= 0 && reg <= 12){
		return r->reg_no[reg];
	}
	if (reg==13){
		return r->reg_no[USR_SYS_SP];
	}
	if (reg==14){
		return r->reg_no[USR_SYS_LR];
	}
	if (reg==15){
		return r->reg_no[PC];
	}
	if (reg==16){
		return r->reg_no[CPSR_REG];
	}
	return 1;
}

uint32_t read_cpsr(registers r) {
	return r->reg_no[CPSR_REG];
}

uint32_t read_spsr(registers r) {
	uint8_t current_mode = get_mode(r);
	switch(current_mode){
		case USR:
		case SYS:
			return -1;
		case FIQ:
			return r->reg_no[FIQ_SPSR];
		case IRQ:
			return r->reg_no[IRQ_SPSR];
		case SVC:
			return r->reg_no[SVC_SPSR];
		case ABT:
			return r->reg_no[ABT_SPSR];
		case UND:
			return r->reg_no[UND_SPSR];
		default:
			return -1;
	}
}

void write_register(registers r, uint8_t reg, uint32_t value) {
	uint8_t current_mode = get_mode(r);
	if (reg>=0 && reg<=7){
		r->reg_no[reg]=value;
	}
	else if (reg>=8 && reg<=12){
		if(current_mode == FIQ){
			r->reg_no[reg + FIQ_OFFSET]=value;
		} else {
			r->reg_no[reg]=value;
		}
	}
	else if (reg==13){
		switch(current_mode){
			case USR:
			case SYS:
				r->reg_no[USR_SYS_SP]=value;
			case FIQ:
				r->reg_no[FIQ_SP]=value;
			case IRQ:
				r->reg_no[IRQ_SP]=value;
			case SVC:
				r->reg_no[SVC_SP]=value;
			case ABT:
				r->reg_no[ABT_SP]=value;
			case UND:
				r->reg_no[UND_SP]=value;
			default:
				break;
		}
	}
	else if (reg==14){
		switch(current_mode){
			case USR:
			case SYS:
				r->reg_no[USR_SYS_LR]=value;
			case FIQ:
				r->reg_no[FIQ_LR]=value;
			case IRQ:
				r->reg_no[IRQ_LR]=value;
			case SVC:
				r->reg_no[SVC_LR]=value;
			case ABT:
				r->reg_no[ABT_LR]=value;
			case UND:
				r->reg_no[UND_LR]=value;
			default:
				break;
		}
	}
	else if (reg==17){
		write_spsr(r,value);
	}
	else if (reg==15){
		r->reg_no[PC]=value;
	}
	else if(reg==16){
		r->reg_no[CPSR_REG]=value;
	}
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	if(reg >= 0 && reg <= 12){
		r->reg_no[reg] = value;
	}
	else if (reg==13){
		r->reg_no[USR_SYS_SP] = value;
	}
	else if (reg==14){
		r->reg_no[USR_SYS_LR] = value;
	}
	else if (reg==15){
		r->reg_no[PC] = value;
	}
	else if (reg==16){
		r->reg_no[CPSR_REG] = value;
	}
}

void write_cpsr(registers r, uint32_t value) {
	r->reg_no[CPSR_REG] = value;
}

void write_spsr(registers r, uint32_t value) {
	uint8_t current_mode = get_mode(r);
	switch(current_mode){
		case USR:
		case SYS:
			break;
		case FIQ:
			r->reg_no[FIQ_SPSR]= value;
		case IRQ:
			r->reg_no[IRQ_SPSR]= value;
		case SVC:
			r->reg_no[SVC_SPSR]= value;
		case ABT:
			r->reg_no[ABT_SPSR]= value;
		case UND:
			r->reg_no[UND_SPSR]= value;
		default:
			break;
	}
}
