/*
  ASesnor - Arduino sesnor library 
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
  https://github.com/AndreiDegtiarev/ASensor

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#pragma once

#include "OneWireSensor.h"


class DHTSensor : public OneWireSensor
{
public:
	enum ReadType
	{
		Temperature,
		Humidity
	};
private:
	DHT *_dht;
	DHTSensor *_srcSensor;
	ReadType _readType;
	float _last_temperature;
	float _last_humidity;
public:
	DHTSensor(DHT::DHT_MODEL_t model,ReadType readType,int port,float low_limit,float high_limit,unsigned long pause_length):OneWireSensor(port,low_limit,high_limit,readType==Temperature?1:0,pause_length)
	{
		_dht=new DHT();
		_dht->setup(port,model);
		_srcSensor=NULL;
		initialize(readType);
	}
	DHTSensor(ReadType readType,DHTSensor *src_sensor,float low_limit,float high_limit,unsigned long pause_length):OneWireSensor(0,low_limit,high_limit,readType==Temperature?1:0,pause_length)
	{
		_dht=NULL;
		_srcSensor=src_sensor;
		initialize(readType);
	}
protected:
	void initialize(ReadType readType)
	{
		Name=F("DHT");
		_readType=readType;
		_last_temperature=0;
		_last_humidity=0;
	}
public:
	virtual void measure()
	{
		DHT::DHT_ERROR_t status= DHT::ERROR_NONE;
		if(_dht!=NULL)
		{
			_last_temperature = _dht->getTemperature();
			_last_humidity = _dht->getHumidity();
			status=_dht->getStatus();
		}
		else if(_srcSensor!=NULL)
		{
			_last_temperature = _srcSensor->_last_temperature;
			_last_humidity = _srcSensor->_last_humidity;
		}
		/*Serial.print("Status: ");
		Serial.print(_dht->getStatusString());
		Serial.print(" Tempr: ");
		Serial.print(_last_temperature);
		Serial.print(" Humidity: ");
		Serial.println(_last_humidity);*/
		if(status == DHT::ERROR_NONE)
		{
			if(_readType == Temperature && !isnan(_last_temperature))
				SetData(_last_temperature);
			else if(_readType == Humidity && !isnan(_last_humidity))
				SetData(_last_humidity);
		}
		//delay(_dht->getMinimumSamplingPeriod()); 
	}
};