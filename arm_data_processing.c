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

void and_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){
   
   //Rd := Rn AND shifter_operand
	uint32_t result;
	
	result= arm_read_usr_register(p,rn) & val_sh;
           arm_write_usr_register(p,rd, result);

}

void sub_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){
    
    //Rd := Rn - shifter_operand
    uint32_t result;
    result= arm_read_usr_register(p,rn) - val_sh;
           arm_write_usr_register(p,rd, result);
}

void add_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){
	
	//Rd := Rn + shifter_operand
	uint32_t result;
 
    result= arm_read_usr_register(p,rn) + val_sh;
           arm_write_usr_register(p,rd, result);

}

void eor_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){
	
	//Rd := Rn EOR shifter_operand

	uint32_t result;

   result= arm_read_usr_register(p,rn) ^ val_sh;
           arm_write_usr_register(p,rd, result);

}


void rsb_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){

	//Rd := shifter_operand - Rn

   uint32_t result;

   result= val_sh - arm_read_usr_register(p,rn);
           arm_write_usr_register(p,rd, result);

}

void adc_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){

	//Rd := Rn + shifter_operand + Carry Flag
	uint32_t result;

	result= arm_read_usr_register(p,rn) + val_sh + get_bit(arm_read_cpsr(p),29);
            arm_write_usr_register(p,rd, result);

}

void sbc_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	uint32_t result;
		
		result= arm_read_usr_register(p,rn) - val_sh - (~(get_bit(arm_read_cpsr(p),29)));
	            arm_write_usr_register(p,rd, result);
	}

	


void rsc_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	uint32_t result;

	result= val_sh - arm_read_usr_register(p,rn) - (~(get_bit(arm_read_cpsr(p),29)));
	     arm_write_usr_register(p,rd, result);
}

	void orr_processing(arm_core p,uint8_t rn,uint8_t rd,int val_sh){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
	uint32_t result;

	result= arm_read_usr_register(p,rn) | val_sh;
	        arm_write_usr_register(p,rd, result);
	
}

void mov_processing(arm_core p,uint8_t rd,int val_sh){
	
	//Rd := shifter_operand (no first operand)
	
	        arm_write_usr_register(p,rd, val_sh);
	
}

void bic_processing(arm_core p,uint8_t rn,uint8_t rd, int val_sh){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	uint32_t result;

	result= arm_read_usr_register(p,rn) & (~val_sh);
	        arm_write_usr_register(p,rd, result);

	
}

void mvn_processing(arm_core p,uint8_t rd, int val_sh){

	//Move Not Rd := NOT shifter_operand (no first operand)

	arm_write_usr_register(p,rd, (~val_sh));

}
	




void tst_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh){

	//Update flags after Rn AND shifter_operand
	uint32_t resultat,operande_1;

	and_processing(p,rd,rn,val_sh);

	resultat=arm_read_usr_register(p,rd);
	operande_1=arm_read_usr_register(p,rn);

	//indicateur Z
	if(resultat == 0){
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<30)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<30))));
	
	//indicateur N
	if (get_bit(resultat,31) == 1)
	{
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<31)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<31))));
    

	//indicateur V
	if(operande_1 !=0 && (resultat/operande_1) != val_sh){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // dans un AND C=0;
    arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));
	
}

void teq_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh){

	//Update flags after Rn EOR shifter_operand

	uint32_t resultat;

	eor_processing(p,rd,rn,val_sh);

	resultat=arm_read_usr_register(p,rd);

	//indicateur Z
	if(resultat == 0){
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<30)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<30))));
	
	//indicateur N
	if (get_bit(resultat,31) == 1)
	{
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<31)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<31))));
    

	//indicateur V
	//V=0 dans un xor
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C;
    //C=0 dans un xor
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));
	
}

void cmp_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh){

	//Update flags after Rn - shifter_operand
	

	uint32_t resultat,operande_1;

	sub_processing(p,rd,rn,val_sh);

	resultat=arm_read_usr_register(p,rd);
	operande_1=arm_read_usr_register(p,rn);

	//indicateur Z
	if(resultat == 0){
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<30)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<30))));
	
	//indicateur N
	if (get_bit(resultat,31) == 1)
	{
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<31)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<31))));
    

	//indicateur V
	if(get_bit(operande_1,31) == get_bit(val_sh,31) && get_bit(operande_1,31) != get_bit(resultat,31)){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C;
    /* a completer */
	
}

void cmn_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh){

	//Update flags after Rn + shifter_operand

	uint32_t resultat,operande_1;

	add_processing(p,rd,rn,val_sh);

	resultat=arm_read_usr_register(p,rd);
	operande_1=arm_read_usr_register(p,rn);

	//indicateur Z
	if(resultat == 0){
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<30)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<30))));
	
	//indicateur N
	if (get_bit(resultat,31) == 1)
	{
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<31)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<31))));
    

	//indicateur V
	if(get_bit(operande_1,31) == get_bit(val_sh,31) && get_bit(operande_1,31) != get_bit(resultat,31)){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C
    /* a completer */
	
}

