/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardI2CClass.h"

// ---------------------------------------------------------------------------------
// Calback handler for I2CTransfer
void I2CTransferReceive ( int numBytes ) {}
// ---------------------------------------------------------------------------------
void ScoreboardI2CTransferClass::StartAsMaster ( )
{
    Wire.begin();

    //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
    _i2c_transfer.begin ( details ( _i2c_datagram ), &Wire );

    UPRINTLN ( F ( "I2C transfer started as master" ) );
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CTransferClass::StartAsSlave ( uint8_t slave_address )
{
    // Set the I2C slave address
    Wire.begin ( slave_address );

    // Start the library, pass in the data details and the name of the stream. Can be Serial, Serial1, Serial2, Wire etc.
    _i2c_transfer.begin ( details ( _i2c_datagram ), &Wire );

    // define handler function on receiving data from master to slave
    Wire.onReceive ( I2CTransferReceive );

    UPRINTLN ( F ( "I2C transfer started as slave" ) );
}
// ---------------------------------------------------------------------------------
// broadcast the scoreboard datagram
void ScoreboardI2CTransferClass::Broadcast ( )
{
    for ( uint8_t fieldID = 0; fieldID < FIELDS; fieldID++ )
        this->Broadcast ( fieldID );
}
// ---------------------------------------------------------------------------------
// broadcast the scoreboard datagram
void ScoreboardI2CTransferClass::Broadcast ( uint8_t fieldID )
{
    scoreboard[fieldID].GetDatagram ( &_i2c_datagram );
    _i2c_transfer.sendData ( I2C_ADDRESS_DMD_0 );
    //i2c_transfer.sendData ( I2C_ADDRESS_DMD_1 );	// currently DMD0 is same address as DMD1
    _i2c_transfer.sendData ( I2C_ADDRESS_DMD_2 );
    USTREAM ( F ( "I2C broadcast: msg=" ) << _i2c_datagram.messageID << F ( " field=" ) << _i2c_datagram.fieldID + 1 << F ( " score1=" ) << _i2c_datagram.score1 << F ( " score2=" ) << _i2c_datagram.score2 );
    USTREAM ( F ( " systime=" ) << _i2c_datagram.systemTime << F ( " matchtime=" ) << _i2c_datagram.matchTime << F ( " flags=" ) << _i2c_datagram.flags << endl );
}
// ---------------------------------------------------------------------------------
bool ScoreboardI2CTransferClass::Process()
{
    //check and see if a data packet has come in from the master
    if ( _i2c_transfer.receiveData() )
    {
        // set the scoreboard datagrams (each scoreboard will check the ID)
        for ( uint8_t id = 0; id < FIELDS; id++ )
            scoreboard[id].SetDatagram ( &_i2c_datagram );

        USTREAM ( F ( "I2C receive: msg=" ) << _i2c_datagram.messageID << F ( " field=" ) << _i2c_datagram.fieldID + 1 << F ( " score1=" ) << _i2c_datagram.score1 << F ( " score2=" ) << _i2c_datagram.score2 );
        USTREAM ( F ( " systime=" ) << _i2c_datagram.systemTime << F ( " matchtime=" ) << _i2c_datagram.matchTime << F ( " flags=" ) << _i2c_datagram.flags << endl );
        return true;
    }
    return false;
}
// ---------------------------------------------------------------------------------
// Set the system time from the RTC
void ScoreboardI2CClockClass::SetSystemTimefromRTC()
{
    tmElements_t te;
    time_t tm;

    // Get current time
    if ( RTC.read ( te ) )
    {
        tm = RTC.get();
        setTime ( tm );

        USTREAM ( F ( "Real time clock detected " ) );
        WSUtility::PrintTime ( tm );
        USTREAM ( F ( "Synched system time to   " ) );
		WSUtility::PrintTime ( tm );
    }
    else
    {
        if ( RTC.chipPresent() )
            UPRINTLN ( F ( "Real time clock is stopped. Please run the SetTime example to initialize the time and begin running." ) );
        else
            UPRINTLN ( F ( "Real time clock read error. Please check the circuitry." ) );

        UPRINT ( F ( "Current system time is " ) );
		WSUtility::PrintTime ( now() );
    }
}
// ---------------------------------------------------------------------------------
// Check and set the RTC from the compiled time
void ScoreboardI2CClockClass::SetRTCfromCompilerTime()
{
    tmElements_t tm;

    // get the date and time the compiler was run
    if ( ParseDate ( __DATE__, &tm ) && ParseTime ( __TIME__, &tm ) )
    {
        // and configure the RTC with this info
        RTC.write ( tm );
    }
}
// ---------------------------------------------------------------------------------
bool ScoreboardI2CClockClass::ParseTime ( const char *str, TimeElements *tm )
{
    int Hour, Min, Sec;

    if ( sscanf ( str, "%d:%d:%d", &Hour, &Min, &Sec ) != 3 ) return false;
    tm->Hour = Hour;
    tm->Minute = Min;
    tm->Second = Sec;
    return true;
}
// ---------------------------------------------------------------------------------
bool ScoreboardI2CClockClass::ParseDate ( const char *str, TimeElements *tm )
{
    char Month[12];
    int Day, Year;
    uint8_t monthIndex;

    if ( sscanf ( str, "%s %d %d", Month, &Day, &Year ) != 3 ) return false;
    for ( monthIndex = 0; monthIndex < 12; monthIndex++ )
    {
        if ( strcmp ( Month, _monthName[monthIndex] ) == 0 ) break;
    }
    if ( monthIndex >= 12 ) return false;
    tm->Day = Day;
    tm->Month = monthIndex + 1;
    tm->Year = CalendarYrToTm ( Year );
    return true;
}

// ---------------------------------------------------------------------------------
// A flag to indicate a generated alert interrupt
volatile bool gaugeLowPowerAlert = false;
void GaugeLowPower()
{
    gaugeLowPowerAlert = true;
}
// ---------------------------------------------------------------------------------
/*
// LiFuelGauge constructor parameters
// 1. IC type, MAX17043 or MAX17044
// 2. Number of interrupt to which the alert pin is associated (Optional)
// 3. ISR to call when an alert interrupt is generated (Optional)
//
// Creates a LiFuelGauge instance for the MAX17043 IC
// and attaches lowPower to INT0 (PIN2 on most boards, PIN3 on Leonardo)
*/
static LiFuelGauge gauge ( MAX17043, 0, GaugeLowPower ); // LiPo battery fuel guage
// ---------------------------------------------------------------------------------
void ScoreboardI2CBatteryClass::Start()
{
    gauge.reset();						// Resets MAX17043
    delay ( 200 );						// Waits for the initial measurements to be made
    gauge.setAlertThreshold ( 10 );		// Sets the Alert Threshold to 10% of full capacity
		
    USTREAM ( F ( "I2C battery started voltage=" ) << gauge.getVoltage() << F ( " charge=" ) << gauge.getSOC() << F ( "% alert=" ) << gauge.getAlertThreshold() << "%" << endl );
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardI2CBatteryClass::Charge()
{
    int soc = gauge.getSOC();

	USTREAM(F("I2C Battery SOC: ") << gauge.getSOC() << F("% Cell: ") << gauge.getVoltage() << F("V") << endl);

    return ( uint8_t ) ( constrain ( soc, 0, 100 ) );
}
// ---------------------------------------------------------------------------------
bool ScoreboardI2CBatteryClass::Alert()
{
    return gaugeLowPowerAlert;
}
// ---------------------------------------------------------------------------------
ScoreboardI2CEEPROMClass::ScoreboardI2CEEPROMClass ( void )
{
    Wire.begin();
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CEEPROMClass::write ( unsigned long dataAddress, uint8_t data )
{
    // TODO
    /*
    Wire.beginTransmission((uint8_t)((0x500000 | dataAddress) >> 16)); // B1010xxx
    Wire.write((uint8_t)((dataAddress & WORD_MASK) >> 8)); // MSB
    Wire.write((uint8_t)(dataAddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
    delay(5);
    */
}
// ---------------------------------------------------------------------------------
uint8_t ScoreboardI2CEEPROMClass::read ( unsigned long dataAddress )
{
    /*
    uint8_t data = 0x00;
    Wire.beginTransmission((uint8_t)((0x500000 | dataAddress) >> 16)); // B1010xxx
    Wire.write((uint8_t)((dataAddress & WORD_MASK) >> 8)); // MSB
    Wire.write((uint8_t)(dataAddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom((uint8_t)((0x500000 | dataAddress) >> 16), 1);
    if (Wire.available()) data = Wire.peek();
    return data;
    */
    return 0;
}
// ---------------------------------------------------------------------------------
#ifdef ESP8266
static PCF8574	pcf8574A ( I2C_PCF8574A_ADDRESS );
static Ticker	pcfIoTicker;
static Ticker	pcfLedTicker;
volatile static bool PCFFlag = false;

// ---------------------------------------------------------------------------------
// These functions sense the start of a pin event (FALLING), then fire a timer to handle
// debouncing, short presses and long button presses. After the button presses are done,
// we revert back to the pin sense interrupt to save resources.
// ---------------------------------------------------------------------------------
// Callback handler for PCF8574A ticker interrupts
void PCFIoTickerInterrupt()
{
    PCFFlag = true;
}
// ---------------------------------------------------------------------------------
// Callback handler for PCF8574A pin sense interrupts
void PCFIoPinInterrupt()
{
    // disable the pin sense interrupt
    detachInterrupt ( digitalPinToInterrupt ( I2C_PCF8574A_INTERRUPT_PIN ) );

    // start the ticker to trigger polling
    pcfIoTicker.attach_ms ( I2C_PCF8574A_TIMER_MILLIS, PCFIoTickerInterrupt );

    PCFFlag = true;
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CIOClass::Start ( void )
{
    Wire.begin();

    // Most ready-made PCF8574 modules seem to lack an internal pullup-resistor,
    // so you have to use the ESP8266 - internal one or else it won't work
    pinMode ( I2C_PCF8574A_INTERRUPT_PIN, INPUT_PULLUP );

    // Set the interrupt flag to false
    PCFFlag = false;

    // If there is a button handler, then start the PFC IRQ. Pins default to
    // high, therefore we want the interrupt to trigger on the 'FALLING' edge
    if ( !_buttonList.IsEmpty() )
    {
        attachInterrupt ( digitalPinToInterrupt ( I2C_PCF8574A_INTERRUPT_PIN ), PCFIoPinInterrupt, FALLING );
        USTREAM ( F ( "PCF8574A started using interrupts on pin " ) << I2C_PCF8574A_INTERRUPT_PIN << endl );
    }
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CIOClass::Process ( )
{
    ScoreboardButtonClass* button;
    bool allButtonsAreIdle = true;

    // Has an interrupt been triggered?
    if ( !PCFFlag )
        return;

    // Get the current IO pin state
    uint8_t pinstate = pcf8574A.read8();

#ifdef BUTTON_DEBUG
    USTREAM ( F ( "Millis=" ) << millis() %10000 << F ( " Pins=" ) );
    for ( uint8_t i = 0; i < 8; i++ )
        UPRINT ( bitRead ( pinstate, i ) );
    USTREAM ( endl );
#endif

    // Loop through each button handler to see if there is anything to do
    _buttonList.Begin();
    while ( !_buttonList.IsEnd() )
    {
        button = _buttonList.GetValue();

        if ( button->Process ( pinstate ) != true )
            allButtonsAreIdle = false;

        _buttonList.Next();
    }

    // If all the buttons are now idle (i.e. we're not waiting for debouncing or any
    // functions to complete, stop polling the ticker and go back to a pin interrupt
    if ( allButtonsAreIdle )
    {
        pcfIoTicker.detach();
        attachInterrupt ( digitalPinToInterrupt ( I2C_PCF8574A_INTERRUPT_PIN ), PCFIoPinInterrupt, FALLING );
        UPRINTLN ( F ( "Buttons are idle" ) );
    }

    // All done
    PCFFlag = false;
}
// ---------------------------------------------------------------------------------
// Short press
void ScoreboardI2CIOClass::ShortPressButton ( uint8_t pin, ButtonHandlerFunction buttonhandler, int buttonvalue )
{
    ScoreboardButtonClass* button = new ScoreboardButtonClass ( pin, buttonhandler, buttonvalue );
    _buttonList.Push ( button );
    USTREAM ( F ( "Button added num=" ) << String ( _buttonList.Count() ) << F ( " pin=" ) << pin << F ( " delay=0 value=" ) << button->Value() << endl );
}
// ---------------------------------------------------------------------------------
// Long press
void ScoreboardI2CIOClass::LongPressButton ( uint8_t pin, ButtonHandlerFunction buttonhandler, int buttonvalue )
{
    ScoreboardButtonClass* button = new ScoreboardButtonClass ( pin, buttonhandler, buttonvalue, I2C_BUTTON_LONGPRESS_MILLIS, I2C_BUTTON_REPEAT_MILLIS );
    _buttonList.Push ( button );
    USTREAM ( F ( "Button added num=" ) << String ( _buttonList.Count() ) << F ( " pin=" ) << pin << F ( " delay=" ) << I2C_BUTTON_LONGPRESS_MILLIS << F ( " value=" ) << button->Value() << endl );
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CIOClass::LedOn ( uint8_t pin )
{
    pcf8574A.write ( pin, I2C_LED_STATE_ON );
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CIOClass::LedOff ( uint8_t pin )
{
    pcf8574A.write ( pin, I2C_LED_STATE_OFF );
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CIOClass::LedToggle(uint8_t pin)
{
	uint8_t ledState = pcf8574A.read(pin);

	pcf8574A.write(pin, (ledState == I2C_LED_STATE_OFF ? I2C_LED_STATE_ON : I2C_LED_STATE_OFF) );
}
/*
void ScoreboardI2CIOClass::LedBlink(uint8_t pin, uint16_t on_durn, uint16_t off_durn)
{
_ledMode = IO_LED_MODE_BLINK;
_onMillis = on_millis;
_offMillis = off_millis;
#if defined (ESP8266)
_ledTicker.detach();
_ledTickerOnMillis = _onMillis;
_ledTickerOffMillis = _offMillis;
digitalWrite(_ledPin, IO_LED_STATE_OFF);
ESPLEDOn();
//#else
this->Process();
//#endif
}
// ---------------------------------------------------------------------------------
void ScoreboardI2CIOClass::LedFlash(uint8_t pin)
{

}
*/

#endif

