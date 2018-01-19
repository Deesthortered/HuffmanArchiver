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
			this->bit_size = bit_sz;
			this->byte_size = (bit_sz / 8) + (bit_sz % 8 ? 1 : 0);
			if (this->byte_size)
				this->arr = new char[this->byte_size];
		}
		BitSet(const BitSet &obj)
		{
			this->bit_size = obj.bit_size;
			this->byte_size = obj.byte_size;
			if (this->byte_size)
			{
				this->arr = new char[this->byte_size];
				for (register size_t i = 0; i < this->byte_size; i++)
					this->arr[i] = obj.arr[i];
			}
		}
		~BitSet()
		{
			if (this->byte_size) delete[] this->arr;
			this->byte_size = 0;
			this->bit_size = 0;
		}

		BitSet& operator=(BitSet &obj)
		{
			if (this->byte_size) delete[] this->arr;
			this->bit_size = obj.bit_size;
			this->byte_size = obj.byte_size;
			if (this->byte_size)
			{
				this->arr = new char[this->byte_size];
				for (register size_t i = 0; i < this->byte_size; i++)
					this->arr[i] = obj.arr[i];
			}
			return *this;
		}

		void Resize(size_t bit_sz)
		{
			if (bit_sz <= (this->byte_size << 3)) // тут учитывается ноль
			{
				this->bit_size = bit_sz;
				return;
			}
			size_t byte_sz = (bit_sz >> 3) + ((bit_sz % 8) ? 1 : 0);
			if (this->byte_size)
			{
				char *new_arr = new char[byte_sz];
				for (register size_t i = 0; i < this->byte_size; i++)
					new_arr[i] = this->arr[i];
				delete[] this->arr;
				this->arr = new_arr;
				this->byte_size = byte_sz;
				this->bit_size = bit_sz;
				return;
			}
			this->arr = new char[byte_sz];
			this->byte_size = byte_sz;
			this->bit_size = bit_sz;
		}
		void Reserve(size_t bit_sz)
		{
			if (bit_sz <= (this->byte_size << 3)) return;
			size_t byte_sz = (bit_sz >> 3) + ((bit_sz % 8) ? 1 : 0);
			if (this->byte_size)
			{
				char *new_arr = new char[byte_sz];
				for (register size_t i = 0; i < this->byte_size; i++)
					new_arr[i] = this->arr[i];
				delete[] this->arr;
				this->arr = new_arr;
				this->byte_size = byte_sz;
				return;
			}
			this->arr = new char[byte_sz];
			this->byte_size = byte_sz;
		}
		void ShrinkToFit()
		{
			size_t byte_sz = (this->bit_size >> 3) + ((this->bit_size % 8) ? 1 : 0);
			if (this->byte_size == byte_sz) return;
			if (byte_sz)
			{
				char *new_arr = new char[byte_sz];
				for (register size_t i = 0; i < byte_sz; i++)
					new_arr[i] = this->arr[i];
				delete[] this->arr;
				this->arr = new_arr;
				this->byte_size = byte_sz;
				return;
			}
			delete[] this->arr;
			this->byte_size = 0;
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
			if (iter > this->bit_size) throw "Out of memory";
			size_t i1 = iter >> 3;
			size_t i2 = iter - ((iter >> 3) << 3);
			return this->arr[i1] & (1 << (7 - i2));
		}
		void SetValue(size_t iter, bool val)
		{
			if (iter >= this->bit_size) return;
			size_t i1 = iter >> 3;
			size_t i2 = iter - ((iter >> 3) << 3);
			if (val) this->arr[i1] |= (1 << (7 - i2));
			else this->arr[i1] &= (-2 << (7 - i2));
		}

		void PushBack(bool val)
		{
			if (!this->byte_size)
			{
				this->byte_size = 1;
				this->bit_size = 1;
				this->arr = new char[1];
				SetValue(0, val);
				return;
			}
			if (this->bit_size < (this->byte_size << 3))
			{
				this->bit_size++;
				SetValue(this->bit_size-1, val);
				return;
			}
			char *new_arr = new char[(this->byte_size << 1)];
			for (register size_t i = 0; i < this->byte_size; i++)
				new_arr[i] = this->arr[i];
			delete[] this->arr;
			this->arr = new_arr;
			this->byte_size <<= 1;
			this->bit_size++;
			SetValue(this->bit_size-1, val);
		}
		void PopBack()
		{
			if (!this->bit_size) return;
			this->bit_size--;
		}

		void GetMemory(char *buff)
		{
			for (register size_t i = 0; i < this->byte_size; i++)
				buff[i] = this->arr[i];
		}
		void SetMemory(char *buff, size_t bit_sz)
		{
			if (this->byte_size) delete[] this->arr;
			this->bit_size = bit_sz;
			this->byte_size = (bit_sz >> 3) + ((bit_sz % 8) ? 1 : 0);
			this->arr = new char[this->byte_size];
			for (register size_t i = 0; i < this->byte_size; i++)
				this->arr[i] = buff[i];
		}
		char GetFirstByte()
		{
			return this->arr[0];
		}
		void ConcatSet(BitSet bs)
		{
			size_t byte_sz = (this->bit_size >> 3) + ((this->bit_size - ((this->bit_size >> 3) << 3)) ? 1 : 0);
			size_t d = (byte_sz << 3) - this->bit_size;
			if (bs.bit_size <= d)
			{
				this->arr[byte_sz - 1] &= ~((1 << d) - 1);
				this->arr[byte_sz-1] |= ((((-128 >> (bs.bit_size - 1)) & bs.arr[0]) >> (8 - d)) & ((1 << d) - 1));
				this->bit_size += bs.bit_size;
				return;
			}
			this->Reserve(this->bit_size + bs.bit_size);
			if (d)
			{
				this->arr[byte_sz - 1] &= ~((1 << d) - 1);
				this->arr[byte_sz - 1] |= ((((-128 >> (d - 1)) & bs.arr[0]) >> (8 - d)) & ((1 << d) - 1));
				bs.MoveLeft(d);
				this->bit_size += d;
			}
			size_t byte_sz2 = (bs.bit_size >> 3) + ((bs.bit_size - ((bs.bit_size >> 3) << 3)) ? 1 : 0);
			for (register size_t i = 0; i < byte_sz2; i++)
				this->arr[byte_sz + i] = bs.arr[i];
			this->bit_size += bs.bit_size;
		}
		void MoveLeft(size_t n)
		{
			if (!n) return;
			if (n >= this->bit_size)
			{
				this->Resize(0);
				return;
			}
			size_t i1 = n >> 3;
			size_t i2 = n - ((n >> 3) << 3);
			if (i1)
			{
				for (register size_t i = 0; i < this->byte_size - i1; i++)
					this->arr[i] = this->arr[i + i1];
				this->bit_size -= (i1 << 3);
			}
			if (i2)
			{
				for (register size_t i = 0; i < this->byte_size - 1; i++)
				{
					this->arr[i] <<= i2;
					char k = (1 << i2) - 1, c = k << (8 - i2);
					c &= this->arr[i + 1];
					c >>= (8 - i2);
					c &= k; 
					this->arr[i] |= c;
				}
				this->arr[this->byte_size - 1] <<= i2;
				this->bit_size -= i2;
			}
		}

		friend bool operator< (BitSet &a, BitSet &b) 
		{
			if (a.bit_size < b.bit_size) return true;
			if (a.bit_size > b.bit_size) return false;
			size_t m = (a.bit_size < b.bit_size ? a.bit_size : b.bit_size);
			for (register size_t i = 0; i < m; i++)
			{
				if (a.GetValue(i) && !b.GetValue(i)) return false;
				if (!a.GetValue(i) && b.GetValue(i)) return true;
			}
			return false;
		}
		friend bool operator> (BitSet &a, BitSet &b)
		{
			if (a.bit_size > b.bit_size) return true;
			if (a.bit_size < b.bit_size) return false;
			size_t m = (a.bit_size < b.bit_size ? a.bit_size : b.bit_size);
			for (register size_t i = 0; i < m; i++)
			{
				if (!a.GetValue(i) && b.GetValue(i)) return false;
				if (a.GetValue(i) && !b.GetValue(i)) return true;
			}
			if (a.bit_size > b.bit_size) return true;
			return false;
		}
		bool operator == (BitSet &obj)
		{
			if (this->bit_size != obj.bit_size) return false;
			for (register size_t i = 0; i < this->bit_size; i++)
				if (this->GetValue(i) != obj.GetValue(i)) return false;
			return true;
		}
	};
}