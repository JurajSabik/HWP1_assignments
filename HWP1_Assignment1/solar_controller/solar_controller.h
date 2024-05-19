/*
 * solar_controller.h
 *
 * Created: 18/05/2024 13.51.33
 *  Author: jurin
 */ 

#include "../leds/led_array.h"

#ifndef SOLAR_CONTROLLER_H_
#define SOLAR_CONTROLLER_H_

// Button wrapper
enum BUTTONS {
	NO_PRESS = 0,
	T_LOW_BUTTON = 1,
	T_HIGH_BUTTON = 2,
	T_CURRENT_BUTTON = 3
};

// Led wrapper
enum LEDS {
	T_LOW_LED = 1,
	T_HIGH_LED = 2,
	T_CURRENT_LED = 3,
	T_VALVE_LED = 6,
	T_PUMP_LED = 7
};

enum SYSTEM_STATE {
	STATE_CURRENT_TEMP,
	STATE_LOW_TEMP,
	STATE_HIGH_TEMP
	};

#define SET_TEMP 10 // value on the matrix keyboard that sets the temp


void set_state_leds(uint8_t led_no, uint8_t state);
void set_control_leds(uint8_t led_no, uint8_t state);





#endif /* SOLAR_CONTROLLER_H_ */