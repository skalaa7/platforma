#include "interconnect.hpp"

Interconnect::Interconnect(sc_core::sc_module_name name): sc_module(name), offset(sc_core::SC_ZERO_TIME)
{
    from_sw.register_b_transport(this, &Interconnect::b_transport);
    SC_REPORT_INFO("Interconnect", "Constructed.");
}

Interconnect::~Interconnect()
{
  SC_REPORT_INFO("Interconnect", "Destroyed.");  
}

void Interconnect::b_transport(pl_t &pl, sc_core::sc_time &offset)  
{
    sc_dt::uint64 addr = pl.get_address(); //set payload address
    sc_dt::uint64 taddr = addr & 0x00FFFFFF; //mask to get local address
	  
	if(addr >= VP_ADDR_BRAM_L && addr <= VP_ADDR_BRAM_H) //transport for bram
    {
      std::cout << "sending to bram" << std::endl;
      pl.set_address(taddr); //set local address
      to_bram->b_transport(pl, offset); //transport  //from hard
      
    }
	else if(addr >= VP_ADDR_HARD_INT_L && addr <= VP_ADDR_HARD_INT_H) //transport for hardware
    {
      std::cout << "sending to hard (int)" << std::endl;
      pl.set_address(taddr); //set local address
      
      to_hw_int->b_transport(pl, offset); //transport  //from interconnect
      std::cout << "urade se" << std::endl;
    } 
      
    else if(addr >= VP_ADDR_HARD_NUM_T_L && addr <= VP_ADDR_HARD_NUM_T_H) //transport for hardware
    {
      std::cout << "sending to hard (num_t)" << std::endl;
      pl.set_address(taddr); //set local address
      to_hw_num_t->b_transport(pl, offset); //transport 
    } 
       else //error
    {
      SC_REPORT_ERROR("Interconnect", "Wrong address.");
      pl.set_response_status ( tlm::TLM_ADDRESS_ERROR_RESPONSE ); 
    }

  offset += sc_core::sc_time(10, sc_core::SC_NS); //increment time
}

/*b_transport method has a single transaction argument passed by non-const reference and a second argument to annotate timing
 //init_socket->b_transport(trans, delay);
The b_transport method has a timing annotation argument. This single argument is used on both the call to
and the return from b_transport to indicate the time of the start and end of the transaction, respectively,
relative to the current simulation time.*/ 
 /*The method operator-> of the target socket shall call method operator-> of the port in the target socket
       (on the backward path), and shall return the value returned by operator-> of the port.*/
       
