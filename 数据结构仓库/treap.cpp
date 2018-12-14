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
	Treap_node *right; //���Һ��ӽڵ�
	Treap_node *pre;   //ǰ��������ά����С����
	long priority;  //���ȼ�
	long data;    //��������
	long same;     // �м�����ͬ��Ԫ��
	long size;     //size��ά������������
	long rand_priority() //���Ż�����������ɣ���˵�����ظ�����
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
	void maintain() //  ά��size��
	{
		size = same;
		if (left != NULL)
			size += left->size;
		if (right != NULL)
			size += right->size;
	}
};
class Treap //�����棬�ǵݹ飬֧���ظ�Ԫ��
{
protected:
	Treap_node *root;  //���ڵ�
	long node_sum;   //���нڵ�����
	Treap_node* minimun(Treap_node *x)  //̽Ѱ��xΪ������Сֵ
	{
		while (x->left != NULL)   //һֱ������
			x = x->left;
		return x;
	}
	Treap_node* maxmun(Treap_node *x)  //̽Ѱ��xΪ�������ֵ
	{
		while (x->right != NULL)  //һֱ������
			x = x->right;
		return x;
	}
	Treap_node *successor(Treap_node *x)   //���нڵ�ĺ�̣�������x����Сֵ
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
	Treap_node *predecessor(Treap_node *x)   //ǰ������С��x�����ֵ  ��̻��������ok
	{
		if (x->left != NULL)
			return maxmun(x->left);     //����������
		Treap_node *y = x->pre;
		while (y != NULL && x == y->left)   //�������Ҳ��������Ҹ��ڵ�
		{
			x = y;
			y = y->pre;
		}
		return y;
	}
	void rotate_left(Treap_node *part) //����
	{
		Treap_node *father = part->pre;
		if (father->pre->left == father)
			father->pre->left = part;
		else
			father->pre->right = part;
		part->pre = father->pre;
		if (part->left != NULL)
			part->left->pre = father; //�����������ͬԪ��ʱ�ṹ��ը��
		father->pre = part;
		father->right = part->left;
		part->left = father;
		part->left->maintain();   //ͬʱά��size��
		part->maintain();
		father->maintain();
	}
	void rotate_right(Treap_node *part)  //����,��������������,����ճ���ķ����ok
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
	void min_heapify(Treap_node *part) //ά����С��������� ,����ʵ��
	{
		while (part->pre != NULL && part->pre->priority > part->priority)  //����ϸ�ά������
		{
			Treap_node *father = part->pre;
			if (part->pre->data > part->data) //����
				rotate_right(part);
			else    //����,��������������,����ճ���ķ����ok
				rotate_left(part);
		}
	}
public:
	void Delete(long x)   //ɾ���ڵ㣬���ܽڵ�治����
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
				while (find_p->right != NULL && find_p->left != NULL)   //�����нڵ������ת��һ��Ϊ����ɾ��
				{
					if (find_p->left->priority < find_p->right->priority)
						rotate_right(find_p->left);
					else
						rotate_left(find_p->right);
					find_p->maintain();
				}
				Treap_node *father = find_p->pre;
				if (find_p->left == NULL && find_p->right == NULL)  //�жϺ�ɾ���ڵ㣬����ճ���ķ���
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
	Treap()//Ԥ���ڵ�root ������ȼ�
	{
		root = new Treap_node(NULL, NULL, NULL, -10000000);
		root->same = 100000000;
		for (int i = 0; i < 233; ++i) // �������������������
			root->rand_priority();
		root->priority = -1;
		node_sum = 0;
	};
	long lower_bound(long x)   //�����ڵĽڵ��Ȳ�����ɾ����ǰ����������һ�����ø�
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
	long upper_bound(long x) //ͬlower_bound
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
	void insert(long data)//ͬ����������
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
		min_heapify(node); //ά����С����
	}
	Treap_node *find_x(long x)//Ѱ��keyΪx�Ľڵ�
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
	long kth(long k)  //Ĭ��������Ϊ����k��Ԫ��
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
	long kth_2(long k)  //Ĭ��������Ϊ����kСԪ��
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
	long rank(long x)   //��ѯ����x������,����
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
	long rank_2(long x)   //��ѯ����x������,����
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
