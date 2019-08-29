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