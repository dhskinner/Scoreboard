/*
 Author:		Dave Skinner
 Description:	Hotspot controller for scoreboard using Adafruit HUZZAH ESP8266
*/
// ---------------------------------------------------------------------------------
// Set the scoreboard mode - refer to ScoreboardConfigs.h for options

#define SCOREBOARD_HOTSPOT_ESP
#define SCOREBOARD_ACCESS_POINT		true	// if true, then start as access point with captive portal

// Just a reminder
#ifndef ESP8266
#error Remember to set the board to Adafruit HUZZAH ESP8266
#endif

// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------
// Globals
ScoreboardRadioClass radio ( RF24_ADDRESS, RF24_CE, RF24_CS );
ScoreboardWebClass web ( 80 );
uint8_t score = 0;
unsigned long updateMillis = 0;

// ---------------------------------------------------------------------------------
void setup ( void )
{
    // Start serial
    Serial.begin ( 115200 );
    delay ( 100 );
    UPRINTLN ( F ( "\n\rStarting scoreboard hotspot" ) );

    // Start the scoreboard objects with their field ID
    for ( uint8_t id = 0; id < FIELDS; id++ )
        scoreboard[id].ID ( id );

    // Deselect SPI devices by setting their chipselect lines HIGH
    // (otherwise the SPI devices will interfere with each other)
    WSUtility::DeselectSPI ( spi, sizeof ( spi ) );

    // Start the LiPo battery fuel gauge
    i2cbattery.Start();

    // Start the I2C IO handlers
    i2cio.ShortPressButton ( 0, SetScore1, 1 );
    i2cio.ShortPressButton ( 1, SetScore1, -1 );
    i2cio.ShortPressButton ( 2, SetScore2, 1 );
    i2cio.ShortPressButton ( 3, SetScore2, -1 );
    i2cio.LongPressButton ( 0, SetScore1, 5 );
    i2cio.LongPressButton ( 1, SetScore1, -5 );
    i2cio.LongPressButton ( 2, SetScore2, 5 );
    i2cio.LongPressButton ( 3, SetScore2, -5 );
    i2cio.Start();

    // Start the radio (defaults to offline mode)
    radio.Start();
    radio.On();

    // Start the web server
    web.Start ( LED_PIN, SCOREBOARD_ACCESS_POINT );

    UPRINTLN ( F ( "Startup complete" ) );
}
// ---------------------------------------------------------------------------------
// Set the Score1 for Field 1
void SetScore1 ( int increment )
{
    USTREAM ( F ( "Set Score1 increment=" ) << increment << endl );
    scoreboard[DEFAULT_FIELD].ScoreIncrement ( SCORE1, increment );
    radio.Send ( SET_SCORE, RF24_SCOREBOARD_ADDRESS, DEFAULT_FIELD );
}
// ---------------------------------------------------------------------------------
// Set the Score2 for Field 1
void SetScore2 ( int increment )
{
    USTREAM ( F ( "Set Score2 increment=" ) << increment << endl );
    scoreboard[DEFAULT_FIELD].ScoreIncrement ( SCORE2, increment );
    radio.Send ( SET_SCORE, RF24_SCOREBOARD_ADDRESS, DEFAULT_FIELD );
}
// ---------------------------------------------------------------------------------
void loop ( )
{
    //ArduinoOTA.handle();
    i2cio.Process();
    radio.Process();
    web.Process();

    if ( millis() > updateMillis )
    {
        if ( !web.Connected() )
            web.ConnectWifi();

        updateMillis += 1000;
    }
    yield();
}
