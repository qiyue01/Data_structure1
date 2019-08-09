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
	const int sigma = 26;
	long long base = 1;
	struct node
	{
		node *child[2];
		int times;
		bool lazy;
		node()
		{
			lazy = times = 0;
		}
	};
	node pool[14500000];
	node *root, *cur = pool, *null;
	node *history[610000];
	node *new_()
	{
		cur->child[0] = cur->child[1] = null;
		return cur++;
	}
	void init()
	{
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
	void push_down()
	{

	}
	void insert(int a)
	{
		node *found = root;
		trans(a);
		found->times++;
		for (int i = 0; i < sigma; ++i)
		{
			if (found->child[kkk[i]] == null)
			{
				found->child[kkk[i]] = new_();
				found = found->child[kkk[i]];
			}
			else
				found = found->child[kkk[i]];
			found->times++;
		}
	}
	node *insert_new(int a, node *old)
	{
		node *found = old, *_new = new_(), *_new2;
		//cout << old->times << endl;
		_new2 = _new;
		trans(a);
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
		return _new2;
	}
	int query(int x, int L, int R)
	{
		node *l = history[max(L - 2, 0)], *r = history[R - 1];
		trans(x);
		int ret;
		for (int i = 0; i < sigma; ++i)
		{
			ret = r->child[!kkk[i]]->times - l->child[!kkk[i]]->times;
			if (ret)
			{
				r = r->child[(!kkk[i])];
				l = l->child[(!kkk[i])];
				ans[i] = !kkk[i];
			}
			else
			{
				r = r->child[(kkk[i])];
				l = l->child[(kkk[i])];
				ans[i] = kkk[i];
			}
		}
		x = x ^ retrans();
		return x;
	}
}
using namespace trie01;