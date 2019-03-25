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
	for (int k = 0; k < max2; k++)  //Ô¤´¦Àífather
		for (int i = 1; i <= n; i++)
			if (father[k][i])
				father[k + 1][i] = father[k][father[k][i]];
}