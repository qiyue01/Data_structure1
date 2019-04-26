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
using namespace std;

/*
const long long maxn = 3010;
long long n, m, a[maxn][maxn][4];

void add(long long x, long long y, long long v) {
	for (int i = x; i <= n; i += i & -i)
		for (int j = y; j <= m; j += j & -j) {
			a[i][j][0] += v;
			a[i][j][1] += y * v;
			a[i][j][2] += x * v;
			a[i][j][3] += x * y * v;
		}
	return;
}
void add(int a, int b, int c, int d, int x)
{
	add(a, b, x);
	add(a, d + 1, -x);
	add(c + 1, b, -x);
	add(c + 1, d + 1, x);
}
long long ask(long long x, long long y) {
	long long ans = 0;
	for (int i = x; i; i -= i & -i)
		for (int j = y; j; j -= j & -j)
			ans += (x + 1) * (y + 1) * a[i][j][0] - (x + 1) * a[i][j][1] - (y + 1) * a[i][j][2] + a[i][j][3];
	return ans;
}

long long query(long long a, long long b, long long c, long long d)
{
	return ask(c, d) - ask(a - 1, d) - ask(c, b - 1) + ask(a - 1, b - 1);
}
*/

/*
long long A[5000][5000];
long long
int N, M;

void update(int x, int y, long long val) {
	for (int i = x; i <= N; i += i & (-i)) {
		for (int j = y; j <= M; j += j & (-j)) {
			A[i][j] += val;
		}
	}
}

long long getsum(int x, int y) {
	long long sum = 0;
	for (int i = x; i > 0; i -= i & (-i)) {
		for (int j = y; j > 0; j -= j & (-j)) {
			sum += A[i][j];
		}
	}
	return sum;
}
long long getsum(int x, int y, int a, int b)
{
	return getsum(a, b) + getsum(x - 1, y - 1) - getsum(a, y - 1) - getsum(x - 1, b);
}
*/
