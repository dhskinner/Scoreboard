/*
Creative Commons Attribution-ShareAlike 3.0 license

May 2016 @ http://www.walnut-street.net
*/
#include "ScoreboardRadioClass.h"
//#define RF_DEBUG

// ---------------------------------------------------------------------------------
ScoreboardRadioClass::ScoreboardRadioClass ( uint8_t addr, uint8_t ce, uint8_t cs ) : _rfDriver ( ce, cs ), _rfManager ( _rfDriver, addr )
{
}
// ---------------------------------------------------------------------------------
bool ScoreboardRadioClass::Start()
{
    bool result = true;

    // Set the default offline mode
    _offlineflag = true;

    // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
    if ( !_rfManager.init() )
    {
        UPRINTLN ( F ( "Radio init failed" ) );
        result = false;
    }

    if ( !_rfDriver.setChannel ( RF24_CHANNEL ) )
    {
        UPRINTLN ( F ( "Radio set channel failed" ) );
        result = false;
    }

    if ( !_rfDriver.setRF ( RF24_DATA_RATE , RF24_TX_POWER ) )
    {
        UPRINTLN ( F ( "Radio set rate and power failed" ) );
        result = false;
    }

    _rfManager.setRetries ( RF24_RETRIES );
    _rfManager.setTimeout ( RF24_LISTEN_TIMEOUT );
    _rfDriver.setHeaderFrom ( _rfManager.thisAddress() );
	_rf24MessageReceived = now();

    if ( result )
    {
        UPRINTLN ( F ( "Radio startup OK" ) );
    }
    else
    {
        UPRINTLN ( F ( "Radio startup failed" ) );
    }

    return result;
}
// ---------------------------------------------------------------------------------
// Encode and send the datagram in binary, with header, length info and checksum
bool ScoreboardRadioClass::Send ( MessageID id, uint8_t toRFAddress, uint8_t fieldID )
{
    uint8_t i = 0;

    // check if the controller is in offline mode
    if ( _offlineflag )
        return true;

    // get the message data and set the message ID
    scoreboard[fieldID].GetDatagram ( &_rf24Datagram );
    _rf24Datagram.messageID = id;

    // encode the header and the initial checksum value
    uint8_t rf24CS = 0;
    _rf24Buffer[i] = RF24_HEADER_BYTE;
    rf24CS ^= _rf24Buffer[i];
    i++;   // header byte 0
    _rf24Buffer[i] = RF24_PAYLOAD_SIZE;
    rf24CS ^= _rf24Buffer[i];
    i++;   // header byte 1 is the datagram size (used to ensure that the binary structs on both Arduinos are the same size)

    // encode the datagram by getting the struct bytes direct from memory
    uint8_t *address = ( uint8_t* ) &_rf24Datagram;			//address of the first byte of the struct
    for ( uint8_t c = 0; c < RF24_PAYLOAD_SIZE; c++ )
    {
        rf24CS ^= *address;
        _rf24Buffer[i] = *address;
        address = address + 1;     // increment the pointer
        i++;                       // increment the buffer index
    }
    _rf24Buffer[i] = rf24CS;
    i++; // add the checksum

	USTREAM(F("Radio send:    msg=") << _rf24Datagram.messageID << F(" field=") << _rf24Datagram.fieldID + 1 << F(" score1=") << _rf24Datagram.score1 << F(" score2=") << _rf24Datagram.score2);
	USTREAM(F(" systime=") << _rf24Datagram.systemTime << F(" matchtime=") << _rf24Datagram.matchTime << F(" flags=") << _rf24Datagram.flags << endl);

    // Send a reply back to the originator client
    bool result = _rfManager.sendtoWait ( _rf24Buffer, i, toRFAddress );
#ifdef RF_DEBUG
    USTREAM ( F ( "Radio send: bytes=" ) << i << F ( " data=" ) );
    utility.PrintHex ( _rf24Buffer, i );
    USTREAM ( F ( "result=" ) << ( result ? F ( "OK" ) : F ( "fail" ) ) << endl );
#endif
    return result;
}
// ---------------------------------------------------------------------------------
// Receive and decode any waiting packets
bool ScoreboardRadioClass::Receive()
{
    // check if the controller is in offline mode
    if ( _offlineflag )
        return false;

    if ( _rfManager.available() )
    {
        // Wait for a message addressed to us from the client
        uint8_t rf24BufferLength = sizeof ( _rf24Buffer );
        if ( _rfManager.recvfromAck ( _rf24Buffer, &rf24BufferLength, &_rf24From ) )
        {
#ifdef RF_DEBUG
            // print the received data for debugging
            USTREAM ( F ( "Radio rx: bytes=" ) << rf24BufferLength << F ( " data=" ) );
            utility.PrintHex ( _rf24Buffer, rf24BufferLength );
            USTREAM ( endl );
#endif
            // confirm that the message length matches
            if ( rf24BufferLength != RF24_PACKET_SIZE )
            {
#ifdef RF_DEBUG
                USTREAM ( F ( "Radio rx: message length mismatch (bytes=" ) << rf24BufferLength << F ( " expected=" ) << RF24_PACKET_SIZE << F ( ")" ) << endl );
#endif
                return false;
            }

            // confirm the header bytes are OK
            if ( _rf24Buffer[0] != RF24_HEADER_BYTE )
            {
#ifdef RF_DEBUG
                USTREAM ( F ( "Radio rx: header mismatch (header=" ) << _rf24Buffer[0] << F ( " expected=" ) << RF24_HEADER_BYTE << F ( ")" ) << endl );
#endif
                return false;
            }

            // confirm that the struct size is the same in both sender and receiver
            // (probably not necessary as this is also checked in the checksum but left it in anyways)
            if ( _rf24Buffer[1] != RF24_PAYLOAD_SIZE )
            {
#ifdef RF_DEBUG
                USTREAM ( F ( "Radio rx: datagram length mismatch (length= " ) << _rf24Buffer[1] << F ( " expected = " ) << RF24_PAYLOAD_SIZE << F ( ")" ) << endl );
#endif
                return false;
            }

            // calculate the checksum including the header bytes
            // (however excluding the final byte that is the received checksum)
            uint8_t rf24CS = 0;
            for ( uint8_t i = 0; i < ( rf24BufferLength - 1 ); i++ )
                rf24CS ^= _rf24Buffer[i];

            // confirm the checksum matches
            if ( rf24CS == _rf24Buffer[rf24BufferLength - 1] )
            {
                memcpy ( &_rf24Datagram, &_rf24Buffer[RF24_HEADER_SIZE], RF24_PAYLOAD_SIZE );
#ifdef RF_DEBUG
                USTREAM ( F ( "Radio rx: id=" ) << _rf24Datagram.messageID << F ( " score1=" ) << _rf24Datagram.score1 << F ( " score2=" ) << _rf24Datagram.score2 );
                USTREAM ( F ( " systime=" ) << _rf24Datagram.systemTime << F ( " matchtime=" ) << _rf24Datagram.matchTime << F ( " flags=" ) << _rf24Datagram.flags << endl );
#endif
                return true;
            }
            else
            {
#ifdef RF_DEBUG
                USTREAM ( F ( "Radio rx: checksum mismatch (sum=" ) << rf24CS << F ( " expected=" ) << _rf24Buffer[rf24BufferLength - 1] << F ( ")" ) << endl );
#endif
            }
        }
    }
    return false;
}
// ---------------------------------------------------------------------------------
// Call this frequently to process new packets. Returns true if a message was received
// that updated the state of the scoreboard or requires some other action
bool ScoreboardRadioClass::Process()
{
    // check if the controller is in offline mode
    if ( _offlineflag )
        return false;

    if ( this->Receive() )
    {
        _rf24MessageReceived = now();

        USTREAM ( F ( "Radio process: msg=" ) << _rf24Datagram.messageID << F ( " field=" ) << _rf24Datagram.fieldID+1 << F ( " score1=" ) << _rf24Datagram.score1 << F ( " score2=" ) << _rf24Datagram.score2 );
        USTREAM ( F ( " systime=" ) << _rf24Datagram.systemTime << F ( " matchtime=" ) << _rf24Datagram.matchTime << F ( " flags=" ) << _rf24Datagram.flags << endl );

        // check the field ID is in range
        if ( _rf24Datagram.fieldID < FIELDS )
        {
            switch ( _rf24Datagram.messageID )
            {
            case BROADCAST:
                if ( _rf24From == RF24_SCOREBOARD_ADDRESS ) // update all values if a broadcast from the scoreboard is received
                    if ( scoreboard[_rf24Datagram.fieldID].SetDatagram ( &_rf24Datagram ) )
                        return true;
                break;

            // respond to sender with the current state (ignore the rest of message)
            case GET_STATE:
                this->Broadcast ( _rf24Datagram.fieldID );
                break;

            // set the new values, then respond to sender with the current state (ignore the rest of message)
            case SET_SCORE:
            case SET_TIMER:
            case SET_MODE:
                if ( scoreboard[_rf24Datagram.fieldID].SetDatagram ( &_rf24Datagram ) )
                {
                    this->Broadcast ( _rf24Datagram.fieldID );
                    return true;
                }
                break;

            case MESSAGE_NULL:
            default:
                break;
            }
        }
    }
    return false;
}
// ---------------------------------------------------------------------------------
// Broadcast the current scoreboard state
void ScoreboardRadioClass::Broadcast ( uint8_t fieldID )
{
    // check if the controller is in offline mode
    if ( _offlineflag )
        return;

    this->Send ( BROADCAST, RH_BROADCAST_ADDRESS, fieldID );

    USTREAM ( F ( "Radio broadcast: field=" ) << fieldID << F ( " score1=" ) << scoreboard[fieldID].Score1() << F ( " score2=" ) << scoreboard[fieldID].Score2() << endl );
}
// ---------------------------------------------------------------------------------
// Simple signal quality indicator - returns 0 - 5 based on how many seconds since the last broadcast received
// 5 is good signal (received within the last 2 seconds) while 0 is poor (nothing received for 7 seconds or greater)
uint8_t ScoreboardRadioClass::SignalQuality()
{
    int qual = 7 - ( now() - _rf24MessageReceived );
    return constrain ( qual, 0, 5 );
}
// ---------------------------------------------------------------------------------
// Set the radio offline
void ScoreboardRadioClass::Off()
{
    _offlineflag = true;
    UPRINTLN ( F ( "Radio transmitter off" ) );
}
// ---------------------------------------------------------------------------------
// Set the radio online
void ScoreboardRadioClass::On()
{
    _offlineflag = false;
    UPRINTLN ( F ( "Radio transmitter on" ) );
}
// ---------------------------------------------------------------------------------
