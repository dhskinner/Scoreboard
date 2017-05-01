/*
 * Test routines
 */
#ifdef TEST

void TestStart()
{
  // Set a time
  setTime ( 9,15,0,24,2,1972 );
  
  // Swap the fields 
  //scoreboard[FIELD1].SetFlag ( FLAG_SWAP_FIELD_ON );
  
  // Dual field mode
  //scoreboard[FIELD1].SetFlag ( FLAG_DUAL_FIELD_ON );
  
  // Clock mode
  //scoreboard[FIELD1].SetFlag ( FLAG_CLOCK_ON );
  //scoreboard[FIELD2].SetFlag ( FLAG_CLOCK_ON );
  
  // Timer mode
  //scoreboard[FIELD1].Run();
  //scoreboard[FIELD2].Run();
  
  // Brightness
  scoreboard[FIELD1].SetFlag( FLAG_BRIGHTNESS_1 );
  scoreboard[FIELD2].SetFlag( FLAG_BRIGHTNESS_1 );
}

void TestProcess()
{
  // Increment the scores
  //scoreboard[FIELD1].Score0 ( scoreboard[FIELD1].Score0() < MAX_SCORE ? scoreboard[FIELD1].Score0() + 1 : 0 );
  //scoreboard[FIELD1].Score1 ( scoreboard[FIELD1].Score1() < MAX_SCORE ? scoreboard[FIELD1].Score1() + 1 : 0 );
  //scoreboard[FIELD2].Score0 ( scoreboard[FIELD2].Score0() < MAX_SCORE ? scoreboard[FIELD2].Score0() + 1 : 0 );
  //scoreboard[FIELD2].Score1 ( scoreboard[FIELD2].Score1() < MAX_SCORE ? scoreboard[FIELD2].Score1() + 1 : 0 );
  
  //Brightness
  scoreboard[FIELD1].Brightness( scoreboard[FIELD1].Brightness() < 6 ? scoreboard[FIELD1].Brightness() + 1 : 0 );
  scoreboard[FIELD2].Brightness( scoreboard[FIELD2].Brightness() < 6 ? scoreboard[FIELD2].Brightness() + 1 : 0 );

/*
  uint8_t b = scoreboard[FIELD1].Brightness();
  // Scale from 0 = zero brightness to six = 255
  int t = b * 43;
  t = constrain ( t, 0, 255 );
*/
  	
  //result = RFSend ( SET_SCORE, RF24_SCOREBOARD_ADDRESS, FIELD1 );
  //result = RFSend ( SET_SCORE, RF24_SCOREBOARD_ADDRESS, FIELD2 );

  result = RFSend ( SET_MODE_FLAGS, RF24_SCOREBOARD_ADDRESS, FIELD1 );
  result = RFSend ( SET_MODE_FLAGS, RF24_SCOREBOARD_ADDRESS, FIELD2 );
  
  //Broadcast the new scores
  //I2C_Broadcast ( FIELD1 );
  //I2C_Broadcast ( FIELD2 );
}
#endif
