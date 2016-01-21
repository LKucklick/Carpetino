void handle_status(byte stat, byte errc)
{
        
        if(!((errc == 0) || (errc == 2) || (errc == 4))) //if NOT (all ok or over volt or under volt)
        {
          if(status_ok){set_statuspin_low(errc);}          
        } else
          {
            if((errc == 2) && (current > 0 + current_zero_tolerance))          // if over volt and charging current
              {
                if(status_ok){set_statuspin_low(errc);}          
              } else
                {
                  if((errc == 4) && (current < 0 - current_zero_tolerance))          // if under volt and decharge current
                  {
                    if(status_ok){set_statuspin_low(errc);}          
                  } else
                    {
                      if(((stat == 4) || (stat == 16)) && (current > 0 + current_zero_tolerance))          // if (full charged or overcharged) and charging current
                      {
                        if(status_ok){set_statuspin_low(errc);}
                      } else
                        {
                          if((stat == 8) && (current < 0 - current_zero_tolerance))          // if empty and decharge current
                          {
                            if(status_ok){set_statuspin_low(errc);}          
                          } else
                            {
                               if((!status_ok) && (switch_on_allowed)){set_statuspin_high(errc); /*delay(10000);*/ /*last_switch = millis();*/} 
                            }
                        }
                    }
                }
          }
}



byte errorcode()
{
   byte errc = 0;
   
   errc = errc + voltage_check();
  
   //if((millis() - last_switch) >= temp_timeout){errc = errc + 8 * temperature_check();} //sometimes switching the relays causes the micros to reset, producing errors, leading to shut the relays off. temp timeout sets the time in millis to ignore the temperature values in errorcheck
   //errc = errc + 8 * repeated_error(temperature_check());
  
   errc = errc + 64 * current_check();

  return errc;
}

byte statuscode()
{
  byte stat = 0;
  
  stat = stat + 1   * status_ok;
  stat = stat + 2   * full_charged();
  stat = stat + 4   * empty();
  stat = stat + 8   * over_charged();
  stat = stat + 16  * status_is_balancing;
  stat = stat + 32  * status_is_relaxing;
  stat = stat + 64  * !switch_on_allowed;
  stat = stat + 128 * 0;
  
  return stat;
}


byte repeated_error(byte errorcode)
{ 
  byte err = 0;
  
  if(errorcode == 0)
  {
    errorcount = 0;
  } else
    {
      errorcount++; 
    }
    
  if(errorcount > allowed_errors)
  {
    err = errorcode;
  }
  
  return err; 
}

boolean full_charged()
{
  boolean full = false;
  byte full_charge_count = 0;
  
  for(int s = 0; s < number_of_stacks; s++)
  {
      for(int c = 0; c < number_of_cells; c++)
      {
         if(cell_voltages[s][c] >= full_charge_voltage - voltage_full_charge_tolerance)
         {
            if(!(cell_voltages[s][c] > full_charge_voltage))
            {
                full_charge_count++; 
            }             
         }
      }
  }
  if(full_charge_count == number_of_stacks * number_of_cells)
  {
     full = true; 
  }  
  
  status_full_charged = full;
  
  //update charge parameters
  myAmp_hours_counter_dcc    =     0;                                                      
  myAmp_hours_gauge_dcc    =     myAmp_hours_full; 
 
 // switch_on_allowed = !full;
  return full;
}


boolean over_charged()
{
  boolean over = false;
  
  for(int s = 0; s < number_of_stacks; s++)
  {
      for(int c = 0; c < number_of_cells; c++)
      {
            if(cell_voltages[s][c] > full_charge_voltage)
            {
                over = true;
            }             
      }
  }
  return over;
}


boolean empty()
{
  boolean emp = false;
  
  for(int s = 0; s < number_of_stacks; s++)
  {
      for(int c = 0; c < number_of_cells; c++)
      {
         if(cell_voltages[s][c] <= empty_voltage)
         {
            emp = true;          
         }
      }
  }
  status_empty = emp;
  return emp;
}
