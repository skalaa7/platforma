/*interface: A class derived from class sc_interface. An interface proper is an interface, and in the object-
oriented sense a channel is also an interface. However, a channel is not an interface proper.
*/
#ifndef INTERCONNECT_HPP_
#define INTERCONNECT_HPP_
                             
#include <iostream>
#include <fstream>
#include <systemc>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#include "typedefs.hpp"
#include "utils.hpp"

class Interconnect :  public sc_core::sc_module   
{
public:
    Interconnect(sc_core::sc_module_name name);
    ~Interconnect();
    tlm_utils::simple_initiator_socket<Interconnect> to_bram; 
    tlm_utils::simple_initiator_socket<Interconnect> to_hw_int; 
    tlm_utils::simple_initiator_socket<Interconnect> to_hw_num_t; 
    tlm_utils::simple_target_socket<Interconnect> from_sw; 
protected:
    pl_t pl; //payload
    sc_core::sc_time offset; //time
    void b_transport(pl_t &pl, sc_core::sc_time &offset); //transport function
};

#endif // INTERCONNECT_HPP_

/*
     tlm_utils::simple_target_socket<Interconnect> targ_socket;
     tlm_utils::simple_initiator_socket<Interconnect>init_socket;
  */
  
  /* typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
 void b_transport(pl_t&, sc_core::sc_time&);   */
  //void b_transport(TRANS&, sc_time&);
