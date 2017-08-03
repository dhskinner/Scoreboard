/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _WS_UTILITY_H_
#define _WS_UTILITY_H_

#include <Arduino.h>
#include <Print.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Streaming.h>

#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

// Add SERIAL_DEBUG or CONSOLE_DEBUG
// #define SERIAL_DEBUG
// Remember to initialise the "Serial" or "Console" object in your setup code with UBEGIN

#define UHEARTBEAT_BEGIN(pin)		pinMode ( pin , OUTPUT )
#define UHEARTBEAT_UPDATE(pin, interval)	static unsigned long updateMillis = millis(); if (millis() > updateMillis){ updateMillis += interval ; digitalWrite( pin , (digitalRead( pin ) == HIGH ? LOW : HIGH)); }
#define ULEDBEGIN				pinMode(LED_BUILTIN, OUTPUT);
#define ULEDBLINK(x)			for(uint8_t i = 0; i < x; i++){digitalWrite(LED_BUILTIN, HIGH);delay(10);digitalWrite(LED_BUILTIN, LOW);delay(150);}
#define ULEDON()				digitalWrite(LED_BUILTIN,HIGH)
#define ULEDOFF()				digitalWrite(LED_BUILTIN,LOW)
#define ULEDTOGGLE()			digitalWrite(LED_BUILTIN,(digitalRead(LED_BUILTIN)==LOW?HIGH:LOW))
#define UARRAY_SIZE(A)			(sizeof(A) / sizeof((A)[0]))

// Serial debugging macros for Arduino
#if defined (SERIAL_DEBUG)
#define UBEGIN(x)			Serial.begin(x); 					//UDEBUGBEGIN is a macro, debug begin serial
#define UPRINT(...)			Serial.print(__VA_ARGS__)			//UPRINT is a macro, debug print
#define UPRINTLN(...)		Serial.println(__VA_ARGS__)			//UPRINTLN is a macro, debug print with new line
#define UPRINTF(...)		Serial.printf(__VA_ARGS__)
#define USTREAM(...)		Serial << __VA_ARGS__				//USTREAM is a macro, streams debug statements 
#define UPRINTTAB			Serial.print(F("  "))
#define UPRINTLNTAB			Serial << endl << F("  ")
#define UPRINTLNPROMPT		Serial << endl << F("> ")
#define UPRINTPROMPT		Serial << F("> ")
#define UPRINTSPACER		Serial << F ( "------------------------------------" ) << endl

// Console debugging macros for Arduino Yun
#elif defined(CONSOLE_DEBUG)
#define UBEGIN()			Console.begin(); while (!Console) {} // See above
#define UPRINT(...)			Console.print(__VA_ARGS__)
#define UPRINTLN(...)		Console.println(__VA_ARGS__)
#define UPRINTF(...)		Serial.printf(__VA_ARGS__)
#define USTREAM(...)		Console << __VA_ARGS__
#define UPRINTTAB			Console.print(F("  "))
#define UPRINTLNTAB			Console << endl << F("  ")
#define UPRINTLNPROMPT		Console << endl << F("> ")
#define UPRINTPROMPT		Console << F("> ")
#define UPRINTSPACER		Console << F ( "------------------------------------" ) << endl

// Serial debugging macros for ESP8266
#elif defined(ESP8266_DEBUG)
#define UBEGIN(x)			Serial.begin(x); while (!Serial) {}	// See above
#define UPRINT(...)			Serial.print(__VA_ARGS__)
#define UPRINTLN(...)		Serial.println(__VA_ARGS__)
#define UPRINTF(...)		Serial.printf(__VA_ARGS__)
#define USTREAM(...)		Serial << __VA_ARGS__
#define UPRINTTAB			Serial.print(F("  "))
#define UPRINTLNTAB			Serial << endl << F("  ")
#define UPRINTLNPROMPT		Serial << endl << F("> ")
#define UPRINTPROMPT		Serial << F("> ")
#define UPRINTSPACER		Serial << F ( "------------------------------------" ) << endl

