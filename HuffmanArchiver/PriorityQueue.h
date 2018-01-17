#pragma once
#include "Array.h"

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
		PriorityQueue()
		{
			this->arr.~Array();
		}
		~PriorityQueue()
		{
			this->arr.~Array();
		}

		bool IsEmpty()
		{
			return (this->arr.IsEmpty() ? true : false);
		}
		void Push(T1 data, T2 key)
		{
			int i = this->arr.Size();
			this->arr.Push_back(Pair(data, key));
			while (i && this->arr[i].key < this->arr[(i - 1) >> 1].key)
			{
				swap(this->arr[(i - 1) >> 1], this->arr[i]);
				i = (i - 1) >> 1;
			}
		}
		void Pop()
		{
			if (IsEmpty()) return;
			size_t sz = this->arr.Size() - 1;
			size_t i = 0;

			this->arr[0] = this->arr[sz];
			this->arr.Pop_back();

			while ((i << 1) + 1 < sz)
			{
				if ((i << 1) + 2 < sz)
				{
					if (this->arr[(i << 1) + 1].key < this->arr[(i << 1) + 2].key)
					{
						if (this->arr[(i << 1) + 1].key < this->arr[i].key)
						{
							swap(this->arr[(i << 1) + 1], this->arr[i]);
							i = (i << 1) + 1;
						}
						else break;
					}
					else
					{
						if (this->arr[(i << 1) + 2].key < this->arr[i].key)
						{
							swap(this->arr[(i << 1) + 2], this->arr[i]);
							i = (i << 1) + 2;
						}
						else break;
					}
				}
				else if (this->arr[(i << 1) + 1].key < this->arr[i].key)
				{
					swap(this->arr[(i << 1) + 1], this->arr[i]);
					i = (i << 1) + 1;
				}
				else break;
			}
		}
		T1	 ExtractMin()
		{
			if (IsEmpty()) throw "Empty queue";
			return this->arr[0].data;
		}
		void ShrinkToFit()
		{
			this->arr.ShrinkToFit();
		}
	};
}