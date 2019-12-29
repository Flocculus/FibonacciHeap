

#define _FIBONACCI_TREE_HPP_

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
using namespace std;


class FibNode {
public:
	int value;			// the basic elements of a Fibonacci node
	string key; // add one more element to record the keywords (I mean "youtube", "facebook", etc)
	int degree;
	FibNode *left;
	FibNode *right;
	FibNode *parent;
	FibNode *child;
	bool mark;

	FibNode(int input, string input2) :value(input), key(input2), degree(0), mark(false), left(this), right(this), child(NULL), parent(NULL) {}
	// For only one node, its left and right siblings are itself. It doesn't have child or parent. Its degree is 0.


};

class FibHeap {
private:
	int num;
	int maxdegree;
	FibNode *max;
	FibNode **cons;// use for consolidate

public:
	FibHeap();
	~FibHeap();

	FibNode* insert(int value, string value2); // insert a node with value "value"  and key "value2" into the tree, need private function insert(FibNode *node)
	FibNode* removemax(); //remove the max node
	//void combine(FibHeap *other); //combine the trees
	void maximum(int &a, string &b); //Find the key and value of the maximum node, save them at "a" and "b"
	void print(); //print the whole tree, need private function print(FibNode *node, FibNode *prev, int direction)
	void increase(FibNode *node, int value); // increase the key of node by value
	void maximumn(int a, ofstream &s);


private:
	void insert(FibNode *node);// use pointer to insert node
	void addNode(FibNode *node, FibNode *root); // add a node before the root
	void print(FibNode *node, FibNode *prev, int direction);
	void removeNode(FibNode *node);//remove node
	void makeCons(); //create a space for consolidate
	void consolidate();//fix the degree
	FibNode* extractmax();
	void combine(FibNode* node, FibNode* root);
	void cut(FibNode *node, FibNode *parent);
	void cascadingcut(FibNode *node);
	//void updatedegree(FibNode *parent, int degree)

};


FibHeap::FibHeap()
{
	num = 0;
	maxdegree = 0;
	max = NULL;
	cons = NULL;
}

FibHeap::~FibHeap()
{

}

void FibHeap::maximumn(int a, ofstream &s)
{
	if (a > 0)
	{
		FibNode *p;
		p = removemax();
		if (a == 1)
			s << p->key;
		else
			s << p->key << ",";
		maximumn(a - 1, s);
		insert(p);
	}

}

void FibHeap::maximum(int &a, string &b) // find the maximum node and save it to output. return true if success, return false otherwise
{

	b = max->key;
	a = max->value;
}

void FibHeap::removeNode(FibNode *node) //remove a node from list, just change the pointer
{
	node->left->right = node->right;
	node->right->left = node->left;
}

FibNode* FibHeap::removemax()
{
	if (max == NULL)
		return NULL;
	FibNode *child = NULL;
	FibNode *tmp = max;
	// add all the child of max node to the heap's root list
	while (tmp->child != NULL)
	{
		child = tmp->child;
		removeNode(child);
		if (child->right == child)
			tmp->child = NULL;
		else
			tmp->child = child->right;

		addNode(child, max);
		child->parent = NULL;
	}
	//remove the max node
	removeNode(tmp);

	if (tmp->right == tmp)
		max = NULL;
	else
	{
		max = tmp->right; // let the max pointer points to any nonempty root node, and then do consolidate to get the correct answer 
		consolidate();
	}
	num--;
	return tmp;
}

void FibHeap::addNode(FibNode *node, FibNode *root) //change the related pointer to add a node to the heap
{
	node->left = root->left;
	node->left->right = node;
	node->right = root;
	root->left = node;
}

void FibHeap::insert(FibNode *node)
// check if it is the max node， and then just add it to the heap
{
	if (num == 0)
		max = node;
	else
	{
		addNode(node, max);
		if (node->value > max->value)
			max = node;
	}
	num++;
}

FibNode* FibHeap::insert(int input, string input2)
{
	FibNode *node;

	node = new FibNode(input, input2); //create a new FibNode and then add it to the heap
	if (node == NULL)
		return NULL;

	insert(node);
	return node;
}


