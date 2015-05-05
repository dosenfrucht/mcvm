#ifndef _MCVM_STATE_H
#define _MCVM_STATE_H

typedef unsigned char ubyte;
typedef unsigned short uword;

struct mcvm_state {
	ubyte *memory;
	uword *registers;
	ubyte hlt;
};

typedef struct mcvm_state mcvm_state;

#endif