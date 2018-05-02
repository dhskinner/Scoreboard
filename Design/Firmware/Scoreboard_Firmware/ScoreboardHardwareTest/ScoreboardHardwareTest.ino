/*
 Name:		ScoreboardHardwareTest.ino
 Created:	5/14/2017 10:17:09 PM
 Author:	Dave
*/

// ------------------------------------------------------------
// Systems and subsystems to be tested
#define TEST_WIFI_CONTROLLER		// TEST_WIFI_CONTROLLER, TEST_CONTROLLER, TEST_MASTER

#ifdef TEST_WIFI_CONTROLLER
//#define TEST_PCF8574_BUTTONS
#define TEST_PCF8574_BUTTONS_INTERRUPT
#define TEST_PCF8574_LED
#define TEST_FUELGAUGE
#define TEST_NRF24L01
#elif defined TEST_CONTROLLER
#error TODO
#elif defined TEST_MASTER
#define TEST_REALTIMECLOCK
#else
#define TEST_PCF8574
#define TEST_FUELGAUGE
#define TEST_NRF24L01
//#define TEST_REALTIMECLOCK
#endif

// General
#define TICKER_SECONDS			1.0F
#define I2C_PCF8574_ADDRESS		0x20		// IO expander address
#define I2C_PCF8574_INTERUPT	2			// IO expander interrupt pin
#define I2C_MAX17043_ADDRESS	0x36		// Lipo fuel gauge address
#define I2C_SCL_PIN				5
#define I2C_SDA_PIN				4
#define RF24_CE_PIN				16
#define RF24_CS_PIN				15
#define RF24_ADDRESS			1
#define RF24_LISTEN_TIMEOUT		200			// timeout in milliseconds (mainly effects the controller)
#define RF24_CHANNEL			110			// channel 1 to 128 - check which channel is clear before selecting
#define RF24_TX_POWER			RH_NRF24::TransmitPowerm6dBm     // TransmitPower0dBm 
#define RF24_DATA_RATE			RH_NRF24::DataRate250kbps
#define RF24_RETRIES			5			// number of retries per transmission
#define LED_RED_BIT				5
#define LED_BLUE_BIT			6			
#define LED_GREEN_BIT			7	
#define LED_ON					0
#define LED_OFF					1

// ------------------------------------------------------------
// Libraries
#include <Arduino.h>
#include <Streaming.h>
#include <SPI.h>
#include <Wire.h>
#include <LiFuelGauge.h>
#include <Ticker.h>
#include <TimeLib.h>
#include <RHReliableDatagram.h>   // Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RH_NRF24.h>             // Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#include <DS1307RTC.h>

#ifdef ESP8266
#include <Ticker.h>
#include <pcf8574_esp.h>
#endif

// ------------------------------------------------------------
// Globals
Ticker ticker;

void HandleTicker()
{
	ProcessFuelGauge();
	ProcessPCFTicker();
	ProcessRF24();
	ProcessRTC();
	//Serial.print(F("."));
}

void setup()
{
	// start serial
	Serial.begin(115200);
	while (!Serial);             // Leonardo: wait for serial monitor
	Serial.println(F("\n------------------------------\nScoreboard Hardware Test Utility"));
	delay(1000);

	// start I2C
	Wire.begin();
	ScanI2C();

	// start subsystems
	StartPCF();
	StartFuelGauge();
	StartRF24();
	StartRTC();

	// call the ticker every second
	ticker.attach(TICKER_SECONDS, HandleTicker);
}

void loop()
{
	ProcessPCF();
}


