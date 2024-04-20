/*
 * led_array.h
 *
 * Created: 19/02/2024 08.45.20
 *  Author: jurin
 */ 

#include <stdint.h>
#include <stdio.h>

#ifndef LED_ARRAY_H_
#define LED_ARRAY_H_

#define ON 1
#define OFF 0
#define SUCCESS 2
#define ERROR -1

void init_leds();
int8_t set_led(uint8_t led_no, uint8_t state);
int8_t lightbar(uint8_t level);


#endif /* LED_ARRAY_H_ */