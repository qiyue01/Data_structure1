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
struct ScapegoatTree //已经是可重集 
{	
	static const int MAXN = 100010;
	struct Node {
		Node * ch[2];
		int key, size, cover; /* size为有效节点的数量，cover为节点总数量 */
		bool exist; /* 是否存在（即是否被删除） */
		inline void update() //更新节点信息
		{
			size = ch[0]->size + ch[1]->size + (int)exist, cover = ch[0]->cover + ch[1]->cover + 1;
		}
		inline bool check(void) { /* 判断是否需要重构 */
			return ((ch[0]->cover > cover * alpha + 5) || (ch[1]->cover > cover * alpha + 5));
		}
	};
	Node *root, *null;//root根节点指针
	MemoryPool<Node, MAXN> pool;//内存池不多说了
	Node *newNode(int key) //把key作为新key值用于新建节点
	{
		Node *p = pool.alloc();
		p->ch[0] = p->ch[1] = null, p->size = p->cover = 1, p->exist = true, p->key = key;
		return p;
	}
	inline void travel(Node *p, std::vector<Node *> &v) //回收序列的过程
	{
		if (p == null) return;
		travel(p->ch[0], v);
		if (p->exist) v.push_back(p); /* 构建序列 */
		else pool.recycle(p); /* 回收 */
		travel(p->ch[1], v);
	}
	inline Node *divide(std::vector<Node *> &v, int l, int r) //v用于传递需要重构的节点
	{
		if (l >= r) return null;
		int mid = l + r >> 1;
		Node *p = v[mid];
		p->ch[0] = divide(v, l, mid);
		p->ch[1] = divide(v, mid + 1, r);
		p->update(); /* 自底向上维护，先维护子树 */
		return p;
	}
	inline void rebuild(Node *&p) {
		static std::vector<Node *> v;
		v.clear(), travel(p, v), p = divide(v, 0, v.size());//重建的时候直接递归建树
	}
	inline Node **insert(Node *&p, int val) 
	{
		if (p == null) //为空即插入新节点
		{ 
			p = newNode(val);
			return &null;
		}
		else {
			p->size++, p->cover++;
			/* 返回值储存需要重构的位置，若子树也需要重构，本节点开始也需要重构，以本节点为根重构 */
			Node **res = insert(p->ch[val >= p->key], val);
			if (p->check()) res = &p;
			return res;
		}
	}
	inline void erase(Node *p, int id) //惰性删除
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
	inline int rank(int val) //查找每个元素的rank
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
	inline int select(int k)  //找第k大元素
	{
		Node * p = root;
		while (p != null) 
		{
			if (p->ch[0]->size + 1 == k && p->exist) return p->key;
			else if (p->ch[0]->size >= k) p = p->ch[0];
			else k -= p->ch[0]->size + p->exist, p = p->ch[1];
		}
	}
	inline void erase(int k) //清除元素k
	{
		erase(root, rank(k));
		if (root->size < alpha * root->cover) rebuild(root);
	}
	inline void eraseByRank(int k) //按rank清楚元素
	{
		erase(root, k);
		if (root->size < alpha * root->cover) rebuild(root);
	}
};
//ScapegoatTree tree;
