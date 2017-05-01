// ---------------------------------------------------------------------------------
#ifdef IO_BATTERY

#define BATTERY_SAMPLES             3         // Number of samples per analog read
#define BATTERY_SAMPLE_DELAY        10        // Delay between samples in msec
#define BATTERY_REFERENCE_VOLTAGE   5.015     // use 5.0 for a 5.0V ADC reference voltage
#define BATTERY_DIVIDER_RATIO       2.0       // Ratio of the volatage divider (measure with multimeter)
#define BATTERY_SENSE_PIN           A0        // Pin for the battery sensor voltage divider
#define BATTERY_FULL                4.5       // Battery full voltage
#define BATTERY_EMPTY               1.0       // Battery empty voltage
#define BATTERY_RANGE               3.5       // Range between batter full and empty (in volts)

// ---------------------------------------------------------------------------------
void IO_Battery_Start()
{

}
// ---------------------------------------------------------------------------------
uint8_t IO_Battery_Status()
{
  uint16_t sum = 0;               // sum of samples taken
  float voltage = 0.0;            // calculated voltage
  float soc = 0.0;                // percent of battery charge remaining

  // take a number of analog samples and add them up
  for ( uint8_t c = 0; c < BATTERY_SAMPLES; c++ )
  {
    sum += analogRead ( BATTERY_SENSE_PIN );
    delay ( BATTERY_SAMPLE_DELAY );
  }

  // calculate the voltage and state of charge (rounded to the nearest 10%)
  voltage = ( ( float ) sum / ( float ) BATTERY_SAMPLES * 5.015 ) / 1024.0 * BATTERY_DIVIDER_RATIO;
  soc = round ( ( voltage - BATTERY_EMPTY ) / BATTERY_RANGE * 10 ) * 10;
  soc = constrain ( soc , 0, 100 );

  return ( uint8_t ) soc;
}
#endif

