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
	const int N = 100100;
	struct node
	{
		int L, R,sum,sum2,Max;
		list<pair<pair<int, int >,int > > lazy;
	};
	node tree_pool[N*4];
	int pool[N];
	void clear_tree(int sum)
	{
		memset(pool, 0, sizeof(pool));
		for (int i = 0; i < sum * 4; ++i)
			tree_pool[i].lazy.clear();
	}
	void build(int cur, int L, int R)
	{
		int mid = (L + R) / 2;
		tree_pool[cur].L = L;
		tree_pool[cur].R = R;
		tree_pool[cur].sum2 = 0;
		if (L == R)
		{
			tree_pool[cur].sum = pool[L];
			tree_pool[cur].Max = pool[L];
			return;
		}
		build(cur << 1, L, mid);
		build((cur << 1) + 1, mid + 1, R);
		tree_pool[cur].sum = tree_pool[cur << 1].sum+ tree_pool[(cur << 1)+1].sum;
		tree_pool[cur].Max = max(tree_pool[cur << 1].Max , tree_pool[(cur << 1) + 1].Max);
	}
	void push_down(int cur)
	{
		int mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
		if (!tree_pool[cur].lazy.empty())
			for (auto &s : tree_pool[cur].lazy)
			{
				int L = s.first.first,R=s.first.second;
				tree_pool[cur].sum2 += s.second*(s.first.second-s.first.first+1);
				tree_pool[cur].Max += s.second;
				if (tree_pool[cur].L != tree_pool[cur].R)
				{
					if (L <= mid && R > mid)
					{
						tree_pool[cur << 1].lazy.push_back(make_pair(make_pair(L , mid), s.second));
						tree_pool[(cur << 1)+1].lazy.push_back(make_pair(make_pair(mid+1,R), s.second));
					}
					else if (R <= mid)
						tree_pool[cur << 1].lazy.push_back(make_pair(make_pair(L, R), s.second));
					else if (L >= mid + 1)
						tree_pool[(cur << 1) + 1].lazy.push_back(make_pair(make_pair(L, R), s.second));
				}
			}
		tree_pool[cur].lazy.clear();
	}
	void modify(int cur,int position,int w) //单点修改最大值
	{
		int ans = 0, mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
		if (tree_pool[cur].L == tree_pool[cur].R)
		{
			tree_pool[cur].Max = w;
			return ;
		}
		push_down(cur);
		if (position <= mid)
			modify((cur << 1), position, w);
		else
			modify((cur << 1) + 1, position, w);
		tree_pool[cur].Max = max(tree_pool[(cur << 1)].Max, tree_pool[(cur << 1) + 1].Max);
	}
	void range_add(int L,int R,int k)
	{
		tree_pool[1].lazy.push_back(make_pair(make_pair(L, R),k));
	}
	int range_ask_sum(int cur,int L,int R)
	{
		int ans = 0,mid=(tree_pool[cur].L+ tree_pool[cur].R)/2;
		push_down(cur);
		if (L == tree_pool[cur].L && R == tree_pool[cur].R)
			return tree_pool[cur].sum + tree_pool[cur].sum2;
		if (L <= mid && R > mid)
			ans += range_ask_sum((cur << 1), L, mid) + range_ask_sum((cur << 1) + 1, mid + 1, R);
		else if (R <= mid)
			ans += range_ask_sum((cur << 1), L, R);
		else if(L>=mid+1)
			ans += range_ask_sum((cur << 1)+1, L, R);
		return ans;
	}
	int range_ask_max(int cur, int L, int R)
	{
		int ans = 0, mid = (tree_pool[cur].L + tree_pool[cur].R) / 2;
		push_down(cur);
		if (L == tree_pool[cur].L && R == tree_pool[cur].R)
			return tree_pool[cur].Max;
		if (L <= mid && R > mid)
			ans = max(range_ask_max((cur << 1), L, mid) , range_ask_max((cur << 1) + 1, mid + 1, R));
		else if (R <= mid)
			ans =max(ans, range_ask_max((cur << 1), L, R));
		else if (L >= mid + 1)
			ans = max(ans,range_ask_max((cur << 1) + 1, L, R));
		return ans;
	}
}
using namespace segment_tree;
