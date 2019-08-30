namespace trie01
{
	const int sigma = 20;
	long long base = 1;
	struct node
	{
		node *child[2];
		int times;
		int element;
		bool lazy;
		void clear()
		{
			lazy = times = element = 0;
		}
	};
	node *history[200000];
	node _pool[3000000];
	node *root, *cur = _pool, *null;
	node *new_()
	{
		cur->clear();
		cur->child[0] = cur->child[1] = null;
		return cur++;
	}
	void init()
	{
		null = new node();
		null->times = null->element = null->lazy = 0;
		null->child[0] = null->child[1] = null;
		cur = _pool;
		root = new_();
		history[0] = new_();
	}
	void insert(node *ver_s, int pos, int element, int add)
	{
		node *found = ver_s;
		bitset<sigma + 1> kkk = pos;
		found->times += add;
		for (int i = sigma; i >= 0; --i)
		{
			if (found->child[kkk[i]] == null)
			{
				found->child[kkk[i]] = new_();
				found = found->child[kkk[i]];
			}
			else
				found = found->child[kkk[i]];
			found->times += add;
		}
		found->element = element;
	}
	node *insert_new(int pos, int element, node *old, int add)
	{
		node *found = old, *_new = new_(), *_new2;
		bitset<sigma + 1> kkk = pos;
		_new2 = _new;
		for (int i = sigma; i >= 0; --i)
		{
			_new->times += add;
			_new->child[kkk[i]] = new_();
			if (found == null)
				_new = _new->child[kkk[i]];
			else
			{
				_new->times += found->times;
				_new->child[!kkk[i]] = found->child[!kkk[i]];
				_new = _new->child[kkk[i]];
				found = found->child[kkk[i]];
			}
		}
		_new->times += found->times;
		_new->times += add;
		_new->element = element;
		return _new2;
	}
	int rank(node *p, int pos)
	{
		bitset<sigma + 1> kkk = pos;
		int rk = 1;
		for (int i = sigma; i >= 0; --i)
		{
			if (kkk[i])
				rk += p->child[0]->times;
			p = p->child[kkk[i]];
		}
		return rk;
	}
	int find(node *p, int pos)
	{
		bitset<sigma + 1> kkk = pos;
		for (int i = sigma; i >= 0; --i)
			p = p->child[kkk[i]];
		return p->times;
	}
	int kth(node *p, int s)
	{
		for (int i = sigma; i >= 0; --i)
			if (p->child[0]->times < s)
			{
				s -= p->child[0]->times;
				p = p->child[1];
			}
			else
				p = p->child[0];
		return p->element;
	}
	int rank(int L, int R, int pos)
	{
		node* PL = history[L - 1], *PR = history[R];
		bitset<sigma + 1> kkk = pos;
		int rk = 1;
		for (int i = sigma; i >= 0; --i)
		{
			if (kkk[i])
				rk += PR->child[0]->times - PL->child[0]->times;
			PL = PL->child[kkk[i]];
			PR = PR->child[kkk[i]];
		}
		return rk;
	}
	int prefix(node *p, int pos)
	{
		bitset<sigma + 1> kkk = pos;
		int rk = 0;
		for (int i = sigma; i >= 0; --i)
		{
			if (kkk[i])
				rk += p->child[0]->times;
			p = p->child[kkk[i]];
		}
		rk += p->times;
		return rk;
	}
	int querysum(node *p, int L, int R)
	{
		return prefix(p, R) - prefix(p, L - 1);
	}
	int kth(int L, int R, int s)
	{
		node* PL = history[L - 1], *PR = history[R];
		int rk;
		for (int i = sigma; i >= 0; --i)
		{
			rk = PR->child[0]->times - PL->child[0]->times;
			if (rk < s)
			{
				s -= rk;
				PL = PL->child[1];
				PR = PR->child[1];
			}
			else
			{
				PL = PL->child[0];
				PR = PR->child[0];
			}
		}
		return rk;
	}
	void merge(node *a, node *b)
	{
		b->times += a->times;
		for (int i = 0; i < 2; ++i)
		{
			if (a->child[i] != null && b->child[i] != null)
				merge(a->child[i], b->child[i]);
			else if (b->child[i] == null)
				b->child[i] = a->child[i];
		}
	}
	node *merge2(node *x, node *y) //合并x,y,返回合并后的根
	{
		if (x == null) return y;
		if (y == null) return x;
		node *merge_version = new_();
		merge_version->times = y->times + x->times;
		merge_version->child[0] = merge2(x->child[0], y->child[0]);
		merge_version->child[1] = merge2(x->child[1], y->child[1]);
		return merge_version;
	}
	void merge_all(node *x, node *y) // 将x全树合并到y上
	{
		y->times += x->times;
		y->child[0] = merge2(x->child[0], y->child[0]);
		y->child[1] = merge2(x->child[1], y->child[1]);
	}
}
using namespace trie01;