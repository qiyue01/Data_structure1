#include"pch.h"
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
const int MAXN = 110000 + 1;
const int MAXG = MAXN * 2 + 1;
const int MAX_LOG = 19;
const int inf = 0x3f3f3f3f;
int n, q;

struct Edge {
	long long v, w;

	Edge(int v, int w) : v(v), w(w) {}
};

std::vector<Edge> edge[MAXN], g[MAXG];
int a[MAXN];
int tot;

inline void addEdge(int u, int v, int w) {
	//cout << u <<" "<< v << endl;
	g[u].emplace_back(v, w);
	g[v].emplace_back(u, w);
}

inline void binarize(int u, int pre)
{
	static std::vector<Edge *> now;
	now.clear();
	for (auto &p : edge[u])
		if (p.v != pre) now.push_back(&p);
	int o = u;
	for (int i = 0; i < (int)now.size(); i++) {
		addEdge(o, now[i]->v, now[i]->w);
		if (now.size() - i > 2) {
			addEdge(o, ++tot, 0);
			o = tot;
		}
	}
	for (auto &p : edge[u])
		if (p.v != pre) binarize(p.v, u);
}

bool vis[MAXG], vis2[MAXG];
long long sz[MAXG], idx[MAXG], dep[MAXG], fa[MAXG], weight[MAXG];
long long dis[MAXG][MAX_LOG];
long long trans[MAXG][3];

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

void dfs(int u, int pre, int d, int kk) {
	for (auto &p : g[u]) {
		if (!vis[p.v] && p.v != pre) {
			idx[p.v] = idx[u];
			dis[p.v][kk] = dis[u][kk] + p.w;
			if (p.v <= n && vis2[p.v] == 1)
				trans[d][idx[p.v]] = min(trans[d][idx[p.v]], dis[p.v][kk]);
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
	for (auto &p : g[u]) {
		if (!vis[p.v]) {
			dis[p.v][d] = p.w;
			idx[p.v] = c++;
			if (p.v <= n && vis2[p.v] == 1)
				trans[u][idx[p.v]] = min(trans[u][idx[p.v]], p.w);
			dfs(p.v, u, u, d);
			build(p.v, d + 1, u);
		}
	}
}
long long L, R;
long long cur_ans;
long long query(int cur, int kk, int id)
{
	if (!cur)
		return 0;
	for (int i = 0; i < 3; ++i)
		if (id != i)
			cur_ans = min(cur_ans, trans[cur][i] + dis[kk][dep[cur]]);
	if (vis2[cur])
		cur_ans = min(cur_ans, dis[kk][dep[cur]]);
	query(fa[cur], kk, idx[cur]);
}
void update(int cur, int kk, int id)
{
	if (!cur)
		return;
	if (id != -1)
		trans[cur][id] = min(trans[cur][id], dis[kk][dep[cur]]);
	update(fa[cur], kk, idx[cur]);
}
int32_t main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int  u, v, w, a, b, q, kk;
	cin >> n >> q;

	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < MAXG; ++j)
			trans[j][i] = inf;
	memset(vis2, 0, sizeof(vis2));
	vis2[1] = 1;
	for (int i = 0; i < n - 1; ++i)
	{
		cin >> u >> v;
		edge[u].push_back({ v,1 });
		edge[v].push_back({ u,1 });
	}
	int opt;
	tot = n;
	binarize(1, 0);
	build(1, 1, 0);
	while (q--)
	{
		cin >> opt >> kk;
		if (opt == 1)
		{
			if (vis2[kk] == 0)
			{
				vis2[kk] = 1;
				update(kk, kk, -1);
			}
		}
		else
		{
			cur_ans = inf;
			if (vis2[kk])
				cout << 0 << '\n';
			else
			{
				query(kk, kk, -1);
				cout << cur_ans << '\n';
			}
		}
	}
	return 0;
}