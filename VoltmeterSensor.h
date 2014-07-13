#pragma once

#include "ISensor.h"
#include "TimeSerieBuffer.h"

class VoltmeterSensor : ISensor
{
	int _port;
	TimeSerieBuffer *_dataBuffer;
	int _time_step_mus;
public:
	VoltmeterSensor(int port,int reserved_buffer_size,int actual_size)
	{
		_port=port;
		_dataBuffer=new TimeSerieBuffer(1,1023/5.0,reserved_buffer_size,actual_size);
		pinMode(port,INPUT);
		_time_step_mus=100;
	}
	TimeSerieBuffer *Buffer()
	{
		return _dataBuffer;
	}
	virtual const __FlashStringHelper* Name()
	{
		return F("Voltmeter");
	}
	virtual float LowMeasurementLimit()
	{
		return -50;
	}
	virtual float HighMeasurementLimit()
	{
		return 50;
	}
	virtual int Precission()
	{
		return 1;
	}
	void SetTimeStep(int time_step_us)
	{
		_time_step_mus=time_step_us;
		//Log::Number("time_step:",_time_step_us,true);
	}
	float TimeStep()
	{
		return _time_step_mus;
	}
	float TimeLength()
	{
		return (_time_step_mus*_dataBuffer->Size())/1e6;
	}
	int SampleRatio()
	{
		if(_time_step_mus!=0)
			return 1.0e6/_time_step_mus;
		else
			return 0.0;
	}
	virtual bool Measure(float &data)
	{
		data=analogRead(_port);
		return true;
	}
	void MeasureBuffer()
	{
		int size=_dataBuffer->Size();
		_dataBuffer->SetTimeStep(_time_step_mus/1.0e6);
		//Serial.println("Measurement started");
		int * data_y=_dataBuffer->Y();
		for(int i=0;i<size;i++)
		{
			data_y[i]=analogRead(_port);
			delayMicroseconds(_time_step_mus);
		}
		//Serial.println("Measurement finished");
	}

};