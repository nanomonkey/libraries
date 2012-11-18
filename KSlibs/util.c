#include "util.h"

//default = GCU V3
int GCU_version = 3; 
//default = full fill
int GCU_fill = 2;
// P sequence -
// some GCU v 3.0 boards have pressure sensors in opposite sequence
// psequence 1 = P0,P1,P3,P4,P5,P6,P7 = MXP7007,MXP7007,MXP7007,MXP7002,MXP7002,MXP7002
// psequence 2 = P0,P1,P3,P4,P5,P6,P7 = MXP7002,MXP7002,MXP7002,MXP7007,MXP7007,MXP7007
int GCU_psequence = 2;

//number of pressure sensors
int n_pressure = 6; 

void GCU_Setup(int version, int fill, int psequence) {
	GCU_version = version;
	GCU_fill = fill;
	GCU_psequence = psequence;

	switch (GCU_version) {
		case V2:
			n_pressure = 4;
			break;
 		case V3:
 			n_pressure = 6;
 			break;
 		default:
 			n_pressure = 6;
 			//should handle version exception.
 	}
}

void DelayMS(unsigned int count) {
	int i;
    while(count--) {
		for (i=0; i<840; i++);
	}
}
