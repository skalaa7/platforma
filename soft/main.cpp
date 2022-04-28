#include "soft.hpp"

using namespace sc_core;

int sc_main(int argc, char* argv[])
{
	
	Soft("Soft");
	sc_start(20, SC_NS);
	
	return 0;
}
