
module dut_reg (
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

  wire  [13:0] w_addr; 
  wire         w_write; 
  wire         w_read; 

  wire         w_wr_reg0;
  wire         w_wr_reg1;
  wire         w_wr_reg2;

  reg  [31:0] r_dut_reg0;
  reg  [31:0] r_dut_reg1;
  reg  [31:0] r_dut_reg2;

  reg  [31:0] r_rdata;
  reg         r_pready;

  assign w_addr  = paddr[15:2];
  assign w_write = psel & penable & pwrite;
  assign w_read  = psel & penable & ~pwrite;

  assign w_wr_reg0 = w_write & (w_addr == 'd0);
  assign w_wr_reg1 = w_write & (w_addr == 'd1);
  assign w_wr_reg2 = w_write & (w_addr == 'd2);

  //==========
  // Write
  //==========
  //-- reg0
  always @(posedge pclk or negedge presetn) begin
    if (~presetn)       r_dut_reg0 <= 'd0;
    else if (w_wr_reg0) r_dut_reg0 <= pwdata;
  end

  //-- reg1
  always @(posedge pclk or negedge presetn) begin
    if (~presetn)       r_dut_reg1 <= 'd0;
    else if (w_wr_reg1) r_dut_reg1 <= pwdata;
  end

  //-- reg2
  always @(posedge pclk or negedge presetn) begin
    if (~presetn)       r_dut_reg2 <= 'd0;
    else if (w_wr_reg2) r_dut_reg2 <= pwdata;
  end

  //==========
  // Read
  //==========
  always @(posedge pclk or negedge presetn) begin
    if (~presetn) r_rdata <= 'd0;
    else if (w_read)  begin
      case(w_addr)
        'd0: r_rdata <= r_dut_reg0;
        'd1: r_rdata <= r_dut_reg1;
        'd2: r_rdata <= r_dut_reg2;
        default: r_rdata <= 'd0;
      endcase
    end
  end

  //==========
  // READY
  //==========
  always @(posedge pclk or negedge presetn) begin
    if (~presetn) r_pready <= 'd0;
    else          r_pready <= w_write | w_read ;
  end

  assign prdata  = r_rdata;
  assign pready  = r_pready;
  assign pslverr = 1'b0;

endmodule
