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
#include<memory.h>
#include<functional>
using namespace std;
namespace segment_tree
{
	/**
	*    基础部分
	**/
	const int N = 100100;
	struct node
	{
		int L, R,sum,sum2,modify;
		bool lazy;
	};
	struct tree
	{
		node tree_pool[N * 4];
		int pool[N];
		void clear_tree(int sum)
		{
			memset(pool, 0, sizeof(pool));
			for (int i = 0; i < sum * 4; ++i)
				tree_pool[i].lazy = false;
		}
		void update(int cur)
		{
			tree_pool[cur].sum = tree_pool[cur << 1].sum + tree_pool[(cur << 1) + 1].sum;
		}
		void build(int cur, int L, int R)
		{
			int mid = (L + R) / 2;
			tree_pool[cur].L = L;
			tree_pool[cur].R = R;
			tree_pool[cur].sum2 = 0;
			tree_pool[cur].modify = 0;
			if (L == R)
			{
				tree_pool[cur].sum = pool[L];
				return;
			}
			build(cur << 1, L, mid);
			build((cur << 1) + 1, mid + 1, R);
			update(cur);
		}
		/**
		*    维护区间加和区间和操作
		**/
		void push_down(int cur)
		{
			tree_pool[cur].sum += tree_pool[cur].sum2*(tree_pool[cur].R - tree_pool[cur].L + 1);
			tree_pool[cur].lazy = false;
			if (tree_pool[cur].L != tree_pool[cur].R)
			{
				tree_pool[cur << 1].lazy = true;
				tree_pool[(cur << 1) + 1].lazy = true;
				tree_pool[cur << 1].sum2 += tree_pool[cur].sum2;
				tree_pool[(cur << 1) + 1].sum2 += tree_pool[cur].sum2;
			}
			tree_pool[cur].sum2 = 0;
		}
		void range_add(int cur, int L, int R, int k)
		{
			int ans = 0, mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
			if (L == tree_pool[cur].L && R == tree_pool[cur].R)
			{
				tree_pool[cur].sum2 += k;
				tree_pool[cur].lazy = true;
				return;
			}
			tree_pool[cur].sum += k * (R - L + 1);
			if (L <= mid && R > mid)
			{
				range_add((cur << 1), L, mid, k);
				range_add((cur << 1) + 1, mid + 1, R, k);
			}
			else if (R <= mid)
				range_add((cur << 1), L, R, k);
			else if (L >= mid + 1)
				range_add((cur << 1) + 1, L, R, k);
		}
		int range_ask_sum(int cur, int L, int R)
		{
			int ans = 0, mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
			push_down(cur);
			if (L == tree_pool[cur].L && R == tree_pool[cur].R)
				return tree_pool[cur].sum;
			if (L <= mid && R > mid)
				ans += range_ask_sum((cur << 1), L, mid) + range_ask_sum((cur << 1) + 1, mid + 1, R);
			else if (R <= mid)
				ans += range_ask_sum((cur << 1), L, R);
			else if (L >= mid + 1)
				ans += range_ask_sum((cur << 1) + 1, L, R);
			return ans;
		}
		/**
		*    维护区间覆盖和区间和操作
		*    没有写同时支持区间覆盖和区间加版本
		**/
		void push_down_modify(int cur)
		{
			tree_pool[cur].sum = tree_pool[cur].sum2*(tree_pool[cur].R - tree_pool[cur].L + 1);
			tree_pool[cur].lazy = false;
			if (tree_pool[cur].L != tree_pool[cur].R)
			{
				tree_pool[cur << 1].lazy = true;
				tree_pool[(cur << 1) + 1].lazy = true;
				tree_pool[cur << 1].sum2 = tree_pool[cur].sum2;
				tree_pool[cur << 1].sum = tree_pool[cur << 1].sum2*(tree_pool[cur << 1].R - tree_pool[cur << 1].L + 1);
				tree_pool[(cur << 1) + 1].sum2 = tree_pool[cur].sum2;
				tree_pool[(cur << 1) + 1].sum = tree_pool[(cur << 1) + 1].sum2*(tree_pool[(cur << 1) + 1].R - tree_pool[(cur << 1) + 1].L + 1);
			}
		}
		void range_modify(int cur, int L, int R, int k)
		{
			int ans = 0, mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
			if (tree_pool[cur].lazy == true)
				push_down_modify(cur);
			if (L == tree_pool[cur].L && R == tree_pool[cur].R)
			{
				tree_pool[cur].sum2 = k;
				tree_pool[cur].sum = k * (tree_pool[cur].R - tree_pool[cur].L + 1);
				tree_pool[cur].lazy = true;
				return;
			}
			if (L <= mid && R > mid)
			{
				range_modify((cur << 1), L, mid, k);
				range_modify((cur << 1) + 1, mid + 1, R, k);
			}
			else if (R <= mid)
				range_modify((cur << 1), L, R, k);
			else if (L >= mid + 1)
				range_modify((cur << 1) + 1, L, R, k);
			update(cur);
		}
		int range_ask_sum_modify(int cur, int L, int R)
		{
			int ans = 0, mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
			if (tree_pool[cur].lazy)
				push_down_modify(cur);
			if (L == tree_pool[cur].L && R == tree_pool[cur].R)
				return tree_pool[cur].sum;
			if (L <= mid && R > mid)
				ans += range_ask_sum_modify((cur << 1), L, mid) + range_ask_sum_modify((cur << 1) + 1, mid + 1, R);
			else if (R <= mid)
				ans += range_ask_sum_modify((cur << 1), L, R);
			else if (L >= mid + 1)
				ans += range_ask_sum_modify((cur << 1) + 1, L, R);
			return ans;
		}
	};
}
using namespace segment_tree;
