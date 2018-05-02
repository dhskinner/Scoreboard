#define I2C_PCF8574_UPDATE_MILLIS	100
#define I2C_PCF8574_BUTTON_QTY		5

uint8_t lastByte;

#ifdef TEST_PCF8574_BUTTONS

PCF8574 pcf8574(I2C_PCF8574_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN, Wire);

unsigned long pcf8574_update_millis = 0;

void StartPCF()
{
	pcf8574_update_millis = millis();
	lastByte = pcf8574.read8();
}

void ProcessPCF()
{
	if (millis() > pcf8574_update_millis)
	{
		pcf8574_update_millis += I2C_PCF8574_UPDATE_MILLIS;
		CheckPCF();
	}
}

#elif defined (TEST_PCF8574_BUTTONS_INTERRUPT)

PCF8574 pcf8574(I2C_PCF8574_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN, Wire);

volatile bool PCFChanged = false;	// A flag to indicate a generated alert interrupt

void StartPCF()
{
	// Most ready-made PCF8574 - modules seem to lack an internal pullup-resistor, so you have to use the ESP8266 - internal one or else it won't work
	pinMode(I2C_PCF8574_INTERUPT, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(I2C_PCF8574_INTERUPT), PCFInterruptHandler, CHANGE);
	lastByte = pcf8574.read8();
}

void PCFInterruptHandler()
{
	PCFChanged = true;
}

void ProcessPCF()
{
	if (PCFChanged)
	{
		CheckPCF();
		PCFChanged = false;
	}
}

#else
void StartPCF() { ; }
void ProcessPCF() { ; }
#endif

uint8_t pcfState = 0;

void CheckPCF()
{
	uint8_t newByte;
	uint8_t newBit;

	newByte = pcf8574.read8();

	if (newByte != lastByte)
	{
		Serial << millis() << F("\tPCF8574 was ") << printByte(lastByte) << F(" is ") << (newBit ? F("pressed") : F("released")) << endl;

		for (uint8_t i = 0; i < I2C_PCF8574_BUTTON_QTY; i++)
		{
			newBit = bitRead(newByte, i);

			if (newBit != bitRead(lastByte, i))
			{
				Serial << millis() << F("\tButton ") << i << F(" is ") << (newBit ? F("pressed") : F("released")) << endl;
				
				switch (i)
				{
				case 0:
					pcf8574.write(LED_RED_BIT, newBit ? LED_ON : LED_OFF);
					break;
				case 1:
					pcf8574.write(LED_GREEN_BIT, newBit ? LED_ON : LED_OFF);
					break;
				case 2:
					pcf8574.write(LED_BLUE_BIT, newBit ? LED_ON : LED_OFF);
					break;
				case 3:
				case 4:
					pcf8574.write(LED_RED_BIT, newBit ? LED_ON : LED_OFF);
					pcf8574.write(LED_GREEN_BIT, newBit ? LED_ON : LED_OFF);
					pcf8574.write(LED_BLUE_BIT, newBit ? LED_ON : LED_OFF);
					break;
				default:
					pcf8574.write(LED_RED_BIT, LED_OFF);
					pcf8574.write(LED_GREEN_BIT, LED_OFF);
					pcf8574.write(LED_BLUE_BIT, LED_OFF);
					break;
				}
			}
		}
		lastByte = newByte;
	}
}

void ProcessPCFTicker() { ; }
