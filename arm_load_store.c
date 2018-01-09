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
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "util.h"
#include "arm_load_store.h"
#include "arm_core.h"
#include "decodeur_cond_shift.h"

/*************************************************************
Auteur : Louka
Date : 22/12/17
Spec: Prends en entree le coeur arm p et l'instruction ins
renvois le code d'erreur 0 si tout c'est bien passe. modifie
l'etat des registres et de la memoire en fonction de 
l'instruction par effets de bords.
*************************************************************/
int arm_load_store(arm_core p, uint32_t ins) {
	uint8_t Rn,Rd,shift_amount,shift_codeOp,Rm,P,U,I,B,W,L,codeOp;;
    uint16_t immediate;

    Rn = get_bits(ins,19,16); // Address
    Rd = get_bits(ins,15,12); // content to be loaded or stored
    I = get_bit(ins,25); //immediate (0) or register (1) offset
    P = get_bit(ins,24); // post-indexed addressing(0) or pre-indexed addressing(1)
    U = get_bit(ins,23); // +offset (1) or -offset(0)
    B = get_bit(ins,22); // byte(1) or word(0)
    W = get_bit(ins,21);
    // si P=0 : LDR , LDRB , STR or STRB (0) or LDRBT , LDRT , STRBT or STRT (1)
    // si P=1 : the base register is not updated (0) or the base register is updated (1)
    L = get_bit(ins,20); // Load (1) or Store (0)
    immediate = get_bits(ins,11,0); //immediate offset
    shift_amount = get_bits(ins,11,7); // shift amount for register offset
	shift_codeOp = get_bits(ins,6,5); // shift op code for register offset
	Rm = get_bits(ins,3,0); // offset register

    //condition = get_bits(ins,31,28);
    codeOp = get_bits(ins,27,26);

    printf("Rn : %i | Rd : %i\n",Rn,Rd);

    /* Condition Test */
    if(1 /*condition(p,ins)*/){
    	/* Load and store for bytes and words */
    	if(codeOp){
    		if(L){
    			return arm_load(p,Rn,Rd,I,P,U,B,W,0,0,immediate,shift_amount,shift_codeOp,Rm);
    		}
    		else{
    			return arm_store(p,Rn,Rd,I,P,U,B,W,0,0,immediate,shift_amount,shift_codeOp,Rm);
    		}
    	}
    	/* Load and store for half-words and double-words */
    	else{
    		return arm_load_store_half_double(p,ins);
    	}
   	}
    return -1;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
	printf("Debut de la fonction Load_Store_Multiple\n");
	if(condition(p,get_bits(ins,31,28))){
		printf("Condition passé\n");
		uint32_t adr,value = 0;
		int L = get_bit(ins,20);
		int U = get_bit(ins,23);
		int P = get_bit(ins,24);
		int W = get_bit(ins,21);
		int nbreg = 0;
	    if(L){ //Cas du load
	    	printf("Debut de l'Initialisation\n");
	    	adr = arm_read_register(p,get_bits(ins,19,16));
	    	printf("Adresse initialisé : %0x\n",adr);
	    	if(U){ // U == 1 , on remonte les adresses
	    		printf("On Incremente PC \n");
	    		if(P){ // P == 1, la premiere valeur est en dehors de la range
	    			adr += 4;
	    		}
		    	for(int i=0;i<=15;i++){
		    		printf("On parcours les registers\n");
		    		if(get_bit(ins,i)){
		    			printf("Registre %d est affecté\n",i );
		    			arm_read_word(p,adr,&value);
		    			printf("Valeur lu : %0x Pour l'adresse : %0x\n",value,adr );
		    			arm_write_register(p,i,value);
		    			adr+=4;
		    			nbreg++;
		    		}
		    	}
		    	if(W){ // W == 1, on incremente Rn
	    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))+(4*nbreg));
	    		}
	    	}else{
	    		printf("On decremente PC\n");
	    		for(int i=0;i<=15;i++){
		    		printf("On parcours les registers\n");
		    		if(get_bit(ins,i)){
		    			nbreg++;
		    		}
		    	}
		    	adr = arm_read_register(p,get_bits(ins,19,16))-(4*nbreg);
	    		if(P){ // P == 1, la premiere valeur est en dehors de la range
	    			adr+=4;
	    		}
		    	for(int i=0;i<=15;i++){
		    		printf("On parcours les registers\n");
		    		if(get_bit(ins,i)){
		    			printf("Registre %d est affecté\n",i );
		    			arm_read_word(p,adr,&value);
		    			arm_write_register(p,i,value);
		    			adr+=4;
		    		}
		    	}
		    	if(W){ // W == 1, on incremente Rn
	    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))-(4*nbreg));
	    		}
	    	}
	    	return 0;
	    }else{ // Cas du store
	    	adr = arm_read_register(p,get_bits(ins,19,16));
	    	if(U){ // U == 1 , on remonte les adresses
	    		if(P){ // P == 1, la premiere valeur est en dehors de la range
	    			adr+=4;
	    		}
		    	for(int i=15;i>=0;i--){
		    		if(get_bit(ins,i)){
		    			arm_write_word(p,adr,arm_read_register(p,i));
		    			adr+=4;
		    			nbreg++;
		    		}
		    	}
		    	if(W){ // W == 1, on incremente Rn
	    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))+(4*nbreg));
	    		}
	    	}else{
	    		for(int i=0;i<=15;i++){
		    		printf("On parcours les registers\n");
		    		if(get_bit(ins,i)){
		    			nbreg++;
		    		}
		    	}
		    	adr = arm_read_register(p,get_bits(ins,19,16))-(4*nbreg);
	    		if(P){ // P == 1, la premiere valeur est en dehors de la range
	    			adr+=4;
	    		}
		    	for(int i=15;i>=0;i--){
		    		if(get_bit(ins,i)){
		    			arm_write_word(p,adr,arm_read_register(p,i));
		    			adr+=4;
		    		}
		    	}
		    	if(W){ // W == 1, on incremente Rn
	    			arm_write_register(p,get_bits(ins,19,16),arm_read_register(p,get_bits(ins,19,16))-(4*nbreg));
	    		}
	    	}
	    	return 0;
	    }
	}
	return 0;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return -1;
}

