// issp_dp.v

// Generated using ACDS version 18.1 221

`timescale 1 ps / 1 ps
module issp_dp (
		output wire [0:0] source,     //    sources.source
		input  wire       source_clk  // source_clk.clk
	);

	altsource_probe_top #(
		.sld_auto_instance_index ("YES"),
		.sld_instance_index      (0),
		.instance_id             ("NONE"),
		.probe_width             (0),
		.source_width            (1),
		.source_initial_value    ("1"),
		.enable_metastability    ("YES")
	) altera_in_system_sources_probes_inst (
		.source     (source),     //  output,  width = 1,    sources.source
		.source_clk (source_clk), //   input,  width = 1, source_clk.clk
		.source_ena (1'b1)        // (terminated),                        
	);

endmodule