#include <avr/pgmspace.h>
#include <stdio.h>
#include "view.h"
#include "tree.h"
#include "menu.h"

void key0(menu_ptr m, char key);
void key1(menu_ptr m, char key);
void key2(menu_ptr m, char key);
void key3(menu_ptr m, char key);

keyaction_t key0_act = {0, key0};
keyaction_t key1_act = {1, key1};
keyaction_t key2_act = {2, key2};
keyaction_t key3_act = {3, key3};

keyaction_t *  menu_actions[] = {
	&key0_act,
	&key1_act,
	&key2_act,
	&key3_act,
};

void MenuConstruct (menu_ptr menu, menunode_p node, selector_f sfunc) {
	ViewConstruct(&menu->view, (update_f) Menu_update);
	menu->view.actions = &menu_actions;  // I don't think this is the best way to do this...
	menu->select = sfunc;
	menu->node = node;
	menu->top = MenuGetFirst(menu);
	menu->cursor = MenuGetFirst(menu);
}
void MenunodeConstruct (menunode_p item, const char * label, viewfactory_f factory) {
	item->label = label;
	item->factory = factory;
}
menunode_p MenuGetFirst (menu_ptr menu) {
	return GetChild((node_p) menu->node);
}
menunode_p MenuGetLast (menu_ptr menu) {
	menunode_p x = MenuGetFirst(menu);
	while (NextNode((node_p) x)) x = NextNode((node_p) x);
	return x;
}
menunode_p MenuGetParent (menu_ptr menu) {
	return GetParent ((node_p) menu->node);
}

void MenuAppend (menu_ptr menu, menunode_p node) {
	BranchAppend((node_p) MenuGetFirst(menu), (node_p)node);
}
void Menu_update (menu_ptr menu, viewport_ptr port) {
	int x;
	menunode_p i;
	fputc('\f', &port->dev);
	if (menu->top) {
		// Adjust top so that the cursor is within view
		if(IsAhead(menu->top, menu->cursor) > (port->h - 1)) 
			menu->top = NodeSeek(menu->cursor, -(port->h - 1));
		if(IsBehind(menu->top, menu->cursor)) 
			menu->top = menu->cursor;
		i = menu->top;  // Start at the top
		for (x=0; x < (port->h); x++) {
			if (i) {
				fprintf_P(&port->dev, PSTR("% 2i "), IsBehind(i, MenuGetFirst(menu)) + 1);
				fprintf_P(&port->dev, i->label);
			} 
			if (i == menu->cursor) 
				fprintf_P(&port->dev, PSTR(" <-"));
			i = (menunode_p) NextNode(i);  // Advance to next item
			fputc('\n', &port->dev);
		}
	}
	else {
		fprintf_P(&port->dev, PSTR("Nothing Here...\n\n\n"));
	}
	// if (MenuGetParent(menu))
		// fprintf_P(&port->dev, PSTR("EXIT "));
	// else
		// fprintf_P(&port->dev, PSTR("     "));
	// if (menu->cursor->factory || GetChild((node_p) menu->cursor))
		// fprintf_P(&port->dev, PSTR("ENTER"));
	// else
		// fprintf_P(&port->dev, PSTR("     "));
	// if (menu->cursor != MenuGetFirst(menu))
		// fprintf_P(&port->dev, PSTR("  UP "));
	// else
		// fprintf_P(&port->dev, PSTR("     "));
	// if (menu->cursor != MenuGetLast(menu))
		// fprintf_P(&port->dev, PSTR(" DOWN"));
	// else
		// fprintf_P(&port->dev, PSTR("     "));
}

void key0(menu_ptr menu, char key) {
	menu->view.eject(menu);
}
void key1(menu_ptr menu, char key) {
	if (menu->select) menu->select(menu, menu->cursor);
}
void key2(menu_ptr menu, char key) {
	if (menu->cursor && PrevNode(menu->cursor)) 
		menu->cursor = (menunode_p) PrevNode(menu->cursor);
}
void key3(menu_ptr menu, char key) {
	if (menu->cursor && NextNode(menu->cursor)) 
		menu->cursor = (menunode_p) NextNode(menu->cursor);
}