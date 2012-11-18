#include <avr/io.h>
#include <stdint.h>

#include "util.h"

int ANA_available[8];

void ADC_Init() {
	DDRL |= 0xc0;    // MUXA and MUXB are outputs
	PORTL = (PORTL & ~0xc0) | (0 << 6);  // bank number for external MUX
	
	ADMUX = 0x40;    // AVCC is AREF   
	ADCSRA |= 0x80;  // enable ADC
	ADCSRA |= 0x07;  // prescale by 128

	DIDR0 = 0x0f;    // disable digital inputs, ADC0 - ADC3
	DIDR2 = 0xff;    // disable digital inputs, ADC8 - ADC15
	
	if (GCU_version == V2) {
		ANA_available[0] = 1;
		ANA_available[1] = 1;
		ANA_available[2] = 1;
		ANA_available[3] = 1;
	    ANA_available[4] = 0;
		ANA_available[5] = 0;
		ANA_available[6] = 0;
		ANA_available[7] = 0;
	}
	if (GCU_version == V3 && GCU_fill == LITEFILL) {
		ANA_available[0] = 1;
		ANA_available[1] = 1;
		ANA_available[2] = 1;
		ANA_available[3] = 1;
	    ANA_available[4] = 0;
		ANA_available[5] = 0;
		ANA_available[6] = 0;
		ANA_available[7] = 0;
	} else {
		ANA_available[0] = 1;
		ANA_available[1] = 1;
		ANA_available[2] = 1;
		ANA_available[3] = 1;
	    ANA_available[4] = 1;
		ANA_available[5] = 1;
		ANA_available[6] = 1;
		ANA_available[7] = 1;
	}
}


void ADC_Reset() {
}


uint16_t ADC_ReadChanSync(unsigned chan) {
	uint16_t adc;

	if (chan >= 8) {
		ADCSRB |= 0x08;  // MUX high channels (8 - 15)
	} else {
		ADCSRB &= ~0x08; // MUX low channels (0 - 7)
	}

	ADMUX = (ADMUX & ~0x1f) | (chan & 0x07);  // MUX channel number

	ADCSRA |= 0x40;  // start conversion
	while (ADCSRA & 0x40) ;   // wait for conversion to complete
	
	adc = ADCL;
	adc |= ADCH << 8;
	return adc;
}
