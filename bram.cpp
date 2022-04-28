#include "bram.hpp"

Bram::Bram(sc_core::sc_module_name name): sc_module(name)
{
  from_interconnect.register_b_transport(this, &Bram::b_transport);  
  from_hw.register_b_transport(this, &Bram::b_transport);  
  //mem.reserve(MEM_RESERVED);

  SC_REPORT_INFO("BRAM", "Constructed.");
}

Bram::~Bram()
{
  SC_REPORT_INFO("BRAM", "Destroyed.");
}

void Bram::b_transport(pl_t &pl, sc_core::sc_time &offset)
{
    tlm::tlm_command cmd = pl.get_command(); //get payload command  
    //std::cout<<cmd<<std::endl;
    sc_dt::uint64 addr = pl.get_address(); //get payload address 
    //std::cout<<addr<<std::endl;
    unsigned int len = pl.get_data_length(); //get payload length 
    //std::cout<<len<<std::endl;
    unsigned char *buf = pl.get_data_ptr(); //get payload data
    //std::cout<<buf[0]<<buf[1]<<buf[2]<<buf[3]<<std::endl;
       
    switch(cmd)
     {
          case tlm::TLM_WRITE_COMMAND:              //copy from data array to target
          {  
             for (unsigned int i = 0; i < len; ++i)
             {
             		//addr=addr & 0x00FFFFFF;
                 mem[addr++]=buf[i];//write into memory
                 std::cout<<(int)mem[addr]<<std::endl;
                 
             }
             pl.set_response_status( tlm::TLM_OK_RESPONSE );  //succesful
             
          break;
          }
          case tlm::TLM_READ_COMMAND:                //copy from target to data_array 
          {
             for (unsigned int i = 0; i < len; ++i)
             {
               buf[i] = mem[addr++]; //read from memory
             }
               pl.set_response_status( tlm::TLM_OK_RESPONSE );  //succesful
          break;
          }
          default:
          	
             pl.set_response_status( tlm::TLM_COMMAND_ERROR_RESPONSE );  //unable to execute command
    }

  offset += sc_core::sc_time(10, sc_core::SC_NS);
}

/*
   generic payload example
   
  void b_transport( //the target
    tlm::tlm_generic_payload& trans, sc_core::sc_time& t) {
    
    tlm::tlm_command  cmd =trans.get_command();
    sc_dt::uint64 addr = trans.get_address();     //sc_dt::uint64 get_address() const;
    unsigned int len = trans.get_data_length();   //unsigned int get_data_length() const;
    unsigned char *buf = trans.get_data_ptr(); 

    if(adr+len >m_length) {
      trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONCE);  //Check for storage overflow
*/
