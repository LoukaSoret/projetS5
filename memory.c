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
#include <stdlib.h>
#include "memory.h"
#include "util.h"

struct memory_data {
    int size;
    int endianess;
    uint8_t *data;
};

memory memory_create(size_t size, int is_big_endian) {
    memory mem;
    uint32_t data[size];
    mem = (memory) malloc(sizeof(struct memory_data));
    if(mem == NULL){        
        fprintf(stderr, "Error when creating simulated memory\n");
        exit(1);
    }
    mem.data = data;
    mem.size = size;
    mem.endianess = is_big_endian;
    return mem;
}

size_t memory_get_size(memory mem) {
    return mem.size;
}

void memory_destroy(memory mem) {
    free(mem.data);
    mem.size=0;
    mem.endianess=0;
    free(mem);
}

int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
    return -1;
}

int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
    return -1;
}

int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
    return -1;
}

int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    if(address >= 0 && address < mem.size){
        mem.data[address]=value;
        return 0;
    }
    return -1;
}

int memory_write_half(memory mem, uint32_t address, uint16_t value) {
    int res;
    res = memory_write_byte(mem,address,(uint8_t)(value>>sizeof(uint8_t)));
    res |= memory_write_byte(mem,address+1,(uint8_t)((value<<sizeof(uint8_t))>>sizeof(uint8_t)));
    return res;
}

int memory_write_word(memory mem, uint32_t address, uint32_t value) {
    int res;
    res = memory_write_half(mem,address,(uint16_t)(value>>sizeof(uint16_t)));
    res |= memory_write_half(mem,address+2,(uint16_t)((value<<sizeof(uint16_t))>>sizeof(uint16_t)));
    return res;
}
