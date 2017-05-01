/*
Author:			Dave Skinner
Description:	4 x 2 LED Panel for scoreboard
*/

// ---------------------------------------------------------------------------------

// Set the scoreboard mode - refer to ScoreboardConfigs.h for options
#define SCOREBOARD_PANEL DMD_RIGHT_PANEL		//   Define as DMD_LEFT_PANEL or DMD_RIGHT_PANEL

// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------

// Globals
ScoreboardDMD2Class dmd ( DMD_DISPLAYS_ACROSS, DMD_DISPLAYS_DOWN, SCOREBOARD_PANEL );

// ---------------------------------------------------------------------------------
void setup ( void )
{
    // initialize the scoreboard objects with their field ID
    for ( uint8_t id = 0; id < FIELDS; id++ )
        scoreboard[id].ID ( id );

    // Initialise I2C comms
    i2ctransfer.StartAsSlave ( I2C_ADDRESS	);

	// Initialise the dmd
	dmd.Start();
}
// ---------------------------------------------------------------------------------
void loop ( void )
{
    // Process once per loop
	if (i2ctransfer.Process())
		dmd.UpdatePanel();
}
// ---------------------------------------------------------------------------------
