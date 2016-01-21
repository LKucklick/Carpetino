/*
 * Nunchuck functions  -- Talk to a Wii Nunchuck
 *
 * This library is from the Bionic Arduino course : 
 *                          http://todbot.com/blog/bionicarduino/
 *
 * 2007-11 Tod E. Kurt, http://todbot.com/blog/
 *
 * The Wii Nunchuck reading code originally from Windmeadow Labs
 *   http://www.windmeadow.com/node/42
 */
#define nck_adr  0x52
#define serial_showme_nun_buf  false

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
//#define Wire.write(x) Wire.send(x)
//#define Wire.read() Wire.receive()
#endif



static uint8_t nunchuck_buf[6];   // array to store nunchuck data,

// Uses port C (analog in) pins as power & ground for Nunchuck
static void nunchuck_setpowerpins()
{
#define pwrpin PORTC3
#define gndpin PORTC2
    DDRC |= _BV(pwrpin) | _BV(gndpin);
    PORTC &=~ _BV(gndpin);
    PORTC |=  _BV(pwrpin);
    delay(100);  // wait for things to stabilize        
}

// initialize the I2C system, join the I2C bus,
// and tell the nunchuck we're talking to it
static void nunchuck_init()
{ 
    int err=0;
    I2c.begin();                // join i2c bus as master
    COMPRINT("\terr I2c.write right before the begining: ");
    err=I2c.write(nck_adr,0x40,0x00); //configure device for continuous mode
    //COMPRINT("\terr I2c.write right at the begining: "); COMPRINTLN((int)err,DEC);
    /*
    Wire.beginTransmission(0x52);// transmit to device 0x52
#if (ARDUINO >= 100)
    Wire.write((uint8_t)0x40);// sends memory address
    Wire.write((uint8_t)0x00);// sends sent a 0x00.  
#else
    Wire.send((uint8_t)0x40);// sends memory address
    Wire.send((uint8_t)0x00);// sends sent a 0x00.  
#endif
    Wire.endTransmission(true);// stop transmitting
    */
}

// Send a request for data to the nunchuck
// was "send_0x00()"
static void nunchuck_send_request()
{
    int err=0;
    err=I2c.write(nck_adr,0x00);
    //COMPRINT("\terr I2c.write send_request: "); COMPRINTLN((int)err,DEC);
    /*
    Wire.beginTransmission(0x52);// transmit to device 0x52
#if (ARDUINO >= 100)
    Wire.write((uint8_t)0x00);// sends one byte
#else
    Wire.send((uint8_t)0x00);// sends one byte
#endif
    Wire.endTransmission();// stop transmitting
    */
}

// Encode data to format that most wiimote drivers except
// only needed if you use one of the regular wiimote drivers
static char nunchuk_decode_byte (char x)
{
    x = (x ^ 0x17) + 0x17;
    return x;
}

// Receive data back from the nunchuck, 
// returns 1 on successful read. returns 0 on failure
static int nunchuck_get_data()
{
    int cnt=0;
    int x = 0;
    int y = 0;
    int z = 0;
    int availablei2c = 0;
    I2c.read(nck_adr,6); // request data from nunchuck
    
//    y = I2c.receive() << 8;
//  COMPRINT("\tyy1: "); 
//  COMPRINT((byte)y,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); 
//  COMPRINT((int)availablei2c,DEC);
//  y |= I2c.receive();
//  COMPRINT("\tyy: "); 
//  COMPRINT((byte)y,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); 
//  COMPRINT((int)availablei2c,DEC);
//  x = I2c.receive() << 8;
//  COMPRINT("\tx1: "); COMPRINT((byte)x,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); COMPRINT((int)availablei2c,DEC);
//  x |= I2c.receive();
//  COMPRINT("\tx: "); COMPRINT((byte)x,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); COMPRINT((int)availablei2c,DEC);
//  z = I2c.receive() << 8;
//  COMPRINT("\tz1: "); COMPRINT((byte)z,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); COMPRINT((int)availablei2c,DEC);
//  z |= I2c.receive();
//  COMPRINT("\tz: "); COMPRINT((byte)z,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); COMPRINT((int)availablei2c,DEC);
//  availablei2c = I2c.available();
//  COMPRINT("\tavailablei2c: "); COMPRINT((int)availablei2c,DEC);
//  nunchuck_send_request();  // send request for next data payload
//  delay(200);
  
 
    boolean highbyte_on=true;
    while ( I2c.available()) {
    
      nunchuck_buf[cnt] =   I2c.receive() ;
      
      #if (serial_showme_nun_buf)
      {
        COMPRINT("\tnun_buf["); 
        COMPRINT((int)cnt,DEC);
        COMPRINT("]:");
        //COMPRINT((uint8_t)nunchuck_buf[cnt],BIN);
        COMPRINT("/");
        COMPRINT((uint8_t)nunchuck_buf[cnt],DEC);
      }
      #endif

      
      
      cnt++;
    }
    
    
    
    nunchuck_send_request();  // send request for next data payload
    
    // If we recieved the 6 bytes, then go print them
    if (cnt >= 5) {
        return 1;   // success
    }
    return 0; //failure
    
    /*Wire.requestFrom (0x52, 6);// request data from nunchuck
    while (Wire.available ()) {
        // receive byte as an integer
#if (ARDUINO >= 100)
        nunchuck_buf[cnt] = nunchuk_decode_byte( Wire.read() );
#else
        nunchuck_buf[cnt] = nunchuk_decode_byte( Wire.receive() );
#endif
        cnt++;
    }
    nunchuck_send_request();  // send request for next data payload
    // If we recieved the 6 bytes, then go print them
    if (cnt >= 5) {
        return 1;   // success
    }
    return 0; //failure
    */
}

