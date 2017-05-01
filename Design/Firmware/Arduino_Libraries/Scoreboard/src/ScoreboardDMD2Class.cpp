/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardDMD2Class.h"

#ifndef HW_ESP8266

// ---------------------------------------------------------------------------------
ScoreboardDMD2Class::ScoreboardDMD2Class ( uint8_t display_across, uint8_t display_down, DMDRoleID role, uint8_t fieldID ) :
    _dmd ( display_across, display_down ),
    _nextdmd ( display_across * PANEL_WIDTH , display_down * PANEL_HEIGHT )
{
    _dmdRole = role;
    _dmdFieldID = fieldID;
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::Start()
{
    _dmd.begin();

    this->Test();

    UPRINTLN ( F ( "Started DMD Panel" ) );
}
// ---------------------------------------------------------------------------------
// Test pattern - should take less time than the master startup delay
void ScoreboardDMD2Class::Test()
{
    // Set the brightness
    this->SetBrightness();

    for ( uint8_t i = 0; i < _nextdmd.width; i++ )
    {
        // Clear out the frame buffer
        _nextdmd.clearScreen();

        // Draw a vertical line
        _nextdmd.drawLine ( i, 0, i, _nextdmd.height - 1, GRAPHICS_ON );

        // Commit the new frame to the DMD
        _dmd.swapBuffers ( _nextdmd );

        // Wait a bit
        delay ( DMD_TEST_PATTERN_DELAY );
    }

    // Blank the screen
    _nextdmd.clearScreen();
    _dmd.swapBuffers ( _nextdmd );
}
// ---------------------------------------------------------------------------------
// Update the 4x2 LED Score Panels
void ScoreboardDMD2Class::UpdatePanel()
{
    // Is there anything to do?
    if ( !this->RedrawCheck() )
        return;

    // Clear out the frame buffer
    _nextdmd.clearScreen();

    // Set the brightness
    this->SetBrightness();

    // Redraw a single field
    if ( _dmdDualFlag )
        RedrawPanelSingle();

    // Redraw dual Fields
    else
    {
        // Re-draw the display excluding the clock
        this->RedrawPanelDual();

        // Re-draw the clock
        this->RedrawPanelClock();
    }

    // Commit the new frame to the DMD
    _dmd.swapBuffers ( _nextdmd );
}
// ---------------------------------------------------------------------------------
// Update the repeater panel
void ScoreboardDMD2Class::UpdateRepeater ( DMDRepeaterMode mode )
{
    if ( mode == DMDRepeaterMode::DMD_REPEATER_RANDOM )
    {
        this->RandomPixels();
        return;
    }

    // Is there anything to do?
    bool redrawcheck = ( this->RedrawCheck() );

    // Clear out the frame buffer
    _nextdmd.clearScreen();

    // Set the brightness to a fixed value (otherwise the DMD is too bright for indoor use)
    // this->SetBrightness();
    _dmd.setBrightness ( DMD_REPEATER_BRIGHTNESS );

    // Redraw the repeater panel
    if ( mode == DMD_REPEATER_SCORE )
        RedrawRepeaterScore();
    else if ( mode == DMD_REPEATER_TIMER )
        RedrawRepeaterTimer();
    else if ( mode == DMD_REPEATER_CLOCK )
        RedrawRepeaterRealTimeClock();

    // Commit the new frame to the DMD
    _dmd.swapBuffers ( _nextdmd );
}
// ---------------------------------------------------------------------------------
// Update the test panel
void ScoreboardDMD2Class::UpdateTestPanel ( bool forceupdate )
{
    char temp[10];

    // Is there anything to do?
    if ( !this->RedrawCheck() && !forceupdate )
        return;

    // Clear out the frame buffer
    _nextdmd.clearScreen();

    // Set the brightness
    this->SetBrightness();

    // Set the font - this font is 16 pixels high and 7 wide per numeral
    _nextdmd.selectFont ( SystemFont5x7 );

    if ( this->_dmdSwapFlag )
    {
        // Score2 for the currently selected field (always 2 digits)
        this->Print2Digits ( constrain ( scoreboard[_dmdFieldID].Score2(), 0, 99 ), temp, 0 );
        _nextdmd.drawString ( 3, 0, temp );

        // Score1 for the currently selected field (always 2 digits)
        this->Print2Digits ( constrain ( scoreboard[_dmdFieldID].Score1(), 0, 99 ), temp, 0 );
        _nextdmd.drawString ( 18, 0, temp );
    }
    else
    {
        // Score1 for the currently selected field (always 2 digits)
        this->Print2Digits ( constrain ( scoreboard[_dmdFieldID].Score1(), 0, 99 ), temp, 0 );
        _nextdmd.drawString ( 3, 0, temp );

        // Score2 for the currently selected field (always 2 digits)
        this->Print2Digits ( constrain ( scoreboard[_dmdFieldID].Score2(), 0, 99 ), temp, 0 );
        _nextdmd.drawString ( 18, 0, temp );
    }

    // Row 2 is for the timer
    this->Print2Digits ( ( uint8_t ) ( ( float ) _dmdTime / ( float ) 60 ), temp, 0 );
    _nextdmd.drawString ( 3, 9, temp );

    this->Print2Digits ( _dmdTime % 60, temp, 0 );
    _nextdmd.drawString ( 18, 9, temp );

    // Add some squares
    _nextdmd.drawBox ( 15, 10 , 16, 11 );
    _nextdmd.drawBox ( 15, 13, 16, 14 );

    // Add lines on either side to show the field
    if ( _dmdFieldID != DEFAULT_FIELD )
    {
        _nextdmd.drawLine ( 0, 0, 0, _nextdmd.width-1 );
        _nextdmd.drawLine ( _nextdmd.width-1, 0, _nextdmd.width-1, _nextdmd.height-1 );
    }

    // Commit the new frame to the DMD
    _dmd.swapBuffers ( _nextdmd );
}
// ---------------------------------------------------------------------------------
// Check for anything that needs updating on the current display
bool ScoreboardDMD2Class::RedrawCheck()
{
    time_t displayTime = 0;
    bool redraw = false;

    // Check if the brightness has changed since last time
    // (always use the brightness flag from DMD_DEFAULT_FIELD)
    if ( _dmdBrightness != scoreboard[DEFAULT_FIELD].Brightness() )
    {
        _dmdBrightness = scoreboard[DEFAULT_FIELD].Brightness();
        redraw = true;
    }

    // Check if the field flags have changed since last time
    // (always use the dual field and swap field flags from DMD_DEFAULT_FIELD)
    if ( _dmdDualFlag != scoreboard[DEFAULT_FIELD].Dual() ||
            _dmdSwapFlag != scoreboard[DEFAULT_FIELD].Swap() )
    {
        _dmdSwapFlag = scoreboard[DEFAULT_FIELD].Swap();
        _dmdDualFlag = scoreboard[DEFAULT_FIELD].Dual();

        // If we are showing dual fields, which field is this display?
        if ( _dmdDualFlag )
        {
            switch ( _dmdRole )
            {
            case DMD_LEFT_PANEL:
                _dmdFieldID = _dmdSwapFlag ? FIELD2 : FIELD1;
                break;
            case DMD_RIGHT_PANEL:
                _dmdFieldID = _dmdSwapFlag ? FIELD1 : FIELD2;
                break;
            case DMD_TEST_PANEL:
                _dmdFieldID = FIELD2;
            case DMD_REPEATER:
            default:
                break;
            }
            redraw = true;
        }
        else
        {
            // Only showing a single field
            _dmdFieldID = DEFAULT_FIELD;
        }
        redraw = true;
    }

    // Has anything else changed for this field?
    if ( _dmdScore1 != scoreboard[_dmdFieldID].Score1() ||
            _dmdScore2 != scoreboard[_dmdFieldID].Score2() ||
            _dmdTimerMode != scoreboard[_dmdFieldID].MatchTimeMode() )
    {
        _dmdScore1 = scoreboard[_dmdFieldID].Score1();
        _dmdScore2 = scoreboard[_dmdFieldID].Score2();
        _dmdTimerMode = scoreboard[_dmdFieldID].MatchTimeMode();
        redraw = true;
    }

    // Are we in dual mode or on the test panel? If so, then the clocks may need to be redrawn
    displayTime = scoreboard[_dmdFieldID].Clock() ? now() : scoreboard[_dmdFieldID].MatchTime();
    if ( ( _dmdClockFlag != scoreboard[_dmdFieldID].Clock() ) ||
            ( _dmdTime != displayTime ) )
    {
        _dmdClockFlag = scoreboard[_dmdFieldID].Clock();
        _dmdTime = displayTime;
        //USTREAM("Dmdtime=" << _dmdTime << endl);
        redraw = true;
    }
    return redraw;
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RedrawPanelSingle()
{
    uint8_t xpos = 0, ypos = 0;
    char temp[10];

    // Which team to draw?
    if ( ( _dmdRole == DMD_LEFT_PANEL && !_dmdSwapFlag ) || ( _dmdRole == DMD_RIGHT_PANEL && _dmdSwapFlag ) )
    {
        // Home team label
        _nextdmd.selectFont ( FONT14 );
        ypos = ( PANEL_HEIGHT - 14 ) / 2;			// Magic number - 14 is the font height (less a couple of pixels on the bottom)
        xpos = ( _nextdmd.width - _nextdmd.stringWidth ( DMD_TEAM0_NAME ) ) / 2;
        _nextdmd.drawString ( xpos, ypos, DMD_TEAM0_NAME );

        // Home team score
        itoa ( _dmdScore1, temp, 10 );

        // Pick the largest font that will fit
        _nextdmd.selectFont ( NUMERAL44X28 );
        if ( _nextdmd.stringWidth ( temp ) > _nextdmd.width )
            _nextdmd.selectFont ( NUMERAL44X24 );

        ypos = PANEL_HEIGHT + ( PANEL_HEIGHT * 3 - 44 ) / 2;		// Magic number - 44 is the font height
        xpos = ( _nextdmd.width - _nextdmd.stringWidth ( temp ) ) / 2;
        _nextdmd.drawString ( xpos, ypos, temp );
    }
    else
    {
        // Guest team label
        _nextdmd.selectFont ( FONT14 );
        ypos = ( PANEL_HEIGHT - 14 ) / 2;						// Magic number - 14 is the font height (less a couple of pixels on the bottom)
        xpos = ( _nextdmd.width - _nextdmd.stringWidth ( DMD_TEAM1_NAME ) ) / 2;
        _nextdmd.drawString ( xpos, ypos, DMD_TEAM1_NAME );

        // Guest team score
        itoa ( _dmdScore2, temp, 10 );

        // Pick the largest font that will fit
        _nextdmd.selectFont ( NUMERAL44X28 );
        if ( _nextdmd.stringWidth ( temp ) > _nextdmd.width )
            _nextdmd.selectFont ( NUMERAL44X24 );

        ypos = PANEL_HEIGHT + ( PANEL_HEIGHT * 3 - 44 ) / 2;		// Magic number - 44 is the font height
        xpos = ( _nextdmd.width - _nextdmd.stringWidth ( temp ) ) / 2;
        _nextdmd.drawString ( xpos, ypos, temp );
    }
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RedrawPanelDual()
{
    uint8_t xpos = 0, ypos = 0, index = 0;
    char temp[10];

    _nextdmd.selectFont ( FONT14 );
    const uint8_t fontHeight = 14;							// Magic number - 14 is the font height (less a couple of pixels on the bottom)
    index = 0;

    if ( _dmdRole == DMD_LEFT_PANEL )
    {
        // Arrow is on the left
        temp[index] = '<';
        index++;

        // Add the label text
        if ( _dmdSwapFlag )
            index = this->CopyToBuffer ( DMD_FIELD2_NAME, temp, index, DMD_FIELD2_NAME_LENGTH );
        else
            index = this->CopyToBuffer ( DMD_FIELD1_NAME, temp, index, DMD_FIELD1_NAME_LENGTH );
    }
    else
    {
        // Add the label text
        if ( _dmdSwapFlag )
            index = this->CopyToBuffer ( DMD_FIELD1_NAME, temp, index, DMD_FIELD1_NAME_LENGTH );
        else
            index = this->CopyToBuffer ( DMD_FIELD2_NAME, temp, index, DMD_FIELD2_NAME_LENGTH );

        // Arrow is on the right
        temp[index] = '>';
        index++;
    }

    // Null terminate the string
    temp[index] = 0;

    // Center and print the text
    ypos = ( PANEL_HEIGHT - fontHeight ) / 2;
    xpos = ( _nextdmd.width - _nextdmd.stringWidth ( temp ) ) / 2;
    _nextdmd.drawString ( xpos, ypos, temp );

    // Team 0 Label
    ypos += PANEL_HEIGHT;
    xpos = 0;
    _nextdmd.drawString ( xpos, ypos, DMD_TEAM0_SHORTNAME );

    // Team 0 Score
    itoa ( constrain ( _dmdScore1, DMD_DUAL_SCOREMIN, DMD_DUAL_SCOREMAX ), temp, 10 );
    xpos = _nextdmd.width - _nextdmd.stringWidth ( temp );	// right justify
    _nextdmd.drawString ( xpos, ypos, temp );

    // Team 1 Label
    ypos += PANEL_HEIGHT;
    xpos = 0;
    _nextdmd.drawString ( xpos, ypos, DMD_TEAM1_SHORTNAME );

    // Team 1 Score
    itoa ( constrain ( _dmdScore2, DMD_DUAL_SCOREMIN, DMD_DUAL_SCOREMAX ), temp, 10 );
    xpos = _nextdmd.width - _nextdmd.stringWidth ( temp );	// right justify
    _nextdmd.drawString ( xpos, ypos, temp );
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RedrawPanelClock()
{
    uint8_t xpos = 0, ypos = 0;
    char temp[10];

    //	No timer unless we're in dual field mode
    if ( !_dmdDualFlag )
        return;

    // Set the font
    _nextdmd.selectFont ( FONT14 );
    const uint8_t fontHeight = 14;			// Magic number - 14 is the font height (less a couple of pixels on the bottom)

    if ( _dmdClockFlag == true )	// Clock
    {
        this->Print2Digits ( hour(), temp, 0 );
        temp[2] = ':';
        this->Print2Digits ( minute(), temp, 3 );
        temp[5] = ':';
        this->Print2Digits ( second(), temp, 6 );
        temp[8] = 0;
    }
    else							// Timer
    {
        this->Print2Digits ( ( uint8_t ) ( ( float ) _dmdTime / ( float ) 60 ), temp, 0 );
        temp[2] = ':';
        this->Print2Digits ( _dmdTime % 60, temp, 3 );
        temp[5] = 0;
    }
    ypos = PANEL_HEIGHT * 3 + ( PANEL_HEIGHT - fontHeight ) / 2;
    xpos = ( _nextdmd.width - _nextdmd.stringWidth ( temp ) ) / 2;
    _nextdmd.drawString ( xpos, ypos, temp );
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RedrawRepeaterScore()
{
    uint8_t xpos = 0, ypos = 0;
    char temp[10];

    // Set the font - this font is 16 pixels high and 7 wide per numeral
    _nextdmd.selectFont ( NUMERAL16x7 );

    // First score - align to left centre
    itoa ( constrain ( _dmdScore1, DMD_REPEATER_SCOREMIN, DMD_REPEATER_SCOREMAX ), temp, 10 );
    xpos = DMD_REPEATER_SCORE1_CENTER - _nextdmd.stringWidth ( temp ) / 2;
    _nextdmd.drawString ( xpos, ypos, temp );

    // Second score - align to right centre
    itoa ( constrain ( _dmdScore2, DMD_REPEATER_SCOREMIN, DMD_REPEATER_SCOREMAX ), temp, 10 );
    xpos = DMD_REPEATER_SCORE2_CENTER - _nextdmd.stringWidth ( temp ) / 2;
    _nextdmd.drawString ( xpos, ypos, temp );
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RedrawRepeaterTimer()
{
    uint8_t xpos = 0, ypos = 1;
    time_t mins, secs;
    char temp[10];
    uint8_t index = 0;

    // Set the font
    _nextdmd.selectFont ( FONT14X6 );

    // Put the time in a temporary buffer
    mins = _dmdTime / 60;
    index = this->PrintDigits ( mins, temp, index );
    temp[index] = ':';
    index++;
    secs = _dmdTime % 60;
    index = this->Print2Digits ( secs, temp, index );

    // Align to centre
    xpos = DMD_REPEATER_CENTER - ( _nextdmd.stringWidth ( temp ) / 2 ) ;
    _nextdmd.drawString ( xpos, ypos, temp );
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RedrawRepeaterRealTimeClock()
{
    uint8_t xpos = 0, ypos = 1;
    char temp[10];
    uint8_t index = 0;

    // Set the font
    _nextdmd.selectFont ( FONT14X6 );

    // Put the time in a temporary buffer
    index = this->PrintDigits ( hourFormat12(), temp, index );
    temp[index] = ':';
    index++;
    index = this->Print2Digits ( minute(), temp, index );

    // Align to centre
    xpos = DMD_REPEATER_CENTER - ( _nextdmd.stringWidth ( temp ) / 2 );
    _nextdmd.drawString ( xpos, ypos, temp );
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardDMD2Class::Print2Digits ( uint16_t val, char * buffer, uint8_t index )
{
    if ( val < 10 )
        buffer[index] = '0';
    else
        buffer[index] = ( ( uint8_t ) val / 10 ) + '0';

    index++;
    buffer[index] = val % 10 + '0';
    index++;
    buffer[index] = 0;
    return index;
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardDMD2Class::PrintDigits ( uint16_t val, char * buffer, uint8_t index )
{
    if ( val > 10 )
    {
        val = val % 100;
        buffer[index] = val / 10 + '0';
        index++;
    }

    buffer[index] = ( val % 10) + '0';
    index++;
    buffer[index] = 0;
    return index;
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardDMD2Class::CopyToBuffer ( char const * text, char * buffer, uint8_t index, uint8_t len )
{
    for ( uint8_t i = 0; i < len; i++ )
    {
        buffer[index] = text[i];
        index++;
    }

    // null termination
    buffer[index] = 0;

    return ( index );
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::SetBrightness()
{
    // Scale from 0 = zero brightness to six = 255
    // This is a whole bunch of magic numbers TODO
    switch ( _dmdBrightness )
    {
    case 0:
        _dmd.setBrightness ( 10 );
        break;
    case 1:
        _dmd.setBrightness ( 20 );
        break;
    case 2:
        _dmd.setBrightness ( 40 );
        break;
    case 3:
        _dmd.setBrightness ( 80 );
        break;
    case 4:
        _dmd.setBrightness ( 160 );
        break;
    case 5:
        _dmd.setBrightness ( 200 );
        break;
    case 6:
    default:
        _dmd.setBrightness ( 255 );
        break;
    }
    // USTREAM ( F ( "DMD Bright=" ) << _dmdBrightness << endl );
}
// ---------------------------------------------------------------------------------
void ScoreboardDMD2Class::RandomPixels()
{
    static uint8_t randomPixel[DMD_RANDOM_PIXELS][2];
    static uint8_t randomPixelIndex = 0;

    // Clear out the frame buffer
    _nextdmd.clearScreen();

    // Increment the pixel count
    randomPixelIndex++;
    if ( randomPixelIndex >= DMD_RANDOM_PIXELS )
        randomPixelIndex = 0;

    // Generate a new pixel
    randomPixel[randomPixelIndex][0] = random ( _nextdmd.width );
    randomPixel[randomPixelIndex][1] = random ( _nextdmd.height );

    // Draw the pixels
    for ( uint8_t i = 0; i < DMD_RANDOM_PIXELS; i++ )
        _nextdmd.setPixel ( randomPixel[i][0], randomPixel[i][1], GRAPHICS_ON );

    // Commit the new frame to the DMD
    _dmd.swapBuffers ( _nextdmd );
}
#endif
