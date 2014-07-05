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


class DHTTemperatureSensor : public ISensor
{
public:
	enum ReadType
	{
		Temperature,
		Humidity
	};
private:
#ifndef DEMO_SENSORS
	DHT *_dht;
#endif
	DHTSensor *_srcSensor;
	ReadType _readType;
	float _last_temperature;
	float _last_humidity;
public:
	DHTSensor(ReadType readType,int port,double low_application_limit, double hight_application_limit,unsigned long pause_length)
										 :OneWireSensor(port,
														readType==Temperature?-50:1,
														readType==Temperature?50:100,
														low_application_limit,
														hight_application_limit,
														readType==Temperature?1:0,pause_length)
	{
#ifndef DEMO_SENSORS
		_dht=new DHT();
		_dht->setup(port);
#endif
		_srcSensor=NULL;
		initialize(readType);
	}
	DHTSensor(ReadType readType,DHTSensor *src_sensor,float low_application_limit,float hight_application_limit,unsigned long pause_length)
										:OneWireSensor(0,
													   readType==Temperature?-50:1,
													   readType==Temperature?50:100,
													   low_application_limit,
													   hight_application_limit,
													   readType==Temperature?1:0,pause_length)
	{
#ifndef DEMO_SENSORS
		_dht=NULL;
#endif
		_srcSensor=src_sensor;
		initialize(readType);
	}
protected:
	void initialize(ReadType readType)
	{
		_readType=readType;
		_last_temperature=0;
		_last_humidity=0;
	}
public:
	virtual const __FlashStringHelper* Name()
	{
		return F("DHT");
	}

	virtual void Measure()
	{
#ifdef DEMO_SENSORS
		if(_readType == Temperature)
			SetData((float)rand()/RAND_MAX*5+20);
		else
			SetData((float)rand()/RAND_MAX*10+60);
#else
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
#endif
	}
};
