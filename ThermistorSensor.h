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
///Thermistor sensor. No any external libraries are neccesary. Details to member functions see ISensor class documentation

class ThermistorSensor : public ISensor
{
	int _port;
public:
	ThermistorSensor(int port)
	{
		_port=port;
	}
	const __FlashStringHelper* Name()
	{
		return F("Thermistor");
	}
	float LowMeasurementLimit()
	{
		return -100;
	}
	float HighMeasurementLimit()
	{
		return 5000;
	}
	int Precission()
	{
		return 3;
	}
	bool Measure(float &data)
	{
		data = analogRead(_port) / 1023.0;
		return true;
	}
};