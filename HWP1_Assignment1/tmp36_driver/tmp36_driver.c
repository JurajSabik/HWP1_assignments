/*
 * tmp36_driver.c
 *
 * Created: 20/04/2024 20.11.20
 *  Author: jurin
 */ 

#include "tmp36_driver.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>

volatile uint8_t new_measurement_flag;

void tmp36_init(void) {

	new_measurement_flag = 0;

	// Enable is output
	DDRG |= (1 << ENABLE_PIN);

	// Enable is high
	PORTG |= (1 << ENABLE_PIN);

	// Reference voltage VCC (5V)
	ADMUX = (1 << REFS0);

	// ADC channel 15 is selected
	ADMUX |= (1 << MUX0) | (1 << MUX1) | (1 << MUX2);
	ADCSRB |= (1 << MUX5);

	// ADC is enabled and prescaler is set to 128
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// Enable ADC interrupt
	ADCSRA |= (1 << ADIE);

	// Set up timer 1 to trigger once per second
	TCCR1A = 0; // Normal mode
	TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, prescaler 1024
	OCR1A = 15624; // 16MHz Clock / 1024 prescaler / 1 Hz frequency - 1
	TIMSK1 |= (1 << OCIE1A); // Enable timer compare interrup
}

void tmp36_process(float* temperature)
{
	if (!new_measurement_flag) {
		return;
	}
	uint16_t adc_value = ADC;
	*temperature = tmp36_calculate_celsius(adc_value);
	new_measurement_flag = 0;
}

float tmp36_calculate_celsius(uint16_t adc_value) {
	// Convert ADC value to temperature in Celsius
	volatile float voltage = adc_value * 5.0 / 1024.0;
	float celsius = (voltage - 0.5) * 100;
	return celsius;
}

// ISR for ADC conversion complete
ISR(ADC_vect) {
	new_measurement_flag = 1;
}

// Timer1 ISR to trigger ADC conversion
ISR(TIMER1_COMPA_vect) {
	// Start an ADC conversion every second
	ADCSRA |= (1 << ADSC);
}
