
interface apb_if ( input pclk, input presetn);

  logic [15:0] paddr  ;
  logic [ 2:0] pprot  ;
  logic        psel    = '0;
  logic        penable = '0;
  logic        pwrite  = '0;
  logic [31:0] pwdata  = '0;
  logic [ 3:0] pwstrb  = '0;
  logic        pready ;
  logic [31:0] prdata ;
  logic        pslverr;
  logic        pwakeup = '0;

  task write (int addr, int data, int strb='hFFFF_FFFF);
    @(posedge pclk);
    psel    = 1'b1;
    penable = 1'b1;
    pwrite  = 1'b1;
    paddr   = addr[15:0];
    pwdata  = data;
    pwstrb  = strb[3:0];
    // @(posedge pclk iff pready === 1'b1);
    do begin
      @(posedge pclk);
    end while(~pready);

    psel    = 1'b0;
    penable = 1'b0;
  endtask

  task read (int addr, int data, int strb='hFFFF_FFFF);
    @(posedge pclk);
    psel    = 1'b1;
    penable = 1'b1;
    pwrite  = 1'b0;
    paddr   = addr[15:0];
    // @(posedge pclk iff pready === 1'b1);
    do begin
      @(posedge pclk);
    end while(~pready);

    if ((data & strb) !== (prdata & strb)) begin
      $display("Error mismsmatch");
    end

    psel    = 1'b0;
    penable = 1'b0;
  endtask

  task set_pwakeup( bit value);
    @(posedge pclk);
    pwakeup = value;
  endtask

endinterface

