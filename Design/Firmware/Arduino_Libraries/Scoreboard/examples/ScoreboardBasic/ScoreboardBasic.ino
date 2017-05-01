/*
 Author:		Dave Skinner
 Description:	Hotspot controller for scoreboard
*/

// ---------------------------------------------------------------------------------

// Set the scoreboard mode - refer to ScoreboardConfigs.h for options
#define SCOREBOARD_MASTER

// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------

void setup ( void )
{
	UBEGIN(115200);
	delay(100);
	UPRINTLN(F("\n\rStart scoreboard"));

	// initialize the scoreboard objects with their field ID
    for ( uint8_t id = 0; id < FIELDS; id++ )
        scoreboard[id].ID ( id );

	// deselect SPI devices by setting their chipselect lines HIGH
	// (otherwise the SPI devices will interfere with each other)
	utility.DeselectSPI(spi, sizeof(spi));

	UPRINTLN(F("Startup complete"));
}

// ---------------------------------------------------------------------------------

void loop ( void )
{
    // Process every n seconds
	static time_t updateTime = 0;
    if ( now() > updateTime + 10 )
    {
        updateTime = now();
    }
}

// ---------------------------------------------------------------------------------
