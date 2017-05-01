#ifndef __DEFINES_H__
#define __DEFINES_H__

// ---------------------------------------------------------------------------------
// General definitions
// ---------------------------------------------------------------------------------

typedef enum {roleController = 1, roleScoreboardMaster, roleScoreboardSlave_Single, roleScoreboardSlave_LeftLabel, roleScoreboardSlave_LeftNumeral, roleScoreboardSlave_RightLabel, roleScoreboardSlave_RightNumeral} Role;
typedef enum {MESSAGE_NULL = 0, BROADCAST, GET_STATE, SET_SCORE, SET_TIMER, SET_MODE_FLAGS} MessageID;

#define MIN_SCORE               0
#define MAX_SCORE               199
#define MAX_MATCHTIME			5999		// max matchtime in seconds = 5999 = 99 minutes and 99 secs       

#define FIELD1					0
#define FIELD2					1

#define FLAG_CLOCK_OFF			B00000000	// zero flag used for logic only / not transmitted
#define FLAG_TIMER_STOP			B00000000	// zero flag used for logic only / not transmitted
#define FLAG_TIMER_PAUSE        B00000001
#define FLAG_TIMER_RUN          B00000010
#define FLAG_CLOCK_ON           B00000100
#define FLAG_DUAL_FIELD_ON      B00001000
#define FLAG_SWAP_FIELD_ON      B00010000
#define FLAG_BRIGHTNESS_0       B00100000
#define FLAG_BRIGHTNESS_1       B01000000
#define FLAG_BRIGHTNESS_2       B01100000
#define FLAG_BRIGHTNESS_3       B10000000
#define FLAG_BRIGHTNESS_4       B10100000
#define FLAG_BRIGHTNESS_5       B11000000
#define FLAG_BRIGHTNESS_6       B11100000

// ---------------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------------

#ifdef DEBUG_VERBOSE
  #define DEBUG
#endif

#ifdef TEST
  #define DEBUG_
  #define DEBUG_VERBOSE_
#endif

// ---------------------------------------------------------------------------------
// RF24 and SPI Definitions
// ---------------------------------------------------------------------------------

#define RF24_SCOREBOARD_ADDRESS  1    // RF24 address for the scoreboard master 
#define RF24_CONTROLLER_ADDRESS  2    // RF24 address for the controller 
#define RF24_LISTEN_TIMEOUT      200  // timeout in milliseconds (mainly effects the controller)
#define RF24_CHANNEL             110  // channel 1 to 128 - check which channel is clear before selecting
#define RF24_RETRIES             5    // number of retries per transmission
#define RF24_HEADER_BYTE         0x01 // header byte 0 - ASCII SOH (start of header)
#define RF24_HEADER_SIZE         2    // number of bytes in the RF24 header

// ---------------------------------------------------------------------------------
// LED Definitions
// ---------------------------------------------------------------------------------

#define LED_DIGITS              8
#define LED_MIN_DIGIT           0
#define LED_MAX_DIGIT           10
#define LED_SCORE0_INDEX        6
#define LED_SCORE1_INDEX        0

// ---------------------------------------------------------------------------------
// DMD Definitions
// ---------------------------------------------------------------------------------
#define DMD_PERIOD              4000 //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.

// Position the two scores (top left corner X, Y)
/*
#define DMD_SCORE0_X            0
#define DMD_SCORE0_Y            1
#define DMD_SCORE1_X            18
#define DMD_SCORE1_Y            1
*/

#define DMD_DUALSCORE_MIN		0
#define DMD_DUALSCORE_MAX		99

// REMEMBER!!! Font14 only implements upper case letters
#define DMD_TEAM0_NAME			F("WYNNUM")
#define DMD_TEAM1_NAME			F("GUESTS")
#define DMD_TEAM0_SHORTNAME		F("HOME")
#define DMD_TEAM1_SHORTNAME		F("AWAY")
#define DMD_FIELD1_NAME			"FIELD1"
#define DMD_FIELD2_NAME			"FIELD2"
#define DMD_FIELD1_NAME_LENGTH	6
#define DMD_FIELD2_NAME_LENGTH	6
// REMEMBER!!! Font14 only implements upper case letters

