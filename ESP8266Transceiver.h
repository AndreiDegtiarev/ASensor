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
#include "Transceiver.h"

///Warpper about ESP8266 library
class ESP8266Transceiver : public Transceiver
{
	const char *_wlan;
	const char *_password;
	WiFiClient _client;
	Adafruit_MQTT_Client _mqtt;
	LinkedList<Adafruit_MQTT_Publish> _publischSensors;
	LinkedList<SensorManager> &_sensors;
	char _MACAddress[18];

public:
	ESP8266Transceiver(const char *wlan, const char *password, const char *server, LinkedList<SensorManager> &sensors)
		:_mqtt(&_client, server, 1883,"guest1","guest1"), _wlan(wlan), _password(password), _sensors(sensors)
	{

	}
	virtual void setup()
	{
		uint8_t MAC_array[6];
		WiFi.macAddress(MAC_array);
		for (int i = 0; i < sizeof(MAC_array); ++i) {
			sprintf(_MACAddress, "%s%02x:", _MACAddress, MAC_array[i]);
		}
		WiFi.begin(_wlan, _password);
		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			out<<".";
		}
		out << "Connected";
		char buf[100];
		for (int i = 0; i < _sensors.Count(); i++)
		{
			if(_sensors[i]->AppName()!=NULL)
				sprintf(buf, "MAC%s/%s/%s", _MACAddress,"ASensors",  _sensors[i]->AppName());
			else
				sprintf(buf, "MAC%s/%s/%s", _MACAddress,"ASensors",  _sensors[i]->Sensor()->Name());
			char * name = new char[strlen(buf)+1];
			buf[strlen(buf)] = 0;
			strncpy(name, buf, strlen(buf));
			out << buf << strlen(buf) <<endln;
			
			_publischSensors.Add(new Adafruit_MQTT_Publish(&_mqtt, name));
		}
		out << endln;
	}
	void send_data(const char *nodeID,const char *name, float value)
	{
		MQTT_connect();

		// Now we can publish stuff!
		out << F("\nSending val ");
		out << value;
		out << "...";
		for (int i = 0; i < _sensors.Count(); i++)
		{
			const char *sensorName = _sensors[i]->AppName() != NULL ? _sensors[i]->AppName() : _sensors[i]->Sensor()->Name();
			if (sensorName == name)
			{
				if (!_publischSensors[i]->publish(value)) {
					out << F("Failed") << endln;
				}
				else {
					out << F("OK!") << endln;
				}
				break;
			}
		}
	}
	// Function to connect and reconnect as necessary to the MQTT server.
	// Should be called in the loop function and it will take care if connecting.
	void MQTT_connect() {
		int8_t ret;

		// Stop if already connected.
		if (_mqtt.connected()) {
			return;
		}

		out<<"Connecting to MQTT... ";

		uint8_t retries = 3;
		while ((ret = _mqtt.connect()) != 0) { // connect will return 0 for connected
			out << _mqtt.connectErrorString(ret)<<endln;
			out << "Retrying MQTT connection in 5 seconds..." << endln;
			_mqtt.disconnect();
			delay(5000);  // wait 5 seconds
			retries--;
			if (retries == 0) {
				// basically die and wait for WDT to reset me
				while (1);
			}
		}
		out << "MQTT Connected!" << endln;
	}
};