
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned char *fileReadMalloc( const char *filename, size_t & size )
{
	FILE *fh = fopen( filename, "rb" );
	if ( !fh ) {
		size = 0;
		return NULL;
	}
	fseek( fh, 0, SEEK_END );
	size = ftell(fh);
	fseek( fh, 0, SEEK_SET );
	unsigned char *buf = new unsigned char [size];
	fread( buf, 1, size, fh );
	fclose( fh );

	return buf;
}

static int getWidth( unsigned int n )
{
	for ( int i = 31; i >= 0; --i ) if ( (1<<i) & n ) return i+1;
	return 0;
}



int main( int argc, char *argv[] )
{
	if ( argc < 3 ) {
		printf("usage: %s filename modulename\n", argv[0] );
		return -1;
	}

	size_t size;
	unsigned char *pbuf = fileReadMalloc( argv[1], size );
	if ( !size ) {
		fprintf( stderr, "FATAL: file '%s' does not found.\n", argv[1] );
		return -1;
	}

	int width = getWidth( size-1 );

	printf(
		"module %s\n"
		"(\n"
		"\tinput clk,\n"
		"\tinput [%d:0] adr,\n"
		"\toutput [7:0] dat\n"
		");\n"
		"\n"
		"function [7:0] %s_rom;\n"
		"input [%d:0] ad;\n"
		"begin\n"
		"\tcase (ad)\n", argv[2], width-1, argv[2], width-1
	);
	for ( size_t n = 0; n < size; ++n ) {
		printf("\t\t%d'h%02X: %s_rom = 8'h%02X;\n", width, n, argv[2], pbuf[n] );
	}
	printf(
		"\n\t\tdefault: %s_rom = 8'h00;\n\n"
		"\tendcase\n"
		"end\n"
		"endfunction\n"
		"\n"
		"reg [7:0] dat; always @ ( posedge clk ) dat <= %s_rom(adr);\n"
		"//assign dat = %s_rom(adr);\n"
		"\n"
		"endmodule\n\n", argv[2], argv[2], argv[2]
	);

	delete [] pbuf;

	return 0;
}

