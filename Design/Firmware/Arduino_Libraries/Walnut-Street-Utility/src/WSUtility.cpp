/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/

#include "WSUtility.h"

//UtilityClass Utility;

// ------------------------------------------------------------
// Print a time_t value
void WSUtility::PrintTime ( time_t tm )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintTime ( tm, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintTime ( tm, Console );
#endif
}
// ------------------------------------------------------------
// Print a time_t value
void WSUtility::PrintTime ( time_t tm, Print &output )
{
    WSUtility::Print2Digits ( hour ( tm ), output );
    output.print ( F ( ":" ) );
    WSUtility::Print2Digits ( minute ( tm ), output );
    output.print ( F ( ":" ) );
    WSUtility::Print2Digits ( second ( tm ), output );
    output << F ( " " ) << day ( tm ) << F ( "/" ) << month ( tm ) << F ( "/" ) << year ( tm );
    output << F ( " (weekday " ) << weekday ( tm ) << F ( ")" ) << endl;
}
// ------------------------------------------------------------
// Print a number as 2 digits including leading zeros
void WSUtility::Print2Digits ( int number )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::Print2Digits ( number, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::Print2Digits ( number, Console );
#endif
}
// ------------------------------------------------------------
// Print a number as 2 digits including leading zeros
void WSUtility::Print2Digits ( int number, Print &output )
{
    // print a minus sign if needed
    if ( number < 0 )
        output.print ( F ( "-" ) );

    // truncate the value to 2 digits
    number = abs ( number );
    number = number % 100;

    // print out 2 digits
    if ( number < 10 )
        output.print ( F ( "0" ) );
    output.print ( number );
}
// ------------------------------------------------------------
// Print an integer to Serial (default) as 3 digits including leading zeros
void WSUtility::Print3Digits ( int number )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::Print3Digits ( number, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::Print3Digits ( number, Console );
#endif
}
// ------------------------------------------------------------
// Print an integer to a pointer of a Print object as 3 digits including leading zeros
void WSUtility::Print3Digits ( int number, Print &output )
{
    // print a minus sign if needed
    if ( number < 0 )
        output.print ( F ( "-" ) );

    // truncate the value to 3 digits
    number = abs ( number );
    number = number % 1000;

    // print out 3 digits
    if ( number < 100 )
        output.print ( F ( "0" ) );
    if ( number < 10 )
        output.print ( F ( "0" ) );
    output.print ( number );
}
// ------------------------------------------------------------
// Print a number to a Serial (default) object including an abritrary number of leading zeros
void WSUtility::PrintDigits ( int number, uint8_t digits )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintDigits ( number, digits, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintDigits ( number, digits, Console );
#endif
}
// ------------------------------------------------------------
// Print a number to a Print object including an abritrary number of leading zeros
void WSUtility::PrintDigits ( int number, uint8_t digits, Print &output )
{
    // print a minus sign if needed
    if ( number < 0 )
        output.print ( F ( "-" ) );

    // truncate the value to the specified number of digits
    number = abs ( number );
    number = number % ( int ) ( 0.5 + pow ( 10, digits ) );

    // print out each digit or leading zeros
    for ( uint8_t i = digits - 1; i > 0; i-- )
    {
        if ( number < ( int ) ( 0.5 + pow ( 10, i ) ) )
            output.print ( F ( "0" ) );
    }
    output.print ( number );
}
// ------------------------------------------------------------
void WSUtility::PrintByte ( uint8_t b )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintByte ( b, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintByte ( b, Console );
#endif
}
// ------------------------------------------------------------
void WSUtility::PrintByte ( uint8_t b, Print &output )
{
    for ( uint8_t i = 0; i < 8; i++ )
        output.print ( ( b & ( 1 << ( 7 - i ) ) ) == 0 ? 0 : 1 );
}
// ------------------------------------------------------------
void WSUtility::PrintWord ( uint16_t b )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintWord ( b, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintWord ( b, Console );
#endif
}
// ------------------------------------------------------------
void WSUtility::PrintWord ( uint16_t b, Print &output )
{
    for ( uint8_t i = 0; i < 16; i++ )
        output.print ( ( b & ( 1 << ( 15 - i ) ) ) == 0 ? 0 : 1 );
}
// ------------------------------------------------------------
void WSUtility::PrintSpacer()
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintSpacer ( Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintSpacer ( Console );
#endif
}
// ------------------------------------------------------------
void WSUtility::PrintSpacer ( Print &output )
{
    output.println ( F ( "------------------------------------" ) );
}
// ------------------------------------------------------------
void WSUtility::PrintHex ( uint8_t * buf, uint8_t len )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintHex ( buf, len, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintHex ( buf, len, Console );
#endif
}
// ------------------------------------------------------------
void WSUtility::PrintHex ( uint8_t * buf, uint8_t len, Print &output )
{
    for ( uint8_t i = 0; i < len; i++ )
    {
        if ( buf[i] <= 0xF )
            output.print ( '0' );
        output.print ( buf[i], HEX );
        output.print ( ' ' );
    }
}
// ------------------------------------------------------------
void WSUtility::PrintReadableChars ( uint8_t *buf, uint8_t len )
{
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
    WSUtility::PrintReadableChars ( buf, len, Serial );
#elif defined (CONSOLE_DEBUG)
    WSUtility::PrintReadableChars ( buf, len, Console );
#endif
}
// ------------------------------------------------------------
void WSUtility::PrintReadableChars ( uint8_t *buf, uint8_t len, Print &output )
{
    char c;
    for ( uint8_t i = 0; i < len; i++ )
    {
        c = buf[i];
        if ( ( uint8_t ) c < ( uint8_t ) 32 || ( uint8_t ) c > ( uint8_t ) 126 )
            output.print ( '.' );
        else
            output.print ( c );
    }
}
// ------------------------------------------------------------
void WSUtility::DeselectSPI ( const uint8_t *spi_cs_pins, uint8_t number_of_devices )
{
    for ( uint8_t i = 0; i < number_of_devices; i++ )
    {
        pinMode ( spi_cs_pins[i], OUTPUT );
        digitalWrite ( spi_cs_pins[i], HIGH );
    }
}
// ------------------------------------------------------------
bool WSUtility::FindWireDevice ( uint8_t addr, bool verbose )
{
    Wire.beginTransmission ( addr );
    byte error = Wire.endTransmission();

#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG) || defined (CONSOLE_DEBUG)
    if ( verbose )
    {
#if defined (SERIAL_DEBUG) || defined (ESP8266_DEBUG)
        Print &output = Serial;
#elif defined (CONSOLE_DEBUG)
        Print &output = Console;
#else
        Print &output = NULL;
#endif
        if ( error == 0 )
            output << F ( "I2C device found" );
        else if ( error == 4 )
            output << ( "Unknown I2C error" );
        else
            output << F ( "No I2C device found" );

        output << F ( " at address " ) << ( addr < 16 ? F ( "0x0" ) : F ( "0x" ) ) << String ( addr, HEX ) << endl;
    }
#endif
    return ( error == 0 ? true : false );
}
// ------------------------------------------------------------
#ifdef ESP8266
void WSUtility::PrintSystemSpecs()
{
    USTREAM ( endl );
    USTREAM ( F ( "Heap: " ) << system_get_free_heap_size() << endl );
    USTREAM ( F ( "Boot Vers: " ) << system_get_boot_version() << endl );
    USTREAM ( F ( "CPU: " ) << system_get_cpu_freq() << endl );
    USTREAM ( F ( "SDK: " ) << system_get_sdk_version() << endl );
    USTREAM ( F ( "Chip ID: " ) << system_get_chip_id() << endl );
    USTREAM ( F ( "Flash ID: " ) << spi_flash_get_id() << endl );
    USTREAM ( F ( "Flash Size: " ) << ESP.getFlashChipRealSize() << endl );
    USTREAM ( F ( "Vcc: " ) << ESP.getVcc() << endl );
    USTREAM ( endl );
}
#endif
