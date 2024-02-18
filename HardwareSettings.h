#include "pins_arduino.h"
#pragma once

#ifndef HARDWARE_SETTINGS_H
#define HARDWARE_SETTINGS_H

typedef uint8_t pin;

typedef uint8_t UartRateBound;

static const pin CLK = PIND2;
 // serial data
static const pin DAT = PIND3;
// rotor encoders
static const pin ENC_CS = 9;
// driver
static const pin DRR_CS = 8;
// ledboard
static const pin LDB_CS = PIND5;
// keyboard
static const pin KBR_CS = PIND4;
// plugboard
static const pin PGB_RX_CS = PIND7; 
static const pin PGB_TX_CS = PIND6;
// control pane RGB mode led
static const pin MLD_R = PIN_A0; 
static const pin MLD_G = PIN_A1; 
static const pin MLD_B = PIN_A2;
// printer UART
static const uint8_t URT_RB = 9600;
static const ping URT_TX = TXLED0;
static const ping URT_RX = 9600;
// morse speaker
static const pin MRS_S = 0;
// backlight switch
static const pin BLT_S = 0;

/*
                 +----[....]----+
 [URT_TX] <- TXO |.    USBC    .| RAW
 [URT_RX] <- RXI |.            .| GND -> [-]
      [-] <- GND |.            .| RST -> [RST_S]
      [-] <- GND |.  ++++++++  .| VCC -> [+5V]
      [CLK] <- 2 |.  | MEGA |  .| A3 -> [MRS_S]
      [DAT] <- 3 |.  | 32U4 |  .| A2 -> [MLD_B]
   [KBR_CS] <- 4 |.  ++++++++  .| A1 -> [MLD_G]
   [LDB_CS] <- 5 |.            .| A0 -> [MLD_R]
[PGB_TX_CS] <- 6 |.            .| 15 -> [BLT_S]
[PGB_RX_CS] <- 7 |.            .| 14 
   [DRR_CS] <- 8 |.            .| 16
   [ENC_CS] <- 9 |. Pro  Micro .| 10
                 +--------------+
*/

#endif