#include <stdint.h>
#include "vnh_bridge.h"
#include "adc.h"


void vnh_reset(vnh_s* v){
	// Set pins for switches and PWM as outputs
	gpio_set_ddr(&v->mota);
	gpio_set_ddr(&v->motb);
	gpio_set_ddr(&v->pwm);
	// Set enable pins as inputs
	gpio_clr_ddr(&v->ena);
	gpio_clr_ddr(&v->enb);
	// Go into standby mode
	vnh_standby(v);
	// Clear the sample buffer
	int x;
	for (x=0; x < VNH_SAMPLES; x++) {
		v->avg.samples[x] = 0;
	}
	v->avg.head = 0;
	v->avg.accum = 0;
}
void vnh_forward(vnh_s* v){
	v->mod.duty = 0;
	v->mod.count = 0;
	// A=hi, B=lo
	gpio_set_pin(&v->mota);
	gpio_clr_pin(&v->motb);	
	// Enable outputs
	gpio_set_pin(&v->ena);
	gpio_set_pin(&v->enb);
	// Turn on PWM
	v->status.enabled = 1;
}	
void vnh_reverse(vnh_s* v){
	v->mod.duty = 0;
	v->mod.count = 0;
	// A=lo, B=hi
	gpio_clr_pin(&v->mota);
	gpio_set_pin(&v->motb);
	// Enable outputs
	gpio_set_pin(&v->ena);
	gpio_set_pin(&v->enb);
	// Turn on PWM
	v->status.enabled = 1;
}
void vnh_brake(vnh_s* v){
	// Max duty
	v->mod.duty = v->mod.max;
	// A=lo, B=lo
	gpio_clr_pin(&v->mota);
	gpio_clr_pin(&v->motb);
	// Enable outputs
	gpio_set_pin(&v->ena);
	gpio_set_pin(&v->enb);
}
void vnh_standby(vnh_s* v){
	// Disable PWM
	v->status.enabled = 0;
	// Disable A, B
	gpio_clr_pin(&v->ena);
	gpio_clr_pin(&v->enb);
	// Turn off PWM
	gpio_clr_pin(&v->pwm);
	// Toggle inputs to clear any faults
	gpio_set_pin(&v->mota);
	gpio_set_pin(&v->motb);
	gpio_clr_pin(&v->mota);
	gpio_clr_pin(&v->motb);
}	

void vnh_tick(vnh_s* v){
	if (v->avg.head < (VNH_SAMPLES - 1)) {
		v->avg.accum -= v->avg.samples[++v->avg.head];						// Subtract sample at tail and advance head
		v->avg.samples[v->avg.head] = ADC_ReadChanSync(v->csense_adc);	// Overwrite old sample
		v->avg.accum += v->avg.samples[v->avg.head];						// Add the latest sample
	}
	else {
		// Roll over
		v->avg.accum -= v->avg.samples[0];									// Subtract sample at tail and advance head
		v->avg.samples[0] = ADC_ReadChanSync(v->csense_adc);				// Overwrite old sample
		v->avg.accum += v->avg.samples[0];									// Add the latest sample
		v->avg.head = 0;
	}
	if (v->status.enabled) {
		if (v->mod.count < v->mod.duty) gpio_set_pin(&v->pwm);
		else gpio_clr_pin(&v->pwm);
		v->mod.count++;
		if (v->mod.count > v->mod.max) {
			v->mod.count = 0;
			v->mod.duty += v->mod.ramp;
		}
	}
	else {
		gpio_clr_pin(&v->pwm);
	}
}

unsigned int vnh_get_current(vnh_s* v){
	return (v->avg.accum >> VNH_DIVISOR);
}
vnh_status_s vnh_get_status(vnh_s *v){
	return v->status;
}

void vnh_set_period(vnh_s* v, unsigned int period){}
void vnh_set_duty(vnh_s* v, unsigned int duty){}