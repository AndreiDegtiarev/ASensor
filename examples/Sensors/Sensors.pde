#include "DS18B20Sensor.h"
#include "DHTSensor.h"
#include "SoilMoistureSensor.h"
#include "NRF24Transceiver.h"
#include "MeasurementNode.h"
#include "BMP085Sensor.h"

#include "MagnetSensor.h"

LinkedList<ISensor> sensors;
MeasurementNode measurementNode(sensors);

void setup()
{
}
void loop()
{
}