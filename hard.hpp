#ifndef HARD_HPP_
#define HARD_HPP_

#include <systemc>
#include <math.h>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "typedefs.hpp"
#include "utils.hpp"

class Hard : public sc_core::sc_module
{
public:
  // SC_HAS_PROCESS(Hard);
     Hard(sc_core::sc_module_name name);
     ~Hard();
     tlm_utils::simple_initiator_socket<Hard> to_bram;
     tlm_utils::simple_target_socket<Hard> from_bram; 
     tlm_utils::simple_target_socket<Hard> from_interconnect_int; 
     tlm_utils::simple_target_socket<Hard> from_interconnect_num_t; 

protected:
     pl_t pl;
     sc_core::sc_time offset;
     int width, height, ready, start, pivotRow, pivotCol;
     num_t wv_fixed[ROWSIZE][COLSIZE];
     num_t pivot_fixed;
     void startHard();
     void doPivoting(num_t wv[ROWSIZE][COLSIZE],int pivotRow,int pivotCol,num_t pivot);
     void b_transport(pl_t &pl, sc_core::sc_time &offset);
     num_t read_bram(int addr);
     void write_bram(int addr, num_t val);
};

#endif // HARD_HPP_
