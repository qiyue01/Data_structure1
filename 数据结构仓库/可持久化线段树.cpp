#include"pch.h"
#include<iostream>
#include<algorithm>
#include<map>
using namespace std;

namespace tree //动态开点
{
	const int N = 4000000;
	const int Q = 410000;
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
	node *history[Q];
	void build(int *arr, int n)//建权值线段树,输入离散化后的序列【1-n】,或者值域开够也行，因为已经动态开点了，不过对1e8以上的数据离散化后内存用的少很多
	{
		history[0] = new_node(1, n, 0);
		for (int i = 1; i <= n; ++i)
			history[i] = insert(history[i - 1], arr[i]);
	}
	int quer_kth(int l, int r, int k)//查询k大
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