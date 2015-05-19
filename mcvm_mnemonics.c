#include "mcvm_mnemonics.h"

#include <memory.h>

// Memory Management

void mcvm_mn_mov_rr(mcvm_state *s, mcvm_register r0, mcvm_register r1)
{
	uword val = mcvm_reg_get(s, r1);
	mcvm_reg_set(s, r0, val);
}

void mcvm_mn_mov_ri(mcvm_state *s, mcvm_register r0, uword val)
{
	mcvm_reg_set(s, r0, val);
}

void mcvm_mn_load8(mcvm_state *s, mcvm_addr addr, mcvm_register r0)
{
	uword val = 0;
	switch (addr.type) {
	case MCVM_ADDR_REG:
		val = mcvm_mem_reg8(s, addr.reg) & 0xFF;
		break;
	case MCVM_ADDR_IMM:
		val = mcvm_mem_imm8(s, addr.imm) & 0xFF;
		break;
	}
	mcvm_reg_set(s, r0, val);
}

void mcvm_mn_load16(mcvm_state *s, mcvm_addr addr, mcvm_register r0)
{
	uword val = 0;
	switch (addr.type) {
	case MCVM_ADDR_REG:
		val = mcvm_mem_reg16(s, addr.reg);
		break;
	case MCVM_ADDR_IMM:
		val = mcvm_mem_imm16(s, addr.imm);
		break;
	}
	mcvm_reg_set(s, r0, val);
}

void mcvm_mn_store8_ar(mcvm_state *s, mcvm_addr addr, mcvm_register r1)
{
	uword eaddr = 0;
	switch (addr.type) {
	case MCVM_ADDR_REG:
		eaddr = mcvm_reg_get(s, addr.reg);
		break;
	case MCVM_ADDR_IMM:
		eaddr = addr.imm;
		break;
	}
	ubyte val = mcvm_reg_get(s, r1) & 0xFF;
	mcvm_mem_set8(s, eaddr, val);
}

void mcvm_mn_store8_ai(mcvm_state *s, mcvm_addr addr, ubyte val)
{
	uword eaddr = 0;
	switch (addr.type) {
	case MCVM_ADDR_REG:
		eaddr = mcvm_reg_get(s, addr.reg);
		break;
	case MCVM_ADDR_IMM:
		eaddr = addr.imm;
		break;
	}
	mcvm_mem_set8(s, eaddr, val);
}

void mcvm_mn_store16_ar(mcvm_state *s, mcvm_addr addr, mcvm_register r1)
{
	uword eaddr = 0;
	switch (addr.type) {
	case MCVM_ADDR_REG:
		eaddr = mcvm_reg_get(s, addr.reg);
		break;
	case MCVM_ADDR_IMM:
		eaddr = addr.imm;
		break;
	}
	uword val = mcvm_reg_get(s, r1);
	mcvm_mem_set16(s, eaddr, val);
}

void mcvm_mn_store16_ai(mcvm_state *s, mcvm_addr addr, uword val)
{
	uword eaddr = 0;
	switch (addr.type) {
	case MCVM_ADDR_REG:
		eaddr = mcvm_reg_get(s, addr.reg);
		break;
	case MCVM_ADDR_IMM:
		eaddr = addr.imm;
		break;
	}
	mcvm_mem_set16(s, eaddr, val);
}

void mcvm_mn_push8_r(mcvm_state *s, mcvm_register r)
{
	ubyte val = mcvm_reg_get(s, r) & 0xFF;
	uword new_rsp = mcvm_reg_get(s, RSP) - 1;
	mcvm_mem_set8(s, new_rsp, val);
	mcvm_reg_set(s, RSP, new_rsp);
}

void mcvm_mn_push8_i(mcvm_state *s, ubyte val)
{
	uword new_rsp = mcvm_reg_get(s, RSP) - 1;
	mcvm_mem_set8(s, new_rsp, val);
	mcvm_reg_set(s, RSP, new_rsp);
}

void mcvm_mn_push16_r(mcvm_state *s, mcvm_register r)
{
	uword val = mcvm_reg_get(s, r);
	uword new_rsp = mcvm_reg_get(s, RSP) - 1;
	mcvm_mem_set16(s, new_rsp, val);
	mcvm_reg_set(s, RSP, new_rsp - 1);
}

void mcvm_mn_push16_i(mcvm_state *s, uword val)
{
	uword new_rsp = mcvm_reg_get(s, RSP) - 1;
	mcvm_mem_set16(s, new_rsp, val);
	mcvm_reg_set(s, RSP, new_rsp - 1);
}

