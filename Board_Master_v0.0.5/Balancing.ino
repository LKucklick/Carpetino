int balance(long last_current)
{   
    byte errc = 255;                              //declare status and error codes
    byte stat = 255;
    byte err = 0;
    byte err_h = 0;

    status_is_relaxing = true;                    // battery status is relaxing
    
    if(relax(relaxtime) != 0)                             // relax( 'time in minutes; decimal places allowed, eg. 0.5 minutes; standart 10;
    {
        status_is_relaxing = false;               // if relax wasnt successfull (an error occured while relaxing) set status relax to false and return error
        return 1;
    }
    
    status_is_relaxing = false;                              // battery status not relaxing anymore

    if((last_current < 5000) && (last_current > 0 - current_zero_tolerance))          // if the last charging current was below 5A then balance, if it was higher go back in loop
    {    
            status_is_balancing = true;                      // battery status is balancing
                        
            unsigned int voltage_min = voltage_minimum();    //find lowest cell voltage
        
            int balanced_cells = 0;                                    // declares balanced cells variable. counts how many cells are balanced at the moment
            
            int pre_balanced_cells = -1;                               // declares pre_balanced cells variable. remembers how many cells were balanced the last while loop
            
            boolean switchback = true;                                 // allowes the find_cells_to_discharge function to declare cells to decharge
            
            while(balanced_cells < number_of_stacks * number_of_cells) //while not all cells are balanced
            {
            	err_h =   voltage_get();
                if (err_h != 0)  {COMPRINT("Get Voltage ERROR: ");          COMPRINTLN(err_h);}     // Get Voltage values from Linduino
                err = err + err_h;
                
                err_h =   current_get();
                if (err_h != 0)  {COMPRINT("Get Current ERROR: ");          COMPRINTLN(err_h);}     // Get Current values from Linduino
                err = err + err_h;
                /*
                err_h =   temperature_get();
                if (err_h != 0)  {COMPRINT("Get Temperature ERROR: ");      COMPRINTLN(err_h);}     // Get Temperature values from Micros
                err = err + err_h;
                */
                errc = errorcode();                                          // get error- and statuscodes
                stat = statuscode();
                
                if(stat == 18){switch_on_allowed = false;}                    // if the battery turns out to be full charged do not switch the status_ok-pin back on to avoid overcharging. restart necessary
                
                //sd_logdata(stat, errc);
                
                //serial_show_data(errc,stat);                         // Print Voltage, Temperature, Current and/or Status on Serial Monitor
                
                if((!(errc == 0 || errc == 2)) || stat == 18 || err != 0)     // emergency exit; errorcode is allowed to be 0(all ok, just ok-pin low) or 2(overcharged - reason to balance); also gets out if full charged or for get_voltage/current/temperature error
                {
                  find_cells_to_discharge(voltage_min, true, false);                               // sets all discharge cell to zero
                
                  create_config();                                                                 // creates config to stop discharging
                  write_config();                                                                  // writes config; stops discharging
                  
                  status_is_balancing = false;                                                     // status is balancing false
                  if(err != 0){return err;}else{return 2;}                                         // return the errorcode                     
                }
                                    
                balanced_cells = find_cells_to_discharge(voltage_min, false, switchback);                            // identifies cells to be discharged. writes info in discharge_cell; voltage_min is balancing target; false is not set zero; switchback allowes the function to discharge cells; returns the number of balnced cells    
                switchback = tannenbaumeffect;                                                                                  // after first time once discharged cells wont be discharged again if they were ok; to avoid tannenbaum effect                    

            	if(balanced_cells != pre_balanced_cells)                                                             // if there is a change in the number of balanced cells, update the config
            	{
            		create_config();                                                                             // creates config with discharge commands
            		write_config();                                                                              // writes the config on linduino
            		pre_balanced_cells = balanced_cells;                                                         // remembers how many cells are balanced right now
            	}//end if disbalanced
            	delay(300);                                                                                          // wait till next while loop
            }//end while
            
                            
            //delay(5000);                                                                                             // when all cells are balanced wait to go on to relax a bit
    }
    
    status_is_balancing = false;                                        // status balancing false
  
    return 0;                                                           // return success
}

boolean balancing_necessary(byte stat, byte errc)                        // checks if balancing (or relaxing) is necessary
{
   boolean necessary = false;                                            // declare boolean
   
   if((stat == 8) || (stat == 9))                                        // if status = overcharged (8) then necessary; status 9 (overcharged + statuspin high) should not happen.
   {
       necessary = true;                                                 // balancing necessary
   }
   
   if(!((errc == 2) || (errc == 0)) || (voltage_maximum() > 50000))
   {
       necessary = false;
   }
   return necessary;                                                     // return the info
}


