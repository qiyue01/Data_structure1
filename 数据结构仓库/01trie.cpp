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
#include<array>
#include<memory.h>
#include<functional>
using namespace std;
namespace trie01
{
	const int sigma = 31;
	struct node
	{
		node *child[2];
		int times;
		void  clear()
		{
			times = 0;
			child[0] = child[1] = NULL;
		}
	};
	struct trie
	{
		node pool[3600000];
		node *root, *cur = pool;
		node *new_()
		{
			cur->clear();
			return cur++;
		}
		void init()
		{
			cur = pool;
			root = new_();
		}
		bitset<sigma + 1> trans;
		void insert(long long a)
		{
			node *found = root;
			trans = a;
			for (int i = sigma; i >= 0; --i)
			{
				found->times++;
				if (found->child[trans[i]] == NULL)
				{
					found->child[trans[i]] = new_();
					found = found->child[trans[i]];
				}
				else
					found = found->child[trans[i]];
			}
			found->times++;
		}
	}t1, t2;
}
using namespace trie01;