/*****************************************************/

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
   int cond, rn, rd, opcode, val_sh;
   int s, rm;
    
    cond = get_bits(ins, 31, 28);
    opcode = get_bits(ins, 24, 21);
    //s = get_bit(ins, 20);
    rn = get_bits(ins, 19, 16);
    rd = get_bits(ins, 15, 12);
        
    //shift_operands
    //shift(p,shift_codeOp,shift_imm,Rm,0,0)
    s = get_bits(ins,6,5);
    rm = get_bits(ins,3,0);
    
    val_sh = shift(p, s, 0, rm, 0, 1);
    
    if(condition(p, cond)){
    	switch(opcode){
    	case 0 :
    		and_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 1 :
    		eor_processing(p, rn,rd, val_sh);
    		break;
    		
    	case 2 :
    		sub_processing(p, rn,rd, val_sh);
    		break;
    		
    	case 3 :
    		rsb_processing(p, rn, rd, val_sh);
    		break;
    	
    	case 4 :
    		add_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 5 :
    		adc_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 6 :
    		sbc_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 7 :
    		rsc_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 10 :
    		cmp_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 11 :
    		cmn_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 12 :
    		orr_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 13 :
    		mov_processing(p, rd, val_sh);
    		break;
    		
    	case 14 :
    		bic_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 15 :
    		mvn_processing(p, rd, val_sh);
    		break;
    		
    	default:
    		break;
    }
   }
   return 0;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    int rn, rd, opcode, val_sh, cond;
    int shift_imm, s, rm;
    
    cond = get_bits(ins, 31, 28);
    opcode = get_bits(ins, 24, 21);
    rn = get_bits(ins, 19, 16);
    rd = get_bits(ins, 15, 12);
    //rotation = get_bits(ins, 11, 8);
    //value = get_bits(ins, 7, 0);
    //s = get_bit(ins, 20);
    
    //shift_operands
    //shift(p,shift_codeOp,shift_imm,Rm,0,0)
    shift_imm = get_bits(ins, 11, 0);
    s = get_bits(ins,6,5);
    rm = get_bits(ins,3,0);
    
    
    val_sh = shift(p, s, shift_imm, rm, 0, 0);
    
    if(condition(p, cond)){
    	switch(opcode){
    	case 0 :
    		and_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 1 :
    		eor_processing(p, rn,rd, val_sh);
    		break;
    		
    	case 2 :
    		sub_processing(p, rn,rd, val_sh);
    		break;
    		
    	case 3 :
    		rsb_processing(p, rn, rd, val_sh);
    		break;
    	
    	case 4 :
    		add_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 5 :
    		adc_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 6 :
    		sbc_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 7 :
    		rsc_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 8 :
    		tst_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 9 :
    		teq_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 10 :
    		cmp_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 11 :
    		cmn_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 12 :
    		orr_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 13 :
    		mov_processing(p, rd, val_sh);
    		break;
    		
    	case 14 :
    		bic_processing(p, rn, rd, val_sh);
    		break;
    		
    	case 15 :
    		mvn_processing(p, rd, val_sh);
    		break;
    		
    	default:
    		break;
    }
   }
   return 0;
}

/*AUTHOR : Diallo Amadou
DATE : 21/12/2017
Sp�cifications : instructions de traitement de donn�es*/

void maj_ZN(arm_core p,uint32_t resultat){

	//indicateur Z
	if(resultat == 0){
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<30)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<30))));
	
	//indicateur N
	if (get_bit(resultat,31) == 1)
	{
		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<31)));
	}
	else
		arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<31))));
}




void tst_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn AND shifter_operand
	uint32_t resultat,operande_1;
	
	operande_1=arm_read_usr_register(p,rn);
    resultat=operande_1 & val_sh;
	

	maj_ZN(p,resultat);

	//indicateur V
	if(operande_1 !=0 && (resultat/operande_1) != val_sh){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // dans un AND C=0;
    arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));
	
}

void teq_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn EOR shifter_operand

	uint32_t resultat,operande_1;

	operande_1=arm_read_usr_register(p,rn);

	resultat=operande_1 ^ val_sh;

	maj_ZN(p,resultat);

	//indicateur V
	//V=0 dans un xor
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C;
    //C=0 dans un xor
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));
	
}

void cmp_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn - shifter_operand
	

	uint32_t resultat,operande_1;
	operande_1=arm_read_usr_register(p,rn);

	resultat=operande_1 - val_sh;
	

	maj_ZN(p,resultat);
    

	//indicateur V
	if(get_bit(operande_1,31) == get_bit(val_sh,31) && get_bit(operande_1,31) != get_bit(resultat,31)){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C;
    
    if(operande_1!=resultat+val_sh)
    	arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<29)));
      else
      	arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

	
}

void cmn_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn + shifter_operand

	uint32_t resultat,operande_1;

	operande_1=arm_read_usr_register(p,rn);

	resultat=operande_1 + val_sh;
	
    maj_ZN(p,resultat);

	//indicateur V
	if(get_bit(operande_1,31) == get_bit(val_sh,31) && get_bit(operande_1,31) != get_bit(resultat,31)){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C
    
    if(operande_1!=resultat-val_sh)
    	arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<29)));
      else
      	arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
	
}


