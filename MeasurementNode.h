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
#include "ICriticalProcess.h"
#ifndef DEMO_SENSORS
#include "NRF24Transceiver.h"
#endif

class MeasurementNode
{
	LinkedList<SensorManager> &_sensors;
	ICriticalProcess *_criticalProcess;

#ifndef DEMO_SENSORS
	NRF24Transceiver *_radio;
#endif
	const __FlashStringHelper *_nodeID;
public:
	MeasurementNode(/*const __FlashStringHelper *nodeID,*/LinkedList<SensorManager> &sensors):_sensors(sensors)
	{
#ifndef DEMO_SENSORS
		_radio= new NRF24Transceiver(8,9);
#endif
		//_nodeID = nodeID;
		_criticalProcess = NULL;
		//_sensors=sensors;
	}
	void SetID(const __FlashStringHelper *nodeID)
	{
		_nodeID=nodeID;
	}
	void setup()
	{
#ifndef DEMO_SENSORS
		_radio->setup();
#endif
	}
	void SetCriticalProcess(ICriticalProcess *criticalProcess)
	{
		_criticalProcess=criticalProcess;
	}

	bool measure()
	{
		bool retCode=false;
		for(int i=0;i<_sensors.Count();i++)
		{
			if(_criticalProcess!=NULL)
				_criticalProcess->Idle();
			if(_sensors[i]->IsReadyForMeasurement())
			{
				//_sensors[i]->InitMeasurements();
				_sensors[i]->Measure();
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
		  SensorManager *sensorManager=_sensors[i];
		  if(sensorManager->Status()!=Error)
		  { 
#ifndef DEMO_SENSORS
		      _radio->send_data(_nodeID,sensorManager->Sensor()->Name(),sensorManager->GetData());
#endif
		  }
	  }
	}
	void printSerial()
	{
	  for(int i=0;i<_sensors.Count();i++)
	  {
		  SensorManager *sensorManager=_sensors[i];
		  Serial.print(sensorManager->Sensor()->Name());
		  Serial.print(F(": "));
		  if(sensorManager->Status()!=Error)
			  Serial.print(sensorManager->GetData());
		  else
			  Serial.print(F("error"));
		  Serial.print(F("; "));
	  }
	  Serial.println();
	}
};