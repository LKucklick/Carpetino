int measure_photo_sensor()
{
    value = analogRead(lichtsensorPin);
    if(value<450){
            digitalWrite(digitallichtsensorPin, HIGH);
            times = (micros() - times_last);
            times_last = micros();
        } else {digitalWrite(digitallichtsensorPin, LOW);}
}



int measure_speed()                    // Measure Speed and print it out
{
//      	COMPRINT("\tmicro: "); COMPRINT((int)micros(),DEC);
//        
//        innerduration = pulseIn(pin, HIGH, 50000);
//        COMPRINT("\tinnerduration: "); COMPRINT((int)innerduration,DEC);
//        
//        outerduration = 7*innerduration;
//        if(outerduration == 0){ rpmVal = 0;   }
//        else {
//            rpmVal = 60 * (1000000/outerduration);
//            //rpmVal = ​60*(1000000/outerduration);  // time has micro seconds unit
//        }
//        
//        
//        COMPRINT("\touterduration: "); COMPRINT((int)outerduration,DEC);
//        COMPRINT("\trpmVal: "); COMPRINT((int)rpmVal,DEC);
//        COMPRINT("\ttimes: "); COMPRINT((int)times,DEC);
//        COMPRINT("\tlichtsensor: "); COMPRINTLN((int)value,DEC);
//       	
        return 0; 
}
