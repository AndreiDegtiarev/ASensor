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

///Timer class. Implement timer events.
class TimerSensorManager : public SensorManager , public ISensor
{
	uint32_t _msStart;                          //!< time when timer was started
	uint32_t _msInterval;                       //!< timer interval
	bool _isAutoreset;                          //!< defines whether timer event will begenerated only once or continiously
	bool _isEnabled;                            //!< defines whether timer is active
	bool _counter;
public:
	TimerSensorManager():SensorManager(this,0, 1,0,false)
	{
		_isAutoreset = false;
		_isEnabled = false;
		_counter = 0;
	}
	const __FlashStringHelper* Name()
	{
		return F("TimerSensor");
	}
	float LowMeasurementLimit()
	{
		return 0;
	}
	float HighMeasurementLimit()
	{
		return 3.4028235E+38;
	}
	int Precission()
	{
		return 0;
	}
	bool Measure(float &data)
	{
		if (_isEnabled && Elapsed() >= _msInterval)
		{
			_counter=!_counter; //force event message
			if (_isAutoreset)
				Enable();
			else
				Reset();
		}

		data = _counter;
		return true;
	}
	///Set whether timer is in autoreset modus
	void SetAutoReset(bool isAutoreset)
	{
		_isAutoreset = isAutoreset;
	}
	///Set timer interval in milliseconds
	void SetInterval(uint32_t milliseconds)
	{
		_msInterval = milliseconds;
	}
	///Returns true if timer is active
	bool IsEnabled()
	{
		return _isEnabled;
	}
	///Starts timer
	void Enable()
	{
		_isEnabled = true;
		_msStart = millis();
	}
	///Disabels timer
	void Reset()
	{
		_isEnabled = false;
	}
	///Returns actual elapsed time
	uint32_t Elapsed()
	{
		if (_isEnabled)
			return millis() - _msStart;
		else
			return 0;
	}
};