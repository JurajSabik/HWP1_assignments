/*
 * assignment4_main.c
 *
 * Created: 17/05/2024 19.50.12
 *  Author: jurin
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "./display/display.h"
#include "matrix_keyboard.h"

void update_display(uint16_t value) {
	printint_4u(value);
}

int main(void) {
	init_display();
	init_matrix_keyboard();
	bool cancelled;

	
	while (1) {
		// Pass the address of update_display function as a callback
		uint16_t user_input = get_user_input(&cancelled, update_display);
		if(!cancelled){
			update_display(user_input);
		}
		else{
			update_display(0);
		}
	}
}