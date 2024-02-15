#ifndef NOTCH_H
#define NOTCH_H

class Notch 
{
public:
  Notch(char first = -1, char second = -1) : first_position(first), second_position(second) 
  {}

  bool is_static_rotor() 
  {
    return first_position == (-1) && second_position == (-1);
  }

  bool at(char position) 
  {
    return (position == first_position) || (position == second_position);
  }

private:
  char first_position, second_position;
};

#endif