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



class DHTTemperatureSensor : public ISensor
{
private:
#ifndef DEMO_SENSORS
	DHT *_dht;
#endif
	float _last_temperature;
	float _last_humidity;
	bool _isOK;
public:
	DHTTemperatureSensor(int port)
									/*	 :ISensor(port,
														readType==Temperature?-50:1,
														readType==Temperature?50:100,
														low_application_limit,
														hight_application_limit,
														readType==Temperature?1:0,pause_length)*/
	{
#ifndef DEMO_SENSORS
		_dht=new DHT();
		_dht->setup(port);
#endif
		_last_humidity=0;
		_isOK=false;
	}

public:
	virtual const __FlashStringHelper* Name()
	{
		return F("DHT Temperature");
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
	float LastHumidity()
	{
		return _last_humidity;
	}
	bool IsOK()
	{
		return _isOK;
	}
	virtual bool Measure(float &data)
	{
		_isOK=false;
#ifdef DEMO_SENSORS
		data =(float)rand()/RAND_MAX*5+20;
		_last_humidity = (float)rand()/RAND_MAX*10+60;
		_isOK=true;
#else
		DHT::DHT_ERROR_t status= DHT::ERROR_NONE;
		if(_dht!=NULL)
		{
			data = _dht->getTemperature();
			_last_humidity = _dht->getHumidity();
			status=_dht->getStatus();
		}
		/*Serial.print("Status: ");
		Serial.print(_dht->getStatusString());
		Serial.print(" Tempr: ");
		Serial.print(_last_temperature);
		Serial.print(" Humidity: ");
		Serial.println(_last_humidity);*/
		_isOK=status == DHT::ERROR_NONE && !isnan(_last_temperature) && !isnan(_last_humidity);
		//delay(_dht->getMinimumSamplingPeriod()); 
#endif
		return _isOK;
	}
};
