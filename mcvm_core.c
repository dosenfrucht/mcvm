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
	s->registers = malloc(16 * 2 + 3);
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
	// free(s->memory);
	// free(s->registers);
	// free(s);
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

	opcode = mcvm_mem_reg8(s, RIP);

	switch (opcode) {
	case 0x00: // mov     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_mn_mov_rr(s, b0, b1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x01: // mov     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_mov_ri(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x02: // load8   addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		mcvm_mn_load8(s, addr, b1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x03: // load16  addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		mcvm_mn_load16(s, addr, b1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x04: // store8  addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		mcvm_mn_store8_ar(s, addr, b1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x05: // store8  addr imm
		addr = mcvm_mem_get_arg0_addr(s);
		w1   = mcvm_mem_get_arg1_ai(s);
		mcvm_mn_store8_ai(s, addr, w1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x06: // store16 addr reg
		addr = mcvm_mem_get_arg0_addr(s);
		b1   = mcvm_mem_get_arg1_ar(s);
		mcvm_mn_store16_ar(s, addr, b1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x07: // store16 addr imm
		addr = mcvm_mem_get_arg0_addr(s);
		w1   = mcvm_mem_get_arg1_ai(s);
		mcvm_mn_store16_ai(s, addr, w1);
		mcvm_reg_update_rip(s, 6);
		return;
	case 0x08: // push8   reg
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_push8_r(s, b0);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x09: // push8   imm
		w0   = mcvm_mem_get_arg0_imm(s);
		mcvm_mn_push8_i(s, w0);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x0A: // push16  reg
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_push16_r(s, b0);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0B: // push16  imm
		w0   = mcvm_mem_get_arg0_imm(s);
		mcvm_mn_push16_i(s, w0);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x0C: // pop8    reg
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_pop8(s, b0);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0D: // pop16   reg
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_pop16(s, b0);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0E: // pusha
		mcvm_mn_pusha(s);
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x0F: // popa
		mcvm_mn_popa(s);
		mcvm_reg_update_rip(s, 2);
		return;





	case 0x10: // add     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_add(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x11: // add     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_add(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x12: // sub     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_sub(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x13: // sub     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_sub(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x14: // mul     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_mul(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x15: // mul     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_mul(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x16: // div     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_div(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x17: // div     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_div(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x18: // shr     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_shr(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x19: // shr     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_shr(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1A: // shl     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_shl(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1B: // shl     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_shl(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1C: // and     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_and(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1D: // and     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_and(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1E: // or      reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_or(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x1F: // or      reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_or(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x20: // xor     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		w1 = mcvm_reg_get(s, b1);
		mcvm_mn_xor(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x21: // xor     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_xor(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x22: // not     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_not(s, b0);
		mcvm_reg_update_rip(s, 2);
		return;







	case 0x28: // call    reg
		b0   = mcvm_mem_get_arg0_reg(s);
		w0   = mcvm_reg_get(s, RIP);
		mcvm_mn_push16_i(s, w0 + 2);
		mcvm_mn_jmp_r(s, b0);
		return;
	case 0x29: // call    imm
		w0   = mcvm_mem_get_arg0_imm(s);
		w1   = mcvm_reg_get(s, RIP);
		mcvm_mn_push16_i(s, w1 + 4);
		mcvm_mn_jmp_i(s, w0);
		return;
	case 0x2A: // ret
		mcvm_mn_pop16(s, RIP);
		return;
	case 0x2B: // nop
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x2C: // int     reg
		b0   = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_int_r(s, b0);
		return;
	case 0x2D: // int     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		mcvm_mn_int_i(s, w0);
		return;
	case 0x2E: // iret
		mcvm_mn_iret(s);
		return;
	case 0x2F: // hlt
		mcvm_state_set_hlt(s, 1);
		return;


	case 0x30: // cmp     reg  reg
		b0 = mcvm_mem_get_arg0_reg(s);
		b1 = mcvm_mem_get_arg1_rr(s);
		mcvm_mn_cmp_rr(s, b0, b1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x31: // cmp     reg  imm
		b0 = mcvm_mem_get_arg0_reg(s);
		w1 = mcvm_mem_get_arg1_ri(s);
		mcvm_mn_cmp_ri(s, b0, w1);
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x32: // jmp     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		mcvm_mn_jmp_r(s, b0);
		return;
	case 0x33: // jmp     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		mcvm_mn_jmp_i(s, w0);
		return;
	case 0x34: // je      reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_zero(s) == 1) {
			mcvm_mn_jmp_r(s, b0);
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x35: // je      imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_zero(s) == 1) {
			mcvm_mn_jmp_i(s, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x36: // jne     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_zero(s) == 0) {
			mcvm_mn_jmp_r(s, b0);
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x37: // jne     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_zero(s) == 0) {
			mcvm_mn_jmp_i(s, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x38: // jg      reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_zero(s) == 0
			&& mcvm_reg_get_flag_sign(s) == 0) {
			mcvm_mn_jmp_r(s, b0);
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x39: // jg      imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_zero(s) == 0
			&& mcvm_reg_get_flag_sign(s) == 0) {
			mcvm_mn_jmp_i(s, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x3A: // jge     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_sign(s) == 0) {
			mcvm_mn_jmp_r(s, b0);
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x3B: // jge     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_sign(s) == 0) {
			mcvm_mn_jmp_i(s, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x3C: // jl      reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_sign(s) == 1) {
			mcvm_mn_jmp_r(s, b0);
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x3D: // jl      imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_sign(s) == 1) {
			mcvm_mn_jmp_i(s, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	case 0x3E: // jle     reg
		b0 = mcvm_mem_get_arg0_reg(s);
		if (mcvm_reg_get_flag_sign(s) == 1
			|| mcvm_reg_get_flag_zero(s) == 1) {
			mcvm_mn_jmp_r(s, b0);
			return;
		}
		mcvm_reg_update_rip(s, 2);
		return;
	case 0x3F: // jle     imm
		w0 = mcvm_mem_get_arg0_imm(s);
		if (mcvm_reg_get_flag_sign(s) == 1
			|| mcvm_reg_get_flag_zero(s) == 1) {
			mcvm_mn_jmp_i(s, w0);
			return;
		}
		mcvm_reg_update_rip(s, 4);
		return;
	default:
		fprintf(stderr, "unknown opcode: %X\n", opcode);
		_Exit(-1);
		mcvm_reg_update_rip(s, 2);
		return;
	}
}