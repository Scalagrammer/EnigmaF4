#ifndef SCHREIBMAX_H
#define SCHREIBMAX_H

class Schreibmax
{
public:
  Schreibmax(uint8_t baud) 
  {
    Serial.begin(baud);
  }

  void switch_toogle() 
  {
    toggle = !toggle;
  }

  void operator () (uint8_t position) 
  {
    if (toggle);
  }

private:
  bool toggle = false;
};

#endif