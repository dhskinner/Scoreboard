/*
Author:			Dave Skinner
Description:	Test DMD Panel for scoreboard
*/

// ---------------------------------------------------------------------------------

// Set the scoreboard mode - refer to ScoreboardConfigs.h for options
#define SCOREBOARD_TEST_PANEL

// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------

// Globals
ScoreboardDMD2Class dmd ( DMD_DISPLAYS_ACROSS, DMD_DISPLAYS_DOWN, DMD_TEST_PANEL );
ScoreboardLEDClass led(FIELD1, LED_DATA, LED_CLK, LED_CS);

// ---------------------------------------------------------------------------------
void setup ( void )
{
	UBEGIN(115200);
	UPRINTLN(F("Start scoreboard test panel"));

    // initialize the scoreboard objects with their field ID
    for ( uint8_t id = 0; id < FIELDS; id++ )
        scoreboard[id].ID ( id );

	// deselect SPI devices by setting their chipselect lines HIGH
	// (otherwise the SPI devices will interfere with each other)
	WSUtility::DeselectSPI(spi, sizeof(spi));

	// Start I2C comms
	i2ctransfer.StartAsSlave(I2C_ADDRESS);

	// Start the LED
	led.Start();

	// Start the dmd
	dmd.Start();
	dmd.UpdateTestPanel();
	UPRINTLN("Startup complete");
}
// ---------------------------------------------------------------------------------
void loop ( void )
{
	// Process once per loop
	if ( i2ctransfer.Process() )
		dmd.UpdateTestPanel();

	// Process once per second
	static time_t updateTime;
	if (now() != updateTime)
	{
		led.DisplayTimeHHMMSS(now(), 0);
		dmd.UpdateTestPanel();

		updateTime = now();
	}
}
// ---------------------------------------------------------------------------------
