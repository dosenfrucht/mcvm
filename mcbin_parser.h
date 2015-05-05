#ifndef _MCBIN_PARSER_H
#define _MCBIN_PARSER_H

#include "mcvm_state.h"

struct mcbin_file {
	uword num_headers;
	uword **headers;
	ubyte *code;
};

typedef struct mcbin_file mcbin_file;

mcbin_file *mcbin_parser_parse(char *filename);

void mcbin_parser_free(mcbin_file *mcb);

#endif