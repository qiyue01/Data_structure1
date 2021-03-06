// 数据结构仓库.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<vector>
#include<cstring>
#include<list>
using namespace std;
const int maxn = 110000;
const int max2=20;
/*
int dis[maxn];
list<int> part[maxn];
int father[max2][maxn];
int depth[maxn];
void init(int n)
{
	memset(father, 0, sizeof(father));
	memset(depth, 0, sizeof(depth));
	for (int i = 1; i < n; ++i)
		part[i].clear();
}
void addedge(int u,int v)
{
	part[u].push_back(v);
	part[v].push_back(u);
}
void dfs(int x)
{
	for (auto &s : part[x])
	{
		if (father[0][x] != s)
		{
			father[0][s] = x;
			depth[s] = depth[x] + 1;
			dfs(s);
		}
	}
}
void  build(int root,int n)
{
	depth[root] = 1;
	dfs(root);
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
int main()
{
	ios::sync_with_stdio(false);
	int N,M,S;
	cin >> N >> M >> S;
	int u, v;
	init(S);
	for (int i = 0; i < N-1; ++i)
	{
		cin >> u >> v;
		addedge(u, v);
		addedge(v, u);
	}
	build(S, N);
	for (int i = 0; i < M; ++i)
	{
		cin >> u >> v;
		cout << LCA(u, v) << endl;
	}
	return 0;
}*/

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
