#include "pch.h"
#include <iostream>
#include <algorithm>
#include <stack>
#include <vector>
#include <queue>
#define inf 0x7fffffff
using namespace std;
#define null NULL
const int MAXN = 120010;
const int MAXM = 200000;
template<class T, size_t size>//�ڴ�صĶ���
struct MemoryPool {
	T buf[size], *tail, *st[size];
	int top;
	inline T *alloc() { return top ? st[--top] : tail++; }
	inline void recycle(T *x) { st[top++] = x; }
	MemoryPool() : top(0), tail(buf) {}
	void clear()
	{
		top = 0;
		tail = buf;
	}
};
template <typename T>
struct Splay {
	enum Relation { L = 0, R = 1 };
	struct Node {
		Node *child[2], *parent, **root;
		T value, max, lazy,sum;//ά���ĸ�����Ϣ�Լ����
		int size;
		bool reversed, bound;
		inline void init(Node *parent, const T &value, Node **root, bool bound = false) { this->parent = parent, this->value = value, this->lazy = 0, this->max = value, this->reversed = false, this->root = root, this->bound = bound, this->size = 1, child[L] = child[R] = null; }
		inline Relation relation()
		{
			return this == parent->child[L] ? L : R;
		}
		inline void maintain() //�ڵ���Ϣά��
		{
			pushDown(), size = 1 + (child[L] ? child[L]->size : 0) + (child[R] ? child[R]->size : 0);
			max = value;
			sum = value;
			if (child[L]) max = std::max(max, child[L]->max),sum += child[L]->sum;
			if (child[R]) max = std::max(max, child[R]->max),sum += child[R]->sum;
		}
		inline void pushDown() { //�����
			if (reversed) {
				if (child[L]) child[L]->reversed ^= 1;
				if (child[R]) child[R]->reversed ^= 1;
				std::swap(child[L], child[R]);
				reversed = false;
			}
			if (lazy) {
				if (child[L]) child[L]->lazy += lazy, child[L]->value += lazy, child[L]->max += lazy;
				if (child[R]) child[R]->lazy += lazy, child[R]->value += lazy, child[R]->max += lazy;
				lazy = 0;
			}
		}
		inline void rotate() //ά��splay���ʵ���������
		{
			if (parent->parent) parent->parent->pushDown();
			parent->pushDown(), pushDown();
			Relation x = relation();
			Node *oldParent = parent;
			if (oldParent->parent) oldParent->parent->child[oldParent->relation()] = this;
			parent = oldParent->parent, oldParent->child[x] = child[x ^ 1];
			if (child[x ^ 1]) child[x ^ 1]->parent = oldParent;
			child[x ^ 1] = oldParent, oldParent->parent = this, oldParent->maintain(), maintain();
			if (!parent) *root = this;
		}
		inline void splay(Node *targetParent = null) //splay���ĸ�λ�ã�Ĭ��Ϊ��
		{
			while (parent != targetParent) {
				if (parent->parent == targetParent) rotate();
				else {
					parent->parent->pushDown(), parent->pushDown();
					if (parent->relation() == relation()) parent->rotate(), rotate();
					else rotate(), rotate();
				}
			}
		}
		inline int rank() //����rank
		{
			return child[L] ? child[L]->size : 0;
		}
	} *root;
	MemoryPool<Node, MAXN * 2> pool;//�ڴ��
	//Node *pool2[MAXN * 2];
	Splay() : root(null) {}
	inline void build(const T *a, int n)//����������Ԫ��λ��Ӧ[0,n-1];
	{
		root = buildRange(a, 1, n, null);
		buildBound(L), buildBound(R);
	}
	inline Node *buildRange(const T *a, int l, int r, Node *parent)
	{
		if (l > r) return null;
		register int mid = l + r >> 1;
		Node *v = pool.alloc();
		v->init(parent, a[mid - 1], &root);
		//pool2[mid - 1] = v;
		if (l != r) v->child[L] = buildRange(a, l, mid - 1, v), v->child[R] = buildRange(a, mid + 1, r, v);
		return v->maintain(), v;
	}
	inline void buildBound(Relation x)//�Ӹ����ұ߽�
	{
		Node **v = &root, *parent = null;
		while (*v) parent = *v, parent->size++, v = &parent->child[x];
		*v = pool.alloc(), (*v)->init(parent, 0, &root, true), (*v)->maintain();
	}
	inline Node *select(int k)  //����������kλ�õ�Ԫ�أ�����kλ�õ�ָ��
	{
		k++;
		Node *v = root;
		while (v->pushDown(), k != v->rank() + 1)
			v = (k <= v->rank() ? v->child[L] : (k -= v->rank() + 1, v->child[R]));
		return v->splay(), v;
	}
	inline Node *select(int l, int r) //��ȡ���� L-R,����[l��r]���������ĸ�ͬʱά����Ϣ
	{
		Node *lbound = select(l - 1), *rbound = select(r + 1);
		lbound->splay(), rbound->splay(lbound);
		return rbound->child[L];
	}
	inline void update(int l, int r, const T &addition) //�����
	{
		Node *range = select(l, r);
		range->value += addition, range->lazy += addition, range->max += addition;
	}
	inline const T &queryMax(int l, int r) //�������ֵ
	{
		Node *range = select(l, r);
		return range->max;
	}
	inline void reverse(int l, int r) //���䷴ת
	{
		Node *range = select(l, r);
		range->reversed ^= 1;
	}
	void delete_root()//�Լ�д��ɾ��
	{
		Node *newroot = select(root->rank(), root->rank())->parent;
		newroot->child[L] = NULL;
		newroot->maintain();
	}
	int query_rank(Node *query)//ѯ�ʽڵ��rank
	{
		if (query == root)
			return query->rank();
		Node *find_p = query;
		stack<Node *> st;
		while (find_p != root)
		{
			find_p = find_p->parent;
			st.push(find_p);
		}
		while (!st.empty())
		{
			find_p = st.top();
			st.pop();
			find_p->pushDown();
		}
		query->pushDown();
		query->splay();
		return query->rank();
	}
};
int n, m, q[MAXN];
Splay<int> splay;