// Define blank lines to clear debugging statements during precompile
#else
#define UBEGIN(...)
#define UPRINT(...)
#define UPRINTLN(...)
#define UPRINTF(...)
#define USTREAM(...)
#define UPRINTTAB
#define UPRINTLNTAB
#define UPRINTLNPROMPT
#define UPRINTPROMPT
#define UPRINTSPACER
#endif

class WSUtility
{
private:

protected:

public:
    // Print a number as 2 digits including leading zero
    static void Print2Digits ( int number );

    // Print a number as 2 digits including leading zero
    static void Print2Digits ( int number, Print &output );

    // Print a number as 3 digits including leading zero
    static void Print3Digits ( int number );

    // Print a number as 3 digits including leading zero
    static void Print3Digits ( int number, Print &output );

    // Print a number including leading zeros
    static void PrintDigits ( int number, uint8_t digits );

    // Print a number including leading zeros
    static void PrintDigits ( int number, uint8_t digits, Print &output );

    // Print a time_t value
    static void PrintTime ( time_t tm );

    // Print a time_t value
    static void PrintTime ( time_t tm, Print &output );

    // Print a tab
    static void PrintTab();

    // Print a tab
    static void PrintTab ( Print &output );

    // Print an 8 bit byte
    static void PrintByte ( uint8_t b );

    // Print an 8 bit byte
    static void PrintByte ( uint8_t b, Print &output );

    // Print a 16 bit word
    static void PrintWord ( uint16_t b );

    // Print a 16 bit word
    static void PrintWord ( uint16_t b, Print &output );

    // Print a spacer
    static void PrintSpacer();

    // Print a spacer
    static void PrintSpacer ( Print &output );

    // Print a number as HEX (could also just use UPRINT(n, HEX))
    static void PrintHex ( uint8_t * buf, uint8_t len );

    // Print a number as HEX (could also just use UPRINT(n, HEX))
    static void PrintHex ( uint8_t * buf, uint8_t len, Print &output );

    // Print the readable characters in a string
    static void PrintReadableChars ( uint8_t *buf, uint8_t len );

    // Print the readable characters in a string
    static void PrintReadableChars ( uint8_t *buf, uint8_t len, Print &output );

    // A small utility function to deselect SPI devices by setting
    // their chipselect lines HIGH (otherwise the SPI devices will
    // interfere with each other)
    static void DeselectSPI ( const uint8_t *spi_cs_pins, uint8_t number_of_devices );

    // Search for an I2C device - return true if detected
    static bool FindWireDevice ( uint8_t addr, bool verbose = false );

#ifdef ESP8266
    static void PrintSystemSpecs();
#endif
};

/*
class CircularBuffer_Uint8
{
protected:
    uint8_t _buffersize;
    uint8_t _bufferlen, _bufferhead, _buffertail;
    uint8_t *_buffer;

public:
    inline CircularBuffer_Uint8 ( uint8_t buffersize )
    {
        _bufferlen = 0;
        _bufferhead = 0;
        _buffertail = 0;
        _buffersize = buffersize;
        _buffer = new uint8_t[buffersize];
    }

    inline void Add ( uint8_t data )
    {
        // Is the buffer full and we need to overwrite oldest data?
        if ( _bufferlen == _buffersize )
        {
            _bufferlen--;
            _bufferhead = ( _bufferhead + 1 ) % _buffersize;
        }

        _buffer[_buffertail] = data;
        _bufferlen++;
        _buffertail = ( _buffertail + 1 ) % _buffersize;
    }

    inline uint8_t Size()
    {
        return _buffersize;
    }

    inline uint8_t Length()
    {
        return _bufferlen;
    }
};

class CircularBuffer_Uint16
{
protected:
    uint8_t _buffersize;
    uint8_t _bufferlen, _bufferhead, _buffertail;
    uint16_t *_buffer;

public:
    inline CircularBuffer_Uint16 ( uint16_t buffersize )
    {
        _bufferlen = 0;
        _bufferhead = 0;
        _buffertail = 0;
        _buffersize = buffersize;
        _buffer = new uint16_t[buffersize];
    }

    inline void Add ( uint16_t data )
    {
        // Is the buffer full and we need to overwrite oldest data?
        if ( _bufferlen == _buffersize )
        {
            _bufferlen--;
            _bufferhead = ( _bufferhead + 1 ) % _buffersize;
        }

        _buffer[_buffertail] = data;
        _bufferlen++;
        _buffertail = ( _buffertail + 1 ) % _buffersize;
    }

    inline uint8_t Size()
    {
        return _buffersize;
    }

    inline uint8_t Length()
    {
        return _bufferlen;
    }
};
*/

