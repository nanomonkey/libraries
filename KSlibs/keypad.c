#include <avr/io.h>

void Kpd_Init() {
	Kpd_Reset();
}

void Kpd_Reset() {
	PORTA = 0xf0;	// Makse sure all key pins are set for input
	DDRA = 0x0f;	// key0 - key3 are inputs with pullup
	
}


#define PBIT(n) (1 << (n))  // Set a single bit
#define NBIT(n) (0x0f ^ PBIT(n))  // Clear a single bit, but keep the rest set (low nibble)
#define SCAN(n) {PORTA |= NBIT(n); PORTA &= ~PBIT(n); DDRA &= ~NBIT(n); DDRA |= PBIT(n); }
#define NOSCAN { PORTA |= 0x0f; DDRA &= ~0x0f; }  // Set all bits in lower nibble; clear all bit in lower nibble of DDR


int Kpd_Scan() {
	unsigned char i, row;
	Kpd_Reset();  // This fixes a bug where keys aren't read after the display is cleared
	for (i=0; i<4; i++) {
		SCAN(i);
		row = ~((PINA & 0xf0) >> 4);  // Read key bits from upper nibble and shift to lower nibble, then invert
		if (row & 0x01) return i << 2;
		if (row & 0x02) return (i << 2) + 1;
		if (row & 0x04) return (i << 2) + 2;
		if (row & 0x08) return (i << 2) + 3;
	}
	NOSCAN;
	return -1;
}


#undef PBIT
#undef NBIT
#undef SCAN
#undef NOSCAN


int Kpd_GetKeyAsync() {
	static int keylast = -1;

	int keynow;

	keynow = Kpd_Scan();
	if (keynow != keylast) {
		keylast = keynow;
		return keynow;
	}
	return -1;
}



void Kpd_WaitKeyUp() {
	while (Kpd_Scan() != -1) ;
}
