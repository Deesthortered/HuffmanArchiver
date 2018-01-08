#include "Array.h"

namespace spaceArray
{
	template <class S>
	Array<S>::Array()
	{
		this->arr = nullptr;
		this->size = 0;
		this->real_size = 0;
	}

	template <class S>
	Array<S>::~Array()
	{
		S *a = arr;
		delete[] a;
		this->arr = nullptr;
		this->real_size = 0;
		this->size = 0;
	}

	template <class S>
	S Array<S>::operator[] (size_t i)
	{
		if (i >= size) throw "Out of range";
		return arr[i];
	}
	
	template <class S>
	void Array<S>::SetValue(size_t i, S val)
	{
		this->arr[i] = val;
	}

	template <class S>
	size_t Array<S>::Size()
	{
		return this->size;
	}

	template <class S>
	void Array<S>::Push_back(S elem)
	{
		if (size < real_size)
		{
			this->arr[size] = elem;
			size++;
			return;
		}
		S *new_arr = new S[(this->real_size ? this->real_size << 1 : 1)];
		for (register size_t i = 0; i < this->real_size; i++)
			new_arr[i] = this->arr[i];
		delete this->arr;
		new_arr[size++] = elem;
		this->real_size = (this->real_size ? this->real_size << 1 : 1);
		this->arr = new_arr;
	}

	template <class S>
	void Array<S>::Pop_back()
	{
		if (!size) return;
		size--;
	}

	template<class S>
	void Array<S>::Reserve(size_t s)
	{
		if (s <= this->real_size) return;
		S *new_arr = new S[s];
		for (register size_t i = 0; i < this->size; i++)
			new_arr[i] = this->arr[i];
		this->real_size = s;
		delete[] this->arr;
		this->arr = new_arr;
	}

	template <class S>
	void Array<S>::ShrinkToFit()
	{
		S *new_arr = new S[size];
		for (register size_t i = 0; i < this->size; i++)
			new_arr[i] = this->arr[i];
		this->real_size = this->size;
		delete[] this->arr;
		this->arr = new_arr;
	}
}