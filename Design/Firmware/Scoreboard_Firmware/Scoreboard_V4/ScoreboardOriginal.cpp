/*
    Matchtimer.cpp
*/
#include "ScoreboardOriginal.h"

// ---------------------------------------------------------------------------------
// Constructor
Scoreboard::Scoreboard()
{
  this->_fieldID = 0;
  this->_startTime = 0;
  this->_pauseTime = 0;
  this->_score[0] = 0;
  this->_score[1] = 0;
  this->_flags = 0;
}
// ---------------------------------------------------------------------------------
// Get/Set the field ID of this scoreboard object (overloaded)
uint8_t Scoreboard::ID()
{
  return this->_fieldID;
}
uint8_t Scoreboard::ID ( uint8_t id )
{
  this->_fieldID = id;
  return this->_fieldID;
}
// ---------------------------------------------------------------------------------
// Start and/or un-pause the timer
void Scoreboard::Run()
{
  switch ( this->MatchTimeMode() )
  {
  case FLAG_TIMER_PAUSE:        // if paused - restart the timer (and shift startime right by the amount of the pause)
    this->_startTime = now() - ( this->_pauseTime - this->_startTime );
    this->_pauseTime = 0;
    this->ClearFlag ( FLAG_TIMER_PAUSE );
    this->SetFlag ( FLAG_TIMER_RUN );
    break;

  case FLAG_TIMER_STOP:         // if stopped, start the timer
    this->_startTime = now();
    this->SetFlag ( FLAG_TIMER_RUN );
    break;

  case FLAG_TIMER_RUN:         // if running - do nothing
  default:
    break;
  }
}
// ---------------------------------------------------------------------------------
// Pause the timer
void Scoreboard::Pause()
{
  switch ( this->MatchTimeMode() )
  {
  case FLAG_TIMER_RUN:   // if running - start the pause
    _pauseTime = now();
    this->ClearFlag ( FLAG_TIMER_RUN );
    this->SetFlag ( FLAG_TIMER_PAUSE );
    break;

  case FLAG_TIMER_PAUSE:  // if paused - do nothing
  case FLAG_TIMER_STOP:   // if stopped - do nothing
  default:
    break;
  }
}
// ---------------------------------------------------------------------------------
// Stop the timer and reset to zero
void Scoreboard::Stop()
{
  this->ClearFlag ( FLAG_TIMER_RUN );
  this->ClearFlag ( FLAG_TIMER_PAUSE );
  this->_startTime = 0;
  this->_pauseTime = 0;
}
// ---------------------------------------------------------------------------------
// Get/Set the current matchtime (in seconds)
time_t Scoreboard::MatchTime()
{
  switch ( this->MatchTimeMode() )
  {
  case FLAG_TIMER_RUN:
    return constrain ( now() - this->_startTime, 0, MAX_MATCHTIME );
    break;

  case FLAG_TIMER_PAUSE:
    return constrain ( this->_pauseTime - this->_startTime,  0, MAX_MATCHTIME );
    break;

  case FLAG_TIMER_STOP:
  default:
    return 0;
    break;
  }
}
time_t Scoreboard::MatchTime ( time_t tm , uint8_t mode )
{
  // check the mode (just in case there are extraneous bits)
  if ( this->IsFlagSet ( FLAG_TIMER_RUN , mode ) )
  {
    // the timer is running
    this->_startTime = now() -  tm;
    this->_pauseTime = 0;
    this->ClearFlag ( FLAG_TIMER_PAUSE );
    this->SetFlag ( FLAG_TIMER_RUN );
  }
  else if ( this->IsFlagSet ( FLAG_TIMER_PAUSE, mode ) )
  {
    // the timer is paused
    this->_startTime = now() -  tm;
    this->_pauseTime = now();
    this->SetFlag ( FLAG_TIMER_PAUSE );
    this->ClearFlag ( FLAG_TIMER_RUN );
  }
  else
  {
    // the timer is stopped
    this->_startTime = 0;
    this->_pauseTime = 0;
    this->ClearFlag ( FLAG_TIMER_PAUSE );
    this->ClearFlag ( FLAG_TIMER_RUN );
  }

  return this->MatchTime();
}
// ---------------------------------------------------------------------------------
// Get/Set the current mode (overloaded)
uint8_t Scoreboard::MatchTimeMode()
{
  if ( this->IsFlagSet ( FLAG_TIMER_RUN ) )
    return FLAG_TIMER_RUN;
  else if ( this->IsFlagSet ( FLAG_TIMER_PAUSE ) )
    return FLAG_TIMER_PAUSE;
  else
    return FLAG_TIMER_STOP;
}
/*
uint8_t Scoreboard::MatchTimeMode ( uint8_t flag )
{
  switch ( flag )
  {
  case FLAG_TIMER_RUN:
    this->Run();
    break;

  case FLAG_TIMER_PAUSE:
    this->Pause();
    break;

  case FLAG_TIMER_STOP:
  default:
    this->Stop();
    break;
  }
  return this->MatchTimeMode();
}
*/
// ---------------------------------------------------------------------------------
// Set the current system time
void Scoreboard::SystemTime ( time_t tm )
{
  if ( now() != tm )
    setTime ( tm );
}
// ---------------------------------------------------------------------------------
// Get/Set score 0 (overloaded)
uint8_t Scoreboard::Score0()
{
  return this->_score[0];
}
uint8_t Scoreboard::Score0 ( uint8_t score )
{
	if (this->_score[0] != score)
	{
		this->_score[0] = constrain(score, MIN_SCORE, MAX_SCORE);
#ifdef DEBUG
		Serial << F("Set score 0 to ") << this->_score[0] << endl;
#endif
	}
  return this->_score[0];
}
// ---------------------------------------------------------------------------------
// Get/Set score 1 (overloaded)
uint8_t Scoreboard::Score1()
{
  return this->_score[1];
}
uint8_t Scoreboard::Score1 ( uint8_t score )
{
  if ( this->_score[1] != score )
  { 
    this->_score[1] = constrain ( score, MIN_SCORE, MAX_SCORE );
#ifdef DEBUG
	Serial << F("Set score 1 to ") << this->_score[1] << endl;
#endif
  }
  return this->_score[1];
}
// ---------------------------------------------------------------------------------
// Get/Set the score by id number
uint8_t Scoreboard::ScoreID ( uint8_t id )
{
  switch ( id )
  {
  case 0:
    return this->_score[0];
  case 1:
    return this->_score[1];
  default:
    return 0;
  }
}
// ---------------------------------------------------------------------------------
// Get/Set the score by id number
uint8_t Scoreboard::ScoreID ( uint8_t id, uint8_t value )
{
  switch ( id )
  {
  case 0:
    this->_score[0] = value;
    return this->_score[0];
  case 1:
    this->_score[1] = value;
    return this->_score[1];
  default:
    return 0;
  }
}
// ---------------------------------------------------------------------------------
// Get/Set scores
void Scoreboard::Score ( uint8_t score0, uint8_t score1 )
{
  this->Score0 ( score0 );
  this->Score1 ( score1 );
}
// ---------------------------------------------------------------------------------
// Get/Set all flags
uint8_t Scoreboard::Flags()
{
  return this->_flags;
}
uint8_t Scoreboard::Flags ( uint8_t flags )
{
  if ( this->_flags != flags )
    this->_flags = flags;

  return this->_flags;
}
// ---------------------------------------------------------------------------------
uint8_t Scoreboard::SetFlag ( uint8_t flag )
{
  // OR the bitmasks together
  this->_flags = this->_flags | flag;

  return _flags;
}
// ---------------------------------------------------------------------------------
uint8_t Scoreboard::ClearFlag ( uint8_t flag )
{
  // Invert the bit string with the bitwise NOT operator (~), then AND it.
  this->_flags &= ~ ( flag );
  
  return _flags;
}
// ---------------------------------------------------------------------------------
boolean Scoreboard::IsFlagSet ( uint8_t flag )
{
  return this->IsFlagSet ( flag, this->_flags );
}
boolean Scoreboard::IsFlagSet ( uint8_t flag, uint8_t flagByte )
{
  if ( ( flagByte & flag ) == flag )		// check the flag is set
    return true;
  else
    return false;
}
// ---------------------------------------------------------------------------------
// Shortcut to check if the flag is set
boolean Scoreboard::IsSwapFieldsOn()
{
  return this->IsFlagSet ( FLAG_SWAP_FIELD_ON );
}
// ---------------------------------------------------------------------------------
// Shortcut to check if the flag is set
boolean Scoreboard::IsDualFieldsOn()
{
  return this->IsFlagSet ( FLAG_DUAL_FIELD_ON );
}
// ---------------------------------------------------------------------------------
// Shortcut to check if the flag is set
boolean Scoreboard::IsClockOn()
{
  return this->IsFlagSet ( FLAG_CLOCK_ON );
}
// ---------------------------------------------------------------------------------
// Get/Set the brightness level (valid values are 0 to 6 - anything else assumes max brightness)
// Note that the bitmask used for setting the flags is referenced from 1
uint8_t Scoreboard::Brightness()
{
  // Take the 3 MSB of the flag
  uint8_t b = ( uint8_t ) _flags >> 5;

  if ( b == 0 )
  {
    this->SetFlag ( FLAG_BRIGHTNESS_6 );
    return 6;
  }
  else
    return b - 1;
}
// ---------------------------------------------------------------------------------
// Get/Set the brightness level (valid values are 0 to 6 - anything else assumes max brightness)
// Note that the bitmask used for setting the flags is referenced from 1 to seven (zero is invalid)
uint8_t Scoreboard::Brightness ( uint8_t brightness )
{
  // Range check zero to six - if > 6 assume its max brightness
  if ( brightness > 6 )
    brightness = 6;

  // Add one - the bitmask used for setting the flag
  // is referenced from 1 to seven (zero is invalid)
  brightness ++;

  // Clear the upper 3 bits of the bitmask
  _flags = _flags << 3;
  _flags = _flags >> 3;

  // Convert brightness to a bitmask value
  brightness = brightness << 5;

  // OR them together
  this->SetFlag ( brightness );

  return this->Brightness();
}
// ---------------------------------------------------------------------------------
// Get/Set datagram struct
void Scoreboard::GetDatagram ( SCOREBOARD_DATAGRAM *datagram )
{
  datagram->messageID = BROADCAST;
  datagram->fieldID = this->ID();
  datagram->score0 = this->Score0();
  datagram->score1 = this->Score1();
  datagram->systemTime = now();
  datagram->matchTime = this->MatchTime();
  datagram->flags = this->Flags();
}
boolean Scoreboard::SetDatagram ( SCOREBOARD_DATAGRAM *datagram )
{
  uint8_t b = 0;

  // Check the field ID matches
  if ( datagram->fieldID != this->ID() )
    return false;

  // store values depending on the messageID
  switch ( datagram->messageID )
  {
  case BROADCAST:												// Store everything
    this->Flags ( datagram->flags );							// Set the flags
    this->Score0 ( datagram->score0 );							// Set the scores
    this->Score1 ( datagram->score1 );							// Set the scores
    this->SystemTime ( datagram->systemTime );					// Set system time
    this->MatchTime ( datagram->matchTime, datagram->flags );	// Set match time
    break;

  case SET_SCORE:												// Store the scores (ignore the rest)
    this->Score0 ( datagram->score0 );							// Set the scores
    this->Score1 ( datagram->score1 );							// Set the scores
    break;

  case SET_TIMER:												// Store the timer mode and value (ignore the rest)
    if ( this->IsFlagSet ( FLAG_CLOCK_ON , datagram->flags ) )	// Set the clock flag
      this->SetFlag ( FLAG_CLOCK_ON );
    else
      this->ClearFlag ( FLAG_CLOCK_ON );

	// TODO - this currently ignores matchtime sent from the remote station (may cause problem if latency is an issue)
    // this->MatchTime ( datagram->matchTime, datagram->flags );	// Set the match timer
	this->MatchTime( this->MatchTime(), datagram->flags);	// Set the match timer
    break;

  case SET_MODE_FLAGS:											// Store the mode flags (ignore the rest)
    // Dual fields
    if ( IsFlagSet ( FLAG_DUAL_FIELD_ON, datagram->flags ) )
      this->SetFlag ( FLAG_DUAL_FIELD_ON );
    else
      this->ClearFlag ( FLAG_DUAL_FIELD_ON );

    // Swap fields
    if ( IsFlagSet ( FLAG_SWAP_FIELD_ON, datagram->flags ) )
      this->SetFlag ( FLAG_SWAP_FIELD_ON );
    else
      this->ClearFlag ( FLAG_SWAP_FIELD_ON );

    // Brightness - take the 3 MSB of the flag
    b = ( uint8_t ) datagram->flags >> 5;
    this->Brightness ( b - 1 );
    break;

  case MESSAGE_NULL:	// ignore these messages
  case GET_STATE:
  default:
    break;
  }

  return true;
}
