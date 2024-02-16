#ifndef ROTOR_H
#define ROTOR_H

#include "Notch.h"

typedef uint8_t RotorType;

static const RotorType I     =  0;
static const RotorType II    =  1;
static const RotorType III   =  2;
static const RotorType IV    =  3;
static const RotorType V     =  4;
static const RotorType VI    =  5;
static const RotorType VII   =  6;
static const RotorType VIII  =  7;
static const RotorType BETA  =  8;
static const RotorType GAMMA =  9;
static const RotorType UKW_M = 10;

typedef uint8_t Position;

static const Position     TABLE_I[26] = { 4,10,12, 5,11, 6, 3,16,21,25,13,19,14,22,24, 7,23,20,18,15, 0, 8, 1,17, 2, 9};
static const Position    TABLE_II[26] = { 0, 9, 3,10,18, 8,17,20,23, 1,11, 7,22,19,12, 2,16, 6,25,13,15,24, 5,21,14, 4};
static const Position   TABLE_III[26] = { 1, 3, 5, 7, 9,11, 2,15,17,19,23,21,25,13,24, 4, 8,22, 6, 0,10,12,20,18,16,14};
static const Position    TABLE_IV[26] = { 4,18,14,21,15,25, 9, 0,24,16,20, 8,17, 7,23,11,13, 5,19, 6,10, 3, 2,12,22, 1};
static const Position     TABLE_V[26] = {21,25, 1,17, 6, 8,19,24,20,15,18, 3,13, 7,11,23, 0,22,12, 9,16,14, 5, 4, 2,10};
static const Position    TABLE_VI[26] = { 9,15, 6,21,14,20,12, 5,24,16, 1, 4,13, 7,25,17, 3,10, 0,18,23,11, 8, 2,19,22};
static const Position   TABLE_VII[26] = {13,25, 9, 7, 6,17, 2,23,12,24,18,22, 1,14,20, 5, 0, 8,21,11,15, 4,10,16, 3,19};
static const Position  TABLE_VIII[26] = { 5,10,16, 7,19,11,23,14, 2, 1, 9,18,15, 3,25,17, 0,12, 4,22,13, 8,20,24, 6,21};

static const Position  TABLE_BETA[26] = {11, 4,24, 9,21, 2,13, 8,23,22,15, 1,16,12, 3,17,19, 0,10,25, 6, 5,20, 7,14,18};
static const Position TABLE_GAMMA[26] = { 5,18,14,10, 0,13,20, 4,17, 7,12, 1,19, 8,24, 2,22,11,16,15,25,23,21, 6, 9, 3};
static const Position TABLE_UKW_M[22] = {17, 3,14, 1, 9,20,19,10,21, 4, 7,12,11,16, 2,18,13, 0,15, 6, 5, 8};

static const Position rvrs_positions_order[26] = {25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
static const Position drct_positions_order[26] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

static const Position rvrs_rotors_order[4] = {3, 2, 1, 0};
static const Position drct_rotors_order[4] = {0, 1, 2, 3};

Position reflect(Position position, bool encryption_procedure) 
{
  switch (position) {
    case 23:
      position = encryption_procedure ? 24 : 25;
      break;
    case 24:
      position = encryption_procedure ? 25 : 23;
      break;
    case 25:
      position = encryption_procedure ? 23 : 24;
      break;  
    default:
      if (position != 22) position = TABLE_UKW_M[position];
      break;
  };
  return position;
}

class Rotor 
{
public:
  ~Rotor() 
  {
    delete[] table;
    delete notch;
    delete & offset;
    delete & turnover;
  }

  Rotor(RotorType type) 
  {
    table = new Position[26];
    switch (type) {
      case I:
        memcpy(table, TABLE_I, 26);
        notch = new Notch(24);
        break;
      case II:
        memcpy(table, TABLE_II, 26);
        notch = new Notch(12);
        break;
      case III:
        memcpy(table, TABLE_III, 26);
        notch = new Notch(3);
        break;
      case IV:
        memcpy(table, TABLE_IV, 26);
        notch = new Notch(17);
        break;
      case V:
        memcpy(table, TABLE_V, 26);
        notch = new Notch(7);
        break;
      case VI:
        memcpy(table, TABLE_VI, 26);
        notch = new Notch(7, 20);
        break;
      case VII:
        memcpy(table, TABLE_VII, 26);
        notch = new Notch(7, 20);
        break;
      case VIII:
        memcpy(table, TABLE_VIII, 26);
        notch = new Notch(7, 20);
        break;
      case BETA:
        memcpy(table, TABLE_BETA, 26);
        notch = new Notch();
        break;
      case GAMMA:
        memcpy(table, TABLE_BETA, 26);
        notch = new Notch();
        break;
    };
  }

  // TODO : include ring offset
  Position swap(Position position) 
  {
    return table[position];
  }

  // TODO : include ring offset
  Position revert(Position position) 
  {
    for (auto i : drct_positions_order) {
      if (table[i] == position) {
        position = i;
        break;
      }
    }
    return position;
  }

  // TODO : include ring offset
  void set_position(Position value) 
  {
    if (rotable()) {
      while (table[0] != value) { 
        rotate_table_left(); 
      }
    }
  }

  void set_offset(Position value) 
  {
    offset = value;
  }

  Notch get_notch() 
  {
    return *notch;
  }

private:
  Position * table;
  Notch * notch;

  Position offset   = 0;
  Position turnover = 0;

  bool rotable() 
  {
    return !(notch->movable());
  }

  void rotate_table_left() 
  {
    auto first = table[0];

    for (auto i = 1; i < 26; i++) {
      table[i - 1] = table[i];
    }

    table[25] = first;
  }
};

#endif