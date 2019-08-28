#define maxn 2100010
#define M 20
const int BSZ = 25, BNUM = maxn / BSZ + 10;
struct ST
{
	long long n, m;
	array<long long, maxn> s, Lmax, Rmax, bel;
	array<long long, BNUM> L, R, LG, ST[M + 1];
private:
	inline long long query(int l, int r) {
		if (l >= r)
			return LLONG_MIN;
		static int t;
		t = LG[r - l];
		return max(ST[t][l], ST[t][r - (1 << t)]);
	}
public:
	void init(int k)
	{
		n = k;
		for (int i = 1; i <= n; ++i)
			bel[i] = i / BSZ + 1;
		const int B = bel[n];
		LG[0] = -1;
		for (int i = 1; i <= B; ++i)
			LG[i] = LG[i >> 1] + 1;
		for (int i = 1; i <= B; ++i) {
			L[i] = (i - 1) * BSZ;
			R[i] = L[i] + BSZ - 1;
		}
		L[1] = 1, R[B] = n;
		long long last = LLONG_MIN;
		for (int i = 1, now = 1; i <= n; ++i) {
			Lmax[i] = last = std::max(s[i], last);
			if (i >= R[now])
				ST[0][now] = Lmax[i], last = LLONG_MIN, ++now;
		}
		last = LLONG_MIN;
		for (int i = n, now = B; i; --i)
		{
			Rmax[i] = last = max(s[i], last);
			if (i <= L[now])
				last = LLONG_MIN, --now;
		}
		for (int i = 1, pw = 1; i <= M; ++i, pw <<= 1)
		{
			for (int j = 1; j <= B; ++j)
				ST[i][j] = std::max(ST[i - 1][j], ST[i - 1][std::min(j + pw, B)]);
		}
	}
	long long query1(int l, int r)
	{
		if (l > r)
			swap(l, r);
		const int lb = bel[l], rb = bel[r];
		if (lb != rb)
			return max(max(Rmax[l], Lmax[r]), query(lb + 1, rb));
		else
		{
			static long long res;
			res = LLONG_MIN;
			for (int i = l; i <= r; ++i)
				res = max(res, s[i]);
			return res;
		}
	}
}p;