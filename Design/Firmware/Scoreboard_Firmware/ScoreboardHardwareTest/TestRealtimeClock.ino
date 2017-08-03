#ifdef TEST_REALTIMECLOCK

void StartRTC()
{

}

void ProcessRTC()
{
	tmElements_t tm;

	if (RTC.read(tm)) 
	{
		Serial.print(F("RealtimeClock OK: "));
		print2digits(tm.Hour);
		Serial.write(':');
		print2digits(tm.Minute);
		Serial.write(':');
		print2digits(tm.Second);
		Serial.print(F(", Date (D/M/Y) = "));
		Serial.print(tm.Day);
		Serial.write('/');
		Serial.print(tm.Month);
		Serial.write('/');
		Serial.print(tmYearToCalendar(tm.Year));
		Serial.println();
	}
	else 
	{
		if (RTC.chipPresent()) 
			Serial.println(F("RealtimeClock is stopped. Please run the SetTime utility"));
		else
			Serial.println(F("RealtimeClock read error! Please check the circuitry."));
	}
}

void RTCSleep()
{

}

void print2digits(int number) {
	if (number >= 0 && number < 10) {
		Serial.write('0');
	}
	Serial.print(number);
}

#else
void StartRTC() { ; }
void ProcessRTC() { ; }
void RTCSleep() { ; }
#endif
