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
#include "decodeur_cond_shift.h"

/*************************************************************************
Auteur : Amadou 
Date : 19/12/2017
Spec : La fonction fait le mis a jour des flags Z et N par rapport au 
	resultat donne en argument.  
**************************************************************************/
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


/*************************************************************************
Auteur : Amadou 
Date : 19/12/2017
Spec : Les fonctions prennent en argument les registres (soit rn, soit rd, 
	soit les deux), la valeur pre-traite par shift, et le bit s dans les cas 
	ou les fonctions peuvent modifier les cpsr. Les fonctions effecutent les 
	instructions de ARM.  
**************************************************************************/

/*************************************************************************
Auteur : Bianca 
Date : 08/01/2018
Spec : Les fonctions prennent en argument les registres (soit rn, soit rd, 
	soit les deux), la valeur pre-traite par shift, et le bit s dans les cas 
	ou les fonctions peuvent modifier les cpsr. Les fonctions effecutent les 
	instructions de ARM.  
**************************************************************************/

void mvn_processing(arm_core p,uint8_t rd, uint32_t val_sh,  int s){

	//Move Not Rd := NOT shifter_operand (no first operand)

	arm_write_register(p,rd, (~val_sh));
	uint32_t resultat=arm_read_register(p,rd);

	//si le registre destination est en mode user
	        if(s){
	        	maj_ZN(p,resultat);
    
             //The V flag and the rest of the CPSR are unaffected.

		}
}
	




void tst_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn AND shifter_operand
	uint32_t resultat,operande_1;
	
	operande_1=arm_read_register(p,rn);
    resultat=operande_1 & val_sh;
	

	maj_ZN(p,resultat);

    // dans un AND C=0;
	arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<29))));
	
}

void teq_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn EOR shifter_operand

	uint32_t resultat,operande_1;

	operande_1=arm_read_register(p,rn);

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
	
	int c = read_C(p);
	uint32_t resultat,operande_1;
	operande_1=arm_read_register(p,rn);

	resultat=operande_1 - val_sh;
	

	maj_ZN(p,resultat);
    

	//indicateur V
	if(get_bit(operande_1,31) == get_bit(val_sh,31) && get_bit(operande_1,31) != get_bit(resultat,31)){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C;
    
   /* if(operande_1!=resultat+val_sh)
    	arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<29)));
      else
      	arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));*/
      	
      if( (0xffffffff - operande_1) < ~val_sh + 0x1 && !c){
    	printf("C updated\n");
    	arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 29));
    	}
    if( (0xffffffff - operande_1) >= ~val_sh + 0x1 && c) {
    	printf("C updated\n");
    	arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 29));
    }
}

void cmn_processing(arm_core p,uint8_t rn,uint32_t val_sh){

	//Update flags after Rn + shifter_operand

	uint32_t resultat,operande_1;
	int c = read_C(p);

	operande_1=arm_read_register(p,rn);
	
    maj_ZN(p,resultat);

	//indicateur V
	if(get_bit(operande_1,31) == get_bit(val_sh,31) && get_bit(operande_1,31) != get_bit(resultat,31)){

		arm_write_cpsr(p, (arm_read_cpsr(p) | (1<<28)));
	}
	else
        arm_write_cpsr(p, (arm_read_cpsr(p) & (~(1<<28))));

    // Indicateur C
    
    if( (0xffffffff - operande_1) < val_sh && !c){
    	printf("C updated\n");
    	arm_write_cpsr(p, set_bit(arm_read_cpsr(p), 29));
    	}
    if( (0xffffffff - operande_1) >= val_sh && c) {
    	printf("C updated\n");
    	arm_write_cpsr(p, clr_bit(arm_read_cpsr(p), 29));
    }
	
}

void and_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh, int s){
   
  //Rd := Rn AND shifter_operand
	uint32_t resultat;
	
	resultat= arm_read_register(p,rn) & val_sh;
           arm_write_register(p,rd, resultat);

           if(s){
           	  tst_processing(p,rn,val_sh);
           }

}

void sub_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){
    
    //Rd := Rn - shifter_operand
    uint32_t resultat;
    int c = read_C(p);
    
     printf("C before sub = %d\n",c); 
    
    resultat= arm_read_register(p,rn) - val_sh;
   
           arm_write_register(p,rd, resultat);
           if(s){
           	 cmp_processing(p,rn,val_sh);
	}
	
	c = read_C(p);
	printf("C after sub = %d\n",c); 
}

