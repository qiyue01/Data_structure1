#include<memory>
#include<memory.h>
#include<array>
#include <iostream>
#include<vector>
#include<string>
#include<list>
#include<set>
#include<queue>
#include<map>
#include<stack>
#include<algorithm>
#include<unordered_map>
#include<unordered_set>
using namespace std;
const int MAXN = 1500 + 1;
const int MAXG = MAXN * 2 + 1;
const int MAX_LOG = 21;
const int inf = 0x3f3f3f3f;
int n, q;

struct Edge
{
	int v, w;
	Edge(int v, int w) : v(v), w(w) {}
};

std::vector<Edge> g[MAXG];
int a[MAXN];
int tot;

inline void addEdge(int u, int v, int w) {
	//cout << u <<" "<< v << endl;
	g[u].emplace_back(v, w);
	g[v].emplace_back(u, w);
}
int deep[MAXG];
bool vis[MAXG], vis2[MAXG];
long long sz[MAXG], idx[MAXG], dep[MAXG], fa[MAXG], weight[MAXG];
long long dis[MAXG][MAX_LOG];
int lca[MAXG][MAX_LOG], Min[MAXG][MAX_LOG];
void dfsSize(int u, int pre) {
	sz[u] = 1;
	for (auto &p : g[u]) {
		if (!vis[p.v] && p.v != pre) {
			dfsSize(p.v, u);
			sz[u] += sz[p.v];
		}
	}
}
int get(int u, int pre, int n) {
	for (auto &p : g[u])
		if (!vis[p.v] && p.v != pre && sz[p.v] > n) return get(p.v, u, n);
	return u;
}
void dfs1(int u, int p)
{
	for (auto &s : g[u])
		if (s.v != p)
			deep[s.v] = deep[u] + 1, dfs1(s.v, u);
}
void dfs(int u, int pre, int d, int kk) {
	for (auto &p : g[u]) {
		if (!vis[p.v] && p.v != pre) {
			idx[p.v] = idx[u];
			Min[p.v][kk] = min(Min[u][kk], p.w);
			dis[p.v][kk] = dis[u][kk] + p.w;
			if (deep[p.v] > deep[u])
				lca[p.v][kk] = lca[u][kk];
			else
				lca[p.v][kk] = p.v;
			dfs(p.v, u, d, kk);
		}
	}
}

void build(int u, int d, int f)
{
	dfsSize(u, 0);
	vis[u = get(u, 0, sz[u] / 2)] = true;
	fa[u] = f;
	dep[u] = d;
	int c = 0;
	lca[u][dep[u]] = u;
	for (auto &p : g[u]) {
		if (!vis[p.v]) {
			dis[p.v][d] = p.w;
			Min[p.v][d] = p.w;
			idx[p.v] = c++;
			{
				if (deep[p.v] > deep[u])
					lca[p.v][dep[u]] = u;
				else
					lca[p.v][dep[u]] = p.v;
			}
			dfs(p.v, u, u, d);
			build(p.v, d + 1, u);
		}
	}
}
int getfa(int a, int b)
{
	int u, v;
	u = a, v = b;
	while (u != v)
	{
		if (dep[u] < dep[v])
			swap(u, v);
		else
			u = fa[u];
	}
	return u;
}
int query_min(int a, int b)
{
	int u = getfa(a, b);
	//cout << a << ": " << b << " " << u << endl;
	if (a == u)
		return Min[b][dep[u]];
	if (b == u)
		return Min[a][dep[u]];
	return min(Min[a][dep[u]], Min[b][dep[u]]);
}
int query_dis(int a, int b)
{
	int u = getfa(a, b);
	return dis[a][dep[u]] + dis[b][dep[u]];
}
int query_lca(int a, int b)
{
	int u = getfa(a, b);
	if (deep[lca[a][dep[u]]] < deep[lca[b][dep[u]]])
		return lca[a][dep[u]];
	else
		return lca[b][dep[u]];
}
const int INF = 0x3f3f3f3f;