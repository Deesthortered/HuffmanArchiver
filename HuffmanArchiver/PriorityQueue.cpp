#include "PriorityQueue.h"

namespace spacePriorityQueue
{
	template <class T1, class T2>
	PriorityQueue<T1, T2>::PriorityQueue()
	{
		this->arr = spaceArray::Array<Pair>();
	}

	template <class T1, class T2>
	PriorityQueue<T1, T2>::~PriorityQueue()
	{
		this->arr.~Array();
	}


	template <class T1, class T2>
	bool PriorityQueue<T1, T2>::IsEmpty()
	{
		return (this->arr.Size() ? false : true);
	}

	template <class T1, class T2>
	void PriorityQueue<T1, T2>::Push(T1 data, T2 key)
	{
		int i = this->arr.Size();
		this->arr.Push_back(Pair(data, key));
		while (i && this->arr[i].key < this->arr[(i - 1) >> 1].key)
		{
			Pair a = this->arr[(i - 1) >> 1];
			this->arr.SetValue((i - 1) >> 1, this->arr[i]);
			this->arr.SetValue(i, a);
			i = (i - 1) >> 1;
		}
	}

	template <class T1, class T2>
	void PriorityQueue<T1, T2>::Pop()
	{
		if (IsEmpty()) return;
		size_t sz = this->arr.Size() - 1;
		size_t i = 0;

		this->arr.SetValue(0, this->arr[sz]);
		this->arr.Pop_back();

		while ((i << 1) + 1 < sz)
		{
			if ((i << 1) + 2 < sz)
			{
				if (this->arr[(i << 1) + 1].key < this->arr[(i << 1) + 2].key)
				{
					if (this->arr[(i << 1) + 1].key < this->arr[i].key)
					{
						Pair a = this->arr[(i << 1) + 1];
						this->arr.SetValue((i << 1) + 1, this->arr[i]);
						this->arr.SetValue(i, a);
						i = (i << 1) + 1;
					}
				}
				else
				{
					if (this->arr[(i << 1) + 2].key < this->arr[i].key)
					{
						Pair a = this->arr[(i << 1) + 2];
						this->arr.SetValue((i << 1) + 2, this->arr[i]);
						this->arr.SetValue(i, a);
						i = (i << 1) + 2;
					}
				}
			}
			else if (this->arr[(i << 1) + 1].key < this->arr[i].key)
			{
				Pair a = this->arr[(i << 1) + 1];
				this->arr.SetValue((i << 1) + 1, this->arr[i]);
				this->arr.SetValue(i, a);
				i = (i << 1) + 1;
			}
			else break;
		}
	}

	template <class T1, class T2>
	T1 PriorityQueue<T1, T2>::ExtractMin()
	{
		if (IsEmpty()) throw "Empty queue";
		return this->arr[0].data;
	}

	template <class T1, class T2>
	void PriorityQueue<T1, T2>::ShrinkToFit()
	{
		this->arr.ShrinkToFit();
	}
}