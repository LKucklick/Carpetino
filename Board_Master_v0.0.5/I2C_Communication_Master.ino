
int i2c_initialize()                    // Initialize the I2C Communication
{
       Wire.begin();                    // begin i2c communication as a master
       return 0;                        // return success
}


int i2c_sendbytes(int adr, int how_many, byte stringtosend[])       // send bytes to a slave; takes the adress, how many bytes are to be sent and the bytes itself
{
  Wire.beginTransmission(adr);                                      // begin a transmission to the slave
  
  delay(10);                                                        // wait for the slave to get ready
  
  if(Wire.write(stringtosend, how_many) != how_many){return 5;}     // writes the bytes to the wire cache
  
  int err = Wire.endTransmission(true);                             // sends the bytes to the slave
  
  delay(50);                                                        // waits for the slave to process the data
  
  return err;                                                       // return success
}


int i2c_requestbytes(int adr, int how_many, byte *bytes)
{
  byte buff = 0;
  int cnt = 0;
  
  Wire.requestFrom(adr, how_many, true);
  
  delay(10);
  
  if(Wire.available() != how_many){return -1;}
  
  while(Wire.available())
  {
     buff = Wire.read();
     bytes[cnt] = buff;
     cnt++;
     if(cnt > 100){return -2;} 
  }
  
  if(cnt != how_many){return -3;}
  
  return cnt;
}


