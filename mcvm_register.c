#include "mcvm_register.h"

uword mcvm_reg_get(mcvm_state *s, ubyte reg)
{
	return s->registers[reg];
}

void mcvm_reg_set(mcvm_state *s, ubyte reg, uword val)
{
	s->registers[reg] = val;
}

void mcvm_reg_update_rip(mcvm_state *s, uword val)
{
	mcvm_reg_set(s, RIP, mcvm_reg_get(s, RIP) + val);
}

void mcvm_reg_set_flag_zero(mcvm_state *s, ubyte val)
{
	uword flags = mcvm_reg_get(s, RFLAGS);
	uword new_flags = (flags & 0xFFFE) | (val & 0x0001);
	mcvm_reg_set(s, RFLAGS, new_flags);
}

void mcvm_reg_set_flag_sign(mcvm_state *s, ubyte val)
{
	uword flags = mcvm_reg_get(s, RFLAGS);
	uword new_flags = (flags & 0xFFFD) | ((val & 0x0001) << 1);
	mcvm_reg_set(s, RFLAGS, new_flags);
}

ubyte mcvm_reg_get_flag_zero(mcvm_state *s)
{
	return mcvm_reg_get(s, RFLAGS) & 0x0001;
}

ubyte mcvm_reg_get_flag_sign(mcvm_state *s)
{
	return (mcvm_reg_get(s, RFLAGS) & 0x0002) >> 1;
}