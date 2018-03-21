#pragma once
/*
ASensor - Sensor library. Can be used standalone or in conjunction with AWind library
Copyright (C) 2015-2018 Andrei Degtiarev. All right reserved

You can always find the latest version of the library at
https://github.com/AndreiDegtiarev/ASensor

This library is free software; you can redistribute it and/or
modify it under the terms of the MIT license.
Please see the included documents for further information.
*/
///Wrapper for MAX6675 thermocouple sensor. To use this sensor Adafruit MAX6675 library has to be installed. Details to member functions see ISensor class documentation
class MAX6675Thermocouple : public ISensor
{
#ifndef DEMO_SENSORS
	MAX6675 *_thermocouple;
#endif
public:
	MAX6675Thermocouple(int thermoCLK, int thermoCS, int thermoDO)
	{
#ifndef DEMO_SENSORS
		_thermocouple = new MAX6675(thermoCLK, thermoCS, thermoDO);
#endif
	}
	virtual const __FlashStringHelper* Name()
	{
		return F("MAX6675");
	}
	virtual float LowMeasurementLimit()
	{
		return 0;
	}
	virtual float HighMeasurementLimit()
	{
		return 400;
	}
	virtual int Precission()
	{
		return 0;
	}
	virtual bool Measure(float &data)
	{
#ifdef DEMO_SENSORS
		data = (float)rand() / RAND_MAX * 100 + 100;
		return true;
#else
		data = _thermocouple->readCelsius();
#ifdef DEBUG_AWIND
			//out << F("MAX6675: ") << data << endln;
#endif
		return true;
#endif
	}
};