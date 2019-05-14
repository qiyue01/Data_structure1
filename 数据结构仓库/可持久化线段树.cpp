#include"pch.h"
#include<string>
#include<vector>
#include<regex>
#include<algorithm>
#include<random>
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
#include<array>
#include<memory>
#include<functional>
#include<unordered_map>
using namespace std;
//#define int long long
namespace tree1 //函数式线段树 动态开点
{
	const int inf = 100000100;
	const int L_ = 1;
	const int R_ = 0x7fffffff - 1;
	const int N = 16000000;
	struct node
	{
		node *L, *R;
		int l, r, val;
		void clear()
		{
			L = R = NULL;
			l = r = val = 0;
		}
	};
	node pool[N], *cur, *null;
	void clear()
	{
		cur = pool;
		null = cur++;
		null->l = null->r = null->val = 0, null->L = null, null->R = null;
		cur->clear();
	}
	node *new_node(int L, int R, int val)
	{
		cur->l = L, cur->r = R, cur->val = val;
		cur->L = null, cur->R = null;
		return cur++;
	}
	void pl(node *a)
	{
		if ((a->L != null) || (a->l == a->r))
			return;
		int mid = (a->l + a->r) / 2;
		a->L = new_node(a->l, mid, 0);
	}
	void pr(node *a)
	{
		if ((a->R != null) || (a->l == a->r))
			return;
		int mid = (a->l + a->r) / 2;
		a->R = new_node(mid + 1, a->r, 0);
	}