void and_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
   
   //Rd := Rn AND shifter_operand
	uint32_t resultat;
	
	resultat= arm_read_usr_register(p,rn) & val_sh;
           arm_write_usr_register(p,rd, resultat);

           if(s==1){
           	  tst_processing(p,rn,val_sh);
           }

}

void sub_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
    
    //Rd := Rn - shifter_operand
    uint32_t resultat;
    resultat= arm_read_usr_register(p,rn) - val_sh;
           arm_write_usr_register(p,rd, resultat);
           if(resultat && s==1){
           	 cmp_processing(p,rn,val_sh);
           }
}

void add_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
	
	//Rd := Rn + shifter_operand
	uint32_t resultat;
 
    resultat= arm_read_usr_register(p,rn) + val_sh;
           arm_write_usr_register(p,rd, resultat);
           if(resultat && s==1){
           	cmn_processing(p,rn,val_sh);
           }

}

void eor_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
	
	//Rd := Rn EOR shifter_operand

	uint32_t resultat;

   resultat= arm_read_usr_register(p,rn) ^ val_sh;
           arm_write_usr_register(p,rd, resultat);
           if(resultat && s==1){
           	teq_processing(p,rn,val_sh);

           }

}

void rsb_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := shifter_operand - Rn

   uint32_t resultat,valrn;

   valrn=arm_read_usr_register(p,rn);
   resultat= val_sh - valrn;
           arm_write_usr_register(p,rd, resultat);
           
           if(resultat && s==1){
           	   
           	   //mis a jour Z et N
           	   maj_ZN(p,resultat);

           	    //indicateur C

           	    if(val_sh!=resultat+valrn)
    	             arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<29)));
                 else
      	             arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
      	         
      	         //indicateur V
	                  if(get_bit(val_sh,31) == get_bit(valrn,31) && get_bit(val_sh,31) != get_bit(resultat,31)){

		                   arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	                       }
	                      else
                               arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
           	 
                    }

}

void adc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := Rn + shifter_operand + Carry Flag
	uint32_t resultat;

	resultat= arm_read_usr_register(p,rn) + val_sh + get_bit(arm_read_cpsr(p),29);
            arm_write_usr_register(p,rd, resultat);
            if(resultat && s==1){
            	cmn_processing(p,rn,(val_sh+((get_bit(arm_read_cpsr(p),29)))));
            }

}

void sbc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	uint32_t resultat;
		
		resultat= arm_read_usr_register(p,rn) - val_sh - (~(get_bit(arm_read_cpsr(p),29)));
	            arm_write_usr_register(p,rd, resultat);
	            if(resultat && s==1){
	            	cmp_processing(p,rn,(val_sh-(~(get_bit(arm_read_cpsr(p),29)))));
	            }
	}

	


void rsc_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	uint32_t resultat,valrn;
	valrn=arm_read_usr_register(p,rn);

	resultat= val_sh - valrn - (~(get_bit(arm_read_cpsr(p),29)));
	     arm_write_usr_register(p,rd, resultat);
	     if(resultat && s==1){
	     	 maj_ZN(p,resultat);

	     	 //indicateur C

           	    if(val_sh!=resultat+valrn+(~(get_bit(arm_read_cpsr(p),29))))
    	             arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<29)));
                 else
      	             arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
      	         
      	         //indicateur V
	                  if(get_bit(val_sh,31) == get_bit(valrn,31) && get_bit(val_sh,31)== (~(get_bit(arm_read_cpsr(p),29))) && get_bit(val_sh,31) != get_bit(resultat,31)){

		                   arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	                       }
	                      else
                               arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));
	     }
}

	void orr_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
	uint32_t resultat;

	resultat= arm_read_usr_register(p,rn) | val_sh;
	          arm_write_usr_register(p,rd, resultat);
	        
	        if(resultat && s==1){
	        	
	        	maj_ZN(p,resultat);
             //The V flag and the rest of the CPSR are unaffected.

             }
}

void mov_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){
	
	//Rd := shifter_operand (no first operand)
	uint32_t resultat;
	
	        arm_write_usr_register(p,rd, val_sh);
	        resultat=arm_read_usr_register(p,rd);

	        //si le registre destination est en mode user
	        if(resultat && s==1){
	        	
	        	maj_ZN(p,resultat);
    
             //The V flag and the rest of the CPSR are unaffected.
            }	
}

void bic_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	uint32_t resultat;

	resultat= arm_read_usr_register(p,rn) & (~val_sh);
	          arm_write_usr_register(p,rd, resultat);

	          
	        if(resultat && s==1){
	        	
	        	tst_processing(p,rn,(~val_sh));
	        }
	
}

void mvn_processing(arm_core p,uint8_t rd,uint8_t rn,int val_sh,int s){

	//Move Not Rd := NOT shifter_operand (no first operand)

	arm_write_usr_register(p,rd, (~val_sh));
	uint32_t resultat=arm_read_usr_register(p,rd);

	//si le registre destination est en mode user
	        if(resultat && s==1){
	        	maj_ZN(p,resultat);
    
             //The V flag and the rest of the CPSR are unaffected.

             }
}
