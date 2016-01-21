#ifndef config_H
#define config_H

//software parameter
#define refreshtime 1000
boolean switch_on_allowed;            //if the status is ok after startup status_ok will be switched true. if an error occurs status_ok will be switched false. if switch_on_allowed == true status_ok will be set back on true when error is gone. if false, status_ok will stay false after error
boolean serial_show_voltage;
boolean serial_show_temperature;
boolean serial_show_status;
boolean serial_show_dischargecell;

double relaxtime = 5;                // relax timeout in minutes in between charging and balancing or charging and more charging
boolean tannenbaumeffect = false;     // allows a discharged cell after dischaging was turned off to be discharged again if the voltage increased again
unsigned int balancing_aim = 0;  // the limit for discharging. balancing a cell will be stopped when limit is reached. set 0 for voltage_min


//  pin configuration
int ledPin  									=	13 	;	// todo: pinconfig anpassen
int potpin  									=	A0 	;	// Potentiometer-Pin wird deklariert
int rpmPin  									=	2 	;
int pin     									=	7 	;
int lichtsensorPin          					=	1 	;
int digitallichtsensorPin   					=	13 	;
const int statuspin			        			=	2 	;	// status_ok pin 2
const int ledpin                                =	13 	;	// Status LED on Pin 13
const int chipSelect                            =	10 	;	// for sd card



// initialize global variables
int     loop_cnt            = 0     ;
int     val,valMapped       = 50    ;                             // val speichert den Wert des Potentiometers zwischen
byte    zbut;
int     joyy, joyyZero ,valPoti ,joyyApp                    = 0;
int     value               								= 0;
unsigned long rpmVal,innerduration, outerduration           = 0;
unsigned long time,time_last,times,times_last,rpmValSum     = 0;
unsigned long previousMillis 								= 0;        // will store last time LED was updated
const long	  interval = 1000;        	  								// interval at which to blink (milliseconds)


//stack information

#define nbr_of_stacks 1                                                         // Number of Stacks in Battery
#define nbr_of_cells 5                                                          // Number of Cells in Stack
#define nbr_of_current_sensors 1                                                // Number of Current Sonsors in Battery
#define nbr_of_cells_parallel  2                                                // Number of Cells parallel

int number_of_stacks                      =       nbr_of_stacks;                // Number of Stacks in Battery
int number_of_cells                       =       nbr_of_cells;                 // Number of Cells in Stack
int number_of_current_sensors             =       nbr_of_current_sensors;       // Number of Current Censors in Battery
int number_of_cells_parallel              =       nbr_of_cells_parallel;        // Number of Cells parallel


// Limits

long         myAmp_hours_full                   =       10000000 * number_of_cells * number_of_cells_parallel;  //rated by the manufacturer 37 Wh / 3.7V = 10 Ah = 10E6 µAh => 100 Ah in stack
unsigned int over_voltage		        		=		42000;		// 100µV
unsigned int under_voltage						=		27000;		// 100µV
unsigned int full_charge_voltage                =       42000;          // 100µV
unsigned int voltage_full_charge_tolerance      =       50;             // 100µV
unsigned int empty_voltage                      =       27000;          // 100µV
int          over_temperature					=		60;  		// degrees C
int          under_temperature					=      -20;  		// degrees C
long         max_charging_current	        	=       30000;		// mA
long         max_current		        		=       251000;         // mA
unsigned int voltage_plausible_tolerance        =       15000;           //volt is plausible when maximum is not bigger than average + tolerance and minimum not smaller than average - tolerance
int          temperature_tolerance              =       20;              //temp is plausible when maximum is not bigger than average + tolerance and minimum not smaller than average - tolerance



// Measure values

long current; //                                                =        0;                                              // Current Current Value in mA
unsigned int cell_voltages[nbr_of_stacks][nbr_of_cells]     =        {};	                                     // array to store voltage data in 100µV
unsigned int total_voltage[nbr_of_stacks]                   =        {};                                             // Voltage of Stacks in mV
unsigned int battery_voltage                                =        0;                                              // Voltage of whole battery in 10mV

// Configuration register - controls the discharge of specific cells

uint8_t CFGR0_i					=	0xFC;                                                           // ???
uint8_t CFGR1_i					=	0x00;                                                           // ???
uint8_t CFGR2_i					=	0x00;                                                           // ???
uint8_t CFGR3_i					=	0x00;                                                           // ???
uint8_t CFGR4_i					=	0x00;								// Cells 1 - 8,		0x 0 {Cell1, Cell2,  Cell3,  Cell4} 0 {Cell5, Cell6, Cell7, Cell8}
uint8_t CFGR5_i					=	0xF0;								// Cells 9 - 12		0x 0 {Cell9, Cell10, Cell11, Cell12} first four bits discharge timeout (0x0 - 0xF = 0 0.5 1 2 3 4 5 10 15 20 30 40 60 75 90 120 minutes)
uint8_t config[nbr_of_stacks][6]                =	{};		                                                // configuration register to be written
//uint8_t r_config[nbr_of_stacks][8];	

byte discharge_cell[nbr_of_stacks][nbr_of_cells] = {};    // array to store information if a single cell needs to be discharged;											// received configuration register


// Variables for voltage measurement

uint8_t reg					=	0;								// registers for cell measurement; 0: all cells
uint8_t total_ic				=	nbr_of_stacks;							// number of modules/stacks
uint8_t pin_state				=	0;                                                              // ???

#define linduinoadr 60          //slave adress of linduino

// Variables for Current measurement

long sensor_rated_current = 200000;
unsigned int sensor_factor = 6250;
long current_offset;
long current_zero_tolerance = 500;        //the sensor never shown really 0 when it should so everything 0+/- current_zero_tolerance counts as 0.
int current_offset_samples = 25;          // number of samples to determine current offset. set 0 for no offset

// Variables for SOC determination

byte SOC_state;
byte SOC_DCC;
byte SOC_OCV_average;
byte SOC_OCV[nbr_of_stacks][nbr_of_cells]     =        {};

   // Variables for Time management

  long ocv_timer;
  long ccc_counter;
  long ocv_relax_time;
  
  boolean dcc_continuity;
  boolean ocv_continuity;
  
  // Variables Time management in DCC to SOC function
  
  long time_dif_dcc_SOC;
  long time_last_dcc_SOC;
  
  // Variables Columb Counting in DCC to SOC function
  
  long myAmp_hours_counter_dcc;          //measuring solution ((+- 200 A/1024bit) * 0.5s)/3600 s/h=54.25 µAh/bit. unsigned long is to use
  long myAmp_hours_gauge_dcc;
  
  
  
  
  


// Program status identifiers
boolean status_ok;
boolean status_full_charged;
boolean status_empty;
//boolean statuspin_high;
boolean status_is_balancing;
boolean status_is_relaxing;

int allowed_errors = 2;
int errorcount;


// create objects

Servo myservo;                       		// erzeugt ein Servo-Objekt



#endif                                                        // ???
