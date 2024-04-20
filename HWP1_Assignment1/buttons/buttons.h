/*
 * buttons.h
 *
 * Created: 10/04/2024 13.26.02
 *  Author: jurin
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#define OFF 0
#define ON 1
#define ERROR -1

void init_keys();
int8_t get_key(uint8_t key_no);
uint8_t scan_key();

#endif /* BUTTONS_H_ */