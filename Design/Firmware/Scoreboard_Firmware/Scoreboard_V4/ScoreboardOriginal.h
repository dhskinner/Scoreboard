/*
  Matchtimer.h
*/
#ifndef __SCOREBOARD_H__
#define __SCOREBOARD_H__

#include <Arduino.h>
#include <Time.h>
#include "Defines.h"

// ---------------------------------------------------------------------------------
/*
  All RF datagrams have a header - some also use the matchtimer mode flags
  There is a fixed header of 3 bytes -
  trailing checksum
  unused bytes are ignored
  Datagrams are:
    broadcast  messageID, mode, score1, score2, system time, match time, match state, flags
    getState   messageID, mode
    setScore   messageID, mode, score1, score2
    setTimer   messageID, mode
*/

struct SCOREBOARD_DATAGRAM
{
  uint8_t messageID;
  uint8_t fieldID;
  uint8_t score0;
  uint8_t score1;
  time_t systemTime;
  time_t matchTime;
  uint8_t flags;
};

// ---------------------------------------------------------------------------------
class Scoreboard
{
public:
  Scoreboard();								// Constructor
  uint8_t	ID();							// Get/Set the field ID of this scoreboard object
  uint8_t	ID ( uint8_t );					// Get/Set the field ID of this scoreboard object
  void		Run();							// Start and/or un-pause the timer
  void		Pause();						// Pause the timer 
  void		Stop();							// Stop the timer and reset to zero
  time_t	MatchTime();									// Get/Set the match time in seconds
  time_t	MatchTime( time_t tm , uint8_t mode );			// Get/Set the match time in seconds
  uint8_t	MatchTimeMode();				// Get/Set the current mode (one byte)
  //uint8_t   MatchTimeMode ( uint8_t mode );	// Get/Set the current mode (one byte)
  void		SystemTime ( time_t tm );		// Set the system time
  uint8_t	Score0();						// Get/Set Score1
  uint8_t	Score0 ( uint8_t score );		// Get/Set Score1
  uint8_t	Score1();						// Get/Set Score2
  uint8_t	Score1 ( uint8_t score );		// Get/Set Score2
  uint8_t	ScoreID ( uint8_t id );			// Get score by id number (must be zero or 1)
  uint8_t	ScoreID ( uint8_t id, uint8_t value );			// Set score by id number (must be zero or 1)
  void		Score ( uint8_t score0, uint8_t score1 );		// Set both scores
  boolean	IsSwapFieldsOn();				// Shortcut to check if the flag is set
  boolean	IsDualFieldsOn();				// Shortcut to check if the flag is set
  boolean	IsClockOn();					// Shortcut to check if the flag is set
  uint8_t	Brightness();					// Get/Set the brightness level (0 - 6)
  uint8_t	Brightness ( uint8_t brightness );				// Get/Set the brightness level (0 - 6)
  void		GetDatagram ( SCOREBOARD_DATAGRAM *datagram );	// Get/Set a datagram - returns false if the field ID is mismatched
  boolean	SetDatagram ( SCOREBOARD_DATAGRAM *datagram );	
  uint8_t	SetFlag ( uint8_t flag );		// Set one flag
  uint8_t	ClearFlag ( uint8_t flag );		// Clear one flag
  boolean	IsFlagSet(uint8_t flag);		// Check if a flag is set
protected:

private:
  uint8_t Flags();							// Get/Set all current flags
  uint8_t Flags ( uint8_t flags );			// Get/Set all current flags
  static boolean IsFlagSet ( uint8_t flag, uint8_t flagByte ); // Check if a flag is set

  uint8_t _fieldID;							// scoreboard field ID
  time_t  _startTime;						// time that the match timer was started
  time_t  _pauseTime;						// time that the match timer was paused
  uint8_t _score[2];						// match scores
  uint8_t _flags;							// scoreboard flags
};

#endif
