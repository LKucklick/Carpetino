#ifndef Serial_or_OneSheeld_Terminal_Mode
#define Serial_or_OneSheeld_Terminal_Mode true //True means Serial Mode is on, False means OneSheeld_Terminal_Mode is on
#endif




#define VERSION_NUMBER 5



/*
 * WiiChuckDemo mit Kabel + DC Motor wie in fritzing S. 74(oben)  -- 
 *
 * 2008 Tod E. Kurt, http://thingm.com/
 *
 *sieeh
 */
#include <Servo.h>                   // einbinden der Servo Bibliothek (library)
#include <Wire.h>
#include <I2C.h>
#include <Average.h>
#include "config_Mega_Board.h"

#if !Serial_or_OneSheeld_Terminal_Mode

//OneSheeld settings, shields, config and libraries 
#define CUSTOM_SETTINGS
#define INCLUDE_DATA_LOGGER_SHIELD
#define INCLUDE_TERMINAL_SHIELD

/* Include 1Sheeld library. */
#include <OneSheeld.h>
#include "config_OneSheeld.h"

#endif


#include "config_RPM.h"

#include "nunchuck_funcs_I2C.h"
#include "config_optional_output_values.h"



void setup()
{
    int err = 0;
    serial_or_OneSheeld_initialize();
    COMPRINT("COM.begin: ");
    
    #if !Serial_or_OneSheeld_Terminal_Mode
    OneSheeld_datalogger_initialize();
    #endif
    
    nunchuck_initialize();              // in tab "Controller"
    RPM_initialize();
    attachInterrupt(digitalPinToInterrupt(RPM_Sensor_Pin), RPM_ISR, FALLING);
    myservo.attach(9);                  // verbindet das Servoobjekt an Pin 9

    pinMode(pin, INPUT);
    pinMode(digitallichtsensorPin, OUTPUT);
    
     err =   i2c_initialize();
     if (err != 0)  {COMPRINT("Init I2C ERROR: ");           COMPRINTLN(err);}     // Initialize the I2C Communication
}


void loop()
{
    
    //measure_photo_sensor();          //in tab "speed"
    

    
        
    if( loop_cnt > 50 ) { // every 100 msecs get new data
       
        
        #if !Serial_or_OneSheeld_Terminal_Mode
        OneSheeld_log_data();
        #endif
        
        
        loop_cnt = 0;

        controller_get_input();    
        
        RPM_get_data();
        
        print_chosen_output(); 
       }
          
        
    if(valMapped<=15){valMapped = 15;}                       //Unterschwelle bei 15 
         
            
    myservo.write(valMapped);                  // stellt den Servo auf den Wert valMapped
    
    loop_cnt++;
    delay(1);
    
}