/* ====================================================================== */
// General purpose circular buffer for simple data types (e.g. chars)
// T = data type stored in this buffer
// L = index type e.g. uint8_t for buffer size of up to 255 elements
template<class T, class L> class UtilityBuffer
{
protected:
    L _length, _first, _last, _available;
    T *_buffer;

public:
    inline UtilityBuffer ( L available )
    {
        _length = 0;
        _first = 0;
        _last = 0;
        _available = available;
        _buffer = new T[_available];
    }

    inline void Push ( T data )
    {
        // Is the buffer full and we need to overwrite oldest data?
        // TODO add a switch to choose between overwriting oldest data, or discarding new data
        if ( this->IsFull() )
        {
            _length--;
            _first = ( _first + 1 ) % _length;
        }

        _buffer[_last] = data;
        _length++;
        _last = ( _last + 1 ) % _length;
    }

    inline T Pop ( void )
    {
        T data;
        if ( _available > 0 )
        {
            data = _buffer[_first];
            _first = ( _first - 1 ) % _length;
            _length--;
        }
        return data;
    }

    inline T Peek ( void )
    {
        T data;
        if ( _available > 0 )
        {
            data = _buffer[_first];
        }
        return data;
    }

    inline L Available()
    {
        return _available;
    }

    inline L Length()
    {
        return _length;
    }

    inline bool IsEmpty()
    {
        return ( _available == 0 );
    }

    inline bool IsFull()
    {
        return ( _available == _length );
    }
};

/* ====================================================================== */
// General purpose queue for complex data types (e.g. user defined classes)
// T = data type stored in this buffer - MUST have the equals "=" operand implemented
// L = index type e.g. uint8_t for queue size of up to 255 elements
template<class T, class L> class UtilityQueue
{
private:
    struct Node
    {
        T data;
        Node *next;
    };

    Node *_first;
    Node *_last;
    uint8_t _available;

public:

    inline UtilityQueue() : _first ( NULL ), _last ( NULL ), _available ( 0 )
    {
    }

    inline ~UtilityQueue()
    {
        this->Clear();
    }

    inline void Clear()
    {
        Node *next;
        while ( _first )
        {
            next = _first->next;
            delete _first;
            _first = next;
        }
        _first = _last = NULL;
        _available = 0;
    }

    inline bool IsEmpty()
    {
        return ( _available == 0 );
    }

    inline uint8_t Available()
    {
        return _available;
    }

    inline void Push ( T data )
    {
        /*
        Node newnode = new Node();
        newnode.data = data;
        newnode.next = NULL;
        if ( IsEmpty() )
            _first = newnode;
        else
            _last->next = newnode;
        _last = newnode;
        _available++;
        */
    }

    inline T Pop()
    {
        T data;
        if ( !IsEmpty() )
        {
            data = _first->data;
            Node *temp = _first;
            if ( _first == _last )
                _first = _last = NULL;
            else
                _first = _first->next;
            delete temp;
            _available--;
        }
        return data;
    }
};

#endif // _UTILITY_H_
