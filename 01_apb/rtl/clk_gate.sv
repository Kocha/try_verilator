module clk_gate (
    i_clk
  , i_en
  , o_clk
);

  input  i_clk;
  input  i_en ;
  output o_clk;

  assign o_clk = i_clk & i_en;

endmodule

