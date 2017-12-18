struct registers_data {
	uint32_t reg_no[18]; 	//16 registres + cpsr + spsr
	uint8_t mode;					/* 7 modes : 10000 User (0)		10001 FIQ (1)	10010 IRQ (2)		10011 Supervisor (3)		10111 Abort (4)		11011 Undefined (5)		11111 System (6)
	Privileged modes: tous sauf User
	SPSR: tous sauf User et System*/
};



registers registers_create() {
    registers r = malloc(sizeof(registers));
    return r;
}

void registers_destroy(registers r) {
 	free(r);
}

uint8_t get_mode(registers r) {
    return r->mode;
} 

int current_mode_has_spsr(registers r) {
	if (r->mode == 0 || r->mode == 6){
    return 0;
   }
   else{
   	return 1;
   }
}

int in_a_privileged_mode(registers r) {
    if (r->mode == 0){
    return 0;
   }
   else{
   	return 1;
   }
}

uint32_t read_register(registers r, uint8_t reg) {
	return r->reg_no[reg];
}

uint32_t read_usr_register(registers r, uint8_t reg) {
    if(reg >= 0 && reg <= 12){
		return r->reg_no[reg];
	}
	else{
		return -1;
	}
}

uint32_t read_cpsr(registers r) {
    return r->reg_no[16];
}

uint32_t read_spsr(registers r) {
    return r->reg_no[17];
}

void write_register(registers r, uint8_t reg, uint32_t value) {
	r->reg_no[reg] = value;
}

void write_usr_register(registers r, uint8_t reg, uint32_t value) {
	if(reg >= 0 && reg <= 12){
		r->reg_no[reg] = value;
	}
}

void write_cpsr(registers r, uint32_t value) {
	r->reg_no[16] = value;
}

void write_spsr(registers r, uint32_t value) {
	r->reg_no[17] = value;
}
