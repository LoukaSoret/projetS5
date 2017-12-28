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
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"


/*************************************************************
Auteur : Louka
Date : 22/12/17
Spec: Prends en entree le coeur arm p et l'instruction ins
renvois le code d'erreur 0 si tout c'est bien passe. modifie
l'etat des registres et de la memoire en fonction de 
l'instruction par effets de bords.
*************************************************************/
int arm_load_store(arm_core p, uint32_t ins);
int arm_load_store_multiple(arm_core p, uint32_t ins);
int arm_coprocessor_load_store(arm_core p, uint32_t ins);



/***************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Gere les cas des load et store pour signed bytes, double
words et signed et unsigned half words.
****************************************************************/
int arm_load_store_half_double(arm_core p,uint32_t ins);



/**************************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Gestion de toutes les instructions de type load.Prends en arguments
un coeur arm p, les registres Rn (adresse), Rd(destination) et Rs(shift),
le code op shift_codeOp, la valeur immediate shift_imm, et les bits 
I(immediate offset),P(addressing mode),U(+/- offset),B(byte/word access),
W(addressing mode),D(half/double word access). Renvois 0 si tout c'est bien
passe. Modifie les registres et la memoire par effets de bords.
****************************************************************************/
int arm_load(arm_core p,uint8_t Rn,uint8_t Rd,uint8_t I,uint8_t P,uint8_t U,uint8_t B,uint8_t W ,uint8_t H,uint8_t D,uint16_t immediate,uint8_t shift_amount,uint8_t shift,uint8_t Rm);



/**************************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Gestion de toutes les instructions de type store.Prends en arguments
un coeur arm p, les registres Rn (adresse), Rd(destination) et Rs(shift),
le code op shift_codeOp, la valeur immediate shift_imm, et les bits 
I(immediate offset),P(addressing mode),U(+/- offset),B(byte/word access),
W(addressing mode),D(half/double word access). Renvois 0 si tout c'est bien
passe. Modifie les registres et la memoire par effets de bords.
****************************************************************************/
int arm_store(arm_core p,uint8_t Rn,uint8_t Rd,uint8_t I,uint8_t P,uint8_t U,uint8_t B,uint8_t W ,uint8_t H,uint8_t D,uint16_t immediate,uint8_t shift_amount,uint8_t shift,uint8_t Rm);


#endif
