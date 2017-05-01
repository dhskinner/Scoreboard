/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */
 
/**
 * @file printf.h
 *
 * Setup necessary to direct stdout to the Arduino Serial library, which
 * enables 'printf'
 */
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <Arduino.h>
#include <Time.h>
#include <Streaming.h>
#include "printf.h"

#ifdef DEBUG
// ---------------------------------------------------------------------------------
void print2Hex( uint8_t * buf, uint8_t len) 
{
  for (uint8_t i = 0; i < len; i++)
    printf("%3i ", buf[i] );
}

// ---------------------------------------------------------------------------------
void print2Digits(uint8_t number) 
{
  if (number >= 0 && number < 10) 
  {
    Serial.write('0');
  }
  Serial.print(number);
}
// ---------------------------------------------------------------------------------
void print3Digits(uint8_t number) 
{
  if (number >= 0 && number < 100) 
  {
    Serial.write('0');
  }
  if (number >= 0 && number < 10) 
  {
    Serial.write('0');
  }
  Serial.print(number);
}
// ---------------------------------------------------------------------------------
void printTime(time_t tm)
{
  print2Digits(hour(tm));
  Serial.print(F(":"));
  print2Digits(minute(tm));
  Serial.print(F(":"));
  print2Digits(second(tm));  
  Serial << F(" ") << day(tm) << F("/") << month(tm) << F("/") << year(tm) << F(" (weekday ") << weekday(tm) << ")\n\r";  
}
#endif

#endif // __UTILITY_H__
