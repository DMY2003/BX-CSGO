#pragma once

#include "SDK.h"

template<typename T, size_t S>
class CircularList
{
private:
	std::vector<T> data;

public:
	CircularList<T, S>()
	{
		data.clear();
	}

	void clear()
	{
		data.clear();
	}

	void insert(T v)
	{
		if (data.size() == S - 1)
			data.pop_back();

		data.insert(data.begin(), v);
	}

	T at(int i)
	{
		return data.at(i);
	}

	bool valid(int i)
	{
		return i < (int)data.size();
	}

	int max_size()
	{
		return (int)S;
	}

	int size()
	{
		return (int)data.size();
	}

	int last()
	{
		return max(0, (int)data.size() - 1);
	}
};
