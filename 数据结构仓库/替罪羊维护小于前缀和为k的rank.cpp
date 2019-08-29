struct ScapegoatTree //�Ѿ��ǿ��ؼ� 
{
	static const int MAXN = 300010;
	struct Node {
		Node * ch[2];
		long long key, size, cover, sum; /* sizeΪ��Ч�ڵ��������coverΪ�ڵ������� */
		bool exist; /* �Ƿ���ڣ����Ƿ�ɾ���� */
		inline void update() //���½ڵ���Ϣ
		{
			size = ch[0]->size + ch[1]->size + (int)exist, cover = ch[0]->cover + ch[1]->cover + 1;
			sum = key;
			sum += ch[0]->sum + ch[1]->sum;
		}
		inline bool check(void) { /* �ж��Ƿ���Ҫ�ع� */
			return ((ch[0]->cover > cover * alpha + 5) || (ch[1]->cover > cover * alpha + 5));
		}
	};
	Node *root, *null;//root���ڵ�ָ��
	MemoryPool<Node, MAXN> pool;//�ڴ�ز���˵��
	Node *newNode(int key) //��key��Ϊ��keyֵ�����½��ڵ�
	{
		Node *p = pool.alloc();
		p->ch[0] = p->ch[1] = null, p->size = p->cover = 1, p->exist = true, p->key = key, p->sum = key;
		return p;
	}
	inline void travel(Node *p, std::vector<Node *> &v) //�������еĹ���
	{
		if (p == null) return;
		travel(p->ch[0], v);
		if (p->exist) v.push_back(p); /* �������� */
		else pool.recycle(p); /* ���� */
		travel(p->ch[1], v);
	}
	inline Node *divide(std::vector<Node *> &v, int l, int r) //v���ڴ�����Ҫ�ع��Ľڵ�
	{
		if (l >= r) return null;
		int mid = l + r >> 1;
		Node *p = v[mid];
		p->ch[0] = divide(v, l, mid);
		p->ch[1] = divide(v, mid + 1, r);
		p->update(); /* �Ե�����ά������ά������ */
		return p;
	}
	inline void rebuild(Node *&p) {
		static std::vector<Node *> v;
		v.clear(), travel(p, v), p = divide(v, 0, v.size());//�ؽ���ʱ��ֱ�ӵݹ齨��
	}
	inline Node **insert(Node *&p, int val)
	{
		if (p == null) //Ϊ�ռ������½ڵ�
		{
			p = newNode(val);
			return &null;
		}
		else {
			p->size++, p->cover++;
			/* ����ֵ������Ҫ�ع���λ�ã�������Ҳ��Ҫ�ع������ڵ㿪ʼҲ��Ҫ�ع����Ա��ڵ�Ϊ���ع� */
			Node **res = insert(p->ch[val >= p->key], val);
			if (p->check()) res = &p;
			p->update();
			return res;
		}
	}
	inline void erase(Node *p, int id) //����ɾ��
	{
		p->size--;
		register int offset = p->ch[0]->size + p->exist;
		if (p->exist && id == offset)
		{
			p->exist = false;
			return;
		}
		else {
			if (id <= offset) erase(p->ch[0], id);
			else erase(p->ch[1], id - offset);
		}
	}
	ScapegoatTree() {
		null = pool.alloc();
		null->ch[0] = null->ch[1] = null;
		null->cover = null->size = null->key = null->sum = 0;
		root = null;
	}
	inline void insert(int val)
	{
		Node **p = insert(root, val);
		if (*p != null) rebuild(*p);
	}
	inline int rank(long long res) //����ÿ��Ԫ�ص�rank
	{
		Node *p = root;
		if (p->size == 0)
			return 0;
		register int ans = 0;
		while (p != null)
		{
			if (res == 0)
				return ans;
			long long sum2 = p->ch[0]->sum + p->key;
			//cout << p->sum << " " << res << endl;
			if (sum2 > res)
				p = p->ch[0];
			else
			{
				res -= sum2;
				ans += p->ch[0]->size + 1;
				p = p->ch[1];
			}
		}
		return ans;
	}
	inline int select(int k)  //�ҵ�k��Ԫ��
	{
		Node * p = root;
		while (p != null)
		{
			if (p->ch[0]->size + 1 == k && p->exist) return p->key;
			else if (p->ch[0]->size >= k) p = p->ch[0];
			else k -= p->ch[0]->size + p->exist, p = p->ch[1];
		}
	}
};