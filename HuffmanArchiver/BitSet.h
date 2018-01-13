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
		BitSet(size_t bit_sz)
		{
			size_t byte_sz = bit_sz / 8 + (bit_sz % 8 ? 1 : 0);
			this->arr = new char[byte_size];
			this->byte_size = byte_sz;
			this->bit_size = bit_sz;
		}
		BitSet(const BitSet &obj)
		{
			this->byte_size = obj.byte_size;
			this->bit_size = obj.bit_size;
			this->arr = new char[this->byte_size];
			for (size_t i = 0; i < this->byte_size; i++)
				this->arr[i] = obj.arr[i];
		}
		~BitSet()
		{
			if (!this->byte_size) return;
			delete[] this->arr;
			this->bit_size = 0;
			this->byte_size = 0;
		}

		void Resize(size_t bit_sz)
		{
			if (!bit_sz)
			{
				this->~BitSet();
				return;
			}
			if (this->byte_size << 3 > bit_sz)
			{
				this->bit_size = bit_sz;
				return;
			}
			size_t byte_sz = (bit_sz >> 3) + ( (bit_sz - ((bit_sz >> 3) << 3)) ? 1 : 0);
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
			if (bit_sz < this->bit_size) return;
			size_t byte_sz = (bit_sz >> 3) + ((bit_sz - ((bit_sz >> 3) << 3)) ? 1 : 0);
			char *new_arr = new char[byte_sz];
			for (size_t i = 0; i < this->byte_size; i++)
				new_arr[i] = this->arr[i];
			delete[] this->arr;
			this->arr = new_arr;
			this->byte_size = byte_sz;
		}
		void ShrinkToFit()
		{
			size_t byte_sz = (this->bit_size >> 3) + ( (this->bit_size - ((this->bit_size >> 3) << 3)) ? 1 : 0);
			char *new_arr = new char[byte_sz];
			for (size_t i = 0; i < byte_sz; i++)
				new_arr[i] = this->arr[i];
			delete[] this->arr;
			this->arr = new_arr;
			this->byte_size = byte_sz;
		}

		size_t BitSize()
		{
			return this->bit_size;
		}
		size_t ByteSize()
		{
			return this->byte_size;
		}

		bool GetValue(size_t iter)
		{
			if (this->bit_size <= iter) throw "Out of range";
			size_t i1 = iter >> 3;
			size_t i2 = iter - ((iter >> 3) << 3);
			char c = 1;
			while (i2) { c <<= 1; i2--; }
			return this->arr[i1] & c;
		}
		void SetValue(size_t iter, bool val)
		{
			if (this->bit_size <= iter) return;
			size_t i1 = iter >> 3;
			size_t i2 = iter - ((iter >> 3) << 3);
			char c = 1;
			while (i2) { c <<= 1; i2--; }
			if (val) this->arr[i1] = this->arr[i1] | c;
			else
			{
				c = ~c;
				this->arr[i1] = this->arr[i1] & c;
			}
		}

		void PushBack(bool val)
		{
			if (!this->arr)
			{
				this->arr = new char[1];
				this->byte_size = 1;
				SetValue(this->bit_size++, val);
				return;
			}
			if (this->bit_size < this->byte_size << 3)
			{
				SetValue(this->bit_size++, val);
				return;
			}
			char* new_arr = new char[this->byte_size << 1];
			for (size_t i = 0; i < this->byte_size; i++)
				new_arr[i] = this->arr[i];
			delete[] this->arr;
			this->arr = new_arr;
			this->byte_size <<= 1;
			SetValue(this->bit_size++, val);
		}
		void PopBack()
		{
			if (!this->bit_size) return;
			this->bit_size--;
		}

		void GetMemory(char *buff)
		{
			for (size_t i = 0; i < this->byte_size; i++)
				buff[i] = this->arr[i];
		}
		void ConcatSet(BitSet &bs)
		{
			size_t n = this->bit_size + bs.bit_size;
			if (n < (this->byte_size << 3))
			{
				for (size_t i = 0; i < bs.bit_size; i++)
					this->arr[this->bit_size + i] = bs.arr[i];
				this->bit_size = n;
				return;
			}
			char *r = new char[n];
			for (size_t i = 0; i < this->bit_size; i++)
				r[i] = this->arr[i];
			for (size_t i = 0; i < bs.bit_size; i++)
				r[this->bit_size + i] = bs.arr[i];
			delete[] this->arr;
			this->arr = r;
			this->bit_size = n;
			this->byte_size = (n >> 3) + (n - ((n >> 3) << 3) ? 1 : 0);
		}
		void MoveLeft(size_t n)
		{
			if (!n) return;
			if (n >= this->bit_size)
			{
				memset(this->arr, 0, this->byte_size);
				this->bit_size = 0;
				return;
			}
			size_t a = n >> 3;
			size_t b = n - (a << 3);
			if (a && b)
			{
				MoveLeft(a << 3);
				MoveLeft(b);
				return;
			}	
			if (a && !b)
			{
				size_t k = n >> 3;
				for (size_t i = 0; i < this->byte_size - k; i++)
					this->arr[i] = this->arr[i + k];
				memset(this->arr + this->byte_size - k, 0, k);
				this->bit_size -= n;
				return;
			}
			for (size_t i = 0; i < this->byte_size - 1; i++)
				this->arr[i] = (this->arr[i] >> b) & (127 >> (b-1)) | (((127 >> (8 - b - 1)) & this->arr[i + 1]) << (8 - b));
			this->bit_size -= n;
		}

		friend bool operator< (BitSet &a, BitSet &b) 
		{
			size_t sz = (a.bit_size < b.bit_size ? a.bit_size : b.bit_size);
			for (size_t i = 0; i < sz; i++)
			{
				if (a.GetValue(i) && !b.GetValue(i)) return false;
				if (!a.GetValue(i) && b.GetValue(i)) return true;
			}
			if (b.bit_size > a.bit_size) return true;
			return false;
		}
		BitSet& operator=(BitSet &obj)
		{
			this->byte_size = obj.byte_size;
			this->bit_size = obj.bit_size;
			this->arr = new char[this->byte_size];
			for (size_t i = 0; i < this->byte_size; i++)
				this->arr[i] = obj.arr[i];
			return *this;
		}

};
}