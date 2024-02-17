#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "Rotor.h"
#include "RotorDriver.h"
#include "HardwareSettings.h"

typedef uint8_t TranslationProcedure;

static const TranslationProcedure ENCRYPTION_PROCEDURE = 0;
static const TranslationProcedure DECRYPTION_PROCEDURE = 1;

class Translator
{
public:
  Translator(RotorType zero, RotorType first, RotorType second, RotorType third) : plugboard(CLK, PGB_TX_CS, PGB_RX_CS, DAT), driver(CLK, DRR_CS, DAT)
  {
    rotor_set[0] = new Rotor(zero);
    rotor_set[1] = new Rotor(first);
    rotor_set[2] = new Rotor(second);
    rotor_set[3] = new Rotor(third);
  }

  void update_offsets() 
  {
    auto offsets = driver.snapshot();

    for (auto i : drct_rotors_order) {
      rotor_set[i]->set_offset(offsets[i]);
    }
  }

  void update_rotor_types()
  {
    auto types = driver.snapshot();

    for (auto i : drct_rotors_order) {
      delete rotor_set[i];
      auto type = types[i];
      rotor_set[i] = new Rotor((type > GAMMA) ? GAMMA : type);
    }
  }

  void shift_alphabet_space() 
  {
    Notch * notches;

    for (auto i : drct_rotors_order) {
      notches[i] = rotor_set[i]->get_notch();
    }

    auto positions = driver.rotate_all(notches);

    for (auto i : drct_rotors_order) {
      rotor_set[i]->set_position(positions[i]); // synhronize
    }
  }

  uint8_t operator () (uint8_t position)
  {
    position = plugboard.connect(position);

    for (auto i : drct_rotors_order)
      position = rotor_set[i]->swap(position);

    position = reflect(position, translate_encryption);

    for (auto i : rvrs_rotors_order)
      position = rotor_set[i]->revert(position);

    position = plugboard.connect(position);

    return position;
  }

  void switch_procedure(TranslationProcedure procedure) 
  {
    translate_encryption = (procedure == ENCRYPTION_PROCEDURE);
  }

private:
  const Plugboard plugboard; 

  const RotorDriver driver;

  const Rotor * rotor_set[4];
  
  bool translate_encryption = true;
};

#endif