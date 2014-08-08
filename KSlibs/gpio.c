#include <stdint.h>
#include "gpio.h"

#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

// Set bit in DDR
void gpio_set_ddr(gpio_s* p){
	*(p->port - 1) |= _BV(p->pin);
}
// Clear bit in DDR
void gpio_clr_ddr(gpio_s* p){
	*(p->port - 1) &= 0xFF & ~_BV(p->pin);
}
// Set pin in PORT
void gpio_set_pin(gpio_s* p){
	*p->port |= _BV(p->pin);
}
// Clear pin in PORT
void gpio_clr_pin(gpio_s* p){
	*p->port &= 0xFF & ~_BV(p->pin);
}
// Get PIN value
unsigned char gpio_get_pin(gpio_s* p){
	return *(p->port - 2);
}