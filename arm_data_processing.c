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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    int r1, r2, opcode;
    
    opcode = get_bits(ins, 24, 21);
    r1 = get_bits(ins, 19, 16);
    r2 = get_bits(ins, 15, 12);
    
    switch(opcode){
    	case 0 :
    		and(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 1 :
    		eor(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 2 :
    		sub(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 3 :
    		rsb(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 4 :
    		add(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 5 :
    		adc(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 6 :
    		sbc(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 7 :
    		rsc(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 8 :
    		tst(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 9 :
    		teq(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 10 :
    		cmp(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 11 :
    		cmn(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 12 :
    		orr(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 13 :
    		mov(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 14 :
    		bic(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	case 15 :
    		mvn(p->reg->reg_no[r1], p->reg->reg_no[r2]);
    		break;
    		
    	default:
    		break;
    }
}
