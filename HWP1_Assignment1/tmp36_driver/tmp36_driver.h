/*
 * tmp36_driver.h
 *
 * Created: 20/04/2024 20.11.08
 *  Author: jurin
 */ 


#ifndef TMP36_DRIVER_H_
#define TMP36_DRIVER_H_

#include <stdint.h>

#define ENABLE_PIN PG0
#define OUT_PIN PK7

volatile extern uint8_t new_measurement_flag;

void tmp36_init(void);
void tmp36_process(float* temperature);
float tmp36_calculate_celsius(uint16_t adc_value);






#endif /* TMP36_DRIVER_H_ */