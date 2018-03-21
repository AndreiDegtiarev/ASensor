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
///Sensor calibrator helps to recalculate measured values into phisical ones. See GrillThermometer example
class SensorCalibrator
{
	const float * _calibrationTable;
	int _count;
public:
	SensorCalibrator(const float *calibrationTable,int count) :_calibrationTable(calibrationTable), _count(count)
	{
	}
	const float Value(float value)
	{
		int ind = 0;
		for (; ind<_count && _calibrationTable[ind*2]>value; ind++);
		if (ind >= _count - 1)
			ind--;
		//y=ax+b
		//y1 = ax1 + b
		//y2 = ax2 + b
		//y1 = ax1 + y2 - ax2
		//a = y1 - y2 / x1 - x2
		float a = (_calibrationTable[ind * 2+1] - _calibrationTable[(ind + 1)*2+1]) / (_calibrationTable[ind * 2] - _calibrationTable[(ind + 1) * 2]);
		float b = (_calibrationTable[ind * 2+1] - a * _calibrationTable[ind * 2]);
		return a*value + b;
		
	}
};