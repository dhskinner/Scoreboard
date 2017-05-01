/*
 * DEBUGGING
 * battery voltage divider - measure with a multimeter
 * set radio channel
 * look at power management / low power mode / dim the ulcd etc
 * make a clock icon
 * the garbage bin on field 1 and field 2 seems to be a toggle / after erasing it needs to go back to normal (not grey)
 * whats with the flicker when you set the score?
 * some 'back' buttons are red and some are green - make them all same same
 * see if can get rid of 'mounting' on startup
 * Brightness on dual screen for screen 2 doesn;t work - only screen 1 
 *
 * ULCD
 * updates after setting the score are a bit flickery
 * fix the spinner - its still crap
 *
 * FEATURES TO IMPLEMENT
 * set the RF power back to high (add some capacitors to see if it fixes the power issue)
 * make the font14 16 pixels high
 * make the font44 46 pixels high
 * convert the clock on the ulcd to run automatically using a ulcd timer event?
 * add sounds
 * set different kinds of scoreboard types ('real' board, mini etc)
 * save the match start time in eeprom or flash
 * add more controllers - 1 to n
 * add web interface or a relay box from the controller
 * work out how to disable field 2 if unused?
 * add a battery indicator, wireless and offline on each page?
 * add a set the real time clock option
 *
 * DONE
 * the battery icon occasionally goes out of range
 * why does the splash screen flip to the main page so quickly?
 * swap around the controls on the main page because they look silly
 * fix the brightness
 * the brightness slider seems to be 0-7 = make it 0-6
 * make a settings icon
 * something is screwy with field 2 scores on dual field mode
 * fix the battery indicator
 * fix the slight time delay between clocks
 * clean out the serial code / convert to run on a nano
 * fix the timer reset "Yes" button so it goes back to the timer page
 * fix the scoreboard reset button so it actually does does something
 * check the small buttons on Field1 and Field 2 aren;t obscured
 * put all the print and utility functions in a header
 * add an offline mode
 * add 3 digits up to 199
 * add a bugs splash screen
 * fix the skip button
 * field 1 and field 2 pages - change visitors to guests
 * all pages - put the back button in the same place and same color
 * one of the little clock buttons is blue, not green
 * change the backcolor on the sliders to something like black
 */
#include <Arduino.h>
#include "Defines.h"
#include "Config.h"
#include "Scoreboard.h"
#include "Utility.h"
#include <SPI.h>                  // Arduino core library 
#include <Wire.h>                 // Arduino core library 
#include <Streaming.h>            // Arduino core library 
#include <Time.h>                 // Download from http://www.pjrc.com/teensy/td_libs_Time.html  
#include <genieArduino.h>         // Download from https://github.com/4dsystems/ViSi-Genie-Arduino-Library
#ifdef RF_RADIOHEAD 
  #include <RHReliableDatagram.h>   // Download from http://www.airspayce.com/mikem/arduino/RadioHead/
  #include <RH_NRF24.h>             // Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#endif
#ifdef I2C_TRANSFER
  #include <EasyTransferI2C.h>      // Download from https://github.com/madsci1016/Arduino-EasyTransfer
#endif
#ifdef I2C_RTC
  #include <DS1307RTC.h>            // Download from https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
#endif
#ifdef LED_SEGMENT
  #include <LedControlMS.h>         // Download from http://www.instructables.com/id/LED-Matrix-with-Arduino/step2/Arduino-Library/
#endif
#ifdef LED_DMD2
  #include <DMD2.h>				  // Download from https://github.com/freetronics/DMD2
  #include "NUMERAL44x28.h"		  // Fonts
  #include "NUMERAL44x24.h"
  #include "FONT14.h"
#endif  
#ifdef I2C_BATTERY
  #include <LiFuelGauge.h>		  // Download from https://github.com/pAIgn10/LiFuelGauge
#endif
#ifdef DEBUG
  #include "Printf.h"
#endif
#ifdef TEST
//  #include <TimerOne.h>
#endif

// ---------------------------------------------------------------------------------
boolean         result;                 // used generally to capture results of varions operations
boolean         offlineFlag = false;    // flag used to toggle offline mode (used by the controller)
boolean			dmdStartupFlag = true;	// flag used to indicate startup mode (used by the slaves)
Scoreboard      scoreboard[FIELDS];     // scoreboard objects - one per field
// ---------------------------------------------------------------------------------

void setup ( void )
{
  // initialize the scoreboard objects with their field ID
  for ( uint8_t id = 0; id < FIELDS; id++ )
    scoreboard[id].ID ( id );

#ifdef RF_RADIOHEAD
  RF_Start();
#endif

#ifdef I2C_TRANSFER
  I2C_Transfer_Start();
#endif

#ifdef IO_BATTERY
  IO_Battery_Start();
#endif

#ifdef I2C_BATTERY
  I2C_Battery_Start();
#endif

#ifdef I2C_RTC
  I2C_RTC_Start();
#endif

#ifdef LED_DMD2
  DMD_Start();
#endif

#ifdef LED_SEGMENT
  LED_Start();
#endif

#ifdef ULCD
  ULCD_Start();
  ULCD_Update ( true );
#endif

#ifdef TEST
  TestStart();
#endif

// Test the scoreboard
#ifdef SCOREBOARD_MASTER

  // short delay so that the slaves can fire up
  delay ( MASTER_STARTUP_DELAY );
	
  // set the score
  scoreboard[FIELD1].Score ( 199, 199 );
  scoreboard[FIELD2].Score ( 199, 199 );
  I2C_Broadcast ( FIELD1 );
  RF_Broadcast ( FIELD1 );
  I2C_Broadcast ( FIELD2 );
  RF_Broadcast ( FIELD2 );

  // short delay again
  delay ( MASTER_STARTUP_DELAY );

  // Set the scores back to zero
  scoreboard[FIELD1].Score ( 0,0 );
  scoreboard[FIELD2].Score ( 0,0 );
  I2C_Broadcast ( FIELD1 );
  RF_Broadcast ( FIELD1 );
  I2C_Broadcast ( FIELD2 );
  RF_Broadcast ( FIELD2 );
#endif
}

// ---------------------------------------------------------------------------------
void loop ( void )
{
  // Process once per loop
#ifdef RF_RADIOHEAD
  RF_Process();
#endif

#ifdef ULCD
  ULCD_Process();
#endif

#ifdef I2C_TRANSFER
  I2C_Process();
#endif

  // Process once per second
  static time_t updateTime;
  if ( now() != updateTime )
  {
#ifdef ULCD
    ULCD_Update ( false );
#endif

#ifdef I2C_TRANSFER
//    I2C_Broadcast ( second() %2 == 0 ? FIELD1 : FIELD2 );
#endif

#ifdef LED_DMD2
    DMD_Update();
#endif

#ifdef LED_SEGMENT
    LED_Update ( 0 );
#endif

#ifdef SCOREBOARD_MASTER
    RF_Broadcast ( now() % 2 ); // toggle the broadcast between scoreboard 1 and 2 each second
#endif
    updateTime = now();
  }


  #ifdef TEST
	// Process once per second
	static time_t testUpdateTime;
	if ( now () != testUpdateTime )
	{
	  TestProcess();
      testUpdateTime = now();
    }
  #endif
}
// ---------------------------------------------------------------------------------



