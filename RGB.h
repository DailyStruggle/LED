#pragma once

/*
MIT License

Copyright (c) 2017 John Goldsmith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

//shorter integers for better memory management
#include <stdint.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#endif

class RGB {
public:
	//set io pins
	void setPins(uint8_t Rpin, uint8_t Gpin, uint8_t Bpin, uint8_t Spin);
	
	//////////////////////////////////////////////////////////////////////////////////////////////

	//controller functions, to be referenced by interface
	void setDial(uint16_t dialpos);
	void setDial(uint16_t dialpos, uint8_t brightness);
	void setDial(uint16_t dialpos, uint8_t brightness, uint8_t saturation); //set to position in color cylinder
	void Flash(uint16_t dialpos, uint8_t brightness, uint8_t saturation, int spd); //flash given color at a speed
	void Flash(uint16_t dialpos, uint8_t brightness, uint8_t saturation, int spd, uint8_t smoothing); //pulse
	void fullCycle(uint8_t brightness, int spd); //cycle through color wheel
	void Cycle();
	void Cycle(uint8_t brightness, int spd);
	void Cycle(uint8_t arraysize, uint16_t colors[], uint8_t brightness, int spd, uint8_t smoothing); //cycle through given set of colors
																									 //smoothing affects the transition

	////////////////////////////////////////////////////////////////////////////////////////////////

	//feedback
	void Start();
	void Stop();

private: //not referenced by other classes
	//tools
	void Set(uint8_t Rn, uint8_t Gn, uint8_t Bn); //instant RGB set
	void smoothSet(uint8_t Rn, uint8_t Gn, uint8_t Bn, int spd); //transition RGB set
	void DIALtoRGB(uint16_t dialpos); //convert 0-360 to RGB values
	void setBrightness(uint8_t brightness, uint8_t saturation); //lower the RGB values and add white component
	bool interrupt(); //check to stop running loop
	void resetvar(); //debugging tool

	//variables
	uint8_t RPIN, GPIN, BPIN, SPIN; //Red, Green, Blue, and Switch
	uint8_t R = 0, G = 0, B = 0; //current RGB values
	uint16_t Rt = 0, Gt = 0, Bt = 0; //desired RGB values, using 16-bit math, reduced to 8-bit
};

