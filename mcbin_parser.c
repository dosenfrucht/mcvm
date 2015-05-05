#include "mcbin_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

uword ubytes_to_uword(ubyte *bytes)
{
	return (((uword)bytes[0] & 0xFF) << 8) | ((uword)bytes[1] & 0xFF);
}

mcbin_file *mcbin_parser_parse(char *filename)
{
	mcbin_file *mcb = malloc(sizeof(mcbin_file));

	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		// free(mcb);
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	ubyte header[4];
	fread(header, 1, 4, f);

	if (!(header[0] == 0x26 && header[1] == 0x03
		&& header[2] == 0x20 && header[3] == 0x13)) {
		fprintf(stderr, "Invalid header in file: %s\n", filename);
		// free(mcb);
		return NULL;
	}

	size -= 4;

	ubyte size_header_raw[2] = {};

	fread(size_header_raw, 1, 2, f);

	uword size_header = ubytes_to_uword(size_header_raw);
	fread(size_header_raw, 1, 2, f); // read align bytes

	size -= 4;

	if (size_header == 0) {
		size_header = 1;
		mcb->num_headers = 1;
		mcb->headers = malloc(sizeof(uword *));
		mcb->headers[0] = malloc(sizeof(uword) * 3);
		memset(mcb->headers[0], 0, sizeof(uword) * 2);
		mcb->headers[0][2] = size;
	} else {
		mcb->num_headers = size_header;
		mcb->headers = malloc(sizeof(uword *) * size_header);
		for (uword i = 0; i < size_header; i++) {
			mcb->headers[i] = malloc(sizeof(uword) * 3);
			ubyte header_buffer[8];
			fread(header_buffer, 1, 8, f);
			mcb->headers[i][0] = ubytes_to_uword(header_buffer);
			mcb->headers[i][1] = ubytes_to_uword(header_buffer + 2);
			mcb->headers[i][2] = ubytes_to_uword(header_buffer + 4);

			/* printf("Header %d\n", i);
			printf("pos in file   %X\n", mcb->headers[i][0]);
			printf("pos in memory %X\n", mcb->headers[i][1]);
			printf("size of data  %X\n", mcb->headers[i][2]); */
		}
		size -= size_header * 8;
	}

	mcb->code = malloc(size);
	fread(mcb->code, 1, size, f);

	return mcb;
}

void mcbin_parser_free(mcbin_file *mcb)
{
	free(mcb->code);
	for (int i = 0; i < mcb->num_headers; i++) {
	 	free(mcb->headers[i]);
	}
	free(mcb->headers);
	free(mcb);
}