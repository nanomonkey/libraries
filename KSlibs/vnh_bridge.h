/*
  Driver API for VNH2SP30
*/

#include "gpio.h"

#define VNH_UAMP_UNIT (37538)		// Unit for ADC to AMP conversion
#define VNH_PWM_PERIOD ()
#define VNH_SAMPLES (64)
#define VNH_DIVISOR (6)

typedef struct {
	unsigned char enabled:1;			// 0=standby, 1=go
	unsigned char direction:1;			// 0=FWD, 1=REV
	unsigned char fault_A:1;			// Fault detected on channel A
	unsigned char fault_B:1;			// Fault detected on channel B
	unsigned char current:2;			// 0=LOW, 1=MED, 2=HIGH
	unsigned char limit:1;				// 1 if over-current threshold reached
} vnh_status_s;

typedef struct {
	unsigned int head;
	unsigned long accum;
	unsigned int samples[VNH_SAMPLES];
} vnh_sbuf_s;

typedef struct {
	unsigned int max;
	unsigned int duty;
	unsigned int count;
	unsigned int ramp;
} vnh_mod_s;

typedef struct {
	vnh_status_s status;
	gpio_s mota;
	gpio_s motb;
	gpio_s ena;
	gpio_s enb;
	gpio_s pwm;
	unsigned int csense_adc;	// ADC channel for current sense
	unsigned int max_current;	// Current limit, in milliamps
	vnh_mod_s mod;				// Modulation
	vnh_sbuf_s avg;
} vnh_s;

#ifdef __cplusplus
extern "C" {
#endif

void vnh_reset(vnh_s*);		// Reset, clear fault
void vnh_forward(vnh_s*);	// A=hi, B=lo
void vnh_reverse(vnh_s*);	// A=lo, B=hi
void vnh_brake(vnh_s*);		// A=lo, B=lo
void vnh_standby(vnh_s*);	// Disable A, B

void vnh_tick(vnh_s*);

unsigned int vnh_get_current(vnh_s*); // Returns average current
vnh_status_s vnh_get_status(vnh_s*);

#ifdef __cplusplus
}
#endif