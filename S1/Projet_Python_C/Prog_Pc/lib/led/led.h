#ifndef LED_H
#define LED_H

#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>

typedef enum {
    LED_OFF,
    LED_ON,
    LED_BLINK
}LedMode;

void Led_init();
void GetLedState();
void Led_is_On();
void Led_is_Off();
void Set_Led(uint8_t LedState);
void blink_led(uint8_t frequency);
void Led_toggle();

void menu_light();

#endif