#include"pch.h"
#include<string>
#include<vector>

#include<algorithm>
#include<cstring>
#include<cstdio>
#include<deque>
#include<iomanip>
#include<sstream>
#include<stack>
#include<iostream>
#include<limits>
#include<bitset>
#include<list>
#include<queue>
#include<memory>
#include<functional>
using namespace std;
namespace tree1
{
	const int N = 200010;
	struct graph
	{
		long long Begin[N * 2], to[N * 2], Next[N * 2], e, weight[N * 2];
		void add(int u, int v)
		{
			to[++e] = v;
			Next[e] = Begin[u];
			Begin[u] = e;
		}
		void add(int u, int v, int w)
		{
			to[++e] = v;
			Next[e] = Begin[u];
			Begin[u] = e;
			weight[e] = w;
		}
		void init_graph()
		{
			e = 0;
			memset(Begin, -1, sizeof(Begin));
		}
	};
	int Distance[N];
	int vis[N];
	void bfs(graph &g, int a)
	{
		queue<int> que;
		que.push(a);
		int p;
		while (!que.empty())
		{
			p = que.front();
			que.pop();
			for (int i = g.Begin[p]; i != -1; i = g.Next[i])
				if (vis[g.to[i]] == 0)
				{
					que.push(g.to[i]);
					vis[g.to[i]] = 1;
					Distance[g.to[i]] += g.weight[i] + Distance[p];
				}
		}
	}
}
using namespace tree1;
