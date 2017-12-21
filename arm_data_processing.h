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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"

void and_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void sub_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void add_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void eor_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void rsb_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void adc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void sbc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void rsc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void orr_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void mov_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void bic_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void mvn_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s);
void tst_processing(arm_core p,uint8_t rd);
void teq_processing(arm_core p,uint8_t rd);
void cmp_processing(arm_core p,uint8_t rd);
void cmn_processing(arm_core p,uint8_t rd);
void maj_ZNCV(arm_core p,uint32_t value);

int arm_data_processing_shift(arm_core p, uint32_t ins);
int arm_data_processing_immediate_msr(arm_core p, uint32_t ins);

#endif
