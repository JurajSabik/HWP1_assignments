/*
 * ledbar_main.c
 *
 * Created: 20/04/2024 19.43.20
 *  Author: jurin
 */ 
/*
 * HWP1_Assignment1.cpp
 *
 * Created: 12/02/2024 10.10.49
 * Author : jurin
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "leds/led_array.h"
#include "buttons/buttons.h"

int main(void)
{ 
	init_leds();
	init_keys();
	
	uint8_t last_state[6] = {0}; // Stores the last state to minimize unnecessary writes

	while (1) 
    {
		// ASSIGNMENT 1
		
		uint8_t button1_val = get_key(1) == ON; 
		uint8_t button2_val = get_key(2) == ON;

		// Check if the state has changed before updating the LED
		uint8_t new_state[6] = {
			button1_val && button2_val,
			button1_val || button2_val,
			button1_val ^ button2_val,
			!(button1_val && button2_val),
			!(button1_val || button2_val),
			!(button1_val ^ button2_val)
		};

		for (uint8_t i = 0; i < 6; i++) {
			if (new_state[i] != last_state[i]) {
				set_led(i + 1, new_state[i] ? ON : OFF);
				last_state[i] = new_state[i];
			}
		}
		
    }
}

