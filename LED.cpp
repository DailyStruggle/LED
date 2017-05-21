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

#include "LED.h"

void LED::setPins(uint8_t Rpin, uint8_t Gpin, uint8_t Bpin, uint8_t switchpin){
	RPIN = Rpin;
	GPIN = Gpin;
	BPIN = Bpin;
	SPIN = switchpin;
}

void LED::setDial(uint16_t dialpos){
	setDial(dialpos, 255, 255);
}

void LED::setDial(uint16_t dialpos, uint8_t brightness){
	setDial(dialpos, brightness, 255);
}

void LED::setDial(uint16_t dialpos, uint8_t brightness, uint8_t saturation) {
	DIALtoRGB(dialpos);
	setBrightness(brightness, saturation);
	smoothSet(Rt, Gt, Bt, 9);
}

void LED::Flash(uint16_t dialpos, uint8_t brightness, uint8_t saturation, int spd){
	Flash(dialpos, brightness, saturation, spd, 0);
}

void LED::Flash(uint16_t dialpos, uint8_t brightness, uint8_t saturation, int spd, uint8_t smoothing) {
	uint8_t bound = 1;
	spd = spd - 10;
	if (spd<1) {
		bound = -spd + 1;
		spd = 1;
	}
	//time settings, based on spd and smoothing
	int t1, t2;

	//check before dividing by 0
	if (smoothing) {
		t1 = spd*(255 / smoothing)*2;
		t2 = 500 / spd + 500 / t1;
	}
	DIALtoRGB(dialpos);
	setBrightness(brightness, saturation);
	if (!smoothing) {
		Set(Rt, Gt, Bt);
		Rt = 0;
		Gt = 0;
		Bt = 0;
		for (int j = 0; j < bound; j++) {
			if (interrupt()) return;
			delay(500 / spd);
		}
		Set(0, 0, 0);
		for (int j = 0; j < bound; j++) {
			if (interrupt()) return;
			delay(500 / spd);
		}
	}
	//if any smoothing
	else {
		smoothSet(Rt, Gt, Bt, t1);
		Rt = 0;
		Gt = 0;
		Bt = 0;
		for (int j = 0; j < bound; j++) {
			if (interrupt()) return;
			delay(t2);
		}

		smoothSet(0, 0, 0, t1);

		for (int j = 0; j < bound; j++) {
			if (interrupt()) return;
			delay(t2);
		}
	}
}


void LED::fullCycle(uint8_t brightness, int spd) {
	//COLOR WHEEL CYCLE
	//cycles through regular color wheel
	uint8_t bound = 1;
	spd = spd - 20;
	if (spd<1) {
		bound = -spd + 1;
		spd = 1;
	}
	for (int i = 0; i < 360; i++) {
		if (i < 180) {
			for (int j = 0; j<bound; j++) {
				if (interrupt()) return;
				delayMicroseconds(3000 / spd);
			}
		}
		else {
			for (int j = 0; j<bound; j++) {
				if (interrupt()) return;
				delayMicroseconds(5000 / spd);
			}
		}
		DIALtoRGB(i);
		setBrightness(brightness, 255);
		Set(Rt, Gt, Bt);
	}
}

int LED::Cycle(uint8_t arraysize, uint16_t colors[], uint8_t brightness, int spd, uint8_t smoothing) {

	uint8_t bound = 1;
	spd = spd - 10;
	if (spd<1) {
		bound = -spd + 1;
		spd = 1;
	}

	//time settings, based on spd and smoothing
	int t1, t2;

	//check before dividing by 0
	if (smoothing) {
		t1 = spd*(255 / smoothing);
		t2 = 500 / spd + 400 / t1;
	}

	for (int i = 0; i < arraysize; i++) {
		//if no smoothing, just set the colors
		if (!smoothing) {
			DIALtoRGB(colors[i]);
			setBrightness(brightness, 255);
			Set(Rt, Gt, Bt);
			resetvar();
			for (int j = 0; j<bound; j++) {
				if (interrupt()) return 0;
				delay(500 / spd);
			}
		}
		//if any smoothing
		else {
			DIALtoRGB(colors[i]);
			setBrightness(brightness, 255);
			smoothSet(Rt, Gt, Bt, t1);
			Rt = 0;
			Gt = 0;
			Bt = 0;
			for (int j = 0; j<bound; j++) {
				if (interrupt()) return 0;
				delay(t2);
			}
		}
		
	}
	return 0;
}



