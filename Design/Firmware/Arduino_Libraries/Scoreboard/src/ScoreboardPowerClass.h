/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _IOCLASS_h
#define _IOCLASS_h

#ifndef HW_ESP8266

#include <Arduino.h>
#include <Time.h>
#include <ScoreboardDefines.h>
#include <ScoreboardIOClass.h>
#include <WSUtility.h>

// handle:
// - battery monitoring (IO and I2C)
// - last activity monitoring
// - turn off brightness and LED's
// - flash the power LED if its low
// ADD that shakey thing

class ScoreboardPowerClass
{
protected:
	PowerState _powerstate;
	BatteryState _batterystate;

	uint8_t _sensepin;
	time_t _lastactivity;

public:
//	ScoreboardPowerClass( uint8_t ledPin = POWER_LED_PIN, uint8_t sensePin = POWER_SENSE_PIN, );
	void Start();
	void Process();
	void Activity();
	void Update();
};

extern ScoreboardPowerClass IOPower ;

#endif

#endif

