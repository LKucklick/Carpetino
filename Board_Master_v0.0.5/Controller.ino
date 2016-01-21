int nunchuck_initialize()                    // Initialize the Nunchuck Controller
{
      	nunchuck_setpowerpins();
   		nunchuck_init();             // send the initilization handshake
    
    	COMPRINT("WiiChuckDemo ready\n");
        
       	return 0; 
}

int controller_get_input()                    // get input from nunchuck joystick, Potentiometer to controll BLDC-Motor
{
      	nunchuck_get_data();

        // Check if nunchuck input is activated
        zbut = nunchuck_zbutton();
        joyy = nunchuck_joyy();
         #if Serial_or_OneSheeld_Terminal_Mode
        COMPRINT("\tzbut: "); COMPRINT((byte)zbut,DEC);
        COMPRINT("\tjoyy: "); COMPRINT((int)joyy,DEC); 
        #else 
        COMPRINT("\tz: "); COMPRINT(zbut);
        COMPRINT("\tjy: "); COMPRINT(joyy);
        #endif 
        
       
        // Only drive if z-Button is pushed
        if(zbut==1){
         val = joyy;
         valMapped = map(val, 0, 255, 0, 180);                           // Input from nunchuck joystick is normaly from 0 to 255(one byte) and will be converted to 0 to 180(degrees) for the servo
        }
        
        // Use different input here analog Poti
        else {
        valPoti = analogRead(potpin);                                      // Potentiometer-Wert wird ausgelesen
        val= valPoti;
        valMapped = map(val, 0, 1023, 0, 180);                            // Input from Potentiometer is normaly from 0 to 255(one byte) and will be converted to 0 to 180(degrees) for the servo
        }
        
    #if Serial_or_OneSheeld_Terminal_Mode
    
    COMPRINT("\tval: "); COMPRINT((int)val,DEC);
    COMPRINT("\tvalMapped: "); COMPRINT((int)valMapped,DEC);   // this value will be used 
    #else 
    COMPRINT("\tv: "); COMPRINT(val);
    COMPRINTLN("\tvM: "); COMPRINT(valMapped);   // this value will be used  
    #endif
        
        
}


