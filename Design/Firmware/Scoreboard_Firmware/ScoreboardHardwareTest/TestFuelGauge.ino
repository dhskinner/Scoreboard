#ifdef TEST_FUELGAUGE

// LiFuelGauge constructor parameters
// 1. IC type, MAX17043 or MAX17044
// 2. Number of interrupt to which the alert pin is associated (Optional) 
// 3. ISR to call when an alert interrupt is generated (Optional)
// Creates a LiFuelGauge instance for the MAX17043 IC and attaches lowPower to INT0 (PIN2 on most boards, PIN3 on Leonardo)
LiFuelGauge gauge(MAX17043, 0, FuelGaugeInterruptHandler);

// A flag to indicate a generated alert interrupt
volatile boolean lowPowerFlag = false;

void StartFuelGauge()
{
	gauge.reset();  // Resets MAX17043
	delay(200);		// Waits for the initial measurements to be made

	// Sets the Alert Threshold to 10% of full capacity
	gauge.setAlertThreshold(10);
	Serial.println(millis() + String(F("\tLIPO alert threshold is set to ")) + gauge.getAlertThreshold() + F("%"));
}

void ProcessFuelGauge()
{
	Serial.print(millis());
	Serial.print(F("\tLIPO SOC: "));
	Serial.print(gauge.getSOC());  // Gets the battery's state of charge
	Serial.print(F("%, VCELL: "));
	Serial.print(gauge.getVoltage());  // Gets the battery voltage
	Serial.println(F(" Volts"));

	if (lowPowerFlag || 
		(gauge.getSOC() < 10.0 && gauge.getVoltage() < 3.1) )
	{
		Serial.print(millis());
		Serial.println(F("\tWarning, low power - entering sleep mode"));
		gauge.clearAlertInterrupt();  // Resets the ALRT pin
		lowPowerFlag = false;
		//Sleep();
	}
}

void FuelGaugeSleep()
{
	gauge.sleep();  // Forces the MAX17043 into sleep mode
}

void FuelGaugeInterruptHandler()
{
	lowPowerFlag = true;
}

#else
void StartFuelGauge() { ; }
void ProcessFuelGauge() { ; }
void FuelGaugeSleep() { ; }
#endif

