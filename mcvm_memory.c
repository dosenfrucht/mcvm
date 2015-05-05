#include "mcvm_memory.h"

#include <memory.h>
#include "mcvm_register.h"

void mcvm_mem_write(mcvm_state *s,
	uword addr,
	ubyte *from,
	uword size)
{
	memcpy(&s->memory[addr], from, size);
}

void mcvm_mem_read(mcvm_state *s,
	uword addr,
	ubyte *to,
	uword size)
{
	memcpy(to, &s->memory[addr], size);
}

uword mcvm_mem_reg16(mcvm_state *s, ubyte reg)
{
	ubyte higher = s->memory[mcvm_reg_get(s, reg)];
	ubyte lower = s->memory[mcvm_reg_get(s, reg) + 1];
	return ((uword)higher << 8) | (uword)lower;
}

ubyte mcvm_mem_reg8(mcvm_state *s, ubyte reg)
{
	return s->memory[mcvm_reg_get(s, reg)];
}

uword mcvm_mem_imm16(mcvm_state *s, uword addr)
{
	ubyte higher = s->memory[addr];
	ubyte lower = s->memory[addr + 1];
	return ((uword)higher << 8) | (uword) lower;
}

ubyte mcvm_mem_imm8(mcvm_state *s, uword addr)
{
	return s->memory[addr];
}

void mcvm_mem_set8(mcvm_state *s, uword addr, ubyte val)
{
	s->memory[addr] = val;
}

void mcvm_mem_set16(mcvm_state *s, uword addr, uword val)
{
	ubyte higher = (val & 0xFF00) >> 8;
	ubyte lower  = (val & 0x00FF);
	s->memory[addr] = higher;
	s->memory[addr + 1] = lower;
}


ubyte mcvm_mem_get_arg0_reg(mcvm_state *s)
{
	return s->memory[mcvm_reg_get(s, RIP) + 1];
}

uword mcvm_mem_get_arg0_imm(mcvm_state *s)
{
	ubyte higher = s->memory[mcvm_reg_get(s, RIP) + 2];
	ubyte lower = s->memory[mcvm_reg_get(s, RIP) + 3];
	return ((uword)higher << 8) | (uword)lower;
}

mcvm_addr mcvm_mem_get_arg0_addr(mcvm_state *s)
{
	mcvm_addr addr;
	ubyte type   = s->memory[mcvm_reg_get(s, RIP) + 1];
	ubyte higher = s->memory[mcvm_reg_get(s, RIP) + 2];
	ubyte lower  = s->memory[mcvm_reg_get(s, RIP) + 3];
	if ((type & 0x80) == 0x80) {
		addr.type = MCVM_ADDR_REG;
		addr.reg  = type & 0x0F;
	} else {
		addr.type = MCVM_ADDR_IMM;
		addr.imm  = ((uword)higher << 8) | (uword)lower;
	}
	return addr;
}

ubyte mcvm_mem_get_arg1_rr(mcvm_state *s)
{
	return s->memory[mcvm_reg_get(s, RIP) + 3];
}

uword mcvm_mem_get_arg1_ri(mcvm_state *s)
{
	return mcvm_mem_get_arg0_imm(s);
}

ubyte mcvm_mem_get_arg1_ar(mcvm_state *s)
{
	return s->memory[mcvm_reg_get(s, RIP) + 5];
}

uword mcvm_mem_get_arg1_ai(mcvm_state *s)
{
	ubyte higher = s->memory[mcvm_reg_get(s, RIP) + 4];
	ubyte lower = s->memory[mcvm_reg_get(s, RIP) + 5];
	return ((uword)higher << 8) | (uword)lower;
}