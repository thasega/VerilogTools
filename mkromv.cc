
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


//====================================================================================
static int normal(int argc, char *argv[])
{
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
		"output [7:0] data;\n"
		"\n"
		"reg [7:0] data;\n"
		"\n"
		"always @ ( posedge clk ) begin\n"
		"\tcase (adr)\n", argv[2], width-1
	);
	for ( size_t n = 0; n < size; ++n ) {
		printf("\t\t%d'h%02X: data <= 8'h%02X;\n", width, n, pbuf[n] );
	}
	printf(
		"\n\t\tdefault: data <= 8'h00;\n\n"
		"\tendcase\n"
		"end\n"
		"\n"
		"endmodule\n\n"
	);

	delete [] pbuf;
	return 0;
}
//====================================================================================
static int max10(int argc, char *argv[])
{
	size_t size;
	unsigned char *qbuf = fileReadMalloc( argv[1], size );
	if ( !size ) {
		fprintf( stderr, "FATAL: file '%s' does not found.\n", argv[1] );
		return -1;
	}

	int width = getWidth( size-1 );

	size_t osize = size;
	size = 1<<width;
	unsigned char *pbuf = new unsigned char [size];
	memset( pbuf, 0, size );
	memcpy( pbuf, qbuf, osize );
	delete [] qbuf;

	char *modname = argv[2];
	char outsrc[256], outmif[256];
	sprintf( outsrc, "%s.v",   modname );
	sprintf( outmif, "%s.mif", modname );

	FILE *fho = fopen( outsrc, "w" );
	fprintf( fho,
		"/* Generated Automatically by mkromv (max10 mode) */\n\n"
		"// synopsys translate_on\n"
		"\n"
		"module %s\n"
		"(\n"
		"\tinput   tri1 clock,\n"
		"\tinput [%d:0] address,\n"
		"\toutput [7:0] q\n"
		");\n"
		"\taltsyncram altsyncram_component (\n"
		"\t\t.address_a (address),\n"
		"\t\t.clock0 (clock),\n"
		"\t\t.q_a(q),\n"
		"\t\t.aclr0 (1'b0),\n"
		"\t\t.aclr1 (1'b0),\n"
		"\t\t.address_b (1'b1),\n"
		"\t\t.addressstall_a (1'b0),\n"
		"\t\t.addressstall_b (1'b0),\n"
		"\t\t.byteena_a (1'b1),\n"
		"\t\t.byteena_b (1'b1),\n"
		"\t\t.clock1 (1'b1),\n"
		"\t\t.clocken0 (1'b1),\n"
		"\t\t.clocken1 (1'b1),\n"
		"\t\t.clocken2 (1'b1),\n"
		"\t\t.clocken3 (1'b1),\n"
		"\t\t.data_a ({8{1'b1}}),\n"
		"\t\t.data_b (1'b1),\n"
		"\t\t.eccstatus (),\n"
		"\t\t.q_b (),\n"
		"\t\t.rden_a (1'b1),\n"
		"\t\t.rden_b (1'b1),\n"
		"\t\t.wren_a (1'b0),\n"
		"\t\t.wren_b (1'b0));\n"
		"\tdefparam\n"
		"\t\taltsyncram_component.address_aclr_a = \"NONE\",\n"
		"\t\taltsyncram_component.clock_enable_input_a = \"BYPASS\",\n"
		"\t\taltsyncram_component.clock_enable_output_a = \"BYPASS\",\n"
		"\t\taltsyncram_component.init_file = \"./%s\",\n"
		"\t\taltsyncram_component.intended_device_family = \"MAX 10\",\n"
		"\t\taltsyncram_component.lpm_hint = \"ENABLE_RUNTIME_MOD=NO\",\n"
		"\t\taltsyncram_component.lpm_type = \"altsyncram\",\n"
		"\t\taltsyncram_component.numwords_a = %d,\n"
		"\t\taltsyncram_component.operation_mode = \"ROM\",\n"
		"\t\taltsyncram_component.outdata_aclr_a = \"NONE\",\n"
		"\t\taltsyncram_component.outdata_reg_a = \"CLOCK0\",\n"
		"\t\taltsyncram_component.ram_block_type = \"M9K\",\n"
		"\t\taltsyncram_component.widthad_a = %d,\n"
		"\t\taltsyncram_component.width_a = 8,\n"
		"\t\taltsyncram_component.width_byteena_a = 1;\n"
		"\n"
		"endmodule\n"
		"\n",
			modname, (width-1), outmif, size, width
	);
	fclose(fho);


	fho = fopen( outmif, "w" );
	fprintf( fho,
		"-- Generated Automatically by mkromv (max10 mode)\n\n"
		"DEPTH = %d;\n"
		"WIDTH = 8;\n"
		"ADDRESS_RADIX = HEX;\n"
		"DATA_RADIX = HEX;\n"
		"CONTENT\n"
		"BEGIN\n"
		"\n",
		size
	);

	for ( int i = 0; i < size; ++i ) {
		fprintf( fho, "%04X : %02X;\n", i, pbuf[i] );
	}

	fprintf( fho, "\nEND;\n\n");

	fclose(fho);


	delete [] pbuf;
	return 0;
}

//====================================================================================
int main( int argc, char *argv[] )
{
	if ( argc < 3 ) {
		printf("usage: %s filename modulename (-max10)\n", argv[0] );
		return -1;
	}

	if ( argc == 4 ) if (strcmp( argv[3], "-max10") == 0 ) return max10(argc,argv);

	return normal(argc, argv);
}

