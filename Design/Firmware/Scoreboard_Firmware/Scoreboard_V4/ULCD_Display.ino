#ifdef ULCD
// These functions communicate with a 4D Systems Display, configured with ViSi-Genie, utilising the Genie Arduino Library - https://github.com/4dsystems/ViSi-Genie-Arduino-Library.
// The program receives messages from the onscreen objects using the Reported Events. This is triggered each time an object changes on the display, and an event
// is generated and sent automatically. Reported Events originate from the On-Changed event from the object itself, set in the Workshop4 software.
// Application Notes on the 4D Systems Website that are useful to understand this library are found: http://www.4dsystems.com.au/appnotes
// Other good App Notes to read are:
// ViSi-Genie Connecting a 4D Display to an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00017/
// ViSi-Genie Writing to Genie Objects Using an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00018/
// ViSi-Genie A Simple Digital Voltmeter Application using an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00019/
// ViSi-Genie Connection to an Arduino Host with RGB LED Control - http://www.4dsystems.com.au/appnote/4D-AN-00010/
// ViSi-Genie Displaying Temperature values from an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00015/
// ViSi-Genie Arduino Danger Shield - http://www.4dsystems.com.au/appnote/4D-AN-00025/

// Local display state - these store the state of the ulcd as its quicker then retrieving from serial
uint8_t ulcdScore[FIELDS][2];            // current ulcd scores
time_t ulcdTimer[FIELDS];				 // current ulcd timer time
boolean ulcdClock[FIELDS];               // current ulcd clock state
uint8_t ulcdMode[FIELDS];				 // current ulcd mode
uint8_t ulcdBattery;                     // current battery state 0-100
uint8_t ulcdRFSignal;                    // current rf signal quality 0-5
uint8_t ulcdTempScore;                   // temp value used when setting the score
uint8_t ulcdCurrentForm;                 // currently displayed form
uint8_t ulcdTimeoutForm;                 // form to return to after a timeout
uint8_t ulcdReturnToForm;                // previous form (used by dialogs and message boxes to return to previous)
long    ulcdLastEventTime;               // time the last button press or page change occurred
uint8_t ulcdContrast;                    // current ulcd brightness 0=off 15=full brightness
boolean ulcdSwapField;					 // Flag to swap the fields
boolean ulcdDualField;					 // Flag to show dual fields
uint8_t ulcdBrightness;					 // Control the scoreboard brightness
boolean ulcdMute = false;                // boolean to toggle mute or unmute
uint8_t ulcdSpinnerIndex;                // Current image index for the "Working" page. 255 means it is disabled
Genie genie;

