#include "pch.h"
#include <iostream>
#include<vector>
#include<cstring>
#include<list>
#include<set>
#include<queue>
#include<map>
#include<stack>
#include<algorithm>
using namespace std;
const int N = 252000;
namespace tree_split1
{
	//前向星部分
	struct graph
	{
		long long Begin[N * 2], to[N * 2], Next[N * 2], e, weight[N * 2];
		void add(int u, int v)
		{
			to[++e] = v;
			Next[e] = Begin[u];
			Begin[u] = e;
		}
		void add(int u, int v, int w)
		{
			to[++e] = v;
			Next[e] = Begin[u];
			Begin[u] = e;
			weight[e] = w;
		}
		void init_graph()
		{
			e = 0;
			memset(Begin, -1, sizeof(Begin));
		}
	};
	//---------------
//树状数组部分

	class tree_array1  //区间修改 + 区间和
	{
	public:
		int n;
		long long sum1[N], sum2[N];
		long long a[N]; //原序列
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
	int id[N];
	class tree_split
	{
	public:
		long long dep[N], fa[N], top[N], size1[N], son[N], weight[N], new_weight[N];
		int len;
		graph g;
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
			memset(fa, 0, sizeof(fa));
			memset(top, 0, sizeof(top));
			memset(size1, 0, sizeof(size1));
			memset(son, 0, sizeof(son));
			memset(g.Begin, -1, sizeof(g.Begin));
			memset(weight, 0, sizeof(weight));
			len = 0;
		}
		void clear()
		{
			memset(fa, 0, sizeof(fa));
			memset(top, 0, sizeof(top));
			memset(size1, 0, sizeof(size1));
			memset(son, 0, sizeof(son));
			memset(g.Begin, -1, sizeof(g.Begin));
			memset(weight, 0, sizeof(weight));
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
			g.add(u, v);

		}
		void dfs0(int u) //单纯用来求LCA的dfs
		{
			int maxx = 0, son = 0;
			top[u] = u;
			size1[u] = 1;
			for (int i = g.Begin[u]; i != -1; i = g.Next[i])
			{
				if (g.to[i] == fa[u])
					continue;
				fa[g.to[i]] = u;
				dep[g.to[i]] = dep[u] + 1;
				dfs0(g.to[i]);
				size1[u] += size1[g.to[i]];
				if (size1[g.to[i]] > maxx)
				{
					son = g.to[i];
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
			for (int i = g.Begin[u]; i != -1; i = g.Next[i])
			{
				if (g.to[i] != father)
				{
					dfs1(g.to[i], u);
					size1[u] += size1[g.to[i]];
					if (size1[g.to[i]] > size1[son[u]])
						son[u] = g.to[i];
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
			for (int i = g.Begin[u]; i != -1; i = g.Next[i])
			{
				if (g.to[i] == fa[u] || g.to[i] == son[u]) continue;
				dfs2(g.to[i], g.to[i]);
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
	//基于树链剖分的虚树部分
	struct virtual_tree
	{
		graph g;
		tree_split ts;
		int root, top;
		int st[N];
		virtual_tree()
		{
			memset(g.Begin, -1, sizeof(g.Begin));
		}
		static bool cmp(const int &a, const int &b)
		{
			if (id[a] < id[b])
				return true;
			else
				return false;
		}
		void init(int root1)
		{
			ts.init(root1);
			root = root1;
		}
		void clear_virtual()
		{
			g.e = 0;
			for (auto &s : sum)
				g.Begin[s] = -1;
			sum.clear();
		}
		void clear_all()
		{
			g.init_graph();
			ts.clear();
		}
		void add_edge(int u, int v)
		{
			g.add(u, v);
			g.add(v, u);
		}
		void insert(int x)
		{
			if (top == 1)
			{
				st[++top] = x;
				return;
			}
			int lca = ts.LCA(x, st[top]);
			if (lca == st[top])
			{
				st[++top] = x;
				return;
			}
			while (top > 1 && id[st[top - 1]] >= id[lca])
				add_edge(st[top - 1], st[top]), top--;
			if (lca != st[top])
				add_edge(lca, st[top]), st[top] = lca;
			st[++top] = x;
		}
		void build(int *p, int sum)//输入原始序列，以及序列长度
		{
			sort(p, p + sum, cmp);
			top = 1;
			st[top] = root;
			for (int i = 0; i < sum; ++i) insert(p[i]);
			while (top > 1)
			{
				add_edge(st[top - 1], st[top]);
				top--;
			}
		}
		long long dp[N], dp2[N], pre[N];
		set<int> sum;
		void dfs(int cur)
		{
			sum.insert(cur);
			for (int i = g.Begin[cur]; i != -1; i = g.Next[i])
			{
				int k = g.to[i];
				if (id[k] > id[cur])
				{
					pre[k] = cur;
					dfs(k);
					dp[cur] += dp[k];
					dp2[cur] += dp2[k];
				}
			}
		}
	}v_t;
}
using namespace tree_split1;



