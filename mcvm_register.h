#ifndef _MCVM_REGISTER_H
#define _MCVM_REGISTER_H

#include "mcvm_state.h"

enum mcvm_register {
	RAX = 0,
	RBX = 1,
	RCX = 2,
	RDX = 3,
	RSI = 4,
	RDI = 5,
	RBP = 6,
	RSP = 7,
	R8  = 8,
	R9  = 9,
	R10 = 10,
	R11 = 11,
	R12 = 12,
	R13 = 13,
	R14 = 14,
	R15 = 15,
	RFLAGS = 16,
	RIP = 17,
	RINTRET = 18
};

typedef enum mcvm_register mcvm_register;

uword mcvm_reg_get(mcvm_state *s, ubyte reg);
void mcvm_reg_set(mcvm_state *s, ubyte reg, uword val);

void mcvm_reg_update_rip(mcvm_state *s, uword val);

void mcvm_reg_set_flag_zero(mcvm_state *s, ubyte val);
void mcvm_reg_set_flag_sign(mcvm_state *s, ubyte val);

ubyte mcvm_reg_get_flag_zero(mcvm_state *s);
ubyte mcvm_reg_get_flag_sign(mcvm_state *s);

#endif