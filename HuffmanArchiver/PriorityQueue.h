#pragma once
#include "Array.cpp"

namespace spacePriorityQueue
{
	template <class T1, class T2>
	class PriorityQueue
	{
		struct Pair
		{
			T1 data;
			T2 key;
			Pair() = default;
			Pair(T1 _data, T2 _key) : data(_data), key(_key) {}
		};
		spaceArray::Array<Pair> arr;

	public:
		PriorityQueue();
		~PriorityQueue();

		bool IsEmpty();
		void Push(T1, T2);
		void Pop();
		T1 ExtractMin();
		void ShrinkToFit();
	};
}