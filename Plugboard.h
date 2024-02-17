#ifndef PLUGBOARD_H
#define PLUGBOARD_H

#include "Arduino.h"

#include "gpio.h"

class Plugboard 
{
public:
  Plugboard(uint8_t clk, uint8_t cs_tx, uint8_t cs_rx, uint8_t data) : _clk(clk), _cs_rx(cs_rx), _cs_tx(cs_tx), _data(data) 
  {}

  uint8_t connect(uint8_t position) 
  {
    tx_bus[position] = HIGH;

    gpio::shiftout(4, tx_bus, _clk, _cs_tx, _data);

    tx_bus[position] = LOW;

    gpio::shiftin(4, rx_bus, _clk, _cs_rx, _data);

    gpio::shiftout(4, tx_bus, _clk, _cs_tx, _data); // clear

    for (auto i = 0; i < 26; i++) {
      if (rx_bus[i] == HIGH) { 
        position = i;
        rx_bus[i] = LOW;
        break;
      }
    }

    return position;
  }
private:
  const uint8_t _clk, _cs_rx, _cs_tx, _data;
  bool tx_bus[32];
  bool rx_bus[32];
};

#endif