/*
 * matrix_keyboard.h
 *
 * Created: 22/04/2024 12.37.12
 *  Author: jurin
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <util/delay.h>

#ifndef MATRIX_KEYBOARD_H_
#define MATRIX_KEYBOARD_H_

// Define a type for the callback function that handles displaying numbers
typedef void (*display_callback_t)(uint16_t value);

void init_matrix_keyboard(void);
int8_t get_matrix_key(void);

uint8_t get_key_label(uint8_t key_index);
uint16_t get_user_input(bool *cancelled, display_callback_t display_callback);

#endif /* MATRIX_KEYBOARD_H_ */