void mcvm_mn_pop8(mcvm_state *s, mcvm_register r)
{
	uword eaddr = mcvm_reg_get(s, RSP);
	ubyte val = mcvm_mem_imm8(s, eaddr);
	mcvm_reg_set(s, r, val);
	mcvm_reg_set(s, RSP, eaddr + 1);
}

void mcvm_mn_pop16(mcvm_state *s, mcvm_register r)
{
	uword eaddr = mcvm_reg_get(s, RSP);
	uword val = mcvm_mem_imm16(s, eaddr + 1);
	mcvm_reg_set(s, r, val);
	mcvm_reg_set(s, RSP, eaddr + 2);
}

void mcvm_mn_pusha(mcvm_state *s)
{
	uword rsp   = mcvm_reg_get(s, RSP);
	ubyte *dest = s->memory + (rsp - (16 * 2));
	uword *src  = s->registers;
	memcpy(dest, src, 16 * 2);
	mcvm_reg_set(s, RSP, rsp - (16 * 2));
}

void mcvm_mn_popa(mcvm_state *s)
{
	uword rsp  = mcvm_reg_get(s, RSP);
	ubyte *src  = s->memory + rsp;
	uword *dest = s->registers;
	memcpy(dest, src, 16 * 2);
	mcvm_reg_set(s, RSP, rsp + (16 * 2));
}






// Arith

void mcvm_mn_add(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg + val);
}

void mcvm_mn_sub(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg - val);
}

void mcvm_mn_mul(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg * val);
}

void mcvm_mn_div(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg / val);
	mcvm_reg_set(s, RDX, curr_reg % val);
}

void mcvm_mn_shr(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg << val);
}

void mcvm_mn_shl(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg >> val);
}

void mcvm_mn_and(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg & val);
}

void mcvm_mn_or(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg | val);
}

void mcvm_mn_xor(mcvm_state *s, mcvm_register r, uword val)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, curr_reg ^ val);
}

void mcvm_mn_not(mcvm_state *s, mcvm_register r)
{
	uword curr_reg = mcvm_reg_get(s, r);
	mcvm_reg_set(s, r, ~curr_reg);
}



// Misc

void mcvm_mn_int_r(mcvm_state *s, mcvm_register r)
{
	uword rip = mcvm_reg_get(s, RIP);
	uword code = mcvm_reg_get(s, r);
	mcvm_reg_set(s, RINTRET, rip + 2);

	if (mcvm_int(s, code) == MCVM_INT_HANDLED) {
		mcvm_reg_set(s, RIP, rip + 2);
		return;
	}

	uword handler_addr = mcvm_mem_imm16(s, 0x0001 + code * 2);
	mcvm_reg_set(s, RIP, handler_addr);
}

void mcvm_mn_int_i(mcvm_state *s, uword code)
{
	uword rip = mcvm_reg_get(s, RIP);
	mcvm_reg_set(s, RINTRET, rip + 4);

	if (mcvm_int(s, code) == MCVM_INT_HANDLED) {
		mcvm_reg_set(s, RIP, rip + 4);
		return;
	}

	uword handler_addr = mcvm_mem_imm16(s, 0x0001 + code * 2);
	mcvm_reg_set(s, RIP, handler_addr);
}

void mcvm_mn_iret(mcvm_state *s)
{
	uword ret_addr = mcvm_reg_get(s, RINTRET);
	mcvm_reg_set(s, RIP, ret_addr);
}





// Control

void mcvm_mn_cmp_rr(mcvm_state *s, mcvm_register r0, mcvm_register r1)
{
	uword v0 = mcvm_reg_get(s, r0);
	uword v1 = mcvm_reg_get(s, r1);
	uword val = v0 - v1;
	mcvm_reg_set_flag_sign(s, val < 0);
	mcvm_reg_set_flag_zero(s, val == 0);
}

void mcvm_mn_cmp_ri(mcvm_state *s, mcvm_register r0, uword v1)
{
	uword v0 = mcvm_reg_get(s, r0);
	uword val = v0 - v1;
	mcvm_reg_set_flag_sign(s, val < 0);
	mcvm_reg_set_flag_zero(s, val == 0);
}

void mcvm_mn_jmp_r(mcvm_state *s, mcvm_register r)
{
	uword eaddr = mcvm_reg_get(s, r);
	mcvm_reg_set(s, RIP, eaddr);
}

void mcvm_mn_jmp_i(mcvm_state *s, uword val)
{
	mcvm_reg_set(s, RIP, val);
}