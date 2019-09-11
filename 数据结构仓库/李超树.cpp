struct seg
{
	double a, b;
	double operator()(double aa)
	{
		return aa * a + b;
	}
	seg()
	{
		a = 0, b = -1e9;
	}
};
struct node2
{
	node2 *L, *R;
	int l, r;
	seg ss;
	double Min, Max;
	long long val;
};
node2 *root;

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
void build(node2 *x)
{
	pl(x), pr(x);
	if (x->l == x->r)
		return;
	else
		build(x->L), build(x->R);
}
void build_root(int L, int R)
{
	root = new node2();
	root->l = L, root->r = R;
	build(root);
}
int se1;
node2 *segment[50];
double MAXF;
void split(node2 *p, int l)
{
	MAXF = max(p->ss(l), MAXF);
	if (p->l == p->r)
		return;
	int mid = (p->l + p->r) / 2;
	if (l <= mid)
		split(p->L, l);
	else if (l >= mid + 1)
		split(p->R, l);
}
double Fx(int k)
{
	MAXF = 0;
	split(root, k);
	return MAXF;
}
void modify(node2 *p, seg &s)
{
	if (p->l == p->r)
	{
		if (s(p->l) > p->Max)
			p->ss = s, p->Max = p->Min = s(p->l);
		return;
	}
	int mid = (p->l + p->r) / 2;
	double s1 = s(p->l), s2 = s(p->r);
	double k1 = min(s1, s2), k2 = max(s1, s2);
	if (s1 > p->Max)
	{
		p->ss = s;
		p->Max = k2;
		p->Min = k1;
		return;
	}
	else if (k1 < p->Max && k2>p->Min)
		modify(p->L, s), modify(p->R, s);
}