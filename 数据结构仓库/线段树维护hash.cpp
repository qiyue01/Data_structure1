const int N = 100100, sigma = 11;
const long long mod = 987654321, base1 = 12;
long long pre[sigma][N], base[N];
void init()
{
	base[0] = 1;
	for (int i = 1; i < sigma; ++i)
	{
		pre[i][1] = i;
		long long k = i;
		for (int j = 2; j < N; ++j)
			pre[i][j] = (pre[i][j - 1] * base1 + k) % mod;
	}
	for (int i = 1; i < N; ++i)
		base[i] = (base[i - 1] * base1) % mod;
}
struct node2
{
	node2 *L, *R;
	int l, r, lazy_;
	bool lazy;
	long long val;
};
node2 *root;
void build_root(int L, int R)
{
	root = new node2();
	root->l = L, root->r = R;
}
void pl(node2 *a)
{
	if ((a->L != NULL) || (a->l == a->r))
		return;
	int mid = (a->l + a->r) / 2;
	a->L = new node2();
	a->L->l = a->l, a->L->r = mid, a->L->val = 0;
}
void pr(node2 *a)
{
	if ((a->R != NULL) || (a->l == a->r))
		return;
	int mid = (a->l + a->r) / 2;
	a->R = new node2();
	a->R->l = mid + 1, a->R->r = a->r, a->R->val = 0;
}
void update(node2 *p)
{
	if (p->l == p->r)
		return;

	p->val = p->L->val + p->R->val*(base[p->L->r - p->L->l + 1]) % mod;
	p->val = p->val%mod;
}
void pd(node2 *p)
{
	if (!p->lazy || p->l == p->r)
		return;
	p->lazy = false;
	p->L->lazy_ = p->lazy_;
	p->R->lazy_ = p->lazy_;
	p->L->lazy = p->R->lazy = true;
	p->L->val = pre[p->lazy_][p->L->r - p->L->l + 1];
	p->R->val = pre[p->lazy_][p->R->r - p->R->l + 1];
}
void build(int *p, node2 *x)
{
	x->lazy = false;
	pl(x), pr(x);
	if (x->l == x->r)
	{
		x->val = p[x->l];
		return;
	}
	else
		build(p, x->L), build(p, x->R);
	update(x);
}
int se1;
node2 *segment[50];
void split(node2 *p, int l, int r)
{
	pd(p);
	if (p->l == l && p->r == r)
	{
		segment[se1++] = p;
		return;
	}
	int mid = (p->l + p->r) / 2;
	if (l <= mid && r > mid)
		split(p->L, l, mid), split(p->R, mid + 1, r);
	else if (r <= mid)
		split(p->L, l, r);
	else if (l >= mid + 1)
		split(p->R, l, r);
}
void modify(node2 *p, int l, int r, int k)
{
	pd(p);
	if (p->l == l && p->r == r)
	{
		p->lazy = true;
		p->lazy_ = k;
		p->val = pre[p->lazy_][p->r - p->l + 1];
		return;
	}
	int mid = (p->l + p->r) / 2;
	if (l <= mid && r > mid)
		modify(p->L, l, mid, k), modify(p->R, mid + 1, r, k);
	else if (r <= mid)
		modify(p->L, l, r, k);
	else if (l >= mid + 1)
		modify(p->R, l, r, k);
	update(p);
}
long long gethash(int L, int R)
{
	if (L > R)
		return 0;
	se1 = 0;
	split(root, L, R);
	long long sum = 0, sum2 = 0;
	for (int i = 0; i < se1; ++i)
	{
		sum2 += (segment[i]->val*base[sum]) % mod;
		sum += segment[i]->r - segment[i]->l + 1;
		sum2 = sum2 % mod;
	}
	return sum2;
}
