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

#include "LinkedList.h"

class MeasurementNode
{
	LinkedList<OneWireSensor> &_sensors;
	NRF24Transceiver *_radio;
	const __FlashStringHelper *_nodeID;
	void  (*_fncCritical)();
public:
	MeasurementNode(/*const __FlashStringHelper *nodeID,*/LinkedList<OneWireSensor> &sensors,void (*fncCritical)()):_sensors(sensors)
	{
		_radio= new NRF24Transceiver(8,9);
		//_nodeID = nodeID;
		_fncCritical = fncCritical;
		//_sensors=sensors;
	}
	void SetID(const __FlashStringHelper *nodeID)
	{
		_nodeID=nodeID;
	}
	void setup()
	{
		_radio->setup();
	}
	bool measure()
	{
		bool retCode=false;
		for(int i=0;i<_sensors.Count();i++)
		{
			_fncCritical();
			if(_sensors[i]->IsReadyForMeasurement())
			{
				_sensors[i]->init_measurements();
				_sensors[i]->measure();
				retCode = true;
			}
		}
		return retCode;
	}
	bool IsChanged()
	{
	  for(int i=0;i<_sensors.Count();i++)
		if(_sensors[i]->IsChanged())
		{
			//Serial.print(_sensors[i]->Name);
			//Serial.println(" changed");
			return true;
		}
	  return false;
	}
	void sendData()
	{
	  for(int i=0;i<_sensors.Count();i++)
	  {
		  OneWireSensor *onePort=_sensors[i];
		  if(onePort->IsOK())
		      _radio->send_data(_nodeID,onePort->Name,onePort->GetData());
	  }
	}
	void printSerial()
	{
	  for(int i=0;i<_sensors.Count();i++)
	  {
		  OneWireSensor *onePort=_sensors[i];
		  Serial.print(onePort->Name);
		  Serial.print(": ");
		  if(onePort->IsOK())
			  Serial.print(onePort->GetData());
		  else
			  Serial.print("error");
		  Serial.print("; ");
	  }
	  Serial.println();
	}
};