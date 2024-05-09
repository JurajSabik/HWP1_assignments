/*
 * _7_segment.c
 *
 * Created: 07/05/2024 18.37.08
 *  Author: jurin
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "display.h"

volatile uint8_t current_digit = 0;
volatile uint8_t digits_to_display[MAX_DIGITS] = {0};

//   _a_
// f|   |b
//  |_g_|
// e|   |c
//  |_d_|.h

const uint8_t SEGMENT_MAP[10] = {
	0x03, // 0
	0x9F, // 1
	0x25, // 2
	0x0D, // 3
	0x99, // 4
	0x49, // 5
	0x41, // 6
	0x1F, // 7
	0x01, // 8
	0x09  // 9
};

ISR(TIMER3_COMPA_vect) {
	// Clear all digit control lines first
	DISPLAY_CONTROL_PORT |= DISPLAY_CONTROL_PINS;

	// Send segments to display for current digit
	send_segments_to_display(digits_to_display[current_digit]);

	// Activate current digit
	set_digit(current_digit);
		
	// Prepare for the next digit
	current_digit = (current_digit + 1) % MAX_DIGITS;
}

void init_display(){
	
	// Set SI, SCX, RCK as output
	DDRB |= (1 << SI_PIN) | (1 << SCK_PIN) | (1 << RCK_PIN);

	// Set display control pins as output
	DDRF |= DISPLAY_CONTROL_PINS;
	
	DISPLAY_CONTROL_PORT |= DISPLAY_CONTROL_PINS;

	// Initialize the timer for interrupt-driven refresh
	TCCR3B |= (1 << WGM32);  // Configure timer 3 for CTC mode
	OCR3A = 1200;  // Set compare value for 2ms interval at 16 MHz clock with prescaler of 64
	TIMSK3 |= (1 << OCIE3A); // Enable Timer3 compare match A interrupt
	TCCR3B |= (1 << CS31) | (1 << CS30); // Start timer with prescaler 64
}

int8_t printint_4u(uint16_t value){
	if(value > 9999){
		printf("Number too large to show.");
		return -1;
	}
	uint8_t segments[MAX_DIGITS] = {0};
	
	// Fill the segments array with bit representations of each digit 
	split_to_digits_and_bits(value, segments);
	
	// Copy the segment data to the global digits_to_display array used by the ISR
	for (int i = 0; i < MAX_DIGITS; i++) {
		digits_to_display[i] = segments[i];
	}
	
	// Enable interrupts
	sei();
	return 0;
}

void send_segments_to_display(uint8_t segment) {
	for (int bit = 0; bit < 8; bit++) {
		// Send each bit starting from LSB to MSB
		uint8_t bitVal = (segment >> bit) & 0x01;
		send_bit_to_display(bitVal);
	}
	toggle_register_clock(); // Latch the bits to the output register after all bits are sent
}

void send_bit_to_display(uint8_t bitVal) {
	if (bitVal) {
		SI_PORT |= (1 << SI_PIN); // Set SI to high
		} else {
		SI_PORT &= ~(1 << SI_PIN); // Set SI to low
	}
	toggle_shift_clock(); // Clock the bit into the shift register
}

void toggle_shift_clock(void) {
	SCK_PORT &= ~(1 << SCK_PIN); // Clock low
	_delay_us(1);                // Short delay
	SCK_PORT |= (1 << SCK_PIN);  // Clock high
	_delay_us(1);                // Short delay
	SCK_PORT &= ~(1 << SCK_PIN); // Clock low again
}

void toggle_register_clock(void) {
	RCK_PORT &= ~(1 << RCK_PIN); // Clock low
	_delay_us(1);                // Short delay
	RCK_PORT |= (1 << RCK_PIN);  // Clock high
	_delay_us(1);                // Short delay
	RCK_PORT &= ~(1 << RCK_PIN); // Clock low again
}

void set_digit(uint8_t digitNum) {
	DISPLAY_CONTROL_PORT |= DISPLAY_CONTROL_PINS;   // Turn off all digits
	DISPLAY_CONTROL_PORT &= ~(1 << (DISPLAY_PIN_4 - digitNum));		// Turn on selected digit
}

void split_to_digits_and_bits(uint16_t value, uint8_t *digit_bits) {
	for (int i = 0; i < MAX_DIGITS; i++) {
		uint8_t digit = value % 10;				// Extract least significant digit
		digit_bits[i] = SEGMENT_MAP[digit];		// Convert digit to segment encoding
		value /= 10;							// Remove the extracted digit
	}
}

	