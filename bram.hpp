#ifndef BRAM_HPP
#define BRAM_HPP

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <vector>
#include "typedefs.hpp"
#include "utils.hpp" 
#include <iostream>

class Bram : public sc_core::sc_module
{
public:
  Bram (sc_core::sc_module_name name);
  ~Bram();
  
  tlm_utils::simple_initiator_socket<Bram>to_hw; 
  tlm_utils::simple_target_socket<Bram>from_interconnect; //*
  tlm_utils::simple_target_socket<Bram>from_hw;
  
protected:
  void b_transport(pl_t &, sc_core::sc_time &); 
  unsigned char mem[MEM_RESERVED];
};

#endif // BRAM_HPP
