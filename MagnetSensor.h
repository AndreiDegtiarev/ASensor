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

class MagnetSensor : public ISensor
{
public:
	MagnetSensor(int port,float low_limit,float high_limit,unsigned long pause_length):ISensor(port,low_limit,high_limit, 0,pause_length)
	{
		Name=F("Magnet");
	}
	virtual void measure()
	{
		SetData((int)(analogRead(_port)>500?1:0));
	}
};