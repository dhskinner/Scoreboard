#ifdef ULCD

// ---------------------------------------------------------------------------------
void ULCD_Process()
{
  genie.DoEvents(); // This calls the library each loop to process the queued responses from the display

  if ( ulcdCurrentForm == ULCD_WORKING_FORM )
    ULCDUpdateSpinner();
}
// ---------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////
//
// This is the user's event handler. It is called by genieDoEvents()
// when the following conditions are true
//
//    The link is in an IDLE state, and
//    There is an event to handle
//
// The event can be either a REPORT_EVENT frame sent asynchronously
// from the display or a REPORT_OBJ frame sent by the display in
// response to a READ_OBJ request.
//
void ULCDGenieEventHandler ( void )
{
  genieFrame event;
  genie.DequeueEvent ( &event );
  ulcdLastEventTime = millis();

  // Process events
  switch ( event.reportObject.object )
  {
  case GENIE_OBJ_FORM:              // Process form changes
    ULCDProcessForm ( &event );
    break;

  case GENIE_OBJ_USERBUTTON:
  case GENIE_OBJ_WINBUTTON:        //  Process button presses
  case GENIE_OBJ_4DBUTTON:
  case GENIE_OBJ_ANIBUTTON:
  case GENIE_OBJ_TRACKBAR:
    ULCDProcessInput ( &event );
    break;

  default:
    break;
  }
}
// ---------------------------------------------------------------------------------
// Process form change events
void ULCDProcessForm ( genieFrame *event )
{
  ulcdCurrentForm = ( uint8_t ) event->reportObject.index;
}
// ---------------------------------------------------------------------------------
// Process button presses
void ULCDProcessInput ( genieFrame *event )
{
  switch ( event->reportObject.object )
  {
  case GENIE_OBJ_USERBUTTON:
    switch ( event->reportObject.index )
    {
    // ---------- Navigate Between Forms ----------

    // Main
    case INPUT_SPLASH_SKIP_BTN:
    case INPUT_FIELD1_BACK_BTN:
    case INPUT_FIELD2_BACK_BTN:
    case INPUT_SET_BACK_BTN:
      ULCDChangeForm ( ULCD_MAIN_FORM );
      break;

    // Field 1
    case INPUT_MAIN_FIELD1_BTN:
    case INPUT_FIELD1_SCORE0_BACK_BTN:
    case INPUT_FIELD1_SCORE1_BACK_BTN:
    case INPUT_FIELD1_TIMER_BACK_BTN:
      ULCDChangeForm ( ULCD_FIELD1_FORM );
      break;

    // Field 2
    case INPUT_MAIN_FIELD2_BTN:
    case INPUT_FIELD2_SCORE0_BACK_BTN:
    case INPUT_FIELD2_SCORE1_BACK_BTN:
    case INPUT_FIELD2_TIMER_BACK_BTN:
      ULCDChangeForm ( ULCD_FIELD2_FORM );
      break;

    // Settings
    case INPUT_MAIN_SET_BTN:
      ULCDChangeForm ( ULCD_SETTINGS_FORM );
      break;

    // ---------- Prepare to set the Scores ----------

    case INPUT_FIELD1_SCORE0_BTN:
      ulcdTempScore = scoreboard[FIELD1].ScoreID ( 0 );
      genie.WriteObject ( ULCD_FIELD1_SCORE0_SET_LED, ulcdTempScore );
      ULCDChangeForm ( ULCD_FIELD1_SCORE0_FORM );
      break;

    case INPUT_FIELD1_SCORE1_BTN:
      ulcdTempScore = scoreboard[FIELD1].ScoreID ( 1 );
      genie.WriteObject ( ULCD_FIELD1_SCORE1_SET_LED, ulcdTempScore );
      ULCDChangeForm ( ULCD_FIELD1_SCORE1_FORM );
      break;

    case INPUT_FIELD2_SCORE0_BTN:
      ulcdTempScore = scoreboard[FIELD2].ScoreID ( 0 );
      genie.WriteObject ( ULCD_FIELD2_SCORE0_SET_LED, ulcdTempScore );
      ULCDChangeForm ( ULCD_FIELD2_SCORE0_FORM );
      break;

    case INPUT_FIELD2_SCORE1_BTN:
      ulcdTempScore = scoreboard[FIELD2].ScoreID ( 1 );
      genie.WriteObject ( ULCD_FIELD2_SCORE1_SET_LED, ulcdTempScore );
      ULCDChangeForm ( ULCD_FIELD2_SCORE1_FORM );
      break;

    // ---------- Increment or decrement the scores ----------

    case INPUT_FIELD1_SCORE0_MINUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE0_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD1_SCORE0_MINUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE0_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD1_SCORE0_PLUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE0_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD1_SCORE0_PLUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE0_SET_LED, ulcdTempScore );
      break;

    // ----------

    case INPUT_FIELD1_SCORE1_MINUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE1_SET_LED, ulcdTempScore );
      break;
    case INPUT_FIELD1_SCORE1_MINUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE1_SET_LED, ulcdTempScore );
      break;
    case INPUT_FIELD1_SCORE1_PLUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE1_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD1_SCORE1_PLUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD1_SCORE1_SET_LED, ulcdTempScore );
      break;

    // ----------

    case INPUT_FIELD2_SCORE0_MINUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE0_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD2_SCORE0_MINUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE0_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD2_SCORE0_PLUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE0_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD2_SCORE0_PLUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE0_SET_LED, ulcdTempScore );
      break;

    // ----------

    case INPUT_FIELD2_SCORE1_MINUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE1_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD2_SCORE1_MINUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore - 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE1_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD2_SCORE1_PLUS5_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 5, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE1_SET_LED, ulcdTempScore );
      break;

    case INPUT_FIELD2_SCORE1_PLUS1_BTN:
      ulcdTempScore = constrain ( ulcdTempScore + 1, MIN_SCORE, MAX_SCORE );
      genie.WriteObject ( ULCD_FIELD2_SCORE1_SET_LED, ulcdTempScore );
      break;

    // ---------- Set the scores ----------
    case INPUT_FIELD1_SCORE0_SET_BTN:
      ULCDSetScores ( FIELD1, ulcdTempScore, scoreboard[FIELD1].ScoreID ( 1 ), ULCD_FIELD1_SCORE0_FORM );
      break;

    case INPUT_FIELD1_SCORE1_SET_BTN:
      ULCDSetScores ( FIELD1, scoreboard[FIELD1].ScoreID ( 0 ), ulcdTempScore, ULCD_FIELD1_SCORE1_FORM );
      break;

    case INPUT_FIELD2_SCORE0_SET_BTN:
      ULCDSetScores ( FIELD2, ulcdTempScore, scoreboard[FIELD2].ScoreID ( 1 ), ULCD_FIELD2_SCORE0_FORM );
      break;

    case INPUT_FIELD2_SCORE1_SET_BTN:
      ULCDSetScores ( FIELD2, scoreboard[FIELD2].ScoreID ( 0 ), ulcdTempScore, ULCD_FIELD2_SCORE1_FORM );
      break;

    // ---------- Set the Timers ----------

    case INPUT_FIELD1_TIMER_BTN:
      ULCDChangeForm ( ULCD_FIELD1_TIMER_FORM );
      break;

    case INPUT_FIELD1_TIMER_STOP_BTN:
      if ( scoreboard[FIELD1].MatchTimeMode() != FLAG_TIMER_STOP )
        ULCDShowDialog ( ULCD_DIALOG_RESET_TIMER, ULCD_FIELD1_TIMER_FORM );
      break;

    case INPUT_FIELD1_TIMER_PAUSE_BTN:
      ULCDSetTimer ( FIELD1, FLAG_TIMER_PAUSE, false, ULCD_FIELD1_TIMER_FORM );
      break;

    case INPUT_FIELD1_TIMER_START_BTN:
      ULCDSetTimer ( FIELD1, FLAG_TIMER_RUN, false, ULCD_FIELD1_TIMER_FORM );
      break;

    case INPUT_FIELD1_TIMER_CLOCK_BTN:
      ULCDSetTimer ( FIELD1, scoreboard[FIELD1].MatchTimeMode(), ( event->reportObject.data_lsb == ULCD_BUTTON_DOWN ? true : false ), ULCD_FIELD1_TIMER_FORM );
      break;

    // ----------

    case INPUT_FIELD2_TIMER_BTN:
      ULCDChangeForm ( ULCD_FIELD2_TIMER_FORM );
      break;

    case INPUT_FIELD2_TIMER_STOP_BTN:
      if ( scoreboard[FIELD2].MatchTime() != FLAG_TIMER_STOP );
      ULCDShowDialog ( ULCD_DIALOG_RESET_TIMER , ULCD_FIELD2_TIMER_FORM );
      break;

    case INPUT_FIELD2_TIMER_PAUSE_BTN:
      ULCDSetTimer ( FIELD2, FLAG_TIMER_PAUSE, false, ULCD_FIELD2_TIMER_FORM );
      break;

    case INPUT_FIELD2_TIMER_START_BTN:
      ULCDSetTimer ( FIELD2, FLAG_TIMER_RUN, false, ULCD_FIELD2_TIMER_FORM );
      break;

    case INPUT_FIELD2_TIMER_CLOCK_BTN:
      ULCDSetTimer ( FIELD2, scoreboard[FIELD2].MatchTimeMode(), ( event->reportObject.data_lsb == ULCD_BUTTON_DOWN ? FLAG_CLOCK_ON : FLAG_CLOCK_OFF ), ULCD_FIELD2_TIMER_FORM );
      break;

    // ---------- Clear a scoreboard ----------

    case INPUT_FIELD1_CLEAR_BTN:
      ULCDShowDialog ( ULCD_DIALOG_RESET_SCOREBOARD , ULCD_FIELD1_FORM );
      break;

    case INPUT_FIELD2_CLEAR_BTN:
      ULCDShowDialog ( ULCD_DIALOG_RESET_SCOREBOARD , ULCD_FIELD2_FORM );
      break;

    // ---------- Miscellaneous Buttons ----------
    case INPUT_SPLASH_OFFLINE_BTN:
      offlineFlag = true;
      genie.WriteObject ( ULCD_MAIN_ONLINE_STRING, 1 );
      ULCDChangeForm ( ULCD_MAIN_FORM );
      break;

    case INPUT_MESSAGE_OK_BTN:
      ULCDChangeForm ( ulcdReturnToForm );
      break;

    case INPUT_DIALOG_YES_BTN:
      ULCDProcessDialogYes();
      break;

    case INPUT_DIALOG_NO_BTN:
      ULCDChangeForm ( ulcdReturnToForm );
      break;

    default:
      break;
    }
    break;

  case GENIE_OBJ_4DBUTTON:
    switch ( event->reportObject.index )
    {
    case INPUT_SET_DUALFIELD_4DBTN:
      ULCDDualField ( FIELD1, ( event->reportObject.data_lsb == 1 ? true : false ) , ULCD_MAIN_FORM );
      break;

    case INPUT_SET_SWAPFIELD_4DBTN:
      ULCDSwapField ( FIELD1, ( event->reportObject.data_lsb == 1 ? true : false ) , ULCD_MAIN_FORM );
      break;

    default:
      break;
    }
    break;

  case GENIE_OBJ_TRACKBAR:
    switch ( event->reportObject.index )
    {
    case INPUT_SET_BRIGHTNESS_BAR:
      ULCDSetBrightness ( FIELD1, event->reportObject.data_lsb, ULCD_MAIN_FORM );
      ULCDSetBrightness ( FIELD2, event->reportObject.data_lsb, ULCD_MAIN_FORM );
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }
}
// ---------------------------------------------------------------------------------
void ULCDChangeForm ( uint8_t newForm )
{
  switch ( newForm )
  {
  case ULCD_WORKING_FORM:
    ulcdSpinnerIndex = 0;
    genie.WriteObject ( ULCD_WORKING_IMAGE, ULCD_SPINNER_IMAGE[ulcdSpinnerIndex] );
    break;

  case ULCD_MAIN_FORM:
  case ULCD_FIELD1_FORM:
  case ULCD_FIELD2_FORM:
    // If we are switching to one of the primary forms, store it so we can go back if there is a timeout
    ulcdTimeoutForm = newForm;

  default:
    break;
  }
  // change the form
  ulcdCurrentForm = newForm;
  ulcdLastEventTime = millis();
  genie.WriteObject ( GENIE_OBJ_FORM, newForm, 1 );
}
// ---------------------------------------------------------------------------------
void ULCDUpdateSpinner ()
{
  // how many increments elapsed since the last event?
  long  tempIndex = ( ( millis() - ulcdLastEventTime ) / ULCD_SPINNER_TIMING ) % ULCD_SPINNER_IMAGES;

  // Update the spinner on the working page
  if ( ulcdSpinnerIndex != tempIndex )
  {
    ulcdSpinnerIndex = tempIndex;
    genie.WriteObject ( ULCD_WORKING_IMAGE, ULCD_SPINNER_IMAGE[ulcdSpinnerIndex] );
  }
}
// ---------------------------------------------------------------------------------
void ULCDProcessDialogYes()
{
  switch ( ulcdReturnToForm )
  {
  case ULCD_FIELD1_FORM:
    ULCDChangeForm ( ULCD_WORKING_FORM );
    ULCDSetScores ( FIELD1, 0, 0, ULCD_FIELD1_FORM );
    ULCDSetTimer ( FIELD1, FLAG_TIMER_STOP, FLAG_CLOCK_OFF, ULCD_FIELD1_FORM );
    ULCDChangeForm ( ULCD_FIELD1_FORM );
    break;

  case ULCD_FIELD2_FORM:
    ULCDChangeForm ( ULCD_WORKING_FORM );
    ULCDSetScores ( FIELD2, 0, 0, ULCD_FIELD2_FORM );
    ULCDSetTimer ( FIELD2, FLAG_TIMER_STOP, false, ULCD_FIELD2_FORM );
    ULCDChangeForm ( ULCD_FIELD2_FORM );
    break;

  case ULCD_FIELD1_TIMER_FORM:
    ULCDChangeForm ( ULCD_WORKING_FORM );
    ULCDSetTimer ( FIELD1, FLAG_TIMER_STOP, FLAG_CLOCK_OFF, ULCD_FIELD1_FORM );
    ULCDChangeForm ( ULCD_FIELD1_FORM );
    break;

  case ULCD_FIELD2_TIMER_FORM:
    ULCDChangeForm ( ULCD_WORKING_FORM );
    ULCDSetTimer ( FIELD2, FLAG_TIMER_STOP, FLAG_CLOCK_OFF, ULCD_FIELD2_FORM );
    ULCDChangeForm ( ULCD_FIELD2_FORM );
    break;

  default:
    break;

    /*  // Kept for later on
        case ULCD_MAIN_FORM:
        case ULCD_WORKING_FORM:
        case ULCD_FIELD1_SCORE0_FORM:
        case ULCD_FIELD2_SCORE0_FORM:
        case ULCD_SPLASH_FORM:
        case ULCD_MESSAGE_FORM:
        case ULCD_DIALOG_FORM:
        case ULCD_TEXT_SET_FORM:
        case ULCD_TEXT_FORM:
        case ULCD_FIELD1_SCORE1_FORM:
        case ULCD_FIELD2_SCORE1_FORM:
        case ULCD_BLANK_FORM:
    */
  }
}
// ---------------------------------------------------------------------------------
void ULCDShowMessage ( ULCDMessage message , uint8_t form )
{
  ulcdReturnToForm = form;
  genie.WriteObject ( ULCD_MESSAGE_STRING, message );
  ULCDChangeForm ( ULCD_MESSAGE_FORM );
}
// ---------------------------------------------------------------------------------
void ULCDShowDialog ( ULCDDialog dialog , uint8_t form )
{
  ulcdReturnToForm = form;
  genie.WriteObject ( ULCD_DIALOG_STRING, dialog );
  ULCDChangeForm ( ULCD_DIALOG_FORM );
}
// ---------------------------------------------------------------------------------
void ULCDSetScores ( uint8_t fieldID, uint8_t score0, uint8_t score1, uint8_t form )
{
  // Set the 'working' page
  ULCDChangeForm ( ULCD_WORKING_FORM );

  // Check if the score has changed
  if ( score0 != scoreboard[fieldID].ScoreID ( 0 ) || score1 != scoreboard[fieldID].ScoreID ( 1 ) )
  {
    // Attempt to transmit to the main scoreboard
    scoreboard[fieldID].Score ( score0, score1 );                   // set the score
    if ( RFSend ( SET_SCORE, RF24_SCOREBOARD_ADDRESS, fieldID ) )   // transmit it until received or timout (blocking)
    {
      ULCDUpdateField ( fieldID , false );
      ULCDChangeForm ( ulcdTimeoutForm );                           // tx success - change the form back to the previous page (which should be the timeoutform)
    }
    else
      ULCDShowMessage ( ULCD_MESSAGE_TX_ERROR , form );             // tx failed - show an error message
  }

  // Otherwise just flip back to the previous page
  ULCDChangeForm ( ulcdTimeoutForm );
}
// ---------------------------------------------------------------------------------
void ULCDSetTimer ( uint8_t fieldID, uint8_t mode, boolean clock, uint8_t form )
{
  // check if the state is different to the scoreboard object state
  if ( mode != scoreboard[fieldID].MatchTimeMode() || clock != scoreboard[fieldID].IsClockOn() )
  {
    // set the scoreboard clock and timer - note that the state logic is all in the scoreboard object
    if ( clock == true )
      scoreboard[fieldID].SetFlag ( FLAG_CLOCK_ON );
    else
      scoreboard[fieldID].ClearFlag ( FLAG_CLOCK_ON );

    scoreboard[fieldID].MatchTimeMode ( mode );
    ULCDUpdateTimer ( fieldID, true );								 // refresh (but stay on the timer page)

    // Attempt to transmit to the main scoreboard
    if ( !RFSend ( SET_TIMER, RF24_SCOREBOARD_ADDRESS, fieldID ) )
      ULCDShowMessage ( ULCD_MESSAGE_TX_ERROR, form );                // tx failed - show an error message
  }
}
// ---------------------------------------------------------------------------------
void ULCDSetBrightness ( uint8_t fieldID, uint8_t brightness, uint8_t form )
{
  // set the brightness
  scoreboard[fieldID].Brightness ( brightness );

  // Attempt to transmit to the main scoreboard
  if ( !RFSend ( SET_MODE_FLAGS, RF24_SCOREBOARD_ADDRESS, fieldID ) )
    ULCDShowMessage ( ULCD_MESSAGE_TX_ERROR, form );                // tx failed - show an error message
}
// ---------------------------------------------------------------------------------
void ULCDSwapField ( uint8_t fieldID, boolean swapField, uint8_t form )
{
  // set the swap field flag
  if ( swapField )
    scoreboard[fieldID].SetFlag ( FLAG_SWAP_FIELD_ON );
  else
    scoreboard[fieldID].ClearFlag ( FLAG_SWAP_FIELD_ON );

  // Attempt to transmit to the main scoreboard
  if ( !RFSend ( SET_MODE_FLAGS, RF24_SCOREBOARD_ADDRESS, fieldID ) )
    ULCDShowMessage ( ULCD_MESSAGE_TX_ERROR, form );                // tx failed - show an error message
}
// ---------------------------------------------------------------------------------
void ULCDDualField ( uint8_t fieldID, boolean dualField, uint8_t form )
{
  // set the dual screen flag
  if ( dualField )
    scoreboard[fieldID].SetFlag ( FLAG_DUAL_FIELD_ON );
  else
    scoreboard[fieldID].ClearFlag ( FLAG_DUAL_FIELD_ON );

  // Attempt to transmit to the main scoreboard
  if ( !RFSend ( SET_MODE_FLAGS, RF24_SCOREBOARD_ADDRESS, fieldID ) )
    ULCDShowMessage ( ULCD_MESSAGE_TX_ERROR, form );                // tx failed - show an error message
}
// ---------------------------------------------------------------------------------
#endif

