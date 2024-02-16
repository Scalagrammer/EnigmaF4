#ifndef ROTOR_DRIVER_H
#define ROTOR_DRIVER_H

#include <Arduino.h>

#include "Notch.h"
#include "gpio.h"

class RotorDriver 
{
public:
  RotorDriver(uint8_t clk, uint8_t cs, uint8_t data) : _clk(clk), _cs(cs), _data(data)
  {}

  uint8_t * rotate_all(Notch * notches) 
  {
    pinMode(_data, OUTPUT);

    for (auto i = 0; i < 4; i++) {
      auto notch = notches[i];

      if (!notch.movable()) break;

      auto position = turn(i);

      if (!notch.turnover(position)) break;
    }

    return snapshot();
  }

  uint8_t * snapshot() 
  {
    gpio::shiftin(3, state, _clk, _cs, _data);

    uint8_t * positions;

    uint8_t rotor_index = 0;

    for (auto offset = 0; offset <= 15; offset += 5) {
      positions[rotor_index] = decode_position(offset);
      rotor_index += 1;
    }

    return positions;
  }

private:
  const uint8_t _clk, _cs, _data;

  bool state[24];

  uint8_t decode_position(uint8_t offset) {
    return (4 * state[offset]) + (2 * state[offset + 1]) + state[offset + 2] + (3 * state[offset + 3]) + (6 * state[offset + 4]);
  }

  uint8_t turn(uint8_t rotor) 
  {
    switch (rotor) {
      case 0:
        turn_zero();
        break;
      case 1:
        turn_first();
        break;
      case 2:
        turn_second();
        break;
      case 3:
        turn_third();
        break;
    };
    return *(rotor + snapshot());
  }

/*
 *-+------+----+----+----+---+
 * | Step | C0 | C1 | C2 | C3|
 *-+------+----+----+----+---+
 * |    1 |  1 |  0 |  1 |  0|
 *-+------+----+----+----+---+
 * |    2 |  0 |  1 |  1 |  0|
 *-+------+----+----+----+---+
 * |    3 |  0 |  1 |  0 |  1|
 *-+------+----+----+----+---+
 * |    4 |  1 |  0 |  0 |  1|
 *-+------+----+----+----+---+
*/

// [[0, 1, 2, 3], [4, 5, 6, 7], [8, 9, 10, 11], [12, 13, 14, 15]]
//      zero          first         second           third

// reset: 
// 0b00000000|00000000

// zero:
// 0b10100000|00000000
// 0b01100000|00000000
// 0b01010000|00000000
// 0b10010000|00000000

// first:
// 0b00001010|00000000
// 0b00000110|00000000
// 0b00000101|00000000
// 0b00001001|00000000

// second:
// 0b00000000|10100000
// 0b00000000|01100000
// 0b00000000|01010000
// 0b00000000|10010000

// third:
// 0b00000000|00001010
// 0b00000000|00000110
// 0b00000000|00000101
// 0b00000000|00001001

  void turn_zero() 
  {
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b10100000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b01100000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b01010000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b10010000);
    cs_high();
    reset();
  }

  void turn_first() {
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b00001010);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b00000110);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b00000101);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00001001);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    reset();
  }

  void turn_second() 
  {
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b10100000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b01100000);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b01010000);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b10010000);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    reset();
  }

  void turn_third() 
  {
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00001010);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000110);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000101);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00001001);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
    reset();
  }

  void reset() 
  {
    cs_low();
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    shiftOut(_data, _clk, MSBFIRST, 0b00000000);
    cs_high();
  }

  void cs_low() 
  {
    digitalWrite(_cs, LOW);
  }

  void cs_high() 
  {
    digitalWrite(_cs, HIGH);
  }
};

#endif