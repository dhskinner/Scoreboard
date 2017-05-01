/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef __WS_SCOREBOARD_H__
#define __WS_SCOREBOARD_H__

#include <Arduino.h>
#include <ScoreboardDefines.h>

// ---------------------------------------------------------------------------------
// Role specific configs set up in the .ino file:
//		SCOREBOARD_MASTER
//		SCOREBOARD_PANEL DMD_LEFT_PANEL  (left hand side - 2 panels wide x 4 high)
//		SCOREBOARD_PANEL DMD_RIGHT_PANEL (right hand side - 2 panels wide x 4 high)
//		SCOREBOARD_REPEATER
//		SCOREBOARD_CONTROLLER or
//		SCOREBOARD_HOTSPOT_ESP
//		SCOREBOARD_HOTSPOT_YUN
// plus test configs:
//		SCOREBOARD_TEST_PANEL		(test panel that shows all scores and timers etc)
//
// Hardware = HW_UNO, HW_UNO_SHIELD, HW_NANO, HW_MEGA, HW_ETHERMEGA, HW_YUN, HW_ESP8266
// ---------------------------------------------------------------------------------
#ifdef SCOREBOARD_MASTER

// Hardware type
#define HW_NANO

// Startup delay when the master is firing up
#define MASTER_STARTUP_DELAY	3000

// RF24 address
#define RF24_ADDRESS RF24_SCOREBOARD_ADDRESS

#elif defined (SCOREBOARD_PANEL)

// Hardware type
#define HW_NANO

// DMD config
#define DMD_DISPLAYS_ACROSS		2
#define DMD_DISPLAYS_DOWN		4

// DMD role
#if ( SCOREBOARD_PANEL == DMD_LEFT_PANEL )
#define DMD_ROLE			DMD_LEFT_PANEL
#define I2C_ADDRESS			I2C_ADDRESS_DMD_1
#elif ( SCOREBOARD_PANEL == DMD_RIGHT_PANEL )
#define DMD_ROLE			DMD_RIGHT_PANEL
#define I2C_ADDRESS			I2C_ADDRESS_DMD_2
#endif

#elif defined (SCOREBOARD_REPEATER)

// Hardware type
#define HW_REPEATER

// DMD config
#define DMD_ROLE				DMD_REPEATER
#define DMD_DISPLAYS_ACROSS		1
#define DMD_DISPLAYS_DOWN		1

// RF24 address
#define RF24_ADDRESS			4

#elif defined (SCOREBOARD_TEST_PANEL)

// Hardware type
#define HW_UNO_SHIELD

// DMD config
#define DMD_ROLE				DMD_TEST_PANEL
#define DMD_DISPLAYS_ACROSS		1
#define DMD_DISPLAYS_DOWN		1

// I2C address
#define I2C_ADDRESS			I2C_ADDRESS_DMD_0

#elif defined (SCOREBOARD_CONTROLLER)

// Hardware type
#define HW_NANO

// RF24 address
#define RF24_ADDRESS			2

#elif defined (SCOREBOARD_HOTSPOT_ESP)

// Hardware type
#define HW_ESP8266

// RF24 address
#define RF24_ADDRESS			3

#elif defined (SCOREBOARD_HOTSPOT_YUN)

// Hardware type
#define HW_YUN

// RF24 address
#define RF24_ADDRESS			4

#elif defined (SCOREBOARD_LIBRARY)
	// Hardware type
	#define HW_NANO
#else
#error Scoreboard mode not defined
#endif

// ---------------------------------------------------------------------------------
// Hardware configs
// ---------------------------------------------------------------------------------

// What platform is this?
#if defined(ESP8266)
#define HW_PLATFORM_ESP8266
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined (__AVR_ATmega328P__) || defined  (__AVR_ATmega32U4__)
#define HW_PLATFORM_ARDUINO
#else
#error Platform not defined
#endif

