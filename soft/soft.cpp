#include "soft.hpp"
#include "typedefs.hpp"
#include "utils.hpp"

SC_HAS_PROCESS(Soft);

Soft::Soft(sc_core::sc_module_name name)  
  : sc_module(name)
  , offset(sc_core::SC_ZERO_TIME)
{
  baza.open("baza.txt");
  if(!baza.is_open())
    SC_REPORT_ERROR("Soft", "Cannot open file.");

  SC_THREAD(simplex);
  SC_REPORT_INFO("Soft", "Constructed.");
}

Soft::~Soft()
{
  baza.close();
  SC_REPORT_INFO("Soft", "Destroyed.");
}

void Soft::simplex()
{

/*
    ---------------------------------------------
    WRITE PART - read from files, write into BRAMs   //MALO PREPRAVITI!!
    ---------------------------------------------
  */

  offset += sc_core::sc_time(10, sc_core::SC_NS);
  for (int i = 0; i < ROWSIZE; ++i)
  {//write matrix A into bram
        for (int j = 0; j < COLSIZE; ++j)
        {
          baza >> temp;  //iz baze u temp
           offset += sc_core::sc_time(10, sc_core::SC_NS);
          write_bram(p++, (num_t) temp);//write into bram

  write_hard(ADDR_INIT, 1);//start hardware     
   while(read_hard(ADDR_STATUS_INIT) == 0){//wait for hardware to finish - pooling
       offset += sc_core::sc_time(1, sc_core::SC_NS);//increment time
    }

    write_hard(ADDR_CMD, 1);//start hardware

    while(read_hard(ADDR_STATUS) == 0){//wait for hardware to finish - pooling
        offset += sc_core::sc_time(1, sc_core::SC_NS);//increment time
   }
    std::cout << std::endl << "Writing finished." << std::endl;//message
      }
    
 }
    //MakeMatrix
    float wv[ROWSIZE][COLSIZE];
	for(int j=0;j<ROWSIZE; j++)
	{
		for(int i =0;i<COLSIZE;i++)
		{
			wv[j][i]=0;
		}
	}

	fstream myFile;
    myFile.open("baza.txt",ios::in); //otvaram fajl u read modu
	if(myFile.is_open())
    {
        for(int j = 0; j < ROWSIZE; j++)
        {
            for(int i = 0; i< NUMOFVAR; i++)
            {
              myFile >> wv[j][i];
            }
        }
		for(int j = 0;j< NUMOFSLACK;j++)
		{
			myFile >> wv[j][COLSIZE-1];
		}
    }
    myFile.close();

	for(int j=0;j<ROWSIZE-1;j++)
	{
		{
			wv[j][NUMOFVAR+j]=1;  
		}
	}
	//-----------------------------------------------------------------
    //CalculateSimplex 
    int pivotRow;
    int pivotCol;
    bool unbounded=false;
    bool optimality=false;
    float pivot;

    while(!optimality)
    {
        //checkOptimality(wv)
        optimality=true;
        for(int i=0;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<0)//min> max<
                optimality=false;
        }
        //findPivotCol(wv);
        float minnegval=wv[ROWSIZE-1][0];
        int loc=0;
        for(int i=1;i<COLSIZE-1;i++)
        {
            if(wv[ROWSIZE-1][i]<minnegval)
            {
                minnegval=wv[ROWSIZE-1][i];
                loc=i;
            }
        }
        pivotCol=loc;
        //isUnbounded(wv,pivotCol)
        unbounded=true;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
                unbounded=false;
        }
        if(unbounded)
        {
            break;
        }
        //findPivotRow(wv,pivotCol);
        float rat[ROWSIZE-1];
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(wv[j][pivotCol]>0)
            {
                rat[j]=wv[j][COLSIZE-1]/wv[j][pivotCol];
            }
            else
            {
                rat[j]=0;
            }
        }

        float minpozval=99999999;
        loc=0;
        for(int j=0;j<ROWSIZE-1;j++)
        {
            if(rat[j]>0)
            {
                if(rat[j]<minpozval)
                {
                    minpozval=rat[j];
                    loc=j;
                }
            }
        }
        pivotRow=loc;

        pivot=wv[pivotRow][pivotCol];

        //doPivoting(wv_fixed,pivotRow,pivotCol,pivot_fixed);
        
        
       /*
    --------------------------------------------
    WRITE PART - write dimensions of A into HARD IP   
    --------------------------------------------
  */

  write_hard(ADDR_WIDTH, ROWSIZE-1); // -1 because thats how the IP was implemented
  write_hard(ADDR_HEIGHT, COLSIZE-1);

   write_hard(ADDR_CMD, 1);
  int ready = 1;
  while (ready)
    {
      ready = read_hard(ADDR_STATUS);
    }
  write_hard(ADDR_CMD, 0);
  while (!ready)
    {
      ready = read_hard(ADDR_STATUS);
    }

