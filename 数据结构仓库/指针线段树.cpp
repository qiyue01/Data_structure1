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
}
void pd(node2 *p)
{
	if (!p->lazy || p->l == p->r)
		return;
	p->lazy = false;
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