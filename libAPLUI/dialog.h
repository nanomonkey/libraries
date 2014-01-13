typedef struct dialog * dialog_ptr;

typedef struct keylabel {
	
} keylabel_t;

typedef struct dialog {
	struct view;
	const char * message;
} dialog_t;


#ifdef __cplusplus
extern "C" {
#endif

void Dialog_entry (const dialog_ptr);
void Dialog_exit (const dialog_ptr);
void Dialog_update (const dialog_ptr, viewport_ptr port);

#ifdef __cplusplus
}
#endif