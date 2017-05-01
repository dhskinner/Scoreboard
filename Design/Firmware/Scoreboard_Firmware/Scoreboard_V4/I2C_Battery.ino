// ---------------------------------------------------------------------------------
#ifdef I2C_BATTERY

// LiFuelGauge constructor parameters
// 1. IC type, MAX17043 or MAX17044
// 2. Number of interrupt to which the alert pin is associated (Optional)
// 3. ISR to call when an alert interrupt is generated (Optional)
//
// Creates a LiFuelGauge instance for the MAX17043 IC
// and attaches lowPower to INT0 (PIN2 on most boards, PIN3 on Leonardo)
LiFuelGauge gauge ( MAX17043, 0, I2C_Battery_LowPower );

// A flag to indicate a generated alert interrupt
volatile boolean batteryAlert = false;

// ---------------------------------------------------------------------------------
void I2C_Battery_Start()
{
  gauge.reset();					// Resets MAX17043
  delay ( 200 );					// Waits for the initial measurements to be made
  gauge.setAlertThreshold ( 10 );	// Sets the Alert Threshold to 10% of full capacity
}
// ---------------------------------------------------------------------------------
uint8_t I2C_Battery_Status()
{
  int soc = gauge.getSOC();
  return ( uint8_t ) ( constrain ( soc, 0, 100 ) );
}
// ---------------------------------------------------------------------------------
void I2C_Battery_LowPower()
{
  batteryAlert = true;
}
#endif
