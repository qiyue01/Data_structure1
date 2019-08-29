const int MAXN = 500010;
const int INIT_VAL = 1001;
const int INF = 2000000000;
struct Node *null;

struct Node {
	Node *lc, *rc;
	int key, rank, rev, tag, size;
	int sum, lSum, rSum, maxSum;

	Node(int key = 0) : key(key), rank(rand()), lc(null), rc(null), rev(0), tag(INIT_VAL),
		size(1) {
		sum = lSum = rSum = maxSum = key;
	}

	inline void maintain() {
		size = lc->size + rc->size + 1;
		sum = lc->sum + rc->sum + key;
		lSum = std::max(lc->lSum, lc->sum + key + std::max(0, rc->lSum));
		rSum = std::max(rc->rSum, rc->sum + key + std::max(0, lc->rSum));
		maxSum = std::max(0, lc->rSum) + key + std::max(0, rc->lSum);
		maxSum = std::max(maxSum, std::max(lc->maxSum, rc->maxSum)); //维护最大子段和
	}

	inline void cover(int v) {
		if (this == null) return;
		key = v, sum = v * size;
		lSum = rSum = maxSum = std::max(sum, v);
		tag = v;
	}

	inline void reverse() {
		if (this == null) return;
		std::swap(lc, rc);
		std::swap(lSum, rSum);
		rev ^= 1;
	}

	inline void pushDown() {
		if (this == null) return;
		if (tag != INIT_VAL) {
			lc->cover(tag), rc->cover(tag);
			tag = INIT_VAL;
		}
		if (rev) {
			lc->reverse(), rc->reverse();
			rev = 0;
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


int pos, cnt;

inline void insert(int *a) //区间插入
{
	Node *nr = build(a, cnt);
	Pair t = split(rt, pos);
	rt = merge(merge(t.first, nr), t.second);
}

inline void remove() //区间删除
{
	Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
	recycle(t2.second);
	rt = merge(t2.first, t1.second);
}

inline void reverse() //区间翻转
{
	Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
	t2.second->reverse();
	rt = merge(merge(t2.first, t2.second), t1.second);
}

inline void makeSame(int v) //范围修改
{
	Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
	t2.second->cover(v);
	rt = merge(merge(t2.first, t2.second), t1.second);
}

inline long long querySum() //区间求和
{
	Pair t1 = split(rt, pos + cnt - 1), t2 = split(t1.first, pos - 1);
	long long ans = t2.second->sum;
	rt = merge(merge(t2.first, t2.second), t1.second);
	return ans;
}
int n, m, a[MAXN];//初始化数组
void init() {

	null = newNode(-INF);
	null->size = null->sum = 0;
	rt = build(a, n);
}