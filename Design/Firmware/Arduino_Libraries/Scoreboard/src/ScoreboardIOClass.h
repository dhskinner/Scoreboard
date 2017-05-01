/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _IOCLASS_h
#define _IOCLASS_h

#include <Arduino.h>
#include <ScoreboardDefines.h>
#include <WSUtility.h>

// ---------------------------------------------------------------------------------
#if defined (ESP8266)
// Global function definitions used for ESP
// Only one led at a time can be attached
#include <Ticker.h>
void ESPLEDOn();
void ESPLEDOff();
#endif

class ScoreboardIOLEDClass
{
protected:
    uint8_t _ledPin;
    IOLEDMode _ledMode;
    uint16_t _onMillis;
    uint16_t _offMillis;
public:
    ScoreboardIOLEDClass();
    void Start ( uint8_t ledpin );
    void Process();
    void Flash ( uint16_t interval = IO_LED_FLASH_MS );
    void Blink ( uint16_t on_millis = IO_LED_BLINK_ON_MS, uint16_t off_millis = IO_LED_BLINK_OFF_MS );
    void Off();
    void On();
};
// ---------------------------------------------------------------------------------
#ifndef ESP8266
class ScoreboardIOBatteryClass
{
protected:

public:
    static uint8_t Status();
};
#endif
// ---------------------------------------------------------------------------------
class ScoreboardIOButtonClass
{
protected:

public:
    static void Start();
    static void Process();
};

#endif


