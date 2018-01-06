#include "Array.h"

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
	return arr[i]; 
}

template <typename S>
void Array<S>::Size()
{

}

template <typename S>
void Array<S>::Push_back(S i)
{

}

template <typename S>
void Array<S>::Pop_back()
{

}

template <typename S>
void Array<S>::ShrinkToFit()
{

}