#include "apb_req.hpp"

void apb_req::write(uint32_t addr, uint32_t data, uint32_t strb) {
  req_data_t req;
  req.addr  = addr;
  req.prot  = 0;
  req.sel   = true;
  req.enable= true;
  req.write = true;
  req.data  = data;
  req.strb  = strb;
  queue_req.push(req);
};

void apb_req::read(uint32_t addr, uint32_t data, uint32_t mask) {
  req_data_t req;
  req.addr  = addr;
  req.prot  = 0;
  req.sel   = true;
  req.enable= true;
  req.write = false;
  req.data  = data;
  req.strb  = mask;
  queue_req.push(req);
};

void apb_req::set_pwakeup(bool value) {
  m_pwakeup = value;
};

//===============================================
//=== SystemC Thread
//===============================================
void apb_req::th_request() {
  paddr   = 0;
  pprot   = 0;
  psel    = 0;
  penable = 0;
  pwrite  = 0;
  pwdata  = 0;
  pwstrb  = 0;

  for(;;) {
    paddr   = 0;
    pprot   = 0;
    psel    = 0;
    penable = 0;
    pwrite  = 0;
    pwdata  = 0;
    pwstrb  = 0;
    wait();
    if (queue_req.size() > 0) {
      req_data_t req = queue_req.front();
      paddr   = req.addr  ;
      pprot   = req.prot  ;
      psel    = req.sel   ;
      penable = req.enable;
      pwrite  = req.write ;
      if (req.sel && req.enable) {
        if (req.write) {
          pwdata = req.data;
          pwstrb = req.strb;
          do {
            wait();
          } while(!pready.read());
          sc_assert(pslverr.read() == false);
        } else { // read
          uint32_t rdata;
          do {
            wait();
            rdata = prdata.read(); 
          } while(!pready.read());
          sc_assert(pslverr.read() == false);
          if ( (req.data & req.strb) 
              != (rdata & req.strb) ) {
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(8)
               << "DUT : 0x" << rdata
               << ", EXP : 0x" << req.data
               << ", Mask : 0x" << req.strb
               << std::string("\n");
            SC_REPORT_ERROR("APB", ss.str().c_str() );
          }
        }
      }
      queue_req.pop();
      // wait();
    }
  }
};

void apb_req::cth_wakeup() {
  pwakeup.write(0);
  for(;;) {
    wait();
    pwakeup.write(m_pwakeup);
  }
};

