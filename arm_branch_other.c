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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "decodeur_cond_shift.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>

/*AUTHOR : Nikolnikov Kirill
DATE : 21/12/2017
Sp�cifications : Fonction B/BL effectue un branchement vers l'adresse sp�cifi� dans l'instruction ins
Avec la sauvegarde du PC is le bit L est a 1*/


int arm_branch(arm_core p, uint32_t ins) {
    if(condition(p,get_bits(ins,31,28))){ // Si la condition est bonne on �xecute
        int32_t adr = get_bits(ins,23,0);
        if(get_bit(adr,23)){ // Si le bit du poid fort est a 1 alors on extend l'adresse a 30 bits avec des 1
            adr = adr | 0xFF0000000;
        }
        adr = adr<<2; // On decale de deux bit a gauche pour multiplier la valeur par 4
        int L = get_bit(ins,24); 
        if(L){ // Si L est == 1 alors on sauvegarde la valuer de PC dans R14
            arm_write_register(p,14,arm_read_register(p,15));
            arm_write_register(p,15,arm_read_register(p,15)+adr);
        }else{
            arm_write_register(p,15,arm_read_register(p,15)+adr);
        }
    }
    return 0;
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins) {
    if (get_bit(ins, 24)) {
        /* Here we implement the end of the simulation as swi 0x123456 */
        if ((ins & 0xFFFFFF) == 0x123456)
            exit(0);
        return SOFTWARE_INTERRUPT;
    } 
    return UNDEFINED_INSTRUCTION;
}

/*AUTHOR : Nikolnikov Kirill
DATE : 22/12/2017 10:46
Sp�cifications : Fonction MRS copie le registre spsr/cpsr dans un registre g�neral
Pour que l'utilisateur puisse le consulter.*/

int arm_miscellaneous(arm_core p, uint32_t ins) {
    if(condition(p,get_bits(ins,31,28))){ // Si la condition est bonne on �xecute
        int reg = get_bits(ins,15,12);
        if( reg == 15){ // Si le registre destination est le 15 alors le resultat est UNPREDICTABLE
            return -1;
        }else{
            if(get_bit(ins,22)){ // Si le bit R est a 1 alors on load le registre spsr
                arm_write_register(p,reg,arm_read_spsr(p));
            }else{ // On load le registre cpsr sinon
                arm_write_register(p,reg,arm_read_cpsr(p));
            }
        }
        return 0;
    }
    return 0;
}
