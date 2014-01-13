/*


  
  dialog - A sub-class of view.  It represents question posed to the user.  
  The response to that question will trigger some sort of control event.
  
  menu - A sub-class of view.  It presents a list of options to be selected from.
*/


#include <avr/pgmspace.h>
#include <stdio.h>
#include "view.h"

// Default handlers.  These aren't public, so we declare them here instead of the header.
void ViewDefaultEjectFunc(view_ptr view);
void ViewDefaultKeyFunc(view_ptr view, char key);

void ViewConstruct(view_ptr view, update_f update) {
	view->mux = 0;
	view->entry = 0;
	view->exit = 0;
	view->update = update;
	view->key = ViewDefaultKeyFunc;
	view->actions = 0;
	view->eject = ViewDefaultEjectFunc;
	view->priv = 0;
	view->shouldUpdate = 0;
}

int ViewGetUpdate(view_ptr view) {
	if (view) return view->shouldUpdate;
	else return 0;
}
void ViewSetUpdate(view_ptr view) {
	if (view) view->shouldUpdate++;
}
void ViewClearUpdate(view_ptr view) {
	if (view) view->shouldUpdate = 0;
}

keyaction_ptr ViewResolveAction(view_ptr view, char key) {
	// Find the first action that matches and return it.
	int x = 0;
	while (view->actions[x] != 0) {
		if (view->actions[x]->key == key) {
			return view->actions[x];
		}
		x++;
	}
	return 0;
}

keyhand_f ViewResolveKeyFunc(view_ptr view, char key) {
	keyaction_ptr a = ViewResolveAction(view, key);
	if (a) return a->func;
	else return 0;
}

void ViewDefaultKeyFunc(view_ptr me, char key) {
	keyhand_f keyfunc;
	keyfunc = ViewResolveKeyFunc(me, key);
	if(keyfunc) {
		keyfunc(me, key);
		ViewSetUpdate(me);
	}
}

void ViewDefaultEjectFunc(view_ptr me) {
	if (!me->mux) return;
	if (!me->mux->eject) return;
	me->mux->eject(me->mux, me);
}