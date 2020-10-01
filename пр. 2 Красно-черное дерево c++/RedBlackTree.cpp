#include <iostream>
using namespace std;
class RBtree {
	struct node_st
	{
		node_st* p1, * p2;
		int value;
		bool red;
	};
	node_st* tree_root;					
	int nodes_count;					
private:
	node_st* newNode(int value);		
	void delNode(node_st*);				
	void clear(node_st*);				
	node_st* rotateLeft(node_st*);		
	node_st* rotateRight(node_st*);		
	void balanceinsert(node_st**);		
	bool balanceremove1(node_st**);		
	bool balanceremove2(node_st**);		
	bool insert(int, node_st**);	
	bool getMin(node_st**, node_st**);
	bool remove(node_st**, int);
	void show(node_st*, int, char);
	void showSortedArray(node_st*);
public:
	RBtree();
	~RBtree();
	void clear();			
	bool find(int);	
	void insert(int);
	void remove(int);
	int getNodesCount();
	void showSortedArray();
	void show();
};

//===========================
//===Constructor&Destructor==
//===========================

RBtree::RBtree()
{
	tree_root = 0;
	nodes_count = 0;
}

RBtree::~RBtree()
{
	clear(tree_root);
}

//===========================
//==========PRIVATE==========
//===========================

RBtree::node_st* RBtree::newNode(int value)
{
	nodes_count++;
	node_st* node = new node_st;
	node->value = value;
	node->p1 = node->p2 = 0;
	node->red = true;
	return node;
}


void RBtree::delNode(node_st* node)
{
	nodes_count--;
	delete node;
}

void RBtree::clear(node_st* node)
{
	if (!node) return;
	clear(node->p1);
	clear(node->p2);
	delNode(node);
}

void RBtree::show(node_st* node, int depth, char dir)
{
	int n;
	if (!node) return;
	for (n = 0; n < depth; n++) cout << ".";
	printf("%c[%d] (%s)\n", dir, node->value, node->red ? "red" : "black");
	show(node->p1, depth + 2, 'l');
	show(node->p2, depth + 2, 'r');
}

RBtree::node_st* RBtree::rotateLeft(node_st* node)
{
	node_st* p2 = node->p2;
	node_st* p21 = p2->p1;
	p2->p1 = node;
	node->p2 = p21;
	return p2;
}
RBtree::node_st* RBtree::rotateRight(node_st* node)
{
	node_st* p1 = node->p1;
	node_st* p12 = p1->p2;
	p1->p2 = node;
	node->p1 = p12;
	return p1;
}

void RBtree::balanceinsert(node_st** root)
{
	node_st* p1, * p2, * px1, * px2;
	node_st* node = *root;
	if (node->red) return;
	p1 = node->p1;
	p2 = node->p2;
	if (p1 && p1->red) {
		px2 = p1->p2;			
		if (px2 && px2->red) p1 = node->p1 = rotateLeft(p1);
		px1 = p1->p1;
		if (px1 && px1->red) {
			node->red = true;
			p1->red = false;
			if (p2 && p2->red) {	
				px1->red = true;
				p2->red = false;
				return;
			}
			*root = rotateRight(node);
			return;
		}
	}
	if (p2 && p2->red) {
		px1 = p2->p1;				
		if (px1 && px1->red) p2 = node->p2 = rotateRight(p2);
		px2 = p2->p2;
		if (px2 && px2->red) {
			node->red = true;
			p2->red = false;
			if (p1 && p1->red) {	
				px2->red = true;
				p1->red = false;
				return;
			}
			*root = rotateLeft(node);
			return;
		}
	}
}


