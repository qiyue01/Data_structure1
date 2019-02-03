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
const int N = 100010;

int sorted[N];   //排序完的数组
int toleft[30][N];   //toleft[i][j]表示第i层从1到k有多少个数分入左边
int tree[30][N];  //表示每层每个位置的值
//使用的时候先sort，同时 toleft[0]需要使用原数组初始化
void buildingTree(int l, int r, int dep)
{
	if (l == r)    return;
	int mid = (l + r) >> 1;
	int i, sum = mid - l + 1;  //表示等于中间值而且被分入左边的个数
	for (i = l; i <= r; i++)
	{
		if (tree[dep][i] < sorted[mid])    sum--;
	}
	int lpos = l;
	int rpos = mid + 1;
	for (i = l; i <= r; i++)
	{
		if (tree[dep][i] < sorted[mid])    //比中间的数小，分入左边
		{
			tree[dep + 1][lpos++] = tree[dep][i];
		}
		else if (tree[dep][i] == sorted[mid] && sum > 0) //等于中间的数值，分入左边，直到sum==0后分到右边
		{
			tree[dep + 1][lpos++] = tree[dep][i];
			sum--;
		}
		else  //右边
		{
			tree[dep + 1][rpos++] = tree[dep][i];
		}
		toleft[dep][i] = toleft[dep][l - 1] + lpos - l;   //从1到i放左边的个数
	}
	buildingTree(l, mid, dep + 1);
	buildingTree(mid + 1, r, dep + 1);
}

//查询区间第k大的数，[L,R]是大区间，[l,r]是要查询的小区间
int queryTree(int L, int R, int l, int r, int dep, int k)
{
	if (l == r)    return tree[dep][l];
	int mid = (L + R) >> 1;
	int cnt = toleft[dep][r] - toleft[dep][l - 1];  //[l,r]中位于左边的个数
	if (cnt >= k)
	{
		int newl = L + toleft[dep][l - 1] - toleft[dep][L - 1]; //L+要查询的区间前被放在左边的个数
		int newr = newl + cnt - 1;  //左端点加上查询区间会被放在左边的个数
		return queryTree(L, mid, newl, newr, dep + 1, k);
	}
	else
	{
		int newr = r + toleft[dep][R] - toleft[dep][r];
		int newl = newr - (r - l - cnt);
		return queryTree(mid + 1, R, newl, newr, dep + 1, k - cnt);
	}
}