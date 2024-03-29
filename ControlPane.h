#pragma once

#ifndef CONTROL_PANE_H
#define CONTROL_PANE_H

#include <Arduino.h>

typedef uint8_t Counter;
typedef uint8_t BlinkMode;

static const BlinkMode SETTINGS_MODE   = 0;
static const BlinkMode ENCRYPTION_MODE = 1;
static const BlinkMode DECRYPTION_MODE = 2; 
static const BlinkMode ACCEPTED_MODE   = 3;

extern BlinkMode yield_blink_mode(BlinkMode mode);

static void with_accept_settings_blinking(void (* apply_settings) ()) 
{
  auto pushed_mode = yield_blink_mode(SETTINGS_MODE);
  apply_settings();
  yield_blink_mode(ACCEPTED_MODE);
  yield_blink_mode(pushed_mode);
}

class ControlPane 
{
public:
  ControlPane(uint8_t red_pin, uint8_t green_pin, uint8_t blue_pin) : _red_pin(red_pin), _green_pin(green_pin), _blue_pin(blue_pin) 
  {}

  void reset_led() 
  {
    led_off();
    blinks = -1;
  }

  void start_blink_mode(BlinkMode mode) 
  {
    _mode = mode;
  }

  BlinkMode get_blink_mode() 
  {
    return _mode;
  }

  void blink() 
  {
    switch (_mode) {
      case SETTINGS_MODE: 
        blink_settings_mode();
        break;
      case ENCRYPTION_MODE: 
        blink_encryption_mode();
        break;
      case DECRYPTION_MODE: 
        blink_decryption_mode();
        break;
      case ACCEPTED_MODE: 
        blink_accepted();
        break;
    };
  }
private:
  const uint8_t _red_pin, _green_pin, _blue_pin;

  volatile Counter blinks = -1;

  volatile BlinkMode _mode;

  void blink_settings_mode() 
  {
    switch ((++blinks) %= 10) {
      case 0:
        show_blue();
        break;
      case 1:
        led_off();
        break;
    };
  }

  void blink_encryption_mode() 
  {
    switch ((++blinks) %= 50) {
      case 0:
      case 7:
        show_red();
        break;
      case 1:
      case 8:
        led_off();
        break;
    };
  }

  void blink_decryption_mode() 
  {
    switch ((++blinks) %= 50) {
      case 0:
      case 7:
        show_green();
        break;
      case 1:
      case 8:
        led_off();
        break;
    };
  }

  void blink_accepted() 
  {
    switch ((++blinks) %= 50) {
      case 0:
      case 4:
        show_green();
        break;
      case 2:
      case 6:
        show_blue();
        break;
      case 8:
        show_red();
        break;
      case 1:
      case 3:
      case 5:
      case 7:
      case 9:
        led_off();
        break;
    };
  }

  void led_off() 
  {
    show_rgb(0x00, 0x00, 0x00);
  }

  void show_red() 
  {
    show_rgb(0xFF, 0x00, 0x00);
  }

  void show_green() 
  {
    show_rgb(0x00, 0xFF, 0x00);
  }

  void show_blue() 
  {
    show_rgb(0x00, 0x00, 0xFF);
  }

  void show_rgb(uint8_t r, uint8_t g, uint8_t b) 
  {
    analogWrite(_red_pin,   r);
    analogWrite(_green_pin, g);
    analogWrite(_blue_pin,  b);
  }
};

#endif