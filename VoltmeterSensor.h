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
#include "ISensor.h"
#include "TimeSerieBuffer.h"
#ifdef ESP32
#include <driver/adc.h>
#endif

///Voltmeter sensor. No any external libraries are neccesary. Allows measurements not only single value but time serie as well. Details to member functions see ISensor class documentation
class VoltmeterSensor : public ISensor
{
	int _port;
	TimeSerieBuffer *_dataBuffer;
	int _time_step_mus;
	float _voltageReference;
	float _offset;
public:
	///Constructor
	/**
	\param port analogue pin where signal should measured
	\param reserved_buffer_size max possible (reserved) buffer size
	\param actual_size initial buffer size
	*/
	VoltmeterSensor(int port,int reserved_buffer_size,int actual_size,float voltageReference = 5.0,float offest = 0):
		_voltageReference(voltageReference),
		_offset(offest)
	{
		_port=port;
		_dataBuffer=new TimeSerieBuffer(1,1023/ voltageReference,reserved_buffer_size,actual_size, offest);
#ifndef ESP32
		pinMode(port,INPUT);
#endif
		_time_step_mus=100;
	}
	/**
	\param port analogue pin where signal should measured
	*/
	VoltmeterSensor(int port,float voltageReference = 5.0, float offest = 0) :
		_voltageReference(voltageReference),
		_offset(offest)
	{
		_port = port;
		_dataBuffer = NULL;
		pinMode(port, INPUT);
		_time_step_mus = 100;
	}
	///return buffer contains measured data
	TimeSerieBuffer *Buffer()
	{
		return _dataBuffer;
	}
	///Returns internal sensors name. Usefull for debugging
	virtual const __FlashStringHelper* Name()
	{
		return F("Voltmeter");
	}
	///Return lowest possible measurement limit. If value outside of this limit, measurements treated as erroneous
	virtual float LowMeasurementLimit()
	{
		return -50;
	}
	///Return highest possible measurement limit. If value outside of this limit, measurements treated as erroneous
	virtual float HighMeasurementLimit()
	{
		return 50;
	}
	///Return how many decimal places make sence for the sensor
	virtual int Precission()
	{
		return 2;
	}
	///Sets sample time between measurements [microseconds]
	void SetTimeStep(int time_step_us)
	{
		_time_step_mus=time_step_us;
	}
	///Returns time step [microseconds]
	float TimeStep()
	{
		return _time_step_mus;
	}
	///Returns time serie length [seconds]
	float TimeLength()
	{
		return (_time_step_mus*_dataBuffer->Size())/1e6;
	}
	///Performs measurements 
	/**
	\param data reference to the variable that becoms result measurement
	\return true if success
	*/
	virtual bool Measure(float &data)
	{
		InitMeasurement(_port);
		data = MeasureRaw(_port) * _voltageReference / 1023 + _offset;
		return true;
	}
	///Performs measurements of time serie
	void MeasureBuffer()
	{
		InitMeasurement(_port);
		int size=_dataBuffer->Size();
		_dataBuffer->SetTimeStep(_time_step_mus/1.0e6);
		int * data_y=_dataBuffer->Y();
		for(int i=0;i<size;i++)
		{
			data_y[i]= MeasureRaw(_port);
			delayMicroseconds(_time_step_mus);
		}
	}
	static void inline InitMeasurement(int port)
	{
#ifdef ESP32
		adc1_config_width(ADC_WIDTH_BIT_10);
		adc1_config_channel_atten((adc1_channel_t)port, ADC_ATTEN_DB_11);
#endif

	}
	static int inline MeasureRaw(int port)
	{
#ifdef ESP32
		return adc1_get_raw((adc1_channel_t)port);
#else
		return analogRead(port);
#endif

	}

};