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

int and_processing(uint8_t reg1,uint8_t reg2){
   
   //Rd := Rn AND shifter_operand
   reg1=reg1 & reg2 ;

}

int sub_processing(uint8_t reg1,uint8_t reg2){
    
    //Rd := Rn - shifter_operand
    reg1=reg1 - reg2;
}

int add_processing(uint8_t reg1,uint8_t reg2){
	
	//Rd := Rn + shifter_operand
	reg1=reg1 + reg2;
}

int eor_processing(uint8_t reg1,uint8_t reg2){
	
	//Rd := Rn EOR shifter_operand

	reg1=reg1 ^ reg2;

}


int rsb_processing(uint8_t reg1,uint8_t reg2){

	//Rd := shifter_operand - Rn
	reg1=reg2 - reg1;
}

int adc_processing(uint8_t reg1,uint8_t reg2,uint8_t regflag){

	//Rd := Rn + shifter_operand + Carry Flag
	reg1=reg1 - reg2 + regflag;
}

int sbc_processing(uint8_t reg1,uint8_t reg2,uint8_t regflag){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	reg1=reg1 - reg2 - (!regflag);
}

int rsc_processing(uint8_t reg1,uint8_t reg2,uint8_t regflag){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	reg1=reg2 - reg1 - (!regflag);
}

int tst_processing(uint8_t regflag,int flag){

	//Update flags after Rn AND shifter_operand
	regflag=flag;
}

int teq_processing(uint8_t regflag,int flag){

	//Update flags after Rn EOR shifter_operand
	regflag=flag;
}

int cmp_processing(uint8_t regflag,int flag){

	//Update flags after Rn - shifter_operand
	regflag=flag;
}

int cmn_processing(uint8_t regflag,int flag){

	//Update flags after Rn + shifter_operand
	regflag=flag;
}

int orr_processing(uint8_t regflag,int flag){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
}

int mov_processing(uint8_t reg1,uint8_t reg2){
	
	//Rd := shifter_operand (no first operand)
	reg1=reg2;
}

int bic_processing(){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	reg1=reg1 & (!reg2);
}

int mvn_processing(){

	//Move Not Rd := NOT shifter_operand (no first operand)
	reg1=(!reg2);
}





