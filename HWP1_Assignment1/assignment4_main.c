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
#include "./matrix_keyboard/matrix_keyboard.h"
#include "buttons/buttons.h"
#include "leds/led_array.h"
#include "tmp36_driver/tmp36_driver.h"
#include "solar_controller/solar_controller.h"

// Function to update the display with a numeric value, needs to match the callback signature
void update_display(uint16_t value) {
	printint_4u(value);
}

int main(void) {
	init_display();
	init_matrix_keyboard();
	init_keys();
	init_leds();
	tmp36_init();
	
	float current_temperature = 0.0;  // Stores the current temperature
	uint8_t current_key_pressed = 0;  // Key press status
	uint16_t low_temperature = 0;     // Low temperature threshold
	uint16_t high_temperature = 0;    // High temperature threshold
	bool cancelled;                  // Flag to check if input from the matrix keyboard was cancelled

	
	enum SYSTEM_STATE current_state = STATE_CURRENT_TEMP;	// Initial state
	
	while (1) {
		current_key_pressed = scan_key();		// Scan for a keypress
		tmp36_process(&current_temperature);	// Update temperature from sensor
		uint16_t rounded_temp = (uint16_t) current_temperature;	// Round the temperature for display
		
		// Control heating or cooling based on temperature thresholds
		if(rounded_temp > high_temperature){
			set_control_leds(T_PUMP_LED, ON);	// Activate pump if temperature is too high
		}
		else if(rounded_temp < low_temperature){
			set_control_leds(T_VALVE_LED, ON);	// Activate valve if temperature is too low
		}
		
		// State machine handling user interactions and display updates
		switch(current_state){
			case STATE_CURRENT_TEMP:
			printint_4u(rounded_temp);					// Display current temperature
			set_state_leds(T_CURRENT_LED, ON);			// Indicate current temp mode 
			if(current_key_pressed == T_LOW_BUTTON){	// Change state to to the low temp
				current_state = STATE_LOW_TEMP;
			}
			else if(current_key_pressed == T_HIGH_BUTTON){	// Change state to to the high temp
				current_state = STATE_HIGH_TEMP;
			}
			break;
			
			case STATE_LOW_TEMP:
			printint_4u(low_temperature);				// Display low temperature threshold
			set_state_leds(T_LOW_LED, ON);				// Indicate low temp set mode
			if(get_matrix_key_label() == SET_TEMP){		// If 'A' is pressed on the keyboard, enter user input mode
				uint16_t user_input = get_user_input(&cancelled, low_temperature, update_display);
				if(!cancelled){
					low_temperature = user_input;		// If the input was confirmed with '#', set the new low temp
				}
				else{
					update_display(low_temperature);	// If the input was cancelled with '*', display the original temp
				}
			}
			// Handle state transitions
			if(current_key_pressed == T_CURRENT_BUTTON){
				current_state = STATE_CURRENT_TEMP;
			}
			else if(current_key_pressed == T_HIGH_BUTTON){
				current_state = STATE_HIGH_TEMP;
			}
			break;
			
			case STATE_HIGH_TEMP:
			printint_4u(high_temperature);				// Display high temperature threshold
			set_state_leds(T_HIGH_LED, ON);				// Indicate current temp mode
			if(get_matrix_key_label() == SET_TEMP){		// Same as for low temp, update the high threshold
				uint16_t user_input = get_user_input(&cancelled, high_temperature, update_display);
				if(!cancelled){
					high_temperature = user_input;
				}
				else{
					update_display(high_temperature);
				}
			}
			// Handle state transition
			if(current_key_pressed == T_CURRENT_BUTTON){
				current_state = STATE_CURRENT_TEMP;
			}
			else if(current_key_pressed == T_LOW_BUTTON){
				current_state = STATE_LOW_TEMP;
			}
			break;
		}
		
		// Reset key state to avoid repeated processing
		current_key_pressed = NO_PRESS;
		_delay_ms(50); // Small delay for debouncing/stability
		
	}
}