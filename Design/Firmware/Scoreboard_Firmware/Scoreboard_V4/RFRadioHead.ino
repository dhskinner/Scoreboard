#ifdef RF_RADIOHEAD
// ---------------------------------------------------------------------------------
// NRF24 Definitions

RH_NRF24 rfDriver ( RF24_CE, RF24_CS );                 // Singleton instance of the radio driver
RHReliableDatagram rfManager ( rfDriver, thisRFAddress ); // Class to manage message delivery and receipt, using the driver declared above

SCOREBOARD_DATAGRAM rf24Datagram;                        // struct used to transfer data over rf24
const uint8_t rf24DatagramLength = sizeof ( rf24Datagram ); // size of the struct in bytes
uint8_t rf24Buffer[RH_NRF24_MAX_MESSAGE_LEN];            // message buffer for incoming messages
uint8_t rf24From;                                        // address of sender for the last incoming message
time_t  rf24MessageReceived = 0;                         // time the last incoming message was received

// ---------------------------------------------------------------------------------
boolean RFStart()
{
  // disable all other SPI devices
  for ( uint8_t i = 0; i < sizeof ( spi ); i++ )
  {
    pinMode ( i, OUTPUT );
    digitalWrite ( i, HIGH );
  }

  result = true;

  if ( !rfManager.init() ) // Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
    result = false;

  if ( !rfDriver.setChannel ( RF24_CHANNEL ) )
    result = false;

  if ( !rfDriver.setRF ( RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm ) ) 
    result = false;

  rfManager.setRetries ( RF24_RETRIES );
  rfManager.setTimeout ( RF24_LISTEN_TIMEOUT );
  rfDriver.setHeaderFrom ( thisRFAddress );

  return result;
}
// ---------------------------------------------------------------------------------
// Encode and send the datagram in binary, with header, length info and checksum
boolean RFSend ( MessageID id, uint8_t toRFAddress, uint8_t fieldID )
{
  // check if the controller is in offline mode
  if ( offlineFlag == true )
    return true;

  uint8_t i = 0;

  // get the message data and set the message ID
  scoreboard[fieldID].GetDatagram ( &rf24Datagram );
  rf24Datagram.messageID = id;

  // encode the header and the initial checksum value
  uint8_t rf24CS = 0;
  rf24Buffer[i] = RF24_HEADER_BYTE;
  rf24CS ^= rf24Buffer[i];
  i++;   // header byte 0
  rf24Buffer[i] = rf24DatagramLength;
  rf24CS ^= rf24Buffer[i];
  i++;   // header byte 1 is the datagram size (used to ensure that the binary structs on both Arduinos are the same size)

  // encode the datagram by getting the struct bytes direct from memory
  uint8_t *address = ( uint8_t* ) &rf24Datagram;                      //address of the first byte of the struct
  for ( uint8_t c = 0; c < rf24DatagramLength; c++ )
  {
    rf24CS ^= *address;
    rf24Buffer[i] = *address;
    address = address + 1;     // increment the pointer
    i++;                       // increment the buffer index
  }
  rf24Buffer[i] = rf24CS;
  i++; // add the checksum
   
  // Send a reply back to the originator client
  result = rfManager.sendtoWait ( rf24Buffer, i, toRFAddress ) ;

  return result;
}
// ---------------------------------------------------------------------------------
// Receive and decode any waiting packets
boolean RFReceive()
{
  if ( rfManager.available() )
  {
    // Wait for a message addressed to us from the client
    uint8_t rf24BufferLength = sizeof ( rf24Buffer );
    if ( rfManager.recvfromAck ( rf24Buffer, &rf24BufferLength, &rf24From ) )
    {
      // confirm that the message length matches
      if ( rf24BufferLength != rf24DatagramLength + RF24_HEADER_SIZE + 1 )
        return false;

      // confirm the header bytes are OK
      if ( rf24Buffer[0] != RF24_HEADER_BYTE )
        return false;

      // confirm that the struct size is the same in both sender and receiver
      // (probably not necessary as this is also checked in the checksum but left it in anyways)
      if ( rf24Buffer[1] != rf24DatagramLength )
        return false;

      // calculate the checksum including the header bytes
      // (however clearly excluding the final byte that is the received checksum)
      uint8_t rf24CS = 0;
      for ( uint8_t i = 0; i < ( rf24BufferLength - 1 ); i++ )
        rf24CS ^= rf24Buffer[i];

      // confirm the checksum matches
      if ( rf24CS == rf24Buffer[rf24BufferLength - 1] )
      {
        memcpy ( &rf24Datagram, &rf24Buffer[RF24_HEADER_SIZE], rf24DatagramLength );
        return true;
      }
    }
  }
  return false;
}
// ---------------------------------------------------------------------------------
// Process any waiting packets
void RFProcess()
{
  // check if the controller is in offline mode
  if ( offlineFlag == true )
    return;

  if ( RFReceive() )
  {
    // check the field ID is in range
    if ( rf24Datagram.fieldID > FIELDS )
      return;

    switch ( rf24Datagram.messageID )
    {
    case BROADCAST:
      if ( rf24From == RF24_SCOREBOARD_ADDRESS )  // update all values if a broadcast from the scoreboard is received
        scoreboard[rf24Datagram.fieldID].SetDatagram ( &rf24Datagram );

#ifdef ULCD
      ULCD_Update ( false );
#endif
      break;

    // respond to sender with the current state (ignore the rest of message)
    case GET_STATE:
      RFBroadcast ( rf24Datagram.fieldID );
      break;

    // set the new values, then respond to sender with the current state (ignore the rest of message)
    case SET_SCORE:
	case SET_TIMER:
	case SET_MODE_FLAGS:
      scoreboard[rf24Datagram.fieldID].SetDatagram ( &rf24Datagram );

#ifdef SCOREBOARD_MASTER
      I2CBroadcast ( rf24Datagram.fieldID );
      RFBroadcast ( rf24Datagram.fieldID );
#endif

#ifdef ULCD
      ULCD_Update ( false );
#endif
      break;

    case MESSAGE_NULL:
    default:
      break;
    }
    rf24MessageReceived = now();
  }
}
// ---------------------------------------------------------------------------------
// Broadcast the current scoreboard state
void RFBroadcast ( uint8_t fieldID )
{
  RFSend ( BROADCAST, RH_BROADCAST_ADDRESS, fieldID );
}
// ---------------------------------------------------------------------------------
// Simple signal quality indicator - returns 0 - 5 based on how many seconds since the last broadcast received
uint8_t RFSignalQuality()
{
  int qual = 6 - ( now() - rf24MessageReceived );
  return constrain ( qual , 0, 5 );
}
// ---------------------------------------------------------------------------------
#endif
