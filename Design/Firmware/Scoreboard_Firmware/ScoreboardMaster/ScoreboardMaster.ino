/*
Author:			Dave Skinner
Description:	Master controller for scoreboard
*/

// ---------------------------------------------------------------------------------

// Set the scoreboard mode - refer to Scoreboard.h for options
#define SCOREBOARD_MASTER

// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------

ScoreboardRadioClass radio ( RF24_ADDRESS, RF24_CE, RF24_CS );	// RF Radio object
time_t updateTime = 0;											// Last update time in seconds

// ---------------------------------------------------------------------------------
void setup ( void )
{
    UBEGIN ( 115200 );
    UPRINTLN ( F ( "Start scoreboard master" ) );

    // initialize the scoreboard objects with their field ID
    for ( uint8_t id = 0; id < FIELDS; id++ )
        scoreboard[id].ID ( id );

    // deselect SPI devices by setting their chipselect lines HIGH
    // (otherwise the SPI devices will interfere with each other)
    WSUtility::DeselectSPI ( spi, sizeof ( spi ) );

    // get the system time from the RTC
    i2cclock.SetSystemTimefromRTC();

    // Initialise the radio
    radio.Start();
    radio.On();

    // Initialise I2C comms
    i2ctransfer.StartAsMaster ( );

    // short delay so that the slaves can fire up
    delay ( MASTER_STARTUP_DELAY );

    // set the score
    uint8_t score = 199;
    scoreboard[FIELD1].Score ( score, score );
    scoreboard[FIELD2].Score ( score, score );
    i2ctransfer.Broadcast ( );
    radio.Broadcast ( FIELD1 );
    radio.Broadcast ( FIELD2 );

    // short delay again
    delay ( MASTER_STARTUP_DELAY );

    // Set the scores back to zero
    score = 0;
    scoreboard[FIELD1].Score ( score, score );
    scoreboard[FIELD2].Score ( score, score );
    i2ctransfer.Broadcast ();
    radio.Broadcast ( FIELD1 );
    radio.Broadcast ( FIELD2 );

    UPRINTLN ( F ( "Startup complete" ) );

	// Test
	//scoreboard[FIELD1].Run();
}
// ---------------------------------------------------------------------------------
void loop ( void )
{
    // Process once per loop
    if ( radio.Process() )
        i2ctransfer.Broadcast ();

    // Process once per second
    static time_t updateTime;
    if ( now() != updateTime )
    {
        // toggle the broadcast between scoreboard 1 and 2 each second
        i2ctransfer.Broadcast ( now() % 2 ? FIELD1 : FIELD2 );
        radio.Broadcast ( now() % 2 ? FIELD1 : FIELD2 );
        updateTime = now();
    }
}
// ---------------------------------------------------------------------------------
