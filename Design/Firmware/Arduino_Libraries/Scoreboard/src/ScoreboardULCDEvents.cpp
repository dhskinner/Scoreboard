/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardULCDClass.h"

#ifndef HW_ESP8266

// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::Process()
{
    genie.DoEvents(); // This calls the library each loop to process the queued responses from the display

    if ( ulcdCurrentForm == ULCD_WORKING_FORM )
        this->UpdateSpinner();
}
// ---------------------------------------------------------------------------------
// Get/Set the last event time;
long ScoreboardULCDClass::LastEvent()
{
	return ulcdLastEventTime;
}
long ScoreboardULCDClass::LastEvent(long eventtime)
{
	ulcdLastEventTime = eventtime;
	return ulcdLastEventTime;
}
// ---------------------------------------------------------------------------------
// Process form change events
void ScoreboardULCDClass::ProcessForm ( genieFrame *event )
{
    ulcdCurrentForm = ( uint8_t ) event->reportObject.index;
}
// ---------------------------------------------------------------------------------
// Process button presses
void ScoreboardULCDClass::ProcessInput ( genieFrame *event )
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
            this->ChangeForm ( ULCD_MAIN_FORM );
            break;

        // Field 1
        case INPUT_MAIN_FIELD1_BTN:
        case INPUT_FIELD1_SCORE0_BACK_BTN:
        case INPUT_FIELD1_SCORE1_BACK_BTN:
        case INPUT_FIELD1_TIMER_BACK_BTN:
			this->ChangeForm ( ULCD_FIELD1_FORM );
            break;

        // Field 2
        case INPUT_MAIN_FIELD2_BTN:
        case INPUT_FIELD2_SCORE0_BACK_BTN:
        case INPUT_FIELD2_SCORE1_BACK_BTN:
        case INPUT_FIELD2_TIMER_BACK_BTN:
			this->ChangeForm ( ULCD_FIELD2_FORM );
            break;

        // Settings
        case INPUT_MAIN_SET_BTN:
			this->ChangeForm ( ULCD_SETTINGS_FORM );
            break;

        // ---------- Prepare to set the Scores ----------

        case INPUT_FIELD1_SCORE0_BTN:
            ulcdTempScore = scoreboard[FIELD1].ScoreID ( 0 );
            genie.WriteObject ( ULCD_FIELD1_SCORE0_SET_LED, ulcdTempScore );
			this->ChangeForm ( ULCD_FIELD1_SCORE0_FORM );
            break;

        case INPUT_FIELD1_SCORE1_BTN:
            ulcdTempScore = scoreboard[FIELD1].ScoreID ( 1 );
            genie.WriteObject ( ULCD_FIELD1_SCORE1_SET_LED, ulcdTempScore );
			this->ChangeForm ( ULCD_FIELD1_SCORE1_FORM );
            break;

        case INPUT_FIELD2_SCORE0_BTN:
            ulcdTempScore = scoreboard[FIELD2].ScoreID ( 0 );
            genie.WriteObject ( ULCD_FIELD2_SCORE0_SET_LED, ulcdTempScore );
			this->ChangeForm ( ULCD_FIELD2_SCORE0_FORM );
            break;

        case INPUT_FIELD2_SCORE1_BTN:
            ulcdTempScore = scoreboard[FIELD2].ScoreID ( 1 );
            genie.WriteObject ( ULCD_FIELD2_SCORE1_SET_LED, ulcdTempScore );
			this->ChangeForm ( ULCD_FIELD2_SCORE1_FORM );
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
			this->SetScores ( FIELD1, ulcdTempScore, scoreboard[FIELD1].ScoreID ( 1 ), ULCD_FIELD1_SCORE0_FORM );
            break;

        case INPUT_FIELD1_SCORE1_SET_BTN:
			this->SetScores ( FIELD1, scoreboard[FIELD1].ScoreID ( 0 ), ulcdTempScore, ULCD_FIELD1_SCORE1_FORM );
            break;

        case INPUT_FIELD2_SCORE0_SET_BTN:
			this->SetScores ( FIELD2, ulcdTempScore, scoreboard[FIELD2].ScoreID ( 1 ), ULCD_FIELD2_SCORE0_FORM );
            break;

        case INPUT_FIELD2_SCORE1_SET_BTN:
			this->SetScores ( FIELD2, scoreboard[FIELD2].ScoreID ( 0 ), ulcdTempScore, ULCD_FIELD2_SCORE1_FORM );
            break;

        // ---------- Set the Timers ----------

        case INPUT_FIELD1_TIMER_BTN:
			this->ChangeForm ( ULCD_FIELD1_TIMER_FORM );
            break;

        case INPUT_FIELD1_TIMER_STOP_BTN:
            if ( scoreboard[FIELD1].MatchTimeMode() != FLAG_TIMER_STOP )
				this->ShowDialog ( ULCD_DIALOG_RESET_TIMER, ULCD_FIELD1_TIMER_FORM );
            break;

        case INPUT_FIELD1_TIMER_PAUSE_BTN:
			this->SetTimer ( FIELD1, FLAG_TIMER_PAUSE, false, ULCD_FIELD1_TIMER_FORM );
            break;

        case INPUT_FIELD1_TIMER_START_BTN:
			this->SetTimer ( FIELD1, FLAG_TIMER_RUN, false, ULCD_FIELD1_TIMER_FORM );
            break;

        case INPUT_FIELD1_TIMER_CLOCK_BTN:
			this->SetTimer ( FIELD1, scoreboard[FIELD1].MatchTimeMode(), ( event->reportObject.data_lsb == ULCD_BUTTON_DOWN ? true : false ), ULCD_FIELD1_TIMER_FORM );
            break;

        // ----------

        case INPUT_FIELD2_TIMER_BTN:
			this->ChangeForm ( ULCD_FIELD2_TIMER_FORM );
            break;

        case INPUT_FIELD2_TIMER_STOP_BTN:
            if ( scoreboard[FIELD2].MatchTime() != FLAG_TIMER_STOP );
			this->ShowDialog ( ULCD_DIALOG_RESET_TIMER, ULCD_FIELD2_TIMER_FORM );
            break;

        case INPUT_FIELD2_TIMER_PAUSE_BTN:
			this->SetTimer ( FIELD2, FLAG_TIMER_PAUSE, false, ULCD_FIELD2_TIMER_FORM );
            break;

        case INPUT_FIELD2_TIMER_START_BTN:
			this->SetTimer ( FIELD2, FLAG_TIMER_RUN, false, ULCD_FIELD2_TIMER_FORM );
            break;

        case INPUT_FIELD2_TIMER_CLOCK_BTN:
			this->SetTimer ( FIELD2, scoreboard[FIELD2].MatchTimeMode(), ( event->reportObject.data_lsb == ULCD_BUTTON_DOWN ? FLAG_CLOCK_ON : FLAG_CLOCK_OFF ), ULCD_FIELD2_TIMER_FORM );
            break;

        // ---------- Clear a scoreboard ----------

        case INPUT_FIELD1_CLEAR_BTN:
			this->ShowDialog ( ULCD_DIALOG_RESET_SCOREBOARD, ULCD_FIELD1_FORM );
            break;

        case INPUT_FIELD2_CLEAR_BTN:
			this->ShowDialog ( ULCD_DIALOG_RESET_SCOREBOARD, ULCD_FIELD2_FORM );
            break;

        // ---------- Miscellaneous Buttons ----------
        case INPUT_SPLASH_OFFLINE_BTN:
			radio.Off();
            genie.WriteObject ( ULCD_MAIN_ONLINE_STRING, 1 );
			this->ChangeForm ( ULCD_MAIN_FORM );
            break;

        case INPUT_MESSAGE_OK_BTN:
			this->ChangeForm ( ulcdReturnToForm );
            break;

        case INPUT_DIALOG_YES_BTN:
			this->ProcessDialogYes();
            break;

        case INPUT_DIALOG_NO_BTN:
            ChangeForm ( ulcdReturnToForm );
            break;

        default:
            break;
        }
        break;

    case GENIE_OBJ_4DBUTTON:
        switch ( event->reportObject.index )
        {
        case INPUT_SET_DUALFIELD_4DBTN:
			this->DualField ( FIELD1, ( event->reportObject.data_lsb == 1 ? true : false ), ULCD_MAIN_FORM );
            break;

        case INPUT_SET_SWAPFIELD_4DBTN:
			this->SwapField ( FIELD1, ( event->reportObject.data_lsb == 1 ? true : false ), ULCD_MAIN_FORM );
            break;

        default:
            break;
        }
        break;

    case GENIE_OBJ_TRACKBAR:
        switch ( event->reportObject.index )
        {
        case INPUT_SET_BRIGHTNESS_BAR:
			this->SetBrightness ( FIELD1, event->reportObject.data_lsb, ULCD_MAIN_FORM );
			this->SetBrightness ( FIELD2, event->reportObject.data_lsb, ULCD_MAIN_FORM );
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
void ScoreboardULCDClass::ChangeForm ( uint8_t newForm )
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
void ScoreboardULCDClass::UpdateSpinner()
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
void ScoreboardULCDClass::ProcessDialogYes()
{
    switch ( ulcdReturnToForm )
    {
    case ULCD_FIELD1_FORM:
		this->ChangeForm ( ULCD_WORKING_FORM );
		this->SetScores ( FIELD1, 0, 0, ULCD_FIELD1_FORM );
		this->SetTimer ( FIELD1, FLAG_TIMER_STOP, FLAG_CLOCK_OFF, ULCD_FIELD1_FORM );
		this->ChangeForm ( ULCD_FIELD1_FORM );
        break;

    case ULCD_FIELD2_FORM:
		this->ChangeForm ( ULCD_WORKING_FORM );
		this->SetScores ( FIELD2, 0, 0, ULCD_FIELD2_FORM );
		this->SetTimer ( FIELD2, FLAG_TIMER_STOP, false, ULCD_FIELD2_FORM );
		this->ChangeForm ( ULCD_FIELD2_FORM );
        break;

    case ULCD_FIELD1_TIMER_FORM:
		this->ChangeForm ( ULCD_WORKING_FORM );
		this->SetTimer ( FIELD1, FLAG_TIMER_STOP, FLAG_CLOCK_OFF, ULCD_FIELD1_FORM );
		this->ChangeForm ( ULCD_FIELD1_FORM );
        break;

    case ULCD_FIELD2_TIMER_FORM:
		this->ChangeForm ( ULCD_WORKING_FORM );
		this->SetTimer ( FIELD2, FLAG_TIMER_STOP, FLAG_CLOCK_OFF, ULCD_FIELD2_FORM );
		this->ChangeForm ( ULCD_FIELD2_FORM );
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
void ScoreboardULCDClass::ShowMessage ( ULCDMessage message, uint8_t form )
{
    ulcdReturnToForm = form;
    genie.WriteObject ( ULCD_MESSAGE_STRING, message );
	this->ChangeForm ( ULCD_MESSAGE_FORM );
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::ShowDialog ( ULCDDialog dialog, uint8_t form )
{
    ulcdReturnToForm = form;
    genie.WriteObject ( ULCD_DIALOG_STRING, dialog );
	this->ChangeForm ( ULCD_DIALOG_FORM );
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::SetScores ( uint8_t fieldID, uint8_t score1, uint8_t score2, uint8_t form )
{
    // Set the 'working' page
	this->ChangeForm ( ULCD_WORKING_FORM );

    // Check if the score has changed
    if ( score1 != scoreboard[fieldID].Score1 ( ) || score2 != scoreboard[fieldID].Score2 ( ) )
    {
        // Attempt to transmit to the main scoreboard
        scoreboard[fieldID].Score ( score1, score2 );						// set the score
        if ( radio.Send ( SET_SCORE, RF24_SCOREBOARD_ADDRESS, fieldID ) )	// transmit it until received or timout (blocking)
        {
			this->UpdateField ( fieldID, false );
			this->ChangeForm ( ulcdTimeoutForm );							// tx success - change the form back to the previous page (which should be the timeoutform)
        }
        else
			this->ShowMessage ( ULCD_MESSAGE_TX_ERROR, form );				// tx failed - show an error message
    }

    // Otherwise just flip back to the previous page
	this->ChangeForm ( ulcdTimeoutForm );
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::SetTimer ( uint8_t fieldID, uint8_t mode, bool clock, uint8_t form )
{
    // check if the state is different to the scoreboard object state
    if ( mode != scoreboard[fieldID].MatchTimeMode() || clock != scoreboard[fieldID].Clock() )
    {
        // set the scoreboard clock and timer - note that the state logic is all in the scoreboard object
		scoreboard[fieldID].Clock(clock);

        scoreboard[fieldID].MatchTimeMode ( mode );
		this->UpdateTimer ( fieldID, true );								 // refresh (but stay on the timer page)

        // Attempt to transmit to the main scoreboard
        if ( !radio.Send ( SET_TIMER, RF24_SCOREBOARD_ADDRESS, fieldID ) )
            this->ShowMessage ( ULCD_MESSAGE_TX_ERROR, form );             // tx failed - show an error message
    }
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::SetBrightness ( uint8_t fieldID, uint8_t brightness, uint8_t form )
{
    // set the brightness
    scoreboard[fieldID].Brightness ( brightness );

    // Attempt to transmit to the main scoreboard
    if ( !radio.Send ( SET_MODE, RF24_SCOREBOARD_ADDRESS, fieldID ) )
		this->ShowMessage ( ULCD_MESSAGE_TX_ERROR, form );             // tx failed - show an error message
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::SwapField ( uint8_t fieldID, bool swapField, uint8_t form )
{
    // set the swap field flag
    scoreboard[fieldID].Swap ( swapField );

    // Attempt to transmit to the main scoreboard
    if ( !radio.Send ( SET_MODE, RF24_SCOREBOARD_ADDRESS, fieldID ) )
		this->ShowMessage ( ULCD_MESSAGE_TX_ERROR, form );             // tx failed - show an error message
}
// ---------------------------------------------------------------------------------
void ScoreboardULCDClass::DualField ( uint8_t fieldID, bool dualField, uint8_t form )
{
    // set the dual screen flag
    scoreboard[fieldID].Dual ( dualField );

    // Attempt to transmit to the main scoreboard
    if ( !radio.Send ( SET_MODE, RF24_SCOREBOARD_ADDRESS, fieldID ) )
		this->ShowMessage ( ULCD_MESSAGE_TX_ERROR, form );             // tx failed - show an error message
}
// ---------------------------------------------------------------------------------

#endif

