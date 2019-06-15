#include "pch.h"
// luogu-judger-enable-o2

#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include<vector>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
typedef long long LL;
const int MAXN = 200005, INF = 2e9;

int n, q, col[MAXN], ans;

struct Edge {
	int to, w, nxt;
} gra[MAXN << 1], grao[MAXN << 1];
int head[MAXN], heado[MAXN], ecnt, ecnto;

inline void addedge(int u, int v, int w)
{
	gra[ecnt] = Edge{ v, w, head[u] }; head[u] = ecnt++;
}

inline void addedgeo(int u, int v, int w) {
	grao[ecnto] = Edge{ v, w, heado[u] }; heado[u] = ecnto++;
}

// 重构树，使得树的结构有利于边分治
inline void rebuild(int u, int fa) {
	int ff = 0;
	for (int i = heado[u]; ~i; i = grao[i].nxt) {
		int v = grao[i].to, w = grao[i].w;
		if (v == fa) continue;
		if (!ff) {
			addedge(u, v, w);
			addedge(v, u, w);
			ff = u;
		}
		else {
			int k = ++n;
			col[k] = 1;
			addedge(ff, k, 0);
			addedge(k, ff, 0);
			addedge(k, v, w);
			addedge(v, k, w);
			ff = k;
		}
		rebuild(v, u);
	}
}

bool del[MAXN << 1];
int ct, ctsiz, sum, k;
int siz[MAXN], msz[MAXN];

// 计算子树大小
inline void calsiz(int u, int fa) {
	siz[u] = 1;
	for (int i = head[u]; ~i; i = gra[i].nxt) {
		int v = gra[i].to;
		if (del[i >> 1] || v == fa) continue;
		calsiz(v, u);
		siz[u] += siz[v];
	}
}

// 找到子树的中心边
inline void findct(int u, int fa) {
	for (int i = head[u]; ~i; i = gra[i].nxt) {
		int v = gra[i].to;
		if (del[i >> 1] || v == fa) continue;
		findct(v, u);
		int vsiz = std::max(siz[v], sum - siz[v]);
		if (vsiz < ctsiz) {
			ct = i;
			ctsiz = vsiz;
		}
	}
}

struct DisData {
	int u, d; // 节点，节点到根的距离
	inline bool operator<(const DisData &rhs) const {
		return d < rhs.d;
	}
};

std::priority_queue<DisData> s[MAXN][2];
int cnt;

struct NodeData {
	int bel, side, dis; // 子分治结构的编号，中心边的左侧或右侧，到根的距离
};
std::vector<NodeData> ndata[MAXN];
map<int, int> dis[2];
// 把整棵子树的点到根的距离全部求出来，并记下每个点都属于哪些子树
inline void caldis(int u, int fa, int d, int t, int l) {
	if (!col[u])//非虚点处理信息
	{
		s[t][l].push(DisData{ u, d }); //记录下分治结构的距离数据
		ndata[u].push_back(NodeData{ t, l, d }); //记录节点涉及的分治结构，方向以及距离
	}
	for (int i = head[u]; ~i; i = gra[i].nxt) {
		int v = gra[i].to, w = gra[i].w;
		if (del[i >> 1] || v == fa) continue;
		caldis(v, u, d + w, t, l);
	}
}

int mx[MAXN], lch[MAXN], rch[MAXN];
int ctw[MAXN];//中心边边权
int cur_ans;
// 用于更新某个中心边的信息
inline void update(int p)
{
	while (!s[p][0].empty() && col[s[p][0].top().u]) s[p][0].pop();
	while (!s[p][1].empty() && col[s[p][1].top().u]) s[p][1].pop();
	if (s[p][0].empty() || s[p][1].empty()) mx[p] = 0;
	else mx[p] = s[p][0].top().d + ctw[p] + s[p][1].top().d; // 统计经过中心边的路径最大值
	// 统计子分治结构的最大值
	if (lch[p]) mx[p] = std::max(mx[p], mx[lch[p]]);
	if (rch[p]) mx[p] = std::max(mx[p], mx[rch[p]]);
}

// 边分的divide函数
inline int divide(int u) {
	calsiz(u, 0);
	ct = -1; ctsiz = INF; sum = siz[u]; findct(u, 0);
	if (ct == -1) return 0;
	int x = gra[ct].to, y = gra[ct ^ 1].to;
	del[ct >> 1] = true; // 要先删边再递归
	int t = ++cnt;
	ctw[t] = gra[ct].w;

	caldis(x, 0, 0, t, 0); caldis(y, 0, 0, t, 1); //分治递推处理信息
	//静态边分可以直接在这里写信息 涉及边权需要考虑虚边边权
	{
	}
	lch[t] = divide(x); rch[t] = divide(y);//中心边向两边递推处理
	// 合并答案
	update(t);
	return t;
}

// 设为白点，需要把这个点的距离信息加入它属于的所有子分治结构中
inline void setwhite(int u) {
	for (int i = ndata[u].size() - 1; i >= 0; i--) {
		NodeData d = ndata[u][i];
		s[d.bel][d.side].push(DisData{ u, d.dis });
		update(d.bel);
	}
}

// 设为黑点，需要把这个点的距离信息从它属于的所有子分治结构中删除
inline void setblack(int u) {
	for (int i = ndata[u].size() - 1; i >= 0; i--) {
		NodeData d = ndata[u][i];
		update(d.bel);
	}
}

int ut, vt, wt;
char op;

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	memset(head, -1, sizeof(head));
	memset(heado, -1, sizeof(heado));
	cin >> n;
	int u, v;
	for (int i = 0; i < n - 1; ++i)
	{
		cin >> u >> v;
		addedgeo(u, v, 1);
		addedgeo(v, u, 1);
	}
	int aaa = n;
	rebuild(1, 0);
	divide(1);
	int q;
	cin >> q;
	while (q--)
	{
		cin >> op;
		if (op == 'C')
		{
			cin >> u;
			col[u] ^= 1;
			if (col[u])
			{
				setblack(u);
				aaa--;
			}
			else
			{
				setwhite(u);
				aaa++;
			}
		}
		else
		{
			if (aaa == 0)
				cout << -1 << '\n';
			else
				cout << mx[1] << '\n';
		}
	}
	return 0;
}