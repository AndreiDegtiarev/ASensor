#pragma once
/*
ASensor - Sensor library. Can be used standalone or in conjunction with AWind library
Copyright (C)2016 Andrei Degtiarev. All right reserved

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

public:
	MPXPressureSensor(int port)
	{
		_port = port;
	}
	const __FlashStringHelper* Name()
	{
		return F("MPXPressure_Bar");
	}
	float LowMeasurementLimit()
	{
		return 0;
	}
	float HighMeasurementLimit()
	{
		return 1024;
	}
	int Precission()
	{
		return 0;
	}
	bool Measure(float &data)
	{
		float sensorVoltage = analogRead(_port)*5.0 / 1023.0;
		data = (sensorVoltage-0.2)/0.009/100.0; //Bar
		return true;
	}
};