#include "pch.h"
#include<array>
#include<cmath>
#include<string>
#include<vector>
#include<algorithm>
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
#define int long long
struct Point {
	int pos[2];
	Point(int a, int b) { pos[0] = a, pos[1] = b; }
	inline int &operator [] (int x) { return pos[x]; }
	inline bool operator < (const Point &p) const { return pos[0] < p.pos[0] && pos[1] < p.pos[1]; }
	inline bool operator == (const Point &p) const { return pos[0] == p.pos[0] && pos[1] == p.pos[1]; }
	inline bool operator <= (const Point &p) const { return pos[0] <= p.pos[0] && pos[1] <= p.pos[1]; }
};
inline void updateMin(Point &a, Point &b) { a[0] = std::min(a[0], b[0]), a[1] = std::min(a[1], b[1]); }
inline void updateMax(Point &a, Point &b) { a[0] = std::max(a[0], b[0]), a[1] = std::max(a[1], b[1]); }
bool cmp;
long long count1 = 0;
struct Node {
	Node *lc, *rc;
	Point p, l, r;
	bool lazy;
	unsigned long long v, sum, size, sum2;
	Node(const Point &p, int v) : lc(0), rc(0), p(p), l(p), r(p), v(v), sum(v), size(1), sum2(0) {}
	void push_down()
	{
		if (!lazy)
			return;
		lazy = 0;
		if (lc) lc->lazy = 1, lc->sum2 += sum2, lc->v += sum2, lc->sum += lc->size*sum2;
		if (rc) rc->lazy = 1, rc->sum2 += sum2, rc->v += sum2, rc->sum += rc->size*sum2;
		sum2 = 0;
		update();
	}
	inline int querySum(Point &l, Point &r) {
		push_down();
		if (l <= this->l && this->r <= r) return this->sum;
		if (l[0] > this->r[0] || l[1] > this->r[1] || r[0] < this->l[0] || r[1] < this->l[1]) return 0;
		register int sum = 0;
		if (l <= p && p <= r) sum += v;
		if (lc) sum += lc->querySum(l, r);
		if (rc) sum += rc->querySum(l, r);
		return sum;
	}
	void add_sum(Point &l, Point &r, int k)
	{
		push_down();
		if (l <= this->l && this->r <= r)
		{
			this->sum2 += k;
			this->sum += this->size*k;
			this->v += k;
			lazy = 1;
			return;
		}
		if (l[0] > this->r[0] || l[1] > this->r[1] || r[0] < this->l[0] || r[1] < this->l[1]) return;
		if (l <= p && p <= r) v += k;
		//cout <<"aaa "<< p.pos[0] << " " << p.pos[1] <<" "<< k << endl;
		if (lc) lc->add_sum(l, r, k);
		if (rc) rc->add_sum(l, r, k);
		update();
	}
	inline void clear() { lc = rc = 0, l = r = p, sum = v, size = 1, sum2 = 0; }
	inline void *operator new(size_t) {
		static Node *s, *t;
		static size_t size = sizeof(Node) << 12;
		if (s == t) s = (Node*)malloc(size), t = s + (1 << 12);
		return s++;
	}
	inline void update() {
		l = r = p, sum = v, size = 1;
		if (lc) updateMin(l, lc->l), updateMax(r, lc->r), sum += lc->sum, size += lc->size;
		if (rc) updateMin(l, rc->l), updateMax(r, rc->r), sum += rc->sum, size += rc->size;
	}

} *pool[1200001];
inline bool less1(Node *a, Node *b) { return a->p[cmp] < b->p[cmp]; }
int top = 0;
const double A = 0.7;
inline int dis(Point &p1, Point &p2) {
	register int ret = 0;
	ret = abs(p1.pos[0] - p2.pos[0]) + abs(p1.pos[1] - p2.pos[1]);
	return ret;
}
struct KDimensionalTree {
	Node *rt;
	KDimensionalTree() : rt(0) {}
	inline void insert(Node *&x, Node *y, int h = 0, bool comp = 0) {
		if (!x) {
			x = new Node(*y);
			return;
		}
		if (x->p == y->p)
		{
			x->v += y->v, x->sum += y->v;
			return;
		}
		cmp = comp;
		if (less1(y, x)) {
			insert(x->lc, y, h + 1, !comp);
			x->update();
			if (x->lc->size > x->size * A)
				reBuild(x, comp);

		}
		else {
			insert(x->rc, y, h + 1, !comp);
			x->update();
			if (x->rc->size > x->size * A)
				reBuild(x, comp);
		}
	}
	inline void insert(const Point &p, int v) { Node it(p, v); insert(rt, &it); }
	inline void tour(Node *x) {
		if (!x) return;
		tour(x->lc), pool[++top] = x, tour(x->rc), x->clear();
	}
	inline void reBuild(Node *&x, bool comp, int lc, int rc) {
		if (rc < lc) { x = 0; return; }
		cmp = comp;
		int mid = lc + rc >> 1;
		std::nth_element(pool + lc, pool + mid, pool + rc + 1, less1);
		x = pool[mid], reBuild(x->lc, !comp, lc, mid - 1), reBuild(x->rc, !comp, mid + 1, rc), x->update();
	}
	inline void reBuild(Node *&x, bool comp) { top = 0, tour(x), reBuild(x, comp, 1, top); }
	inline int query(int x1, int y1, int x2, int y2) {
		if (!rt) return 0;
		Point l(x1, y1);
		Point r(x2, y2);
		return rt->querySum(l, r);
	}
	int ans;
	/*
	int Get(Node *k, Point &p)
	{
		int tmp = 0;
		tmp += max(0, k->l.pos[0] - p.pos[0]);
		tmp += max(0, k->l.pos[1] - p.pos[1]);
		tmp += max(0, p.pos[0] - k->r.pos[0]);
		tmp += max(0, p.pos[1] - k->r.pos[1]);
		return tmp;
	}
	void query2(Node *p, Point &f)
	{
		int d, dl = 100000000, dr = 100000000;
		d = dis(p->p, f);
		ans = min(ans, d);
		if (p->lc)dl = Get(p->lc, f);
		if (p->rc)dr = Get(p->rc, f);
		if (dl < dr)
		{
			if (dl < ans) query2(p->lc, f);
			if (dr < ans) query2(p->rc, f);
		}
		else
		{
			if (dr < ans) query2(p->rc, f);
			if (dl < ans) query2(p->lc, f);
		}
	}
	*/
	void build(vector<tuple<int, int, int> > &k)
	{
		for (auto &p : k)
		{
			pool[++top] = new Node(Point(get<0>(p), get<1>(p)), get<2>(p));
			//cout << get<0>(p) << " " << get<1>(p) << endl;
		}
		reBuild(rt, 0, 1, top);
	}
} tr;
array<vector<int>, 310000> edge;
array<int, 310000> B, E, deep, id, ans2;
array<bool, 310000> vis;
vector<tuple<int, int, int> > PP;
int len = 0;
void dfs(int a)
{
	vis[a] = true;
	B[a] = ++len;
	id[len] = a;

	for (auto &p : edge[a])
		if (!vis[p])
			deep[p] = deep[a] + 1, dfs(p);
	E[a] = len;
	PP[a - 1] = make_tuple(B[a], deep[a], 0);
}
int32_t main()
{
	std::ios::sync_with_stdio(false);
	std::cin.tie(NULL);
	std::cout.tie(NULL);
	int n, m, op, x, y, u, v, q;
	vis.fill(false);
	deep[1] = 0;
	cin >> n >> q;
	PP.resize(n);
	for (int i = 1; i < n; ++i)
	{
		cin >> u >> v;
		edge[u].push_back(v);
		edge[v].push_back(u);
	}
	dfs(1);
	tr.build(PP);
	while (q--)
	{
		cin >> op;
		if (op == 1)
		{
			cin >> x >> y;
			Point l(0, x), r(1000000, x);
			tr.rt->add_sum(l, r, y);
		}
		else
		{
			cin >> x;
			cout << (tr.query(B[x], 0, E[x], 1000000)) << '\n';
		}
	}
	return 0;
}