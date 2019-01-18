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
namespace hash_map
{
	const int N2 = 5000;  //hashÍ°µÄ´óÐ¡
	list<long long> hash_map3[N2], hash_map2[N2];
	void insert(long long hash, list<long long> *hash_map1)
	{
		hash_map1[hash%N2].push_front(hash);
	}
	bool find_hash(long long hash, list<long long> *hash_map1)
	{
		if (hash_map1[hash%N2].empty())
			return false;
		else
			for (auto &s : hash_map1[hash%N2])
				if (s == hash)
					return true;
		return false;
	}
	void clear(list<long long> *hash_map1)
	{
		for (int i = 0; i < N2; ++i)
			hash_map1[i].clear();
	}
}