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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "decodeur_cond_shift.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>

/*************************************************************
Auteur : Kirill
Spec: Prends en entree le coeur arm p et l'instruction ins.
Effectue un branchement avec ou sans la sauvegarde du PC.
*************************************************************/


int arm_branch(arm_core p, uint32_t ins) {
    printf("Debut de l'instruction Branch\n");
    if(condition(p,get_bits(ins,31,28))){ // Si la condition est bonne on éxecute
        printf("Condition Passé\n");
        int32_t adr = get_bits(ins,23,0);
        int32_t pc = arm_read_register(p,15);
        printf("Valeur de l'adresse initiale : %0x\n",adr );
        int neg = get_bit(adr,23);
        if(neg){ // Si le bit du poid fort est a 1 alors on extend l'adresse a 30 bits avec des 1
            adr = 0x3F000000 | adr;
        }else{
            adr = 0x00000000 | adr;
        }
        printf("Valeur de l'adresse apres l'extension : %0x \n",adr );
        adr = adr<<2; // On decale de deux bit a gauche pour multiplier la valeur par 4
        printf("Valeur de l'adresse apres le decalage a gauche de 2 : %0x \n",adr );
        adr = ~adr;
        printf("Valeur de complement a 1 : %0x \n",adr );
        adr = adr + 1;
        printf("Valeur de complement a 2 : %0x \n",adr );
        int L = get_bit(ins,24); 
        if(L){ // Si L est == 1 alors on sauvegarde la valuer de PC dans R14
            arm_write_register(p,14,pc-4);
        }
        if(neg){
            arm_write_register(p,15,pc-adr);
        }else{
            arm_write_register(p,15,pc+adr);
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
Spécifications : Fonction MRS copie le registre spsr/cpsr dans un registre géneral
Pour que l'utilisateur puisse le consulter.*/

int arm_miscellaneous(arm_core p, uint32_t ins) {
    if(condition(p,get_bits(ins,31,28))){ // Si la condition est bonne on éxecute
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
