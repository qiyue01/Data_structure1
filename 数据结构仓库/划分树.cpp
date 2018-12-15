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

int sorted[N];   //�����������
int toleft[30][N];   //toleft[i][j]��ʾ��i���1��k�ж��ٸ����������
int tree[30][N];  //��ʾÿ��ÿ��λ�õ�ֵ

void buildingTree(int l, int r, int dep)
{
	if (l == r)    return;
	int mid = (l + r) >> 1;
	int i, sum = mid - l + 1;  //��ʾ�����м�ֵ���ұ�������ߵĸ���
	for (i = l; i <= r; i++)
	{
		if (tree[dep][i] < sorted[mid])    sum--;
	}
	int lpos = l;
	int rpos = mid + 1;
	for (i = l; i <= r; i++)
	{
		if (tree[dep][i] < sorted[mid])    //���м����С���������
		{
			tree[dep + 1][lpos++] = tree[dep][i];
		}
		else if (tree[dep][i] == sorted[mid] && sum > 0) //�����м����ֵ��������ߣ�ֱ��sum==0��ֵ��ұ�
		{
			tree[dep + 1][lpos++] = tree[dep][i];
			sum--;
		}
		else  //�ұ�
		{
			tree[dep + 1][rpos++] = tree[dep][i];
		}
		toleft[dep][i] = toleft[dep][l - 1] + lpos - l;   //��1��i����ߵĸ���
	}
	buildingTree(l, mid, dep + 1);
	buildingTree(mid + 1, r, dep + 1);
}

//��ѯ�����k�������[L,R]�Ǵ����䣬[l,r]��Ҫ��ѯ��С����
int queryTree(int L, int R, int l, int r, int dep, int k)
{
	if (l == r)    return tree[dep][l];
	int mid = (L + R) >> 1;
	int cnt = toleft[dep][r] - toleft[dep][l - 1];  //[l,r]��λ����ߵĸ���
	if (cnt >= k)
	{
		int newl = L + toleft[dep][l - 1] - toleft[dep][L - 1]; //L+Ҫ��ѯ������ǰ��������ߵĸ���
		int newr = newl + cnt - 1;  //��˵���ϲ�ѯ����ᱻ������ߵĸ���
		return queryTree(L, mid, newl, newr, dep + 1, k);
	}
	else
	{
		int newr = r + toleft[dep][R] - toleft[dep][r];
		int newl = newr - (r - l - cnt);
		return queryTree(mid + 1, R, newl, newr, dep + 1, k - cnt);
	}
}