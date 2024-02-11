#ifndef LEDBOARD_H
#define LEDBOARD_H

#include <Arduino.h>
#include <GyverShift.h>

class Ledboard {
public:
  Ledboard(uint8_t clk, uint8_t cs, uint8_t data) : leds(cs, data, clk) {}

  void show(uint8_t position) {
    leds.set(position);
    leds.update();
  }

  void hide_all() {
    leds.clearAll();
    leds.update();
  }

private:
  const GyverShift<OUTPUT, 4> leds;
};
#endif