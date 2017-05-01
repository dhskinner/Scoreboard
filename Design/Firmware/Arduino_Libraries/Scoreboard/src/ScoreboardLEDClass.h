/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _LEDCLASS_h
#define _LEDCLASS_h

#ifndef HW_ESP8266

#include <Arduino.h>
#include <LedControlMS.h>         // Download from http://www.instructables.com/id/LED-Matrix-with-Arduino/step2/Arduino-Library/
#include <ScoreboardDefines.h>
#include <ScoreboardClass.h>
#include <WSUtility.h>

// External global objects - you need to define these in the main .ino file
extern ScoreboardClass scoreboard[FIELDS];

class ScoreboardLEDClass
{
protected:
	LedControl _lc;		// MAX72XX SPI LED controller
	boolean _ledClock = false;
	uint8_t _fieldID = FIELD1;

public:
	ScoreboardLEDClass(uint8_t fieldID, int datapin, int clockpin, int chipselectpin);
    void Start();
	void Update();
	void DisplayTimeMMSS(time_t t, uint8_t index);
	void DisplayTimeHHMMSS(time_t t, uint8_t index);
	void DisplayScore(uint8_t score, uint8_t index);
	void Test();
	void ScrollDigits();
};

#endif

#endif

