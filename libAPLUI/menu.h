typedef struct menunode * menunode_p;
typedef struct menu * menu_ptr;

typedef void (* selector_f)(menu_ptr menu, menunode_p node);
typedef void (* menuexit_f)(menu_ptr menu);

typedef struct menunode {
	node_s node;
	viewfactory_f factory;
	const char * label;
} menunode_s;

typedef struct menu {
	view_t view;
	selector_f select;
	menunode_p node;
	menunode_p root;
	menunode_p top;
	menunode_p cursor;
} menu_t;

//extern keyaction_t * menu_actions[];

#ifdef __cplusplus
extern "C" {
#endif

void MenuConstruct (menu_ptr menu, menunode_p root, selector_f sfunc);
void MenunodeConstruct (menunode_p node, const char * label, viewfactory_f factory);

menunode_p MenuGetFirst (menu_ptr menu);
menunode_p MenuGetLast (menu_ptr menu);
menunode_p MenuGetParent (menu_ptr menu);
void MenuAppend (menu_ptr menu, menunode_p node);

void Menu_update (menu_ptr, viewport_ptr);

#ifdef __cplusplus
}
#endif