
#include<string>
#include<set>
#include<vector>
#include<array>
#include<cmath>
#include<algorithm>
#include<cstring>
#include<cstdio>
#include<deque>
#include<iomanip>
#include<sstream>
#include<stack>
#include<iostream>
#include<limits.h>
#include<bitset>
#include<list>
#include<queue>
#include<map>
#include<memory>
#include<functional>
#include<unordered_map>
using namespace std;
struct ioss {
#define endl '\n'
	static const int LEN = 500000;
	char obuf[LEN], *oh = obuf;
	bool eof = false;
	inline char gc() {

		static char buf[LEN], *s, *t, buf2[LEN];
		return (s == t) && (t = (s = buf) + fread(buf, 1, LEN, stdin)), s == t ? -1 : *s++;
	}
	inline ioss & operator >> (long long&x) {
		static char ch, sgn, *p;
		ch = gc(), sgn = 0;
		for (; !isdigit(ch); ch = gc()) { if (ch == -1)return *this; sgn |= ch == '-'; }
		for (x = 0; isdigit(ch); ch = gc())x = x * 10 + (ch^'0');
		sgn && (x = -x); return *this;
	}
	inline ioss & operator >> (int &x) {
		static char ch, sgn, *p;
		ch = gc(), sgn = 0;
		for (; !isdigit(ch); ch = gc())
		{
			if (ch == -1)
			{
				eof = true;
				return *this;
			}
			sgn |= ch == '-';
		}
		for (x = 0; isdigit(ch); ch = gc())x = x * 10 + (ch^'0');
		sgn && (x = -x); return *this;
	}
	inline ioss & operator >> (char&x)
	{
		static char ch;
		for (; !isalpha(ch) && !isdigit(ch); ch = gc())
			if (ch == -1)return *this;
		for (; isalpha(ch) || isdigit(ch); ch = gc())
			x = ch;
		return *this;
	}
	inline ioss & operator >> (string &x)
	{
		static char ch, *p, buf2[LEN];
		for (; !isalpha(ch) && !isdigit(ch); ch = gc())
			if (ch == -1)
			{
				eof = true;
				return *this;
			}
		p = buf2;
		for (; isalpha(ch) || isdigit(ch); ch = gc())
			*p = ch, p++;
		*p = '\0';
		x = buf2;
		return *this;
	}
	inline ioss & operator <<(string &c)
	{
		for (auto &p : c)
			this->operator<<(p);
		return *this;
	}
	inline ioss & operator <<(const char *c)
	{
		while (*c != '\0')
		{
			this->operator<<(*c);
			c++;
		}
		return *this;
	}
	inline ioss & operator <<(const char &c) {
		oh == obuf + LEN ? (fwrite(obuf, 1, LEN, stdout), oh = obuf) : 0;
		*oh++ = c;
		return *this;
	}
	inline ioss & operator <<(int x) {
		static int buf[30], cnt;
		if (x < 0)
			this->operator<<('-'), x = -x;
		if (x == 0)
			this->operator<<('0');
		for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
		while (cnt) this->operator<<((char)buf[cnt--]);
		return *this;
	}
	inline ioss & operator <<(long long x) {
		static int buf[30], cnt;
		if (x < 0)
			this->operator<<('-'), x = -x;
		if (x == 0)
			this->operator<<('0');
		for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 | 48;
		while (cnt) this->operator<<((char)buf[cnt--]);
		return *this;
	}
	~ioss()
	{
		fwrite(obuf, 1, oh - obuf, stdout);
	}
} io;
const int MAXN = 200010;
const int INIT_VAL = 1001;
const int INF = 2000000000;
struct Node *null;

struct Node {
	Node *lc, *rc;
	int  rank, tag, size;
	long long times, sum, t2, key;
	Node(int key = 0) : key(key), rank(rand()), lc(null), rc(null), tag(INIT_VAL),
		size(1) {
		t2 = times = 0, sum = 0;
	}
	inline void maintain()
	{
		size = lc->size + rc->size + 1;
		sum = lc->sum + rc->sum + times * key;
		t2 = times + lc->t2 + rc->t2;
	}
	inline void pushDown() {
		if (this == null) return;
		if (tag != INIT_VAL) {
			tag = INIT_VAL;
		}
	}
} Data[MAXN], *pool[MAXN], *rt;

int poolTop, dataTop;

typedef std::pair<Node *, Node *> Pair;

inline Node *newNode(int key) {
	Node *u;
	if (poolTop) u = pool[poolTop--];
	else u = &Data[dataTop++];
	*u = Node(key);
	return u;
}

inline void remove(Node *u) {
	pool[++poolTop] = u;
}

inline Node *build(int *a, int n) {
	static Node *stack[MAXN], *u, *pre;
	int top = 0;
	for (register int i = 1; i <= n; i++) {
		u = newNode(a[i]);
		pre = null;
		while (top && stack[top]->rank > u->rank) {
			stack[top]->maintain();
			pre = stack[top];
			stack[top--] = null;
		}
		if (top) stack[top]->rc = u;
		u->lc = pre;
		stack[++top] = u;
	}
	while (top) stack[top--]->maintain();
	return stack[1];
}

inline Node *merge(Node *u, Node *v) {
	if (u == null) return v;
	if (v == null) return u;
	if (u->rank < v->rank) {
		u->pushDown();
		u->rc = merge(u->rc, v);
		u->maintain();
		return u;
	}
	else {
		v->pushDown();
		v->lc = merge(u, v->lc);
		v->maintain();
		return v;
	}
}
inline Pair split(Node *u, int k) {
	if (u == null) return Pair(null, null);
	Pair t;
	u->pushDown();
	if (u->lc->size >= k) {
		t = split(u->lc, k);
		u->lc = t.second, t.second = u;
	}
	else {
		t = split(u->rc, k - u->lc->size - 1);
		u->rc = t.first, t.first = u;
	}
	u->maintain();
	return t;
}
inline void recycle(Node *u) {
	if (u == null) return;
	recycle(u->lc), recycle(u->rc);
	remove(u);
}
int  m, a[MAXN];//初始化数组
void init() {

	null = newNode(-INF);
	null->size = 0;
	rt = build(a, 210);
}
void dfs_init(Node *p)
{
	if (p != null)
	{
		dfs_init(p->lc);
		p->t2 = p->times = p->sum = 0;
		dfs_init(p->rc);
	}
}
long long allll = 0;
void add(Node *p, int t, int k)
{
	if (p->key > k)
		add(p->lc, t, k);
	else if (p->key < k)
		add(p->rc, t, k);
	else if (p->key == k)
		p->times += t;
	p->maintain();
}
long long query(Node *p, long long kth)
{
	if (p->lc->t2 > kth)
		return query(p->lc, kth);
	else
	{
		kth -= p->lc->t2;
		if (kth <= p->times)
		{
			return kth * p->key + p->lc->sum;
		}
		else
		{
			kth -= p->times;
			return p->times*p->key + p->lc->sum + query(p->rc, kth);
		}
	}
}
long long he[110000], co[110000], nu[110000];
long long t1[110000], t2[110000];
vector<pair<int, int>> trans[110000];
int main()
{
	return 0;
}