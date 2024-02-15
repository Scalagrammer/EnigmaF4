#ifndef MORSE_SPEAKER_H
#define MORSE_SPEAKER_H

#include <Arduino.h>

static const char MORSE_CODES[26][5] = {
  {'.', '-', ' ', ' ', ' '},   // A
  {'-', '.', '.', '.', ' '},   // B
  {'-', '.', '-', '.', ' '},   // C
  {'-', '.', '.', ' ', ' '},   // D
  {'.', ' ', ' ', ' ', ' '},   // E
  {'.', '.', '-', '.', ' '},   // F
  {'-', '-', '.', ' ', ' '},   // G
  {'.', '.', '.', '.', ' '},   // H
  {'.', '.', ' ', ' ', ' '},   // I
  {'.', '-', '-', '-', ' '},   // J
  {'-', '.', '-', ' ', ' '},   // K
  {'.', '-', '.', '.', ' '},   // L
  {'-', '-', ' ', ' ', ' '},   // M
  {'-', '.', ' ', ' ', ' '},   // N
  {'-', '-', '-', ' ', ' '},   // O
  {'.', '-', '-', '.', ' '},   // P
  {'-', '-', '.', '-', ' '},   // Q
  {'.', '-', '.', ' ', ' '},   // R
  {'.', '.', '.', ' ', ' '},   // S
  {'-', ' ', ' ', ' ', ' '},   // T
  {'.', '.', '-', ' ', ' '},   // U
  {'.', '.', '.', '-', ' '},   // V
  {'.', '-', '-', ' ', ' '},   // W
  {'-', '.', '.', '-', ' '},   // X
  {'-', '.', '-', '-', ' '},   // Y
  {'-', '-', '.', '.', ' '}    // Z
};

class MorseSpeaker 
{
public:
  MorseSpeaker(uint8_t pin) : speaker_pin(pin) 
  {}

  void play(uint8_t position) 
  {
    for (auto code : MORSE_CODES[position]) {
      if (code == '.') {
        play_dot();
      } else if (code == '-') {
        play_dash();
      }
    }
  }

private:
  static const uint8_t dot_duration   = 100;
  static const uint8_t dash_duration  = 200;
  static const uint8_t pause_duration = 200;
  static const uint8_t tone_frequency = 400;

  const uint8_t speaker_pin;

  void play_dot() 
  {
    tone(speaker_pin, tone_frequency, dot_duration);
    delay(pause_duration);
  }

  void play_dash() 
  {
    tone(speaker_pin, tone_frequency, dash_duration);
    delay(pause_duration);
  }
};

#endif