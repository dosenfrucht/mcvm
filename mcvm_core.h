#ifndef _MCVM_CORE_H
#define _MCVM_CORE_H

#include "mcvm_state.h"
#include "mcvm_mnemonics.h"

mcvm_state *mcvm_state_new();
void mcvm_state_delete(mcvm_state *s);
void mcvm_state_set_hlt(mcvm_state *s, ubyte hlt);
ubyte mcvm_state_get_hlt(mcvm_state *s);

void mcvm_run(mcvm_state *s);
void mcvm_execute(mcvm_state *s);

#endif