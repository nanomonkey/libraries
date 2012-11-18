#include <avr/io.h>



void Kpd_Init() {
	PORTA |= 0xf0;
	DDRA &= ~0xf0;    // key0 - key3 are inputs with pullup
}



void Kpd_Reset() {
}



#define PBIT(n) (1 << (n))
#define NBIT(n) (0x0f ^ PBIT(n))
#define SCAN(n) {PORTA |= NBIT(n); PORTA &= ~PBIT(n); DDRA &= ~NBIT(n); DDRA |= PBIT(n); }
#define NOSCAN { PORTA |= 0x0f; DDRA &= ~0x0f; }


int Kpd_Scan() {
	unsigned char i, row;

	for (i=0; i<4; i++) {
		SCAN(i);
		row = ~((PINA & 0xf0) >> 4);
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
