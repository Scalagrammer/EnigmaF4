#include "RotorDriver.h"
#include "Keyboard.h"
#include "Ledboard.h"
#include "ControlPane.h"
#include "Rotor.h"
#include "MorseSpeaker.h"
#include "Notch.h"

#include <TimerOne.h>

#include "HardwareSettings.h"

#define KEY_EVENT_TIMEOUT            100 // ms
#define CONTROL_PANE_BLINK_TIMEOUT 70000 // us
#define ACCEPTED_BLINK_MODE_DURATION 450 // ms

const RotorDriver driver(CLK, DRR_CS, DI);
const Ledboard  ledboard(CLK, LDB_CS, DO);
const Keyboard  keyboard(CLK, KYB_CS, DO);

const ControlPane control_pane(CLD_R, CLD_G, CLD_B);

const Rotor * rotor_set[RTS_C] = {new Rotor(I), new Rotor(II), new Rotor(III), new Rotor(IV)}; // default combination

auto encryption_mode = true;

volatile auto blink_mode = ENCRYPTION;

void setup() {
  cli();
  Timer1.initialize(CONTROL_PANE_BLINK_TIMEOUT);
  Timer1.attachInterrupt(on_blink_timeout);
  sei();
}

void loop() {
  keyboard.listen();
}

void on_blink_timeout() {
  control_pane.blink(blink_mode);
}

void on_key_pressed(Key input) {
  shift_alphabet_space();
  ledboard.show(translate(input));
}

void on_key_released() {
  ledboard.hide_all();
}

void on_encryption_command_keys_typed() {
  encryption_mode = true;
  yield_blink_mode(ENCRYPTION);
}

void on_decryption_command_keys_typed() {
  encryption_mode = false;
  yield_blink_mode(DECRYPTION);
}

void on_ring_positions_command_keys_typed() {
  auto pushed_mode = yield_blink_mode(SETTINGS);

  keyboard.await_accept_command_keys_typed();

  auto offsets = driver.positions();

  for (auto i : drct_rotors_order) {
    rotor_set[i]->set_offset(offsets[i]);
  }

  yield_blink_mode(ACCEPTED);
  yield_blink_mode(pushed_mode);
}

void on_rotor_types_command_keys_typed() {
  auto pushed_mode = yield_blink_mode(SETTINGS);

  keyboard.await_accept_command_keys_typed();

  auto types = driver.positions();

  for (auto i : drct_rotors_order) {
    delete rotor_set[i];
    auto type = types[i];
    rotor_set[i] = new Rotor((type > GAMMA) ? GAMMA : type);
  }

  yield_blink_mode(ACCEPTED);
  yield_blink_mode(pushed_mode);
}

void shift_alphabet_space() {
  Notch * notches;

  for (auto i : drct_rotors_order) {
    notches[i] = rotor_set[i]->get_notch();
  }

  auto positions = driver.rotate_all(notches);

  for (auto i : drct_rotors_order) {
    rotor_set[i]->set_position(positions[i]); // synhronize
  }
}

Position translate(Position position) {
  for (auto i : drct_rotors_order)
    position = rotor_set[i]->swap(position);

  position = reflect(position, encryption_mode);

  for (auto i : rvrs_rotors_order)
    position = rotor_set[i]->revert(position);

  return position;
}

BlinkMode yield_blink_mode(BlinkMode mode) {
  auto pushed_blink_mode = blink_mode;

  cli();
  control_pane.reset_led();
  blink_mode = mode;
  sei();

  if (mode == ACCEPTED) {
    delay(ACCEPTED_BLINK_MODE_DURATION);
  }

  return pushed_blink_mode;
}
