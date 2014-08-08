/*
	Node - any single point in the tree
	Branch - a node, or lateral set of nodes, with one or more descendants
	Twig - a lateral set of nodes with no descendants
	Root - a node with no parent, and the first node in a branch
	Leaf - a node with no descendants and no lateral siblings
	Terminus - the last node in a branch
*/

typedef struct node * node_p;

typedef int (* node_f)(node_p); // Tests a node for some condition.  Returns non-zero for success.

typedef struct node {
	node_p parent;			// Parent node.  NULL if this node is the root.
	node_p children;		// Child node.  NULL if this node is a leaf.
	node_p prev;			// Previous lateral node.  NULL if this node is first.
	node_p next;			// Next lateral node.  NULL if this node is last.
} node_s;

#ifdef __cplusplus
extern "C" {
#endif

void NodeConstruct(node_p node);
/*
Walk the tree looking for the first node that matches.  Traversal is depth 
first, then lateral.  If depth is reached without finding a match, no further
descent is made.
*/
node_p Walk (node_p root, node_f, unsigned int depth);

int IsBranch(node_p);
int IsLeaf(node_p);
int IsTerminus(node_p);
int IsParent(node_p);

int IsAhead(node_p n1, node_p n2);
int IsBehind(node_p n1, node_p n2);

int NextNode(node_p node);
int PrevNode(node_p node);

node_p GetParent(node_p node);
node_p GetChild(node_p parent);
node_p NodeSeek(node_p start, int dist);

void NodeInsert (node_p n1, node_p n2);	// Insert a node into a branch
void BranchAppend (node_p branch, node_p node);		// Append a node to the end of a branch
void BranchRemove (node_p branch, node_p node);		// Remove a node from the branch
void NodeAppend(node_p parent, node_p child);
void NodeRemove(node_p parent, node_p child);
int NodeCompare (node_p n1, node_p n2);  			// Compare two nodes for equal identity

#ifdef __cplusplus
}
#endif