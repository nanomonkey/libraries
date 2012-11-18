#include <stdio.h>

#include "display.h"
#include "keypad.h"
#include "temp.h"
#include "pressure.h"
#include "fet.h"
#include "servos.h"
#include "timer.h"
#include "util.h"

static int scr = 0;
int temp_done = 0;
int temp_start = 0;

#define SCR_HELLO 0
#define SCR_TEMP0 1
#define SCR_TEMP1 2
#define SCR_TEMP2 3
#define SCR_PRESS 4
#define SCR_FETS 5
#define SCR_SERVOS 6
#define NSCR 6


void UI_NextScr() {
	switch (scr) {
	case SCR_HELLO:
		scr = SCR_TEMP0;
		break;
	case SCR_TEMP0:
		if (GCU_fill  == LITEFILL | GCU_fill  == HALFFILL) {
		  scr = SCR_PRESS;
		} else {
		  scr = SCR_TEMP1;
		}
		break;
	case SCR_TEMP1:
		scr = SCR_TEMP2;
		break;
	case SCR_TEMP2:
		scr = SCR_PRESS;
		break;
	case SCR_PRESS:
		scr = SCR_FETS;
		break;
	case SCR_FETS:
		scr = SCR_HELLO; //skip servo screens (not implemented)
		break;
    case SCR_SERVOS:
    	break;
    }
}


void do_hello() {
	Disp_RC(0,0);
	if (GCU_version == V2) {
	Disp_PutStr("   KS GCU V 2.02    ");
	} else if (GCU_version == V3) {
	Disp_PutStr("    KS GCU V 3.0    ");
	}
	Disp_RC(1,0);
	Disp_PutStr("www.allpowerlabs.org");
	Disp_RC(2,0);
	Disp_PutStr("    (C) APL 2011    ");
	Disp_RC(3,0);
	Disp_PutStr("NEXT            HELP");
	Disp_CursOff();
}

//static void do_vitals() {
//	char buf[20];
//	int n;
//	n = start;
// 	Disp_RC(0, 0);
// 	sprintf(buf, "Ttred %3ld  ", Temp_Data[0]);
// 	Disp_PutStr(buf);
// 	sprintf(buf, "Tbred %3ld", Temp_Data[1]);
// 	Disp_PutStr(buf);
// 	Disp_RC(1, 0);
// 	sprintf(buf, "Preac %3ld  ", Press_Data[0] / 25);
// 	Disp_PutStr(buf);
// 	sprintf(buf, "Tbred %3ld", Temp_Data[1]);
// 	Disp_PutStr(buf);
// }

static void do_temp(int start) {
	char buf[20];
	int n;
	n = start;
	
	Disp_RC(0, 0);
	sprintf(buf, "T%02d   %3ld  ", n, Temp_Data[n]);
	Disp_PutStr(buf);
	n++;
	sprintf(buf, "T%02d   %3ld", n, Temp_Data[n]);
	Disp_PutStr(buf);
	
	Disp_RC(1, 0);
	n++;
	sprintf(buf, "T%02d   %3ld  ", n, Temp_Data[n]);
	Disp_PutStr(buf);
	n++;
	sprintf(buf, "T%02d   %3ld", n, Temp_Data[n]);
	Disp_PutStr(buf);
	
	
	Disp_RC(2, 0);
	if (start==12 | GCU_fill == LITEFILL) { //third screen
		Disp_PutStr("                    ");
	} else {
		n++;
		sprintf(buf, "T%02d   %3ld  ", n, Temp_Data[n]);
		Disp_PutStr(buf);
		n++;
		sprintf(buf, "T%02d   %3ld", n, Temp_Data[n]);
		Disp_PutStr(buf);
	}
		
	Disp_RC(3, 0);
	Disp_PutStr("NEXT   [degC]   HELP");
	Disp_CursOff();
}

static void do_press() {
	char buf[40];
	int n = -1;
	int i = 0;
	for (i=0; i<6; i++) {
		if (Press_Available[i] == 1) {
			n++;
			if (n % 2 == 0) {
			Disp_RC(n/2, 0);
			}
			sprintf(buf, "P%1d ", i);
			Disp_PutStr(buf);
			sprintf(buf, "% 4ld", Press_Data[i] / 25);
			Disp_PutStr(buf);
			if (n % 2 == 0) {
				Disp_PutStr("      ");
			}
		}
	}
	if (n<2) {
		Disp_RC(1, 0);
		Disp_PutStr("                    ");
	}
	if (n<5) {
		Disp_RC(2, 0);
		Disp_PutStr("                    ");
	}
	Disp_RC(3, 0);
	Disp_PutStr("NEXT [inH2O*10] HELP");
	Disp_CursOff();
}


static int fet_index = 0;

static void NextFet() {
	fet_index++;
	fet_index %= NFETS;
}

