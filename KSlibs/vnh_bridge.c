#include <stdint.h>
#include "vnh_bridge.h"
#include "adc.h"

void vnh_reset(vnh_s* v){
	// Go into standby mode
	vnh_standby(v);
	// Set pins for switches and PWM as outputs
	gpio_set_ddr(&v->mota);
	gpio_set_ddr(&v->motb);
	gpio_set_ddr(&v->pwm);
	// Set enable pins as inputs
	gpio_clr_ddr(&v->ena);
	gpio_clr_ddr(&v->enb);
	// Reset averaging function
	vnh_avg_reset(v);
	// Reset status flags
	v->status.fault_A = 0;
	v->status.fault_B = 0;
	v->status.limit = 0;
	// Toggle motor outputs to clear any faults
	gpio_set_pin(&v->mota);
	gpio_set_pin(&v->motb);
	gpio_clr_pin(&v->mota);
	gpio_clr_pin(&v->motb);
}

void vnh_standby(vnh_s* v){
	// Set status to stop PWM
	v->status.mode = VNH_STANDBY;
	// Disable A, B
	gpio_clr_pin(&v->ena);
	gpio_clr_pin(&v->enb);
	// Reset PWM counters
	vnh_pwm_reset(v);
}

void vnh_forward(vnh_s* v){
	// Momentarily go to standby
	vnh_standby(v);
	// A=hi, B=lo
	gpio_set_pin(&v->mota);
	gpio_clr_pin(&v->motb);	
	// Enable outputs
	gpio_set_pin(&v->ena);
	gpio_set_pin(&v->enb);
	// Set status
	v->status.mode = VNH_FORWARD;
}
	
void vnh_reverse(vnh_s* v){
	// Momentarily go to standby
	vnh_standby(v);
	// A=lo, B=hi
	gpio_clr_pin(&v->mota);
	gpio_set_pin(&v->motb);
	// Enable outputs
	gpio_set_pin(&v->ena);
	gpio_set_pin(&v->enb);
	// Set status
	v->status.mode = VNH_REVERSE;
}

void vnh_brake(vnh_s* v){
	// Momentarily go to standby
	vnh_standby(v);
	// A=lo, B=lo
	gpio_clr_pin(&v->mota);
	gpio_clr_pin(&v->motb);
	// Enable outputs
	gpio_set_pin(&v->ena);
	gpio_set_pin(&v->enb);
	v->status.mode = VNH_BRAKE;
}	

void vnh_avg_reset(vnh_s* v){
	int x;
	// Clear sample buffer
	for (x=0; x < VNH_SAMPLES; x++) {
		v->avg.samples[x] = 0;
	}
	v->avg.head = 0;
	v->avg.accum = 0;
}

void vnh_pwm_reset(vnh_s* v){
	// Go straight to target duty in fixed mode
	if (v->mod.mode == VNH_PWM_FIXED) 
		v->mod.duty = v->mod.target;
	// Otherwise, start at zero.  PWM code will ramp up
	else v->mod.duty = 0;
	// Reset cycle counter
	v->mod.count = 0;
	// Turn off PWM output
	gpio_clr_pin(&v->pwm);
}

void vnh_tick(vnh_s* v){
	static unsigned int adcpre = 0;
	vnh_pwm_tick(v);
	// ADC sampling is pre-scaled
	// if (++adcpre > 10){
		// vnh_adc_tick(v);
		// adcpre=0;
	// }
	// Fault checks
	v->status.fault_A = !gpio_get_pin(&v->ena);
	v->status.fault_B = !gpio_get_pin(&v->enb);
}

void vnh_pwm_tick(vnh_s* v){
	vnh_status_s stat = v->status;
	switch (stat.mode) {
		case VNH_STANDBY:
			// Just let it go...
			gpio_clr_pin(&v->pwm);
			break;
		case VNH_BRAKE:
			// Clamp down hard!
			gpio_set_pin(&v->pwm);
			break;
		case VNH_FORWARD:
		case VNH_REVERSE:
		default:
			// Everything else in modulation
			if (v->mod.mode != VNH_PWM_NONE) {
				if (v->mod.count < v->mod.duty) 
					gpio_set_pin(&v->pwm);
				else 
					gpio_clr_pin(&v->pwm);
				v->mod.count++;
				if (v->mod.count > VNH_MOD_MAX)
					v->mod.count = 0;
				// Only change duty at the beginning of a cycle
				if (v->mod.count == 0) {
					// Ramp down in limit mode
					if (v->status.limit) {
						if (v->mod.duty < v->mod.ramp) v->mod.duty = 0;
						else v->mod.duty -= v->mod.ramp;
					}
					// Otherwise, ramp towards target duty
					else {
						if (v->mod.duty < v->mod.target) {
							if (v->mod.target - v->mod.duty < v->mod.ramp)
								v->mod.duty = v->mod.target;
							else v->mod.duty += v->mod.ramp;
						}
						else {
							if (v->mod.duty - v->mod.target < v->mod.ramp)
								v->mod.duty = v->mod.target;
							else v->mod.duty -= v->mod.ramp;
						}
					}
				}	
			}
			else gpio_set_pin(&v->pwm);
			break;
	}
}
void vnh_adc_tick(vnh_s* v){
	if (v->avg.head < (VNH_SAMPLES - 1)) {
		v->avg.accum -= v->avg.samples[++v->avg.head];				// Subtract sample at tail and advance head
		v->avg.samples[v->avg.head] = ADC_ReadChanSync(v->adc);		// Overwrite old sample
		v->avg.accum += v->avg.samples[v->avg.head];				// Add the latest sample
	}
	else {
		// Roll over
		v->avg.accum -= v->avg.samples[0];				// Subtract sample at tail and advance head
		v->avg.samples[0] = ADC_ReadChanSync(v->adc);	// Overwrite old sample
		v->avg.accum += v->avg.samples[0];				// Add the latest sample
		v->avg.head = 0;
	}
	v->avg.avg = (v->avg.accum >> VNH_DIVISOR);		// Divide the accumulator
	if (v->climit) {
		// If we're over the limit current go into limit mode
		if (v->avg.avg > v->climit) {
			v->status.limit = 1;
		}
		// If we're in limit mode but no longer above the limit current
		// (minus our hysteresis value), go back to normal mode
		if (v->status.limit && (v->avg.avg < (v->climit - v->chyst))) {
			v->status.limit = 0;
		}
	}
}

unsigned int vnh_get_current(vnh_s* v){
	return v->avg.avg;
}
vnh_status_s vnh_get_status(vnh_s *v){
	return v->status;
}
uint8_t vnh_get_fault(vnh_s* v) {
	return v->status.fault_A | v->status.fault_B | v->status.limit;
}

void vnh_set_period(vnh_s* v, unsigned int period){}
void vnh_set_duty(vnh_s* v, unsigned int duty){}