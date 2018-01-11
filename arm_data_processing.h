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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"

void and_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void sub_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void add_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void eor_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void rsb_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void adc_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void sbc_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void rsc_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void orr_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s);
void mov_processing(arm_core p,uint8_t rd,uint32_t val_sh, int s);
void bic_processing(arm_core p,uint8_t rn,uint8_t rd, uint32_t val_sh, int s);
void mvn_processing(arm_core p,uint8_t rd,uint32_t val_sh, int s);
void tst_processing(arm_core p,uint8_t rn,uint32_t val_sh);
void teq_processing(arm_core p,uint8_t rn,uint32_t val_sh);
void cmp_processing(arm_core p,uint8_t rn,uint32_t val_sh);
void cmn_processing(arm_core p,uint8_t rn,uint32_t val_sh);
void maj_ZNCV(arm_core p,uint32_t value);

/*************************************************************************
Auteur : Bianca
Date : 19/12/2017
Spec : Prends en argument l'instruction en 32 bits. Cette fonction traite 
	les instructions sans les valeurs immediate, donc avec que les registres.
	La fonction decoupe l'instruction et appelle les fonctions necessaire 
	par rapport a son op code. Les fonctions sont appellées que si la 
	condition est satisfiée.
**************************************************************************/
int arm_data_processing_shift(arm_core p, uint32_t ins);

/*************************************************************************
Auteur : Bianca
Date : 19/12/2017
Spec : Prends en argument l'instruction en 32 bits. Cette fonction traite 
	les instructions avec les valeurs immediate. La fonction decoupe 
	l'instruction et appelle les fonctions necessaire par rapport a son op 
	code. Les fonctions sont appellées que si la condition est satisfiée.
**************************************************************************/
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);

#endif
