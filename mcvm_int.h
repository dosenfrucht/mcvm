#ifndef _MCVM_INT_H
#define _MCVM_INT_H

#include "mcvm_core.h"

enum mcvm_int_status {
	MCVM_INT_HANDLED,
	MCVM_INT_NOT_HANDLED
};

typedef enum mcvm_int_status mcvm_int_status;

mcvm_int_status mcvm_int(mcvm_state *s, uword code);

#endif