#pragma once
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
struct Treap_node
{
	Treap_node *left;
	Treap_node *right; //左右孩子节点
	Treap_node *pre;   //前驱，方便维护最小堆序
	long priority;  //优先级
	long data;    //卫星数据
	long same;     // 有几个相同的元素
	long size;     //size域，维护名次树性质
	long rand_priority() //更优化的随机数生成，据说不会重复来着
	{
		static int seed = 233;
		return seed = (int)seed * 482711LL % 2147483647;
	}
	Treap_node(Treap_node *L, Treap_node *R, Treap_node *P, long file) :left(L), right(R), pre(P), data(file)
	{
		priority = rand_priority();
		same = 1;
		size = 1;
	}
	void maintain() //  维护size域
	{
		size = same;
		if (left != NULL)
			size += left->size;
		if (right != NULL)
			size += right->size;
	}
};
class Treap //迭代版，非递归，支持重复元素
{
protected:
	Treap_node *root;  //根节点
	long node_sum;   //所有节点数量
	Treap_node* minimun(Treap_node *x)  //探寻以x为根的最小值
	{
		while (x->left != NULL)   //一直向左走
			x = x->left;
		return x;
	}
	Treap_node* maxmun(Treap_node *x)  //探寻以x为根的最大值
	{
		while (x->right != NULL)  //一直向右走
			x = x->right;
		return x;
	}
	Treap_node *successor(Treap_node *x)   //已有节点的后继，即大于x的最小值
	{
		if (x->right != NULL)
			return minimun(x->right);
		Treap_node *y = x->pre;
		while (y != root && x == y->right)
		{
			x = y;
			y = y->pre;
		}
		return y;
	}
	Treap_node *predecessor(Treap_node *x)   //前驱，即小于x的最大值  后继换个方向就ok
	{
		if (x->left != NULL)
			return maxmun(x->left);     //先找右子树
		Treap_node *y = x->pre;
		while (y != NULL && x == y->left)   //右子树找不到往回找父节点
		{
			x = y;
			y = y->pre;
		}
		return y;
	}
	void rotate_left(Treap_node *part) //左旋
	{
		Treap_node *father = part->pre;
		if (father->pre->left == father)
			father->pre->left = part;
		else
			father->pre->right = part;
		part->pre = father->pre;
		if (part->left != NULL)
			part->left->pre = father; //不处理插入相同元素时结构就炸了
		father->pre = part;
		father->right = part->left;
		part->left = father;
		part->left->maintain();   //同时维护size域
		part->maintain();
		father->maintain();
	}
	void rotate_right(Treap_node *part)  //右旋,就左旋换个方向,复制粘贴改方向就ok
	{
		Treap_node *father = part->pre;
		if (father->pre->left == father)
			father->pre->left = part;
		else
			father->pre->right = part;
		part->pre = father->pre;
		if (part->right != NULL)
			part->right->pre = father;
		father->left = part->right;
		father->pre = part;
		part->right = father;
		part->right->maintain();
		part->maintain();
		father->maintain();
	}
	void min_heapify(Treap_node *part) //维护最小堆序的性质 ,迭代实现
	{
		while (part->pre != NULL && part->pre->priority > part->priority)  //结点上浮维护堆序
		{
			Treap_node *father = part->pre;
			if (part->pre->data > part->data) //右旋
				rotate_right(part);
			else    //左旋,就右旋换个方向,复制粘贴改方向就ok
				rotate_left(part);
		}
	}
public:
	void Delete(long x)   //删除节点，不管节点存不存在
	{
		Treap_node *find_p = find_x(x);
		if (find_p == NULL)
			return;
		else
		{
			node_sum--;
			if (find_p->same > 1)
			{
				find_p->same--;
				find_p->maintain();
				Treap_node *father = find_p->pre;
				while (father != NULL)
				{
					father->maintain();
					father = father->pre;
				}
			}
			else
			{
				while (find_p->right != NULL && find_p->left != NULL)   //两侧有节点的先旋转到一侧为空再删除
				{
					if (find_p->left->priority < find_p->right->priority)
						rotate_right(find_p->left);
					else
						rotate_left(find_p->right);
					find_p->maintain();
				}
				Treap_node *father = find_p->pre;
				if (find_p->left == NULL && find_p->right == NULL)  //判断后删除节点，复制粘贴改方向
					if (father->left == find_p)
						father->left = NULL;
					else
						father->right = NULL;
				else if (find_p->left == NULL && find_p->right != NULL)
				{
					if (father->left == find_p)
						father->left = find_p->right;
					else
						father->right = find_p->right;
					find_p->right->pre = father;
				}
				else if (find_p->left != NULL && find_p->right == NULL)
				{
					if (father->left == find_p)
						father->left = find_p->left;
					else
						father->right = find_p->left;
					find_p->left->pre = father;
				}
				while (father != NULL)
				{
					father->maintain();
					father = father->pre;
				}
			}
		}
	}
	Treap()//预留节点root 最高优先级
	{
		root = new Treap_node(NULL, NULL, NULL, -10000000);
		root->same = 100000000;
		for (int i = 0; i < 233; ++i) // 随机数，看脸决定树高
			root->rand_priority();
		root->priority = -1;
		node_sum = 0;
	};
	long lower_bound(long x)   //不存在的节点先插入再删除找前驱，常数大一点懒得改
	{
		if (find_x(x) == NULL)
		{
			insert(x);
			Treap_node *part = predecessor(find_x(x));
			Delete(x);
			return part->data;
		}
		else
			return predecessor(find_x(x))->data;
	}
	long upper_bound(long x) //同lower_bound
	{
		if (find_x(x) == NULL)
		{
			insert(x);
			Treap_node *part = successor(find_x(x));
			Delete(x);
			return part->data;
		}
		else
			return successor(find_x(x))->data;
	}
	void insert(long data)//同二叉树插入
	{
		node_sum++;
		Treap_node *find_p = root, *part = NULL;
		while (find_p != NULL)
		{
			part = find_p;
			find_p->size++;
			if (data > find_p->data)
				find_p = find_p->right;
			else if (data < find_p->data)
				find_p = find_p->left;
			else
			{
				find_p->same++;
				find_p->maintain();
				return;
			}
		}
		Treap_node *node = new Treap_node(NULL, NULL, part, data);
		if (data > part->data)
			part->right = node;
		else
			part->left = node;
		min_heapify(node); //维护最小堆序
	}
	Treap_node *find_x(long x)//寻找key为x的节点
	{
		Treap_node *part = root;
		while (part != NULL && part->data != x)
		{
			if (x < part->data)
				part = part->left;
			else
				part = part->right;
		}
		return part;
	}
	long kth(long k)  //默认树根不为空求k大元素
	{
		Treap_node *find_p = root->right;
		while (find_p != NULL && k > 0)
		{
			if (find_p->right != NULL)
			{
				if (k > find_p->right->size)
				{
					k -= find_p->right->size;
					k -= find_p->same;
					if (k > 0)
						find_p = find_p->left;
				}
				else
					find_p = find_p->right;
			}
			else
			{
				k -= find_p->same;
				if (k > 0)
					find_p = find_p->left;
			}
		}
		if (find_p == NULL && k > 0)
			return -1;
		return find_p->data;
	}
	long kth_2(long k)  //默认树根不为空求k小元素
	{
		Treap_node *find_p = root->right;
		while (find_p != NULL && k > 0)
		{
			if (find_p->left != NULL)
			{
				if (k > find_p->left->size)
				{
					k -= find_p->left->size;
					k -= find_p->same;
					if (k > 0)
						find_p = find_p->right;
				}
				else
					find_p = find_p->left;
			}
			else
			{
				k -= find_p->same;
				if (k > 0)
					find_p = find_p->right;
			}
		}
		if (find_p == NULL && k > 0)
			return -1;
		return find_p->data;
	}
	long rank(long x)   //查询数据x的排名,降序
	{
		Treap_node *part = find_x(x);
		if (part == NULL)
			return -1;
		Treap_node *father = part->pre;
		long  sum = 1;
		if (part->right != NULL)
			sum += part->right->size;
		while (father != NULL)
		{
			if (father->right != NULL && father->right != part)
				sum += father->right->size;
			if (father->data > x)
				sum += father->same;
			part = father;
			father = father->pre;
		}
		return sum;
	}
	long rank_2(long x)   //查询数据x的排名,升序
	{
		Treap_node *part = find_x(x);
		if (part == NULL)
			return -1;
		Treap_node *father = part->pre;
		long  sum = 1;
		if (part->left != NULL)
			sum += part->left->size;
		while (father != root)
		{
			if (father->left != NULL && father->left != part)
				sum += father->left->size;
			if (father->data < x)
				sum += father->same;
			part = father;
			father = father->pre;
		}
		return sum;
	}
};
