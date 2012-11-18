#include <avr/io.h>

#include "util.h"
#include "adc.h"

// global GCU variables
int GCU_version;
int GCU_fill;
int GCU_psequence;
int n_pressure;

// mV / kPa
static long sensitivity[] = {286, 286, 1000, 1000, 0, 0};


// rough offset in counts
#define OFFSET (-512l)

// fine offset in Pa
static long offset[] = {0, 0, 0, 0, 0, 0};

// result in Pa
#define COMPENSATE(x,i) (( (((signed)(x)) + (OFFSET)) * (ADC_REF_V / ADC_MAXCOUNT) / sensitivity[i]) + offset[i])

long int Press_Data[6];
int Press_Available[6];

void Press_Init() {
	int i;
	switch (GCU_version) {
		case V2:
			sensitivity[0] = 286;
			sensitivity[1] = 286;
			sensitivity[2] = 1000;
			sensitivity[3] = 1000;
			sensitivity[4] = 0;
			sensitivity[5] = 0;
			if (GCU_fill == HALFFILL) {
				Press_Available[0] = 1;
				Press_Available[1] = 0;
				Press_Available[2] = 1;
				Press_Available[3] = 0;
				Press_Available[4] = 0;
				Press_Available[5] = 0;
			} else if (GCU_fill == FULLFILL) {
				Press_Available[0] = 1;
				Press_Available[1] = 1;
				Press_Available[2] = 1;
				Press_Available[3] = 1;
				Press_Available[4] = 0;
				Press_Available[5] = 0;
			}
			break;
 		 case V3:

 		  	if (GCU_psequence == P777222) {
 				sensitivity[0] = 286;
				sensitivity[1] = 286;
				sensitivity[2] = 286;
				sensitivity[3] = 1000;
				sensitivity[4] = 1000;
				sensitivity[5] = 1000;
		  	}
	      	if (GCU_psequence == P222777) {
 				sensitivity[0] = 1000;
				sensitivity[1] = 1000;
				sensitivity[2] = 1000;
				sensitivity[3] = 286;
				sensitivity[4] = 286;
				sensitivity[5] = 286;
		  	}
		  	if (GCU_psequence == P777722) {
		  	 	sensitivity[0] = 286;
				sensitivity[1] = 286;
				sensitivity[2] = 286;
				sensitivity[3] = 286;
				sensitivity[4] = 1000;
				sensitivity[5] = 1000;
		  	}
			if (GCU_fill == HALFFILL || GCU_fill == LITEFILL) {
				if (GCU_psequence == P777722) {
					Press_Available[0] = 1;
					Press_Available[1] = 0;
					Press_Available[2] = 0;
					Press_Available[3] = 0;
					Press_Available[4] = 1;
					Press_Available[5] = 0;
				} else {
					Press_Available[0] = 1;
					Press_Available[1] = 0;
					Press_Available[2] = 0;
					Press_Available[3] = 1;
					Press_Available[4] = 0;
					Press_Available[5] = 0;
				}
			} else if (GCU_fill == FULLFILL) {
				Press_Available[0] = 1;
				Press_Available[1] = 1;
				Press_Available[2] = 1;
				Press_Available[3] = 1;
				Press_Available[4] = 1;
				Press_Available[5] = 1;
			}
 			break;
 		default:
 		 	sensitivity[0] = 286;
			sensitivity[1] = 286;
			sensitivity[2] = 286;
			sensitivity[3] = 1000;
			sensitivity[4] = 1000;
			sensitivity[5] = 1000;
 			//should handle version exception.
 	}
	for (i=0; i<6; i++) Press_Data[i] = 0;
}


void Press_Reset() {
}


void Press_ReadAll() {
	ADC_SETBANK(0);
	Press_Data[0] = COMPENSATE(ADC_ReadChanSync(12), 0);
 	ADC_SETBANK(1);
 	Press_Data[1] = COMPENSATE(ADC_ReadChanSync(12), 1);
 	ADC_SETBANK(2);
 	Press_Data[2] = COMPENSATE(ADC_ReadChanSync(12), 2);
 	ADC_SETBANK(3);
 	Press_Data[3] = COMPENSATE(ADC_ReadChanSync(12), 3);
 	ADC_SETBANK(0);
 	Press_Data[4] = COMPENSATE(ADC_ReadChanSync(13), 4);
 	ADC_SETBANK(1);
 	Press_Data[5] = COMPENSATE(ADC_ReadChanSync(13), 5);
}


#undef COMPENSATE
#undef SENSITIVITY
#undef OFFSET