// ---------------------------------------------------------------------------------
void ULCD_Start()
{
  // Initialize values
  for ( uint8_t id = 0; id < FIELDS; id++ )
  {
    ulcdScore[id][0] = 255;
    ulcdScore[id][1] = 255;
    ulcdTimer[id] = 255;
    ulcdMode[id] = 255;
    ulcdClock[id] = false;
  }
  ulcdBattery = 255;
  ulcdRFSignal = 255;
  ulcdCurrentForm = ULCD_BLANK_FORM;
  ulcdTimeoutForm = ULCD_MAIN_FORM;
  ulcdReturnToForm = ULCD_MAIN_FORM;
  ulcdLastEventTime = millis();
  ulcdContrast = 15;
  ulcdMute = false;

  // Use Serial for talking to the Genie Library, and to the 4D Systems display
  // 200K Baud is good for most Arduinos. Galileo should use 115200.
#if defined(NANO) || defined (UNO)
  Serial.begin ( ULCD_SERIAL_BAUD );
  genie.Begin ( Serial );
#else
  Serial1.begin ( ULCD_SERIAL_BAUD );
  genie.Begin ( Serial1 );
#endif
  genie.AttachEventHandler ( ULCDGenieEventHandler ); // Attach the user function Event Handler for processing events

  // Reset the Display (change D4 to D2 if you have original 4D Arduino Adaptor)
  // THIS IS IMPORTANT AND CAN PREVENT OUT OF SYNC ISSUES, SLOW SPEED RESPONSE ETC
  // If NOT using a 4D Arduino Adaptor, digitalWrites must be reversed as Display Reset is Active Low, and
  // the 4D Arduino Adaptors invert this signal so must be Active High.
  pinMode ( ULCD_RESETLINE, OUTPUT );   // Set the reset pin on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
#if defined(NANO) || defined(UNO)
  digitalWrite ( ULCD_RESETLINE, LOW ); // reset the Display
  delay ( 100 );                        // hold the pin state for a moment
  digitalWrite ( ULCD_RESETLINE, HIGH ); // unreset the Display
#else
  digitalWrite ( ULCD_RESETLINE, HIGH ); // reset the Display
  delay ( 100 );                        // hold the pin state for a moment
  digitalWrite ( ULCD_RESETLINE, LOW ); // unreset the Display
#endif
  delay ( 3500 );                       // let the display start up after the reset (this is important)
  genie.WriteContrast ( ulcdContrast ); // 0 = Display OFF, though to 15 = Max Brightness ON.

  // Show the splash screen
  ULCDChangeForm ( ULCD_SPLASH_FORM );
}
// ---------------------------------------------------------------------------------
void ULCD_Update ( boolean forceUpdate )
{
  // Update the ulcd for each field
  ULCDUpdateField ( FIELD1 , forceUpdate );
  ULCDUpdateField ( FIELD2 , forceUpdate );

  // Update the battery, RF meter and mode controls
  ULCDUpdateRFSignal();
  ULCDUpdateBattery();
  ULCDUpdateMode();

  // Check whether the splash screen timeout has elapsed
  if ( ulcdCurrentForm == ULCD_SPLASH_FORM )
  {
    if ( ( millis() - ulcdLastEventTime ) > ULCD_SPLASH_TIMEOUT )
      ULCDChangeForm ( ULCD_MAIN_FORM );
  }

  // Check whether any other timeout has elapsed
  if ( ( ulcdCurrentForm != ULCD_MAIN_FORM ) && ( ulcdCurrentForm != ULCD_FIELD1_FORM ) && ( ulcdCurrentForm != ULCD_FIELD2_FORM ) )
  {
    if ( ( millis() - ulcdLastEventTime ) > ULCD_FORM_TIMEOUT )
      ULCDChangeForm ( ulcdTimeoutForm );
  }
}