// Print the input data we have recieved
// accel data is 10 bits long
// so we read 8 bits, then we have to add
// on the last 2 bits.  That is why I
// multiply them by 2 * 2
static void nunchuck_print_data()
{ 
    static int i=0;
    int joy_x_axis = nunchuck_buf[0];
    int joy_y_axis = nunchuck_buf[1];
    int accel_x_axis = nunchuck_buf[2]; // * 2 * 2; 
    int accel_y_axis = nunchuck_buf[3]; // * 2 * 2;
    int accel_z_axis = nunchuck_buf[4]; // * 2 * 2;

    int z_button = 0;
    int c_button = 0;

    // byte nunchuck_buf[5] contains bits for z and c buttons
    // it also contains the least significant bits for the accelerometer data
    // so we have to check each bit of byte outbuf[5]
    if ((nunchuck_buf[5] >> 0) & 1) 
        z_button = 1;
    if ((nunchuck_buf[5] >> 1) & 1)
        c_button = 1;

    if ((nunchuck_buf[5] >> 2) & 1) 
        accel_x_axis += 1;
    if ((nunchuck_buf[5] >> 3) & 1)
        accel_x_axis += 2;

    if ((nunchuck_buf[5] >> 4) & 1)
        accel_y_axis += 1;
    if ((nunchuck_buf[5] >> 5) & 1)
        accel_y_axis += 2;

    if ((nunchuck_buf[5] >> 6) & 1)
        accel_z_axis += 1;
    if ((nunchuck_buf[5] >> 7) & 1)
        accel_z_axis += 2;

    #if Serial_or_OneSheeld_Terminal_Mode

    if (bitRead(measurement_output_flag, 1))
    {   
    if (bitRead(measurement_output_flag, 7))
    {
    COMPRINT(i,DEC);
    COMPRINT("\t");

    COMPRINT("joy:");
    COMPRINT(joy_x_axis,DEC);
    COMPRINT(",");
    COMPRINT(joy_y_axis, DEC);
    COMPRINT("  \t");

    COMPRINT("acc:");
    COMPRINT(accel_x_axis, DEC);
    COMPRINT(",");
    COMPRINT(accel_y_axis, DEC);
    COMPRINT(",");
    COMPRINT(accel_z_axis, DEC);
    COMPRINT("\t");

    COMPRINT("z,cbut:");
    COMPRINT(z_button, DEC);
    COMPRINT(",");
    COMPRINT(c_button, DEC);

    COMPRINT("\r\n");  // newline 
    }
    else 
    {
      COMPRINT("\t");
      COMPRINT("joyY:");
      COMPRINT(joy_y_axis, DEC);
      COMPRINT("  \t");
      COMPRINT("zbut:");
      COMPRINT(z_button, DEC);
    }
    }
    
    #else COMPRINT(i); 
    #endif
      
    

    i++;
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_zbutton()
{
    return ((nunchuck_buf[5] >> 0) & 1) ? 0 : 1;  // voodoo
}

// returns zbutton state: 1=pressed, 0=notpressed
static int nunchuck_cbutton()
{
    return ((nunchuck_buf[5] >> 1) & 1) ? 0 : 1;  // voodoo
}

// returns value of x-axis joystick
static int nunchuck_joyx()
{
    return nunchuck_buf[0]; 
}

// returns value of y-axis joystick
static int nunchuck_joyy()
{
    return nunchuck_buf[1];
}

// returns value of x-axis accelerometer
static int nunchuck_accelx()
{
    return nunchuck_buf[2];   // FIXME: this leaves out 2-bits of the data
}

// returns value of y-axis accelerometer
static int nunchuck_accely()
{
    return nunchuck_buf[3];   // FIXME: this leaves out 2-bits of the data
}

// returns value of z-axis accelerometer
static int nunchuck_accelz()
{
    return nunchuck_buf[4];   // FIXME: this leaves out 2-bits of the data
}
