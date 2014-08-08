#include <stdio.h>
#include "view.h"
#include "dialog.h"

void Dialog_entry (dialog_ptr me) {
}
void Dialog_exit (dialog_ptr me) {
}
void Dialog_update (dialog_ptr me, viewport_ptr port) {
	
}
	/*
	void Dialog_Adj_update (const view_ptr v) {
	dialog_adj_t * i = (dialog_adj_t *) v->env;
	if (v->shouldUpdate) {
		Disp_Clear();
		Disp_RC(0,0); fprintf_P(v->dev, i->label);
		Disp_RC(1,0);
		switch (CfgGetType(i->value)) {
			case CFGTYPE_BYTE:
				fprintf_P(v->dev, PSTR("%u"), (byte) *CfgGetVal(i->value));
				break;
			case CFGTYPE_INT:
				fprintf_P(v->dev, PSTR("%i"), (int) *CfgGetVal(i->value));
				break;
			case CFGTYPE_UINT:
				fprintf_P(v->dev, PSTR("%u"), (unsigned int) *CfgGetVal(i->value));
				break;
			case CFGTYPE_LONG:
				fprintf_P(v->dev, PSTR("%li"), (long) *CfgGetVal(i->value));
				break;
			case CFGTYPE_ULONG:
				fprintf_P(v->dev, PSTR("%lu"), (unsigned long) *CfgGetVal(i->value));
				break;
			default:
				break;
		}
		Disp_RC(2,0); 
		fprintf_P(
			v->dev, 
			PSTR("%S %lu %lu %lu"), 
			CfgGetTypeStr(CfgGetType(i->value)), 
			CfgGetMin(i->value),
			CfgGetMax(i->value),
			CfgGetDef(i->value)
		);
		Disp_RC(3,0); fprintf_P(v->dev, PSTR("EXIT"));
		//Disp_RC(3,5); fprintf_P(v->dev, PSTR("ENTER"));
		Disp_RC(3,12); fprintf_P(v->dev, PSTR("+"));
		Disp_RC(3,17); fprintf_P(v->dev, PSTR("-"));
		v->shouldUpdate = 0;
	}
}
*/
