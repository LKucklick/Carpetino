/*-----------------------------
 * Optinal Output Values (OOV)
 change change change
 -----------------------------*/
   


// Set bits in this variable to get different output
uint8_t   measurement_output_flag=0;     



 // Initialize the communication, but only if it hasn't been done before
int OOV_initialize()                   
{


#ifndef COMPRINT
    #define Serial_or_OneSheeld_Terminal_Mode true //True means Serial Mode is on, False means OneSheeld_Terminal_Mode is on
    #if Serial_or_OneSheeld_Terminal_Mode
    /* Start Serial communication. */
    Serial.begin(115200);
    #define COMPRINT Serial.print
    #define COMPRINTLN Serial.println
    
    
    #else
    /* Start OneSheeld communication. */
    OneSheeld.begin();
    #define COMPRINT Terminal.print
    #define COMPRINTLN Terminal.println
    
    #endif  
#endif
   
    COMPRINTLN("Optinal Output Values (OOV)_init\n");
  
}


void serialEvent()
{
  while(Serial.available())
  {
    char ch = Serial.read();

    COMPRINT(ch);
    COMPRINT(",BIN:");
    COMPRINTLN(ch,BIN);
    
   if( ch == 'A' ) // is this an ascii A?
    {
       COMPRINTLN("");
       COMPRINTLN("-------Mode A is ON----------");
       COMPRINTLN("");
       COMPRINT("Before measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
       bitSet(measurement_output_flag, 0);
       COMPRINT("After measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
    }
    if( ch == 'a' ) // is this an ascii a?
    {
       COMPRINTLN("");
       COMPRINTLN("-------Mode A is OFF----------");
       COMPRINTLN("");
       COMPRINT("Before measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
       bitClear(measurement_output_flag, 0);
       COMPRINT("After measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
    }
    if( ch == 'B' ) // is this an ascii A?
    {
       COMPRINTLN("");
       COMPRINTLN("-------Mode B is ON----------");
       COMPRINTLN("");
       COMPRINT("Before measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
       bitSet(measurement_output_flag, 1);
       COMPRINT("After measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
    }
    if( ch == 'b' ) // is this an ascii a?
    {
       COMPRINTLN("");
       COMPRINTLN("-------Mode B is OFF----------");
       COMPRINTLN("");
       COMPRINT("Before measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
       bitClear(measurement_output_flag, 1);
       COMPRINT("After measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
    }
    if( ch == 'N' ) // is this an ascii N?
    {
       COMPRINTLN("");
       COMPRINTLN("-------All Modes are OFF----------");
       COMPRINTLN("");
       COMPRINT("Before measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
       measurement_output_flag = 0;
       COMPRINT("After measurement_output_flag:");
       COMPRINT(measurement_output_flag);
       COMPRINT(",BIN:");
       COMPRINTLN(measurement_output_flag,BIN);
    }
    else if (ch == 10) // Newline-Symbol? Just in case we need to do something when the Newline-Symbol occurs.
    {
       
       
    }
      
  }
}


void print_chosen_output()
  {
  if( bitRead(measurement_output_flag, 0)) // is this an ascii A?
    {
      COMPRINT("-------Mode A is ON----------");
          }
 if( bitRead(measurement_output_flag, 1)) // is this an ascii A?
    {
      COMPRINTLN("-------Mode B is ON----------");
    }
  }