// ---------------------------------------------------------------------------------
// ULCD Definitions
// ---------------------------------------------------------------------------------

#define ULCD_SPLASH_TIMEOUT    3000          // timeout to return from the splash screen (msecs)
#define ULCD_FORM_TIMEOUT      60000         // timeout to return to the previous main form (msecs)
#define ULCD_BUTTON_DOWN       1
#define ULCD_BUTTON_UP         0

#define ULCD_ENABLE            1             // helpers
#define ULCD_DISABLE           0

typedef enum { ULCD_MESSAGE_TX_ERROR = 0, ULCD_MESSAGE_CONNECTING } ULCDMessage;
typedef enum { ULCD_DIALOG_RESET_SCOREBOARD = 0, ULCD_DIALOG_RESET_TIMER } ULCDDialog;
	
#define ULCD_STRING_CLOCK		0
#define ULCD_STRING_STOP		1
#define ULCD_STRING_PAUSE		2
#define ULCD_STRING_RUN			3

#define ULCD_SPINNER_TIMING		200
#define ULCD_SPINNER_IMAGES		14
const uint8_t ULCD_SPINNER_IMAGE[] = { 7, 8, 9, 10, 11, 12, 13, 0, 1, 2, 3, 4, 5, 6};

// ---------------------------------------------------------------------------------
// These index numbers are for the corresponding objects in the uLCD Genie interface
// FORMS
#define ULCD_MAIN_FORM 0
#define ULCD_FIELD1_SCORE0_FORM 1
#define ULCD_FIELD1_TIMER_FORM 2
#define ULCD_FIELD2_SCORE0_FORM 3
#define ULCD_SPLASH_FORM 4
#define ULCD_FIELD1_FORM 5
#define ULCD_FIELD2_FORM 6
#define ULCD_MESSAGE_FORM 7
#define ULCD_DIALOG_FORM 8
#define ULCD_FIELD2_TIMER_FORM 10
#define ULCD_SETTINGS_FORM 11
#define ULCD_WORKING_FORM 12
#define ULCD_FIELD1_SCORE1_FORM 13
#define ULCD_FIELD2_SCORE1_FORM 14
#define ULCD_BLANK_FORM 15
// MAIN FORM
#define ULCD_MAIN_FIELD1_BTN 33,3
#define ULCD_MAIN_FIELD2_BTN 33,5
#define ULCD_MAIN_SET_BTN 33,4
#define ULCD_MAIN_BATTERY 11,4
#define ULCD_MAIN_SIGNAL 27,0
#define ULCD_MAIN_TIMER 23,0
#define ULCD_MAIN_ONLINE_STRING 17,15
// SETTINGS FORM
#define ULCD_SET_DUALFIELD_4DBTN 30,0
#define ULCD_SET_SWAPFIELD_4DBTN 30,2
#define ULCD_SET_BRIGHTNESS_BAR 5,1
#define ULCD_SET_BACK_BTN 33,52
// FIELD1 - SCORE0
#define ULCD_FIELD1_SCORE0_SET_LED 15,3
#define ULCD_FIELD1_SCORE0_MINUS1_BTN 33,22
#define ULCD_FIELD1_SCORE0_MINUS5_BTN 33,21
#define ULCD_FIELD1_SCORE0_PLUS5_BTN 33,24
#define ULCD_FIELD1_SCORE0_PLUS1_BTN 33,25
#define ULCD_FIELD1_SCORE0_BACK_BTN 33,23
#define ULCD_FIELD1_SCORE0_SET_BTN 33,26
// FIELD1 - SCORE1
#define ULCD_FIELD1_SCORE1_SET_LED 15,0
#define ULCD_FIELD1_SCORE1_MINUS1_BTN 33,13
#define ULCD_FIELD1_SCORE1_MINUS5_BTN 33,14
#define ULCD_FIELD1_SCORE1_PLUS5_BTN 33,34
#define ULCD_FIELD1_SCORE1_PLUS1_BTN 33,35
#define ULCD_FIELD1_SCORE1_BACK_BTN 33,12
#define ULCD_FIELD1_SCORE1_SET_BTN 33,11
// FIELD2 - SCORE0
#define ULCD_FIELD2_SCORE0_SET_LED 15,6
#define ULCD_FIELD2_SCORE0_MINUS1_BTN 33,16
#define ULCD_FIELD2_SCORE0_MINUS5_BTN 33,15
#define ULCD_FIELD2_SCORE0_PLUS5_BTN 33,18
#define ULCD_FIELD2_SCORE0_PLUS1_BTN 33,19
#define ULCD_FIELD2_SCORE0_BACK_BTN 33,17
#define ULCD_FIELD2_SCORE0_SET_BTN 33,20
// FIELD2 - SCORE1
#define ULCD_FIELD2_SCORE1_SET_LED 15,1
#define ULCD_FIELD2_SCORE1_MINUS1_BTN 33,37
#define ULCD_FIELD2_SCORE1_MINUS5_BTN 33,36
#define ULCD_FIELD2_SCORE1_PLUS5_BTN 33,40
#define ULCD_FIELD2_SCORE1_PLUS1_BTN 33,39
#define ULCD_FIELD2_SCORE1_BACK_BTN 33,38
#define ULCD_FIELD2_SCORE1_SET_BTN 33,41
// FIELD1 - TIMER
#define ULCD_FIELD1_TIMER_SET_LED 15,4
#define ULCD_FIELD1_TIMER_SET_STRING 17,2
#define ULCD_FIELD1_TIMER_STOP_BTN 33,1
#define ULCD_FIELD1_TIMER_PAUSE_BTN 33,0
#define ULCD_FIELD1_TIMER_START_BTN 33,2
#define ULCD_FIELD1_TIMER_BACK_BTN 33,6
#define ULCD_FIELD1_TIMER_CLOCK_BTN 33,10
// FIELD2 - TIMER
#define ULCD_FIELD2_TIMER_SET_LED 15,11
#define ULCD_FIELD2_TIMER_SET_STRING 17,13
#define ULCD_FIELD2_TIMER_STOP_BTN 33,47
#define ULCD_FIELD2_TIMER_PAUSE_BTN 33,48
#define ULCD_FIELD2_TIMER_START_BTN 33,49
#define ULCD_FIELD2_TIMER_BACK_BTN 33,51
#define ULCD_FIELD2_TIMER_CLOCK_BTN 33,50
// WORKING
#define ULCD_WORKING_IMAGE 27,0
// SPLASH
#define ULCD_SPLASH_SKIP_BTN 33,7
#define ULCD_SPLASH_OFFLINE_BTN 33,8
//FIELD1
#define ULCD_FIELD1_SCORE0_LED 15,5
#define ULCD_FIELD1_SCORE1_LED 15,8
#define ULCD_FIELD1_TIMER_LED 15,7
#define ULCD_FIELD1_TIMER_STRNG 17,5
#define ULCD_FIELD1_SCORE0_BTN 33,31
#define ULCD_FIELD1_SCORE1_BTN 33,29
#define ULCD_FIELD1_TIMER_BTN 33,30
#define ULCD_FIELD1_BACK_BTN 33,32
#define ULCD_FIELD1_CLEAR_BTN 33,33
#define ULCD_FIELD1_BATTERY 11,0
#define ULCD_FIELD1_SIGNAL 27,2
//FIELD2
#define ULCD_FIELD2_SCORE0_LED 15,2
#define ULCD_FIELD2_SCORE1_LED 15,10
#define ULCD_FIELD2_TIMER_LED 15,9
#define ULCD_FIELD2_TIMER_STRNG 17,7
#define ULCD_FIELD2_SCORE0_BTN 33,42
#define ULCD_FIELD2_SCORE1_BTN 33,46
#define ULCD_FIELD2_TIMER_BTN 33,43
#define ULCD_FIELD2_BACK_BTN 33,45
#define ULCD_FIELD2_CLEAR_BTN 33,44
#define ULCD_FIELD2_BATTERY 11,1
#define ULCD_FIELD2_SIGNAL 27,3
// MESSAGE
#define ULCD_MESSAGE_STRING 17,3
#define ULCD_MESSAGE_OK_BTN 33,27
// DIALOG
#define ULCD_DIALOG_STRING 17,4
#define ULCD_DIALOG_YES_BTN 33,28
#define ULCD_DIALOG_NO_BTN 33,9

