/*
Author:			Dave Skinner
Description:	4 x 2 LED Panel for scoreboard
*/
// ---------------------------------------------------------------------------------
#define SCOREBOARD_PANEL DMD_RIGHT_PANEL		// Define as DMD_LEFT_PANEL or DMD_RIGHT_PANEL
#define SCOREBOARD_PANEL_AUTO_SCAN				// Define as SCOREBOARD_PANEL_MANUAL_SCAN or SCOREBOARD_PANEL_AUTO_SCAN scan
// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// Globals
ScoreboardDMD2Class dmd(DMD_DISPLAYS_ACROSS, DMD_DISPLAYS_DOWN, SCOREBOARD_PANEL);

// ---------------------------------------------------------------------------------
#ifdef SCOREBOARD_PANEL_AUTO_SCAN

void setup(void)
{
	UBEGIN(115200);
	delay(100);
	USTREAM(F("Started panel:\tmode=") << SCOREBOARD_PANEL << endl);

	// initialize the scoreboard objects with their field ID
	for (uint8_t id = 0; id < FIELDS; id++)
		scoreboard[id].ID(id);

	// Initialise I2C comms
	i2ctransfer.StartAsSlave(I2C_ADDRESS);

	// Initialise the dmd
	dmd.Start(true);

	UHEARTBEAT_BEGIN(13);
}

void loop(void)
{
	UHEARTBEAT_UPDATE(13, 1000);

	if (i2ctransfer.Process())
		dmd.UpdatePanel();
}
// ---------------------------------------------------------------------------------
#elif defined SCOREBOARD_PANEL_MANUAL_SCAN

#ERROR THIS CODE IS NOT TESTED

#include <TimerOne.h>	

void setup(void)
{
	UBEGIN(115200);
	delay(100);
	USTREAM(F("Started panel:\tmode=") << SCOREBOARD_PANEL << endl);

	// initialize the scoreboard objects with their field ID
	for (uint8_t id = 0; id < FIELDS; id++)
		scoreboard[id].ID(id);

	// Initialise I2C comms
	i2ctransfer.StartAsSlave(I2C_ADDRESS);

	// Initialise the dmd
	dmd.Start(true);

	UHEARTBEAT_BEGIN(13);

	// Start a manual timer
	Timer1.initialize(16UL);
	Timer1.attachInterrupt(scan);
}

void loop(void)
{
	UHEARTBEAT_UPDATE(13, 1000);

	// Process once per loop
	if (i2ctransfer.Process())
		dmd.UpdatePanel();
}

void scan()
{
	dmd.Scan();	// Manually scan the DMD
}
#endif