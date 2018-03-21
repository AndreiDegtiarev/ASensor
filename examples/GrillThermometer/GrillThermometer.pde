/*
  AFrame - Arduino framework library for ASensor and AWind libraries
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can always find the latest version of the library at 
  https://github.com/AndreiDegtiarev/ASensor

  This library is free software; you can redistribute it and/or
  modify it under the terms of the MIT license.
  Please see the included documents for further information.
*/
///wifi Grill thermometer sensor sends data to mqtt message broker (in my case RabbitMQ). WeSHA (project on AndreiDegtiarev github account) server display data in broweser as single value and chart. RabbitMQ and WeSHA server are running on Raspberri PI 3
///Installation: AFrame + https://github.com/markruys/arduino-DHT libraries have to be installed

#define DEBUG_AWIND //!<remove comments if name of window is need to known during runtime. Be carrefull about SRAM
//#define DEMO_SENSORS



#define USER_WIFI   //wifi settings are placed in ..\wifi.h otherwise they can be defined directly in current module 


#define max(a, b) ((a)>(b) ? (a) : (b))
#define min(a, b) ((a)<(b) ? (a) : (b))
#define F(s)     (s)
#define __FlashStringHelper char


#include "AHelper.h"
#include "LinkedList.h"
#include "ISensor.h"
#include "SensorManager.h"


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "ESP8266Transceiver.h"
#ifdef USER_WIFI
#include "..\wifi.h"
#else
#define WLAN_SSID       ""
#define WLAN_PASS       ""
#endif
#define AIO_SERVER      "192.168.2.116"


#include "max6675.h"
#include "MAX6675Thermocouple.h"
#include "ThermistorSensor.h"

#include "MeasurementNode.h"


//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process

MeasurementNode measurementNode(sensors, new ESP8266Transceiver(WLAN_SSID, WLAN_PASS, AIO_SERVER, sensors));

float calibrationTableExternal[] =  {
										21,22.1343,
										55,51.5647,
										60,56.4582,
										66,62.5218,
										71,67.7343,
										75,72.0087,
										79,76.3759,
										89,87.6999,
										116,121.1718,
										121,127.8343,
										124,131.9014,
										134,145.8354,
										140,154.4742,
										144,160.3494,
										148,166.3174,

										};
SensorCalibrator calibratorExternal = SensorCalibrator(calibrationTableExternal,15);
float calibrationTableInternal[] = {
	0.46,	49.7621,
	0.39,	55.16365,
	0.29,	62.88015,
	0.26,	65.1951,
	0.2,	    69.825,
	0.1,    77.5415

};
SensorCalibrator calibratorinternal = SensorCalibrator(calibrationTableInternal, 6);

uint8_t MAC_array[6]; 
char MAC_char[18];

void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(115200);
	out<<F("Setup")<<endln;

	WiFi.macAddress(MAC_array);
	for (int i = 0; i < sizeof(MAC_array); ++i) {
		sprintf(MAC_char, "%s%02x:", MAC_char, MAC_array[i]);
	}

	out<<MAC_char<<endln;

	//sensors
	delay(1000); 
	sensors.Add(new SensorManager(new MAX6675Thermocouple(4, 5, 2), 0, 400, 1000 * 5,false,"External", &calibratorExternal));
	sensors.Add(new SensorManager(new ThermistorSensor(A0), 0, 500, 1000 * 5, false, "Internal", &calibratorinternal));
	measurementNode.Initialize();
	measurementNode.SetupPLXLog();
	out<<F("End setup")<<endln;
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
			measurementNode.LogResultsPLX();
		}

	}
}