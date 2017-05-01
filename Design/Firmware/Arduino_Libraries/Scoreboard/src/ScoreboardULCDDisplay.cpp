/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/

#include <ScoreboardULCDClass.h>

#ifndef HW_ESP8266

// ---------------------------------------------------------------------------------
// 
/////////////////////////////////////////////////////////////////////
//
// This is the user's event handler. It is called by genieDoEvents()
// when the following conditions are true
//
//    The link is in an IDLE state, and
//    There is an event to handle
//
// The event can be either a REPORT_EVENT frame sent asynchronously
// from the display or a REPORT_OBJ frame sent by the display in
// response to a READ_OBJ request.
//
// ---------------------------------------------------------------------------------
Genie genie;	

// ---------------------------------------------------------------------------------
void GenieEventHandler ( void )
{
    genieFrame event;
    genie.DequeueEvent ( &event );
    ulcd.LastEvent ( millis() );

    // Process events
    switch ( event.reportObject.object )
    {
    case GENIE_OBJ_FORM:              // Process form changes
        ulcd.ProcessForm ( &event );
        break;

    case GENIE_OBJ_USERBUTTON:
    case GENIE_OBJ_WINBUTTON:        //  Process button presses
    case GENIE_OBJ_4DBUTTON:
    case GENIE_OBJ_ANIBUTTON:
    case GENIE_OBJ_TRACKBAR:
        ulcd.ProcessInput ( &event );
        break;

    default:
        break;
    }
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::Start ( uint8_t hwserial, uint16_t hwserialbaud, uint8_t resetline, bool resetlineinvert )
{
    // Use Serial for talking to the Genie Library, and to the 4D Systems display
    // 200K Baud is good for most Arduinos. Galileo should use 115200. Note that
    // the serial object is selected using compiler macros to avoid issues if hardware
    // is not present (however is an invalid serial port is selected it may clash with
    // any comms on Serial0)
    switch ( hwserial )
    {
    case 1:
#if defined(HAVE_HWSERIAL1)
        Serial1.begin ( hwserialbaud );
		genie.Begin(Serial1);
#endif
        break;

    case 2:
#if defined(HAVE_HWSERIAL2)
        Serial2.begin ( hwserialbaud );
		genie.Begin(Serial2);
#endif
        break;

    case 3:
#if defined(HAVE_HWSERIAL3)
        Serial3.begin ( hwserialbaud );
		genie.Begin(Serial3);
#endif
        break;

    case 0:
    default:
#if defined(HAVE_HWSERIAL0)
        Serial.begin ( hwserialbaud );
		genie.Begin(Serial);
#endif
        break;
    }

    // Attach the user function Event Handler for processing events
    genie.AttachEventHandler ( GenieEventHandler ); //

    // Reset the Display (change D4 to D2 if you have original 4D Arduino Adaptor)
    // THIS IS IMPORTANT AND CAN PREVENT OUT OF SYNC ISSUES, SLOW SPEED RESPONSE ETC
    // If NOT using a 4D Arduino Adaptor, digitalWrites must be reversed as Display Reset
    // is Active Low, and the 4D Arduino Adaptors invert this signal so must be Active High.
    pinMode ( resetline, OUTPUT );								// Set the reset pin on Arduino to Output (4D Arduino Adaptor V2 - Display Reset)
    digitalWrite ( resetline, resetlineinvert ? LOW : HIGH );	// reset the Display
    delay ( 100 );												// hold the pin state for a moment
    digitalWrite ( resetline, resetlineinvert ? HIGH : LOW );	// unreset the Display
    delay ( 3500 );												// let the display start up after the reset (this is important)
    genie.WriteContrast ( ulcdContrast );						// 0 = Display OFF, though to 15 = Max Brightness ON.

    // Initialize values (most are invalid so they update first time)
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

    // Show the splash screen
    this->ChangeForm ( ULCD_SPLASH_FORM );
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::Update ( bool forceUpdate )
{
    // Update the ulcd for each field
    UpdateField ( FIELD1, forceUpdate );
    UpdateField ( FIELD2, forceUpdate );

    // Update the battery, RF meter and mode controls
    UpdateRFSignal();
    UpdateBattery();
    UpdateMode();

    // Check whether the splash screen timeout has elapsed
    if ( ulcdCurrentForm == ULCD_SPLASH_FORM )
    {
        if ( ( millis() - ulcdLastEventTime ) > ULCD_SPLASH_TIMEOUT )
            ChangeForm ( ULCD_MAIN_FORM );
    }

    // Check whether any other timeout has elapsed
    if ( ( ulcdCurrentForm != ULCD_MAIN_FORM ) && ( ulcdCurrentForm != ULCD_FIELD1_FORM ) && ( ulcdCurrentForm != ULCD_FIELD2_FORM ) )
    {
        if ( ( millis() - ulcdLastEventTime ) > ULCD_FORM_TIMEOUT )
            ChangeForm ( ulcdTimeoutForm );
    }
}

// ---------------------------------------------------------------------------------
// Update the specified field
void ScoreboardULCDClass::UpdateField ( uint8_t fieldID, bool forceUpdate )
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
    UpdateTimer ( fieldID, forceUpdate );
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::UpdateRFSignal()
{
    // check if we need to update the RF signal
    if ( ulcdRFSignal != radio.SignalQuality() )
    {
        ulcdRFSignal = radio.SignalQuality();
        genie.WriteObject ( ULCD_MAIN_SIGNAL, ulcdRFSignal );
        genie.WriteObject ( ULCD_FIELD1_SIGNAL, ulcdRFSignal );
        genie.WriteObject ( ULCD_FIELD2_SIGNAL, ulcdRFSignal );
    }
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::UpdateBattery()
{
    uint8_t soc = 0;

#if defined (IO_BATTERY)
    soc = io.BatteryStatus();
#elif defined (I2C_BATTERY)
    soc = i2c.BatteryStatus();
#endif

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
void ScoreboardULCDClass::UpdateMode()
{
    // check if we need to update each control
    if ( ulcdSwapField != scoreboard[FIELD1].Swap() )
    {
        ulcdSwapField = scoreboard[FIELD1].Swap();
        genie.WriteObject ( ULCD_SET_SWAPFIELD_4DBTN, ulcdSwapField ? ULCD_BUTTON_DOWN : ULCD_BUTTON_UP );
    }

    if ( ulcdDualField != scoreboard[FIELD1].Dual() )
    {
        ulcdDualField = scoreboard[FIELD1].Dual();
        genie.WriteObject ( ULCD_SET_DUALFIELD_4DBTN, ulcdDualField ? ULCD_BUTTON_DOWN : ULCD_BUTTON_UP );
    }

    if ( ulcdBrightness != scoreboard[FIELD1].Brightness() )
    {
        ulcdBrightness = scoreboard[FIELD1].Brightness();
        genie.WriteObject ( ULCD_SET_BRIGHTNESS_BAR, ulcdBrightness );
    }
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::UpdateTimer ( uint8_t fieldID, bool forceUpdate )
{
    uint16_t value = 0;		// set a default value to check if its changed
    uint8_t str = 0;      // string 0 shows the clock

    // first check the clock flag and update the clock button
    if ( ( ulcdClock[fieldID] != scoreboard[fieldID].Clock() ) || forceUpdate )
    {
        ulcdClock[fieldID] = scoreboard[fieldID].Clock();
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
            value = hour() * 100;										// hours   (0-23)
            value += minute();											// minutes (0-59)
            forceUpdate = true;
        }
    }
    else
    {
        if ( ulcdTimer[fieldID] != scoreboard[fieldID].MatchTime() || forceUpdate )		// check if ulcd is set to scoreboard time
        {
            ulcdTimer[fieldID] = scoreboard[fieldID].MatchTime();
            value = scoreboard[fieldID].MatchTime();
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