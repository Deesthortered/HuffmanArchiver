#pragma once
namespace spaceArray
{
	template <typename S>
	class Array
	{
		S *arr;
		size_t size;
		size_t real_size;
	public:
		Array();
		~Array();

		S operator[] (size_t);
		size_t Size();
		void Push_back(S);
		void Pop_back();
		void ShrinkToFit();
	};
}