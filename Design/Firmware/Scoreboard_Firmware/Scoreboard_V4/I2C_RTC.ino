
#ifdef I2C_RTC
// ---------------------------------------------------------------------------------
// Set the system time from the RTC
boolean I2C_RTC_Start()
{
  tmElements_t te;
  time_t tm;

  // Get current time
  if ( RTC.read ( te ) )
  {
    tm = RTC.get();
    setTime ( tm );
    /*
    #ifdef DEBUG
    	Serial << F("Real time clock detected ");
    	printTime(tm);
    	Serial << F("Synched system time to   ");
    	printTime(tm);
    #endif
    */
    return true;
  }
  else
  {
    /*
    #ifdef DEBUG
    	if (RTC.chipPresent())
    	  Serial.println(F("Real time clock is stopped. Please run the SetTime example to initialize the time and begin running."));
    	else
    	  Serial.println(F("Real time clock read error. Please check the circuitry."));

    	Serial.print(F("Current system time is "));
    	printTime(now());
    #endif
    */
    return false;
  }
}

#endif
