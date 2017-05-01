/*
  Name:		UtilityTest.ino
  Created:	12/10/2015 9:09:18 AM
  Author:	Dave
*/

// Remember to set SERIAL_DEBUG in Utility.h
#define LED_PIN 13

#include <Arduino.h>
#include <Time.h>
#include <Utility.h>



time_t updateTime = 0;

// ------------------------------------------------------------
void setup()
{
  // Define  pin to show a heartbeat LED
  UHEARTBEAT_BEGIN(LED_PIN);

  // Start serial debugging statements
  UBEGIN ( 115200 );
  UPRINTLN ( F ( "Utility Library Test Sketch" ) );
}
// ------------------------------------------------------------
void loop()
{
  if ( now() != updateTime )
  {
    // Stream something
    USTREAM ( F("The ") << F("time ") <<  F("is ") );

    // Print something
    Utility.PrintTime(now());

    // Update the heartbeat LED
    UHEARTBEAT_UPDATE(LED_PIN);
    
    updateTime = now();
  }
}
