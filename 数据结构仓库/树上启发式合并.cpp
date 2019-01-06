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
namespace dsu_on_tree
{
	const int N = 100000 + 50;
	int to[2 * N + 5], Next[2 * N + 5], head[N];
	map<int, int>* ans[N];
	int topo1[N], ans2[N], time, fa[N], K, cnt;
	void add(int u, int v)
	{
		to[++cnt] = v;
		Next[cnt] = head[u];
		head[u] = cnt;
	}
	void insert(int u, int v)
	{
		add(u, v);
		add(v, u);
	}
	void init(int n)
	{
		for (int i = 0; i < n; ++i)
			ans[i]=new map<int,int>();
		memset(ans2, 0, sizeof(ans2));
		cnt = 0;
		memset(fa, 0, sizeof(fa));
		memset(head, -1, sizeof(head));
	}
	map<int, int> * merge1(map<int, int> *a, map<int, int> *b, int p)
	{
		if (a->size() > b->size())
		{
			ans2[fa[p]] = ans2[p];
			p = fa[p];
			for (auto &s : *(b))
			{
				if (a->find(s.first) != a->end()) {
					int p2 = (*a)[s.first];
					if (p2 == K)
						ans2[p]--;
					else if (p2 + s.second == K)
						ans2[p]++;
					(*a)[s.first] = s.second + p2;
				}
				else
				{
					a->insert(s);
					if (s.second == K)
						ans2[p]++;
				}
			}
			b->clear();
			return a;
		}
		p = fa[p];
		for (auto &s : *a) {
			if (b->find(s.first) != b->end())
			{
				int p2 = (*b)[s.first];
				if (p2 == K)
					ans2[p]--;
				else if (p2 + s.second == K)
					ans2[p]++;
			    (*b)[s.first] = s.second + p2;
			}
			else
			{
				b->insert(s);
				if (s.second == K)
					ans2[p]++;
			}
		}
		a->clear();
		return b;
	}

	void topo(int root)
	{
		memset(fa, -1, sizeof(fa));
		memset(topo1, 0, sizeof(topo1));
		queue<int> que;
		que.push(root);
		int cur = 0;
		time = 0;
		while (!que.empty())
		{
			cur = que.front();
			topo1[time++] = cur;
			que.pop();
			for (int i = head[cur]; i != -1; i = Next[i])
				if (to[i] != fa[cur])
				{
					que.push(to[i]);
					fa[to[i]] = cur;
				}
		}
	}
	void solve(int N)
	{
		topo(1);
		for (int i = 1; i <= N; ++i)
			if (K == 1)
				ans2[i] = 1;
		for (int i = time - 1; i > 0; --i)
			 ans[fa[topo1[i]]]=merge1(ans[topo1[i]], ans[fa[topo1[i]]], topo1[i]);
	}
}
using namespace dsu_on_tree;


