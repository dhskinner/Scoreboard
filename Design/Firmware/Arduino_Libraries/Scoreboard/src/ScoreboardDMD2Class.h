/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _DMD2CLASS_h
#define _DMD2CLASS_h

#ifndef HW_ESP8266

#include <Arduino.h>
#include <SPI.h>		// Arduino core library
#include <Time.h>		// Arduino core library
#include <DMD2.h>		// Download from https://github.com/freetronics/DMD2

#include <ScoreboardDefines.h>
#include <ScoreboardClass.h>
#include <WSUtility.h>

#include <Font_NUMERAL16x7.h>
#include <Font_NUMERAL44x28.h>
#include <Font_NUMERAL44x24.h>
#include <Font_FONT14.h>
#include <Font_FONT14x6.h>
#include <Font_SystemFont5x7.h>

// External global objects - you need to define these in the main .ino file
extern ScoreboardClass scoreboard[FIELDS];

class ScoreboardDMD2Class
{
protected:
    // double buffering
    SPIDMD		_dmd;
    DMDFrame	_nextdmd;

    // Set these up with out-of-range values so they are refreshed first time round
    uint8_t		_dmdScore1 = 254;
    uint8_t		_dmdScore2 = 254;
    uint8_t		_dmdTimerMode = 254;
    boolean		_dmdDualFlag = false;
    boolean		_dmdSwapFlag = false;
    boolean		_dmdClockFlag = false;
    uint8_t		_dmdBrightness = 32;
    time_t		_dmdTime = 0;
    bool		_dmdStartup = true;
    DMDRoleID	_dmdRole = DMD_TEST;
    uint8_t		_dmdFieldID = FIELD1;

public:
    ScoreboardDMD2Class ( uint8_t display_across, uint8_t display_down, DMDRoleID role, uint8_t fieldID = DEFAULT_FIELD );
    void Start();
    void Test();
    void UpdatePanel();
    void UpdateRepeater ( DMDRepeaterMode mode );
    void UpdateTestPanel ( bool forceupdate = false );
    bool RedrawCheck();
    void RedrawPanelSingle();
    void RedrawPanelDual();
    void RedrawPanelClock();
    void RedrawRepeaterScore();
    void RedrawRepeaterTimer();
	void RedrawRepeaterRealTimeClock();
    uint8_t PrintDigits ( uint16_t val, char * buffer, uint8_t index );
	uint8_t Print2Digits ( uint16_t val, char * buffer, uint8_t index );
    uint8_t CopyToBuffer ( char const * text, char * buffer, uint8_t index, uint8_t len );
    void SetBrightness();
    void RandomPixels();
};

#endif

#endif

