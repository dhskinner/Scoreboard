/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#ifndef _RFRADIOCLASS_h
#define _RFRADIOCLASS_h

#include <Arduino.h>
#include <TimeLib.h>
#include <SPI.h>                  // Arduino core library 
#include <RHReliableDatagram.h>   // Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#include <RH_NRF24.h>             // Download from http://www.airspayce.com/mikem/arduino/RadioHead/
#include <ScoreboardDefines.h>
#include <ScoreboardClass.h>
#include <WSUtility.h>

// External global objects - you need to define these in the main .ino file
extern ScoreboardClass scoreboard[FIELDS];		// Scoreboard class objects

class ScoreboardRadioClass
{
protected:
    RH_NRF24 _rfDriver;								// Singleton instance of the radio driver
    RHReliableDatagram _rfManager;					// Class to manage message delivery and receipt
    ScoreboardDatagram _rf24Datagram;				// struct used to transfer data over rf24
//    uint8_t _rf24DatagramLength;					// size of the struct in bytes
    uint8_t _rf24Buffer[RH_NRF24_MAX_MESSAGE_LEN];	// message buffer for incoming messages
    uint8_t _rf24From;								// address of sender for the last incoming message
    time_t  _rf24MessageReceived = now();			// time the last incoming message was received
    bool _offlineflag = true;
public:
    ScoreboardRadioClass ( uint8_t addr, uint8_t ce, uint8_t cs );
    bool Start();
    bool Process();
    bool Receive();
    bool Send ( MessageID id, uint8_t toRFAddress, uint8_t fieldID );
    void Broadcast ( uint8_t fieldID );
    uint8_t SignalQuality();
    void Off();
    void On();
};

#endif

