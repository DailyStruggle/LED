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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

PURPOSE:
give an arduino the functions of a standard RGB controller
[Does not include interface code]

I am now working on generalized audio functions. Let me know if you have requests

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

INPUT VARIABLES:

dialpos (0-360) - position on standard color wheel
				  *note R=0, G= 180, B=270

brightness (0-255) - brightness of led, using pwm

saturation (0-255) - distance from color white

spd (0-20) - rate of change

smoothing (0-255) - adds transition time

colors[] (0-360) - a given set of color-wheel colors

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

FUNCTIONS:

setDial(uint16_t dialpos, uint8_t brightness, uint8_t saturation);
*converts position on a circle to RGB values, adjusts the brightness and saturation levels, then changes the RGB output


Flash(uint16_t dialpos, uint8_t brightness, uint8_t saturation, int spd, uint8_t smoothing);
*given a color, turns LED on and off
*smoothing changes the flash to a pulse


fullCycle(uint8_t brightness, int spd);
*cycles through 360 color values


Cycle(uint8_t arraysize, uint16_t colors[], uint8_t brightness, int spd, uint8_t smoothing);
*cycles through a set of given colors
*smoothing slows the change
