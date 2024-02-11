#ifndef NOTCH_H
#define NOTCH_H

class Notch {
public:
  Notch(char first = -1, char second = -1) : first_position(first), second_position(second) {}

  bool has_position(char position) {
    return position == first_position || position == second_position;
  }

private:
  char  first_position;
  char second_position;
};

#endif