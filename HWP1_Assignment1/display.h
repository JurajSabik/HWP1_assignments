/*
 * display.h
 *
 * Created: 07/05/2024 20.40.54
 *  Author: jurin
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

//   _a_
// f|   |b
//  |_g_|
// e|   |c
//  |_d_|.h

//#define SEG_0 0x03	//0b00000011
//#define SEG_1 0x9F	//0b10011111
//#define SEG_2 0x2B	//0b00101011
//#define SEG_3 0x0D	//0b00001101
//#define SEG_4 0x93	//0b10010011
//#define SEG_5 0x49	//0b01001001
//#define SEG_6 0x41	//0b01000001
//#define SEG_7 0x1F	//0b00011111
//#define SEG_8 0x01	//0b00000001
//#define SEG_9 0x09	//0b00001001

#define SI_PORT PORTB
#define SI_PIN  PB2		// data in
#define SCK_PORT PORTB
#define SCK_PIN  PB1	// shift register clock (1 pulse to shift 1 bit in the shift register)
#define RCK_PORT PORTB
#define RCK_PIN  PB0	// storage register clock (1 pulse to shift out all content to outputs)
#define DISPLAY_CONTROL_PORT PORTF
#define DISPLAY_CONTROL_PINS (0x0F)  // PF0 to PF3
#define DISPLAY_PIN_1 PF0
#define DISPLAY_PIN_2 PF1
#define DISPLAY_PIN_3 PF2
#define DISPLAY_PIN_4 PF3

// Maximum number of digits that can be showed on the display
#define MAX_DIGITS 4

volatile extern uint8_t current_digit;
volatile extern uint8_t digits_to_display[MAX_DIGITS]; // Array to hold the segment data for each digit

void init_display(void);
int8_t printint_4u(uint16_t value);

void send_bit_to_display(uint8_t bitVal);
void toggle_shift_clock(void);
void toggle_register_clock(void);
void set_digit(uint8_t digitNum);
void split_to_digits_and_bits(uint16_t value, uint8_t *digit_bits);
void send_segments_to_display(uint8_t segment);




#endif /* DISPLAY_H_ */