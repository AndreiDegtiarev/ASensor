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


class DHTHumiditySensor : public ISensor
{

	DHTTemperatureSensor *_srcSensor;
public:

	DHTHumiditySensor(DHTTemperatureSensor *src_sensor)
								/*		:ISensor(   readType==Temperature?-50:1,
													   readType==Temperature?50:100,
													   low_application_limit,
													   hight_application_limit,
													   readType==Temperature?1:0,pause_length) */
	{
		_srcSensor = src_sensor;
	}

public:
	virtual const __FlashStringHelper* Name()
	{
		return F("DHT Humidity");
	}
	virtual float LowMeasurementLimit()
	{
		return 1;
	}
	virtual float HighMeasurementLimit()
	{
		return 100;
	}
	virtual int Precission()
	{
		return 0;
	}
	virtual bool Measure(float &data)
	{
		data=_srcSensor->LastHumidity();
		return _srcSensor->IsOK();
	}
};
