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
	const int sigma = 32;
	long long base = 1;
	struct node
	{
		node *child[2];
		int times;
		node()
		{
			times = 0;
			child[0] = child[1] = NULL;
		}
	};
	node pool[1500000];
	node *root, *cur = pool;
	node *new_()
	{
		return cur++;
	}
	void init()
	{
		root = new_();
	}
	array<bool, sigma> kkk, ans;
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
	void insert(int a)
	{
		node *found = root;
		trans(a);
		for (int i = 0; i < sigma; ++i)
		{
			if (found->child[kkk[i]] == NULL)
			{
				found->child[kkk[i]] = new_();
				found = found->child[kkk[i]];
			}
			else
				found = found->child[kkk[i]];
		}
		found->times++;
	}
	long long query(long long a)
	{
		node *found = root;
		trans(a);
		for (int i = 0; i < sigma; ++i)
		{
			if (found->child[(!kkk[i])])
			{
				found = found->child[(!kkk[i])];
				ans[i] = !kkk[i];
			}
			else
			{
				found = found->child[kkk[i]];
				ans[i] = kkk[i];
			}
		}
		return retrans() ^ a;
	}
}
using namespace trie01;