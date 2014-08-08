/*
	viewport - An abstraction of an input and output device.  It polls for user input events, and displays program outputs.  
	Input events are passed on to the attached view.

	view - A view is the presentation layer, interacting through a viewport.  It is the generic case of higher-level
	UI constructs.  It reacts to inputs and generates outputs.  When a view internally decides to end its control
	of the display, it calls is eject() function.  By default this passes the eject event on to the multiplexer.
	
	enter function - Called on entry into the view
	exit function - Called when exiting the view
	update function - Called periodically to display any updates
	
	viewmux - Multiplexes views onto the display.
	
*/

typedef struct view * view_ptr;
typedef struct viewmux * viewmux_ptr;
typedef struct viewport * viewport_ptr;
typedef struct keyaction * keyaction_ptr;

typedef view_ptr (* viewfactory_f)();
	
typedef void (* view_f)(view_ptr);
typedef void (* update_f)(view_ptr, viewport_ptr);
typedef void (* keyhand_f)(view_ptr, char);
typedef void (* vmuxeject_f)(viewmux_ptr, view_ptr);
typedef view_ptr (* vmuxaccept_f)(viewmux_ptr);

typedef struct viewport {
	FILE dev;
	unsigned int w;
	unsigned int h;
	unsigned int row;
	unsigned int col;
} viewport_t;

typedef struct keyaction {
	char key;
	keyhand_f func;
} keyaction_t;

typedef struct view {
	viewmux_ptr mux;			// The multiplexer handling this view
	view_f entry;				// Entry hook, for interacting with control
	view_f exit;				// Exit hook, for interacting with control
	update_f update;			// Update hook, for drawing to the viewport
	keyhand_f key;				// Handler function that all keys pass through
	keyaction_ptr * actions;	// List of key action hooks
	view_f eject;				// Call this when we're done with the current view
	void * priv;				// Anonymous private data for callback functions
	unsigned int shouldUpdate;	// Flag to notify us it's time to update
} view_t;

typedef struct viewmux {
	viewport_ptr port;
	view_ptr current;
	vmuxaccept_f accept;
	vmuxeject_f eject;
} viewmux_t;

#ifdef __cplusplus
extern "C" {
#endif
void ViewConstruct(view_ptr view, update_f update);

keyhand_f ViewResolveKeyFunc(view_ptr view, char key);
keyaction_ptr ViewResolveAction(view_ptr view, char key);

int ViewGetUpdate(view_ptr view);
void ViewSetUpdate(view_ptr view);
void ViewClearUpdate(view_ptr view);

void VmuxConstruct(viewmux_ptr mux, viewport_ptr port);
void VmuxTick(viewmux_ptr mux);
void VmuxSwitch(viewmux_ptr mux, view_ptr view);

void VmuxDefaultEjectFunc(viewmux_ptr me, view_ptr view);

#ifdef __cplusplus
}
#endif