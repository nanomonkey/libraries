#ifndef GPIO_H
#define GPIO_H
typedef struct gpio {
	volatile uint8_t *port;
	unsigned char pin;
} gpio_s;

#ifdef __cplusplus
extern "C" {
#endif
void gpio_set_ddr(gpio_s*);				// Set bit in DDR
void gpio_clr_ddr(gpio_s*);				// Clear bit in DDR
void gpio_set_pin(gpio_s*);				// Set pin in PORT
void gpio_clr_pin(gpio_s*);				// Clear pin in PORT
unsigned char gpio_get_pin(gpio_s*);	// Get PIN value

#ifdef __cplusplus
}
#endif

#endif // GPIO_H