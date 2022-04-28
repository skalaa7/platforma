#ifndef SOFT_HPP_
#define SOFT_HPP_

#include <iostream>
#include <fstream>
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <deque>
#include <vector>
#include <cmath>
#include "typedefs.hpp"

class Soft : public sc_core::sc_module
{
public:
       Soft(sc_core::sc_module_name name);
       ~Soft();
         
       tlm_utils::simple_initiator_socket<Soft>to_interconnect; 

protected:
       void simplex();
       pl_t pl;
       sc_core::sc_time offset;
       std::ifstream baza;
       
       num_t wv_fixed[ROWSIZE][COLSIZE];
       num_t pivot_fixed;
       float temp;
       unsigned char p = 0;
       
       void read_bram(sc_dt::uint64 addr, num_t &valM);
       void write_bram(sc_dt::uint64 addr, num_t valM);
       int read_hard(sc_dt::uint64 addr);
       void write_hard(sc_dt::uint64 addr, int val);       
};

#endif // SOFT_HPP_
