/*
Author:			Dave Skinner
Description:	Remote controller for scoreboard
*/
// ---------------------------------------------------------------------------------

// Set the scoreboard mode - refer to ScoreboardConfigs.h for options
#define SCOREBOARD_CONTROLLER

// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------

// Globals
ScoreboardRadioClass radio ( RF24_CHANNEL, RF24_CE, RF24_CS );	// RF Radio master object

// ---------------------------------------------------------------------------------
void setup ( void )
{
    // initialize the scoreboard objects with their field ID
    for ( uint8_t id = 0; id < FIELDS; id++ )
        scoreboard[id].ID ( id );

	// deselect SPI devices by setting their chipselect lines HIGH
	// (otherwise the SPI devices will interfere with each other)
	WSUtility::DeselectSPI(spi, sizeof(spi));

    // Initialise the radio and ulcd
    radio.Start();
    ulcd.Start ( ULCD_SERIAL_PORT, ULCD_SERIAL_BAUD, ULCD_RESET_LINE, ULCD_RESET_INVERT );
    ulcd.Update ( true );
}
// ---------------------------------------------------------------------------------
void loop ( void )
{
    // Process once per loop
    if ( radio.Process() )
        ulcd.Update ( false );

    ulcd.Process();

    // Process once per second
    static time_t updateTime;
    if ( now() != updateTime )
    {
        ulcd.Update ( false );
        updateTime = now();
    }
}
// ---------------------------------------------------------------------------------
