void ScanI2C()
{
	byte error, address;
	int nDevices;

	Serial.println(F("Scanning I2C..."));

	nDevices = 0;
	for (address = 1; address < 127; address++)
	{
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0)
		{
			Serial.print(F("I2C device found at address 0x"));
			if (address < 16)
				Serial.print(F("0"));
			Serial.print(address, HEX);
			Serial.print(F(":\t"));

			// Known devices
			switch (address)
			{
			case 0x20:
				Serial.println(F("PCF8574 IO expander"));
				break;
			case 0x36:
				Serial.println(F("LIPO fuel gauge"));
				break;
			default:
				Serial.println(F("unidentified device"));
				break;
			}

			nDevices++;
		}
		else if (error == 4)
		{
			Serial.print(F("Unknown error at address 0x"));
			if (address < 16)
				Serial.print(F("0"));
			Serial.println(address, HEX);
		}
	}
	if (nDevices == 0)
		Serial.println(F("No I2C devices found"));
	else
		Serial.println(F("Finished I2C scan"));
}



