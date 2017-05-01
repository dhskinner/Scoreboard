/*
Author:			Dave Skinner
Description:	Repeater for scoreboard using one DMD panel
*/

// ---------------------------------------------------------------------------------

// Set the scoreboard mode - refer to ScoreboardConfigs.h for options
#define SCOREBOARD_REPEATER

// ---------------------------------------------------------------------------------

// External Libraries
#include <Arduino.h>
#include <Scoreboard.h>
#define SERIAL_DEBUG
// ---------------------------------------------------------------------------------

// Globals
ScoreboardDMD2Class dmd(DMD_DISPLAYS_ACROSS, DMD_DISPLAYS_DOWN, DMD_REPEATER);
ScoreboardRadioClass radio(RF24_ADDRESS, RF24_CE, RF24_CS);	// RF Radio object
time_t updatetime = 0;
time_t swapmodetime = 0;
DMDRepeaterMode mode = DMD_REPEATER_SCORE;

// ---------------------------------------------------------------------------------
void setup ( void )
{
	UBEGIN(115200);
	UPRINTLN(F("Start scoreboard test panel"));

	// initialize the scoreboard objects with their field ID
	for (uint8_t id = 0; id < FIELDS; id++)
		scoreboard[id].ID(id);

	// deselect SPI devices by setting their chipselect lines HIGH
	// (otherwise the SPI devices will interfere with each other)
	WSUtility::DeselectSPI(spi, sizeof(spi));

	// Start the RTC
	i2cclock.SetSystemTimefromRTC();

	// Initialise the radio
	radio.Start();
	radio.On();

	// Dim the display
	scoreboard[DEFAULT_FIELD].Brightness(0);

	// Start the dmd
	dmd.Start();
	UPRINTLN("Startup complete");
}
// ---------------------------------------------------------------------------------
void loop ( void )
{
    // Process once per loop
	radio.Process();

    // Process once per second
    if ( now() > updatetime )
    {
        // If there is no signal, switch to the real time clock
        if ( radio.SignalQuality() == 0 )
		{
			mode = DMD_REPEATER_CLOCK;
        }
		else if (scoreboard->MatchTimeMode() != FLAG_TIMER_STOP)
		{
			// alternate the score and timer
			if (now() >= swapmodetime)
			{
				if (mode == DMD_REPEATER_SCORE )
				{
					mode = DMD_REPEATER_TIMER;
					swapmodetime = now() + DMD_REPEATER_TIMER_ON;
				}
				else {
					mode = DMD_REPEATER_SCORE;
					swapmodetime = now() + DMD_REPEATER_SCORE_ON;
				}
			}
		}
		else
		{
			// just show the scores 
			mode = DMD_REPEATER_SCORE;
		}
		dmd.UpdateRepeater(mode);
        updatetime = now();
    }
	delay(100);
}
// ---------------------------------------------------------------------------------


