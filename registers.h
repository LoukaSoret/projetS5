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
#ifndef __REGISTERS_H__
#define __REGISTERS_H__
#include <stdint.h>

#define FIQ_OFFSET 5

// SP index
#define USR_SYS_SP 18
#define SVC_SP 19
#define ABT_SP 20
#define FIQ_SP 21
#define IRQ_SP 22
#define UND_SP 23
// LR index
#define USR_SYS_LR 24
#define SVC_LR 25
#define ABT_LR 26
#define FIQ_LR 27
#define IRQ_LR 28
#define UND_LR 29

// SPSR index
#define SVC_SPSR 31
#define ABT_SPSR 32
#define FIQ_SPSR 33
#define IRQ_SPSR 34
#define UND_SPSR 35

#define PC 30
#define CPSR_REG 31 



typedef struct registers_data *registers;

/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Creer des registres vide.
**************************************************************************/
registers registers_create();

/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Detruit les registres
**************************************************************************/
void registers_destroy(registers r);


/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Revoie le mode.
**************************************************************************/
uint8_t get_mode(registers r);

/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Revoie vrai si le mode a l'access � le spsr. Revoie faux sinon.
**************************************************************************/
int current_mode_has_spsr(registers r);

/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Revoie vrai si le mode est privilig�. Revoie faux sinon.
**************************************************************************/
int in_a_privileged_mode(registers r);


uint32_t read_register(registers r, uint8_t reg);
uint32_t read_usr_register(registers r, uint8_t reg);

/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Revoie la valeur dans le cpsr.
**************************************************************************/
uint32_t read_cpsr(registers r);

uint32_t read_spsr(registers r);
void write_register(registers r, uint8_t reg, uint32_t value);
void write_usr_register(registers r, uint8_t reg, uint32_t value);

/*************************************************************************
Auteur : Bianca
Date : 18/12/2017
Spec : Prends en argument une valeur. Ecrit le valeur dans le cpsr
**************************************************************************/
void write_cpsr(registers r, uint32_t value);
void write_spsr(registers r, uint32_t value);

#endif


