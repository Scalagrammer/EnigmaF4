#include "Stream.h"
#include "HardwareSerial.h"
#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifndef KEY_EVENT_TIMEOUT
#define KEY_EVENT_TIMEOUT 10
#endif

#include <Arduino.h>
#include <GyverShift.h>

typedef uint8_t Key;

/*
  A - 0 
  B - 1 
  C - 2 
  D - 3 
  E - 4 
  F - 5 
  G - 6 
  H - 7 
  I - 8 
  J - 9 
  K - 10
  L - 11
  M - 12
  N - 13
  O - 14
  P - 15
  Q - 16
  R - 17
  S - 18
  T - 19
  U - 20
  V - 21
  W - 22
  X - 23
  Y - 24
  Z - 25
*/

static const Key IK = 255;
static const Key M  =  12;
static const Key E  =   4;
static const Key D  =   3;
static const Key R  =  17;
static const Key W  =  22;
static const Key O  =  14;
static const Key K  =  10;

extern void on_key_released();

extern void on_key_pressed(Key key);

extern void on_decryption_command_keys_typed();
extern void on_encryption_command_keys_typed();

extern void on_rotor_types_command_keys_typed();
extern void on_ring_positions_command_keys_typed();

class Keyboard {
public:
  Keyboard(uint8_t clk, uint8_t cs, uint8_t data) : keys(cs, data, clk) {}

  void listen() {
    while (true) {
      if (!keys.update()) delay(KEY_EVENT_TIMEOUT);
      recognize_and_handle();
    }
  }    

  void await_accept_command_keys_typed() {
  await:
    while (!keys.update()) delay(KEY_EVENT_TIMEOUT);
    if (keys[O] && keys[K]) return; // TODO exclusive mode
    goto await; 
  }

private:

  const GyverShift<INPUT, 4> keys;

  void recognize_and_handle() {
    if (keys[M] && keys[E]) {
      on_encryption_command_keys_typed();
    } else if (keys[M] && keys[D]) {
      on_decryption_command_keys_typed();
    } else if (keys[M] && keys[R]) {
      on_ring_positions_command_keys_typed();  
    } else if (keys[M] && keys[W]) {
      on_rotor_types_command_keys_typed();  
    // } else if () { TODO : check for valid range & exclusive mode
    } else for (auto k = 0; k < 26; k++) {
      if (keys[k]) {
        on_key_pressed(k);
        await_key_released(k);
        on_key_released();
        break;
      }
    } 
  }

  void await_key_released(Key key) {
  await:
    while (!keys.update()) delay(KEY_EVENT_TIMEOUT);
    if (keys[key]) 
    goto await; 
  }
};

#endif