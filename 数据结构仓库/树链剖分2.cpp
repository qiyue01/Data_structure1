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
	//ǰ���ǲ���
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
//��״���鲿��
	long long sum1[N], sum2[N];
	long a[N]; //ԭ����
	class tree_array1  //�����޸� + �����
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
		void add(int p, long x) {
			for (int i = p; i <= n; i += i & -i)
			{
				sum1[i] += x;
				sum2[i] += x * p;
			}
		}
		void range_add(int l, int r, long x) {
			add(l, x);
			add(r + 1, -x);
		}
		long ask(int p) {
			long res = 0;
			for (int i = p; i != 0; i -= i & -i)
				res += (p + 1) * sum1[i] - sum2[i];
			return res;
		}
		long range_ask(int l, int r) {
			return ask(r) - ask(l - 1) + a[r] - a[l - 1];
		}
	};
	//------------------------
	//�����ʷֲ���
int dep[N], fa[N], top[N], size1[N], son[N], id[N], weight[N], new_weight[N];
int len;
class tree_split
{
public:
	void query(tree_array1 &p, int x, int y, int k) //�����ֵ query���忴��Ŀд ����ԭ�����
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
	long query2(tree_array1 &p, int x, int y) //����ѯ�� query���忴��Ŀд ����ԭ�����
	{
		long ans = 0;
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
	void dfs0(int u) //����������LCA��dfs
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
	void dfs1(int u, int father)//һ�Ͷ������������ʷֵ�Ԥ����
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
int main()
{

	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int n, m, p, r, u, v, q, q1, q2, q3,in;
	cin >> n >> m >> r >> p;
	tree_split part;
	for (int i = 1; i <= n; ++i)
	{
		cin >> in;
		part.add_weight(i, in);
	}
	init_graph();
	for (int i = 1; i < n; ++i)
	{
		cin >> u >> v;
		part.add_edge(u, v);
		part.add_edge(v, u);
	}
	tree_array1 part1(n);
	part.init(r);
	for (int i = 0; i <= n; ++i)
		a[i] = new_weight[i];
	part1.init();
	for (int i = 0; i < m; ++i)
	{
		cin >> q;
		if (q == 1)
		{
			cin >> q1 >> q2 >> q3;
			part.query(part1, q1, q2, q3);
		}
		else if (q == 2)
		{
			cin >> q1 >> q2;
			cout<<part.query2(part1, q1, q2) % p<<'\n';
		}
		else if (q == 3)
		{
			cin >> q1 >> q2;
			part1.range_add(id[q1],id[q1] + size1[q1] - 1, q2);
		}
		else
		{
			cin >> q1;
			cout<<part1.range_ask(id[q1],id[q1]+size1[q1] - 1) % p<<'\n';
		}
	}
	return 0;
}


