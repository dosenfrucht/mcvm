#include <stdio.h>

#include "mcbin_parser.h"

#include "mcvm_state.h"
#include "mcvm_core.h"
#include "mcvm_register.h"
#include "mcvm_memory.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "No input.\n");
		return -1;
	}
	mcvm_state *s = mcvm_state_new();

	mcbin_file *f = mcbin_parser_parse(argv[1]);
	if (f == NULL) {
		fprintf(stderr, "Could not open file: %s\n", argv[1]);
		return -1;
	}

	for (int i = 0; i < f->num_headers; i++) {
		mcvm_mem_write(s, f->headers[i][1], f->code + f->headers[i][0],
				f->headers[i][2]);
	}

	mcbin_parser_free(f);

	mcvm_reg_set(s, RIP, 0);
	mcvm_reg_set(s, RSP, 0x7DFE);

	mcvm_run(s);

	mcvm_state_delete(s);
	return 0;
}
