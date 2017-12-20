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



uint16_t imm;                        //s�quence contenant rotationimmediate
uint8_t rotate=get_bits(imm,11,8);   //valeur de la rotation
uint8_t immediate=get_bits(imm,7,0); //valeur de immediate



void and(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){
   
   //Rd := Rn AND shifter_operand
	uint32_t result;
	
	result= arm_read_usr_register(p,rn) & (immed << rotate);
           arm_write_usr_register(p,rd, result);

	if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
       //  arm_write_cpsr(p, value)

	} 

}

void sub(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){
    
    //Rd := Rn - shifter_operand
    uint32_t result;
    result= arm_read_usr_register(p,rn) - (immed << rotate);
           arm_write_usr_register(p,rd, result);

        if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
        // arm_write_cpsr(p, value)

	} 
}

void add(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){
	
	//Rd := Rn + shifter_operand
	uint32_t result;
 
    result= arm_read_usr_register(p,rn) + (immed << rotate);
           arm_write_usr_register(p,rd, result);

            if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         //arm_write_cpsr(p, value)

	} 
}

void eor(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){
	
	//Rd := Rn EOR shifter_operand

	uint32_t result;

   result= arm_read_usr_register(p,rn) ^ (immed << rotate);
           arm_write_usr_register(p,rd, result);

            if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         //arm_write_cpsr(p, value)

	} 

}


void rsb(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Rd := shifter_operand - Rn

   uint32_t result;

   result= (immed << rotate) - arm_read_usr_register(p,rn);
           arm_write_usr_register(p,rd, result);

           if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         //arm_write_cpsr(p, value)

	} 

}

void adc(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Rd := Rn + shifter_operand + Carry Flag
	uint32_t result;

	result= arm_read_usr_register(p,rn) + (immed << rotate) + read_C(p);
            arm_write_usr_register(p,rd, result);

             if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         //arm_write_cpsr(p, value)

	} 
}

void sbc(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	uint32_t result;
		
		result= arm_read_usr_register(p,rn) - (immed << rotate) - (~(read_C(p));
	            arm_write_usr_register(p,rd, result);
	
	 if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         //arm_write_cpsr(p, value)

	} 

	
}

void rsc(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	uint32_t result;

	result= (immed << rotate) - arm_read_usr_register(p,rn) - (~(read_C(p));
	     arm_write_usr_register(p,rd, result);

	     if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         //arm_write_cpsr(p, value)

	} 
	
}

void tst(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Update flags after Rn AND shifter_operand
	
}

void teq(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Update flags after Rn EOR shifter_operand
	
}

void cmp(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Update flags after Rn - shifter_operand
	
}

void cmn(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Update flags after Rn + shifter_operand
	
}

void orr(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
	uint32_t result;

	result= arm_read_usr_register(p,rn) | (immed << rotate);
	        arm_write_usr_register(p,rd, result);
	
}

void mov(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){
	
	//Rd := shifter_operand (no first operand)
	
	        arm_write_usr_register(p,rd, (immed << rotate));
	
}

void bic(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	uint32_t result;

	result= arm_read_usr_register(p,rn) & (~(immed << rotate));
	        arm_write_usr_register(p,rd, result);
	
}

void mvn(arm_core p,uint8_t rd,uint8_t rn,uint8_t rotate,uint8_t immed,int s){

	//Move Not Rd := NOT shifter_operand (no first operand)

	arm_write_usr_register(p,rd, (~(immed << rotate));
	
}

int maj_ZNCV(uint32_t value){

	if(get_N)
}





