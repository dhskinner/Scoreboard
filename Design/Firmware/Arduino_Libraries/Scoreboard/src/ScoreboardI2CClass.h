/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _I2CCLASS_h
#define _I2CCLASS_h

#include <Arduino.h>

#if defined (ESP8266)
#include <Ticker.h>
#include <functional>
#include <pcf8574_esp.h>			// Download from https://github.com/WereCatf/PCF8574_ESP 
#endif

#include <EasyTransferI2C.h>		// Download from https://github.com/madsci1016/Arduino-EasyTransfer
#include <DS1307RTC.h>				// Download from https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
#include <LiFuelGauge.h>			// Download from https://github.com/pAIgn10/LiFuelGauge
#include <ScoreboardDefines.h>
#include <ScoreboardButtonClass.h>
#include <ScoreboardClass.h>
#include <WSUtility.h>

// External global objects - you need to define these in the main .ino file
extern ScoreboardClass scoreboard[FIELDS];

class ScoreboardI2CTransferClass
{
protected:
    EasyTransferI2C _i2c_transfer;				// easyTransfer object
    ScoreboardDatagram _i2c_datagram;			// struct used to transfer data over I2C
public:
    void StartAsMaster();
    void StartAsSlave ( uint8_t slave_address );
    bool Process();
    void Broadcast ( );
    void Broadcast ( uint8_t fieldID );
    void Receive ( int numBytes );
};
// ---------------------------------------------------------------------------------
class ScoreboardI2CClockClass
{
protected:
    const char *_monthName[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    bool ParseDate ( const char *str, TimeElements *tm );
    bool ParseTime ( const char *str, TimeElements *tm );

public:
    void SetSystemTimefromRTC();
    void SetRTCfromCompilerTime();
};
// ---------------------------------------------------------------------------------
class ScoreboardI2CBatteryClass
{
protected:

public:
    static void Start();
    static bool Alert();
    static uint8_t Charge();
};
// ---------------------------------------------------------------------------------
#define FULL_MASK 0x7FFFF
#define DEVICE_MASK 0x7F0000
#define WORD_MASK 0xFFFF

// AT24C32 eeprom
class ScoreboardI2CEEPROMClass
{
public:
    ScoreboardI2CEEPROMClass();
    static void write ( unsigned long, uint8_t );
    static uint8_t read ( unsigned long );
};
// ---------------------------------------------------------------------------------
// PCF8574 IO
#ifdef ESP8266
class ScoreboardI2CIOClass
{
public:
    void Start();
    void Process();
    void ShortPressButton ( uint8_t pin, ButtonHandlerFunction buttonhandler, int buttonvalue = 0 );
    void LongPressButton ( uint8_t pin, ButtonHandlerFunction buttonhandler, int buttonvalue = 0 );
	//void LedBlink (uint8_t pin);
	//void LedFlash (uint8_t pin);
	void LedOn (uint8_t pin);
	void LedOff (uint8_t pin);
	void LedToggle(uint8_t pin);
protected:
    List <ScoreboardButtonClass*> _buttonList;
};
#endif

// ---------------------------------------------------------------------------------
// Global objects in scoreboard.h
extern ScoreboardI2CEEPROMClass i2ceeprom;
extern ScoreboardI2CTransferClass i2ctransfer;
extern ScoreboardI2CBatteryClass i2cbattery;
extern ScoreboardI2CClockClass i2cclock;
#ifdef ESP8266
extern ScoreboardI2CIOClass i2cio;
#endif
#endif