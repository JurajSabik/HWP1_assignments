/*
 * main.c
 *
 * Created: 07/05/2024 17.36.50
 *  Author: jurin
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "display.h"

int main(void) {
	init_display();
	uint16_t cntr = 0;
	while (1) {
		printint_4u(cntr);  
		cntr = (cntr + 1) % 10000;  
		_delay_ms(50);  
	}
}


