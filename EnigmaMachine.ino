#include "RotorDriver.h"
#include "Keyboard.h"
#include "Ledboard.h"
#include "Plugboard.h"
#include "ControlPane.h"
#include "Rotor.h"
#include "MorseSpeaker.h"
#include "Notch.h"
#include "Translator.h"
#include "Schreibmax.h"

#include <TimerOne.h>

#include "HardwareSettings.h"

#define KEY_EVENT_TIMEOUT            100 // ms
#define CONTROL_PANE_BLINK_TIMEOUT 70000 // us
#define ACCEPTED_BLINK_MODE_DURATION 450 // ms

Schreibmax schreibmax(URT_RB);
Ledboard ledboard(CLK, LDB_CS, DAT);
Keyboard keyboard(CLK, KBR_CS, DAT); 
Translator translator(I, II, III, IV);
ControlPane control_pane(CLD_R, CLD_G, CLD_B);

volatile auto blink_mode = ENCRYPTION_MODE;

void setup() 
{
  cli();
  Timer1.initialize(CONTROL_PANE_BLINK_TIMEOUT);
  Timer1.attachInterrupt(on_blink_timeout);
  sei();
}

void loop() 
{
  keyboard.listen();
}

void on_blink_timeout() 
{
  control_pane.blink(blink_mode);
}

void on_key_pressed(Key position) 
{
  ledboard.show(translator(position));
}

void on_key_released(Key input) 
{
  ledboard.hide_all();
}

void on_printout_command_typed() 
{ 
  schreibmax.switch_toogle();
  auto pushed_mode = yield_blink_mode(ACCEPTED_MODE);
  yield_blink_mode(pushed_mode);
}

void on_encryption_command_typed() 
{
  translator.switch_procedure(ENCRYPTION);
  yield_blink_mode(ENCRYPTION_MODE);
}

void on_decryption_command_typed() 
{
  translator.switch_procedure(DECRYPTION);
  yield_blink_mode(DECRYPTION_MODE);
}

void on_set_offsets_command_typed() 
{
  auto pushed_mode = yield_blink_mode(SETTINGS_MODE);

  keyboard.await_accept_command_typed();
  translator.update_offsets();

  yield_blink_mode(ACCEPTED_MODE);
  yield_blink_mode(pushed_mode);
}

void on_set_walzen_command_typed() 
{
  auto pushed_mode = yield_blink_mode(SETTINGS_MODE);

  keyboard.await_accept_command_typed();
  translator.update_rotor_types();

  yield_blink_mode(ACCEPTED_MODE);
  yield_blink_mode(pushed_mode);
}

BlinkMode yield_blink_mode(BlinkMode mode) 
{
  auto pushed_blink_mode = blink_mode;

  cli();
  control_pane.reset_led();
  blink_mode = mode;
  sei();

  if (mode == ACCEPTED_MODE) {
    delay(ACCEPTED_BLINK_MODE_DURATION);
  }

  return pushed_blink_mode;
}
