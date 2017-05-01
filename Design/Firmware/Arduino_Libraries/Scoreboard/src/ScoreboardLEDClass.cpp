/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardLEDClass.h"

#ifndef HW_ESP8266

// ---------------------------------------------------------------------------------
ScoreboardLEDClass::ScoreboardLEDClass ( uint8_t fieldID, int datapin, int clockpin, int chipselectpin ) : _lc ( datapin, clockpin, chipselectpin, 1 )
{
    _fieldID = fieldID;
}
// ---------------------------------------------------------------------------------
void ScoreboardLEDClass::Start()
{
    // The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
    _lc.shutdown ( 0, false );

    // Set the brightness to a medium values and clear the display
    _lc.setIntensity ( 0, 8 );
    _lc.clearDisplay ( 0 );

    for ( int i = 0; i < LED_DIGITS; i++ )
        _lc.setDigit ( 0, i, 8, true );
    delay ( 500 );
    _lc.clearDisplay ( 0 );

	UPRINTLN(F("Started seven segment LED"));
}
// ---------------------------------------------------------------------------------
void ScoreboardLEDClass::Update ( )
{
    if ( _ledClock != scoreboard[_fieldID].Clock() )
    {
        _ledClock = scoreboard[_fieldID].Clock();
        _lc.clearDisplay ( 0 );
    }

    // update the LED
    if ( _ledClock )
        this->DisplayTimeHHMMSS ( now(), 0 );
    else
        this->DisplayTimeMMSS ( scoreboard[_fieldID].MatchTime(), 2 );
}
// ---------------------------------------------------------------------------------
// Display up to 99 minutes and 59 seconds
void ScoreboardLEDClass::DisplayTimeMMSS ( time_t t, uint8_t index )
{
    // Set the seconds - between 0 and 59
    uint16_t temp = second ( t );
    _lc.setDigit ( 0, index, uint8_t ( temp % 10 ), false ); // <- the last boolean is to show decimal
    _lc.setDigit ( 0, index + 1, uint8_t ( temp / 10 ), false );

    // Set the minutes - between 0 and 99
    temp = minute ( t ) + hour ( t ) * 60;
    temp = constrain ( temp, 0, 99 );
    _lc.setDigit ( 0, index + 2, uint8_t ( temp % 10 ), true );
    if ( temp > 9 )
        _lc.setDigit ( 0, index + 3, uint8_t ( temp / 10 ), false );
    else
        _lc.setChar ( 0, index + 3, '0', false );
}
// ---------------------------------------------------------------------------------
// Display hours, minutes and seconds
void ScoreboardLEDClass::DisplayTimeHHMMSS ( time_t t, uint8_t index )
{
    // Set the seconds - between 0 and 59
    uint16_t temp = second ( t );
    _lc.setDigit ( 0, index, uint8_t ( temp % 10 ), false ); // <- the last boolean is to show decimal
    _lc.setDigit ( 0, index + 1, uint8_t ( temp / 10 ), false );

    // Set the minutes - between 0 and 59
    temp = minute ( t );
    _lc.setChar ( 0, index + 2, '-', false );
    _lc.setDigit ( 0, index + 3, uint8_t ( temp % 10 ), false );
    if ( temp > 9 )
        _lc.setDigit ( 0, index + 4, uint8_t ( temp / 10 ), false );
    else
        _lc.setChar ( 0, index + 4, '0', false );

    // Set the hours - between 0 and 23
    temp = hour ( t );
    _lc.setChar ( 0, index + 5, '-', false );
    _lc.setDigit ( 0, index + 6, uint8_t ( temp % 10 ), false );
    if ( temp > 9 )
        _lc.setDigit ( 0, index + 7, uint8_t ( temp / 10 ), false );
    else
        _lc.setChar ( 0, index + 7, '0', false );
}
// ---------------------------------------------------------------------------------
// Display the score up to 99
void ScoreboardLEDClass::DisplayScore ( uint8_t score, uint8_t index )
{
    // Set the score between 0 and 99
    score = constrain ( score, MIN_SCORE, MAX_SCORE );

    _lc.setDigit ( 0, index, uint8_t ( score % 10 ), false );  // first digit
    if ( score > 9 )
        _lc.setDigit ( 0, index + 1, uint8_t ( score / 10 ), false ); // second digit
    else
        _lc.setChar ( 0, index + 1, ' ', false );
}
// ---------------------------------------------------------------------------------
void ScoreboardLEDClass::Test()
{
    // for ( int i = 0; i < LED_DIGITS; i++ )
    //     _lc.setDigit ( 0, i, i, false );

    for ( int i = 0; i < LED_DIGITS + LED_MAX_DIGIT; i++ )
    {
        this->ScrollDigits();
        delay ( 100 );
    }
    _lc.clearDisplay ( 0 );
}
// ---------------------------------------------------------------------------------
// This method will scroll all the hexa-decimal  numbers and letters on the display.
// You will need at least four 7-Segment digits. otherwise it won't really look that good.
void ScoreboardLEDClass::ScrollDigits()
{
    static int scrollIndex = 0;
    for ( int i = 0; i < LED_DIGITS; i++ )
    {
        if ( ( scrollIndex - i >= LED_MIN_DIGIT ) && ( scrollIndex - i < LED_MAX_DIGIT ) )
            _lc.setDigit ( 0, i, scrollIndex - i, false );
        else
            _lc.setChar ( 0, i, ' ', false );
    }
    scrollIndex = scrollIndex >= LED_MAX_DIGIT + LED_DIGITS ? 0 : scrollIndex + 1;
}

#endif