//-------------------------------------------------------------------------------------------------
    //Ispisivanje rezultata
    if(unbounded)
    {
        cout<<"Unbounded"<<endl;
    }
    else
    {
        //solutions(wv);
        for(int i=0;i<NUMOFVAR; i++)  //every basic column has the values, get it form B array
        {
            int count0 = 0;
            int index = 0;
            for(int j=0; j<ROWSIZE-1; j++)
            {
                if(wv[j][i]==0.0)
                {
                    count0 = count0+1;
                }
                else if(wv[j][i]==1)
                {
                    index = j;
                }
            }
            if(count0 == ROWSIZE - 2 )
            {
                cout<<"variable"<<i+1<<": "<<wv[index][COLSIZE-1]<<endl;  //every basic column has the values, get it form B array
            }
            else
            {
                cout<<"variable"<<i+1<<": "<<0<<endl;
            }
        }

        cout<<""<<endl;
        cout<<endl<<"Optimal solution is "<<wv[ROWSIZE-1][COLSIZE-1]<<endl;
    }

    //return 0;
}
                           //DANAS SE RADI!!  19.4.2022.
}



  


 //READ: Read data from BRAM
 
 void Soft::read_bram(sc_dt::uint64 addr, num_t &valM)
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  pl.set_address((addr*BUFF_SIZE) | VP_ADDR_BRAM_BASE);
  pl.set_data_length(BUFF_SIZE);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_READ_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  to_interconnect->b_transport(pl,offset);

  valM = to_fixed(buf);

}

//WRITE: Write data into BRAM

void Soft::write_bram(sc_dt::uint64 addr, num_t valM)
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  to_uchar(buf,valM);
  pl.set_address((addr/**BUFF_SIZE)*/ | VP_ADDR_BRAM_BASE);
  pl.set_data_length(BUFF_SIZE);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_WRITE_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  to_interconnect->b_transport(pl,offset);
}


int Soft::read_hard(sc_dt::uint64 addr)
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  pl.set_address((addr*BUFF_SIZE) | VP_ADDR_HARD_BASE);
  pl.set_data_length(BUFF_SIZE);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_READ_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  sc_core::sc_time offset = sc_core::SC_ZERO_TIME;
  to_interconnect->b_transport(pl,offset);
  return to_int(buf);
}

void Soft::write_hard(sc_dt::uint64 addr, int val)
{
  pl_t pl;
  unsigned char buf[BUFF_SIZE];
  to_uchar (buf, val);
  pl.set_address((addr*BUFF_SIZE) | VP_ADDR_HARD_BASE);
  pl.set_data_length(BUFF_SIZE);
  pl.set_data_ptr(buf);
  pl.set_command( tlm::TLM_WRITE_COMMAND );
  pl.set_response_status ( tlm::TLM_INCOMPLETE_RESPONSE );
  to_interconnect->b_transport(pl,offset);
}





 
 

