#ifdef I2C_TRANSFER

EasyTransferI2C i2c;                // easyTransfer object
SCOREBOARD_DATAGRAM i2c_datagram;   // struct used to transfer data over I2C

// ---------------------------------------------------------------------------------
void I2C_Transfer_Start()
{
#ifdef SCOREBOARD_MASTER
  Wire.begin();

  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  i2c.begin ( details ( i2c_datagram ), &Wire );
#else 
  // Set the I2C Slave address - default is DMD_ROLE_0
  #if (DMD_ROLE == 2)
    Wire.begin ( I2C_SLAVE_ADDRESS_DMD_2 );
  #elif (DMD_ROLE ==  1)
	Wire.begin ( I2C_SLAVE_ADDRESS_DMD_1 );
  #else
	Wire.begin ( I2C_SLAVE_ADDRESS_DMD_0 );
  #endif

  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  i2c.begin ( details ( i2c_datagram ), &Wire );

  //define handler function on receiving data
  Wire.onReceive ( I2C_Receive );
#endif
}
// ---------------------------------------------------------------------------------
void I2C_Broadcast ( uint8_t fieldID )
{
#ifdef SCOREBOARD_MASTER
  // broadcast the scoreboard datagram
  scoreboard[fieldID].GetDatagram ( &i2c_datagram );

  i2c.sendData ( I2C_SLAVE_ADDRESS_DMD_0 );
  //i2c.sendData ( I2C_SLAVE_ADDRESS_DMD_1 );	// not used - DMD0 is same address as DMD1
  i2c.sendData ( I2C_SLAVE_ADDRESS_DMD_2 );
#endif
}
// ---------------------------------------------------------------------------------
void I2C_Process()
{
  //check and see if a data packet has come in from the master
  if ( i2c.receiveData() )
  {
    // send the data to the scoreboards (each scoreboard will check the ID)
    for ( uint8_t id = 0; id < FIELDS; id++ )
      scoreboard[id].SetDatagram ( &i2c_datagram );

#if defined(LED_DMD2) || defined (LED_DMD)
	dmdStartupFlag = false;		// this flag is used for the startup sequence
    DMD_Update ( );
#endif

#if defined(LED_SEGMENT)
    LED_Update ( 0 );
#endif
  }
}
// ---------------------------------------------------------------------------------
void I2C_Receive ( int numBytes ) {}
// ---------------------------------------------------------------------------------
#endif
