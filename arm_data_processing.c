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
    return UNDEFINED_INSTRUCTION;
}

// Instructions de traitement de données

void and_processing(arm_core p,uint8_t reg1,uint8_t reg2){
   
   //Rd := Rn AND shifter_operand
   uint32_t result;

   result= arm_read_usr_register(p,reg1) & arm_read_usr_register(p,reg2);
           arm_write_usr_register(p,reg1, result);



}

void sub_processing(arm_core p,uint8_t reg1,uint8_t reg2){
    
    //Rd := Rn - shifter_operand

   uint32_t result;

   result= arm_read_usr_register(p,reg1) - arm_read_usr_register(p,reg2);
           arm_write_usr_register(p,reg1, result);
}

void add_processing(arm_core p,uint8_t reg1,uint8_t reg2){
	
	//Rd := Rn + shifter_operand
	
   uint32_t result;

   result= arm_read_usr_register(p,reg1) + arm_read_usr_register(p,reg2);
           arm_write_usr_register(p,reg1, result);
}

void eor_processing(arm_core p,uint8_t reg1,uint8_t reg2){
	
	//Rd := Rn EOR shifter_operand

	uint32_t result;

   result= arm_read_usr_register(p,reg1) ^ arm_read_usr_register(p,reg2);
           arm_write_usr_register(p,reg1, result);

}


void rsb_processing(arm_core p,uint8_t reg1,uint8_t reg2){

	//Rd := shifter_operand - Rn

   uint32_t result;

   result= arm_read_usr_register(p,reg2) - arm_read_usr_register(p,reg1);
   arm_write_usr_register(p,reg1, result);

}

void adc_processing(arm_core p,uint8_t reg1,uint8_t reg2,uint8_t regflag){

	//Rd := Rn + shifter_operand + Carry Flag
	add_processing(p,reg1,reg2);
	add_processing(p,reg1,regflag);
}

void sbc_processing(arm_core p,uint8_t reg1,uint8_t reg2,uint8_t regflag){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	uint32_t result;
	sub_processing(p,reg1,reg2);

	result= arm_read_usr_register(p,reg1) - !(arm_read_cpsr(p));
	arm_write_usr_register(p,reg1, result);
	

	
}

void rsc_processing(uint8_t reg1,uint8_t reg2,uint8_t regflag){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	uint32_t result;
	rsb_processing(p,reg1,reg2);
	result= arm_read_usr_register(p,reg1) - !(arm_read_cpsr(p));
	arm_write_usr_register(p,reg1, result);
}

void tst_processing(uint8_t regflag,int flag){

	//Update flags after Rn AND shifter_operand
	
}

void teq_processing(uint8_t regflag,int flag){

	//Update flags after Rn EOR shifter_operand
	
}

void cmp_processing(uint8_t regflag,int flag){

	//Update flags after Rn - shifter_operand
	
}

void cmn_processing(uint8_t regflag,int flag){

	//Update flags after Rn + shifter_operand
	
}

void orr_processing(uint8_t regflag,int flag){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
}

void mov_processing(uint8_t reg1,uint8_t reg2){
	
	//Rd := shifter_operand (no first operand)
	
}

void bic_processing(){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	
}

void mvn_processing(){

	//Move Not Rd := NOT shifter_operand (no first operand)
	
}





