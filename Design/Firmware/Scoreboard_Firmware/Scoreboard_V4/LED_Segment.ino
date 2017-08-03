#ifdef LED_SEGMENT  
// ---------------------------------------------------------------------------------
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn 
 pin 11 is connected to the CLK 
 pin 10 is connected to LOAD 
 We have only a single MAX72XX.
 */
// LedControl lc = LedControl(12,11,10,1);
// LedControl lc = LedControl(11, 13, 10, 1);
// LedControl lc = LedControl(7, 6, 5, 1);
// LedControl lc = LedControl(4, 3, 2, 1);
LedControl lc = LedControl(4, 2, 3, 1);
boolean ledClock = false;

// ---------------------------------------------------------------------------------
boolean LED_Start()
{
  // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  lc.shutdown(0,false);
  
  // Set the brightness to a medium values and clear the display 
  lc.setIntensity(0,8);
  lc.clearDisplay(0);

  for (int i = 0; i < LED_DIGITS; i++)
    lc.setDigit(0, i, 8, true); 
    
  delay(1000);
  lc.clearDisplay(0);
  
  /*
  for (int i = 0; i < LED_DIGITS + LED_MAX_DIGIT; i++)
  {
    LEDScrollDigits();
    delay(100);
  }
  lc.clearDisplay(0);
  */
  
  return true;
}
// ---------------------------------------------------------------------------------
void LED_Update(uint8_t fieldID)
{
  if( ledClock != scoreboard[FIELD_ID].IsFlagSet( FLAG_CLOCK_ON ) )
  {
    ledClock = scoreboard[FIELD_ID].IsFlagSet( FLAG_CLOCK_ON );
    lc.clearDisplay(0);
  }
  
  // update the LED 
  if ( ledClock )
    LEDDisplayTimeHHMMSS( now(), 0 );    
  else
    LEDDisplayTimeMMSS( scoreboard[fieldID].MatchTime(), 2 );
}
// ---------------------------------------------------------------------------------
// Display up to 99 minutes and 59 seconds
void LEDDisplayTimeMMSS(time_t t, uint8_t index) 
{ 
  // Set the seconds - between 0 and 59  
  uint16_t temp = second(t);
  lc.setDigit(0, index,   uint8_t(temp%10), false);       // <- the last boolean is to show decimal 
  lc.setDigit(0, index+1, uint8_t(temp/10), false); 
    
  // Set the minutes - between 0 and 99 
  temp = minute(t) + hour(t) * 60;
  temp = constrain (temp, 0, 99); 
  lc.setDigit(0, index+2, uint8_t(temp%10), true);
  if (temp > 9)
    lc.setDigit(0, index+3, uint8_t(temp/10), false);
  else
    lc.setChar(0, index+3, '0', false);  
}
// ---------------------------------------------------------------------------------
// Display hours, minutes and seconds
void LEDDisplayTimeHHMMSS(time_t t, uint8_t index) 
{ 
  // Set the seconds - between 0 and 59  
  uint16_t temp = second(t);
  lc.setDigit(0, index,   uint8_t(temp%10), false);       // <- the last boolean is to show decimal 
  lc.setDigit(0, index+1, uint8_t(temp/10), false); 
 
  // Set the minutes - between 0 and 59  
  temp = minute(t);
  lc.setChar (0, index+2, '-', false); 
  lc.setDigit(0, index+3, uint8_t(temp%10), false);
  if (temp > 9)
    lc.setDigit(0, index+4, uint8_t(temp/10), false);
  else
    lc.setChar(0, index+4, '0', false);  
    
  // Set the hours - between 0 and 23  
  temp = hour(t); 
  lc.setChar (0, index+5, '-', false); 
  lc.setDigit(0, index+6, uint8_t(temp%10), false);
  if (temp > 9)
    lc.setDigit(0, index+7, uint8_t(temp/10), false);
  else
    lc.setChar(0, index+7, '0', false); 
}
// ---------------------------------------------------------------------------------
// Display the score up to 99
void LEDDisplayScore(uint8_t score, uint8_t index) 
{ 
  // Set the score between 0 and 99 
  score = constrain(score, MIN_SCORE, MAX_SCORE);

  lc.setDigit(0, index, uint8_t(score%10), false);        // first digit
  if (score > 9)  
    lc.setDigit(0, index+1, uint8_t(score/10), false);    // second digit
  else
    lc.setChar(0, index+1, ' ', false);     
}
// ---------------------------------------------------------------------------------
void LEDTest() 
{ 
  for(int i = 0; i < LED_DIGITS; i++) 
      lc.setDigit(0, i, i, false);
}
// ---------------------------------------------------------------------------------
// This method will scroll all the hexa-decimal  numbers and letters on the display. 
// You will need at least four 7-Segment digits. otherwise it won't really look that good.
void LEDScrollDigits() 
{  
  static int scrollIndex = 0;
  for(int i = 0; i < LED_DIGITS; i++) 
  {
    if ( (scrollIndex - i >= LED_MIN_DIGIT) &&  (scrollIndex - i < LED_MAX_DIGIT) )
      lc.setDigit(0, i,  scrollIndex - i, false);
    else 
      lc.setChar(0, i, ' ', false);
  }
  scrollIndex = scrollIndex >= LED_MAX_DIGIT + LED_DIGITS ? 0 : scrollIndex+1;
}
#endif
