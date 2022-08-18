
module dut_top (
    pclk
  , presetn 
  , paddr
  , pprot
  , psel
  , penable
  , pwrite
  , pwdata
  , pwstrb
  , pready
  , prdata
  , pslverr
  , pwakeup
);

  input         pclk   ;
  input         presetn;
  input  [15:0] paddr  ;
  input  [ 2:0] pprot  ;
  input         psel   ;
  input         penable;
  input         pwrite ;
  input  [31:0] pwdata ;
  input  [ 3:0] pwstrb ;
  output        pready ;
  output [31:0] prdata ;
  output        pslverr;
  input         pwakeup;

  wire  w_pclk;

  clk_gate u_clk_gate (
    .i_clk (pclk)
   ,.i_en  (pwakeup)
   ,.o_clk (w_pclk)
  );

  dut_reg  u_dut_reg (
    .pclk    ( w_pclk  )
   ,.presetn ( presetn ) 
   ,.paddr   ( paddr   )
   ,.pprot   ( pprot   )
   ,.psel    ( psel    )
   ,.penable ( penable )
   ,.pwrite  ( pwrite  )
   ,.pwdata  ( pwdata  )
   ,.pwstrb  ( pwstrb  )
   ,.pready  ( pready  )
   ,.prdata  ( prdata  )
   ,.pslverr ( pslverr )
  );

endmodule
