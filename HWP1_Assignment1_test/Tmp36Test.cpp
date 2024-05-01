#include "gtest/gtest.h"

extern "C" {
#include <avr/io.h>
#include "tmp36_driver/tmp36_driver.h"

	void ISR_TIMER1_COMPA_vect();
	void ISR_ADC_vect();
}


class TMP36DriverTest : public ::testing::Test {
protected:
	void SetUp() override {
		tmp36_init();
	}
	void TearDown() override {

	}
};

TEST_F(TMP36DriverTest, init_should_set_adc_and_output) {
	EXPECT_EQ(new_measurement_flag, 0);

	// enable is output
	EXPECT_TRUE(DDRG & (1 << ENABLE_PIN));

	// enable is high
	EXPECT_TRUE(PORTG & (1 << ENABLE_PIN));

	// reference voltage VCC
	EXPECT_TRUE(ADMUX & (1 << REFS0));

	// ADC channel 15 is selected
	// MUX values for channel 15 are 100111
	EXPECT_EQ(ADMUX & 0x0F, 0x07); 
	EXPECT_TRUE(ADCSRB & (1 << MUX5)); 

	// ADC is enabled and prescaler is set to 128
	EXPECT_EQ(ADCSRA, (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADIE));
}

TEST_F(TMP36DriverTest, calculate_celsius_should_return_correct_celsius) {
	
	uint16_t test_adc_value = 205; 
	float expected_temperature = 50.1; 
	float calculated_temperature = tmp36_calculate_celsius(test_adc_value);

	EXPECT_NEAR(calculated_temperature, expected_temperature, 0.1);
}

TEST_F(TMP36DriverTest, process_should_update_passed_pointer_with_correct_value) {
	float expected_temperature = 50.1;
	float retrieved_temperature = 0;

	// Clear ADC start conversion flag
	ADCSRA &= ~(1 << ADSC);

	// Simulate timer compare match interrupt
	ISR_TIMER1_COMPA_vect();

	ADC = 205;
	ISR_ADC_vect();

	tmp36_process(&retrieved_temperature);
	EXPECT_NEAR(expected_temperature, retrieved_temperature, 0.1);

}

TEST_F(TMP36DriverTest, adc_interrupt_sets_flag) {
	// Simulate an ADC complete interrupt
	ADC = 300; // Set a mock ADC value
	ISR_ADC_vect(); // Call the ISR

	// Check if last_adc_value is updated correctly
	EXPECT_EQ(new_measurement_flag, 1);
}

TEST_F(TMP36DriverTest, timer_interrupt_should_trigger_conversion) {
	// Clear ADC start conversion flag
	ADCSRA &= ~(1 << ADSC);

	// Simulate timer compare match interrupt
	ISR_TIMER1_COMPA_vect();

	// Check if ADC start conversion flag is set
	EXPECT_TRUE(ADCSRA & (1 << ADSC));
}