/**************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Prends en argument une adresse, un offset et un bit U
et renvois adresse + offset si U=1 ou adresse + offset si U=0
***************************************************************/
int offsetHandling(int address,int offset,uint8_t U){
	if(U){
		return address + offset;
	} else {
		return address - offset;
	}
}

/***************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Gere les cas des load et store pour signed bytes, double
words et signed et unsigned half words.
****************************************************************/
int arm_load_store_half_double(arm_core p,uint32_t ins)
{
	uint8_t Rn,Rd,shift_amount,shift_codeOp,Rm,P,U,I,W,L,S,H,tst;
    uint16_t immediate;
    Rn = get_bits(ins,19,16); // Address
    Rd = get_bits(ins,15,12); // content to be loaded or stored
    P = get_bit(ins,24); //post-indexed addressing(0) or pre-indexed addressing(1)
    U = get_bit(ins,23); // +offset (1) or -offset(0)
    I = ! get_bit(ins,22); // immediate (1) or register (0) offset
    W = get_bit(ins,21); // P=0 : MUST be 1 | P=0 Rn written back (1) or unchanged (0)
    L = get_bit(ins,20); // Load (1) or Store (0)
    S = get_bit(ins,6);
    H = get_bit(ins,5);
    tst = (L<<2) | (S<<1) | H;
    if(!P && !W){W=1;} // Compatibilitee fonctions

    immediate = ((uint16_t)(get_bits(ins,11,8))<<4) | get_bits(ins,3,0)  ; //immediate offset
    shift_amount = 0; // shift amount for register offset
	shift_codeOp = 0; // shift op code for register offset
	Rm = get_bits(ins,3,0); // offset register

    switch(tst) // LSH
    {
    	/* L=0, S=0, H=1 Store halfword. */
    	case 0b001 :
    		return arm_load(p,Rn,Rd,I,P,U,0,W,1,0,immediate,shift_amount,shift_codeOp,Rm);
    		break;
    	/* L=0, S=1, H=0 Load doubleword. */
    	case 0b010 :
    		return arm_load(p,Rn,Rd,I,P,U,1,W,0,1,immediate,shift_amount,shift_codeOp,Rm);
    		break;
    	/* L=0, S=1, H=1 Store doubleword. */
    	case 0b011 :
    		return arm_load(p,Rn,Rd,I,P,U,0,W,0,1,immediate,shift_amount,shift_codeOp,Rm);
    		break;
    	/* L=1, S=0, H=1 Load unsigned halfword. */
    	case 0b111 :
    	case 0b101 :
    		return arm_load(p,Rn,Rd,I,P,U,1,W,1,0,immediate,shift_amount,shift_codeOp,Rm);
    		break;
    	/* L=1, S=1, H=0 Load signed byte. */
    	case 0b110 :
    		return arm_load(p,Rn,Rd,I,P,U,1,W,0,0,immediate,shift_amount,shift_codeOp,Rm);
    		break;
    	default :
    		return -1;
    		break;
    }
}

