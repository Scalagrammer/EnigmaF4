#ifndef ROTOR_DRIVER_H
#define ROTOR_DRIVER_H

#include "HardwareSerial.h"
#include <Arduino.h>

#include "Notch.h"

class RotorDriver {
public:
  RotorDriver(uint8_t clk, uint8_t cs, uint8_t data) {
  // RotorDriver(uint8_t clk, uint8_t cs, uint8_t data) {
    // this->clk  = clk;
    // this->cs   = cs;
    // this->data = data;
    // pinMode(clk,  OUTPUT);
    // pinMode(cs,   OUTPUT);
    // pinMode(data, OUTPUT);
  }
  
  bool * rotate_all(Notch * notches) {

    bool rotated[4] = {false, false, false, false};

    for (auto i = 0; i < 4; i++) {
      auto position = turn(i);
      rotated[i] = true;
      if (!notches[i].has_position(position)) break;
    }

    return rotated;
  }

  void turn_all() {
    for (auto i = 0; i < 4; i++) turn(i);
  }

  uint8_t * positions() {
    return new uint8_t[4]{0,1,2,3}; // TODO : read whole encoders range and recognize
  }

private:
  uint8_t clk, cs, data = 0;

  uint8_t turn(uint8_t rotor) {
    switch (rotor) {
      case 0:
        turn_zero();
        return 0; // TODO : read encoders range and recognize
      case 1:
        turn_first();
        return 0; // TODO : read encoders range and recognize
      case 2:
        turn_second();
        return 0; // TODO : read encoders range and recognize
      case 3:
        turn_third();
        return 0; // TODO : read encoders range and recognize
    };
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

  void turn_zero() {
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b10100000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b01100000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b01010000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b10010000);
    select_chip(HIGH);
    reset();
  }

  void turn_first() {
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b00001010);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b00000110);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b00000101);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b00001001);
    select_chip(HIGH);
    reset();
  }

  void turn_second() {
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b10100000);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b01100000);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b01010000);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b10010000);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    reset();
  }

  void turn_third() {
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00001010);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000110);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000101);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00001001);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
    reset();
  }

  void reset() {
    select_chip(LOW);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    shiftOut(data, clk, MSBFIRST, 0b00000000);
    select_chip(HIGH);
  }

  void select_chip(uint8_t flag) {
    digitalWrite(cs, flag);
  }
};

#endif