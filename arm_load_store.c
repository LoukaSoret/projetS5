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
#include "util.h"
#include "debug.h"
#include "decodeur_cond_shift.h"

int arm_load_store(arm_core p, uint32_t ins) {
	uint8_t Rn,Rd,shift_amount,shift,Rm,P,U,I,B,W,L,condition,codeOp;;
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
	shift = get_bits(ins,6,5); // shift op code for register offset
	Rm = get_bits(ins,3,0); // offset register

    condition = get_bits(ins,31,28);
    codeOp = get_bits(ins,27,26);

    /* Condition Test */
    if(condition(p,ins)){
    	/* Load and store for bytes and words */
    	if(codeOp){
    		if(L){
    			return arm_load_byte_word(p,Rn,Rd,I,P,U,B,W,0,0,immediate,shift_amount,shift,Rm);
    		}
    		else{
    			return arm_store_byte_word(p,Rn,Rd,I,P,U,B,W,0,0,immediate,shift_amount,shift,Rm);
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
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_half_double(arm_core p,uint32_t ins)
{
	uint8_t Rn,Rd,shift_amount,shift,Rm,P,U,I,W,L,S,H,tst;
    uint16_t immediate;
    Rn = get_bits(ins,19,16); // Address
    Rd = get_bits(ins,15,12); // content to be loaded or stored
    P = get_bit(ins,24); //post-indexed addressing(0) or pre-indexed addressing(1)
    U = get_bit(ins,23); // +offset (1) or -offset(0)
    I = get_bit(ins,22); // immediate (1) or register (0) offset
    W = get_bit(ins,21); // P=0 : MUST be 1 | P=0 Rn written back (1) or unchanged (0)
    L = get_bit(ins,20); // Load (1) or Store (0)
    S = get_bit(ins,6);
    H = get_bit(ins,5);
    tst = (L<<2) | (S<<1) | H;
    if(!P && !W){W=1;} // Compatibilitee fonctions

    immediate = ((uint16_t)(get_bits(ins,11,8))<<4) | get_bits(ins,3,0)  ; //immediate offset
    shift_amount = 0; // shift amount for register offset
	shift = 0; // shift op code for register offset
	Rm = get_bits(ins,3,0); // offset register

    switch(tst) // LSH
    {
    	/* L=0, S=0, H=1 Store halfword. */
    	case 0b001 :
    		return arm_load_byte_word(p,Rn,Rd,I,P,U,0,W,1,0,immediate,shift_amount,shift,Rm);
    		break;
    	/* L=0, S=1, H=0 Load doubleword. */
    	case 0b010 :
    		return arm_load_byte_word(p,Rn,Rd,I,P,U,1,W,0,1,immediate,shift_amount,shift,Rm);
    		break;
    	/* L=0, S=1, H=1 Store doubleword. */
    	case 0b011 :
    		return arm_load_byte_word(p,Rn,Rd,I,P,U,0,W,0,1,immediate,shift_amount,shift,Rm);
    		break;
    	/* L=1, S=0, H=1 Load unsigned halfword. */
    	case 0b111 :
    	case 0b101 :
    		return arm_load_byte_word(p,Rn,Rd,I,P,U,1,W,1,0,immediate,shift_amount,shift,Rm);
    		break;
    	/* L=1, S=1, H=0 Load signed byte. */
    	case 0b110 :
    		return arm_load_byte_word(p,Rn,Rd,I,P,U,1,W,0,0,immediate,shift_amount,shift,Rm);
    		break;
    	default :
    		return -1;
    		break;
    }
}

int arm_load_byte_word(arm_core p,uint8_t Rn,uint8_t Rd,uint8_t I,uint8_t P,uint8_t U,uint8_t B,uint8_t W ,uint8_t H,uint8_t D,uint16_t immediate,uint8_t shift_amount,uint8_t shift,uint8_t Rm)
{
	uint8_t VdByte;
	uint16_t VdHalf;
    uint32_t Vn,Vd;

	/* pre-indexed addressing */
	if(P){
		/* the base register is not updated */
		if(W){
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) - immediate;
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd+1, Vd);
					}
					else if(B){
						arm_read_byte(p, Vn, &VdByte);
						arm_write_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) - shift(p,ins);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) + immediate;
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd+1, Vd);
					}
					else if(B){
					arm_read_byte(p, Vn, &VdByte);
					arm_write_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) + shift(p,ins);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
		}
		/* the base register is updated */
		else{
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) - immediate;
					arm_write_register(p,Rn,Vn);
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd+1, Vd);
					}
					else if(B){
					arm_read_byte(p, Vn, &VdByte);
					arm_write_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) - shift(p,ins);
						arm_write_register(p,Rn,Vn);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) + immediate;
					arm_write_register(p,Rn,Vn);
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd+1, Vd);
					}
					else if(B){
						arm_read_byte(p, Vn, &VdByte);
						arm_write_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) + shift(p,ins);
						arm_write_register(p,Rn,Vn);
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
		}
	}
	/* post-indexed addressing */
	else{
		/* normal memory access */
		if(W){
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn - immediate);
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd+1, Vd);
					}
					else if(B){
					arm_read_byte(p, Vn, &VdByte);
					arm_write_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn - arm_read_register(p, Rm));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn - shift(p,ins));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn + immediate);
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd+1, Vd);
					}
					else if(B){
					arm_read_byte(p, Vn, &VdByte);
					arm_write_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn + arm_read_register(p, Rm));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn + shift(p,ins));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
		}
		/* User mode memory access */
		else{
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn - immediate);
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_usr_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_usr_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_usr_register(p, Rd+1, Vd);
					}
					else if(B){
					arm_read_byte(p, Vn, &VdByte);
					arm_write_usr_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_usr_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn - arm_read_register(p, Rm));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_usr_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_usr_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn - shift(p,ins));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_usr_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_usr_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn + immediate);
					if(H){
						arm_read_half(p, Vn, &VdHalf);
						arm_write_usr_register(p, Rd, VdHalf);
					}
					else if(D && Rd<14){
						arm_read_word(p, Vn, &Vd);
						arm_write_usr_register(p, Rd, Vd);
						Vn += 4;
						arm_read_word(p, Vn, &Vd);
						arm_write_usr_register(p, Rd+1, Vd);
					}
					else if(B){
					arm_read_byte(p, Vn, &VdByte);
					arm_write_usr_register(p, Rd, VdByte);
					}
					else{
						arm_read_word(p, Vn, &Vd);
						arm_write_usr_register(p, Rd, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn + arm_read_register(p, Rm));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_usr_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_usr_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn + shift(p,ins));
						if(H){
							arm_read_half(p, Vn, &VdHalf);
							arm_write_usr_register(p, Rd, VdHalf);
						}
						else if(D && Rd<14){
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
							Vn += 4;
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd+1, Vd);
						}
						else if(B){
							arm_read_byte(p, Vn, &VdByte);
							arm_write_usr_register(p, Rd, VdByte);
						}
						else{
							arm_read_word(p, Vn, &Vd);
							arm_write_usr_register(p, Rd, Vd);
						}
						return 0;
					}
				}
			}
		}
	}
	return -1;
}

