#pragma once

namespace spaceBitSet
{
	class BitSet
	{
		char *arr;
		size_t bit_size;
		size_t byte_size;

	public:
		BitSet()
		{
			this->arr = nullptr;
			this->bit_size = 0;
			this->byte_size = 0;
		}
		~BitSet()
		{
			delete[] this->arr;
			this->bit_size = 0;
			this->byte_size = 0;
		}

		void Resize(size_t bit_sz)
		{
			size_t byte_sz = bit_sz / 8 + (bit_sz % 8 ? 1 : 0);
			char *new_arr = new char[byte_sz];
			for (size_t i = 0; i < this->byte_size; i++)
				new_arr[i] = this->arr[i];
			delete[] this->arr;
			this->arr = new_arr;
			this->byte_size = byte_sz;
			this->bit_size = bit_sz;
		}
		void Reserve(size_t bit_sz)
		{
			size_t byte_sz = bit_sz / 8 + (bit_sz % 8 ? 1 : 0);
			char *new_arr = new char[byte_sz];
			for (size_t i = 0; i < this->byte_size; i++)
				new_arr[i] = this->arr[i];
			delete[] this->arr;
			this->arr = new_arr;
			this->byte_size = byte_sz;
		}
		void ShrinkToFit()
		{

		}

		bool GetValue(size_t iter)
		{
			if (this->bit_size <= iter) throw "Out of range";
			size_t i1 = iter / 8;
			size_t i2 = iter % 8;
			char c = 1;
			while (i2) { c <<= 1; i2--; }
			return this->arr[i1] & c;
		}
		void SetValue(size_t iter, bool val)
		{
			if (this->bit_size <= iter) return;
			size_t i1 = iter / 8;
			size_t i2 = iter % 8;
			char c = 1;
			while (i2) { c <<= 1; i2--; }
			if (val)
			{
				this->arr[i1] = this->arr[i1] | c;
			}
			else
			{
				c = ~c;
				this->arr[i1] = this->arr[i1] & c;
			}
		}

		
		const bool operator< (const BitSet &a, const BitSet &b) 
		{

		}

	};
}