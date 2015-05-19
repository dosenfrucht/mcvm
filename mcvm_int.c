#include "mcvm_int.h"

#include <stdio.h>

mcvm_int_status mcvm_int(mcvm_state *s, uword code)
{
	switch (code) {
	case 26:
		putchar(mcvm_reg_get(s, RAX));
		return MCVM_INT_HANDLED;
	case 21:
		printf("%d", mcvm_reg_get(s, RAX));
		return MCVM_INT_HANDLED;
	default:
		return MCVM_INT_NOT_HANDLED;
	}
	return MCVM_INT_NOT_HANDLED;
}