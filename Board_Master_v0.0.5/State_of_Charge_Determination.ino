
//Ladezustandsbestimmung

byte charge_ocv_discharge()
  {
    
    if(current - current_zero_tolerance > 0)
    {
      ocv_timer = 0;
      SOC_state = 1; //Closed Circuit Charge State is on, maybe todo: divide in recuperation and charge
    }
    
    if(current + current_zero_tolerance < 0) //(DCC)
    {
      ocv_timer = 0;
      
      //check if last step was in discharge state
      if(SOC_state==2){
          dcc_continuity = true;
      }
      else{
          dcc_continuity = false;
      }
      
      SOC_state = 2; //Closed Circuit Discharge(DCC) State is on 
    }
    
    else
    {
      //check if last step was in pseudo open circuit state
      if(SOC_state==0){
          ocv_continuity = true;
      }
      else{
          ocv_continuity = false;
          ocv_timer = (micros()/1000);
      }
      
      SOC_state = 0; //?Pseudo? Open Circuit State is on 
    }

      switch (SOC_state) {
      case 1:         //Closed Circuit Charge State no determination is made, may be add a recuperation and charge mehtod
      ccc_counter++;
      return 199;
      break;
      case 2:         //Closed Circuit Discharge State(DCC) Columb Counting is used 
        discharge_columb_counting_to_SOC();
      return(SOC_DCC);
      break;
      default:        //todoQ ?Pseudo? Open Circuit State(OCV) or Pseudo Ruhespannungsmessung?, if current is under a certain tolerance?
      
      if((micros()/1000) - ocv_timer > ocv_relax_time) //let the cell's voltage relax for a resonable time
      {
         
         ocv_to_SOC();                
         
         
         return(SOC_OCV_average);
      }
      return(SOC_DCC);
      break;
      }


  }






int discharge_columb_counting_to_SOC()  
{
  //time step
  if(dcc_continuity == true){
      
      long time_now_dcc_SOC = micros();
      time_dif_dcc_SOC      = time_now_dcc_SOC - time_last_dcc_SOC;
      time_last_dcc_SOC     = time_now_dcc_SOC;

  } else {
      
      time_dif_dcc_SOC      = refreshtime;                                          // maybe todo: look for other solution

  }
  //current consumption in time step
  //note: beware of the low time resolution

  myAmp_hours_counter_dcc = myAmp_hours_counter_dcc + current *  time_dif_dcc_SOC;

  myAmp_hours_gauge_dcc   = myAmp_hours_full - myAmp_hours_counter_dcc;             // todo: change myAmp_hours_full from constant value to measured capacity

  SOC_DCC                 = byte((myAmp_hours_gauge_dcc * 100) / myAmp_hours_full);        // SOC_DCC in percentage
}

int ocv_to_SOC()
{
  byte SOC_OCV_sumup = 0;
//Fit the Open Circuit Voltage (ocv) to corresponding SOC just an example
  // out[] holds the values wanted in percentage
  int out[] = {0,5,10,25,52,80,85,90,100};

  // in[] holds the measured Volts values for defined distances
  // note: the values in array should have increasing values
  int in[]  = { 31860, 36190, 36730, 37520, 38310, 40050, 40420, 40870, 41710};

  for(int s = 0; s < number_of_stacks; s++)
        {
            for(int c = 0; c < number_of_cells; c++)
            {
                val = cell_voltages[s][c];
                SOC_OCV[s][c] = byte(multiMap(val, in, out, 9));
                SOC_OCV_sumup = SOC_OCV_sumup + SOC_OCV[s][c];
            }
        }
  SOC_OCV_average = SOC_OCV_sumup / (number_of_stacks * number_of_cells);
}

int multiMap(int val, int* _in, int* _out, uint8_t size)
{
  // take care the value is within range
  // val = constrain(val, _in[0], _in[size-1]);
  if (val <= _in[0]) return _out[0];
  if (val >= _in[size-1]) return _out[size-1];

  // search right interval
  uint8_t pos = 1;  // _in[0] allready tested
  while(val > _in[pos]) pos++;

  // this will handle all exact "points" in the _in array
  if (val == _in[pos]) return _out[pos];

  // interpolate in the right segment for the rest
  return (val - _in[pos-1]) * (_out[pos] - _out[pos-1]) / (_in[pos] - _in[pos-1]) + _out[pos-1];
} 