// ---------------------------------------------------------------------------------
// These index numbers are same as above but just for the buttons
#define INPUT_SET_DUALFIELD_4DBTN 0
#define INPUT_SET_SWAPFIELD_4DBTN 2
#define INPUT_SET_BRIGHTNESS_BAR 1
#define INPUT_SET_BACK_BTN 52
#define INPUT_MAIN_FIELD1_BTN 3
#define INPUT_MAIN_FIELD2_BTN 5
#define INPUT_MAIN_SET_BTN 4
#define INPUT_FIELD1_SCORE0_MINUS1_BTN 22
#define INPUT_FIELD1_SCORE0_MINUS5_BTN 21
#define INPUT_FIELD1_SCORE0_PLUS5_BTN 24
#define INPUT_FIELD1_SCORE0_PLUS1_BTN 25
#define INPUT_FIELD1_SCORE0_BACK_BTN 23
#define INPUT_FIELD1_SCORE0_SET_BTN 26
#define INPUT_FIELD1_SCORE1_MINUS1_BTN 13
#define INPUT_FIELD1_SCORE1_MINUS5_BTN 14
#define INPUT_FIELD1_SCORE1_PLUS5_BTN 34
#define INPUT_FIELD1_SCORE1_PLUS1_BTN 35
#define INPUT_FIELD1_SCORE1_BACK_BTN 12
#define INPUT_FIELD1_SCORE1_SET_BTN 11
#define INPUT_FIELD2_SCORE0_MINUS1_BTN 16
#define INPUT_FIELD2_SCORE0_MINUS5_BTN 15
#define INPUT_FIELD2_SCORE0_PLUS5_BTN 18
#define INPUT_FIELD2_SCORE0_PLUS1_BTN 19
#define INPUT_FIELD2_SCORE0_BACK_BTN 17
#define INPUT_FIELD2_SCORE0_SET_BTN 20
#define INPUT_FIELD2_SCORE1_MINUS1_BTN 37
#define INPUT_FIELD2_SCORE1_MINUS5_BTN 36
#define INPUT_FIELD2_SCORE1_PLUS5_BTN 40
#define INPUT_FIELD2_SCORE1_PLUS1_BTN 39
#define INPUT_FIELD2_SCORE1_BACK_BTN 38
#define INPUT_FIELD2_SCORE1_SET_BTN 41
#define INPUT_FIELD1_TIMER_STOP_BTN 1
#define INPUT_FIELD1_TIMER_PAUSE_BTN 0
#define INPUT_FIELD1_TIMER_START_BTN 2
#define INPUT_FIELD1_TIMER_BACK_BTN 6
#define INPUT_FIELD1_TIMER_CLOCK_BTN 10
#define INPUT_FIELD2_TIMER_STOP_BTN 47
#define INPUT_FIELD2_TIMER_PAUSE_BTN 48
#define INPUT_FIELD2_TIMER_START_BTN 49
#define INPUT_FIELD2_TIMER_BACK_BTN 51
#define INPUT_FIELD2_TIMER_CLOCK_BTN 50
#define INPUT_SPLASH_SKIP_BTN 7
#define INPUT_SPLASH_OFFLINE_BTN 8
#define INPUT_FIELD1_SCORE0_BTN 31
#define INPUT_FIELD1_SCORE1_BTN 29
#define INPUT_FIELD1_TIMER_BTN 30
#define INPUT_FIELD1_BACK_BTN 32
#define INPUT_FIELD1_CLEAR_BTN 33
#define INPUT_FIELD2_SCORE0_BTN 42
#define INPUT_FIELD2_SCORE1_BTN 46
#define INPUT_FIELD2_TIMER_BTN 43
#define INPUT_FIELD2_BACK_BTN 45
#define INPUT_FIELD2_CLEAR_BTN 44
#define INPUT_MESSAGE_OK_BTN 27
#define INPUT_DIALOG_YES_BTN 28
#define INPUT_DIALOG_NO_BTN 9

// ---------------------------------------------------------------------------------
#endif
