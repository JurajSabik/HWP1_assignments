/*
 * matrix_keyboard.c
 *
 * Created: 22/04/2024 12.37.03
 *  Author: jurin
 */ 


#include "matrix_keyboard.h"


typedef struct keyboard_descr_t
{
	volatile uint8_t* ddr;
	volatile uint8_t* pin;
	volatile uint8_t* port;
	uint8_t bit;
} keyboard_descr_t;

keyboard_descr_t row_pins[] = {
	{&DDRB, &PINB, &PORTB, PB7},
	{&DDRB, &PINB, &PORTB, PB5},
	{&DDRB, &PINB, &PORTB, PB4},
	{&DDRE, &PINE, &PORTE, PE3}
};

keyboard_descr_t col_pins[] = {
	{&DDRE, &PINE, &PORTE, PE5},
	{&DDRH, &PINH, &PORTH, PH3},
	{&DDRH, &PINH, &PORTH, PH5},
	{&DDRL, &PINL, &PORTL, PL1}	 
};

void init_matrix_keyboard(void) {
	// Initialize rows as outputs and set them high
	for (uint8_t i = 0; i < 4; i++) {
		*(row_pins[i].ddr) |= _BV(row_pins[i].bit);  // Set as output
		*(row_pins[i].port) |= _BV(row_pins[i].bit); // Set high
	}
	
	// Initialize columns as inputs with pull-ups enabled
	for (uint8_t i = 0; i < 4; i++) {
		*(col_pins[i].ddr) &= ~_BV(col_pins[i].bit);  // Set as input
		*(col_pins[i].port) |= _BV(col_pins[i].bit); // Enable pull-up
	}
}

uint8_t get_matrix_key_label(){
	return get_key_label(get_matrix_key());
}

int8_t get_matrix_key() {
	for (uint8_t row = 0; row < 4; row++) {
		// Activate row (set low)
		*(row_pins[row].port) &= ~_BV(row_pins[row].bit);
		
		for (uint8_t col = 0; col < 4; col++) {
			// Check if any column is low (button pressed)
			if ((*(col_pins[col].pin) & _BV(col_pins[col].bit)) == 0) {
				// Deactivate row (set high)
				*(row_pins[row].port) |= _BV(row_pins[row].bit);
				return (row * 4) + col + 1; // Return key number (1-based index)
			}
		}
		
		// Deactivate row (set high)
		*(row_pins[row].port) |= _BV(row_pins[row].bit);
	}
	return 0; // No key pressed
}

uint8_t get_key_label(uint8_t key_index) {
	uint8_t key_labels[17] = {
		255,  
		1, 2, 3, 10,   
		4, 5, 6, 11,   
		7, 8, 9, 12,   
		15, 0, 14, 13};  
	if (key_index > 0 && key_index < 17) {
		return key_labels[key_index];
	}
	return 255; //invalid input
}

uint16_t get_user_input(bool *cancelled, uint16_t initial_value, display_callback_t display_callback) {
	uint16_t input_value = 0;
	uint8_t key_index;
	uint8_t key_label;

	*cancelled = false;  // Initialize the cancelled flag

	while (true) {
		key_index = get_matrix_key();  // Get the key pressed
		if (key_index != 0) {          // If a key is pressed
			key_label = get_key_label(key_index);  // Get the label of the key

			if (key_label == 255) continue;  // Skip processing if the input is invalid
			
			if (key_label == 14) {  // If '#' (enter)
				return input_value;  // Return the collected input
			} 
			else if (key_label == 15) {  // If '*' (cancel)
				*cancelled = true;  // Set the cancelled flag
				return initial_value;  
			} 
			else if (key_label <= 9 && key_label >= 0) {  // Check if it's a valid digit
				input_value = input_value * 10 + key_label;  // Append it to the input value
			}

			if (display_callback != NULL) {
				display_callback(input_value);  // Call the callback to update the display
			}

			_delay_ms(200);  // Debounce delay
		}
	}
}