//print the detail of a heap
// node: the node you are now printing
//prev: the node's parent or brother
//direction: 1,the prev is the node's parent
//			 2, means the prev is the nodes brother.
void FibHeap::print(FibNode *node, FibNode *prev, int direction)
{
	FibNode *start = node;

	if (node == NULL)
		return;
	do // the loop go though all the sibling from the left child
	{
		if (direction == 1)
			cout << node->value << "(" << node->degree << ")(" << node->key << ")" << " is " << prev->value << "'s child" << endl;
		else
			cout << node->value << "(" << node->degree << ")(" << node->key << ")" << " is " <<  prev->value << "'s sibling" << endl;

		if (node->child != NULL) // if a node has children, go downstairs and create a new loop
			print(node->child, node, 1);

		// move to next sibling node
		prev = node;
		node = node->right;
		direction = 2;
	} while (node != start);
}

void FibHeap::print() // print from the max node
{
	int i = 0;
	FibNode *p;

	if (max == NULL)
		return;

	cout << "detail of FibHeap" << endl;
	p = max;
	do {
		i++;
		cout << i << ". "  << p->value << "(" << p->degree << ")" << "(" << p->key << ")" << "  is root" << endl;

		print(p->child, p, 1);
		p = p->right;
	} while (p != max);
	cout << endl;
}

void FibHeap::makeCons()
{
	int olddegree = maxdegree;

	maxdegree = (log(num) / log(2.0)) + 1; //calculate the maxdegree since we have num of nodes

	if (olddegree >= maxdegree)
		return;

	cons = (FibNode **)realloc(cons, sizeof(FibHeap *) *(maxdegree + 1));
}

void FibHeap::consolidate()
{
	int i, d, D;
	FibNode *x, *y;
	makeCons();//create the space for hash
	D = maxdegree + 1;

	for (i = 0; i < D; i++)
		cons[i] = NULL;

	while (max != NULL)
	{
		x = extractmax();
		d = x->degree;
		while (cons[d] != NULL)
		{
			y = cons[d];
			if (x->value < y->value)
				swap(x, y);

			combine(y, x);
			cons[d] = NULL;
			d++;
		}
		cons[d] = x;
	}
	max = NULL;

	for (i = 0; i < D; i++)
	{
		if (cons[i] != NULL)
		{
			if (max == NULL)
				max = cons[i];
			else
			{
				addNode(cons[i], max);
				if ((cons[i])->value > max->value)
					max = cons[i];
			}
		}
	}
}

FibNode *FibHeap::extractmax() // extract the max node and all its subtrees
{
	FibNode *p = max;

	if (p == p->right) // only 1 node in the heap
		max = NULL;
	else
	{
		removeNode(p);
		max = p->right; // max pointer just points to a nonempty node, it will be corrected at function consolidate 

	}
	p->left = p->right = p; //let it be a single node, function removeNode doesn't do this
	return p;
}

void FibHeap::combine(FibNode *node, FibNode *root) // combine two fibonacci tree
{
	removeNode(node);
	if (root->child == NULL)
		root->child = node;
	else
		addNode(node, root->child);

	node->parent = root;  // change the pointer and update the degree and reset the mark
	root->degree++;
	node->mark = false;
}

void FibHeap::increase(FibNode *node, int input) //increase the value of the node by input
{
	FibNode *parent;

	if (max == NULL || node == NULL)
		return;

	node->value += input;
	parent = node->parent;
	if (parent != NULL && node->value > parent->value)
	{
		cut(node, parent);
		cascadingcut(parent);
	}

	if (node->value > max->value)
		max = node;
}

void FibHeap::cut(FibNode *node, FibNode *parent)
{
	removeNode(node);
	//updatedegree(parent, node->degree);
	parent->degree--;
	if (node->right == node)
		parent->child = NULL;
	else
		parent->child = node->right;

	node->parent = NULL;
	node->left = node;
	node->right = node;
	node->mark = false;
	addNode(node, max);
}

void FibHeap::cascadingcut(FibNode *node)
{
	FibNode *parent = node->parent;
	if (parent != NULL)
	{
		if (node->mark == false)
			node->mark = true;
		else
		{
			cut(node, parent);
			cascadingcut(parent);
		}
	}
}

//void FibHeap::updatedegree(FibNode *parent, int degree)
//{
//	parent->degree--;
//}

