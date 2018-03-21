/*
This file is part of ASensor library

Copyright (c) 2014-2018 Andrei Degtiarev

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this file except in compliance with the License.  You may
obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.
*/
#pragma once
class CurrentmeterSensor : public ISensor
{
	int _voltageProbeOnePort;
	int _voltageProbeTwoPort;
	float _resistance;
	float _baseVoltage;
public:
	///Constructor
	/**
	\param port analogue pin where signal should measured
	\param reserved_buffer_size max possible (reserved) buffer size
	\param actual_size initial buffer size
	*/
	CurrentmeterSensor(int voltageProbeOnePort, int voltageProbeTwoPort, float resistance,float baseVoltage):
		_voltageProbeOnePort(voltageProbeOnePort),
		_voltageProbeTwoPort(voltageProbeTwoPort),
		_resistance(resistance),
		_baseVoltage(baseVoltage)
	{
		pinMode(_voltageProbeOnePort, INPUT);
		pinMode(_voltageProbeTwoPort, INPUT);
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
		auto voltage1 = analogRead(_voltageProbeOnePort) * _baseVoltage / 1023;
		auto voltage2 = analogRead(_voltageProbeTwoPort) * _baseVoltage / 1023;
		data = (voltage2 - voltage1) / _resistance;
		//out << "voltage1:" << voltage1 << " voltage2:" << voltage2 << " current:" << data << endln;
	}
};