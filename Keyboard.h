#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifndef KEY_EVENT_TIMEOUT
#define KEY_EVENT_TIMEOUT 10
#endif

#include <Arduino.h>
#include "gpio.h"

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
static const Key K  =  10;
static const Key O  =  14;
static const Key D  =   3;
static const Key E  =   4;
static const Key W  =  22;
static const Key P  =  15;
static const Key C  =   2;

extern void on_key_released(Key key);

extern void on_key_pressed(Key key);

extern void on_printout_command_typed();

extern void on_decryption_command_typed();
extern void on_encryption_command_typed();

extern void on_set_walzen_command_typed();
extern void on_set_offsets_command_typed();

class Keyboard 
{
public:
  Keyboard(uint8_t clk, uint8_t cs, uint8_t data) : _clk(clk), _cs(cs), _data(data) 
  {}

  void listen() 
  {
    while (true) {
      if (!updated()) delay(KEY_EVENT_TIMEOUT);
      accept();
    }
  }    

  void await_accept_command_typed() 
  {
  await:
    while (!updated()) delay(KEY_EVENT_TIMEOUT);
    if (keys[O] && keys[K]) return; // TODO exclusive mode
    goto await; 
  }

private:
  bool keys[26];

  const uint8_t _clk, _cs, _data;

  void await_key_released(Key key) 
  {
  await:
    while (!updated()) delay(KEY_EVENT_TIMEOUT);
    if (keys[key]) 
    goto await; 
  }

  bool updated() 
  {
    return gpio::shiftin(4, keys, _clk, _cs, _data);
  }

  void accept() 
  {
    if (keys[C] && keys[P]) {
      on_printout_command_typed();
    } else if (keys[C] && keys[E]) {
      on_encryption_command_typed();
    } else if (keys[C] && keys[D]) {
      on_decryption_command_typed();
    } else if (keys[C] && keys[O]) {
      on_set_offsets_command_typed();  
    } else if (keys[C] && keys[W]) {
      on_set_walzen_command_typed();  
    } else for (auto k = 0; k < 26; k++) {
      if (keys[k]) {
        on_key_pressed(k);
        await_key_released(k);
        on_key_released(k);
        break;
      }
    } 
  }
};

#endif