void add_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){
	
	//Rd := Rn + shifter_operand
	uint32_t resultat;
	int c = read_C(p);;
 	printf("C before add = %d\n",c);
 	
 	printf("bit S = %d\n",s);
    resultat= arm_read_register(p,rn) + val_sh;
           arm_write_register(p,rd, resultat);
           
           printf("Result of add = %d\n",resultat);
           if(s){
           printf("IF ENTERED\n");
           	cmn_processing(p,rn,val_sh);
           }
       c = read_C(p);
       printf("C after add = %d\n",c);

}

void eor_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){
	
	//Rd := Rn EOR shifter_operand

	uint32_t resultat;

   resultat= arm_read_register(p,rn) ^ val_sh;
           arm_write_register(p,rd, resultat);
           if(s){
           	teq_processing(p,rn,val_sh);

			}
}


void rsb_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){

	//Rd := shifter_operand - Rn

   uint32_t resultat,valrn;
   int c = read_C(p);
	
	printf("C before RSB processing = %d\n",c);
   valrn=arm_read_register(p,rn);
   resultat= val_sh - valrn;
           arm_write_register(p,rd, resultat);
           
           if(s){
           	   
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
					
		c = read_C(p);
		printf("C after RSB processing = %d\n",c);
}

void adc_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){

	//Rd := Rn + shifter_operand + Carry Flag
	uint32_t resultat;
	int c = read_C(p);
	
	printf("C = %d\n",c);

	resultat= arm_read_register(p,rn) + val_sh + get_bit(arm_read_cpsr(p),29);
            arm_write_register(p,rd, resultat);
            if(s){
            	cmn_processing(p,rn,(val_sh+((get_bit(arm_read_cpsr(p),29)))));
				}

}

void sbc_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){

	//Rd := Rn - shifter_operand - NOT(Carry Flag)
	uint32_t resultat;
	int c = get_bit(arm_read_cpsr(p),29);
	
	resultat= arm_read_register(p,rn) - val_sh - !c;
	arm_write_register(p,rd, resultat);
	if(s){
		cmp_processing(p,rn,(val_sh-(~(get_bit(arm_read_cpsr(p),29)))));
	}
}

	


