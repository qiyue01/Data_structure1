#include "pch.h"
#include <iostream>
#include <set>
#include <tuple>
using namespace std;
namespace ODT
{
	/*
	* д����ʱ�м����з��Ҷ˵㣬���з���˵��п��ܵ��µ�����ʧЧ
	* ������� Ӧ�ø����ݽṹ���������串�ǲ���
	* ��������¸��ӶȲ��б�֤������һ����ô�����˷������������
	* ���߸��Ӷȱ����ߺ�
	* д�������������������䴦��tuple����׷��Ԫ��
	*/
	typedef tuple<int, int, int> Tuple;
	set<Tuple> tree;
	set<Tuple>::iterator split(int pos)
	{
		Tuple p = make_tuple(pos, -1, 0);
		auto it1 = tree.lower_bound(p);
		if (it1 != tree.end() && get<0>(*it1) == pos)
			return it1;
		--it1;
		int L = get<0>(*it1), R = get<1>(*it1);
		int V = get<2>(*it1);
		tree.erase(it1);
		tree.insert(make_tuple(L, pos - 1, V));
		return tree.insert(make_tuple(pos, R, V)).first;
	}
	void assign(int L, int R, int k)//���串�� �ؼ�����
	{
		auto it2 = split(R + 1);
		auto it1 = split(L);
		tree.erase(it1, it2);
		tree.insert(make_tuple(L, R, k));
	}
	int query_sum(int L, int R)
	{
		auto it2 = split(R + 1);
		auto it1 = split(L);
		int ans = 0;
		while (it1 != it2)
		{
			ans += get<2>(*it1)*(get<1>(*it1) - get<0>(*it1) + 1);
			it1++;
		}
		return ans;
	}
	void insert(int L, int R, int k)
	{
		tree.insert(make_tuple(L, R, k));
	}
}
using namespace ODT;