int arm_store_byte_word(arm_core p,uint8_t Rn,uint8_t Rd,uint8_t I,uint8_t P,uint8_t U,uint8_t B,uint8_t W ,uint8_t H,uint8_t D,uint16_t immediate,uint8_t shift_amount,uint8_t shift,uint8_t Rm) {
	uint32_t Vn,Vd;
    
	/* pre-indexed addressing */
	if(P){
		/* the base register is not updated */
		if(W){
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) - immediate;
					Vd = arm_read_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) - shift(p,ins);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) + immediate;
					Vd = arm_read_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}	
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) + shift(p,ins);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
		}
		/* the base register is updated */
		else{
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) - immediate;
					arm_write_register(p,Rn,Vn);
					Vd = arm_read_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) - shift(p,ins);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn) + immediate;
					arm_write_register(p,Rn,Vn);
					Vd = arm_read_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) + shift(p,ins);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
		}
	}
	/* post-indexed addressing */
	else{
		/* normal memory access */
		if(W){
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn - immediate);
					Vd = arm_read_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn - arm_read_register(p, Rm));
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn - shift(p,ins));
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn + immediate);
					Vd = arm_read_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn + arm_read_register(p, Rm));
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn + shift(p,ins));
						Vd = arm_read_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
		}
		/* User mode memory access */
		else{
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn - immediate);
					Vd = arm_read_usr_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn - arm_read_register(p, Rm));
						Vd = arm_read_usr_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn - shift(p,ins));
						Vd = arm_read_usr_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
			/* +offset */
			else{
			 	/* immediate offset */
			    if(!I){
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn + immediate);
					Vd = arm_read_usr_register(p, Rd);
					if(H){
						arm_write_half(p, Vn, Vd);
					}
					else if(D && Rd<14){
						arm_write_word(p, Vn, Vd);
						Vn += 4;
						Vd = arm_read_register(p, Rd+1);
						arm_write_word(p, Vn, Vd);
					}
					else if(B){
					arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn + arm_read_register(p, Rm));
						Vd = arm_read_usr_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn + shift(p,ins));
						Vd = arm_read_usr_register(p, Rd);
						if(H){
							arm_write_half(p, Vn, Vd);
						}
						else if(D && Rd<14){
							arm_write_word(p, Vn, Vd);
							Vn += 4;
							Vd = arm_read_register(p, Rd+1);
							arm_write_word(p, Vn, Vd);
						}
						else if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
					}
				}
			}
		}
	}
	return -1;
}