#ifndef _MCVM_MEMORY_H
#define _MCVM_MEMORY_H

#include "mcvm_state.h"

enum mcvm_addr_type {
	MCVM_ADDR_REG,
	MCVM_ADDR_IMM
};

struct mcvm_addr {
	enum mcvm_addr_type type;
	union {
		ubyte reg;
		uword imm;
	};
};

typedef struct mcvm_addr mcvm_addr;

void mcvm_mem_write(mcvm_state *s,
	uword addr,
	ubyte *from,
	uword size);

void mcvm_mem_read(mcvm_state *s,
	uword addr,
	ubyte *to,
	uword size);

uword mcvm_mem_reg16(mcvm_state *s, ubyte reg);
ubyte mcvm_mem_reg8(mcvm_state *s, ubyte reg);
uword mcvm_mem_imm16(mcvm_state *s, uword addr);
ubyte mcvm_mem_imm8(mcvm_state *s, uword addr);

void mcvm_mem_set8(mcvm_state *s, uword addr, ubyte val);
void mcvm_mem_set16(mcvm_state *s, uword addr, uword val);


ubyte mcvm_mem_get_arg0_reg(mcvm_state *s);
uword mcvm_mem_get_arg0_imm(mcvm_state *s);
mcvm_addr mcvm_mem_get_arg0_addr(mcvm_state *s);
ubyte mcvm_mem_get_arg1_rr(mcvm_state *s);
uword mcvm_mem_get_arg1_ri(mcvm_state *s);
ubyte mcvm_mem_get_arg1_ar(mcvm_state *s);
uword mcvm_mem_get_arg1_ai(mcvm_state *s);


#endif