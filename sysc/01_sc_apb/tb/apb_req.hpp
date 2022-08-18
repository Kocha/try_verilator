
#pragma once

#include <queue>
#include <sstream>
#include "systemc.h"

SC_MODULE( apb_req ){
  sc_in<bool>      pclk;
  sc_in<bool>      presetn;
  sc_out<uint32_t> paddr  ;
  sc_out<uint32_t> pprot  ;
  sc_out<bool>     psel   ;
  sc_out<bool>     penable;
  sc_out<bool>     pwrite ;
  sc_out<uint32_t> pwdata ;
  sc_out<uint32_t> pwstrb ;
  sc_in<bool>      pready ;
  sc_in<uint32_t>  prdata ;
  sc_in<bool>      pslverr;
  sc_out<bool>     pwakeup;

  struct req_data_t {
    uint32_t  addr  ;
    uint32_t  prot  ;
    bool      sel   ;
    bool      enable;
    bool      write ;
    uint32_t  data  ;
    uint32_t  strb  ;
  };

  std::queue<req_data_t> queue_req;
  bool      m_pwakeup;

  void  write(uint32_t addr, uint32_t data, uint32_t strb=0xFFFFFFFF);
  void  read (uint32_t addr, uint32_t data, uint32_t mask=0xFFFFFFFF);
  void  set_pwakeup(bool value);

  void  th_request();
  void  cth_wakeup();

  SC_CTOR( apb_req ):
     pclk("pclk")
   , presetn("presetn")
   , paddr("paddr")
   , pprot("pprot")
   , psel("psel")
   , penable("penable")
   , pwrite("pwrite")
   , pwdata("pwdata")
   , pwstrb("pwstrb")
   , pready("pready")
   , prdata ("prdata")
   , pslverr("pslverr")
   , pwakeup("pwakeup")
  {
    m_pwakeup = 0;

    SC_THREAD( th_request );
      sensitive << pclk.pos();
      async_reset_signal_is( presetn, false );
      // dont_initialize();

    SC_CTHREAD( cth_wakeup, pclk.pos() );
      async_reset_signal_is( presetn, false );
      // dont_initialize();
  }
};

