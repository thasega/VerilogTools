
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
		"module %s( clk, adr, data );\n"
		"\n"
		"input clk;\n"
		"input [%d:0] adr;\n"
		"\n"
		"output [3:0] data;\n"
		"\n"
		"reg [3:0] data;\n"
		"\n"
		"always @ ( posedge clk ) begin\n"
		"\tcase (adr)\n", argv[2], width-1
	);
	for ( size_t n = 0; n < size; ++n ) {
		printf("\t\t%d'h%02X: data <= 4'h%1X;\n", width, n, pbuf[n] & 0x0F );
	}
	printf(
		"\n\t\tdefault: data <= 4'h0;\n\n"
		"\tendcase\n"
		"end\n"
		"\n"
		"endmodule\n\n"
	);

	delete [] pbuf;

	return 0;
}

