const int sigma = 62;
struct Base
{
	long long d[sigma + 1];
	long long base = 1, p[sigma + 1], cntp;
	int addtag;
	Base()
	{
		addtag = 1;
		memset(d, 0, sizeof(d));
	}
	void add(long long x)
	{
		for (int i = sigma; i >= 0; i--)
		{
			if (x&(base << i))
			{
				if (d[i])
					x ^= d[i];
				else
				{
					d[i] = x;
					return;//�ǵ��������ɹ�һ��Ҫ�˳�
				}
			}
		}
		addtag = 0;
	}
	bool check(long long x)
	{
		for (int i = sigma; i >= 0; i--)
		{
			if (x&(base << i))
			{
				if (d[i])
					x ^= d[i];
				else
				{
					return 0;
				}
			}
		}
		return 1;
	}
	long long MAX()
	{
		long long anss = 0;
		for (int i = sigma; i >= 0; i--)//�ǵô����Ի������λ��ʼ
			if ((anss^d[i]) > anss)anss ^= d[i];
		return anss;
	}
	Base inters(Base &a, Base &b)//��
	{
		Base ans, c = b, d = b;
		long long x, T;
		for (int i = 0; i <= sigma; ++i)
		{
			x = a.d[i];
			if (!x)
				continue;
			int j = i;
			T = 0;
			for (; j >= 0; --j)
			{
				if ((x >> j) & 1)
					if (c.d[j])
					{
						x ^= c.d[j]; T ^= d.d[j];
					}
					else break;
			}
			if (!x)
				ans.d[i] = T;
			else
			{
				c.d[j] = x;
				d.d[j] = T;
			}
		}
		return ans;
	}
	void work()//�������Ի�����k��
	{
		for (int i = sigma; i >= 0; i--)
			if (d[i])
				p[cntp++] = d[i];
	}
	long long Solve(long long k)
	{
		long long L = 1, R = 1ll << cntp, m, x = 0, lc, rc;
		k += addtag;
		if (k > R)return -1;
		for (int i = 0; i < cntp; i++)
		{
			lc = x, rc = x ^ p[i]; m = L + R >> 1;
			if (lc > rc)swap(lc, rc);
			if (k <= m)R = m, x = lc;
			else L = m + 1, x = rc;
		}
		return x;
	}
}b;