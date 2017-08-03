#ifdef TEST_PCF8574_WITHOUT_INTERRUPTS

PCF8574 pcf8574(I2C_PCF8574_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN, Wire);

void StartPCF()
{
}

void ProcessPCFTicker()
{
	uint8_t state;

	pcf8574.write(LED_RED_BIT, LED_OFF);
	pcf8574.write(LED_GREEN_BIT, LED_OFF);
	pcf8574.write(LED_BLUE_BIT, LED_OFF);

	switch (second() % 3)
	{
	case 0:
		pcf8574.write(LED_RED_BIT, LED_ON);
		break;
	case 1:
		pcf8574.write(LED_GREEN_BIT, LED_ON);
		break;
	case 2:
	default:
		pcf8574.write(LED_BLUE_BIT, LED_ON);
		break;
	}

	state = pcf8574.read8();

	for (uint8_t i = 0; i < 5; i++)
	{
		if (CheckBit(state, i))
			Serial << F("Button ") << i << F(" is pressed ") << endl;
	}
}

void PCFSleep()
{
	pcf8574.write(LED_RED_BIT, LED_OFF);
	pcf8574.write(LED_GREEN_BIT, LED_OFF);
	pcf8574.write(LED_BLUE_BIT, LED_OFF);
}

void ProcessPCFInterrupt() { ; }

bool CheckBit(uint8_t state, uint8_t bit)
{
	if (state & (1 << bit))
		return false;
	else
		return true;
}

#elif defined (TEST_PCF8574_WITH_INTERRUPTS)

#define BUTTON_QTY	5

PCF8574 pcf8574(I2C_PCF8574_ADDRESS, I2C_SDA_PIN, I2C_SCL_PIN, Wire);
Button button0 = {};
ButtonWithInterrupt button1(1);
ButtonWithInterrupt button2(2);
ButtonWithInterrupt button3(3);
ButtonWithInterrupt button4(4);
ButtonWithInterrupt *buttons[] = { &button0, &button1, &button2, &button3, &button4 };

// A flag to indicate a generated alert interrupt
volatile bool PCFChanged = false;

void StartPCF()
{
	// Most ready-made PCF8574 - modules seem to lack an internal pullup-resistor, so you have to use the ESP8266 - internal one or else it won't work
	pinMode(I2C_PCF8574_INTERUPT, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(I2C_PCF8574_INTERUPT), PCFInterruptHandler, CHANGE);
}

void ProcessPCFTicker()
{
	pcf8574.write(LED_RED_BIT, LED_OFF);
	pcf8574.write(LED_GREEN_BIT, LED_OFF);
	pcf8574.write(LED_BLUE_BIT, LED_OFF);

	switch (second() % 3)
	{
	case 0:
		pcf8574.write(LED_RED_BIT, LED_ON);
		break;
	case 1:
		pcf8574.write(LED_GREEN_BIT, LED_ON);
		break;
	case 2:
	default:
		pcf8574.write(LED_BLUE_BIT, LED_ON);
		break;
	}
}

void ProcessPCFUpdate()
{
	if (PCFChanged)
	{
		uint8_t newstate = pcf8574.read8();

		for (uint8_t i = 0; i < BUTTON_QTY; i++)
		{
			buttons[i]->update(newstate);

			if (buttons[i]->toggled())
				Serial << F("Button ") << buttons[i]->id() << F(" is ") << buttons[i]->pressed() << endl;
		}
		PCFChanged = false;
	}
}

void PCFSleep()
{
	pcf8574.write(LED_RED_BIT, LED_OFF);
	pcf8574.write(LED_GREEN_BIT, LED_OFF);
	pcf8574.write(LED_BLUE_BIT, LED_OFF);
}

void PCFInterruptHandler()
{
	PCFChanged = true;
}

#else
void StartPCF() { ; }
void PCFSleep() { ; }
void ProcessPCFTicker() { ; }
void ProcessPCFInterrupt() { ; }
#endif