static void FetPlus() {
	Fet_Data[fet_index] += 10;
	if (Fet_Data[fet_index] > 100) Fet_Data[fet_index] = 100;
}

static void FetMinus() {
	Fet_Data[fet_index] -= 10;
	if (Fet_Data[fet_index] < 0) Fet_Data[fet_index] = 0;
}

static void Fet_PutCurs() {
	Disp_RC(fet_index / 3, (fet_index % 3) * 7);
	Disp_CursOn();
}

static void do_fets() {
	char buf[20];

	Disp_RC(0, 0);
	sprintf(buf, "F%1d %3d ", 0, Fet_Data[0]);
	Disp_PutStr(buf);
	sprintf(buf, "F%1d %3d ", 1, Fet_Data[1]);
	Disp_PutStr(buf);
	sprintf(buf, "F%1d %3d", 2, Fet_Data[2]);
	Disp_PutStr(buf);
	
	Disp_RC(1, 0);
	sprintf(buf, "F%1d %3d ", 3, Fet_Data[3]);
	Disp_PutStr(buf);
	if (GCU_fill != 1) {
		sprintf(buf, "F%1d %3d ", 4, Fet_Data[4]);
		Disp_PutStr(buf);
		sprintf(buf, "F%1d %3d ", 5, Fet_Data[5]);
		Disp_PutStr(buf);
		
		Disp_RC(2, 0);
		sprintf(buf, "F%1d %3d ", 6, Fet_Data[6]);
		Disp_PutStr(buf);
		sprintf(buf, "F%1d %3d       ", 7, Fet_Data[7]);
		Disp_PutStr(buf);
	} else {
		Disp_PutStr("             ");
		Disp_RC(2, 0);
		Disp_PutStr("                    ");
	}
	
	Disp_RC(3, 0);
	Disp_PutStr("NEXT  ADV   +    -  ");
	Fet_PutCurs();
}


static int srv_index = 0;

static void NextSRV() {
	srv_index++;
	srv_index %= NSERVO;
}

static void SRVPlus() {
	Servo_Data[srv_index] += 10;
	if (Servo_Data[srv_index] > 100) Servo_Data[srv_index] = 100;
}

static void SRVMinus() {
	Servo_Data[srv_index] -= 10;
	if (Servo_Data[srv_index] < 0) Servo_Data[srv_index] = 0;
}

static void SRV_PutCurs() {
	switch (srv_index) {
	case 0:
		Disp_RC(0, 0);
		break;
	case 1:
		Disp_RC(0, 13);
		break;
	case 2:
		Disp_RC(1, 0);
		break;
	}
	Disp_CursOn();
}

static void do_servos() {
	char buf[30];

	Disp_RC(0, 0);
	sprintf(buf, "SV0 %3d      SV1 %3d", Servo_Data[0], Servo_Data[1]);
	Disp_PutStr(buf);
	Disp_RC(1, 0);
	sprintf(buf, "SV2 %3d             ", Servo_Data[2]);
	Disp_PutStr(buf);
	Disp_RC(2, 0);
	sprintf(buf, "RPM %5d           ", Timer_Data);
	Disp_PutStr(buf);
	Disp_RC(3, 0);
	Disp_PutStr("NEXT  ADV   +    -  ");
	SRV_PutCurs();
}


static void do_help() {
	Disp_RC(0, 0);
	Disp_PutStr("    Help!  HELP!!   ");
	Disp_RC(1, 0);
	Disp_PutStr("                    ");
	Disp_RC(2, 0);
	Disp_PutStr("    gekgasifier.    ");
	Disp_RC(3, 0);
	Disp_PutStr("  pbworks.com/GCU   ");
	DelayMS(10);
	(void) Kpd_WaitKeyUp();
}


void UI_DoScr() {
	switch (scr) {
	case SCR_HELLO:
		do_hello();
		break;
	case SCR_TEMP0:
		do_temp(0);
		break;
	case SCR_TEMP1:
		do_temp(6);
		break;	
	case SCR_TEMP2:
		do_temp(12);
		break;
	case SCR_PRESS:
		do_press();
		break;
	case SCR_FETS:
		do_fets();
		break;
	case SCR_SERVOS:
		do_servos();
		break;
	}
}


void UI_HandleKey(int k) {
	if (k < 0) return;
	if (k == 0) {
		UI_NextScr();
		return;
	}
	if (scr == SCR_FETS) {
		switch (k) {
		case 1:
			NextFet();
			break;
		case 2:
			FetPlus();
			break;
		case 3:
			FetMinus();
			break;
		}
		return;
	}
	if (scr == SCR_SERVOS) {
		switch (k) {
		case 1:
			NextSRV();
			break;
		case 2:
			SRVPlus();
			break;
		case 3:
			SRVMinus();
			break;
		}
		return;
	}
	if (k == 3) {
		do_help();
		return;
	}
}


void UI_Init() {
}


void UI_Reset() {
	scr = 0;
}
