/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardPowerClass.h"

#ifndef HW_ESP8266

// ---------------------------------------------------------------------------------
/*
void ScoreboardPowerClass::ScoreboardPowerClass( uint8_t ledPin, uint8_t sensePin )
{
	_ledpin = ledPin;
	_sensepin = sensePin;
	_powerstate = POWER_NORMAL;
	_batterystate = BATTERY_OK;
}
*/
// ---------------------------------------------------------------------------------
void ScoreboardPowerClass::Start()
{
	//Activity();

	// initialize digital pins
	//pinMode(_ledpin, OUTPUT);
}
// ---------------------------------------------------------------------------------
void ScoreboardPowerClass::Process()
{
	// toggle the LED

}
// ---------------------------------------------------------------------------------
void ScoreboardPowerClass::Update()
{
	time_t idle;

	// is the battery OK?
	



	// has there been recent activity?
	idle = now() - _lastactivity;

	if ( idle > POWER_ACTIVITY_TIMEOUT)
	{
		// Enter a low power state 
		_powerstate = POWER_LOW;
	}
	else if (idle > POWER_BACKLIGHT_TIMEOUT)
	{
		// Enter a low power state 
		_powerstate = POWER_LOW;
	}
	else
		_powerstate = POWER_NORMAL;

}
// ---------------------------------------------------------------------------------
void ScoreboardPowerClass::Activity()
{
	// pat the dog 
	_lastactivity = now();
}

#endif
