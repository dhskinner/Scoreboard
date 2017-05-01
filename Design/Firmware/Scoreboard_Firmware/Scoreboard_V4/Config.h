#ifndef __CONFIG_H__
#define __CONFIG_H__

// ---------------------------------------------------------------------------------
// Hardware configuration (see below for role specific config)
// ---------------------------------------------------------------------------------
// Mode
#define SCOREBOARD_CONTROLLER		// Define one of SCOREBOARD_MASTER, SCOREBOARD_SLAVE, or SCOREBOARD_CONTROLLER
#define DMD_ROLE				2	// If defined as a slave, you also need to specify which type of slave:
									// 0 = test config with a single DMD panel (only displays field 1)
									// 1 = left hand side (2 panels wide x 4 high)
									// 2 = right hand side (2 panels wide x 4 high)
// Test mode
#ifndef SCOREBOARD_SLAVE
  #define TEST_  				// Enter test mode (or turn this off with TEST_OFF)
#endif

// Fields
#define FIELDS                  2  // number of fields supported by this controller (0-255 as limited by RAM) 
#define FIELD_ID                0  // for multi fields going to a single display, which field do you want to show? (0 or 1)

// Serial Baud Rates
#define ULCD_SERIAL_BAUD        115200

// ---------------------------------------------------------------------------------
// Role configuration
// ---------------------------------------------------------------------------------

#ifdef SCOREBOARD_MASTER
  #define NANO                      // UNO, UNO_SHIELD, NANO, MEGA or ETHERMEGA
  #define I2C_TRANSFER
  #define I2C_RTC
  #define RF_RADIOHEAD           
  const uint8_t thisRFAddress = RF24_SCOREBOARD_ADDRESS;  
  #define MASTER_STARTUP_DELAY	3000 // startup delay when the master is firing up
  
#elif defined(SCOREBOARD_SLAVE)
  #define NANO                      // UNO, UNO_SHIELD, NANO, MEGA or ETHERMEGA
  #define LED_DMD2
  #define I2C_TRANSFER
  #define SLAVE_TEST_PATTERN_DELAY	30  // delay to control timing of the DMD test pattern
    
  #if (DMD_ROLE == 0)
    #define LED_SEGMENT
  #endif
  
  // Set up the DMD panels depending on DMD_ROLE:
  // 0 = test config with a single DMD panel (only displays field 1)
  // 1 = left hand side (2 panels wide x 4 high)
  // 2 = right hand side  (2 panels wide x 4 high)
  #if (DMD_ROLE == 0)
	#define DMD_DISPLAYS_ACROSS		1
	#define DMD_DISPLAYS_DOWN		1
	#define DMD_PIXELS_ACROSS		32
	#define DMD_PIXELS_DOWN			16
	#define DMD_NUMERAL_GAP			2
  #else
	#define DMD_DISPLAYS_ACROSS		2
	#define DMD_DISPLAYS_DOWN		4
	#define DMD_PIXELS_ACROSS		64
	#define DMD_PIXELS_DOWN			64
	#define DMD_NUMERAL_GAP			2
  #endif
   
#elif defined(SCOREBOARD_CONTROLLER)
  #define NANO //MEGA_                     // UNO, UNO_SHIELD, NANO, MEGA or ETHERMEGA
  #define ULCD
  #define RF_RADIOHEAD  
  #define IO_BATTERY         
  const uint8_t thisRFAddress = RF24_CONTROLLER_ADDRESS;
#endif

// ---------------------------------------------------------------------------------
// I2C Address config
// ---------------------------------------------------------------------------------
#define I2C_SLAVE_ADDRESS_DMD_0   9		//define slave i2c address for DMD 8 (equals 08 plus the R/W bit at LSB set on)
#define I2C_SLAVE_ADDRESS_DMD_1   9		//define slave i2c address for DMD 1 (equals 08 plus the R/W bit at LSB set on)
#define I2C_SLAVE_ADDRESS_DMD_2  11		//define slave i2c address for DMD 2 (equals 10 plus the R/W bit at LSB set on)

// ---------------------------------------------------------------------------------
// RF24 SPI Pin config
// ---------------------------------------------------------------------------------
#if defined(UNO_SHIELD) 
  const uint8_t RF24_CE =       14;       // equals analog A0
  const uint8_t RF24_CS =       15;       // equals analog A1
  const uint8_t spi[] = {       9, 10 };  // SPI pins
  
#elif defined(UNO)
  const uint8_t ULCD_RESETLINE = 4;       // change this if you are not using an Arduino Adaptor Shield Version 2 
  const uint8_t RF24_CE =       9;
  const uint8_t RF24_CS =       10;
  const uint8_t spi[] = {       9, 10 };  // SPI pins

#elif defined(NANO)
  const uint8_t ULCD_RESETLINE = 2;        
  const uint8_t RF24_CE =       9;
  const uint8_t RF24_CS =       10;
  const uint8_t spi[] = {       9, 10 };  // SPI pins
  
#elif defined(MEGA)
  const uint8_t ULCD_RESETLINE = 4;        
  const uint8_t RF24_CE =       9;
  const uint8_t RF24_CS =       10;
  const uint8_t spi[] = {       9, 10 };  // SPI pins

#elif defined(ETHERMEGA)
  const uint8_t ULCD_RESETLINE = 4;        
  const uint8_t RF24_CE =       40;
  const uint8_t RF24_CS =       42;
  const uint8_t spi[] = {       4,        // micro SD
                                6,        // bank0
                                7,        // bank1
                                10,       // wiznet
                                53  };    // atmega2560 SS line
#endif

// ---------------------------------------------------------------------------------
#endif
