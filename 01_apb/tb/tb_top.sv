
`include "apb_if.sv"

module tb_top();

  logic clk    = '0;
  logic resetn = '1;

// time t_clk20 = 50ns; // 20MHz

  apb_if apb_if(clk, resetn);

  //=========================
  // DUT
  //-------------------------
  dut_top u_top(
    .pclk    ( clk     )
   ,.presetn ( resetn  ) 
   ,.paddr   ( apb_if.paddr   )
   ,.pprot   ( apb_if.pprot   )
   ,.psel    ( apb_if.psel    )
   ,.penable ( apb_if.penable )
   ,.pwrite  ( apb_if.pwrite  )
   ,.pwdata  ( apb_if.pwdata  )
   ,.pwstrb  ( apb_if.pwstrb  )
   ,.pready  ( apb_if.pready  )
   ,.prdata  ( apb_if.prdata  )
   ,.pslverr ( apb_if.pslverr )
   ,.pwakeup ( apb_if.pwakeup )
  );

  //=========================
  // Clock
  //-------------------------
  always begin
    // #(t_clk20 / 2);
    #(50ns / 2);
    clk <= ~clk;
  end

  //=========================
  // Reset
  //-------------------------
  initial begin
    #200ns;
    resetn = '0;
    #200ns;
    @(posedge clk);
    resetn = '1;
  end
  
  //=========================
  // Test
  //-------------------------
  initial begin
    $timeformat(-6, 3, " us", 8);
    $display("Start simulation.");
    @(posedge resetn);
    $display("Release resetn : %t", $realtime);
    repeat(5) @(posedge clk);
    apb_if.set_pwakeup(1);
    repeat(10) @(posedge clk);
    $display("Set wakeup : %t", $realtime);

    apb_if.write(1, 'hFFFFFFFF);
    repeat(10) @(posedge clk);
    apb_if.read (1, 'hFFFFFFFF, 'hFFFF0000);
    repeat(10) @(posedge clk);
    apb_if.write(0, 'hAAAAAAAA);
    apb_if.read (0, 'hAAAAAAAA, 'hFFFFFFFF);
    apb_if.write(0, 'h55555555);
    apb_if.read (0, 'h55555555, 'hFFFFFFFF);
    repeat(10) @(posedge clk);

    $display("End simulation : %t", $realtime);
    $finish();
  end

  //=========================
  // Waves
  //-------------------------
  initial begin
    $dumpfile("waves.vcd");
    $dumpvars(0, u_top);
  end

endmodule
