/*
 * main.c
 *
 * Created: 29/04/2024 10.32.38
 *  Author: jurin
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "tmp36_driver/tmp36_driver.h"
#include "leds/led_array.h"

int main(void)
{
	sei();
	tmp36_init();
	init_leds();
	
	int16_t old_temp = 0;
	float new_temperature = 0.0;
		
	while (1) {
        tmp36_process(&new_temperature); 
		volatile int16_t rounded_temp = (int16_t) new_temperature;
		
		if(rounded_temp != old_temp){
		
			if (rounded_temp < 18) {
			  lightbar(0);
		 } else if (rounded_temp <= 25) {
			    // Scale temperatures from 18 to 25 to LED bars 1 to 7
			    uint8_t led_level = 1 + (rounded_temp - 18) * (8 - 1) / (25 - 18);
		     lightbar(led_level);
		  } else {
		      lightbar(8);
			}
			old_temp = rounded_temp;
		}
    }
}