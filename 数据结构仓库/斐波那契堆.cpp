#include"pch.h"
#include<string>
#include<vector>
#include<regex>
#include<algorithm>
#include<random>
#include<map>
#include<set>
#include<deque>
#include<iomanip>
#include<sstream>
#include<stack>
#include<iostream>
#include<limits>
#include<bitset>
#include<list>
#include<queue>
#include<memory.h>
#include<functional>
#include<utility>
using namespace std;
template<typename T>
struct Fibonacci_node
{
	Fibonacci_node<T> *father;
	Fibonacci_node<T> *child;
	Fibonacci_node<T> *left;
	Fibonacci_node<T> *right;
	bool mark;
	long degree;
	T storage;
	Fibonacci_node(Fibonacci_node<T>* Father, Fibonacci_node<T> *Child, Fibonacci_node<T> *Left, Fibonacci_node<T> *Right, bool Mark, long Degree, T a) :
		father(Father), child(Child), left(Left), right(Right), mark(Mark), degree(Degree), storage(a) {};
};
template<typename T>
class Fibonacci_heap //´ý¸ÄÁ¼
{
protected:
	Fibonacci_node<T> * Min;
	Fibonacci_node<T> * root_list;
	long node_sum;
public:
	void decrease_key(Fibonacci_node<T> *a, T t)
	{
		a->storage = t;
		Fibonacci_node<T> *interim = a->father;
		if (interim != NULL && a->storage < interim->storage)
		{
			cut(a, interim);
			cascading_cut(interim);
		}
		if (a->storage < Min->storage)
			Min = a;
	}
	long size()
	{
		return node_sum;
	}
	Fibonacci_heap() :Min(NULL), node_sum(0)
	{
		root_list = new Fibonacci_node<T>(NULL, NULL, NULL, NULL, false, 0, T());
	};
	Fibonacci_node<T>* insert(T a)
	{
		Fibonacci_node<T> *part = new Fibonacci_node<T>(NULL, NULL, NULL, NULL, false, 0, a);
		if (Min == NULL)
		{
			part->left = root_list;
			part->right = root_list;
			root_list->left = part;
			root_list->right = part;
			Min = part;
		}
		else
		{
			part->left = root_list->left;
			part->right = root_list;
			root_list->left->right = part;
			root_list->left = part;
		}
		if (part->storage < Min->storage)
			Min = part;
		node_sum++;
		return part;
	}
	void insert(Fibonacci_node<T> *part)
	{
		if (Min == NULL)
		{
			part->left = root_list;
			part->right = root_list;
			root_list->left = part;
			root_list->right = part;
			Min = part;
		}
		else
		{
			part->left = root_list->left;
			part->right = root_list;
			root_list->left->right = part;
			root_list->left = part;
		}
		if (part->storage < Min->storage)
			Min = part;
		node_sum++;
	}
	void Fibonacci_heap_union(Fibonacci_heap &a)
	{
		if (node_sum == 0)
		{
			*this = a;
			return;
		}
		else if (a.size() == 0)
			return;
		Fibonacci_node<T> * part3 = NULL;
		part3 = root_list->right;
		root_list->right = a.root_list->right;
		a.root_list->right->left = root_list;
		part3->left = a.root_list->left;
		a.root_list->left->right = part3;
		if (a.Min->storage < Min->storage)
			Min = a.Min;
		node_sum += a.size();
	}
	T extract_min()
	{
		T p = Min->storage;
		Fibonacci_node<T> *deleting = Min;
		if (Min != NULL)
		{
			if (Min->child != NULL)
			{
				Fibonacci_node<T> *children = NULL;
				while (Min->child->right != Min->child)
				{
					children = Min->child->right;
					Min->child->right = children->right;
					children->right->left = Min->child;
					children->left = root_list;
					root_list->right->left = children;
					children->right = root_list->right;
					root_list->right = children;
					children->father = NULL;
				}
				Min->child->left = root_list;
				root_list->right->left = Min->child;
				Min->child->right = root_list->right;
				root_list->right = Min->child;
				Min->child->father = NULL;
				Min->child = NULL;
			}
			if (node_sum == 1)
				Min = NULL;
			else
			{
				Min->left->right = Min->right;
				Min->right->left = Min->left;
				Min = NULL;
				consolidate();
			}
		}
		node_sum--;
		return p;
	}
	void consolidate()
	{
		vector<Fibonacci_node<T> *> path(50, NULL);
		Fibonacci_node<T> *interim = root_list->right;
		while (interim != root_list)
		{
			Fibonacci_node<T> *interim2 = interim;
			interim = interim->right;
			int d = interim2->degree;
			while (path[d] != NULL)
			{
				Fibonacci_node<T> *part = path[d];
				if (interim2->storage > part->storage)
					swap(interim2, part);
				Fibonacci_heap_link(part, interim2);
				path[d] = NULL;
				d++;
			}
			path[d] = interim2;
		}
		for (auto s : path)
			if (s != NULL)
				if (Min == NULL)
					Min = s;
				else if (s->storage < Min->storage)
					Min = s;

	}
	void Fibonacci_heap_link(Fibonacci_node<T> *y, Fibonacci_node<T> *x)
	{
		y->left->right = y->right;
		y->right->left = y->left;
		if (x->child != NULL)
		{
			y->left = x->child;
			y->right = x->child->right;
			x->child->right->left = y;
			x->child->right = y;
		}
		else
		{
			x->child = y;
			y->right = y;
			y->left = y;
		}
		y->father = x;
		x->degree++;
		y->mark = false;
	}
	void cut(Fibonacci_node<T> * x, Fibonacci_node<T> * y)
	{
		if (y->degree == 1)
			y->child = NULL;
		else
		{
			y->child = x->right;
			x->left->right = x->right;
			x->right->left = x->left;
		}
		y->degree--;
		x->right = root_list->right;
		x->left = root_list;
		root_list->right->left = x;
		root_list->right = x;
		x->father = NULL;
		x->mark = false;
	};
	void cascading_cut(Fibonacci_node<T> *y)
	{
		Fibonacci_node<T> * z = y->father;
		if (z != NULL)
			if (y->mark == false)
				y->mark = true;
			else
			{
				cut(y, z);
				cascading_cut(z);
			}
	};
	void Delete(Fibonacci_node<T> *x)
	{
		decrease_key(x, -1000000);
		extract_min();
	}
};
