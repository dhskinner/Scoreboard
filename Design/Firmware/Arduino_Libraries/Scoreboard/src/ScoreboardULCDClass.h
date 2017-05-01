/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _ULCDCLASS_h
#define _ULCDCLASS_h

#ifndef HW_ESP8266

#include <Arduino.h>
#include <HardwareSerial.h>
#include <genieArduino.h>         // Download from https://github.com/4dsystems/ViSi-Genie-Arduino-Library
#include <ScoreboardDefines.h>
#include <ScoreboardClass.h>
#include <ScoreboardRadioClass.h>
#include <ScoreboardIOClass.h>
#include <ScoreboardI2CClass.h>
#include <WSUtility.h>

// These functions communicate with a 4D Systems Display, configured with ViSi-Genie, utilising the Genie Arduino Library - https://github.com/4dsystems/ViSi-Genie-Arduino-Library.
// The program receives messages from the onscreen objects using the Reported Events. This is triggered each time an object changes on the display, and an event
// is generated and sent automatically. Reported Events originate from the On-Changed event from the object itself, set in the Workshop4 software.
// Application Notes on the 4D Systems Website that are useful to understand this library are found: http://www.4dsystems.com.au/appnotes
// Other good App Notes to read are:
// ViSi-Genie Connecting a 4D Display to an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00017/
// ViSi-Genie Writing to Genie Objects Using an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00018/
// ViSi-Genie A Simple Digital Voltmeter Application using an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00019/
// ViSi-Genie Connection to an Arduino Host with RGB LED Control - http://www.4dsystems.com.au/appnote/4D-AN-00010/
// ViSi-Genie Displaying Temperature values from an Arduino Host - http://www.4dsystems.com.au/appnote/4D-AN-00015/
// ViSi-Genie Arduino Danger Shield - http://www.4dsystems.com.au/appnote/4D-AN-00025/

// External global objects - you need to define these in the main .ino file
extern	ScoreboardClass scoreboard[FIELDS];
extern	ScoreboardRadioClass radio;
extern	Genie genie;

class ScoreboardULCDClass
{
protected:
    // Local display state - these store the state of the ulcd as its quicker then retrieving from serial
    uint8_t ulcdScore[FIELDS][2];		// current ulcd scores
    time_t ulcdTimer[FIELDS];			// current ulcd timer time
    bool ulcdClock[FIELDS];				// current ulcd clock state
    uint8_t ulcdMode[FIELDS];			// current ulcd mode
    uint8_t ulcdBattery;				// current battery state 0-100
    uint8_t ulcdRFSignal;				// current rf signal quality 0-5
    uint8_t ulcdTempScore;				// temp value used when setting the score
    uint8_t ulcdCurrentForm;			// currently displayed form
    uint8_t ulcdTimeoutForm;			// form to return to after a timeout
    uint8_t ulcdReturnToForm;			// previous form (used by dialogs and message boxes to return to previous)
	uint8_t ulcdContrast;				// current ulcd brightness 0=off 15=full brightness
	uint8_t ulcdBrightness;				// Control the scoreboard brightness
	uint8_t ulcdSpinnerIndex;			// Current image index for the "Working" page. 255 means it is disabled
    long    ulcdLastEventTime;			// time the last button press or page change occurred
    bool	ulcdSwapField;				// Flag to swap the fields
    bool	ulcdDualField;				// Flag to show dual fields
    bool	ulcdMute;					// boolean to toggle mute or unmute

public:
    void Start (uint8_t hwserial, uint16_t hwserialbaud, uint8_t resetline, bool resetlineinvert);
    void Update ( bool forceUpdate );
    void UpdateField ( uint8_t fieldID, bool forceUpdate );
    void UpdateRFSignal();
    void UpdateBattery();
    void UpdateMode();
    void UpdateTimer ( uint8_t fieldID, bool forceUpdate );
    void Process();
    void ProcessForm ( genieFrame *event );
    void ProcessInput ( genieFrame *event );
    void ChangeForm ( uint8_t newForm );
    void UpdateSpinner();
    void ProcessDialogYes();
    void ShowMessage ( ULCDMessage message, uint8_t form );
    void ShowDialog ( ULCDDialog dialog, uint8_t form );
    void SetScores ( uint8_t fieldID, uint8_t score1, uint8_t score2, uint8_t form );
    void SetTimer ( uint8_t fieldID, uint8_t mode, bool clock, uint8_t form );
    void SetBrightness ( uint8_t fieldID, uint8_t brightness, uint8_t form );
    void SwapField ( uint8_t fieldID, bool swapField, uint8_t form );
    void DualField ( uint8_t fieldID, bool dualField, uint8_t form );
    long LastEvent();
    long LastEvent ( long eventtime );
};

extern ScoreboardULCDClass ulcd;

#endif

#endif