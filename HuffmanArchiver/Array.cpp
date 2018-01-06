#include "Array.h"

namespace spaceArray
{
	template <typename S>
	Array<S>::Array()
	{
		this->arr = nullptr;
		this->size = 0;
		this->real_size = 0;
	}

	template <typename S>
	Array<S>::~Array()
	{
		for (register size_t i = 0; i < this->real_size; i++) delete arr[i];
		this->real_size = 0;
		this->size = 0;
	}


	template <typename S>
	S Array<S>:: operator[] (size_t i)
	{
		if (i >= size) throw "Out of range";
		return arr[i];
	}

	template <typename S>
	size_t Array<S>::Size()
	{
		return this->size;
	}

	template <typename S>
	void Array<S>::Push_back(S elem)
	{
		if (size < real_size)
		{
			this->arr[size] = elem;
			size++;
			return;
		}
		S *new_arr = new S[this->real_size << 1];
		for (register size_t i = 0; i < this->real_size; i++)
		{
			new_arr[i] = this->arr[i];
			delete arr[i];
		}
		new_arr[size++] = elem;
		this->real_size <<= 1;
		this->arr = new_arr;
	}

	template <typename S>
	void Array<S>::Pop_back()
	{
		if (!size) return;
		size--;
	}

	template <typename S>
	void Array<S>::ShrinkToFit()
	{
		for (register size_t i = this->size; i < this->real_size; i++)
			delete this->arr[i];
		this->real_size = this->size;
	}
}