// This is a very hacky hack to solve a problem with Visual Micro
#ifndef ESP8266_ESP12
#define ESP8266_ESP12	"ESP8266_ESP12"
#else
#error ESP8266_ESP12 is defined! Maybe visual micro has been fixed - try deleting this
#endif

// ---------------------------------------------------------------------------------

#if defined(HW_UNO_SHIELD)				// This is the yellow shield in the test panel (hand soldered on a freetronics prototype board)
const uint8_t RF24_CE = 14;				// equals analog A0
const uint8_t RF24_CS = 15;				// equals analog A1
const uint8_t LED_DATA = 4;				// 7 Segment LED data pin
const uint8_t LED_CLK = 2;				// 7 Segment LED clock pin
const uint8_t LED_CS = 3;				// 7 Segment LED chipselect pin
const uint8_t spi[] = { 9, 10, 14, 15, 3 };	// SPI CS pins

#elif defined(HW_UNO)
const uint8_t RF24_CE = 9;
const uint8_t RF24_CS = 10;
const uint8_t spi[] = { 9, 10 };		// SPI pins

#elif defined(HW_NANO)
#define ULCD_SERIAL_PORT		0		// Hardware serial port used for ULCD comms (must be 0 to 3)
#define ULCD_SERIAL_BAUD		(uint16_t) 115200	// Hardware serial baud rate for ULCD comms 
#define ULCD_RESET_INVERT		false	// Invert the signal if you are not using an an Arduino Adaptor Shield Version 2
#define ULCD_RESET_LINE			2		// change this if you are not using an Arduino Adaptor Shield Version 2
const uint8_t RF24_CE = 9;
const uint8_t RF24_CS = 10;
const uint8_t spi[] = { 9, 10 };		// SPI pins

#elif defined(HW_REPEATER)
#define ULCD_SERIAL_PORT		0		// Hardware serial port used for ULCD comms (must be 0 to 3)
#define ULCD_SERIAL_BAUD		(uint16_t) 115200	// Hardware serial baud rate for ULCD comms 
#define ULCD_RESET_INVERT		false	// Invert the signal if you are not using an an Arduino Adaptor Shield Version 2
#define ULCD_RESET_LINE			2		// change this if you are not using an Arduino Adaptor Shield Version 2
const uint8_t RF24_CE = 17;				// Pin Analog A3
const uint8_t RF24_CS = 16;				// Pin Analog A2
const uint8_t spi[] = { 9, 10, 16, 17 };	// SPI pins

#elif defined(HW_MEGA)
#define ULCD_SERIAL_PORT		1		// Hardware serial port used for ULCD comms (must be 0 to 3)
#define ULCD_SERIAL_BAUD		(uint16_t) 115200	// Hardware serial baud rate for ULCD comms 
#define ULCD_RESET_INVERT		false	// Invert the signal if you are not using an an Arduino Adaptor Shield Version 2
#define ULCD_RESET_LINE			4		// change this if you are not using an Arduino Adaptor Shield Version 2
const uint8_t RF24_CE = 9;
const uint8_t RF24_CS = 10;
const uint8_t spi[] = { 9, 10 };		// SPI pins

#elif defined(HW_ETHERMEGA)
const uint8_t RF24_CE = 40;
const uint8_t RF24_CS = 42;
const uint8_t spi[] = { 4,				// micro SD
                        6,				// bank0
                        7,				// bank1
                        10,				// wiznet
                        53				// atmega2560 SS line
                      };

#elif defined(HW_YUN)
const uint8_t RF24_CE = 9;
const uint8_t RF24_CS = 10;
const uint8_t spi[] = { 9, 10 };		// SPI pins

#elif defined(HW_ESP8266)
const uint8_t LED_PIN = 0;
const uint8_t RF24_CE = 16;
const uint8_t RF24_CS = 15;
const uint8_t spi[] = { 15, 16 };		// SPI pins

#else
#error No compatible hardware defined
#endif

