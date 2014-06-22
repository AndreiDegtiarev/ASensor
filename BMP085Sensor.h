/*
  ASesnor - Arduino sesnor library 
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
  https://github.com/AndreiDegtiarev/ASensor

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#pragma once

#include "OneWireSensor.h"

class BMP085Sensor : public OneWireSensor
{
	Adafruit_BMP085_Unified *_bmp ;
public:
	BMP085Sensor(float low_limit,float high_limit,unsigned long pause_length):OneWireSensor(10085,low_limit,high_limit,0,pause_length)
	{
		Name=F("BMP085");
 		_bmp = new Adafruit_BMP085_Unified(_port);
		if(!_bmp->begin())
		{
			/* There was a problem detecting the BMP085 ... check your connections */
			Serial.print(F("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!"));
		}
	}
	virtual void measure()
	{
		 sensors_event_t event;
		 _bmp->getEvent(&event);
		 if (event.pressure)
		 {
			SetData(event.pressure);
		 }
	}
};