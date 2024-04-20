/*
 * buttons.c
 *
 * Created: 10/04/2024 13.26.17
 *  Author: jurin
 */ 

#include <avr/io.h>
#include <stdio.h>
#include "buttons.h"

void init_keys() {
	DDRC &= ~(0x3F); //0bxx000000
    DDRD &= ~(0x0C); //0bxxxx00xx  
	PORTC |= 0x3F;
    PORTD |= 0x0C;
}

int8_t get_key(uint8_t key_no) {
	if (key_no > 8 || key_no < 1) {
		printf("Incorrect button number entered.");
		return ERROR;
	}

    key_no -= 1;
    uint8_t mask; // Mask for checking the pin status

    // Determine the mask and the port based on the button number
    if (key_no < 6) { // For PC0 to PC5 (button0 to button5)
        mask = 1 << key_no;
        if ((PINC & mask) == 0) {
            return ON;
        }
    }
    else { // For PD2 to PD3 (button6 to button7)
        mask = 1 << (key_no - 6 + 2); // key_no - 6 gives 0 for button6 and 1 for button7, +2 adjusts for PD2, PD3
        if ((PIND & mask) == 0) {
            return ON;
        }
    }

    return OFF;
}

// Return the number of the first pressed button [1...8], 0 if no key is pushed
uint8_t scan_key() {
    for (uint8_t i = 1; i <= 8; i++) {
        if (get_key(i) == ON) {
            return i; // Return the number of the pressed button
        }
    }
    return 0; // Return 0 if no button is pressed
}
