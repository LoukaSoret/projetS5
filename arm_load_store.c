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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"

int arm_load_store(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
	uint32_t *adr = NULL;
	int L = get_bit(ins,20);
	int U = get_bit(ins,23);
	int P = get_bit(ins,24);
	int W = get_bit(ins,21);
	int nbreg = 0;
    if(L){ //Cas du load
    	arm_read_word(p,arm_read_register(p,get_bits(ins,19,16)),adr);
    	if(U){ // U == 1 , on remonte les adresses
    		if(P){ // P == 1, la premiere valeur est en dehors de la range
    			adr--;
    		}
	    	for(uint8_t i=15;i>=0;i--){
	    		if(get_bit(ins,i)){
	    			arm_write_register(p,i,*adr);
	    			adr--;
	    			nbreg++;
	    		}
	    	}
	    	if(W){ // W == 1, on incremente Rn
    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))-(4*nbreg));
    		}
    	}else{
    		if(P){ // P == 1, la premiere valeur est en dehors de la range
    			adr++;
    		}
	    	for(uint8_t i=15;i>=0;i--){
	    		if(get_bit(ins,i)){
	    			arm_write_register(p,i,*adr);
	    			adr++;
	    			nbreg++;
	    		}
	    	}
	    	if(W){ // W == 1, on incremente Rn
    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))+(4*nbreg));
    		}
    	}
    }else{ // Cas du store
    	arm_read_word(p,arm_read_register(p,get_bits(ins,19,16)),adr);
    	if(U){ // U == 1 , on remonte les adresses
    		if(P){ // P == 1, la premiere valeur est en dehors de la range
    			adr--;
    		}
	    	for(uint8_t i=15;i>=0;i--){
	    		if(get_bit(ins,i)){
	    			*adr = arm_read_register(p,i);
	    			adr--;
	    			nbreg++;
	    		}
	    	}
	    	if(W){ // W == 1, on incremente Rn
    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))-(4*nbreg));
    		}
    	}else{
    		if(P){ // P == 1, la premiere valeur est en dehors de la range
    			adr++;
    		}
	    	for(uint8_t i=15;i>=0;i--){
	    		if(get_bit(ins,i)){
	    			*adr = arm_read_register(p,i);
	    			adr++;
	    			nbreg++;
	    		}
	    	}
	    	if(W){ // W == 1, on incremente Rn
    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))+(4*nbreg));
    		}
    	}
    }

    return 0;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
