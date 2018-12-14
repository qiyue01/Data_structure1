#include "pch.h"
/*
#include <iostream>
#include<vector>
#include<cstring>
#include<list>
using namespace std;

const int MAXN = 500010;
int n, m, s, dep[MAXN], fa[MAXN], top[MAXN], size1[MAXN];
list<int> edge[MAXN];
inline void Add(int u, int v) {
	edge[u].push_back(v);
	edge[v].push_back(u);
}
void dfs(int u) {
	int i, maxs = 0, son = 0;
	top[u] = u;
	size1[u] = 1;
	for (auto &s:edge[u]) {
		if (s == fa[u]) continue;
		fa[s] = u;
		dep[s] = dep[u] + 1;
		dfs(s);
		size1[u] += size1[s];
		if (size1[s] > maxs) maxs = size1[son = s];
	}
	if (son) top[son] = u;
}

int find(int u) {
	return top[u] = top[u] == u ? u : find(top[u]);
}

int LCA(int u, int v) {
	if (find(u) != find(v))
		return dep[top[u]] > dep[top[v]] ? LCA(fa[top[u]], v) : LCA(u, fa[top[v]]);
	else return dep[u] > dep[v] ? v : u;
}
/*
int main()
{
	ios::sync_with_stdio(false);
	int N, M, S;
	cin >> N >> M >> S;
	int u, v;
	for (int i = 0; i < N - 1; ++i)
	{
		cin >> u >> v;
		Add(u, v);
		Add(v, u);
	}
	dfs(S);
	for (int i = 0; i < M; ++i)
	{
		cin >> u >> v;
		cout << LCA(u, v) << endl;
	}
	return 0;
}*/
