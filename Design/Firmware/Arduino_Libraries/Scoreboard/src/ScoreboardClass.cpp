/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardClass.h"
// ---------------------------------------------------------------------------------
// Constructor
ScoreboardClass::ScoreboardClass()
{
    this->_fieldID = 0;
    this->_timerReference = 0;
    this->_score[SCORE1] = 0;
    this->_score[SCORE2] = 0;
    this->_flags = FLAG_BRIGHTNESS_6;
    this->TeamName[0] = String ( DMD_TEAM0_NAME );
    this->TeamName[1] = String ( DMD_TEAM1_NAME );
}
// ---------------------------------------------------------------------------------
// Get/Set the field ID of this scoreboard object (overloaded)
uint8_t ScoreboardClass::ID()
{
    return this->_fieldID;
}
uint8_t ScoreboardClass::ID ( uint8_t id )
{
    this->_fieldID = id;
    return this->_fieldID;
}
// ---------------------------------------------------------------------------------
// Start and/or un-pause the timer
void ScoreboardClass::Run()
{
    switch ( this->MatchTimeMode() )
    {
    case FLAG_TIMER_PAUSE:        // if paused - restart the timer (and shift startime right by the amount of the pause)
        this->_timerReference = millis() - this->_timerReference;
        this->ClearFlag ( FLAG_TIMER_PAUSE );
        this->SetFlag ( FLAG_TIMER_RUN );
        break;

    case FLAG_TIMER_STOP:         // if stopped, start the timer
        this->_timerReference = millis();
        this->SetFlag ( FLAG_TIMER_RUN );
        break;

    case FLAG_TIMER_RUN:         // if running - do nothing
    default:
        break;
    }
}
// ---------------------------------------------------------------------------------
// Pause the timer
void ScoreboardClass::Pause()
{
    switch ( this->MatchTimeMode() )
    {
    case FLAG_TIMER_RUN:   // if running - pause
        this->_timerReference = millis() - this->_timerReference;
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
void ScoreboardClass::Stop()
{
    this->ClearFlag ( FLAG_TIMER_RUN );
    this->ClearFlag ( FLAG_TIMER_PAUSE );
    this->_timerReference = 0;
}
// ---------------------------------------------------------------------------------
// Get/Set the current matchtime (in seconds)
time_t ScoreboardClass::MatchTime()
{
	// round to nearest second
	uint32_t tm = this->MatchTimeMillis();
	if (tm % 1000 > 500)
		return (time_t) (tm / 1000) + 1;
	else
		return (time_t) (tm / 1000);
}
// Get/Set the current matchtime (in seconds)
// WARNING you must set the timer mode first
time_t ScoreboardClass::MatchTime ( time_t tm ) //, uint8_t mode )
{
    // check the mode (just in case there are extraneous bits)
    if ( this->IsFlagSet ( FLAG_TIMER_RUN ) )
        this->_timerReference = millis() - tm * 1000;
    else if ( this->IsFlagSet ( FLAG_TIMER_PAUSE ) )
        this->_timerReference =tm * 1000;
    else
        this->Stop();

    return this->MatchTime();
}
// Get/Set the current matchtime (in millis)
uint32_t ScoreboardClass::MatchTimeMillis()
{
    uint32_t tm;

    switch ( this->MatchTimeMode() )
    {
    case FLAG_TIMER_RUN:
        tm = millis() - this->_timerReference;
        break;

    case FLAG_TIMER_PAUSE:
        tm = this->_timerReference;
        break;

    case FLAG_TIMER_STOP:
    default:
        tm = 0;
        break;
    }
    if ( tm > (uint32_t) MAX_MATCHTIME * 1000 )
        tm = (uint32_t) MAX_MATCHTIME * 1000;

    return tm;
}
// ---------------------------------------------------------------------------------
// Get/Set the current mode (overloaded)
uint8_t ScoreboardClass::MatchTimeMode()
{
    if ( this->IsFlagSet ( FLAG_TIMER_RUN ) )
        return FLAG_TIMER_RUN;
    else if ( this->IsFlagSet ( FLAG_TIMER_PAUSE ) )
        return FLAG_TIMER_PAUSE;
    else
        return FLAG_TIMER_STOP;
}
uint8_t ScoreboardClass::MatchTimeMode ( uint8_t flag )
{
    if ( this->IsFlagSet ( FLAG_CLOCK_ON, flag ) )	// Set the clock flag
        this->Clock ( true );
    else
        this->Clock ( false );

    // check the mode (just in case there are extraneous bits)
    if ( this->IsFlagSet ( FLAG_TIMER_RUN, flag ) )
        this->Run();
    else if ( this->IsFlagSet ( FLAG_TIMER_PAUSE, flag ) )
        this->Pause();
    else
        this->Stop();

    return this->MatchTimeMode();
}
// ---------------------------------------------------------------------------------
// Set the current system time
void ScoreboardClass::SystemTime ( time_t tm )
{
    if ( now() != tm )
        setTime ( tm );
}
// ---------------------------------------------------------------------------------
// Get/Set score 0 (overloaded)
uint8_t ScoreboardClass::Score1()
{
    return this->_score[SCORE1];
}
uint8_t ScoreboardClass::Score1 ( uint8_t score )
{
    if ( this->_score[SCORE1] != score )
        this->_score[SCORE1] = constrain ( score, MIN_SCORE, MAX_SCORE );

    return this->_score[SCORE1];
}
// ---------------------------------------------------------------------------------
// Get/Set score 1 (overloaded)
uint8_t ScoreboardClass::Score2()
{
    return this->_score[SCORE2];
}
uint8_t ScoreboardClass::Score2 ( uint8_t score )
{
    if ( this->_score[SCORE2] != score )
        this->_score[SCORE2] = constrain ( score, MIN_SCORE, MAX_SCORE );

    return this->_score[SCORE2];
}
// ---------------------------------------------------------------------------------
// Increment the score by id
uint8_t	ScoreboardClass::ScoreIncrement ( uint8_t id, int increment )
{
    int temp;

    if ( id != SCORE1 && id != SCORE2 )
        return 0;

    temp = this->_score[id] + increment;
    temp = constrain ( temp, MIN_SCORE, MAX_SCORE );
    this->_score[id] = ( uint8_t ) temp;
    return this->_score[id];
}

// ---------------------------------------------------------------------------------
// Get/Set the score by id
uint8_t ScoreboardClass::ScoreID ( uint8_t id )
{
    switch ( id )
    {
    case SCORE1:
    case SCORE2:
        return this->_score[id];
    default:
        return 0;
    }
}
// ---------------------------------------------------------------------------------
// Get/Set the score by id
uint8_t ScoreboardClass::ScoreID ( uint8_t id, uint8_t value )
{
    switch ( id )
    {
    case SCORE1:
    case SCORE2:
        this->_score[id] = value;
        return this->_score[id];
    default:
        return 0;
    }
}
// ---------------------------------------------------------------------------------
// Get/Set scores
void ScoreboardClass::Score ( uint8_t score1, uint8_t score2 )
{
    this->Score1 ( score1 );
    this->Score2 ( score2 );
}
// ---------------------------------------------------------------------------------
// Get/Set all flags
uint8_t ScoreboardClass::Flags()
{
    return this->_flags;
}
uint8_t ScoreboardClass::Flags ( uint8_t flags )
{
    if ( this->_flags != flags )
        this->_flags = flags;

    return this->_flags;
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardClass::SetFlag ( uint8_t flag )
{
    // OR the bitmasks together
    this->_flags = this->_flags | flag;

    return _flags;
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardClass::ClearFlag ( uint8_t flag )
{
    // Invert the bit string with the bitwise NOT operator (~), then AND it.
    this->_flags &= ~ ( flag );

    return _flags;
}
// ---------------------------------------------------------------------------------
boolean ScoreboardClass::IsFlagSet ( uint8_t flag )
{
    return this->IsFlagSet ( flag, this->_flags );
}
boolean ScoreboardClass::IsFlagSet ( uint8_t flag, uint8_t flagByte )
{
    if ( ( flagByte & flag ) == flag )		// check the flag is set
        return true;
    else
        return false;
}
// ---------------------------------------------------------------------------------
// Get/Set the flag for dual fields
boolean	ScoreboardClass::Dual()
{
    return this->IsFlagSet ( FLAG_DUAL_ON );
}
// Get/Set the flag for dual fields
boolean	ScoreboardClass::Dual ( boolean state )
{
    if ( state )
        this->SetFlag ( FLAG_DUAL_ON );
    else
        this->ClearFlag ( FLAG_DUAL_ON );
    return state;
}
// ---------------------------------------------------------------------------------
// Get/Set the flag for swap
boolean	ScoreboardClass::Swap()
{
    return this->IsFlagSet ( FLAG_SWAP_ON );
}
// Get/Set the flag for swap
boolean	ScoreboardClass::Swap ( boolean state )
{
    if ( state )
        this->SetFlag ( FLAG_SWAP_ON );
    else
        this->ClearFlag ( FLAG_SWAP_ON );
    return state;

}
// ---------------------------------------------------------------------------------
// Get/Set the flag for clock on or off
boolean	ScoreboardClass::Clock()
{
    return this->IsFlagSet ( FLAG_CLOCK_ON );
}
// Get/Set the flag for clock on or off
boolean	ScoreboardClass::Clock ( boolean state )
{
    if ( state )
        this->SetFlag ( FLAG_CLOCK_ON );
    else
        this->ClearFlag ( FLAG_CLOCK_ON );
    return state;
}
// ---------------------------------------------------------------------------------
// Get/Set the brightness level (valid values are 0 to 6 - anything else assumes max brightness)
// Note that the bitmask used for setting the flags is referenced from 1
uint8_t ScoreboardClass::Brightness()
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
uint8_t ScoreboardClass::Brightness ( uint8_t brightness )
{
    // Range check zero to six - if > 6 assume its max brightness
    if ( brightness > 6 )
        brightness = 6;

    // Add one - the bitmask used for setting the flag
    // is referenced from 1 to seven (zero is invalid)
    brightness++;

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
void ScoreboardClass::GetDatagram ( ScoreboardDatagram *datagram )
{
    datagram->messageID = BROADCAST;
    datagram->fieldID = this->ID();
    datagram->score1 = this->Score1();
    datagram->score2 = this->Score2();
    datagram->systemTime = now();
    datagram->matchTime = this->MatchTime();
    datagram->flags = this->Flags();
}
boolean ScoreboardClass::SetDatagram ( ScoreboardDatagram *datagram )
{
    uint8_t b = 0;

    // Check the field ID matches
    if ( datagram->fieldID != this->ID() )
        return false;

    // store values depending on the messageID
    switch ( datagram->messageID )
    {
    case BROADCAST:													// Store everything
        this->Flags ( datagram->flags );							// Set the flags
        this->Score1 ( datagram->score1 );							// Set the scores
        this->Score2 ( datagram->score2 );							// Set the scores
        this->SystemTime ( datagram->systemTime );					// Set system time
        _flags = datagram->flags;									// Set the flags
        this->MatchTime ( datagram->matchTime );					// Set match time
        break;

    case SET_SCORE:													// Store the scores (ignore the rest)
        this->Score1 ( datagram->score1 );							// Set the scores
        this->Score2 ( datagram->score2 );							// Set the scores
        break;

    case SET_TIMER:													// Store the timer mode and value (ignore the rest)
        if ( this->IsFlagSet ( FLAG_CLOCK_ON, datagram->flags ) )	// Set the clock flag
            this->SetFlag ( FLAG_CLOCK_ON );
        else
            this->ClearFlag ( FLAG_CLOCK_ON );
        //this->MatchTime ( datagram->matchTime, datagram->flags );	// Set the match timer
        this->MatchTimeMode ( datagram->flags );					// Set the match timer mode only
        break;

    case SET_MODE:													// Store the mode flags (ignore the rest)
        // Clock													// Dual fields															// Dual fields														// Dual fields
        if ( IsFlagSet ( FLAG_CLOCK_ON, datagram->flags ) )
            this->SetFlag ( FLAG_CLOCK_ON );
        else
            this->ClearFlag ( FLAG_CLOCK_ON );

        // Dual fields
        if ( IsFlagSet ( FLAG_DUAL_ON, datagram->flags ) )
            this->SetFlag ( FLAG_DUAL_ON );
        else
            this->ClearFlag ( FLAG_DUAL_ON );

        // Swap
        if ( IsFlagSet ( FLAG_SWAP_ON, datagram->flags ) )
            this->SetFlag ( FLAG_SWAP_ON );
        else
            this->ClearFlag ( FLAG_SWAP_ON );

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
