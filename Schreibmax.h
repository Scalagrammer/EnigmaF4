#ifndef SCHREIBMAX_H
#define SCHREIBMAX_H

#include <SoftwareSerial.h>

#define SO  0x0E
#define DC4 0x14
#define ESC 0x1B
#define FS  0x1C
#define GS  0x1D

static const uint8_t PRINTABLE_CHARS[27];

static const uint8_t SEPARATOR = 26;

class Schreibmax
{
public:
  Schreibmax(uint8_t baud) 
  {
    
  }

  void switch_toogle() 
  {
    if (toggle = !toggle) {
      carriage_position = 0;
    }
  }

  void operator () (uint8_t position) 
  {
    if (toggle) printout_position(position);
  }

private:
  bool toggle = false;

  uint8_t carriage_position = 0;

  uint8_t buf[19]; // [XXXX XXXX XXXX XXXX]

/*
 char[] buf = new char[19];

    int carriagePosition = 0;

    void printout(char position) {

        switch (carriagePosition) {
            case 4, 9, 14 -> buf[carriagePosition++] = ' ';
        }

        buf[carriagePosition++] = position;

        if (0 == (carriagePosition %= 19)) {
            for (int i = 0; i < 19; i++) {
                System.out.print(buf[i]);
            }
            System.out.println();
        }
    }
*/

  void printout_position(uint8_t position) {

    switch (carriage_position) {
      case  4:
      case  9:
      case 14:
        buf[carriage_position++] = PRINTABLE_CHARS[SEPARATOR];
    }

    buf[carriage_position++] = PRINTABLE_CHARS[SEPARATOR];

    if (0 == (carriage_position %= 19)) {

    }
  }
};

#endif