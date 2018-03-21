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
///MPX pressure seonsor sensor from Freescale Semiconductor. No any external libraries are neccesary. Details to member functions see ISensor class documentation
class MPXPressureSensor : public ISensor
{
	int _port;
	float _offset_volt;
	float _factor_volt;
public:
	MPXPressureSensor(int port, float offset_volt, float factor_volt)
	{
		_port = port;
		_offset_volt = offset_volt;
		_factor_volt = factor_volt;
	}
	const __FlashStringHelper* Name()
	{
		return F("MPXPressure_Bar");
	}
	float LowMeasurementLimit()
	{
		return -10;
	}
	float HighMeasurementLimit()
	{
		return 10;
	}
	int Precission()
	{
		return 3;
	}
	bool Measure(float &data)
	{
		float sensorVoltage = analogRead(_port)*5.0 / 1023.0;
		data = (sensorVoltage - _offset_volt) * _factor_volt; 
		return true;
	}
};