	node* insert(node *old_version, int k)//单点插入 从旧版本生成新版本 权值线段树
	{
		int l = old_version->l, r = old_version->r;
		node * new_version = new_node(l, r, old_version->val + 1);
		if (l != r)
		{
			int mid = (l + r) / 2;
			if (k >= mid + 1)
			{
				pr(old_version);
				new_version->R = insert(old_version->R, k);
				new_version->L = old_version->L;
			}
			else
			{
				pl(old_version);
				new_version->L = insert(old_version->L, k);
				new_version->R = old_version->R;
			}
		}
		return new_version;
	}
	void insert_cur(node *cur_version, int k)//单点插入 插入某个版本 权值线段树
	{
		int l = cur_version->l, r = cur_version->r;
		int mid = (l + r) / 2;
		cur_version->val++;
		if (l != r)
		{
			if (k >= mid + 1)
				pr(cur_version), insert_cur(cur_version->R, k);
			else
				pl(cur_version), insert_cur(cur_version->L, k);
		}
	}
	void insert_cur_n(node *cur_version, int k, int data)//单点+ 插入某个版本 普通线段树
	{
		int l = cur_version->l, r = cur_version->r;
		int mid = (l + r) / 2;
		cur_version->val += data;
		if (l != r)
		{
			if (k >= mid + 1)
				pr(cur_version), insert_cur_n(cur_version->R, k, data);
			else
				pl(cur_version), insert_cur_n(cur_version->L, k, data);
		}
	}
	node* insert_n(node *old_version, int k, int data)//单点+ 从旧版本生成新版本 普通线段树
	{
		int l = old_version->l, r = old_version->r;
		node * new_version = new_node(l, r, old_version->val + data);
		if (l != r)
		{
			int mid = (l + r) / 2;
			if (k >= mid + 1)
			{
				pr(old_version);
				new_version->R = insert_n(old_version->R, k, data);
				new_version->L = old_version->L;
			}
			else
			{
				pl(old_version),
					new_version->L = insert_n(old_version->L, k, data);
				new_version->R = old_version->R;
			}
		}
		return new_version;
	}
	node *merge(node *x, node *y) //合并x,y,返回合并后的根
	{
		if (x == null) return y;
		if (y == null) return x;
		node *merge_version = new_node(x->l, x->r, y->val + x->val);
		merge_version->L = merge(x->L, y->L);
		merge_version->R = merge(x->R, y->R);
		return merge_version;
	}
	void update(node *x) //权值线段树清空区间后的维护
	{
		if (x->l == x->r) return;
		x->val = 0;
		x->val += x->L->val;
		x->val += x->R->val;
	}
	void merge_all(node *x, node *y) // 将x全树合并到y上
	{
		y->val += x->val;
		y->L = merge(x->L, y->L);
		y->R = merge(x->R, y->R);
	}
	int query_sum(node *x, int L, int R) //区间求和 可以直接用来代替单点查询
	{
		if (x == null) return 0;
		int sum = 0;
		int mid = (x->l + x->r) / 2;
		if (x->l == L && x->r == R)
			return x->val;
		else
		{
			if (L <= mid && R > mid)
				return query_sum(x->L, L, mid) + query_sum(x->R, mid + 1, R);
			else if (R <= mid)
				return query_sum(x->L, L, R);
			else if (L >= mid + 1)
				return query_sum(x->R, L, R);
		}
	}
	void merge(int L, int R, node *x, node *y) //将x的区间合并到y上
	{
		if (x == null) return;
		int mid = (x->l + x->r) / 2;
		if (x->l == L && x->r == R)
		{
			y->val += x->val;
			y->L = merge(x->L, y->L);
			y->R = merge(x->R, y->R);
			//x->L = null, x->R = null; //是否清空原区间 可选
		}
		else
		{
			pl(y), pr(y);
			if (L <= mid && R > mid)
				merge(L, mid, x->L, y->L), merge(mid + 1, R, x->R, y->R);
			else if (R <= mid)
				merge(L, R, x->L, y->L);
			else if (L >= mid + 1)
				merge(L, R, x->R, y->R);
		}
		update(x);
	}
	int query_rank(node *p, int L, int R, int k)
	{
		int l = L_, r = R_;
		int all_rank = 1;
		while (l != r)
		{
			int rk = 0;
			rk += p->L->val;
			int mid = (l + r) / 2;
			if (k <= mid)
			{
				r = mid;
				p = p->L;
			}
			else
			{
				all_rank += rk;
				l = mid + 1;
				p = p->R;
			}
		}
		return all_rank;
	}
	struct node2
	{
		node2 *L, *R;
		int l, r, val;
		node *tree2;
		vector<pair<int, int> > *lazy;
	};
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
	void build(int *p, node2 *x)
	{
		x->lazy = new vector<pair<int, int>>();
		x->tree2 = new_node(L_, R_, 0);
		pl(x), pr(x);
		if (x->l == x->r)
		{
			x->val = p[x->l];
			return;
		}
		else
			build(p, x->L), build(p, x->R);
		//update(x);

	}
	int se1;
	node * segment[50];
	void pd(node2 *p)
	{
		for (auto &s : *(p->lazy))
			insert_cur_n(p->tree2, s.first, s.second);
		(*p->lazy).clear();
	}
	void split(node2 *p, int l, int r)
	{
		if (p->l == l && p->r == r)
		{
			pd(p);
			segment[se1++] = (p->tree2);
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
	node *history[100010];
	int query_kth(node2 *p, int L, int R, int k)
	{
		se1 = 0;
		node *lh = history[L - 1], *rh = history[R];
		split(p, L, R);
		int l = L_, r = R_;
		while (l != r)
		{
			int rk = 0;
			rk += rh->L->val;
			rk -= lh->L->val;
			int mid = (l + r) / 2;
			for (int i = 0; i < se1; ++i)
				rk += segment[i]->L->val;
			if (rk >= k)
			{
				r = mid;
				rh = rh->L;
				lh = lh->L;
				for (int i = 0; i < se1; ++i)
					segment[i] = segment[i]->L;
			}
			else
			{
				l = mid + 1;
				k -= rk;
				rh = rh->R;
				lh = lh->R;
				for (int i = 0; i < se1; ++i)
					segment[i] = segment[i]->R;
			}
		}
		return l;
	}
	int query_rank(node2 *p, int L, int R, int k)
	{
		se1 = 0;
		node *lh = history[L - 1], *rh = history[R];
		split(p, L, R);
		int l = L_, r = R_;
		int all_rank = 1;
		while (l != r)
		{
			int rk = 0;
			rk += rh->L->val;
			rk -= lh->L->val;
			int mid = (l + r) / 2;
			for (int i = 0; i < se1; ++i)
				rk += segment[i]->L->val;
			if (k <= mid)
			{
				r = mid;
				rh = rh->L;
				lh = lh->L;
				for (int i = 0; i < se1; ++i)
					segment[i] = segment[i]->L;
			}
			else
			{
				all_rank += rk;
				l = mid + 1;
				rh = rh->R;
				lh = lh->R;
				for (int i = 0; i < se1; ++i)
					segment[i] = segment[i]->R;
			}
		}
		return all_rank;
	}
	void split2(node2 *p, int l, int pre, int k)
	{
		if (p->l != p->r)
		{
			int mid = (p->l + p->r) / 2;
			if (l <= mid)
				split2(p->L, l, pre, k);
			else
				split2(p->R, l, pre, k);
		}
		//insert_cur_n(p->tree2, pre, -1);
		//insert_cur_n(p->tree2, k, 1);
		p->lazy->push_back({ pre, -1 });
		p->lazy->push_back({ k, 1 });
	}
}
using namespace tree1;