#include"pch.h"
#include<string>
#include<vector>

#include<algorithm>
#include<cstring>
#include<cstdio>
#include<deque>
#include<iomanip>
#include<sstream>
#include<stack>
#include<iostream>
#include<limits>
#include<bitset>
#include<list>
#include<queue>
#include<memory>
#include<functional>
using namespace std;
template<class T, size_t size>
struct MemoryPool {
	T buf[size], *tail;
	T *st[size];
	int top;
	MemoryPool() : top(0), tail(buf) {}
	inline T *alloc() {
		return top ? st[--top] : tail++;
	}
	inline void recycle(T *p) {
		st[top++] = p;
	}
};
const double alpha = 0.75;
struct ScapegoatTree //�Ѿ��ǿ��ؼ� 
{	
	static const int MAXN = 100010;
	struct Node {
		Node * ch[2];
		int key, size, cover; /* sizeΪ��Ч�ڵ��������coverΪ�ڵ������� */
		bool exist; /* �Ƿ���ڣ����Ƿ�ɾ���� */
		inline void update() //���½ڵ���Ϣ
		{
			size = ch[0]->size + ch[1]->size + (int)exist, cover = ch[0]->cover + ch[1]->cover + 1;
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
		p->ch[0] = p->ch[1] = null, p->size = p->cover = 1, p->exist = true, p->key = key;
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
		null->cover = null->size = null->key = 0;
		root = null;
	}
	inline void insert(int val) 
	{
		Node **p = insert(root, val);
		if (*p != null) rebuild(*p);
	}
	inline int rank(int val) //����ÿ��Ԫ�ص�rank
	{
		Node *p = root;
		register int ans = 1;
		while (p != null) 
		{
			if (p->key >= val) p = p->ch[0];
			else 
			{
				ans += p->ch[0]->size + p->exist;
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
	inline void erase(int k) //���Ԫ��k
	{
		erase(root, rank(k));
		if (root->size < alpha * root->cover) rebuild(root);
	}
	inline void eraseByRank(int k) //��rank���Ԫ��
	{
		erase(root, k);
		if (root->size < alpha * root->cover) rebuild(root);
	}
};
//ScapegoatTree tree;
