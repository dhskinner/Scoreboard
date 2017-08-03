/*
 Author:		Dave Skinner
 Description:	Hotspot controller for scoreboard using Adafruit HUZZAH ESP8266
*/
/*
There is a tri-color LED attached to the IOC expander, programmed as follows:
	RED Long Flash - once per second - means low power
	RED Short Flash - once per second - means battery is > 20% but controller out of range of scoreboard
	GREEN Short Flash - once per second - means battery is > 20% and in range of scoreboard 
	GREEN on button press - sending message
	BLUE - wifi activity

	Battery is rechargeable using a standard USB Type B Plug / charging takes a couple of hours
*/
// ---------------------------------------------------------------------------------
#define SCOREBOARD_HOTSPOT_ESP								// Set the scoreboard mode - refer to ScoreboardConfigs.h for options
#define SCOREBOARD_HOTSPOT_RF24_ADDRESS		3				// Set the address - scorebord master is 1, ulcd is 2, wifi is 3 etc 

#ifndef ESP8266
#error Remember to set the board to Adafruit HUZZAH ESP8266	// Just a reminder
#endif
// ---------------------------------------------------------------------------------

#include <Arduino.h>
#include <Scoreboard.h>

// ---------------------------------------------------------------------------------
// Globals
ScoreboardRadioClass radio(SCOREBOARD_HOTSPOT_RF24_ADDRESS, RF24_CE, RF24_CS);
ScoreboardWebClass web(80);
bool batteryLedIsOn = false;
unsigned long updateMillis = 0;

// ---------------------------------------------------------------------------------
void setup(void)
{
	// Start serial
	UBEGIN(115200);
	delay(100);
	UPRINTLN(F("\n\rStarting scoreboard hotspot"));

	// Start the scoreboard objects with their field ID
	for (uint8_t id = 0; id < FIELDS; id++)
		scoreboard[id].ID(id);

	// Deselect SPI devices by setting their chipselect lines HIGH
	// (otherwise the SPI devices will interfere with each other)
	WSUtility::DeselectSPI(spi, sizeof(spi));

	// Start the LiPo battery fuel gauge
	i2cbattery.Start();

	// Start the I2C IO handlers
	i2cio.ShortPressButton(I2C_BUTTON_LEFT_UP, SetScore1, 1);
	i2cio.ShortPressButton(I2C_BUTTON_LEFT_DN, SetScore1, -1);
	i2cio.ShortPressButton(I2C_BUTTON_RIGHT_UP, SetScore2, 1);
	i2cio.ShortPressButton(I2C_BUTTON_RIGHT_DN, SetScore2, -1);
	i2cio.LongPressButton(I2C_BUTTON_LEFT_UP, SetScore1, 5);
	i2cio.LongPressButton(I2C_BUTTON_LEFT_DN, SetScore1, -5);
	i2cio.LongPressButton(I2C_BUTTON_RIGHT_UP, SetScore2, 5);
	i2cio.LongPressButton(I2C_BUTTON_RIGHT_DN, SetScore2, -5);
	i2cio.Start();

	// Start the radio (defaults to offline mode if there is an error on startup).
	radio.Start();
	radio.On();

	// Try requesting the current scoreboard state
	radio.Send(GET_STATE, RF24_SCOREBOARD_ADDRESS, DEFAULT_FIELD);
	delay(200);
	radio.Process();

	// Start the web server
	web.Start(LED_PIN, WEB_ACCESS_POINT);

	UPRINTLN(F("Startup complete"));
}
// ---------------------------------------------------------------------------------
// Increment Score1 for Field 1
void SetScore1(int increment)
{
	SetScore(SCORE1, increment);
}
// ---------------------------------------------------------------------------------
// Increment Score2 for Field 1
void SetScore2(int increment)
{
	SetScore(SCORE2, increment);
}
// ---------------------------------------------------------------------------------
// Set the score 
void SetScore(int scoreid, int increment)
{
	USTREAM(F("Button set:\tfield=") << DEFAULT_FIELD << F(" Score=") << scoreid << F(" increment=") << increment << endl);
	scoreboard[DEFAULT_FIELD].ScoreIncrement(scoreid, increment);
	if (radio.SignalQuality() != 0)	// discard buttonpress if the radio is out of range, or has just started up
		radio.Send(SET_SCORE, RF24_SCOREBOARD_ADDRESS, DEFAULT_FIELD);
}
// ---------------------------------------------------------------------------------
void loop()
{
	//ArduinoOTA.handle();
	i2cio.Process();
	radio.Process();
	web.Process();

	if (millis() > updateMillis)
	{
		// Is web connectivity established?
		if (!web.Connected())
			web.ConnectWifi();

		// Is the battery OK?
		i2cbattery.Print();
		if (i2cbattery.IsLowPower())
		{
			// if less than 7%, go to sleep
			if (i2cbattery.Charge() < 7.0)
				sleep();
			else
			{
				// if less than 20%, flash red
				if (batteryLedIsOn)
					i2cio.LedOn(I2C_LED_RED);
				else
					i2cio.LedOff(I2C_LED_RED);
				batteryLedIsOn = !batteryLedIsOn;
			}
		}
		else
		{
			// Are we in range of the scoreboard?
			if (radio.SignalQuality() == 0)
				i2cio.LedBlinkOnce(I2C_LED_RED, I2C_LED_BLINK_MILLIS);
		}
		updateMillis += 1000;
	}
	yield();
}

void sleep()
{
	i2cio.LedOff(I2C_LED_RED);
	i2cio.LedOff(I2C_LED_GREEN);
	i2cio.LedOff(I2C_LED_BLUE);
	i2cbattery.Sleep();
	radio.Sleep();
	ESP.deepSleep(999999999 * 999999999U, WAKE_NO_RFCAL);
}
