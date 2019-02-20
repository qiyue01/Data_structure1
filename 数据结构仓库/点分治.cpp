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
namespace tree_split2
{


	struct graph
	{
		int Begin[N * 2], to[N * 2], Next[N * 2], e, weight[N * 2];
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
	graph p;
	bool vis[N];
	int root, size1[N], size2[N], allnode;
	void getroot(int now, int fa)  //得到根
	{
		size1[now] = 1;
		size2[now] = 0;
		for (int i = p.Begin[now]; i != -1; i = p.Next[i])
		{
			int t = p.to[i];
			if (t == fa || vis[t]) continue;
			getroot(t, now);
			size1[now] += size1[t];
			size2[now] = max(size2[now], size1[t]);
		}
		size2[now] = max(allnode - size1[now], size2[now]);
		if (size2[now] < size2[root]) root = now;
	}
	int ans;
	int deep[2 * N];
	void getdeep(int r, int fa) //当前轮处理数据
	{
		for (int i = p.Begin[r]; i != -1; i = p.Next[i])
		{
			if (p.to[i] != fa && vis[p.to[i]] == false)
			{
				deep[p.to[i]] = deep[r] + p.weight[i];
				getdeep(p.to[i], r);
			}
		}
	}
	void calc2(int a, int fa)//计算当前子树
	{
		for (int i = p.Begin[a]; i != -1; i = p.Next[i])
		{
			if (p.to[i] != fa && vis[p.to[i]] == false)
				calc2(p.to[i], a);
		}
	}
	int sumnode;
	void del_(int a, int fa, int now) //清除当前子树贡献
	{
		sumnode++;
		for (int i = p.Begin[a]; i != -1; i = p.Next[i])
		{
			if (p.to[i] != fa && vis[p.to[i]] == false)
				del_(p.to[i], a, now);
		}
	}
	void ins_(int a, int fa)//回退当前子树贡献
	{
		for (int i = p.Begin[a]; i != -1; i = p.Next[i])
		{
			if (p.to[i] != fa && vis[p.to[i]] == false)
				ins_(p.to[i], a);
		}
	}
	int all_[N * 2];
	void calc(int r)
	{
		getroot(r, r);
		vis[root] = true;
		deep[root] = 0;
		getdeep(root, root);
		for (int i = p.Begin[root]; i != -1; i = p.Next[i])
		{
			int L = p.to[i];
			if (vis[L] == false)
			{
				del_(L, L, L);
				calc2(L, L);
				ins_(L, L);
				all_[L] = size1[L];
			}
		}
		for (int i = p.Begin[root]; i != -1; i = p.Next[i])
			if (vis[p.to[i]] == false)
			{
				root = 0;
				allnode = all_[p.to[i]];
				calc(p.to[i]);
			}
	}
	void init(int n)
	{
		memset(vis, 0, sizeof(vis));
		memset(size1, 0, sizeof(size1));
		memset(size2, 0, sizeof(size2));
		root = 0;
		p.init_graph();
		allnode = n;
		size2[0] = n + 10;
	}
	int gcd(int a, int b)
	{
		if (b == 0) return a;
		return gcd(b, a%b);
	}

}
using namespace tree_split2;