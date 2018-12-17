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
		// 如果连接的结点是当前结点的父亲结点，则不处理
		if (s != faz[u])
		{
			dfs1(s, u, depth + 1);
			// 收敛的时候将当前结点的siz加上子结点的siz
			siz[u] += siz[s];
			// 如果没有设置过重结点son或者子结点v的siz大于之前记录的重结点son，则进行更新
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
	
}