// ---------------------------------------------------------------------------------
// Arduino core libraries
#include <SPI.h>				// Arduino core library
#include <Streaming.h>			// Arduino core library 
// ---------------------------------------------------------------------------------
// Third party libraries
#include <TimeLib.h>			// Download from https://github.com/PaulStoffregen/Time
#include <RHReliableDatagram.h>	// Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RH_NRF24.h>			// Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#include <ArduinoJson.h>		// Download from https://github.com/bblanchon/ArduinoJson
#include <DMD2.h>				// Download from https://github.com/freetronics/DMD2
#include <EasyTransferI2C.h>	// Download from https://github.com/madsci1016/Arduino-EasyTransfer
#include <DS1307RTC.h>			// Download from https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
#include <LiFuelGauge.h>		// Download from https://github.com/pAIgn10/LiFuelGauge
#include <LedControlMS.h>		// Download from http://www.instructables.com/id/LED-Matrix-with-Arduino/step2/Arduino-Library/
#include <genieArduino.h>		// Download from https://github.com/4dsystems/ViSi-Genie-Arduino-Library
#include <ArduinoJson.h>		// Download from https://github.com/bblanchon/ArduinoJson
#include <MemoryFree.h>			// Download from https://github.com/maniacbug/MemoryFree

// ---------------------------------------------------------------------------------
// Font files
#include <Font_SystemFont5x7.h>
#include <Font_NUMERAL44x28.h>
#include <Font_NUMERAL44x24.h>
#include <Font_NUMERAL16x7.h>	
#include <Font_FONT14.h>
#include <Font_FONT14x6.h>
// ---------------------------------------------------------------------------------
// Libraries and classes for specific platforms
#if defined ( HW_YUN )
#include <Bridge.h>				// Arduino core library for Yun
#include <BridgeServer.h>		// Arduino core library for Yun
#include <BridgeClient.h>		// Arduino core library for Yun

#elif defined (ESP8266)
#include <ESP8266WiFi.h>		// Download from https://github.com/esp8266/Arduino
#include <Ticker.h>				// Download from https://github.com/esp8266/Arduino
#include <pcf8574_esp.h>		// Download from https://github.com/WereCatf/PCF8574_ESP 
#include <functional>
/*
#include <WiFiClient.h>			// Download from https://github.com/esp8266/Arduino
#include <DNSServer.h>			// Download from https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>		// Download from https://github.com/esp8266/Arduino
#include <ESP8266WebServer.h>	// Download from https://github.com/esp8266/Arduino
#include <WiFiUdp.h>			// Download from https://github.com/esp8266/Arduino
#include <ArduinoOTA.h>			// Download from https://github.com/esp8266/Arduino
#include <WiFiManager.h>        // Download from https://github.com/tzapu/WiFiManager
#include <pcf8574_esp.h>		// Download from https://github.com/WereCatf/PCF8574_ESP
*/
#endif

// ---------------------------------------------------------------------------------
// Scoreboard class libraries
#include <WSUtility.h>
#include <ScoreboardClass.h>
#include <ScoreboardButtonClass.h>
#include <ScoreboardDMD2Class.h>
#include <ScoreboardI2CClass.h>
#include <ScoreboardIOClass.h>
#include <ScoreboardLEDClass.h>
#include <ScoreboardPowerClass.h>
#include <ScoreboardRadioClass.h>
#include <ScoreboardULCDClass.h>
#include <ScoreboardWebContent.h>
#include <ScoreboardWebClass.h>

// ---------------------------------------------------------------------------------

// Scorebord global objects (no initialisation is needed)
ScoreboardClass scoreboard[FIELDS];
ScoreboardI2CEEPROMClass i2ceeprom;
ScoreboardI2CTransferClass i2ctransfer;
ScoreboardI2CBatteryClass i2cbattery;
ScoreboardI2CClockClass i2cclock;
ScoreboardIOLEDClass onboardLed;

#ifdef ESP8266
	ScoreboardI2CIOClass i2cio;
#endif

#ifndef ESP8266
	ScoreboardULCDClass ulcd;
#endif
						
// ---------------------------------------------------------------------------------
#endif // __SCOREBOARD_H__
