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
using namespace std;
namespace segment_2D
{
	const int N = 3000010;
	struct node
	{
		node * l, *r;
		int pos[2][2];
		int split;
		int val, lazy, maxval;
		void clear()
		{
			maxval = split = pos[0][1] = pos[0][0] = pos[1][1] = pos[1][0] = -1;
			l = r = 0;
			lazy = val = 0;
		}
	};
	node pool[N];
	node * null = pool, *cur = pool, *root;
	node * new_node()
	{
		++cur;
		cur->l = null, cur->r = null;
		return cur;
	}
	void build_root(int x1, int y1, int x2, int y2)
	{
		root = new_node();
		null->val = 0;
		root->pos[0][0] = x1, root->pos[0][1] = x2, root->pos[1][0] = y1, root->pos[1][1] = y2;
		root->split = 0;
		if (x1 == x2)
			root->split = 1;
	}
	void  pl(node * p)
	{
		if (p->pos[0][0] == p->pos[0][1] && p->pos[1][0] == p->pos[1][1])
			return;
		if (p->l != null)
		{
			return;
		}
		p->l = new_node();
		if (p->pos[0][0] == p->pos[0][1])
		{
			int mid = (p->pos[1][0] + p->pos[1][1]) / 2;
			p->l->pos[0][0] = p->pos[0][0], p->l->pos[0][1] = p->pos[0][1];
			p->l->pos[1][0] = p->pos[1][0], p->l->pos[1][1] = mid;
			p->l->split = 1;
		}
		else if (p->pos[1][0] == p->pos[1][1])
		{
			int mid = (p->pos[0][0] + p->pos[0][1]) / 2;
			p->l->pos[1][0] = p->pos[1][0], p->l->pos[1][1] = p->pos[1][1];
			p->l->pos[0][0] = p->pos[0][0], p->l->pos[0][1] = mid;
			p->l->split = 0;
		}
		else
		{
			int k = p->split;
			int mid = (p->pos[k][0] + p->pos[k][1]) / 2;
			p->l->split = !k;
			p->l->pos[!k][0] = p->pos[!k][0], p->l->pos[!k][1] = p->pos[!k][1];
			p->l->pos[k][0] = p->pos[k][0], p->l->pos[k][1] = mid;
		}
	}
	void pr(node * p)
	{
		if (p->pos[0][0] == p->pos[0][1] && p->pos[1][0] == p->pos[1][1])
			return;
		if (p->r != null)
			return;
		p->r = new_node();
		if (p->pos[0][0] == p->pos[0][1])
		{
			int mid = (p->pos[1][0] + p->pos[1][1]) / 2;
			p->r->pos[0][0] = p->pos[0][0], p->r->pos[0][1] = p->pos[0][1];
			p->r->pos[1][0] = mid + 1, p->r->pos[1][1] = p->pos[1][1];
			p->r->split = 1;
		}
		else if (p->pos[1][0] == p->pos[1][1])
		{
			int mid = (p->pos[0][0] + p->pos[0][1]) / 2;
			p->r->pos[1][0] = p->pos[1][0], p->r->pos[1][1] = p->pos[1][1];
			p->r->pos[0][0] = mid + 1, p->r->pos[0][1] = p->pos[0][1];
			p->r->split = 0;
		}
		else
		{
			int k = p->split;
			int mid = (p->pos[k][0] + p->pos[k][1]) / 2;
			p->r->split = !k;
			p->r->pos[!k][0] = p->pos[!k][0], p->r->pos[!k][1] = p->pos[!k][1];
			p->r->pos[k][0] = mid + 1, p->r->pos[k][1] = p->pos[k][1];

		}
	}
	int query(node *p, int x1, int y1, int x2, int y2)
	{
		if (p == 0 || p->val == 0)
			return 0;
		if (p->pos[0][0] >= x1 && p->pos[0][1] <= x2 && p->pos[1][0] >= y1 && p->pos[1][1] <= y2)
			return p->val;
		int k = p->split;
		int mid = (p->pos[k][0] + p->pos[k][1]) / 2;
		if (k)
		{
			if (y1 >= mid + 1)
				return query(p->r, x1, y1, x2, y2);
			else if (y2 <= mid)
				return query(p->l, x1, y1, x2, y2);
			else if (y1 <= mid && y2 >= mid + 1)
				return query(p->l, x1, y1, x2, mid) + query(p->r, x1, mid + 1, x2, y2);
		}
		else
		{
			if (x1 >= mid + 1)
				return query(p->r, x1, y1, x2, y2);
			else if (x2 <= mid)
				return query(p->l, x1, y1, x2, y2);
			else if (x1 <= mid && x2 >= mid + 1)
				return query(p->l, x1, y1, mid, y2) + query(p->r, mid + 1, y1, x2, y2);
		}

	}
	void insert(node *p, int x1, int y1, int val)
	{
		p->val += val;
		if (p->pos[0][0] == x1 && p->pos[0][1] == x1)
			if (p->pos[1][0] == y1 && p->pos[1][1] == y1)
				return;
		int k = p->split;
		int mid = (p->pos[k][0] + p->pos[k][1]) / 2;
		if (!k)
		{
			if (x1 >= mid + 1)
				pr(p), insert(p->r, x1, y1, val);
			else
				pl(p), insert(p->l, x1, y1, val);
		}
		else
		{
			if (y1 >= mid + 1)
			{
				pr(p), insert(p->r, x1, y1, val);
			}
			else
				pl(p), insert(p->l, x1, y1, val);
		}
	}
}
using namespace segment_2D;