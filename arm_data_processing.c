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
#include "decodeur_cond_shift.h"

void and_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
   
   //Rd := Rn AND shifter_operand
	uint32_t result;
	
	result= arm_read_usr_register(p,rn) & val_sh;
           arm_write_usr_register(p,rd, result);

	if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
       tst_processing(p,rd);

	} 

}

void sub_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
    
    //Rd := Rn - shifter_operand
    uint32_t result;
    result= arm_read_usr_register(p,rn) - val_sh;
           arm_write_usr_register(p,rd, result);

        if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
        cmp_processing(arm_core p,uint8_t rd);

	} 
}

void add_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
	
	//Rd := Rn + shifter_operand
	uint32_t result;
 
    result= arm_read_usr_register(p,rn) + val_sh;
           arm_write_usr_register(p,rd, result);

            if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         cmn_processing(p,rd);

	} 
}

void eor_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
	
	//Rd := Rn EOR shifter_operand

	uint32_t result;

   result= arm_read_usr_register(p,rn) ^ val_sh;
           arm_write_usr_register(p,rd, result);

            if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         teq_processing(p,rd);

	} 

}


void rsb_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := shifter_operand - Rn

   uint32_t result;

   result= val_sh - arm_read_usr_register(p,rn);
           arm_write_usr_register(p,rd, result);

           if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 

}

void adc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := Rn + shifter_operand + Carry Flag
	uint32_t result;

	result= arm_read_usr_register(p,rn) + val_sh + read_C(p);
            arm_write_usr_register(p,rd, result);

             if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 
}

void sbc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	uint32_t result;
		
		result= arm_read_usr_register(p,rn) - val_sh - (~(read_C(p));
	            arm_write_usr_register(p,rd, result);
	
	 if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 

	


void rsc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	uint32_t result;

	result= val_sh - arm_read_usr_register(p,rn) - (~(read_C(p));
	     arm_write_usr_register(p,rd, result);

	     if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 
}

	void orr_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
	uint32_t result;

	result= arm_read_usr_register(p,rn) | val_sh;
	        arm_write_usr_register(p,rd, result);

	     if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 
	
}

void mov_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
	
	//Rd := shifter_operand (no first operand)
	
	        arm_write_usr_register(p,rd, val_sh);

	     if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 
	
}

void bic_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	uint32_t result;

	result= arm_read_usr_register(p,rn) & (~val_sh);
	        arm_write_usr_register(p,rd, result);

	     if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,result);

	} 
	
}

void mvn_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Move Not Rd := NOT shifter_operand (no first operand)

	arm_write_usr_register(p,rd, (~val_sh);

	     if(s==1)  // si les  indicateurs NZCV de cpsr doivent �tre maj
	{
         maj_ZNCV(p,arm_read_usr_register(p,rd));

	} 
}
	




void tst_processing(arm_core p,uint8_t rd){

	//Update flags after Rn AND shifter_operand

	maj_ZNCV(p,arm_read_usr_register(p,rd));

	
}

void teq_processing(arm_core p,uint8_t rd){

	//Update flags after Rn EOR shifter_operand
	maj_ZNCV(p,arm_read_usr_register(p,rd));
	
}

void cmp_processing(arm_core p,uint8_t rd){

	//Update flags after Rn - shifter_operand
	maj_ZNCV(p,arm_read_usr_register(p,rd));
	
}

void cmn_processing(arm_core p,uint8_t rd){

	//Update flags after Rn + shifter_operand
	maj_ZNCV(p,arm_read_usr_register(p,rd));
	
}


 // fonction de mise � jour des r�gistres

void maj_ZNCV(arm_core p,uint32_t value){
	
	uint32_t val;

	val=arm_read_cpsr(p);

    //indicateur N
	if(get_bit(value,31) == 1) // voir si le bit de poids fort est �gal � 1 et dans ce cas N=1
	{
		set_bit(val,31);
	} 

    //indicateur Z
	if(value == 0){

		set_bit(val,30);

	} 

	//indicateur C
	if(get_bit(value,29)){

       /* a completer*/
	} 

	//indicateur V
	if(get_bit(value,28)){

        /* a completer */
	} 

   arm_write_cpsr(p, val);
}


/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
   int cond, rn, rd, opcode, s, shift, val_shift;
    
    cond = get_bits(ins, 31, 28);
    opcode = get_bits(ins, 24, 21);
    s = get_bit(ins, 20);
    rn = get_bits(ins, 19, 16);
    rd = get_bits(ins, 15, 12);
    sh = get_bits(ins, 11, 0);
    
    val_shift = shift(p, sh);
    
    if(condition(cond)){
    	switch(opcode){
    	case 0 :
    		and_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 1 :
    		eor_processing(rn,rd, val_sh, s);
    		break;
    		
    	case 2 :
    		sub_processing(rn,rd, val_sh, s);
    		break;
    		
    	case 3 :
    		rsb_processing(rn, rd, val_sh, s);
    		break;
    	
    	case 4 :
    		add_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 5 :
    		adc_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 6 :
    		sbc_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 7 :
    		rsc_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 10 :
    		cmp_processing(rn, val_sh, s); 
    		break;
    		
    	case 11 :
    		cmn_processing(rn, val_sh, s);
    		break;
    		
    	case 12 :
    		orr_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 13 :
    		mov_processing(rd, val_sh, s);
    		break;
    		
    	case 14 :
    		bic_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 15 :
    		mvn_processing(rd, val_sh, s);
    		break;
    		
    	default:
    		break;
    }
   }
   return 0;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    int rn, rd, opcode, value, s, rotation, val_sh, cond;
    
    cond = get_bits(ins, 31, 28);
    opcode = get_bits(ins, 24, 21);
    rn = get_bits(ins, 19, 16);
    rd = get_bits(ins, 15, 12);
    rotation = get_bits(ins, 11, 8);
    value = get_bits(ins, 7, 0);
    s = get_bit(ins, 20);
    
    val_sh = ror(value, rotation);
    
    if(condition(cond)){
    	switch(opcode){
    	case 0 :
    		and_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 1 :
    		eor_processing(rn,rd, val_sh, s);
    		break;
    		
    	case 2 :
    		sub_processing(rn,rd, val_sh, s);
    		break;
    		
    	case 3 :
    		rsb_processing(rn, rd, val_sh, s);
    		break;
    	
    	case 4 :
    		add_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 5 :
    		adc_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 6 :
    		sbc_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 7 :
    		rsc_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 8 :
    		tst(rn, val_sh, s);
    		break;
    		
    	case 9 :
    		teq(rn, val_sh, s);
    		break;
    		
    	case 10 :
    		cmp_processing(rn, val_sh, s);
    		break;
    		
    	case 11 :
    		cmn_processing(rn, val_sh, s);
    		break;
    		
    	case 12 :
    		orr_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 13 :
    		mov_processing(rd, val_sh, s);
    		break;
    		
    	case 14 :
    		bic_processing(rn, rd, val_sh, s);
    		break;
    		
    	case 15 :
    		mvn_processing(rd, val_sh, s);
    		break;
    		
    	default:
    		break;
    }
   }
   return 0;
}


