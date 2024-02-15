#ifndef LEDBOARD_H
#define LEDBOARD_H

#include <Arduino.h>

#include "gpio.h"

class Ledboard 
{
public:
  Ledboard(uint8_t clk, uint8_t cs, uint8_t data) : _clk(clk), _cs(cs), _data(data) 
  {}

  void show(uint8_t position) 
  {
    leds[position] = true;
    gpio::shiftout(4, leds, _clk, _cs, _data);
    leds[position] = false;
  }

  void hide_all() 
  {
    gpio::shiftout(4, leds, _clk, _cs, _data);
  }

private:
  bool leds[32];
  const uint8_t _clk, _cs, _data;
};
#endif