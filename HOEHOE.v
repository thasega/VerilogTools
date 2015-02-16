// synopsys translate_on

module HOEHOE
(
	input [12:0] address,
	input   tri1 clock,
	output [7:0] q
);
	altsyncram altsyncram_component (
		.address_a (address),
		.clock0 (clock),
		.q_a(q),
		.aclr0 (1'b0),
		.aclr1 (1'b0),
		.address_b (1'b1),
		.addressstall_a (1'b0),
		.addressstall_b (1'b0),
		.byteena_a (1'b1),
		.byteena_b (1'b1),
		.clock1 (1'b1),
		.clocken0 (1'b1),
		.clocken1 (1'b1),
		.clocken2 (1'b1),
		.clocken3 (1'b1),
		.data_a ({8{1'b1}}),
		.data_b (1'b1),
		.eccstatus (),
		.q_b (),
		.rden_a (1'b1),
		.rden_b (1'b1),
		.wren_a (1'b0),
		.wren_b (1'b0));
	defparam
		altsyncram_component.address_aclr_a = "NONE",
		altsyncram_component.clock_enable_input_a = "BYPASS",
		altsyncram_component.clock_enable_output_a = "BYPASS",
		altsyncram_component.init_file = "./HOEHOE.mif",
		altsyncram_component.intended_device_family = "MAX 10",
		altsyncram_component.lpm_hint = "ENABLE_RUNTIME_MOD=NO",
		altsyncram_component.lpm_type = "altsyncram",
		altsyncram_component.numwords_a = 8192,
		altsyncram_component.operation_mode = "ROM",
		altsyncram_component.outdata_aclr_a = "NONE",
		altsyncram_component.outdata_reg_a = "CLOCK0",
		altsyncram_component.ram_block_type = "M9K",
		altsyncram_component.widthad_a = 13,
		altsyncram_component.width_a = 8,
		altsyncram_component.width_byteena_a = 1;

endmodule

