#include <avr/io.h>


#include "fet.h"


int Fet_Data[8];

static uint16_t IO_addresses[8] = {
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL)
};

static uint8_t IO_bits[8] = {
	0x08,
	0x10,
	0x20,
	0x08,
	0x10,
	0x20,
	0x08,
	0x10
};



static void Fet_Set(unsigned n, unsigned value) {
	if (value) {
	_SFR_IO8(IO_addresses[n]) |= IO_bits[n];
	} else {
	_SFR_IO8(IO_addresses[n]) &= ~IO_bits[n];
	}
}



void Fet_Init() {
	PORTE &= ~0x38;
	DDRE |= 0x38;   // fet0, fet1, fet2

	PORTH &= ~0x38; // fet3, fet4, fet5
	DDRH |= 0x38;

	PORTL &= ~0x18; // fet6, fet7
	DDRL |= 0x18;

	Fet_Reset();
}



void Fet_Reset() {
	int i;

	for (i=0; i<8; i++) {
		Fet_Data[i] = 0;
	}
	Fet_WriteAll();
}



void Fet_WriteAll() {
	int i;

	for (i=0; i<8; i++) {
		Fet_Set(i, Fet_Data[i]);
	}
}

