#ifndef GPIO_H
#define GPIO_H

#include <Arduino.h>

namespace gpio {

  void shiftout(uint8_t byte_amount, bool * state, uint8_t clk, uint8_t cs, uint8_t data) 
  {
    pinMode(data, OUTPUT);
    
    digitalWrite(cs, LOW);
    
    for (auto i = 0; i < byte_amount; i++) {

      auto offset = i * 8;
      
/*
  LSBFIRST 
[load begin]->|                |<-[shift out]
     cs   ->--|________________|--
     clk  ->--|_-_-_-_-_-_-_-_-|--
     data ->__|_x-x_x-x_x-x_x_x|__
               7 6 5 4 3 2 1 0
*/        

      for (auto j = 0; j < 8; i++) {
        digitalWrite(clk, LOW);
        digitalWrite(data, state[j + offset] ? HIGH : LOW);
        digitalWrite(clk, HIGH);
      }
    }

    digitalWrite(cs, HIGH);
  }

  bool shiftin(uint8_t byte_amount, bool * state, uint8_t clk, uint8_t cs, uint8_t data) 
  {
    pinMode(data, INPUT);

    bool * buf;

    digitalWrite(cs, LOW);

    for (auto i = 0; i < byte_amount; i++) {

      auto offset = i * 8;

      for (auto j = 0; j < 8; j++) {
        digitalWrite(clk, LOW);
        buf[j + offset] = (digitalRead(data) << (7 - j)) == HIGH;      
        digitalWrite(clk, HIGH);
      }
    }

    digitalWrite(cs, HIGH);

    auto updated = false;

    auto bit_amount = byte_amount * 8;

    for (auto i = 0; i < bit_amount; i++) {
      if (state[i] != buf[i]) updated = true;
      state[i] = buf[i];
    }

    return updated;
  }
};

#endif