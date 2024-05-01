#include "gtest/gtest.h"

extern "C" {
#include <avr/io.h>
#include "leds/led_array.h"
}

#define OVER_LIMIT_LED 9 
#define UNDER_LIMIT_LED 0
#define UNDER_LIMIT_BAR -1
#define CORRECT_LED 3
#define WRONG_STATE 99

class LEDArrayTest : public ::testing::Test {
protected:
	void SetUp() override {
		init_leds();
	}
	void TearDown() override{

	}
};

TEST_F(LEDArrayTest, led_direction_set_to_output) {
  EXPECT_EQ(DDRA, 0xFF);
}

TEST_F(LEDArrayTest, leds_off_after_init){
	EXPECT_EQ(PORTA, 0xFF);
}
TEST_F(LEDArrayTest, set_led_with_over_limit_led_should_return_error){
	int8_t result = set_led(OVER_LIMIT_LED, ON);
	EXPECT_EQ(result, ERROR);
}
TEST_F(LEDArrayTest, set_led_with_under_limit_led_should_return_error) {
	int8_t result = set_led(UNDER_LIMIT_LED, ON);
	EXPECT_EQ(result, ERROR);
}
TEST_F(LEDArrayTest, set_led_with_correct_led_should_return_success) {
	int8_t result = set_led(CORRECT_LED, ON);
	EXPECT_EQ(result, SUCCESS);
}
TEST_F(LEDArrayTest, set_led_with_wrong_state_should_return_error) {
	int8_t result = set_led(CORRECT_LED, WRONG_STATE);
	EXPECT_EQ(result, ERROR);
}
TEST_F(LEDArrayTest, set_led_with_correct_state_should_return_success) {
	int8_t result = set_led(CORRECT_LED, ON);
	EXPECT_EQ(result, SUCCESS);
}
TEST_F(LEDArrayTest, set_led_on_with_correct_led_should_set){
	int8_t result = set_led(CORRECT_LED, ON);
	EXPECT_EQ(PORTA, 0xFB);
	EXPECT_EQ(result, SUCCESS);
}
TEST_F(LEDArrayTest, set_led_off_with_correct_led_should_clear) {
	int8_t result = set_led(CORRECT_LED, OFF);
	EXPECT_EQ(PORTA, 0xFF);
	EXPECT_EQ(result, SUCCESS);
}

TEST_F(LEDArrayTest, lightbar_with_over_limit_led_should_return_error) {
	int8_t result = lightbar(OVER_LIMIT_LED);
	EXPECT_EQ(result, ERROR);
}

TEST_F(LEDArrayTest, lightbar_with_under_limit_led_should_return_error) {
	int8_t result = lightbar(UNDER_LIMIT_BAR);
	EXPECT_EQ(result, ERROR);
}

TEST_F(LEDArrayTest, lightbar_with_correct_leds_sets_leds) {
	int8_t result = lightbar(CORRECT_LED);
	EXPECT_EQ(PORTA, 0xF8);
	EXPECT_EQ(result, SUCCESS);
}
