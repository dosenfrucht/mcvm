#ifndef _MCVM_MNEMONICS_H
#define _MCVM_MNEMONICS_H

#include "mcvm_core.h"
#include "mcvm_register.h"
#include "mcvm_memory.h"
#include "mcvm_int.h"


// Memory Management
void mcvm_mn_mov_rr(mcvm_state *s, mcvm_register r0, mcvm_register r1);
void mcvm_mn_mov_ri(mcvm_state *s, mcvm_register r0, uword val);
void mcvm_mn_load8(mcvm_state *s, mcvm_addr addr, mcvm_register r0);
void mcvm_mn_load16(mcvm_state *s, mcvm_addr addr, mcvm_register r0);
void mcvm_mn_store8_ar(mcvm_state *s, mcvm_addr addr, mcvm_register r1);
void mcvm_mn_store8_ai(mcvm_state *s, mcvm_addr addr, ubyte val);
void mcvm_mn_store16_ar(mcvm_state *s, mcvm_addr addr, mcvm_register r1);
void mcvm_mn_store16_ai(mcvm_state *s, mcvm_addr addr, uword val);
void mcvm_mn_push8_r(mcvm_state *s, mcvm_register r);
void mcvm_mn_push8_i(mcvm_state *s, ubyte val);
void mcvm_mn_push16_r(mcvm_state *s, mcvm_register r);
void mcvm_mn_push16_i(mcvm_state *s, uword val);
void mcvm_mn_pop8(mcvm_state *s, mcvm_register r);
void mcvm_mn_pop16(mcvm_state *s, mcvm_register r);
void mcvm_mn_pusha(mcvm_state *s);
void mcvm_mn_popa(mcvm_state *s);


// Arith
void mcvm_mn_add(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_sub(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_mul(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_div(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_shr(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_shl(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_and(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_or (mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_xor(mcvm_state *s, mcvm_register r, uword val);
void mcvm_mn_not(mcvm_state *s, mcvm_register r);


// Misc
void mcvm_mn_int_r(mcvm_state *s, mcvm_register r);
void mcvm_mn_int_i(mcvm_state *s, uword code);
void mcvm_mn_iret(mcvm_state *s);


// Control
void mcvm_mn_cmp_rr(mcvm_state *s, mcvm_register r0, mcvm_register r1);
void mcvm_mn_cmp_ri(mcvm_state *s, mcvm_register r0, uword v1);
void mcvm_mn_jmp_r(mcvm_state *s, mcvm_register r);
void mcvm_mn_jmp_i(mcvm_state *s, uword val);


#endif