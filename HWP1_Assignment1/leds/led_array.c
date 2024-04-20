/*
 * led_array.c
 *
 * Created: 19/02/2024 08.44.23
 *  Author: jurin
 */ 

#include <avr/io.h>
#include "led_array.h"


void init_leds() {
	DDRA = 0xFF;
	PORTA = 0xFF;
}

int8_t set_led(uint8_t led_no, uint8_t state) {
	if (led_no < 1 || led_no > 8) {
		printf("Incorrect LED number entered.");
		return ERROR;
	}
	if (state != ON && state != OFF) {
		printf("Incorrect state entered.");
		return ERROR;
	}
	led_no -= 1;
	if (!state) {
		PORTA |= _BV(led_no);
		return SUCCESS;
	}
	else {
		PORTA &= ~_BV(led_no);
		return SUCCESS;
	}
}

int8_t lightbar(uint8_t level) {
	// reg val for debug
	//uint8_t reg = (uint8_t)PORTA;
	if (level < 0 || level > 8) {
		printf("Incorrect lightbar level entered.");
		return ERROR;
	}
	if (level == 0) {
		PORTA = 0xFF;
		return SUCCESS;
	}
	level -= 1;
	PORTA = 0xFF;
	for (int i = 0; i <= level; i++) {
		PORTA &= ~(1 << i);
	}
	return SUCCESS;
}

