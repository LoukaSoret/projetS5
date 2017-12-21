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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

static int arm_execute_instruction(arm_core p) {
    uint32_t instruction,error;
    error = arm_fetch(p, &instruction);
    if(error){
    	printf("Erreur lors du fetch en memoire de l'istruction courante.\n");
    	return 1;
    }
    else{
    	switch(get_bits(instruction, 27, 25))
    	{
    		case 0b000:
    			switch((get_bits(instruction,24,23)<<3) | (get_bit(instruction,7))<<2 | (get_bit(instruction,4))<<1 | (get_bit(instruction,20)) ){
    				case 0b10010:
                    case 0b10000:
                    case 0b10100:
    					arm_miscellaneous(p, instruction);
    					break;
    				default:
    					if (get_bit(instruction,4) && get_bit(instruction,7)){
    						if(get_bits(instruction,6,5)){
                                /* Extra load/stores for signed bytes, halfwords and doublewords */
                                arm_load_store(p, instruction);
                            }
                            else{
                                /* Multiplies */
                            }
    					}
    					else{
    						/* Data processing register/immediate shift */
    						arm_data_processing_shift(p, instruction);
    					}
    					break;
    			}
    			break;
    		case 0b001:
    			switch(get_bits(instruction,24,20))
    			{
    				case 0b10010:
    				case 0b10110:
    					/* Move immediate to status register */
    					break;
    				case 0b10000:
    				case 0b10100:
    					/* Undefined instruction  */
    					break;
    				default:
    					/* Data processing immediate */
    					arm_data_processing_immediate_msr(p,instruction);
    					break;
    			}
    			break;
    		case 0b010:
    			/* Load/store immediate offset */
    			arm_load_store(p, instruction);
    			break;
    		case 0b011:
    			if (get_bit(instruction,4)){
    				/* Media instructions */
    				break;
    			}
    			else{
    				/* Load/store register offset */
    				arm_load_store(p, instruction);
    				break;
    			}
    			break;
    		case 0b100:
    			/* Load/store multiple */
    			arm_load_store_multiple(p, instruction);
    		case 0b101:
    			/* Branch and branch with link */
    			arm_branch(p, instruction);
    		case 0b110:
    			/* Coprocessor load/store and double register transfers */
    			arm_coprocessor_load_store(p, instruction);
    		case 0b111:
    			/* Coprocessor data/registers processing and Software interrupt */
    			arm_coprocessor_others_swi(p, instruction);
    		default:
    			break;
    	}
    }
    return 0;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result)
        arm_exception(p, result);
    return result;
}
