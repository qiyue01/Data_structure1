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

// �ع�����ʹ�����Ľṹ�����ڱ߷���
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

// ����������С
inline void calsiz(int u, int fa) {
	siz[u] = 1;
	for (int i = head[u]; ~i; i = gra[i].nxt) {
		int v = gra[i].to;
		if (del[i >> 1] || v == fa) continue;
		calsiz(v, u);
		siz[u] += siz[v];
	}
}

// �ҵ����������ı�
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
	int u, d; // �ڵ㣬�ڵ㵽���ľ���
	inline bool operator<(const DisData &rhs) const {
		return d < rhs.d;
	}
};

std::priority_queue<DisData> s[MAXN][2];
int cnt;

struct NodeData {
	int bel, side, dis; // �ӷ��νṹ�ı�ţ����ıߵ������Ҳ࣬�����ľ���
};
std::vector<NodeData> ndata[MAXN];
map<int, int> dis[2];
// �����������ĵ㵽���ľ���ȫ���������������ÿ���㶼������Щ����
inline void caldis(int u, int fa, int d, int t, int l) {
	if (!col[u])//����㴦����Ϣ
	{
		s[t][l].push(DisData{ u, d }); //��¼�·��νṹ�ľ�������
		ndata[u].push_back(NodeData{ t, l, d }); //��¼�ڵ��漰�ķ��νṹ�������Լ�����
	}
	for (int i = head[u]; ~i; i = gra[i].nxt) {
		int v = gra[i].to, w = gra[i].w;
		if (del[i >> 1] || v == fa) continue;
		caldis(v, u, d + w, t, l);
	}
}

int mx[MAXN], lch[MAXN], rch[MAXN];
int ctw[MAXN];//���ı߱�Ȩ
int cur_ans;
// ���ڸ���ĳ�����ıߵ���Ϣ
inline void update(int p)
{
	while (!s[p][0].empty() && col[s[p][0].top().u]) s[p][0].pop();
	while (!s[p][1].empty() && col[s[p][1].top().u]) s[p][1].pop();
	if (s[p][0].empty() || s[p][1].empty()) mx[p] = 0;
	else mx[p] = s[p][0].top().d + ctw[p] + s[p][1].top().d; // ͳ�ƾ������ıߵ�·�����ֵ
	// ͳ���ӷ��νṹ�����ֵ
	if (lch[p]) mx[p] = std::max(mx[p], mx[lch[p]]);
	if (rch[p]) mx[p] = std::max(mx[p], mx[rch[p]]);
}

// �߷ֵ�divide����
inline int divide(int u) {
	calsiz(u, 0);
	ct = -1; ctsiz = INF; sum = siz[u]; findct(u, 0);
	if (ct == -1) return 0;
	int x = gra[ct].to, y = gra[ct ^ 1].to;
	del[ct >> 1] = true; // Ҫ��ɾ���ٵݹ�
	int t = ++cnt;
	ctw[t] = gra[ct].w;

	caldis(x, 0, 0, t, 0); caldis(y, 0, 0, t, 1); //���ε��ƴ�����Ϣ
	//��̬�߷ֿ���ֱ��������д��Ϣ �漰��Ȩ��Ҫ������߱�Ȩ
	{
	}
	lch[t] = divide(x); rch[t] = divide(y);//���ı������ߵ��ƴ���
	// �ϲ���
	update(t);
	return t;
}

// ��Ϊ�׵㣬��Ҫ�������ľ�����Ϣ���������ڵ������ӷ��νṹ��
inline void setwhite(int u) {
	for (int i = ndata[u].size() - 1; i >= 0; i--) {
		NodeData d = ndata[u][i];
		s[d.bel][d.side].push(DisData{ u, d.dis });
		update(d.bel);
	}
}

// ��Ϊ�ڵ㣬��Ҫ�������ľ�����Ϣ�������ڵ������ӷ��νṹ��ɾ��
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