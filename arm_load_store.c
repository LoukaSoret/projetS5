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

int arm_load_store(arm_core p, uint32_t ins) {
    uint8_t condition,codeOp;
    condition = get_bits(ins,31,28);
    codeOp = get_bits(ins,27,26);

    /* Condition Test */
    if(condition){
    	/* Load and store for bytes and words */
    	if(codeOp){
			return arm_load_store_byte_word(p,ins);
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
	uint8_t Rn,Rd,shift_amount,shift,Rm,P,U,I,W,L,S,H,VdByte,tst;
    uint16_t immediate;
    uint32_t Vn,Vd;
    Rn = get_bits(ins,19,16); // Address
    Rd = get_bits(ins,15,12); // content to be loaded or stored
    P = get_bit(ins,24); //post-indexed addressing(0) or pre-indexed addressing(1)
    U = get_bit(ins,23); // +offset (1) or -offset(0)
    I = get_bit(ins,22); // immediate (1) or register (0) offset
    W = get_bit(ins,21); // P=0 : MUST be 1 | P=0 Rn written back (1) or unchanged (0)
    L = get_bit(ins,20);
    S = get_bit(ins,6);
    H = get_bit(ins,5);
    tst = (L<<2) & (S<<1) & H;

    switch(tst)
    {
    	/* L=0, S=0, H=1 Store halfword. */
    	case 0b001 :
    		return store_halfword(Rn,Rd,P,U,I,W);
    		break;
    	/* L=0, S=1, H=0 Load doubleword. */
    	case 0b010 :
    		return load_doubleword(Rn,Rd,P,U,I,W);
    		break;
    	/* L=0, S=1, H=1 Store doubleword. */
    	case 0b011 :
    		return store_doubleword(Rn,Rd,P,U,I,W);
    		break;
    	/* L=1, S=0, H=1 Load unsigned halfword. */
    	case 0b101 :
    		return load_uhalfword(Rn,Rd,P,U,I,W);
    		break;
    	/* L=1, S=1, H=0 Load signed byte. */
    	case 0b110 :
    		return load_byte(Rn,Rd,P,U,I,W);
    		break;
    	/* L=1, S=1, H=1 Load signed halfword */
    	case 0b111 :
    		return load_halfword(Rn,Rd,P,U,I,W);
    		break;
    	default :
    		return -1;
    		break;
    }
}

int arm_load_byte_word(arm_core p,uint32_t ins)
{
	uint8_t Rn,Rd,shift_amount,shift,Rm,P,U,B,W,L,VdByte;
    uint16_t immediate;
    uint32_t Vn,Vd;
    Rn = get_bits(ins,19,16); // Address
    Rd = get_bits(ins,15,12); // content to be loaded or stored
    P = get_bit(ins,24); // post-indexed addressing(0) or pre-indexed addressing(1)
    U = get_bit(ins,23); // +offset (1) or -offset(0)
    B = get_bit(ins,22); // byte(1) or word(0)
    W = get_bit(ins,21); 
    // si P=0 : LDR , LDRB , STR or STRB (0) or LDRBT , LDRT , STRBT or STRT (1)
    // si P=1 : the base register is not updated (0) or the base register is updated (1) 
    L = get_bit(ins,20); //Load(1) or store(0)
/* LOAD */
if(L){
	/* pre-indexed addressing */
	if(P){
		/* the base register is not updated */
		if(W){
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) - immediate;
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						if(B){
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
			    		Vn = arm_read_register(p, Rn) - shiftDecode(Rm,shift_amount,shift);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) + immediate;
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						if(B){
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
			    		Vn = arm_read_register(p, Rn) + shiftDecode(Rm,shift_amount,shift);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) - immediate;
					arm_write_register(p,Rn,Vn);
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						if(B){
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
			    		Vn = arm_read_register(p, Rn) - shiftDecode(Rm,shift_amount,shift);
						arm_write_register(p,Rn,Vn);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) + immediate;
					arm_write_register(p,Rn,Vn);
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						if(B){
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
			    		Vn = arm_read_register(p, Rn) + shiftDecode(Rm,shift_amount,shift);
						arm_write_register(p,Rn,Vn);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn - immediate);
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn - arm_read_register(p, Rm));
						if(B){
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
			    		arm_write_register(p,Rn,Vn - shiftDecode(Rm,shift_amount,shift));
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn + immediate);
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn + arm_read_register(p, Rm));
						if(B){
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
			    		arm_write_register(p,Rn,Vn + shiftDecode(Rm,shift_amount,shift));
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn - immediate);
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn - arm_read_register(p, Rm));
						if(B){
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
						arm_write_usr_register(p,Rn,Vn - shiftDecode(Rm,shift_amount,shift));
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn + immediate);
					if(B){
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
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn + arm_read_register(p, Rm));
						if(B){
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
						arm_write_usr_register(p,Rn,Vn + shiftDecode(Rm,shift_amount,shift));
						if(B){
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
}

int arm_store_byte_word(arm_core p,uint32_t ins)
{
	uint8_t Rn,Rd,shift_amount,shift,Rm,P,U,B,W,L,VdByte;
    uint16_t immediate;
    uint32_t Vn,Vd;
    Rn = get_bits(ins,19,16); // Address
    Rd = get_bits(ins,15,12); // content to be loaded or stored
    P = get_bit(ins,24); // post-indexed addressing(0) or pre-indexed addressing(1)
    U = get_bit(ins,23); // +offset (1) or -offset(0)
    B = get_bit(ins,22); // byte(1) or word(0)
    W = get_bit(ins,21); 
    // si P=0 : LDR , LDRB , STR or STRB (0) or LDRBT , LDRT , STRBT or STRT (1)
    // si P=1 : the base register is not updated (0) or the base register is updated (1) 

	/* pre-indexed addressing */
	if(P){
		/* the base register is not updated */
		if(W){
			/* -offset */
			if(!U){
			 	/* immediate offset */
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) - immediate;
					Vd = arm_read_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						Vd = arm_read_register(p, Rd);
						if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) - shiftDecode(Rm,shift_amount,shift);
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) + immediate;
					Vd = arm_read_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}	
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						Vd = arm_read_register(p, Rd);
						if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) + shiftDecode(Rm,shift_amount,shift);
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) - immediate;
					arm_write_register(p,Rn,Vn);
					Vd = arm_read_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) - arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) - shiftDecode(Rm,shift_amount,shift);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn) + immediate;
					arm_write_register(p,Rn,Vn);
					Vd = arm_read_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn) + arm_read_register(p, Rm);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(B){
							arm_write_byte(p, Vn, Vd);
						}
						else{
							arm_write_word(p, Vn, Vd);
						}
						return 0;
			    	}
			    	/* scaled register offset */ 
			    	else{
			    		Vn = arm_read_register(p, Rn) + shiftDecode(Rm,shift_amount,shift);
						arm_write_register(p,Rn,Vn);
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn - immediate);
					Vd = arm_read_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn - arm_read_register(p, Rm));
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    		arm_write_register(p,Rn,Vn - shiftDecode(Rm,shift_amount,shift));
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_register(p, Rn);
					arm_write_register(p,Rn,Vn + immediate);
					Vd = arm_read_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_register(p, Rn);
			    		arm_write_register(p,Rn,Vn + arm_read_register(p, Rm));
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    		arm_write_register(p,Rn,Vn + shiftDecode(Rm,shift_amount,shift));
						Vd = arm_read_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn - immediate);
					Vd = arm_read_usr_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn - arm_read_register(p, Rm));
						Vd = arm_read_usr_register(p, Rd);
						if(B){
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
						arm_write_usr_register(p,Rn,Vn - shiftDecode(Rm,shift_amount,shift));
						Vd = arm_read_usr_register(p, Rd);
						if(B){
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
			    if(!get_bit(ins,25)){
					immediate = get_bits(ins,11,0);
					Vn = arm_read_usr_register(p, Rn);
					arm_write_usr_register(p,Rn,Vn + immediate);
					Vd = arm_read_usr_register(p, Rd);
					if(B){
						arm_write_byte(p, Vn, Vd);
					}
					else{
						arm_write_word(p, Vn, Vd);
					}
					return 0;
			    }
			    /* register/scaled register offset */
			    else{
			    	shift_amount = get_bits(ins,11,7);
			    	shift = get_bits(ins,6,5);
			    	Rm = get_bits(ins,3,0);
			    	/* register register offset */
			    	if(shift == 0 && shift_amount == 0){
			    		Vn = arm_read_usr_register(p, Rn);
						arm_write_usr_register(p,Rn,Vn + arm_read_register(p, Rm));
						Vd = arm_read_usr_register(p, Rd);
						if(B){
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
						arm_write_usr_register(p,Rn,Vn + shiftDecode(Rm,shift_amount,shift));
						Vd = arm_read_usr_register(p, Rd);
						if(B){
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
}