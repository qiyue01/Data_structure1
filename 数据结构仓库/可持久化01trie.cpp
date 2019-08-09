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
	const int sigma = 21;
	long long base = 1;
	struct node
	{
		node *child[2];
		int times;
		int element;
		void clear()
		{
			times = element = 0;
		}
	};
	node pool[10000000];
	node *root, *cur = pool, *null;
	node *history[610000];
	node *new_()
	{
		cur->clear();
		cur->child[0] = cur->child[1] = null;
		return cur++;
	}
	void init()
	{
		cur = pool;
		null = new node();
		null->times = 0;
		null->child[0] = null->child[1] = null;
		root = new_();
		history[0] = new_();
	}
	array<bool, sigma> kkk, ans, _L, _R;
	void trans(int a)
	{
		kkk.fill(0);
		for (int i = 0; i < sigma; ++i)
		{
			kkk[i] = a & 1;
			a = a >> 1;
		}
		reverse(kkk.begin(), kkk.end());
	}
	long long retrans()
	{
		long long a = base << sigma, ans1 = 0;
		for (auto &s : ans)
		{
			a = a >> 1;
			ans1 += s * a;
		}
		return ans1;
	}
	void insert(node *ver_s, int pos, int element, int add)
	{
		node *found = ver_s;
		trans(pos);
		found->times += add;
		for (int i = 0; i < sigma; ++i)
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
	node *insert_new(int pos, int element, node *old)
	{
		node *found = old, *_new = new_(), *_new2;
		//cout << old->times << endl;
		_new2 = _new;
		trans(pos);
		for (int i = 0; i < sigma; ++i)
		{
			_new->times++;
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
		_new->times++;
		_new->element = element;
		return _new2;
	}
	int rank(node *p, int pos)
	{
		trans(pos);
		int rk = 1; //1的话含义即为新数的rank 0的话ans即表示有多少个数在当前数前面
		for (int i = 0; i < sigma; ++i)
		{
			if (kkk[i])
				rk += p->child[0]->times;
			p = p->child[kkk[i]];
		}
		return rk;
	}

	int kth(node *p, int s)
	{
		for (int i = 0; i < sigma; ++i)
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
		trans(pos);
		int rk = 0;
		for (int i = 0; i < sigma; ++i)
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
		for (int i = 0; i < sigma; ++i)
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
		return PR->element;
	}
}
using namespace trie01;