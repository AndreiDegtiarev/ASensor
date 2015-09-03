/*
  AFrame - Arduino framework library for ASensor and AWind libraries
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can always find the latest version of the library at 
  https://github.com/AndreiDegtiarev/ASensor

  This library is free software; you can redistribute it and/or
  modify it under the terms of the MIT license.
  Please see the included documents for further information.
*/
///Sensor example demonstrates basic measuremnt node that measures and sends via radio temerature and humidity results
///Installation: AFrame + https://github.com/markruys/arduino-DHT libraries have to be installed
///Wiring: DHT sensor should be connected to the pin 10
///Radio sender: see rf24 library
#include <OneWire.h>
#include <DHT.h>
#include <SPI.h>
#include <RF24.h>

#include "ISensor.h"
#include "DHTHumiditySensor.h"
#include "SensorManager.h"
#include "MeasurementNode.h"

#include "LinkedList.h"

//pin on Arduino where temperature sensor is connected (in demo is meaningless)
int temperature_port=10;

//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors);

void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out<<F("Setup")<<endl;

	//sensors
	DHTTemperatureSensor *inTempr=new DHTTemperatureSensor(temperature_port);
	DHTHumiditySensor *inHumidity=new DHTHumiditySensor(inTempr);
	//sensor managers. Manager defines measurement limits and measurement delays
	sensors.Add(new SensorManager(inTempr,15,40,1000*10)); //0
	sensors.Add(new SensorManager(inHumidity,0,80,1000*10)); //1

	delay(1000); 

	out<<F("End setup")<<endl;
}
void loop()
{
	//measure (if necessary -see delay parameter in sensor manager)
	if(measurementNode.Measure())
	{
		if(measurementNode.IsChanged())
		{
			measurementNode.SendData();
		//following if is only for debugging purposes
			measurementNode.LogResults();
		}

	}
}