int find_cells_to_discharge(unsigned int voltage_min, boolean set_zero, boolean allow_switch_back)                        // identifies cells with higher voltage than lowest one
{
    int balanced_cells = 0;    // counts the number of balanced cells
    unsigned int balancingaim;
    
    if(balancing_aim == 0)
    {
      balancingaim = voltage_min;
    } else
    {
      balancingaim = balancing_aim;
    }
    
    for (int s = 0; s < number_of_stacks; s++)
    {
        for (int c = 0; c < number_of_cells; c++)
    	{
    		if (cell_voltages [s][c] > balancingaim)                                                                  // if the cell has higher voltage than voltage_min
    		{
    			if(allow_switch_back){discharge_cell[s][c] = 1;}                                                 // mark it as to be discharged. if allow switch back is true all the time the tannenbaum effect will occur
                        if((!allow_switch_back) && (discharge_cell[s][c] == 0)){balanced_cells++;}                       // counts the cells witch were already turned off but have higher voltage now as balanced
    		}//end if
    		else
    		{
    			discharge_cell[s][c] = 0;                                                                        // if cell voltage reached the aim stop discharging
    			balanced_cells++;                                                                                // count cell as balanced
    		}//end else
    
                if(set_zero){discharge_cell[s][c] = 0;}                                                                  // if set_zero is true all cells wont be discharged
    
    	}//end for c
    }// end for s
   
  if(set_zero){return -1;}                                                 // returns -1 if set_zero was true
  
  return balanced_cells;                                                   // returns the number of balanced cells
}



int relax(double relaxtime)                                    // relaxes the battery after charging. after all it does nothing than waiting and checking if there is an error; relaxtime is the time in minutes
{
    byte errc = 255;                                           // error- and statusidentifiers
    byte stat = 255;
    byte err = 0;
    byte err_h = 0;
    
    unsigned long starttime = (micros() / (1000));             //starttime at relax begin in seconds
    
    while( ((micros() / (1000)) - starttime) < (relaxtime * 60 * 1000) )    // as long as the given time has not exceeded
    {
        //err_h =   write_config();
        //if(err_h != 0)   {COMPRINT("Write Config ERROR: ");         COMPRINTLN(err_h);}
        //err = err + err_h;
        
        err_h =   voltage_get();
        if (err_h != 0)  {COMPRINT("Get Voltage ERROR: ");          COMPRINTLN(err_h);}     // Get Voltage values from Linduino
        err = err + err_h;
                
        err_h =   current_get();
        if (err_h != 0)  {COMPRINT("Get Current ERROR: ");          COMPRINTLN(err_h);}     // Get Current values from Linduino
        err = err + err_h;
                
        /*
        err_h =   temperature_get();
        if (err_h != 0)  {COMPRINT("Get Temperature ERROR: ");      COMPRINTLN(err_h);}     // Get Temperature values from Micros
        err = err + err_h;
        */
        errc = errorcode();                                  // get errorcode
        stat = statuscode();                                 // get statuscode
        
//        if(stat == 34){switch_on_allowed = false;}            // if the battery turns out to be full charged do not switch the status_ok-pin back on to avoid overcharging. restart necessary            
        
        //sd_logdata(stat, errc);
        
        //serial_show_data(errc,stat);                 // Print Voltage, Temperature, Current and/or Status on Serial Monitor

        if((!(errc == 0 || errc == 2)) ||/* stat == 34 ||*/ err != 0){if(err != 0){return err;}else{return 1;}}    // emergency exit; errorcode is allowed to be 0(all ok, just ok-pin low) or 2(overcharged - reason to balance); also gets out if full charged or for get_voltage/current/temperature error
        
    	delay (1000);                                         // wait to slow down the loop
    
    }//end while
  
  return 0;                                                   // return success
}


int write_config()                                            // writes the config on linduino
{
  byte conf[7];                                               // declares the array to be sent
  int err = 0;                                                // errorcode 
  
  for(int stack = 0; stack < number_of_stacks; stack++)       // sends config for each stack; one by one
  {
      conf[0] = ((stack + 1) * 16 + 3);                       // writes the message ident. +3 tells the linduino that the config will be written and stack+1 tells for which stack
  
      for(int i = 0; i <= 5; i++)
      {
           conf[i+1] = config[number_of_stacks - (stack + 1)][i];                      // save the config in the temp array
      }
  
  err = i2c_sendbytes(linduinoadr, 7, conf);                  // sends the config to linduino
  //COMPRINTLN("config_writen");
  //delay(5000);
  
  if ( err != 0 ){ return ((stack + 1) * 10 + err);}          // if communication was not successful return errorcode
  }  return err;                                              // return success
}



void create_config()                                        // prepares the config array with the latest informations
{
    for(int s = 0; s < number_of_stacks; s++)               // repeat for each stack
    {
	config[s][0] = CFGR0_i;                                // standart values; no influence on discharging
	config[s][1] = CFGR1_i;
	config[s][2] = CFGR2_i;
	config[s][3] = CFGR3_i;

	config[s][4] = discharge_cell[s][0] * 1     // writes the discharge information. one bit per cell
                     + discharge_cell[s][1] * 2 
                     + discharge_cell[s][2] * 4 
                     + discharge_cell[s][3] * 8 
                     + discharge_cell[s][4] * 16 
                     + discharge_cell[s][5] * 32 
                     + discharge_cell[s][6] * 64 
                     + discharge_cell[s][7] * 128;
                     
	config[s][5] = CFGR5_i                                             // bit 4-7 are not relevant for discharging
                     + discharge_cell[s][8]  * 1     // writes the discharge information. one bit per cell 
                     + discharge_cell[s][9]  * 2 
                     + discharge_cell[s][10] * 4 
                     + discharge_cell[s][11] * 8;
                     
        //config[0][4] = 0xFF;              
        //config[0][5] = 0xFF; 
    }//end for
}//end create_config


