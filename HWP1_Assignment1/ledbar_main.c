/*
 * ledbar_main.cpp
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
	
	while (1) 
    {
		// POLLING LIGHTBAR TEST
		uint8_t old_key_val = 0;
		uint8_t new_key_val = scan_key();
		if (!(new_key_val == 0 || new_key_val == old_key_val)){
			lightbar(new_key_val);
			old_key_val = new_key_val;
		}
    }
}

