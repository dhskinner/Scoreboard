#ifdef LED_DMD2

// double buffering
SPIDMD		dmd ( DMD_DISPLAYS_ACROSS, DMD_DISPLAYS_DOWN );
DMDFrame	nextDMDFrame ( DMD_PIXELS_ACROSS, DMD_PIXELS_DOWN );

// Set these up with out-of-range values so they are refreshed first time round
uint8_t   dmdScore0 = 254;
uint8_t   dmdScore1 = 254;
uint8_t	  dmdMode = 254;
boolean   dmdDualFieldFlag = false;
boolean   dmdSwapFieldFlag = false;
boolean   dmdClockFlag = false;
uint8_t   dmdBrightness = 32;
time_t	  dmdTime = 0;

// ---------------------------------------------------------------------------------
boolean DMD_Start()
{
  dmd.begin();

  DMD_Test();

  return true;
}
// ---------------------------------------------------------------------------------
// Test pattern - should take less time than the master startup delay
void DMD_Test()
{
  // Set the brightness
  DMDSetBrightness();

  for ( uint8_t i = 0; i < DMD_PIXELS_ACROSS; i++ )
  {
    // Clear out the frame buffer
    nextDMDFrame.clearScreen();

    // Draw a vertical line
    nextDMDFrame.drawLine ( i, 0, i, DMD_PIXELS_DOWN - 1, GRAPHICS_ON );

    // Commit the new frame to the DMD
    dmd.swapBuffers ( nextDMDFrame );

    // Wait a bit
    delay ( SLAVE_TEST_PATTERN_DELAY );
  }
  
  // Blank the screen
  nextDMDFrame.clearScreen(); 
  dmd.swapBuffers ( nextDMDFrame ); 
}
// ---------------------------------------------------------------------------------
// Display the Scores based on DMD_ROLE
void DMD_Update()
{
  boolean redraw = false;
  uint8_t fieldID = FIELD1;	// default to field 1

  if (dmdStartupFlag == true)	// if this flag is true it means nothing has been received yet from the master
	return;
	
  // Check if any flags have changed since last time (always use the dual and swap field flags from field 1)
  if ( dmdDualFieldFlag != scoreboard[fieldID].IsDualFieldsOn() ||
       dmdSwapFieldFlag != scoreboard[fieldID].IsSwapFieldsOn() )
  {
    dmdSwapFieldFlag = scoreboard[fieldID].IsSwapFieldsOn();
    dmdDualFieldFlag = scoreboard[fieldID].IsDualFieldsOn();
    redraw = true;
  }

  // Check we still have the right fieldID
  if ( dmdDualFieldFlag )
    if ( ( DMD_ROLE == 1 && dmdSwapFieldFlag ) || ( DMD_ROLE == 2 && !dmdSwapFieldFlag ) )
      fieldID = FIELD2;

  // Has anything else changed?
  if ( dmdScore0 != scoreboard[fieldID].Score0() ||
       dmdScore1 != scoreboard[fieldID].Score1() ||
       dmdMode != scoreboard[fieldID].MatchTimeMode() ||
       dmdBrightness != scoreboard[fieldID].Brightness() ||
       redraw == true
     )
  {
    // Grab the state
    dmdScore0 = scoreboard[fieldID].Score0();
    dmdScore1 = scoreboard[fieldID].Score1();
    dmdMode = scoreboard[fieldID].MatchTimeMode();
    dmdBrightness = scoreboard[fieldID].Brightness();

    // Clear out the frame buffer
    nextDMDFrame.clearScreen();

    // Set the brightness
    DMDSetBrightness();

    // Re-draw the display excluding the clock
    DMDRedraw();

    // Set the flag
    redraw = true;
  }

  // are we in dual mode
  if ( dmdDualFieldFlag )
  {
    // has the clock changed?
    if ( dmdClockFlag != scoreboard[fieldID].IsClockOn() ||
         dmdTime != ( scoreboard[fieldID].IsClockOn() ? now() : scoreboard[fieldID].MatchTime() ) ||
         redraw == true )
    {
      // Grab the state
      dmdClockFlag = scoreboard[fieldID].IsClockOn();
      dmdTime = ( dmdClockFlag ? now() : scoreboard[fieldID].MatchTime() );

      // If no other fields have been redrawn in this iteration,
      // copy the whole existing frame buffer from the current DMD then clear the lower panel
      if ( redraw != true )
      {
        nextDMDFrame.copyFrame ( dmd, 0, 0 );
        nextDMDFrame.drawFilledBox ( 0, PANEL_HEIGHT*3 , PANEL_WIDTH*2, PANEL_HEIGHT*4 - 1, GRAPHICS_OFF );
      }

      // Re-draw the clock
      DMDRedrawClock();

      // Set the flag
      redraw = true;
    }
  }

  // Commit the new frame to the DMD
  if ( redraw == true )
    dmd.swapBuffers ( nextDMDFrame );
}
// ---------------------------------------------------------------------------------
void DMDRedraw()
{
  uint8_t xpos = 0, ypos = 0, index = 0;
  char temp[10];

  // ------------------------------
  //	Normal scoring mode
  if ( !dmdDualFieldFlag )
  {
    if ( ( DMD_ROLE == 1 && !dmdSwapFieldFlag ) || ( DMD_ROLE == 2 && dmdSwapFieldFlag ) )
    {
      // Home team label
      nextDMDFrame.selectFont ( FONT14 );
      ypos = ( PANEL_HEIGHT - 14 ) /2;			// Magic number - 14 is the font height (less a couple of pixels on the bottom)
      xpos = ( nextDMDFrame.width - nextDMDFrame.stringWidth ( DMD_TEAM0_NAME ) ) /2;
      nextDMDFrame.drawString ( xpos, ypos, DMD_TEAM0_NAME );

      // Home team score
      itoa ( dmdScore0, temp, 10 );

      // Pick the largest font that will fit
      nextDMDFrame.selectFont ( NUMERAL44X28 );
      if ( nextDMDFrame.stringWidth ( temp ) > nextDMDFrame.width )
        nextDMDFrame.selectFont ( NUMERAL44X24 );

      ypos = PANEL_HEIGHT + ( PANEL_HEIGHT*3 - 44 ) /2;		// Magic number - 44 is the font height
      xpos = ( nextDMDFrame.width - nextDMDFrame.stringWidth ( temp ) ) /2;
      nextDMDFrame.drawString ( xpos, ypos, temp );
    }
    else
    {
      // Guest team label
      nextDMDFrame.selectFont ( FONT14 );
      ypos = ( PANEL_HEIGHT - 14 ) /2;						// Magic number - 14 is the font height (less a couple of pixels on the bottom)
      xpos = ( nextDMDFrame.width - nextDMDFrame.stringWidth ( DMD_TEAM1_NAME ) ) /2;
      nextDMDFrame.drawString ( xpos, ypos, DMD_TEAM1_NAME );

      // Guest team score
      itoa ( dmdScore1 , temp, 10 );

      // Pick the largest font that will fit
      nextDMDFrame.selectFont ( NUMERAL44X28 );
      if ( nextDMDFrame.stringWidth ( temp ) > nextDMDFrame.width )
        nextDMDFrame.selectFont ( NUMERAL44X24 );

      ypos = PANEL_HEIGHT + ( PANEL_HEIGHT*3 - 44 ) /2;		// Magic number - 44 is the font height
      xpos = ( nextDMDFrame.width - nextDMDFrame.stringWidth ( temp ) ) /2;
      nextDMDFrame.drawString ( xpos, ypos, temp );
    }
  }
  // ------------------------------
  // Dual scoring mode
  else
  {
    nextDMDFrame.selectFont ( FONT14 );
    const uint8_t fontHeight = 14;							// Magic number - 14 is the font height (less a couple of pixels on the bottom)
    index = 0;

    if ( DMD_ROLE == 1 )
    {
      // Arrow is on the left
      temp[index]='<';
      index ++;
    }

    // Add the label text
    if ( ( DMD_ROLE == 1 && !dmdSwapFieldFlag ) || ( DMD_ROLE == 2 && dmdSwapFieldFlag ) )
      index = DMDCopyToBuffer ( DMD_FIELD1_NAME, temp, index, DMD_FIELD1_NAME_LENGTH );
    else
      index = DMDCopyToBuffer ( DMD_FIELD2_NAME, temp, index, DMD_FIELD2_NAME_LENGTH );

    if ( DMD_ROLE == 2 )
    {
      // Arrow is on the right
      temp[index]='>';
      index++;
    }

    // Null terminate the string
    temp[index] = 0;

    // Center and print the text
    ypos = ( PANEL_HEIGHT - fontHeight ) /2;
    xpos = ( nextDMDFrame.width - nextDMDFrame.stringWidth ( temp ) ) /2;
    nextDMDFrame.drawString ( xpos, ypos, temp );

    // Team 0 Label
    ypos += PANEL_HEIGHT;
    xpos = 0;
    nextDMDFrame.drawString ( xpos, ypos, DMD_TEAM0_SHORTNAME );

    // Team 0 Score
    itoa ( constrain ( dmdScore0, DMD_DUALSCORE_MIN, DMD_DUALSCORE_MAX ), temp, 10 );
    xpos = DMD_PIXELS_ACROSS - nextDMDFrame.stringWidth ( temp );	// right justify
    nextDMDFrame.drawString ( xpos, ypos, temp );

    // Team 1 Label
    ypos += PANEL_HEIGHT;
    xpos = 0;
    nextDMDFrame.drawString ( xpos, ypos, DMD_TEAM1_SHORTNAME );

    // Team 1 Score
    itoa ( constrain ( dmdScore1, DMD_DUALSCORE_MIN, DMD_DUALSCORE_MAX ), temp, 10 );
    xpos = DMD_PIXELS_ACROSS - nextDMDFrame.stringWidth ( temp );	// right justify
    nextDMDFrame.drawString ( xpos, ypos, temp );
  }
}
// ---------------------------------------------------------------------------------
void DMDRedrawClock()
{
  uint8_t xpos = 0, ypos = 0;
  char temp[10];

  // ------------------------------
  //	Normal scoring mode
  if ( !dmdDualFieldFlag )
  {
    // no timer
  }
  // ------------------------------
  // Dual scoring mode
  else
  {
    // Set the font
    nextDMDFrame.selectFont ( FONT14 );
    const uint8_t fontHeight = 14;			// Magic number - 14 is the font height (less a couple of pixels on the bottom)

    if ( dmdClockFlag == true )	// Clock
    {
      DMDPrint2Digits ( hour (), temp, 0 );
      temp[2] = ':';
      DMDPrint2Digits ( minute (), temp, 3 );
      temp[5] = ':';
      DMDPrint2Digits ( second (), temp, 6 );
      temp[8] = 0;
    }
    else						// Timer
    {
      DMDPrint2Digits ( ( uint8_t ) ( ( float ) dmdTime / ( float ) 60 ) , temp, 0 );
      temp[2] = ':';
      DMDPrint2Digits ( dmdTime%60, temp, 3 );
      temp[5] = 0;
    }
    ypos = PANEL_HEIGHT*3 + ( PANEL_HEIGHT - fontHeight ) /2 ;
    xpos = ( nextDMDFrame.width - nextDMDFrame.stringWidth ( temp ) ) /2;
    nextDMDFrame.drawString ( xpos, ypos, temp );
  }
}
// ---------------------------------------------------------------------------------
void DMDPrint2Digits ( uint8_t val, char * buffer, uint8_t index )
{
  if ( val < 10 )
    buffer[index] = '0';
  else
    buffer[index] = ( ( uint8_t ) val/10 ) + '0';

  index++;
  buffer[index] = val%10 + '0';
}
// ---------------------------------------------------------------------------------
uint8_t DMDCopyToBuffer ( char * text, char * buffer, uint8_t index, uint8_t len )
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
void DMDSetBrightness()
{
  // Scale from 0 = zero brightness to six = 255
  int temp = dmdBrightness * 43;
  dmd.setBrightness ( constrain ( temp, 0, 255 ) );
}
#endif

