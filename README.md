ASensor library
-------------
Introduction
------------
ASensor (arduino sensor) library that contains wrappers for different sensors implemented in external libraries. Can be used as standalone or together with AWind library

Example:
--------
Sensors example demonstrates basic measurement node that measures and sends via radio temperature and humidity results
Installation: AFrame + https://github.com/markruys/arduino-DHT + http://maniacbug.github.com/RF24 libraries need to be installed
In RF24 library the file RF24_config.h has to be changed: line number 40 has to be changed from "#if 1" to "#if 0"
Wiring: DHT sensor should be connected to the pin 10
Radio sender: see rf24 library

Documentation
------------
API documentation: http://andreidegtiarev.github.io/ASensor/

Installation
------------
To install you need download https://github.com/AndreiDegtiarev/AFrame/archive/master.zip, https://github.com/AndreiDegtiarev/ASensor/archive/master.zip, unzip them into arduinosketchfolder/libraries/ folder and then remove the "-master" in the folder name or just "clone into desktop" them.
Optionally if you need specific sensors following libraries need to be installed:
RF24:   http://maniacbug.github.com/RF24 (see remarks about modifications above)
DHT: https://github.com/markruys/arduino-DHT
BMP085: https://github.com/adafruit/Adafruit_Sensor + https://github.com/adafruit/Adafruit_BMP085_Unified

License
------------
ASensor library can be redistributed and/or modified under the terms of the MIT license. Please see the included documents for further information.