// ---------------------------------------------------------------------------------
// Update the specified field
void ULCDUpdateField ( uint8_t fieldID, boolean forceUpdate )
{
  // Scores
  for ( uint8_t scoreID = 0; scoreID < 2; scoreID++ )
  {
    if ( ( scoreboard[fieldID].ScoreID ( scoreID ) != ulcdScore[fieldID][scoreID] ) || forceUpdate )
    {
      ulcdScore[fieldID][scoreID] = scoreboard[fieldID].ScoreID ( scoreID );

      if ( fieldID == FIELD1 )
      {
        if ( scoreID == 0 )
          genie.WriteObject ( ULCD_FIELD1_SCORE0_LED, scoreboard[fieldID].ScoreID ( scoreID ) );
        else if ( scoreID == 1 )
          genie.WriteObject ( ULCD_FIELD1_SCORE1_LED, scoreboard[fieldID].ScoreID ( scoreID ) );
      }
      else if ( fieldID == FIELD2 )
      {
        if ( scoreID == 0 )
          genie.WriteObject ( ULCD_FIELD2_SCORE0_LED, scoreboard[fieldID].ScoreID ( scoreID ) );
        else if ( scoreID == 1 )
          genie.WriteObject ( ULCD_FIELD2_SCORE1_LED, scoreboard[fieldID].ScoreID ( scoreID ) );
      }
    }
  }

  // Timer
  ULCDUpdateTimer ( fieldID, forceUpdate );
}
// ---------------------------------------------------------------------------------
void ULCDUpdateRFSignal()
{
  // check if we need to update the RF signal
  if ( ulcdRFSignal != RFSignalQuality() )
  {
    ulcdRFSignal = RFSignalQuality();
    genie.WriteObject ( ULCD_MAIN_SIGNAL, ulcdRFSignal );
    genie.WriteObject ( ULCD_FIELD1_SIGNAL, ulcdRFSignal );
    genie.WriteObject ( ULCD_FIELD2_SIGNAL, ulcdRFSignal );
  }
}
// ---------------------------------------------------------------------------------
void ULCDUpdateBattery()
{
  uint8_t soc = IO_Battery_Status();

  // check if we need to update the battery
  if ( ulcdBattery != soc )
  {
    ulcdBattery = soc;
    genie.WriteObject ( ULCD_MAIN_BATTERY, ulcdBattery );
    genie.WriteObject ( ULCD_FIELD1_BATTERY, ulcdBattery );
    genie.WriteObject ( ULCD_FIELD2_BATTERY, ulcdBattery );
  }
}
// ---------------------------------------------------------------------------------
void ULCDUpdateMode()
{
  // check if we need to update each control
  if ( ulcdSwapField != scoreboard[FIELD1].IsSwapFieldsOn() )
  {
    ulcdSwapField = scoreboard[FIELD1].IsSwapFieldsOn();
    genie.WriteObject ( ULCD_SET_SWAPFIELD_4DBTN, ulcdSwapField ? ULCD_BUTTON_DOWN : ULCD_BUTTON_UP );
  }

  if ( ulcdDualField != scoreboard[FIELD1].IsDualFieldsOn() )
  {
    ulcdDualField = scoreboard[FIELD1].IsDualFieldsOn();
    genie.WriteObject ( ULCD_SET_DUALFIELD_4DBTN, ulcdDualField ? ULCD_BUTTON_DOWN : ULCD_BUTTON_UP );
  }

  if ( ulcdBrightness != scoreboard[FIELD1].Brightness() )
  {
    ulcdBrightness = scoreboard[FIELD1].Brightness();
    genie.WriteObject ( ULCD_SET_BRIGHTNESS_BAR, ulcdBrightness );
  }
}
// ---------------------------------------------------------------------------------
void ULCDUpdateTimer ( uint8_t fieldID, boolean forceUpdate )
{
  uint16_t value = 0;		// set a default value to check if its changed
  uint8_t str = 0;      // string 0 shows the clock

  // first check the clock flag and update the clock button
  if ( ( ulcdClock[fieldID] != scoreboard[fieldID].IsClockOn() ) || forceUpdate )
  {
    ulcdClock[fieldID] = scoreboard[fieldID].IsClockOn();
    if ( fieldID == FIELD1 )
      genie.WriteObject ( ULCD_FIELD1_TIMER_CLOCK_BTN, ( ulcdClock[fieldID] ? ULCD_BUTTON_DOWN : ULCD_BUTTON_UP ) );
    else if ( fieldID == FIELD2 )
      genie.WriteObject ( ULCD_FIELD2_TIMER_CLOCK_BTN, ( ulcdClock[fieldID] ? ULCD_BUTTON_DOWN : ULCD_BUTTON_UP ) );
    forceUpdate = true;
  }

  // then check the string and buttons (and force an update if the mode flag has changed)
  if ( ( ulcdMode[fieldID] != scoreboard[fieldID].MatchTimeMode() ) || forceUpdate )
  {
    // Set the mode value
    ulcdMode[fieldID] = scoreboard[fieldID].MatchTimeMode();

    // Update the strings (clock mode is str=0, 1 = stop, 2 = pause, 3 = run)
    if ( ulcdClock[fieldID] )
      str = ULCD_STRING_CLOCK;
    else
    {
      switch ( scoreboard[fieldID].MatchTimeMode() )
      {
      case FLAG_TIMER_PAUSE:
        str = ULCD_STRING_PAUSE;
        break;

      case FLAG_TIMER_RUN:
        str = ULCD_STRING_RUN;
        break;

      case FLAG_TIMER_STOP:
      default:
        str = ULCD_STRING_STOP;
        break;
      }
    }
    // send the string id to the ulcd
    if ( fieldID == FIELD1 )
    {
      genie.WriteObject ( ULCD_FIELD1_TIMER_SET_STRING, str );
      genie.WriteObject ( ULCD_FIELD1_TIMER_STRNG, str );
    }
    else if ( fieldID == FIELD2 )
    {
      genie.WriteObject ( ULCD_FIELD2_TIMER_SET_STRING, str );
      genie.WriteObject ( ULCD_FIELD2_TIMER_STRNG, str );
    }

    // Update the buttons (these are configured as radio buttons)
    switch ( scoreboard[fieldID].MatchTimeMode() )
    {
    case FLAG_TIMER_PAUSE:
      if ( fieldID == FIELD1 )
        genie.WriteObject ( ULCD_FIELD1_TIMER_PAUSE_BTN, ULCD_BUTTON_DOWN );
      else if ( fieldID == FIELD2 )
        genie.WriteObject ( ULCD_FIELD2_TIMER_PAUSE_BTN, ULCD_BUTTON_DOWN );
      break;
    case FLAG_TIMER_RUN:
      if ( fieldID == FIELD1 )
        genie.WriteObject ( ULCD_FIELD1_TIMER_START_BTN, ULCD_BUTTON_DOWN );
      else if ( fieldID == FIELD2 )
        genie.WriteObject ( ULCD_FIELD2_TIMER_START_BTN, ULCD_BUTTON_DOWN );
      break;
    case FLAG_TIMER_STOP:
    default:
      if ( fieldID == FIELD1 )
        genie.WriteObject ( ULCD_FIELD1_TIMER_STOP_BTN, ULCD_BUTTON_DOWN );
      else if ( fieldID == FIELD2 )
        genie.WriteObject ( ULCD_FIELD2_TIMER_STOP_BTN, ULCD_BUTTON_DOWN );
      break;
    }
    forceUpdate = true;
  }

  // then check the time LED's
  if ( ulcdClock[fieldID] )
  {
    if ( ulcdTimer[fieldID] != now() || forceUpdate )				// check if ulcd is set to system time
    {
      ulcdTimer[fieldID] = now();
      value = hour() * 100;											// hours   (0-23)
      value += minute();											// minutes (0-59)
      forceUpdate = true;
    }
  }
  else
  {
    if ( ulcdTimer[fieldID] != scoreboard[fieldID].MatchTime() || forceUpdate )		// check if ulcd is set to scoreboard time
    {
      ulcdTimer[fieldID] = scoreboard[fieldID].MatchTime();
      value = scoreboard[fieldID].MatchTime() ;
      value = ( scoreboard[fieldID].MatchTime() / 60 ) * 100;		// minutes (0-99)
      value += ( scoreboard[fieldID].MatchTime() % 60 );			// seconds (0-59)
      forceUpdate = true;
    }
  }

  // Finally - if the value has changed then send it to the ulcd!
  if ( forceUpdate )
  {
    if ( fieldID == FIELD1 )
    {
      genie.WriteObject ( ULCD_FIELD1_TIMER_LED, value );
      genie.WriteObject ( ULCD_FIELD1_TIMER_SET_LED, value );
    }
    else if ( fieldID == FIELD2 )
    {
      genie.WriteObject ( ULCD_FIELD2_TIMER_LED, value );
      genie.WriteObject ( ULCD_FIELD2_TIMER_SET_LED, value );
    }
  }
}
#endif
