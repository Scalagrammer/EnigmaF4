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

void setup() 
{
  cli();
  Timer1.initialize(CONTROL_PANE_BLINK_TIMEOUT);
  Timer1.attachInterrupt(on_interrupt_timeout);
  sei();
}

void loop() 
{
  keyboard.listen();
}

void on_interrupt_timeout() 
{
  control_pane.blink();
}

void on_key_pressed(Key position) 
{
  position = translator(position);
  ledboard.show(position); 
  schreibmax(position);
}

void on_key_released(Key position) 
{
  ledboard.hide_all();
  // schreibmax(position); // test
}

void on_printout_command_typed() 
{ 
  schreibmax.switch_toogle();
}

void on_encryption_command_typed() 
{
  translator.switch_procedure(ENCRYPTION_PROCEDURE);
  yield_blink_mode(ENCRYPTION_MODE);
}

void on_decryption_command_typed() 
{
  translator.switch_procedure(DECRYPTION_PROCEDURE);
  yield_blink_mode(DECRYPTION_MODE);
}

void on_set_offsets_command_typed() 
{
  keyboard.await_accept_command_typed();
  translator.update_offsets();
}

void on_set_walzen_command_typed() 
{
  keyboard.await_accept_command_typed();
  translator.update_rotor_types();
}

BlinkMode yield_blink_mode(BlinkMode mode) 
{
  auto pushed_mode = control_pane.get_blink_mode();

  cli();
  control_pane.start_blink_mode(mode);
  sei();

  if (mode == ACCEPTED_MODE) {
    delay(ACCEPTED_BLINK_MODE_DURATION);
  }

  return pushed_mode;
}
 