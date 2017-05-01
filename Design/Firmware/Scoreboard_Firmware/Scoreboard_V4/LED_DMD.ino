#ifdef LED_DMD

/*
// ---------------------------------------------------------------------------------
DMD       dmd(DMD_DISPLAYS_ACROSS, DMD_DISPLAYS_DOWN);
uint8_t   dmdScore0 = 254; 
uint8_t   dmdScore1 = 254;
uint8_t   dmdBrightness = 255;
// ---------------------------------------------------------------------------------
boolean DMDStart()
{
  //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
  Timer1.initialize( DMD_PERIOD );     //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
  Timer1.attachInterrupt( DMDScan );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

  //clear/init the DMD pixels held in RAM
  dmd.clearScreen( true );             //true is normal (all pixels off), false is negative (all pixels on)
  return true;
}
/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------
void DMDScan()
{
  dmd.scanDisplayBySPI();
}

// ---------------------------------------------------------------------------------
// Display the Scores based on DMD_ROLE
void DMDUpdate(uint8_t fieldID)
{
#if (DMD_ROLE == 0)		// 0 = test config with a single DMD panel (only displays field 1)
  if( scoreboard[fieldID].Score0() != dmdScore0 || scoreboard[fieldID].Score1() != dmdScore1 )
  {
	 dmd.clearScreen( true );
	DMDDisplayScoreSmall(scoreboard[fieldID].Score0(), DMD_SCORE0_X, DMD_SCORE0_Y);
	DMDDisplayScoreSmall(scoreboard[fieldID].Score1(), DMD_SCORE1_X, DMD_SCORE1_Y);
	dmdScore0 = scoreboard[fieldID].Score0();
	dmdScore1 = scoreboard[fieldID].Score1();
  }

  // ----------
   
#elif (DMD_ROLE == 1)	// 1 = left hand side label (2 panels wide x 1 high)

// ----------

#elif (DMD_ROLE == 2)	// 2 = left hand side numeral (2 panels wide x 3 high)		
  if( scoreboard[fieldID].Score0() != dmdScore0 )
  {
	dmd.clearScreen( true );
	DMDDisplayScoreLarge(scoreboard[fieldID].Score0(), DMD_SCORE0_X, DMD_SCORE0_Y);
	dmdScore0 = scoreboard[fieldID].Score0();
  }

// ----------
  
#elif (DMD_ROLE == 3)	// 3 = right hand side label (2 panels wide x 1 high)

// ----------

#elif (DMD_ROLE == 4)	// 4 = right hand side numeral (2 panels wide x 3 high)
  if( scoreboard[fieldID].Score1() != dmdScore1 )
  {
	dmd.clearScreen( true );
	DMDDisplayScoreLarge(scoreboard[fieldID].Score1(), DMD_SCORE1_X, DMD_SCORE1_Y);
	dmdScore1 = scoreboard[fieldID].Score1();
  }
#endif	
}

#if (DMD_ROLE == 0)
// ---------------------------------------------------------------------------------
// Display the score up to 99 in small font (works for panels 1 high x 1 across)
void DMDDisplayScoreSmall(uint8_t score, uint8_t xpos, uint8_t ypos)
{  
  // Set the score between 0 and 99
  score = constrain(score, MIN_SCORE, 99);

  if (score > 9)
    DMDDrawNumeral_14x6(xpos, ypos, (uint8_t) score / 10);                                      // tens digit
  DMDDrawNumeral_14x6(xpos + DMD_NUMERAL_14x6_WIDTH + DMD_NUMERAL_14x6_GAP, ypos, (uint8_t) score % 10);   // ones digit 
}

// ---------------------------------------------------------------------------------
void DMDDrawNumeral_14x6(uint8_t xpos, uint8_t ypos, uint8_t num)
{
	for (uint8_t y = 0; y < DMD_NUMERAL_14x6_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < DMD_NUMERAL_14x6_WIDTH; x++)
		{
			if ( (DMD_NUMERAL_14x6[num][y] & (1 << x)) )
			dmd.writePixel( x + xpos, y + ypos, GRAPHICS_NORMAL, true);
		}
	}
}
#endif

#if (DMD_ROLE == 2) || (DMD_ROLE == 4)
// ---------------------------------------------------------------------------------
// Display the score up to 199 in large font (designed for panels 3 high x 2 across)
void DMDDisplayScoreLarge(uint8_t score, uint8_t xpos, uint8_t ypos)
{
	// Set the score between 0 and 199
	score = constrain(score, MIN_SCORE, 199);
/*
	if (score > 99)
	{				
		// display 3 digits in large/narrow font
		DMDDrawNumeral_44x24(xpos, ypos, 1);							   // draw a 1  
		xpos += DMD_NUMERAL_44x24_WIDTH_ONE + DMD_NUMERAL_44x24_GAP;		
		DMDDrawNumeral_44x24(xpos, ypos, (uint8_t) (score - 100) / 10);   // tens digit
		xpos += DMD_NUMERAL_44x24_WIDTH + DMD_NUMERAL_44x28_GAP;
		DMDDrawNumeral_44x24(xpos, ypos, (uint8_t) (score - 100) % 10);   // ones digit
	}
	else
	{
		
		// display 1 or two digits in large/normal width font
		if (score > 9)
		{
			DMDDrawNumeral_44x28(xpos, ypos, (uint8_t) score / 10);         // tens digit
			xpos += DMD_NUMERAL_44x28_WIDTH + DMD_NUMERAL_44x28_GAP;			
		}
		else 
			xpos += (DMD_NUMERAL_44x28_WIDTH + DMD_NUMERAL_44x28_GAP)/2;	// centre a single digit
		
		DMDDrawNumeral_44x28(xpos, ypos, (uint8_t) score % 10);				// ones digit
	//}
}
/*
// ---------------------------------------------------------------------------------
void DMDDrawNumeral_44x24(uint8_t xpos, uint8_t ypos, uint8_t num)
{
	for (uint8_t y = 0; y < DMD_NUMERAL_44x24_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < DMD_NUMERAL_44x24_WIDTH; x++)
		{
			if ( (DMD_NUMERAL_44x24[num][y] & (1 << x)) )
			dmd.writePixel( x + xpos, y + ypos, GRAPHICS_NORMAL, true);
		}
	}
}

// ---------------------------------------------------------------------------------
void DMDDrawNumeral_44x28(uint8_t xpos, uint8_t ypos, uint8_t num)
{
	for (uint8_t y = 0; y < DMD_NUMERAL_44x28_HEIGHT; y++)
	{
		for (uint8_t x = 0; x < DMD_NUMERAL_44x28_WIDTH; x++)
		{
			if ( (DMD_NUMERAL_44x28[num][y] & (1 << x)) )
				dmd.writePixel( x + xpos, y + ypos, GRAPHICS_NORMAL, true);
		}
	}
}
#endif
*/

#endif
