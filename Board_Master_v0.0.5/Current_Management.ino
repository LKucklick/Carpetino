/*
  todo:
  -   auf dezentrale Strommessung umstellen
      -  Empfang nur eines Ampermesswertes 

*/


int current_get()
{
   unsigned int   curr_v     = 0;        // current sensorvalue from linduino. linduino measures 0-1023 for 49 times and sends a value from 0 - 49*1023=50127. one unit equals 4,9mV so 49 times the analog read equals the voltage in 0,1mV
   unsigned int   curr_vref  = 0;        // vref sensorvalue from linduino. linduino measures 0-1023 for 49 times and sends a value from 0 - 49*1023=50127. one unit equals 4,9mV so 49 times the analog read equals the voltage in 0,1mV
   byte           values[5]  = {};       // temp array to store recieved data
   byte           ident[1]   = {};       // request identifier
   int            err        = 0;        // errorcode
   
   current = ((max_current * -1) - 100);   // set the actual current on a to high value to make sure an error will occur if the communication in not successful

   ident[0] = 4;                           // request ident 4 tells the linduino the with the next request the current value is wanted
  
   err = i2c_sendbytes(linduinoadr, 1, ident);    // sends the ident to linduino
   if(err != 0){return (400 + err);}
  
   delay(80);                                     // gives linduino time to refresh its current value
  
   err = 5 - i2c_requestbytes(linduinoadr, 5, values);    // request the current value from linduino
   if(err != 0){return (400 + 6);}   
            
   ident[0] = values[0];                                  // saves the first recieved byte as ident
            
   if(ident[0] != 4){return (400 + 7);}                   // compares recieved ident with sent one. to identify connection errors
                    
   curr_v = 0;                                            // sets the current value to zero
   curr_v = curr_v + (values[1] * 256);                   // increases the current value by the first byte
   curr_v = curr_v + (values[2] * 1);                     // adds the second byte
   curr_vref = curr_vref + (values[3] * 256);                   // increases the vref value by the first byte
   curr_vref = curr_vref + (values[4] * 1);                     // adds the second byte
   
   //curr_vref = 25000;                                     // sets v_ref to dummy value; will be requested in program versions to come

   current = current_interpolation(curr_v, curr_vref) + current_offset;    // converts the recieved value to real current value
   
   //COMPRINTLN(curr_v);
   //COMPRINTLN(curr_vref);
   //COMPRINTLN(current + current_offset);
   //COMPRINTLN("");

   SOC_state = charge_ocv_discharge();
  
   return err;                                            // return success
}  

int current_check()
{
   int           stat      = 0;        // current status: 0: ok; 1: upper current limit reached; 2: lower current limit reached

  if(current - max_charging_current >= 0)   { stat = stat + 1; }  // Over Charging Current check

  if(current <= -1 * max_current)  { stat = stat + 2; }  // Over Current check
  
  return stat;     // return result 
}

long current_interpolation(unsigned int curr_v, unsigned int curr_vref)          // converts the current value in 100uV to a real value in mA
{
  long curren = 0;                                                               // temp variable

  curren = (((int(curr_v - curr_vref)) * sensor_rated_current) / sensor_factor);      // conversion; curr_v and curr_vref from linduino; sensor rated current and sensor factor from datasheet
  
  return curren;                                                                 // return the current value in mA
}
