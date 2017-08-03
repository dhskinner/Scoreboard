#ifdef TEST_NRF24L01

RH_NRF24 rfDriver(RF24_CE_PIN, RF24_CS_PIN);				// Singleton instance of the radio driver
RHReliableDatagram rfManager(rfDriver, RF24_ADDRESS);		// Class to manage message delivery and receipt

uint8_t rfBuffer[RH_NRF24_MAX_MESSAGE_LEN];					// message buffer for incoming messages
uint8_t rfFrom;												// address of sender for the last incoming message
time_t  rfMessageReceived = now();							// time the last incoming message was received

// Channel info
const uint8_t num_channels = 126;
uint8_t values[num_channels];
const int num_reps = 100;

// A flag to indicate a generated alert interrupt
volatile bool RF24Flag = false;

void StartRF24()
{
	bool result = true;

	// Defaults after init are 2.402 GHz (channel 2), 2Mbps, 0dBm
	if (!rfManager.init())
	{
		Serial.println(F("RF24 Radio init failed"));
		result = false;
	}

	if (!rfDriver.setChannel(RF24_CHANNEL))
	{
		Serial.println(F("RF24 Radio set channel failed"));
		result = false;
	}

	if (!rfDriver.setRF(RF24_DATA_RATE, RF24_TX_POWER))
	{
		Serial.println(F("RF24 Radio set rate and power failed"));
		result = false;
	}

	rfManager.setRetries(RF24_RETRIES);
	rfManager.setTimeout(RF24_LISTEN_TIMEOUT);
	rfDriver.setHeaderFrom(rfManager.thisAddress());
	rfMessageReceived = now();

	if (result)
		Serial.println(F("RF24 Radio startup OK"));
	else
		Serial.println(F("RF24 Radio startup failed"));

	/*
	radio.begin();
	radio.setAutoAck(false);

	// Get into standby mode
	radio.startListening();
	radio.stopListening();

	// Print out header, high then low digit
	int i = 0;
	while (i < num_channels)
	{
		Serial << F("%") << (i >> 4);
		++i;
	}
	Serial.println();
	i = 0;
	while (i < num_channels)
	{
		Serial << F("%") << (i & 0xf);
		++i;
	}
	Serial.println();
	*/
}

void ProcessRF24()
{
	/*
	// Clear measurement values
	memset(values, 0, sizeof(values));

	// Scan all channels num_reps times
	int rep_counter = num_reps;
	while (rep_counter--)
	{
		int i = num_channels;
		while (i--)
		{
			// Select this channel
			radio.setChannel(i);

			// Listen for a little
			radio.startListening();
			delayMicroseconds(225);


			// Did we get a carrier?
			if (radio.testCarrier()) {
				++values[i];
			}
			radio.stopListening();
		}
	}

	// Print out channel measurements, clamped to a single hex digit
	int i = 0;
	while (i < num_channels)
	{
		Serial << F("%") << (min(0xf, values[i] & 0xf));
		++i;
	}
	Serial.println();

	/*
	if (RF24Flag)
	{
	RF24Flag = false;
	// do stuff
	}
	delay(1);
	*/
}

void RF24InterruptHandler()
{
	RF24Flag = true;
}

#else
void StartRF24() { ; }
void ProcessRF24() { ; }
#endif

