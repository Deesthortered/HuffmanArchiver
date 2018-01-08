#pragma once

namespace spaceArray
{
	template <class S>
	class Array
	{
		S *arr;
		size_t size;
		size_t real_size;
	public:
		Array();
		~Array();

		S operator[] (size_t);
		void SetValue(size_t, S);
		size_t Size();
		void Push_back(S);
		void Pop_back();
		void Reserve(size_t);
		void ShrinkToFit();
	};
}