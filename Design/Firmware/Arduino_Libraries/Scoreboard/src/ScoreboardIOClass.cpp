/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardIOClass.h"

// ---------------------------------------------------------------------------------
#if defined (ESP8266)

Ticker _ledTicker;
uint8_t _ledTickerPin = 255;
uint16_t _ledTickerOnMillis = 0;
uint16_t _ledTickerOffMillis = 0;

void ESPLEDOn()
{
    bool state = digitalRead ( _ledTickerPin );	// get the current state of GPIO pin
    if ( state == IO_LED_STATE_OFF )				// if off, turn on. If already on, do nothing and wait for the next time round
    {
        digitalWrite ( _ledTickerPin, IO_LED_STATE_ON );
        _ledTicker.attach_ms ( _ledTickerOnMillis, ESPLEDOff );
    }
}
void ESPLEDOff()
{
    digitalWrite ( _ledTickerPin, IO_LED_STATE_OFF );
    _ledTicker.attach_ms ( _ledTickerOffMillis, ESPLEDOn );
}
#endif
// ---------------------------------------------------------------------------------
ScoreboardIOLEDClass::ScoreboardIOLEDClass()
{
    _ledPin = 255;
    _ledMode = IO_LED_MODE_OFF;
    _onMillis = 0;
    _offMillis = 0;
}
void ScoreboardIOLEDClass::Start ( uint8_t ledpin )
{
    _ledPin = ledpin;
    pinMode ( _ledPin, OUTPUT );
    digitalWrite ( _ledPin, IO_LED_STATE_OFF );

#if defined (ESP8266)
    // Set up global values (only one led per ticker)
    _ledTickerPin = _ledPin;
#endif
}
// ---------------------------------------------------------------------------------
void ScoreboardIOLEDClass::Process()
{
//TODO add code to process for Arduino (without using a timer)
}
// ---------------------------------------------------------------------------------
void ScoreboardIOLEDClass::Flash ( uint16_t interval )
{
    Blink ( interval, interval );
    _ledMode = IO_LED_MODE_FLASH;
}
// ---------------------------------------------------------------------------------
void ScoreboardIOLEDClass::Blink ( uint16_t on_millis, uint16_t off_millis )
{
    _ledMode = IO_LED_MODE_BLINK;
    _onMillis = on_millis;
    _offMillis = off_millis;
#if defined (ESP8266)
    _ledTicker.detach();
    _ledTickerOnMillis = _onMillis;
    _ledTickerOffMillis = _offMillis;
    digitalWrite ( _ledPin, IO_LED_STATE_OFF );
    ESPLEDOn();
#else
    this->Process();
#endif
}
// ---------------------------------------------------------------------------------
void ScoreboardIOLEDClass::Off()
{
    _ledMode = IO_LED_MODE_OFF;
#if defined (ESP8266)
    _ledTicker.detach();
#endif
    digitalWrite ( _ledPin, IO_LED_STATE_OFF );
}
// ---------------------------------------------------------------------------------
void ScoreboardIOLEDClass::On()
{
    _ledMode = IO_LED_MODE_ON;
#if defined (ESP8266)
    _ledTicker.detach();
#endif
    digitalWrite ( _ledPin, IO_LED_STATE_ON );
}
// ---------------------------------------------------------------------------------
#ifndef ESP8266
uint8_t ScoreboardIOBatteryClass::Status ( )
{
    uint16_t sum = 0;               // sum of samples taken
    float voltage = 0.0;            // calculated voltage
    float soc = 0.0;                // percent of battery charge remaining

    // take a number of analog samples and add them up
    for ( uint8_t c = 0; c < BATTERY_SAMPLES; c++ )
    {
        sum += analogRead ( BATTERY_SENSE_PIN );
        delay ( BATTERY_SAMPLE_DELAY );
    }

    // calculate the voltage and state of charge (rounded to the nearest 10%)
    voltage = ( ( float ) sum / ( float ) BATTERY_SAMPLES * 5.015 ) / 1024.0 * BATTERY_DIVIDER_RATIO;
    soc = round ( ( voltage - BATTERY_EMPTY ) / BATTERY_RANGE * 10 ) * 10;
    soc = constrain ( soc, 0, 100 );

    return ( uint8_t ) soc;
}
#endif
// ---------------------------------------------------------------------------------
void ScoreboardIOButtonClass::Start()
{

}
// ---------------------------------------------------------------------------------
void ScoreboardIOButtonClass::Process()
{

}