bool RBtree::balanceremove1(node_st** root)
{
	node_st* node = *root;
	node_st* p1 = node->p1;
	node_st* p2 = node->p2;
	if (p1 && p1->red) {
		p1->red = false; return false;
	}
	if (p2 && p2->red) { // случай 1
		node->red = true;
		p2->red = false;
		node = *root = rotateLeft(node);
		if (balanceremove1(&node->p1)) node->p1->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st* p21 = p2->p1;
	node_st* p22 = p2->p2;
	if (p21 && p21->red) mask |= 1;
	if (p22 && p22->red) mask |= 2;
	switch (mask)
	{
	case 0:		// случай 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
		p2->red = true;
		return true;
	case 1:
	case 3:		// случай 3 - if(p21 && p21->red)
		p2->red = true;
		p21->red = false;
		p2 = node->p2 = rotateRight(p2);
		p22 = p2->p2;
	case 2:		// случай 4 - if(p22 && p22->red)
		p2->red = node->red;
		p22->red = node->red = false;
		*root = rotateLeft(node);
	}
	return false;
}

bool RBtree::balanceremove2(node_st** root)
{
	node_st* node = *root;
	node_st* p1 = node->p1;
	node_st* p2 = node->p2;
	if (p2 && p2->red) { p2->red = false; return false; }
	if (p1 && p1->red) { // случай 1
		node->red = true;
		p1->red = false;
		node = *root = rotateRight(node);
		if (balanceremove2(&node->p2)) node->p2->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st* p11 = p1->p1;
	node_st* p12 = p1->p2;
	if (p11 && p11->red) mask |= 1;
	if (p12 && p12->red) mask |= 2;
	switch (mask) {
	case 0:		// случай 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
		p1->red = true;
		return true;
	case 2:
	case 3:		// случай 3 - if(p12 && p12->red)
		p1->red = true;
		p12->red = false;
		p1 = node->p1 = rotateLeft(p1);
		p11 = p1->p1;
	case 1:		// случай 4 - if(p11 && p11->red)
		p1->red = node->red;
		p11->red = node->red = false;
		*root = rotateRight(node);
	}
	return false;
}

bool RBtree::insert(int value, node_st** root)
{
	node_st* node = *root;
	if (!node) *root = newNode(value);
	else {
		if (value == node->value) return true;
		if (insert(value, value < node->value ? &node->p1 : &node->p2)) return true;
		balanceinsert(root);
	}
	return false;
}

bool RBtree::getMin(node_st** root, node_st** res)
{
	node_st* node = *root;
	if (node->p1) {
		if (getMin(&node->p1, res)) return balanceremove1(root);
	}
	else {
		*root = node->p2;
		*res = node;
		return !node->red;
	}
	return false;
}

bool RBtree::remove(node_st** root, int value)
{
	node_st* t, * node = *root;
	if (!node) return false;
	if (node->value < value) {
		if (remove(&node->p2, value))	return balanceremove2(root);
	}
	else if (node->value > value) {
		if (remove(&node->p1, value))	return balanceremove1(root);
	}
	else {
		bool res;
		if (!node->p2) {
			*root = node->p1;
			res = !node->red;
		}
		else {
			res = getMin(&node->p2, root);
			t = *root;
			t->red = node->red;
			t->p1 = node->p1;
			t->p2 = node->p2;
			if (res) res = balanceremove2(root);
		}
		delNode(node);
		return res;
	}
	return 0;
}

void RBtree::showSortedArray(node_st* root)
{
	if (root->p1 != NULL)
		showSortedArray(root->p1);
	cout << root->value << " ";
	if (root->p2 != NULL)
		showSortedArray(root->p2);
}

//===========================
//===========PUBLIC==========
//===========================

void RBtree::show()
{
	printf("TREE:\n");
	show(tree_root, 0, 'T');
}

void RBtree::insert(int value)
{
	insert(value, &tree_root);
	if (tree_root) tree_root->red = false;
}


void RBtree::remove(int value)
{
	remove(&tree_root, value);
}

void RBtree::clear()
{
	clear(tree_root);
	tree_root = 0;
}

void RBtree::showSortedArray()
{
	showSortedArray(tree_root);
}

int RBtree::getNodesCount()
{
	return nodes_count;
}

bool RBtree::find(int value)
{
	node_st* node = tree_root;
	while (node) {
		if (node->value == value) return true;
		node = node->value > value ? node->p1 : node->p2;
	}
	return false;
}

int main()
{
	RBtree tree;
	int n;
	cout << "n: ";
	cin >> n;
	int *arr = new int[n];
	for (int i = 0; i < n; i++)
	{
		cout << "arr[" << i << "]: ";
		int temp;
		cin >> temp;
		tree.insert(temp);
	}
	cout << "Tree: \n";
	tree.show();
	cout << "Array: \n";
	tree.showSortedArray();
	return 0;
}



