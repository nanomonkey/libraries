#include "tree.h"

void NodeConstruct(node_p node) {
	node->parent = 0;
	node->children = 0;
	node->prev = 0;
	node->next = 0;
}

// Insert a node to the list of siblings
void NodeInsert (node_p dst, node_p src) {
	src->prev = dst;
	src->next = dst->next;
	dst->next = src;
	src->next->prev = src;
}

// Append a node to the end of a branch
void BranchAppend (node_p branch, node_p node) {
	if (branch->parent)
		node->parent = branch->parent;
	if (node->prev = Walk(branch, IsTerminus, 0))
		node->prev->next = node;
}

// Remove a node from a branch
void BranchRemove (node_p parent, node_p child) {}

// Append a node to the children of another node
void NodeAppend(node_p parent, node_p child){
	if (parent->children) 
		BranchAppend(parent->children, child);
	else
		parent->children = child;
		child->parent = parent;
}
void NodeRemove(node_p parent, node_p child){}

// Compare identity
int NodeCompare(node_p n1, node_p n2) {
	return (n1 == n2);
}

int IsLeaf(node_p node) { // True if this is the only node in the branch
	return !(
		(int) node->next || 
		(int) node->prev || 
		(int) node->children
	);
}
int IsBranch(node_p node) { // True if there's other nodes attached to this one
	return (
		node && (
			(int) node->next || 
			(int) node->prev || 
			(int) node->children
		)
	);
} 
int IsTerminus(node_p node) { // True if there's no next node on the branch
	return !(
		(int) node &&
		(int) node->next
	);
} 
int IsParent(node_p node) {
	return (int) node->children;
}
int IsAhead(node_p n1, node_p n2){
	node_p x = n1;
	int i = 0;
	while (!NodeCompare(n2, x) && x) {
		x = NextNode(x);
		i++;
	}
	if (!x) i = 0;
	return i;
}
int IsBehind(node_p n1, node_p n2){
	node_p x = n1;
	int i = 0;
	while (!NodeCompare(n2, x) && x) {
		x = PrevNode(x);
		i++;
	}
	if (!x) i = 0;
	return i;
}

int NextNode(node_p node){
	if (node) return node->next;
	else return 0;
}
int PrevNode(node_p node){
	if (node) return node->prev;
	else return 0;
}
node_p GetParent(node_p node) {
	return node->parent;
}
node_p GetChild(node_p parent) {
	return parent->children;
}
node_p NodeSeek(node_p start, int dist){
	node_p x = start;
	int i = 0;
	while (x && i != dist) {
		if (dist > 0) {
			if (x->next) x = x->next;
			i++;
		}
		else {
			if (x->prev) x = x->prev;
			i--;
		}
	}
	return x;
}
// Walk the tree looking for a matching node
node_p Walk (node_p node, node_f testfunc, unsigned int depth) {
	node_p x;
	if (!node) return 0; // NULL node gets none!
	if (testfunc(node)) return node; // This node is the one we want.
	if (depth > 0) // Make sure we don't go too deep
		if(x = Walk(node->children, testfunc, depth - 1)) // Walk down
			return x;
	if (x = Walk(node->next, testfunc, depth))  // Walk across
		return x;
}