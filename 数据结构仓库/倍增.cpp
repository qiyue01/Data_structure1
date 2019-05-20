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
const int maxn = 230000;
const int max2 = 22;
vector<int> part_[maxn];
int father[max2][maxn];
int depth[maxn];
void init(int n)
{
	memset(father, 0, sizeof(father));
	memset(depth, 0, sizeof(depth));
	for (int i = 1; i < n; ++i)
		part_[i].clear();
}
void addedge(int u, int v)
{
	part_[u].push_back(v);
	part_[v].push_back(u);
}
void dfs2(int x)
{
	for (auto &s : part_[x])
	{
		if (father[0][x] != s)
		{
			father[0][s] = x;
			depth[s] = depth[x] + 1;
			dfs2(s);
		}
	}
}
void  build(int root, int n)
{
	depth[root] = 1;
	dfs2(root);
	for (int k = 0; k < max2; k++)  //预处理father
		for (int i = 1; i <= n; i++)
			if (father[k][i])
				father[k + 1][i] = father[k][father[k][i]];
}
int LCA(int u, int v)
{
	if (depth[u] > depth[v])
		return LCA(v, u);
	if (depth[u] != depth[v]) {   //先将v上提，使两个结点在同一深度上
		for (int k = 20 - 1; k >= 0; k--) {
			if (depth[v] - (1 << k) >= depth[u]) //1<<k即2^k  v上提后不能在u的上方 
				v = father[k][v];
		}
	}
	if (u == v) return u;  //如果在同一结点上，直接得LCA 
	for (int k = max2 - 1; k >= 0; k--) {  //二分法一点点缩小范围 
		if (father[k][u] == 0) continue;  //不能越界
		if (father[k][u] != father[k][v]) {  //如果fa不同，将两点都上提 
			u = father[k][u];
			v = father[k][v];
		}
	}
	return father[0][u];
}
