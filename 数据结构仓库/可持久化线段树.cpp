#include"pch.h"
#include<string>
#include<cstring>
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
#include<memory>
#include<functional>
#include<cstdio>
using namespace std;

namespace tree //函数式线段树 动态开点
{
	/*********
	* 我们以每个node 节点为线段树的根 基本所有操作都需要有相同的值域
	* 在权值线段树 与 普通线段树中 每个节点val贮存的信息都不同
	* 这个线段树合并也进行了持久化 旧的信息依旧可以访问
	* N指定了内存池的大小 使用前先clear初始化内存池
* 洛谷p3919的可持久化数组的示范
	**********/
	const int N = 16000000;
	const int Q = 4000;
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
	void update(node *x) //权值线段树清空区间后的维护
	{
		x->val = 0;
		if (x->L != NULL) x->val += x->L->val;
		if (x->R != NULL) x->val += x->R->val;
	}
	node* insert(node *old_version, int k)//单点插入 从旧版本生成新版本 权值线段树
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
	void insert_cur(node *cur_version, int k)//单点插入 插入某个版本 权值线段树
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
	void insert_cur_n(node *cur_version, int k, int data)//单点+ 插入某个版本 普通线段树 
	{
		push_down(cur_version);
		int l = cur_version->l, r = cur_version->r;
		int mid = (l + r) / 2;
		cur_version->val += data;
		if (l != r)
		{
			if (k >= mid + 1)
				insert_cur_n(cur_version->R, k, data);
			else
				insert_cur_n(cur_version->L, k, data);
		}
	}
	node* insert_n(node *old_version, int k, int data)//单点+ 从旧版本生成新版本 普通线段树 
	{
		push_down(old_version);
		int l = old_version->l, r = old_version->r;
		node * new_version = new_node(l, r, old_version->val + data);
		if (l != r)
		{
			int mid = (l + r) / 2;
			if (k >= mid + 1)
			{
				new_version->R = insert_n(old_version->R, k, data);
				new_version->L = old_version->L;
			}
			else
			{
				new_version->L = insert_n(old_version->L, k, data);
				new_version->R = old_version->R;
			}
		}
		return new_version;
	}
	node *merge(node *x, node *y) //合并x,y,返回合并后的根
	{
		if (x == NULL) return y;
		if (y == NULL) return x;
		node *merge_version = new_node(x->l, x->r, y->val + x->val);
		merge_version->L = merge(x->L, y->L);
		merge_version->R = merge(x->R, y->R);
		return merge_version;
	}
	void merge_all(node *x, node *y) // 将x全树合并到y上
	{
		y->val += x->val;
		y->L = merge(x->L, y->L);
		y->R = merge(x->R, y->R);
	}
	int query_sum(node *x, int L, int R) //区间求和 可以直接用来代替单点查询
	{
		if (x == NULL) return 0;
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
		if (x == NULL) return;
		int mid = (x->l + x->r) / 2;
		if (x->l == L && x->r == R)
		{
			y->val += x->val;
			y->L = merge(x->L, y->L);
			y->R = merge(x->R, y->R);
			//x->L = NULL, x->R = NULL; //是否清空原区间 可选
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
	/*
	* 查询区间k大的示例
	*/
	node *history[Q];
	void build(int *arr, int n)//建权值线段树,输入离散化后的序列【1-n】,或者值域开够也行，因为已经动态开点了，不过对1e8以上的数据离散化后内存用的少很多
	{
		history[0] = new_node(1, n, 0);
		for (int i = 1; i <= n; ++i)
			history[i] = insert(history[i - 1], arr[i]);
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
