#pragma once

#ifndef HARDWARE_SETTINGS_H
#define HARDWARE_SETTINGS_H

typedef uint8_t pin;

typedef uint8_t UartRateBound;

static const pin CLK = 0;
static const pin DAT = 0; // serial data
// TODO : define actual values when PCBs will be ready
static const pin DRR_CS = 0; // driver
static const pin LDB_CS = 0; // ledboard
static const pin KBR_CS = 0; // keyboard
// plugboard
static const pin PGB_RX_CS = 0; 
static const pin PGB_TX_CS = 0;
// control pane RGB led
static const pin CLD_R = 0; 
static const pin CLD_G = 0; 
static const pin CLD_B = 0;
// printer UART
static const uint8_t URT_RB = 9600;

#endif