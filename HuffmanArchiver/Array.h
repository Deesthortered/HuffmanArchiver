#pragma once
template <typename S>
class Array
{
	S *arr;
	size_t size;
	size_t real_size;
public:
	Array();
	~Array();

	S operator[] (size_t i);
	size_t Size();
	void Push_back(S i);
	void Pop_back();
	void ShrinkToFit();
};

