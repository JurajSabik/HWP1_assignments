/*
 * solar_controller.c
 *
 * Created: 18/05/2024 13.51.46
 *  Author: jurin
 */ 

#include "solar_controller.h"

void set_state_leds(uint8_t led_no, uint8_t state){
	// Turn off all LEDs first if the state is true
	if (state){
		set_led(1, OFF);
		set_led(2, OFF);
		set_led(3, OFF);

		set_led(led_no, ON);
		} else {
		// If the state is false, just turn off the requested LED
		set_led(led_no, OFF);
	}
}

void set_control_leds(uint8_t led_no, uint8_t state){
	// Turn off all LEDs first if the state is true
	if (state){
		set_led(6, OFF);
		set_led(7, OFF);
		
		set_led(led_no, ON);
		} else {
		// If the state is false, just turn off the requested LED
		set_led(led_no, OFF);
	}
}