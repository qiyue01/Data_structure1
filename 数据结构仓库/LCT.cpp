#include "pch.h"
#include<iostream>
#include<cstring>
#include<cstdio>
#include<algorithm>
#include<cmath>
#include<string>
#define ll long long 
#define inf 2000000000
#define mod 1000000007
#define N 300005
using namespace std;
namespace LCT
{
	/********
	*随便改了份板子
	*支持功能：
	*1.在线删边加边
	*2.在线判连通性
	*3.询问两点路径和
	*4.询问两点路径最值
	*5.询问两点路径xor
	*以下为洛谷p3690的AC代码
	*
	*******/
	int  top, cnt;
	int c[N][2];
	int mx[N], fa[N], v[N], tag[N];
	int q[N], a[N], b[N], last[N];
	bool rev[N];
	int s[N];
	struct edge { int to, next; }e[N << 1];
	void insert(int u, int v)
	{
		e[++cnt].to = v; e[cnt].next = last[u]; last[u] = cnt;
		e[++cnt].to = u; e[cnt].next = last[v]; last[v] = cnt;
	}
	void update(int x)
	{
		int l = c[x][0], r = c[x][1];
		s[x] = s[l] ^ s[r] ^ v[x];
		mx[x] = max(mx[l], mx[r]);
		mx[x] = max(mx[x], v[x]);
	}
	void pushdown(int x)
	{
		int l = c[x][0], r = c[x][1];
		if (rev[x])
		{
			rev[l] ^= 1; rev[r] ^= 1; rev[x] ^= 1;
			swap(c[x][0], c[x][1]);
		}
		if (tag[x])
		{
			if (l) { tag[l] += tag[x]; mx[l] += tag[x]; v[l] += tag[x]; }
			if (r) { tag[r] += tag[x]; mx[r] += tag[x]; v[r] += tag[x]; }
			tag[x] = 0;
		}
	}
	bool isroot(int x)
	{
		return c[fa[x]][0] != x && c[fa[x]][1] != x;
	}
	void rotate(int x)
	{
		int y = fa[x], z = fa[y], l, r;
		if (c[y][0] == x)l = 0; else l = 1; r = l ^ 1;
		if (!isroot(y))
		{
			if (c[z][0] == y)c[z][0] = x;
			else c[z][1] = x;
		}
		fa[x] = z; fa[y] = x; fa[c[x][r]] = y;
		c[y][l] = c[x][r]; c[x][r] = y;
		update(y); update(x);
	}
	void splay(int x)
	{
		top = 0; q[++top] = x;
		for (int i = x; !isroot(i); i = fa[i])
			q[++top] = fa[i];
		while (top)pushdown(q[top--]);
		while (!isroot(x))
		{
			int y = fa[x], z = fa[y];
			if (!isroot(y))
			{
				if (c[y][0] == x ^ c[z][0] == y)rotate(x);
				else rotate(y);
			}
			rotate(x);
		}
	}

	void access(int x)
	{
		for (int t = 0; x; t = x, x = fa[x])
			splay(x), c[x][1] = t, update(x);
	}
	void makeroot(int x)
	{
		access(x); splay(x); rev[x] ^= 1;
	}
	int find(int x)
	{
		access(x); splay(x);
		while (c[x][0])x = c[x][0];
		return x;
	}
	void link(int x, int y) 
	{
		makeroot(x);
		if (find(y) != x) fa[x] = y;
	}
	bool cut(int x, int y) //特判防止原边不连通
	{
		makeroot(x);
		if (find(y) == x && fa[x] == y && !c[x][1] && c[y][0]==x)
		{
			access(y); splay(y);
			c[y][0] = fa[c[y][0]] = 0; update(y);
			return true;
		}
		return false;
	}
	void add(int x, int y, int val)
	{
		makeroot(x); access(y); splay(y);
		tag[y] += val; mx[y] += val; v[y] += val;
	}
	void split(int x, int y) //提取路径 x-y
	{
		makeroot(x);
		access(y);
		splay(y);
	}
	void clear(int n)
	{
		for (int i = 0; i <= n; i++)
			v[i]=last[i] = tag[i] = rev[i] = fa[i] = c[i][0] = c[i][1] = 0;
		mx[0] = -inf; cnt = 0;
	}
	void init(int *a, int n) //输入原始点权初始化同时基于已有的树构建
	{
		for (int i = 1; i <= n; i++)
			mx[i] = v[i] = a[i];
		q[++top] = 1;
		for (int k = 1; k <= top; k++)
		{
			int now = q[k];
			for (int i = last[now]; i; i = e[i].next)
				if (e[i].to != fa[now])
				{
					fa[e[i].to] = now;
					q[++top] = e[i].to;
				}
		}
	}
}
using namespace LCT;
int qq[N];
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= n; ++i)
		cin >> qq[i];
	clear(n);
	init(qq, n);
	int pp;
	int x, y, w;
	while (m--)
	{
		cin >> pp >> x >> y;
		if (pp == 1)
		{
			link(x, y);
		}
		else if (pp == 2)
		{
			cut(x, y);
		}
		else if (pp == 3)
		{
			makeroot(x);
			splay(x);
			v[x] = y;
			update(x);
		}
		else
		{
			split(x, y);
			cout << s[y] << '\n';
		}
	}
	return 0;
}