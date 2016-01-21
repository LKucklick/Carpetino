int pins_initialize()                                           // Initialize the PIN Configuration on Arduino
{
      pinMode(ledpin, OUTPUT);                                  // sets the digital pin as output
      digitalWrite(ledpin, LOW);                                // sets pin low
      
      pinMode(statuspin, OUTPUT);				// sets the digital pin as output
      set_statuspin_low(255);
      
      pinMode(SS, OUTPUT);                                      // pin 10 for sd card
      
      return 0; 
}


void led_blink(int how_many)                // blinks with pin 13 (see pinconfig)
{
    int i = 0;
    
    for (i = 0; i < how_many; i++)
    {
        digitalWrite(ledpin, HIGH);
	delay (5);
        digitalWrite(ledpin, LOW);
        if (i < how_many - 1){delay (50);}
    }   
}

void set_statuspin_high(byte errc)
{
  digitalWrite(statuspin, HIGH);                              // sets pin low
  status_ok = true; 
  COMPRINT("Status_Pin: HIGH      "); 
  if(errc != 255)
  {
    COMPRINT("Status: " + String(statuscode()+256, BIN));                                       // print status
    COMPRINTLN("       Error: " + String(errc+256, BIN));                                        // print error
  }else{COMPRINTLN("");}
  COMPRINTLN("");
}

void set_statuspin_low(byte errc)
{
  digitalWrite(statuspin, LOW);                              // sets pin high
  status_ok = false;  
  COMPRINT("Status_Pin: LOW       ");
  if(errc != 255)
  {
    COMPRINT("Status: " + String(statuscode()+256, BIN));                                       // print status
    COMPRINTLN("       Error: " + String(errc+256, BIN));                                        // print error
  }else{COMPRINTLN("");}
  COMPRINTLN("");
}
