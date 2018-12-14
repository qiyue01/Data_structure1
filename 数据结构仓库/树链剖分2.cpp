#include "pch.h"
#include <iostream>
#include<vector>
#include<cstring>
#include<list>
#include<queue>
#include<stack>
#include<algorithm>
using namespace std;
const int MAXN = (255555 << 2) + 10;
int siz[MAXN];//number of son
int top[MAXN];//top of the heavy link
int son[MAXN];//heavy son of the node
int dep[MAXN];//depth of the node
int faz[MAXN];//father of the node
int tid[MAXN];//ID -> DFSID
int rnk[MAXN];//DFSID -> ID
list<int> edge[MAXN];
int cnt=1;
void add(int u, int v)
{
	edge[u].push_back(v);
}
void dfs1(int u, int father, int depth)
{
	dep[u] = depth;
	faz[u] = father;
	siz[u] = 1;
	for (auto &s : edge[u])
		// ������ӵĽ���ǵ�ǰ���ĸ��׽�㣬�򲻴���
		if (s != faz[u])
		{
			dfs1(s, u, depth + 1);
			// ������ʱ�򽫵�ǰ����siz�����ӽ���siz
			siz[u] += siz[s];
			// ���û�����ù��ؽ��son�����ӽ��v��siz����֮ǰ��¼���ؽ��son������и���
			if (son[u] == -1 || siz[s] > siz[son[u]])
				son[u] = s;
		}
}
void dfs2(int u, int t) 
{
	top[u] = t; 
	tid[u] = cnt; 
	rnk[cnt] = u; 
	cnt++;
	if (son[u] == -1)
		return;
	dfs2(son[u], t);
	for(auto &s:edge[u])
		if (s != son[u] && s != faz[u]) 
			dfs2(s, s);
}
int Lca(int x, int y) {
	while (top[x] != top[y])
	{
		if (dep[top[x]] > dep[top[y]]) x = faz[top[x]]; else y = faz[top[y]];
	}
	if (dep[x] > dep[y])
		return y;
	return x;
}
int main()
{
	memset(son, -1, sizeof(son));
	ios::sync_with_stdio(false);
	int a, b, c;
	cin >> a >> b >> c;
	int a1, a2;
	for (int i = 0; i < a - 1; ++i)
	{
		cin >> a1;
		cin >> a2;
		add(a1, a2);
		add(a2, a1);
	}
	dfs1(c,c,1);
	dfs2(c,c);
	for (int i = 0; i < b; ++i)
	{
		cin >> a1 >> a2;
		cout << Lca(a1, a2) << endl;
	}
}

