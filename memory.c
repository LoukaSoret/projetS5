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
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"
#include "util.h"

struct memory_data {
    int size;
    int endianess;
    uint8_t *data;
    
};

/********************************************************
Auteur : Louka
Date : 18/12/2017
Spec: Prends en argument la taille de la memoire et son
endianness et renvois une memoire avec les specifications
demandees.
*********************************************************/
memory memory_create(size_t size, int is_big_endian) {
    memory mem;
    uint8_t *data = malloc(size*sizeof(uint8_t));
    mem = (memory) malloc(sizeof(struct memory_data));
    if(mem == NULL || data == NULL){        
        fprintf(stderr, "Error when creating simulated memory\n");
        exit(1);
    }
    mem->data = data;
    mem->size = size;
    mem->endianess = is_big_endian;
    return mem;
}

/****************************************
Auteur: Louka
Date: 18/12/2017
Spec: donne la taille de la memoire mem.
*****************************************/
size_t memory_get_size(memory mem) {
    return mem->size;
}

/****************************************
Auteur: Louka
Date: 18/12/2017
Spec: libere la memoire allouee pour mem.
*****************************************/
void memory_destroy(memory mem) {
    free(mem->data);
    mem->size=0;
    mem->endianess=0;
    free(mem);
}


/**************************************************************
Auteur: Kirill
Date: 18/12/2017
Spec: Stock la valeur du byte de la memoire a l'adresse adress
dans value. Renvois 0 si tout c'est bien passe -1 sinon.
***************************************************************/
int memory_read_byte(memory mem, uint32_t address, uint8_t *value) {
    if(address >= 0 && address < mem->size){
        *value = mem->data[address];
        return 0;
    }
    return -1;
}

/********************************************************************
Auteur: Kirill (+Louka)
Date: 18/12/2017
Spec: Stock la valeur du half word de la memoire a l'adresse
adress dans value. Renvois 0 si tout c'est bien passe -1 en cas
d'erreur d'access memoire et 1 en cas de désalignement de la memoire.
*********************************************************************/
int memory_read_half(memory mem, uint32_t address, uint16_t *value) {
    uint8_t frst_half;
    uint8_t scnd_half;
    int ctrl1,ctrl2;
    ctrl1 = memory_read_byte(mem,address,&frst_half);
    ctrl2 = memory_read_byte(mem,address+1,&scnd_half);
    if (ctrl1 || ctrl2){
        return -1;
    }
    else if(address%2 != 0){
        return 1;
    }
    if(mem->endianess){
        *value = (uint16_t)((((uint16_t) frst_half)<<8) | scnd_half);
    }
    else{
        *value = (uint16_t)((((uint16_t) scnd_half)<<8) | frst_half);
    }
    return 0;
}

/*
Auteur: Kirill (+Louka)
Date: 18/12/2017
Spec: Stock la valeur du word de la memoire a l'adresse
adress dans value. Renvois 0 si tout c'est bien passe -1 sinon.
*/
int memory_read_word(memory mem, uint32_t address, uint32_t *value) {
    uint16_t frst_half ;
    uint16_t scnd_half;
    int ctrl1,ctrl2;
    ctrl1 = memory_read_half(mem,address,&frst_half);
    ctrl2 = memory_read_half(mem,address+2,&scnd_half);
    if (ctrl1 || ctrl2){
        return -1;
    }
    else if(address%4 != 0){
        return 1;
    }
    if(mem->endianess){
        *value = (uint32_t)((((uint32_t) frst_half)<<16) | scnd_half);
    }
    else{
        *value = (uint32_t)((((uint32_t) scnd_half)<<16) | frst_half);
    }
    return 0;
}

/************************************************************************
Auteur: Kirill
Date: 18/12/2017
Spec: Stock la valeur du byte contenue dans value dans la  memoire a 
l'adresse adress. Renvois 0 si tout c'est bien passe -1 sinon.
*************************************************************************/
int memory_write_byte(memory mem, uint32_t address, uint8_t value) {
    if(address >= 0 && address < mem->size){
        mem->data[address]=value;
        return 0;
    }
    return -1;
}

/************************************************************************
Auteur: Kirill (+Louka)
Date: 18/12/2017
Spec: Stock la valeur du half word contenue dans value dans la  memoire a
l'adresse adress. Renvois 0 si tout c'est bien passe -1 sinon.
*************************************************************************/
int memory_write_half(memory mem, uint32_t address, uint16_t value) {
    int res;
    if(address%2 != 0){
        return 1;
    }
    if(mem->endianess){
        res = memory_write_byte(mem,address,((uint8_t)((value >> 8) & 0xff)));
        res |= memory_write_byte(mem,address+1,((uint8_t)((value) & 0xff)));
    }
    else{
        res = memory_write_byte(mem,address,((uint8_t)((value) & 0xff)));
        res |= memory_write_byte(mem,address+1,((uint8_t)((value >> 8) & 0xff)));   
    }
    return res;
}

/************************************************************************
Auteur: Kirill (+Louka)
Date: 18/12/2017
Spec: Stock la valeur du word contenue dans value dans la memoire a 
l'adresse adress. Renvois 0 si tout c'est bien passe -1 sinon.
*************************************************************************/
int memory_write_word(memory mem, uint32_t address, uint32_t value) {
    int res;
    if(address%4 != 0){
        return 1;
    }
    if(mem->endianess){
        res = memory_write_half(mem,address,((uint16_t)((value>>16) & 0xffff)));
        res |= memory_write_half(mem,address+2,(uint16_t)((value) & 0xffff));
    }
    else{
        res = memory_write_half(mem,address,(uint16_t)((value) & 0xffff));
        res |= memory_write_half(mem,address+2,((uint16_t)((value>>16) & 0xffff)));
    }
    return res;
}