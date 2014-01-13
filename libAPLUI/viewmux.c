#include <stdio.h>
#include "view.h"

view_ptr VmuxDefaultAcceptFunc(viewmux_ptr me);
void VmuxDefaultEjectFunc(viewmux_ptr me, view_ptr view);

void VmuxConstruct(viewmux_ptr mux, viewport_ptr port) {
	mux->port = port;
	mux->current = 0;
	mux->accept = VmuxDefaultAcceptFunc;
	mux->eject = VmuxDefaultEjectFunc;
}

// Eject the current view and install a new one.  It's okay for view to be NULL
void VmuxSwitch(viewmux_ptr mux, view_ptr view) {
	//mux->eject(mux, mux->current);		// Get rid of the current view (should we really do this here?)
	if (view) {
		view->mux = mux;					// In case it wasn't set before
		if(view->entry) view->entry(view);  // Call the entry hook if it exists
		ViewSetUpdate(view);				// Set flag to update next cycle
	}
	mux->current = view;
}

void VmuxTick(viewmux_ptr mux) {
	char key;
	if (mux->current) {
		if((key = fgetc(&mux->port->dev)) != EOF)
			mux->current->key (mux->current, key);
	}
	if (mux->current) { // Check again, the view might have disappeared!
		if (ViewGetUpdate(mux->current)) {
			mux->current->update(mux->current, mux->port);
			ViewClearUpdate(mux->current);
		}
	}
	else {	// What do you do if there's no view, currently?  Ask for one!
		VmuxSwitch(mux, mux->accept(mux));
	}
}

view_ptr VmuxDefaultAcceptFunc(viewmux_ptr me) {
	return 0;
}
void VmuxDefaultEjectFunc(viewmux_ptr me, view_ptr view) {
	if (view) {
		if (view->exit) view->exit(view);	// Call the exit hook to let the view know it was ejected
		view->mux = 0;		// This may not be necessary
	}
	me->current = 0;	// Now we don't have a view
}