/*

 */
#ifndef Serial_or_OneSheeld_Terminal_Mode
#define Serial_or_OneSheeld_Terminal_Mode true //True means Serial Mode is on, False means OneSheeld_Terminal_Mode is on
#endif

#ifndef RPM_printOut_Mode
#define RPM_printOut_Mode true //True means Mode is on
#endif

// the RPM RPM_Sensor_Pin is connected to pin D2 and will be set as a PullUp resistor in the init
#define RPM_Sensor_Pin 2 // TODO put this in the pinconfig

/* Reserve a counter. */
volatile int RPM_counter = 0;
int RPM_last_counter     = 0;
int RPM_counter2         = 0;

uint32_t RPM_lastTime    = 0;   // the last time the interrupt was triggered
uint32_t RPM_time_diff   = 0;
uint32_t RPM_now         = 0;
uint32_t RPM_all_counts  = 0;
uint32_t RPM_lastDistance  = 0;
uint8_t  RPM_constant_CtD  = 12; //ca. 12mm per count
uint32_t RPM_Distance  = 0;
float    RPM_avg_velocity = 0.0;
// EXAMPLE DELETE 
///* A name for the LED on pin 13. */
//const int RPM_Sensor_PinPin = A0;    // pin that the RPM_Sensor_Pin is attached to
//int ledPin = 13;
/* Boolean to start logging. */ 
bool RPM_startFlag = false;
// EXAMPLE DELETE TILL HERE



 // Initialize the Input and Output
int RPM_initialize()                   
{

#ifndef COMPRINT
#if Serial_or_OneSheeld_Terminal_Mode
/* Start Serial communication. */
Serial.begin(115200);
#define COMPRINT Serial.print
#define COMPRINTLN Serial.println

#else
//    /* Start OneSheeld communication. */
//    OneSheeld.begin();
//    #define COMPRINT Terminal.print
//    #define COMPRINTLN Terminal.println

#endif
#endif    
    pinMode(RPM_Sensor_Pin, INPUT);
    digitalWrite(RPM_Sensor_Pin, HIGH); // schaltet den Pullup-Widerstand ein
    //pinMode(LED, OUTPUT);
    COMPRINTLN("RPM_init\n");
  
}

// time and counts
int RPM_get_data()                     
{
  
  RPM_now         = micros();                 //TODO 
  RPM_time_diff   = RPM_now - RPM_lastTime;
  RPM_lastTime    = RPM_now;
  
  
  //disabele interrupts to not get any interferences
  uint8_t oldSREG = SREG;                     //save old interrupt register
  cli();                                      //disabele interrupts
  RPM_last_counter = RPM_counter;             //save value in different variable
  RPM_counter = 0;
  SREG = oldSREG;                             //restore interrupt

  
  
  //store counts
  RPM_all_counts = RPM_all_counts + RPM_last_counter;



  //calculate distance[mm] since last measurement
  RPM_lastDistance = RPM_constant_CtD * RPM_last_counter;
  
  //store distance
  RPM_Distance = RPM_Distance + RPM_lastDistance;


  //average velocity
  RPM_avg_velocity = RPM_lastDistance / RPM_time_diff;

  //digitalWrite(LED, zustand);
  
  
  
    
}

// Print out RPM values
void RPM_print()
  {
  COMPRINT("RPM_now: ");
  COMPRINT(RPM_now);
  COMPRINT("\t RPM_time_diff: ");
  COMPRINT(RPM_time_diff);
  
  COMPRINT("\t RPM_last_counter: ");
  COMPRINT(RPM_last_counter);
  COMPRINT("\t RPM_all_counts: ");
  COMPRINT(RPM_all_counts);
  
  COMPRINT("\t RPM_lastDistance: ");
  COMPRINT(RPM_lastDistance);
  COMPRINT("\t RPM_Distance: ");
  COMPRINT(RPM_Distance);

  COMPRINT("\t RPM_avg_velocity: ");
  COMPRINT(RPM_avg_velocity);
  }

// Interrupt-Service-Routine
void RPM_ISR()
  {
  RPM_counter++;
  }
