#include"pch.h"
#include<iostream>
#include<algorithm>
#include<map>
namespace tree //动态开点
{
	const int N = 14000000;
	const int Q = 4;
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
	node pool[N], *cur;
	void clear()
	{
		cur = pool;
		cur->clear();
	}
	node *new_node(int L, int R, int val)
	{
		cur->l = L, cur->r = R, cur->val = val;
		return cur++;
	}
	void push_down(node *a) //创建合适的子节点
	{
		if ((a->L != NULL && a->R != NULL) || (a->l == a->r))
			return;
		int mid = (a->l + a->r) / 2;
		a->L = new_node(a->l, mid, 0);
		a->R = new_node(mid + 1, a->r, 0);
	}
	void update(node *x)
	{
		x->val = 0;
		if (x->L != NULL) x->val += x->L->val;
		if (x->R != NULL) x->val += x->R->val;
	}
	node* insert(node *old_version, int k)//单点插入
	{
		push_down(old_version);
		int l = old_version->l, r = old_version->r;
		node * new_version = new_node(l, r, old_version->val + 1);
		if (l != r)
		{
			int mid = (l + r) / 2;
			if (k >= mid + 1)
			{
				new_version->R = insert(old_version->R, k);
				new_version->L = old_version->L;
			}
			else
			{
				new_version->L = insert(old_version->L, k);
				new_version->R = old_version->R;
			}
		}
		return new_version;
	}
	void insert_cur(node *cur_version, int k)
	{
		push_down(cur_version);
		int l = cur_version->l, r = cur_version->r;
		int mid = (l + r) / 2;
		cur_version->val++;
		if (l != r)
		{
			if (k >= mid + 1)
				insert_cur(cur_version->R, k);
			else
				insert_cur(cur_version->L, k);
		}
	}
	node *history[Q];
	node *merge(node *x, node *y) //合并x,y,返回合并后的根
	{
		if (x == NULL) return y;
		if (y == NULL) return x;
		node *merge_version = new_node(x->l, x->r, y->val + x->val);
		merge_version->L = merge(x->L, y->L);
		merge_version->R = merge(x->R, y->R);
		return merge_version;
	}
	void merge(int L, int R, node *x, node *y)
	{
		if (x == NULL) return;
		int mid = (x->l + x->r) / 2;
		if (x->l == L && x->r == R)
		{
			y->val += x->val;
			y->L = merge(x->L, y->L);
			y->R = merge(x->R, y->R);
			x->L = NULL, x->R = NULL;
		}
		else
		{
			push_down(y);
			if (L <= mid && R > mid)
				merge(L, mid, x->L, y->L), merge(mid + 1, R, x->R, y->R);
			else if (R <= mid)
				merge(L, R, x->L, y->L);
			else if (L >= mid + 1)
				merge(L, R, x->R, y->R);
		}
		update(x);
	}
	void build(int *arr, int n)//建权值线段树,输入离散化后的序列【1-n】,或者值域开够也行，因为已经动态开点了，不过对1e8以上的数据离散化后内存用的少很多
	{
		history[0] = new_node(1, n, 0);
		for (int i = 1; i <= n; ++i)
			history[i] = insert(history[i - 1], arr[i]);
	}

	int query(node *x, int k) //单点查询
	{
		if (x == NULL || x->val == 0)
			return 0;
		if (x->l == x->r)
			return x->val;
		int mid = (x->l + x->r) / 2;
		if (k >= mid + 1)
			return query(x->R, k);
		else
			return query(x->L, k);
	}
	int query_kth(int l, int r, int k)//查询k大
	{
		node * LH = history[l - 1], *RH = history[r];//左右两边的历史版本
		while (RH->r != RH->l)
		{

			int p = RH->L->val - LH->L->val;
			if (k > p)
			{
				k -= p;
				RH = RH->R;
				LH = LH->R;
			}
			else
			{
				RH = RH->L;
				LH = LH->L;
			}

		}
		return RH->r;
	}
}

using namespace tree;