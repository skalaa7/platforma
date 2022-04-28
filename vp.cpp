#include "vp.hpp"

Vp::Vp (sc_core::sc_module_name name)
  : sc_module (name)
  , soft("Soft")
  , hard("Hard")
  , bram("Bram")
  , interconnect("Interconnect")
{
  soft.to_interconnect.bind(interconnect.from_sw); 
  interconnect.to_bram.bind(bram.from_interconnect); 
  bram.to_hw.bind(hard.from_bram); 
  interconnect.to_hw_int.bind(hard.from_interconnect_int); 
  interconnect.to_hw_num_t.bind(hard.from_interconnect_num_t); 
  hard.to_bram.bind(bram.from_hw);
 
  SC_REPORT_INFO("Virtual Platform", "Constructed.");
}

Vp::~Vp()
{
  SC_REPORT_INFO("Virtual Platform", "Destroyed.");
}
