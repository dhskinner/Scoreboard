/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include <ScoreboardButtonClass.h>
#ifdef ESP8266
// ---------------------------------------------------------------------------------
ScoreboardButtonClass::ScoreboardButtonClass ( uint8_t bpin, ButtonHandlerFunction bfunction, int bfunctionvalue, unsigned long bmillis, unsigned long brepeatmillis )
    : _pin ( bpin )
    , _currentReading ( RELEASED )
    , _previousReading ( RELEASED )
    , _state ( ButtonState::IDLE )
    , _delay ( bmillis )
    , _repeatDelay ( brepeatmillis )
    , _stateTimer ( 0 )
    , _functionPointer ( bfunction )
    , _functionValue ( bfunctionvalue )
{
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardButtonClass::Pin()
{
    return _pin;
}
// ---------------------------------------------------------------------------------
int ScoreboardButtonClass::Value()
{
    return _functionValue;
}
// ---------------------------------------------------------------------------------
// Is the button currently pressed
bool ScoreboardButtonClass::IsPressed()
{
    return _currentReading;
}
// ---------------------------------------------------------------------------------
// Is the button currently released
bool ScoreboardButtonClass::IsReleased()
{
    return !_currentReading;
}
// ---------------------------------------------------------------------------------
// Is the button currently idle (meaning that the button is fully RELEASED,
// and is not waiting for debouncing or anything else to complete. Note this is a
// ButtonState ENUM, not the boolean PRESSED OR RELEASED
bool ScoreboardButtonClass::IsIdle()
{
    if ( _state == ButtonState::IDLE || _state == ButtonState::JUSTRELEASE )
        return true;
    else
        return false;
}
// ---------------------------------------------------------------------------------
// has the button just been toggled from on -> off, or vice versa
bool ScoreboardButtonClass::JustToggled()
{
    if ( _state == ButtonState::JUSTPRESS || _state == ButtonState::JUSTRELEASE )
        return true;
    return false;
}
// ---------------------------------------------------------------------------------
// has the button gone from off -> on
bool ScoreboardButtonClass::JustPressed()
{
    if ( _state == ButtonState::JUSTPRESS )
    {
        _state = ButtonState::PRESS;
        return true;
    }
    else
        return false;
}
// ---------------------------------------------------------------------------------
// has the button gone from on -> off
bool ScoreboardButtonClass::JustReleased()
{
    if ( _state == ButtonState::JUSTRELEASE )
    {
        _state = ButtonState::IDLE;
        return true;
    }
    else
        return false;
}
// ---------------------------------------------------------------------------------
// This processes the button, including debouncing, delayed (long) presses and repeats
bool ScoreboardButtonClass::Process ( uint8_t pinstate )
{
    // check to see if you just pressed the button
    // (i.e. the input went from LOW to HIGH), and you've waited
    // long enough since the last press to ignore any noise
    bool newReading = bitRead ( pinstate, _pin );

#ifdef BUTTON_DEBUG
	USTREAM ( F ( "Button " ) << _pin << F ( " is " ) << ( newReading == PRESSED ? F ( "on " ) : F ( "off " ) ) );
#endif

    // if the pin reading has changed, reset the delay timer
    if ( newReading != _previousReading )
    {
        _state = ButtonState::DEBOUNCE;
        _stateTimer = millis() + I2C_BUTTON_DEBOUNCE_MILLIS;
#ifdef BUTTON_DEBUG
        USTREAM ( F ( " (debounce)" ) << endl );
#endif
    }

    // whatever the reading is at, it's been there for longer than
    // the debounce delay, so take it as the actual current state.
    else if ( millis() >= _stateTimer )
    {
        switch ( _state )
        {
        case ButtonState::DEBOUNCE:

            // Button has just been debounced and is in a new state
            _currentReading = newReading;

            // Was it released?
            if ( _currentReading == RELEASED )
            {
#ifdef BUTTON_DEBUG
				USTREAM ( F ( " (released)" ) << endl );
#endif
                _state = ButtonState::JUSTRELEASE;
                break;
            }

            // The button was just pressed...
#ifdef BUTTON_DEBUG
            USTREAM ( F ( " (pressed)" ) );
#endif
            _state = ButtonState::JUSTPRESS;

            // Do we need to wait for a delay timer on button presses?
            if ( _delay != 0 )
            {
#ifdef BUTTON_DEBUG
                USTREAM ( F ( " (delay)" ) << endl );
#endif
                _stateTimer += _delay;
                break;
            }

        // Fall through to state 'justpress'

        case ButtonState::JUSTPRESS:

            // Do the callback function
#ifdef BUTTON_DEBUG
            USTREAM ( F ( " (callback)" ) << endl );
#endif
            _functionPointer ( _functionValue );

            // Is there a repeat setting?
            if ( _repeatDelay != 0 )
            {
                _stateTimer += _repeatDelay;	// leave in state 'justpress'
#ifdef BUTTON_DEBUG
				USTREAM(F("(repeat)") << endl);
#endif
            }
            else
                _state = ButtonState::PRESS;	// change to state 'press'
            break;

        case ButtonState::PRESS:
#ifdef BUTTON_DEBUG
			USTREAM(endl);
#endif
            break;								// do nothing / just keep polling until the button is released

		case ButtonState::JUSTRELEASE:			// fall through to idle
		default:								// do nothing - button is 'idle'
#ifdef BUTTON_DEBUG
			USTREAM(endl);
#endif
			break;
        }
    }
    else
    {
#ifdef BUTTON_DEBUG
        USTREAM ( endl );
#endif
    }
    // save the reading for next time through the loop
    _previousReading = newReading;

    // return the button idle state
    return this->IsIdle();
}
#endif
