int voltage_get()
{
    byte   values[25] = {};
    byte   ident[1]   = {0};
    int    err        = 0;
    

  for(int s = 0; s < number_of_stacks; s++)
  {
      for(int c = 0; c < number_of_cells; c++)
      {
          cell_voltages[s][c] = 0;
      }
  }


    for(int stack = 0; stack < number_of_stacks; stack++)
    {
            ident[0]   =   (stack + 1) * 16 + 1;
            
            err     =   i2c_sendbytes(linduinoadr, 1, ident);
            if(err != 0)                           {return (100 + (stack+1)*10 + err);}
            
            if(stack == 0)                         {delay(100);}
            
            err     =   25 - i2c_requestbytes(linduinoadr, 25, values);
            if(err != 0)                           {return (100 + (stack+1)*10 + 6);}           
            
            ident[0]   =   values[0];
            
            if  ((ident[0] / 16) != (stack+1))         {return (100 + (stack+1)*10 + 7);}
            if  ((ident[0] % 16) != 1)                 {return (100 + (stack+1)*10 + 8);}
            
            for  (int i = 0; i < number_of_cells; i++)
            {
                cell_voltages[stack][i] = values[2 * i + 1] * 256 + values[2 * i + 2];
            }  
    }  
  return err;
}




int voltage_check()
{ 
  int            stat      = 0;        // voltage status: 0: ok; 1: not plausible; 2: upper voltage limit reached; 4: lower voltage limit reached
  unsigned int   maximum   = voltage_maximum();        // maximum voltage of all stacks
  unsigned int   minimum   = voltage_minimum();    // minimum voltage of all stacks
  
  

  
// Voltage plausibility check  
  
  if(maximum - minimum > voltage_plausible_tolerance){ stat = stat + 1; }

// Over Voltage check

  if(maximum > over_voltage)   { stat = stat + 2; }
  
// Under Voltage check

  if(minimum < under_voltage)  { stat = stat + 4; }  
  
  
  return stat;
}

int voltage_initialize()
{
  int err = 0;
  
  err     =   voltage_get();
  if(err != 0)                {return err;}
  
  find_cells_to_discharge(voltage_minimum(), true, false);
  
  create_config(); 
  
  err = write_config();
  if(err != 0)                {return err;}

  return err;
}

unsigned int voltage_minimum()
{
   //find lowest cell voltage
    unsigned int voltage_min = 65535;
    
    for (int s = 0; s < number_of_stacks; s++)
    {	
        for (int c = 0; c < number_of_cells; c++)
        {
        	if (cell_voltages[s][c] < voltage_min)
    		{
    			voltage_min = cell_voltages[s][c];
    		}
        }//end c			
    }//end s
    
  return voltage_min;
}

unsigned int voltage_maximum()
{
   //find highest cell voltage
    unsigned int voltage_max = 0;
    
    for (int s = 0; s < number_of_stacks; s++)
    {	
        for (int c = 0; c < number_of_cells; c++)
        {
        	if (cell_voltages[s][c] > voltage_max)
    		{
    			voltage_max = cell_voltages[s][c];
    		}
        }//end c			
    }//end s
    
  return voltage_max;
}

unsigned int voltage_stack(int stack)                    // returns total voltage of one stack
{  
  unsigned int totvol = 0;
  
  for (int i = 0; i < number_of_cells; i++)
  {
    totvol = totvol + cell_voltages[stack][i] * 0.1;    
  }
  
  return totvol;
}

unsigned int voltage_battery()                           // returns total voltage of all stacks / the whole battery
{  
  unsigned int batvol = 0;
  
  for (int i = 0; i < number_of_stacks; i++)
  {
    batvol = batvol + voltage_stack(i) * 0.1;  
  }
  
  return batvol;
}



