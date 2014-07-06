#pragma once

#include "ISensor.h"
#include "TimeSerieBuffer.h"

class VoltmeterSensor : ISensor
{
	int _port;
	TimeSerieBuffer<int> *_dataBuffer;
public:
	VoltmeterSensor(int port,int buffer_size)
	{
		_port=port;
		_dataBuffer=new TimeSerieBuffer<int>(1,1023/5.0,buffer_size);
		pinMode(port,INPUT);
	}
	TimeSerieBuffer<int> *Buffer()
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
	virtual bool Measure(float &data)
	{
		data=analogRead(_port);
		return true;
	}
	void MeasureBuffer(int sample_ratio)
	{
		int size=_dataBuffer->Size();
		int delay_time=(int)(1e6*1.0/sample_ratio);
		_dataBuffer->SetTimeStep(delay_time/1.0e6);
		//Serial.println("Measurement started");
		int * data_y=_dataBuffer->Y();
		for(int i=0;i<size;i++)
		{
			data_y[i]=analogRead(_port);
			delayMicroseconds(delay_time);
		}
		//Serial.println("Measurement finished");
	}

};