/*

 */
#ifndef Serial_or_OneSheeld_Terminal_Mode
#define Serial_or_OneSheeld_Terminal_Mode true //True means Serial Mode is on, False means OneSheeld_Terminal_Mode is on
#endif



/* Reserve a counter. */
int counter = 0;
/* Boolean to start logging. */ 
bool startFlag = false;




#ifndef COMPRINT
int serial_or_OneSheeld_initialize()                    // Initialize the Serial Monitor Communication
{
    
          
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
    
    
    if(Serial_or_OneSheeld_Terminal_Mode){
    COMPRINTLN("");
    COMPRINTLN("");
    COMPRINT("Board_Master_v0.0.");
    COMPRINTLN(VERSION_NUMBER);
    COMPRINTLN("SerialMode.begin\n");}
    else{
    COMPRINTLN("");
    COMPRINTLN("");
    COMPRINT("Board_Master_v0.0.");
    COMPRINTLN(VERSION_NUMBER);
    COMPRINTLN("OneSheeld.begin\n");}
    
}
#endif
int OneSheeld_datalogger_initialize()                    // OneSheeld_datalogger
{
      
  /* Save any previous logged values. */
  Logger.stop();
  COMPRINT("OneSheeld begin and old data saved!");
}

int OneSheeld_log_data()                    // OneSheeld_datalogger
{
  valPoti = analogRead(potpin);
 /* Check if Poti is over certain Value. */
  if(valPoti > 500)       //TODO change me to nunchuck c button or other datalogging condition
  {
    digitalWrite(ledPin,HIGH);
     if(!startFlag)
    {
    /* First insure to save previous logged values. */
    Logger.stop();
    #if !Serial_or_OneSheeld_Terminal_Mode
    /* Set a delay. */
    OneSheeld.delay(50);
    #endif
    /* Start logging in a new CSV file. */
    Logger.start("Test:DataLogger__Terminal__analogRead");
    /* Set startFlag. */
    startFlag = true;
    }
  }
  else
  {
    /* Turn off the LED. */
    digitalWrite(ledPin, LOW);
  }
  /* Check logging started. */
  if(startFlag)
  {
    /* Add noise level values as a column in the CSV file. */
    Logger.add("valPoti",valPoti);
    Logger.add("startFlag",startFlag);
    /* Log the row in the file. */
    Logger.log();  
    #if !Serial_or_OneSheeld_Terminal_Mode
    /* Delay for 1 second. */
    OneSheeld.delay(50);
    #endif
    /* Increment counter. */
    counter++;
    /* Stop logging after 20 readings and save the CSV file. */
    if(counter==10)
    {
      Logger.add("End","End");
      /* Save the logging CSV file. */
      Logger.stop();
      /* Reset counter. */
      counter=0;
      /* Start Logging again. */
      Logger.start("Zwischen Stopp Datei");
      startFlag = false;
    }
   }
}
