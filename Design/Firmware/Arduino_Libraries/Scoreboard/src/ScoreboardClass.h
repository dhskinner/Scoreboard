/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef __SCOREBOARD_CLASS_H__
#define __SCOREBOARD_CLASS_H__

#include <Arduino.h>
#include <TimeLib.h>
#include <ScoreboardDefines.h>
#include <WSUtility.h>

// ---------------------------------------------------------------------------------
/*
All RF datagrams have a header - some also use the matchtimer mode flags
There is a fixed header of 3 bytes, plus a trailing checksum. Unused bytes are ignored
Datagrams are:
broadcast  messageID, mode, score1, score2, system time, match time, match state, flags
getState   messageID, mode
setScore   messageID, mode, score1, score2
setTimer   messageID, mode
*/

// Struct for datagram - for safety, initialise
// all data fields with invalid values
struct ScoreboardDatagram
{
    uint8_t messageID = MESSAGE_NULL;
    uint8_t fieldID = 255;
    uint8_t score1 = 255;
    uint8_t score2 = 255;
    uint32_t systemTime = 0;
    uint32_t matchTime = 0;
    uint8_t flags = 0;
};

// ---------------------------------------------------------------------------------
class ScoreboardClass
{
public:
    ScoreboardClass();										// Constructor
    uint8_t	ID();											// Get/Set the field ID of this scoreboard object
    uint8_t	ID ( uint8_t );									// Get/Set the field ID of this scoreboard object
    void	Run();											// Start and/or un-pause the timer
    void	Pause();										// Pause the timer
    void	Stop();											// Stop the timer and reset to zero
    time_t	MatchTime();									// Get/Set the match time in seconds
    time_t	MatchTime ( time_t tm );						// Get/Set the match time in seconds
	uint32_t MatchTimeMillis();								// Get the match time in milliseconds
    uint8_t	MatchTimeMode();								// Get/Set the current mode (one byte)
    uint8_t MatchTimeMode ( uint8_t mode );					// Get/Set the current mode (one byte)
    void	SystemTime ( time_t tm );						// Set the system time
    uint8_t	Score1();										// Get/Set Score1
    uint8_t	Score1 ( uint8_t score );						// Get/Set Score1
    uint8_t	Score2();										// Get/Set Score2
    uint8_t	Score2 ( uint8_t score );						// Get/Set Score2
    uint8_t	ScoreIncrement ( uint8_t score, int increment );	// Add/Subtract from a score
    uint8_t	ScoreID ( uint8_t id );							// Get score by id number (must be zero or 1)
    uint8_t	ScoreID ( uint8_t id, uint8_t value );			// Set score by id number (must be zero or 1)
    void	Score ( uint8_t score1, uint8_t score2 );		// Set both scores
    boolean	Dual();											// Get/Set the flag for dual fields
	boolean	Dual( boolean state );							// Get/Set the flag for dual fields
	boolean	Swap();											// Get/Set the flag for swap 
	boolean	Swap(boolean state );							// Get/Set the flag for swap 
	boolean	Clock();										// Get/Set the flag for clock on or off
	boolean	Clock(boolean state );							// Get/Set the flag for clock on or off
    uint8_t	Brightness();									// Get/Set the brightness level (0 - 6)
    uint8_t	Brightness ( uint8_t brightness );				// Get/Set the brightness level (0 - 6)
	String TeamName[2];										// Team names (string is publicly exposed)
    void	GetDatagram ( ScoreboardDatagram *datagram );	// Get/Set a datagram - returns false if the field ID is mismatched
    boolean	SetDatagram ( ScoreboardDatagram *datagram );

protected:

private:
    uint8_t Flags();												// Get/Set all current flags
    uint8_t Flags ( uint8_t flags );								// Get/Set all current flags
    boolean IsFlagSet ( uint8_t flag );								// Check if a flag is set
    static boolean IsFlagSet ( uint8_t flag, uint8_t flagByte );	// Check if a flag is set
	uint8_t	SetFlag(uint8_t flag);									// Set one flag
	uint8_t	ClearFlag(uint8_t flag);								// Clear one flag

    uint8_t _fieldID;												// scoreboard field ID
	uint32_t _timerReference;										// reference in millis for the match timer
    uint8_t _score[2];												// match scores
    uint8_t _flags;													// scoreboard flags
};

#endif
