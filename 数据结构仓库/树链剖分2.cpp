#include "pch.h"
#include <iostream>
#include<vector>
#include<cstring>
#include<list>
#include<queue>
#include<stack>
#include<algorithm>
using namespace std;
const int N = 102000;
namespace tree_split1
{
	//前向星部分
	int Begin[N*2], to[N*2], Next[N*2], e;
	void add(int u, int v)
	{
		to[++e] = v;
		Next[e] = Begin[u];
		Begin[u] = e;
	}
	void init_graph()
	{
		e = 0;
		memset(Begin, -1,sizeof(Begin));
	}
	//---------------
//树状数组部分
	long long sum1[N], sum2[N];
	long long a[N]; //原序列
	class tree_array1  //区间修改 + 区间和
	{
	public:
		int n;
		tree_array1(int n1)
		{
			n = n1;
		}
		void init()
		{
			for (int i = 1; i <= n; ++i)
				a[i] += a[i - 1];
			memset(sum1, 0, sizeof(sum1));
			memset(sum2, 0, sizeof(sum2));
		}
		void add(int p, long long x) {
			for (int i = p; i <= n; i += i & -i)
			{
				sum1[i] += x;
				sum2[i] += x * p;
			}
		}
		void range_add(int l, int r, long long x) {
			add(l, x);
			add(r + 1, -x);
		}
		long long ask(int p) {
			long long res = 0;
			for (int i = p; i != 0; i -= i & -i)
				res += (p + 1) * sum1[i] - sum2[i];
			return res;
		}
		long long range_ask(int l, int r) {
			return ask(r) - ask(l - 1) + a[r] - a[l - 1];
		}
	};
	//------------------------
	//树链剖分部分
int dep[N], fa[N], top[N], size1[N], son[N], id[N], weight[N], new_weight[N];
int len;
class tree_split
{
public:
	void query(tree_array1 &p, int x, int y, int k) //区间加值 query具体看题目写 输入原树编号
	{
		while (top[x] != top[y])
		{
			if (dep[top[x]] < dep[top[y]]) swap(x, y);
			p.range_add(id[top[x]], id[x], k);
			x = fa[top[x]];

		}
		if (dep[x] > dep[y]) swap(x, y);
		p.range_add(id[x], id[y], k);
	}
	long long query2(tree_array1 &p, int x, int y) //区间询问 query具体看题目写 输入原树编号
	{
		long long ans = 0;
		while (top[x] != top[y])
		{
			if (dep[top[x]] < dep[top[y]]) swap(x, y);
			ans += p.range_ask(id[top[x]], id[x]);
			x = fa[top[x]];
		}
		if (dep[x] > dep[y]) swap(x, y);
		ans += p.range_ask(id[x], id[y]);
		return ans;
	}
	tree_split()
	{
		memset(fa, 0,sizeof(fa));
		memset(top, 0, sizeof(fa));
		memset(size1, 0, sizeof(fa));
		memset(son, 0,sizeof(fa));
		len = 0;
	}
	void init(int root)
	{
		dep[root] = 0;
		dfs1(root, root);
		dfs2(root, root);
	}
	void add_weight(int i, int w)
	{
		weight[i] = w;
	}
	void add_edge(int u, int v)
	{
		add(u, v);
	}
	void dfs0(int u) //单纯用来求LCA的dfs
	{
		int maxx = 0, son = 0;
		top[u] = u;
		size1[u] = 1;
		for (int i = Begin[u]; i != -1; i = Next[i])
		{
			if (to[i] == fa[u])
				continue;
			fa[to[i]] = u;
			dep[to[i]] = dep[u] + 1;
			dfs0(to[i]);
			size1[u] += size1[to[i]];
			if (size1[to[i]] > maxx)
			{
				son = to[i];
				maxx = size1[son];
			}
		}
		if (son != 0)
			top[son] = u;
	}
	void dfs1(int u, int father)//一和二是完整树链剖分的预处理
	{
		dep[u] = dep[father] + 1;
		fa[u] = father;
		size1[u] = 1;
		for (int i = Begin[u]; i != -1; i =Next[i])
		{
			if (to[i] != father)
			{
				dfs1(to[i], u);
				size1[u] += size1[to[i]];
				if (size1[to[i]] > size1[son[u]])
					son[u] = to[i];
			}
		}
	}
	void dfs2(int u, int topf)
	{
		id[u] = ++len;
		new_weight[len] = weight[u];
		top[u] = topf;
		if (son[u] == 0)return;
		dfs2(son[u], topf);
		for (int i = Begin[u]; i != -1; i = Next[i])
		{
			if (to[i] == fa[u] || to[i] == son[u]) continue;
			dfs2(to[i], to[i]);
		}
	}
	int find(int u)
	{
		return top[u] = top[u] == u ? u : find(top[u]);
	}
	int LCA(int u, int v) {
		if (find(u) != find(v))
			return dep[top[u]] > dep[top[v]] ? LCA(fa[top[u]], v) : LCA(u, fa[top[v]]);
		else return dep[u] > dep[v] ? v : u;
	}
};

}
using namespace tree_split1;



