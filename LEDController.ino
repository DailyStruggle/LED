#include "LED.h"

static const uint8_t red = 9;
static const uint8_t green = 10;
static const uint8_t blue = 11;
static const uint8_t Switch = 2;

uint16_t c[3] = {0, 180, 270};
bool started = false;

LED *led;

void setup() {
  Serial.begin(9600);
  led = new LED;
  led->setPins(red, green, blue, Switch);
}

void loop() {
  if(!started){
    led->Start();
    started = true;
  }
}
