#include "gtest/gtest.h"
#include "fake_avr_sfr.c"

extern "C" {
#include <avr/interrupt.h>
#include <avr/io.h>
#include "buttons/buttons.h"
}

#define OVER_LIMIT_BUTTON 9 
#define UNDER_LIMIT_BUTTON 0
#define CORRECT_BUTTON_REGC 3
#define CORRECT_BUTTON_REGD 8

class ButtonTest : public ::testing::Test {
protected:
	void SetUp() override {
		init_keys();
	}
	void TearDown() override {
		PINC = 0xFF;
		PIND = 0xFF;
	}
};

TEST_F(ButtonTest, init_keys_should_set_pins_input) {
	EXPECT_EQ(DDRC & 0x3F, 0x00);
	EXPECT_EQ(DDRD & 0x0C, 0x00);
}

TEST_F(ButtonTest, init_keys_should_set_pins_pullup){
	EXPECT_EQ(PORTC & 0x3F, 0x3F);
	EXPECT_EQ(PORTD & 0x0C, 0x0C);
}

TEST_F(ButtonTest, get_key_with_over_limit_button_should_return_error) {
	int8_t result = get_key(OVER_LIMIT_BUTTON);
	EXPECT_EQ(result, ERROR);
}

TEST_F(ButtonTest, get_key_with_under_limit_button_should_return_error) {
	int8_t result = get_key(UNDER_LIMIT_BUTTON);
	EXPECT_EQ(result, ERROR);
}

TEST_F(ButtonTest, get_key_in_register_C_with_all_buttons_unpressed_should_return_off) {
	int8_t result = get_key(CORRECT_BUTTON_REGC);
	EXPECT_EQ(result, OFF);
}

TEST_F(ButtonTest, get_key_in_register_D_with_all_buttons_unpressed_should_return_off) {
	int8_t result = get_key(CORRECT_BUTTON_REGD);
	EXPECT_EQ(result, OFF);
}

TEST_F(ButtonTest, get_key_in_register_C_with_pressed_button_should_return_on) {
	// Simulate button 3 (PC2) is pressed by clearing the bit
	PINC = ~(1 << (CORRECT_BUTTON_REGC - 1));

	int8_t result = get_key(CORRECT_BUTTON_REGC);
	EXPECT_EQ(result, ON);
}

TEST_F(ButtonTest, get_key_in_register_D_with_pressed_button_should_return_on) {
	// button8 = PD3, button7 = PD2
	PIND = ~(1 << (CORRECT_BUTTON_REGD - 5));

	int8_t result = get_key(CORRECT_BUTTON_REGD);
	EXPECT_EQ(result, ON);
}

TEST_F(ButtonTest, scan_key_in_register_C_with_pressed_button_should_return_index) {
	PINC = ~(1 << (CORRECT_BUTTON_REGC - 1));

	uint8_t result = scan_key();
	EXPECT_EQ(result, CORRECT_BUTTON_REGC);
}

TEST_F(ButtonTest, scan_key_in_register_D_with_pressed_button_should_return_index) {
	// button8 = PD3, button7 = PD2
	PIND = ~(1 << (CORRECT_BUTTON_REGD - 5));

	uint8_t result = scan_key();
	EXPECT_EQ(result, CORRECT_BUTTON_REGD);
}

TEST_F(ButtonTest, scan_key_with_pressed_multiple_buttons_should_return_lowest) {
	PINC = ~(1 << (CORRECT_BUTTON_REGC - 1));
	PINC = ~(1 << (CORRECT_BUTTON_REGC - 2));

	uint8_t result = scan_key();
	EXPECT_EQ(result, CORRECT_BUTTON_REGC - 1);
}

TEST_F(ButtonTest, scan_key_with_with_no_button_pressed_should_return_zero) {
	uint8_t result = scan_key();
	EXPECT_EQ(result, 0);
}



