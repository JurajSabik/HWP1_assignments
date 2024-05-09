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

#include "./display/display.h"
#include "./tmp36_driver/tmp36_driver.h"


int main(void) {
	init_display();
	tmp36_init();
		
	float new_temperature = 0.0;
	
	while (1) {
		tmp36_process(&new_temperature);
		volatile uint16_t rounded_temp = (uint16_t) new_temperature;
		printint_4u(rounded_temp);
	}
}