void rsc_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){

	//Rd := shifter_operand - Rn - NOT(Carry Flag)
	uint32_t resultat,valrn;
	valrn=arm_read_register(p,rn);
	int c = read_C(p);

	resultat= val_sh - valrn - !c;
	     arm_write_register(p,rd, resultat);
	     
	     printf("RESULTAT RSC processing = %d\n",resultat);
	     
	     if(s){
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

void orr_processing(arm_core p,uint8_t rn,uint8_t rd,uint32_t val_sh,  int s){

	//Logical (inclusive) OR Rd := Rn OR shifter_operand
	uint32_t resultat;

	resultat= arm_read_register(p,rn) | val_sh;
	          arm_write_register(p,rd, resultat);
	        
	        if(s){
	        	
	        	maj_ZN(p,resultat);
             //The V flag and the rest of the CPSR are unaffected.

		}
}

void mov_processing(arm_core p,uint8_t rd,uint32_t val_sh,  int s){
	
	//Rd := shifter_operand (no first operand)
	uint32_t resultat;
	printf("val_sh mov processing = %d\n", val_sh);
	        arm_write_register(p,rd, val_sh);
	        resultat=arm_read_register(p,rd);
	        
	        
	        printf("result = %d\n",resultat);

	        //si le registre destination est en mode user
	        if(s){
	        	
	        	maj_ZN(p,resultat);
    
             //The V flag and the rest of the CPSR are unaffected.
	} 

}

void bic_processing(arm_core p,uint8_t rn,uint8_t rd, uint32_t val_sh,  int s){

	//Bit Clear Rd := Rn AND NOT(shifter_operand)
	uint32_t resultat;
	int c = read_C(p);

	printf("C before BIC processing = %d\n",c);
	resultat= arm_read_register(p,rn) & (~val_sh);
	          arm_write_register(p,rd, resultat);

	          
	        if(s){
	        	
	        	tst_processing(p,rn,(~val_sh));
	}
	c = read_C(p);
	printf("C after BIC processing = %d\n",c);
	
}


/*************************************************************************
Auteur : Bianca
Date : 19/12/2017
Spec : Prends en argument l'instruction en 32 bits. Cette fonction traite 
	les instructions sans les valeurs immediate, donc avec que les registres.
	La fonction decoupe l'instruction et appelle les fonctions necessaire 
	par rapport a son op code. Les fonctions sont appellées que si la 
	condition est satisfiée.
**************************************************************************/

int arm_data_processing_shift(arm_core p, uint32_t ins) {
   int cond, rn, rd, opcode, s;
   uint8_t sh, rm, shift_imm, rs, I;
   uint32_t val_sh;
    
    cond = get_bits(ins, 31, 28);
    opcode = get_bits(ins, 24, 21);
    s = get_bit(ins, 20);
    rn = get_bits(ins, 19, 16);
    rd = get_bits(ins, 15, 12);

    I = get_bit(ins,4);
    shift_imm = get_bits(ins, 11, 7);
    rs = get_bits(ins, 11, 8);
    sh = get_bits(ins,6,5);
    rm = get_bits(ins,3,0);
    
     printf("SHIFT I %d\n",I);
    printf("SHIFT shift_imm %d\n", shift_imm);
    printf("SHIFT rs %d\n",rs);
    printf("SHIFT sh %d\n",sh);
    printf("SHIFT rm %d\n",rm);
    
    val_sh = shift(p, sh, shift_imm, rm, rs, I);
    printf("val_sh shift = %d\n",val_sh);
    
    if(condition(p, cond)){
    	switch(opcode){
    	case 0 :
    		and_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 1 :
    		eor_processing(p, rn,rd, val_sh, s);
    		break;
    		
    	case 2 :
    		sub_processing(p, rn,rd, val_sh, s);
    		break;
    		
    	case 3 :
    		rsb_processing(p, rn, rd, val_sh, s);
    		break;
    	
    	case 4 :
    		add_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 5 :
    		adc_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 6 :
    		sbc_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 7 :
    		rsc_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 10 :
    		cmp_processing(p, rn, val_sh);
    		break;
    		
    	case 11 :
    		cmn_processing(p, rn, val_sh);
    		break;
    		
    	case 12 :
    		orr_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 13 :
    		mov_processing(p, rd, val_sh, s);
    		break;
    		
    	case 14 :
    		bic_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 15 :
    		mvn_processing(p, rd, val_sh, s);
    		break;
    		
    	default:
    		break;
    }
   }
   return 0;
}

/*************************************************************************
Auteur : Bianca
Date : 19/12/2017
Spec : Prends en argument l'instruction en 32 bits. Cette fonction traite 
	les instructions avec les valeurs immediate. La fonction decoupe 
	l'instruction et appelle les fonctions necessaire par rapport a son op 
	code. Les fonctions sont appellées que si la condition est satisfiée.
**************************************************************************/

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    int rn, rd, opcode, cond, s, rotation;
    uint32_t val_sh, imm;
    
    cond = get_bits(ins, 31, 28);
    opcode = get_bits(ins, 24, 21);
    rn = get_bits(ins, 19, 16);
    rd = get_bits(ins, 15, 12);
    s = get_bit(ins, 20);
	
	 rotation = get_bits(ins, 11, 8);
    imm = get_bits(ins, 7, 0);
    printf("Rotation = %d\n",rotation);
    printf("Immediate = %d\n",imm);
    val_sh = ror(imm, rotation);
    printf("val_sh processing data = %d\n",val_sh);
    
    if(condition(p, cond)){
    	switch(opcode){
    	case 0 :
    		and_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 1 :
    		eor_processing(p, rn,rd, val_sh, s);
    		break;
    		
    	case 2 :
    		sub_processing(p, rn,rd, val_sh, s);
    		break;
    		
    	case 3 :
    		rsb_processing(p, rn, rd, val_sh, s);
    		break;
    	
    	case 4 :
    		add_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 5 :
    		adc_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 6 :
    		sbc_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 7 :
    		rsc_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 8 :
    		tst_processing(p, rn, val_sh);
    		
    		
    		break;
    		
    	case 9 :
    		teq_processing(p, rn, val_sh);
    		break;
    		
    	case 10 :
    		cmp_processing(p, rn, val_sh);
    		break;
    		
    	case 11 :
    		cmn_processing(p, rn, val_sh);
    		break;
    		
    	case 12 :
    		orr_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 13 :
    		mov_processing(p, rd, val_sh, s);
    		break;
    		
    	case 14 :
    		bic_processing(p, rn, rd, val_sh, s);
    		break;
    		
    	case 15 :
    		mvn_processing(p, rd, val_sh, s);
    		break;
    		
    	default:
    		break;
    }
   }
   return 0;
}
