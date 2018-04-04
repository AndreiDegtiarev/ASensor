/*
ASensor - Sensor library. Can be used standalone or in conjunction with AWind library
Copyright (C) 2015-2018 Andrei Degtiarev. All right reserved

You can always find the latest version of the library at
https://github.com/AndreiDegtiarev/ASensor

This library is free software; you can redistribute it and/or
modify it under the terms of the MIT license.
Please see the included documents for further information.
*/
#pragma once
#include "VoltmeterSensor.h"
class CurrentmeterSensor : public ISensor
{
	int _voltageProbeOnePort;
	int _voltageProbeTwoPort;
	float _resistance;
	float _baseVoltage;
public:
	///Constructor
	/**
	\param voltageProbeOnePort analogue pin where first voltage should be measured
	\param voltageProbeTwoPort analogue pin where second voltage should be measured
	\param resistance resistance of power resistor
	\param baseVoltage voltage of arduino board
	*/
	CurrentmeterSensor(int voltageProbeOnePort, int voltageProbeTwoPort, float resistance,float baseVoltage):
		_voltageProbeOnePort(voltageProbeOnePort),
		_voltageProbeTwoPort(voltageProbeTwoPort),
		_resistance(resistance),
		_baseVoltage(baseVoltage)
	{
#ifndef ESP32
		pinMode(_voltageProbeOnePort, INPUT);
		pinMode(_voltageProbeTwoPort, INPUT);
#endif
	}
	///Returns internal sensors name. Usefull for debugging
	virtual const __FlashStringHelper* Name()
	{
		return F("Currentmeter");
	}
	///Return how many decimal places make sence for the sensor
	virtual int Precission()
	{
		return 3;
	}
	///Return lowest possible measurement limit. If value outside of this limit, measurements treated as erroneous
	virtual float LowMeasurementLimit()
	{
		return -10;
	}
	///Return highest possible measurement limit. If value outside of this limit, measurements treated as erroneous
	virtual float HighMeasurementLimit()
	{
		return 10;
	}
	///Performs measurements 
	/**
	\param data reference to the variable that becoms result measurement
	\return true if success
	*/
	virtual bool Measure(float &data)
	{
		VoltmeterSensor::InitMeasurement(_voltageProbeOnePort);
		auto voltage1 = VoltmeterSensor::MeasureRaw(_voltageProbeOnePort) * _baseVoltage / 1023;
		VoltmeterSensor::InitMeasurement(_voltageProbeTwoPort);
		auto voltage2 = VoltmeterSensor::MeasureRaw(_voltageProbeTwoPort) * _baseVoltage / 1023;
		data = (voltage2 - voltage1) / _resistance;
		out << "voltage1:" << voltage1 << " voltage2:" << voltage2 << " current:" << data << endln;
	}
};