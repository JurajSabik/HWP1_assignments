/*
 * tmp36_driver.h
 *
 * Created: 20/04/2024 20.11.08
 *  Author: jurin
 */ 


#ifndef TMP36_DRIVER_H_
#define TMP36_DRIVER_H_

static int16_t temperature_celsius;

uint8_t tmp36_init(void);
void tmp36_process(void);
uint16_t tmp36_get_raw_temp(void);
void tmp36_calculate_celsius(void);






#endif /* TMP36_DRIVER_H_ */