#include "mcvm_state.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "mcvm_core.h"
#include "mcvm_memory.h"
#include "mcvm_register.h"

mcvm_state *mcvm_state_new()
{
	mcvm_state *s = malloc(sizeof(mcvm_state));
	s->memory = malloc(1 << 16);
	if (s->memory == NULL) {
		fprintf(stderr, "Not enough memory available\n");
		exit(-1);
	}
	s->registers = malloc(16 * 2 + 2);
	if (s->registers == NULL) {
		fprintf(stderr, "Not enough memory available\n");
		exit(-1);
	}
	s->hlt = 0;
	return s;
}

void mcvm_state_delete(mcvm_state *s)
{
	// Bug? OS says double free..
	//free(s->memory);
	//free(s->registers);
	//free(s);
}

void mcvm_state_set_hlt(mcvm_state *s, ubyte hlt)
{
	s->hlt = hlt;
}

ubyte mcvm_state_get_hlt(mcvm_state *s)
{
	return s->hlt;
}

void mcvm_run(mcvm_state *s)
{
	while (!mcvm_state_get_hlt(s)) {
		mcvm_execute(s);
	}
}

void mcvm_execute(mcvm_state *s)
{
	static ubyte opcode = 0;
	static uword w0 = 0;
	static uword w1 = 0;
	static ubyte b0 = 0;
	static ubyte b1 = 0;
	static mcvm_addr addr;

	static ubyte btmp = 0;
	static uword wtmp = 0;

	opcode = mcvm_mem_reg8(s, RIP);

	switch (opcode) {
	case 0x00: // mov     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x01: // mov     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x02: // load8   addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		switch (addr.type) {
		case MCVM_ADDR_REG:
			btmp = mcvm_mem_reg8(s, addr.reg);
			break;
		case MCVM_ADDR_IMM:
			btmp = mcvm_mem_imm8(s, addr.imm);
			break;
		}
		mcvm_reg_set(s, b1, btmp);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x03: // load16  addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		switch (addr.type) {
		case MCVM_ADDR_REG:
			wtmp = mcvm_mem_reg16(s, addr.reg);
			break;
		case MCVM_ADDR_IMM:
			wtmp = mcvm_mem_imm16(s, addr.imm);
			break;
		}
		mcvm_reg_set(s, b1, wtmp);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x04: // store8  addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		switch (addr.type) {
		case MCVM_ADDR_REG:
			wtmp = mcvm_reg_get(s, addr.reg);
			break;
		case MCVM_ADDR_IMM:
			wtmp = addr.imm;
			break;
		}
		mcvm_mem_set8(s, wtmp, (ubyte) (mcvm_reg_get(s, b1) & 0xFF));
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x05: // store8  addr imm
		addr = mcvm_mem_get_arg0_addr(s);
		w1   = mcvm_mem_get_arg1_ai(s);
		switch (addr.type) {
		case MCVM_ADDR_REG:
			wtmp = mcvm_reg_get(s, addr.reg);
			break;
		case MCVM_ADDR_IMM:
			wtmp = addr.imm;
			break;
		}
		mcvm_mem_set8(s, wtmp, (ubyte) (w1 & 0xFF));
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x06: // store16 addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		switch (addr.type) {
		case MCVM_ADDR_REG:
			wtmp = mcvm_reg_get(s, addr.reg);
			break;
		case MCVM_ADDR_IMM:
			wtmp = addr.imm;
			break;
		}
		mcvm_mem_set16(s, wtmp, mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x07: // store16 addr imm
		addr = mcvm_mem_get_arg0_addr(s);
		w1   = mcvm_mem_get_arg1_ai(s);
		switch (addr.type) {
		case MCVM_ADDR_REG:
			wtmp = mcvm_reg_get(s, addr.reg);
			break;
		case MCVM_ADDR_IMM:
			wtmp = addr.imm;
			break;
		}
		mcvm_mem_set16(s, wtmp, w1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x08: // push8   reg
		wtmp = mcvm_reg_get(s, RSP) - 1;
		b0   = mcvm_mem_get_arg0_reg(s);
		btmp = (ubyte) (mcvm_reg_get(s, b0) & 0xFF);
		mcvm_mem_set8(s, wtmp, btmp);
		mcvm_reg_set(s, RSP, wtmp);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x09: // push8   imm
		wtmp = mcvm_reg_get(s, RSP) - 1;
		w0   = mcvm_mem_get_arg0_imm(s);
		btmp = (ubyte) (w0 & 0xFF);
		mcvm_mem_set8(s, wtmp, btmp);
		mcvm_reg_set(s, RSP, wtmp);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x0A: // push16  reg
		wtmp = mcvm_reg_get(s, RSP) - 2;
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mem_set16(s, wtmp, mcvm_reg_get(s, b0));
		mcvm_reg_set(s, RSP, wtmp);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0B: // push16  imm
		wtmp = mcvm_reg_get(s, RSP) - 2;
		w0   = mcvm_mem_get_arg0_imm(s);
		mcvm_mem_set16(s, wtmp, w0);
		mcvm_reg_set(s, RSP, wtmp);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x0C: // pop8    reg
		wtmp = mcvm_reg_get(s, RSP);
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_reg_set(s, b0, mcvm_mem_imm8(s, wtmp));
		mcvm_reg_set(s, RSP, wtmp + 2);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0D: // pop16   reg
		wtmp = mcvm_reg_get(s, RSP);
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_reg_set(s, b0, mcvm_mem_imm16(s, wtmp));
		mcvm_reg_set(s, RSP, wtmp + 2);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0E: // pusha
		wtmp = mcvm_reg_get(s, RSP);
		for (int i = 0; i < 16; i++) {
			mcvm_mem_set16(s, wtmp - (2 + 2 * i),
				mcvm_reg_get(s, i));
		}
		mcvm_reg_set(s, RSP, wtmp - 2 * 16);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0F: // popa
		wtmp = mcvm_reg_get(s, RSP);
		for (int i = 0, j = 15; i < 16; i++, j--) {
			mcvm_reg_set(s, j, mcvm_mem_imm16(s, wtmp + 2 * i));
		}
		mcvm_reg_set(s, RSP, wtmp + 2 * 16);
		mcvm_reg_update_rip(s, 2);
		return;

	case 0x10: // add     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) + mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x11: // add     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) + w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x12: // sub     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) - mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x13: // sub     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) - w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x14: // mul     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) * mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x15: // mul     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) * w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x16: // div     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, RAX, mcvm_reg_get(s, b0) / mcvm_reg_get(s, b1));
		mcvm_reg_set(s, RDX, mcvm_reg_get(s, b0) % mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x17: // div     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, RAX, mcvm_reg_get(s, b0) / w1);
		mcvm_reg_set(s, RDX, mcvm_reg_get(s, b0) % w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x18: // shr     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) >> mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x19: // shr     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) >> w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1A: // shl     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) << mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1B: // shl     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) << w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1C: // and     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) & mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1D: // and     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) & w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1E: // or      reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) | mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1F: // or      reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) | w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x20: // xor     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) ^ mcvm_reg_get(s, b1));
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x21: // xor     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_reg_set(s, b0, mcvm_reg_get(s, b0) ^ w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x22: // not     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		mcvm_reg_set(s, b0, ~mcvm_reg_get(s, b0));
		mcvm_reg_update_rip(s, 2);
		return;

	case 0x28: // call    reg
		wtmp = mcvm_reg_get(s, RSP) - 2;
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mem_set16(s, wtmp, mcvm_reg_get(s, RIP) + 2);
		mcvm_reg_set(s, RSP, wtmp);
		mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
		return;
	case 0x29: // call    imm
		wtmp = mcvm_reg_get(s, RSP) - 2;
		w0   = mcvm_mem_get_arg0_imm(s);
		mcvm_mem_set16(s, wtmp, mcvm_reg_get(s, RIP) + 4);
		mcvm_reg_set(s, RSP, wtmp);
		mcvm_reg_set(s, RIP, w0);
		return;
	case 0x2A: // ret
		wtmp = mcvm_reg_get(s, RSP);
		mcvm_reg_set(s, RIP, mcvm_mem_imm16(s, wtmp));
		mcvm_reg_set(s, RSP, wtmp + 2);
		return;
	case 0x2B: // nop
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x2C: // int     reg
		mcvm_reg_update_rip(s, 2);
		fprintf(stderr, "int reg not implemented.\n");
		return;
	case 0x2D: // int     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (w0 == 26) {
			printf("%c", mcvm_reg_get(s, RAX));
			mcvm_reg_update_rip(s, 4);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		fprintf(stderr, "int imm not implemented.\n");
		return;
	case 0x2E: // iret
		mcvm_reg_update_rip(s, 2);
		fprintf(stderr, "iret not implemented.\n");
		return;
	case 0x2F: // hlt
		mcvm_state_set_hlt(s, 1);
		return;


	case 0x30: // cmp     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		wtmp = mcvm_reg_get(s, b0) - mcvm_reg_get(s, b1);
		mcvm_reg_set_flag_sign(s, wtmp < 0);
		mcvm_reg_set_flag_zero(s, wtmp == 0);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x31: // cmp     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		wtmp = mcvm_reg_get(s, b0) - w1;
		mcvm_reg_set_flag_sign(s, wtmp < 0);
		mcvm_reg_set_flag_zero(s, wtmp == 0);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x32: // jmp     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
		return;
	case 0x33: // jmp     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		mcvm_reg_set(s, RIP, w0);
		return;
	case 0x34: // je      reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x35: // je      imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x36: // jne     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (!mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x37: // jne     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (!mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x38: // jg      reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (!mcvm_reg_get_flag_sign(s) && !mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x39: // jg      imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (!mcvm_reg_get_flag_sign(s) && !mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x3A: // jge     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (!mcvm_reg_get_flag_sign(s)) {
			mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x3B: // jge     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (!mcvm_reg_get_flag_sign(s)) {
			mcvm_reg_set(s, RIP, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x3C: // jl      reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_sign(s)) {
			mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x3D: // jl      imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_sign(s)) {
			mcvm_reg_set(s, RIP, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x3E: // jle     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_sign(s) || mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, mcvm_reg_get(s, b0));
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x3F: // jle     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_sign(s) || mcvm_reg_get_flag_zero(s)) {
			mcvm_reg_set(s, RIP, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	default:
		mcvm_reg_update_rip(s, 1);
		return;
	}
}