#include "Vdut_top.h"
#include "apb_req.hpp"

#if VM_TRACE
#include <verilated_vcd_sc.h>
#endif

int sc_main(int argc, char** argv) {

#if VM_TRACE
  Verilated::traceEverOn(true);
#endif

  Verilated::commandArgs(argc, argv);

  sc_clock clk{"clk", 10, SC_NS, 0.5, 3, SC_NS, true};
  sc_signal<bool>     presetn;
  sc_signal<uint32_t> paddr  ;
  sc_signal<uint32_t> pprot  ;
  sc_signal<bool>     psel   ;
  sc_signal<bool>     penable;
  sc_signal<bool>     pwrite ;
  sc_signal<uint32_t> pwdata ;
  sc_signal<uint32_t> pwstrb ;
  sc_signal<bool>     pready ;
  sc_signal<uint32_t> prdata ;
  sc_signal<bool>     pslverr;
  sc_signal<bool>     pwakeup;

  // instance and connect
  Vdut_top* top = new Vdut_top{"top"};
  top->pclk(clk);
  top->presetn(presetn);
  top->paddr  (paddr  );
  top->pprot  (pprot  );
  top->psel   (psel   );
  top->penable(penable);
  top->pwrite (pwrite );
  top->pwdata (pwdata );
  top->pwstrb (pwstrb );
  top->pready (pready );
  top->prdata (prdata );
  top->pslverr(pslverr);
  top->pwakeup(pwakeup);

  apb_req* apb = new apb_req("apb_req");
  apb->pclk(clk);
  apb->presetn(presetn);
  apb->paddr  (paddr  );
  apb->pprot  (pprot  );
  apb->psel   (psel   );
  apb->penable(penable);
  apb->pwrite (pwrite );
  apb->pwdata (pwdata );
  apb->pwstrb (pwstrb );
  apb->pready (pready );
  apb->prdata (prdata );
  apb->pslverr(pslverr);
  apb->pwakeup(pwakeup);

  // You must do one evaluation before enabling waves, in order to allow
  // SystemC to interconnect everything for testing.
  sc_start(1, SC_NS);

#if VM_TRACE
    // If verilator was invoked with --trace argument,
    // and if at run time passed the +trace argument, turn on tracing
    VerilatedVcdSc* tfp = nullptr;
    const char* flag = Verilated::commandArgsPlusMatch("trace");
    if (flag && 0 == strcmp(flag, "+trace")) {
        cout << "Enabling waves into vlt_dump.vcd...\n";
        tfp = new VerilatedVcdSc;
        top->trace(tfp, 99);  // Trace 99 levels of hierarchy
        tfp->open("vlt_dump.vcd");
    }
#endif

  // Start Simulation
  // while (!Verilated::gotFinish()) { sc_start(1, SC_NS); }

  std::cout << "*** Start Simulation" << std::endl;
  presetn = 1;
  sc_start(100, SC_NS);
  presetn = 0;
  sc_start(100, SC_NS);
  presetn = 1;
  sc_start(100, SC_NS);
  std::cout << "*** Release Reset at " << sc_time_stamp() << std::endl;

  sc_start(300, SC_NS);
  apb->set_pwakeup(true);
  std::cout << "*** WakeUp at " << sc_time_stamp() << std::endl;
  sc_start(50, SC_NS);

  apb->write(1, 0xFFFFFFFF);
  sc_start(100, SC_NS);
  apb->read (1, 0xFFFFFFFF, 0xFFFF0000);
  sc_start(100, SC_NS);
  apb->write(0, 0xAAAAAAAA);
  apb->read (0, 0xAAAAAAAA, 0xFFFFFFFF);
  apb->write(0, 0x55555555);
  apb->read (0, 0x55555555, 0xFFFFFFFF);
  sc_start(200, SC_NS);

  std::cout << "*** End Simulation at " << sc_time_stamp() << std::endl;

#if VM_TRACE
  if (tfp) { tfp->close(); tfp = nullptr; }
#endif

#if VM_COVERAGE
  VerilatedCov::write("coverage.dat");
#endif

  delete top;
  delete apb;
  return 0;
}