/**************************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Gestion de toutes les instructions de type load.Prends en arguments
un coeur arm p, les registres Rn (adresse), Rd(destination) et Rs(shift),
le code op shift_codeOp, la valeur immediate shift_amount, et les bits 
I(immediate offset),P(addressing mode),U(+/- offset),B(byte/word access),
W(addressing mode),D(half/double word access). Renvois 0 si tout c'est bien
passe. Modifie les registres et la memoire par effets de bords.
****************************************************************************/
int arm_load(arm_core p,uint8_t Rn,uint8_t Rd,uint8_t I,uint8_t P,uint8_t U,uint8_t B,uint8_t W ,uint8_t H,uint8_t D,uint16_t immediate,uint8_t shift_amount,uint8_t shift_codeOp,uint8_t Rm)
{
	int error=0;
	uint8_t VdByte;
	uint16_t VdHalf;
    uint32_t Vn,Vd;

    if(!I){
		if(!P){
			Vn = arm_read_register(p, Rn);
			if(W){ arm_write_register(p,Rn,offsetHandling(Vn,immediate,U)); }
			else { arm_write_usr_register(p,Rn,offsetHandling(Vn,immediate,U)); }
		}	
		else{
			Vn = offsetHandling(arm_read_register(p,Rn),immediate,U);				
			if(W){ arm_write_register(p,Rn,Vn); }
		}
    }
    else{
		if(!P){
			Vn = arm_read_register(p, Rn);
			if(W){ arm_write_register(p,Rn,offsetHandling(Vn,shift(p,shift_codeOp,shift_amount,Rm,0,0),U)); }
			else { arm_write_usr_register(p,Rn,offsetHandling(Vn,shift(p,shift_codeOp,shift_amount,Rm,0,0),U)); }
		}	
		else{
			printf("SHAD NO !\n");
			Vn = offsetHandling(arm_read_register(p, Rn),shift(p,shift_codeOp,shift_amount,Rm,0,0),U);				
			if(W){ arm_write_register(p,Rn,Vn); }
		}
	}
	if(H){
		error |= arm_read_half(p, Vn, &VdHalf);
		if(!P && !W){
			arm_write_usr_register(p, Rd, VdHalf);
		} else{
			arm_write_register(p, Rd, VdHalf);
		}
	}
	else if(D && Rd<14){
		if(!P && !W){ 
			error |= arm_read_word(p, Vn, &Vd);
			arm_write_usr_register(p, Rd, Vd);
			Vn += 4;
			error |= arm_read_word(p, Vn, &Vd);
			arm_write_usr_register(p, Rd+1, Vd);
		} else {
			error |= arm_read_word(p, Vn, &Vd);
			arm_write_register(p, Rd, Vd);
			Vn += 4;
			error |= arm_read_word(p, Vn, &Vd);
			arm_write_register(p, Rd+1, Vd);		
		}
	}
	else if(B){
		error |= arm_read_byte(p, Vn, &VdByte);
		if(!P && !W){
			arm_write_usr_register(p, Rd, VdByte);;
		} else{
			arm_write_register(p, Rd, VdByte);
		}
	}
	else{
		error |= arm_read_word(p, Vn, &Vd);
		if(!P && !W){
			arm_write_usr_register(p, Rd, Vd);
		} else{
			arm_write_register(p, Rd, Vd);
		}
	}
	return error;
}


/**************************************************************************
Auteur : Louka
Date : 23/12/2017
Spec : Gestion de toutes les instructions de type store.Prends en arguments
un coeur arm p, les registres Rn (adresse), Rd(destination) et Rs(shift),
le code op shift_codeOp, la valeur immediate shift_amount, et les bits 
I(immediate offset),P(addressing mode),U(+/- offset),B(byte/word access),
W(addressing mode),D(half/double word access). Renvois 0 si tout c'est bien
passe. Modifie les registres et la memoire par effets de bords.
****************************************************************************/
int arm_store(arm_core p,uint8_t Rn,uint8_t Rd,uint8_t I,uint8_t P,uint8_t U,uint8_t B,uint8_t W ,uint8_t H,uint8_t D,uint16_t immediate,uint8_t shift_amount,uint8_t shift_codeOp,uint8_t Rm) {
	int error=0;
	uint32_t Vn,Vd;
    
    if(!I){
		if(!P){
			Vn = arm_read_register(p, Rn);
			if(W){ arm_write_register(p,Rn,offsetHandling(Vn,immediate,U)); }
			else { arm_write_usr_register(p,Rn,offsetHandling(Vn,immediate,U)); }
		}	
		else{
			Vn = offsetHandling(arm_read_register(p,Rn),immediate,U);				
			if(W){ arm_write_register(p,Rn,Vn); }
		}
    }
    else{
		if(!P){
			Vn = arm_read_register(p, Rn);
			if(W){ arm_write_register(p,Rn,offsetHandling(Vn,shift(p,shift_codeOp,shift_amount,Rm,0,0),U)); }
			else { arm_write_usr_register(p,Rn,offsetHandling(Vn,shift(p,shift_codeOp,shift_amount,Rm,0,0),U)); }
		}	
		else{
			Vn = offsetHandling(arm_read_register(p, Rn),shift(p,shift_codeOp,shift_amount,Rm,0,0),U);				
			if(W){ arm_write_register(p,Rn,Vn); }
		}
	}
	if(!P && !W){
		Vd = arm_read_usr_register(p, Rd);
	} else {
		Vd = arm_read_register(p, Rd);
	}
	if(H){
		error |= arm_write_half(p, Vn, Vd);
	}
	else if(D && Rd<14){
		error |= arm_write_word(p, Vn, Vd);
		Vn += 4;
		if(!P && !W){
			Vd = arm_read_usr_register(p, Rd+1);
		} else {
			Vd = arm_read_register(p, Rd+1);
		}
		Vd = arm_read_register(p, Rd+1);
		error |= arm_write_word(p, Vn, Vd);
	}
	else if(B){
		error |= arm_write_byte(p, Vn, Vd);
	}
	else{
		error |= arm_write_word(p, Vn, Vd);
	}
	
	return 0;
}
