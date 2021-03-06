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
#include<array>
#include<memory.h>
#include<functional>
using namespace std;
namespace trie01
{
	const int sigma = 31;
	struct node
	{
		node *child[2];
		int times;
		void  clear()
		{
			times = 0;
			child[0] = child[1] = NULL;
		}
	};
	struct trie
	{
		node pool[3600000];
		node *root, *cur = pool;
		node *new_()
		{
			cur->clear();
			return cur++;
		}
		void init()
		{
			cur = pool;
			root = new_();
		}
		bitset<sigma + 1> trans;
		void insert(long long a)
		{
			node *found = root;
			trans = a;
			for (int i = sigma; i >= 0; --i)
			{
				found->times++;
				if (found->child[trans[i]] == NULL)
				{
					found->child[trans[i]] = new_();
					found = found->child[trans[i]];
				}
				else
					found = found->child[trans[i]];
			}
			found->times++;
		}
	}t1, t2;
}
using namespace trie01;
/*
#define int long long
namespace trie01
{
	const int sigma = 35;
	long long base = 1;
	struct node
	{
		node *child[2];
		int times;
		int element;
		bool lazy;
		void clear()
		{
			lazy = times = element = 0;
		}
	};
	node *history[610000];
	node pool[17000000];
	node *root, *cur = pool, *null;
	node *new_()
	{
		cur->clear();
		cur->child[0] = cur->child[1] = null;
		return cur++;
	}
	void init()
	{
		null = new node();
		null->times = null->element = null->lazy = 0;
		null->child[0] = null->child[1] = null;
		root = new_();
		history[0] = new_();
	}
	void insert(node *ver_s, int pos, int element, int add)
	{
		node *found = ver_s;
		bitset<sigma + 1> kkk = pos;
		found->times += add;
		for (int i = sigma; i >= 0; --i)
		{
			if (found->child[kkk[i]] == null)
			{
				found->child[kkk[i]] = new_();
				found = found->child[kkk[i]];
			}
			else
				found = found->child[kkk[i]];
			found->times += add;
		}
		found->element = element;
	}
	node *insert_new(int pos, int element, node *old, int add)
	{
		node *found = old, *_new = new_(), *_new2;
		bitset<sigma + 1> kkk = pos;
		_new2 = _new;
		for (int i = sigma; i >= 0; --i)
		{
			_new->times += add;
			_new->child[kkk[i]] = new_();
			if (found == null)
				_new = _new->child[kkk[i]];
			else
			{
				_new->times += found->times;
				_new->child[!kkk[i]] = found->child[!kkk[i]];
				_new = _new->child[kkk[i]];
				found = found->child[kkk[i]];
			}
		}
		_new->times += found->times;
		_new->times += add;
		_new->element = element;
		return _new2;
	}
	int rank(node *p, int pos)
	{
		bitset<sigma + 1> kkk = pos;
		int rk = 1;
		for (int i = sigma; i >= 0; --i)
		{
			if (kkk[i])
				rk += p->child[0]->times;
			p = p->child[kkk[i]];
		}
		return rk;
	}
	int find(node *p, int pos)
	{
		bitset<sigma + 1> kkk = pos;
		for (int i = sigma; i >= 0; --i)
			p = p->child[kkk[i]];
		return p->times;
	}
	int kth(node *p, int s)
	{
		for (int i = sigma; i >= 0; --i)
			if (p->child[0]->times < s)
			{
				s -= p->child[0]->times;
				p = p->child[1];
			}
			else
				p = p->child[0];
		return p->element;
	}
	int rank(int L, int R, int pos)
	{
		node* PL = history[L - 1], *PR = history[R];
		bitset<sigma + 1> kkk = pos;
		int rk = 1;
		for (int i = sigma; i >= 0; --i)
		{
			if (kkk[i])
				rk += PR->child[0]->times - PL->child[0]->times;
			PL = PL->child[kkk[i]];
			PR = PR->child[kkk[i]];
		}
		return rk;
	}
	int kth(int L, int R, int s)
	{
		node* PL = history[L - 1], *PR = history[R];
		int rk;
		for (int i = sigma; i >= 0; --i)
		{
			rk = PR->child[0]->times - PL->child[0]->times;
			if (rk < s)
			{
				s -= rk;
				PL = PL->child[1];
				PR = PR->child[1];
			}
			else
			{
				PL = PL->child[0];
				PR = PR->child[0];
			}
		}
		return rk;
	}
}
using namespace trie01;
long long Max = INT_MAX;
*/