void LED::Start() {
	Set(0, 0, 0);
	smoothSet(255, 0, 0, 10);
	delay(150);
	smoothSet(0, 255, 0, 10);
	delay(150);
	smoothSet(0, 0, 255, 10);
	delay(150);
	smoothSet(0, 0, 0, 10);
	delay(300);
	smoothSet(255, 255, 255, 10);
	delay(150);
	smoothSet(0, 0, 0, 10);
	delay(500);
}

void LED::Stop() {
	smoothSet(0, 0, 0, 6);
	analogWrite(RPIN, 0);
	analogWrite(GPIN, 0);
	analogWrite(BPIN, 0);
}



void LED::Set(uint8_t Rn, uint8_t Gn, uint8_t Bn) {
	//save new values
	R = Rn;
	G = Gn;
	B = Bn;
	//set saved values
	analogWrite(RPIN, R);
	analogWrite(GPIN, G);
	analogWrite(BPIN, B);
	return;
}

void LED::smoothSet(uint8_t Rn, uint8_t Gn, uint8_t Bn, int spd) {
	//low speed control setup
	uint8_t bound = 1;
	spd = spd - 10;
	if (spd<1) {
		bound = -spd + 1;
		spd = 1;
	}

	//compare current RGB to desired RGB
	while (R != Rn || G != Gn || B != Bn) {
		if (interrupt()) return;
		//move current RGB closer to desired RGB
		if (R > Rn) R--;
		else if (R < Rn) R++;
		if (G > Gn) G--;
		else if (G < Gn) G++;
		if (B > Bn) B--;
		else if (B < Bn) B++;

		//write new current RGB to PWM pins
		analogWrite(RPIN, R);
		analogWrite(BPIN, B);
		analogWrite(GPIN, G);

		//if spd > 1, delay 500us / spd  ,   else delay 500us * bound
		//looped delay is less likely to have overflow
		for (int i = 0; i < bound; i++) {
			if (interrupt()) return;
			delayMicroseconds(500 / spd);
		}
	}
	return;
}

void LED::DIALtoRGB(uint16_t dialpos) {
	//circle shift
	while (dialpos >= 360) dialpos -= 360;
	uint16_t temp = dialpos;
	uint8_t range = 0;


	//stores to temp instead of RGB
	if (dialpos >= 0 && dialpos <= 180) {
		range = 1;
		Rt = (255 - 1.416*temp);
		Gt = 1.416*temp;
	}
	else if (dialpos > 180 && dialpos <= 270) {
		range = 2;
		temp -= 180;    //put temp between 0 and 90
		Gt = (255 - 2.833*temp);
		Bt = 2.833*temp;
	}
	else if (dialpos > 270) {
		range = 3;
		temp -= 270;  //put temp between 0 and 90
		Bt = (255 - 2.833*temp);
		Rt = 2.833*temp;
	}
	return;
}

void LED::setBrightness(uint8_t brightness, uint8_t saturation) {
	if (!saturation) {
		Rt = 255;
		Gt = 255;
		Bt = 255;
		return;
	}

	//this is why they're 16-bit
	Rt = (Rt*(brightness)) / 255;
	Gt = (Gt*(brightness)) / 255;
	Bt = (Bt*(brightness)) / 255;
	Rt = (saturation*Rt + (255 - saturation) * 255) / 255;
	Gt = (saturation*Gt + (255 - saturation) * 255) / 255;
	Bt = (saturation*Bt + (255 - saturation) * 255) / 255;

	if (Rt>255)Rt = 255;
	if (Gt>255)Gt = 255;
	if (Bt>255)Bt = 255;
}

bool LED::interrupt() {
	if (!digitalRead(SPIN)) return true;
	if (Serial.available() > 0) return true;
	return false;
	smoothSet(0, 0, 0, 6);
	resetvar();
}

void LED::resetvar() {
	R = 0;
	G = 0;
	B = 0;
	Rt = 0;
	Gt = 0;
	Bt = 0;
}