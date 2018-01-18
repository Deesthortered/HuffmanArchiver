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
			if (byte_sz) this->arr = new char[byte_size];
			this->byte_size = byte_sz;
			this->bit_size = bit_sz;
		}
		BitSet(const BitSet &obj)
		{
			this->byte_size = obj.byte_size;
			this->bit_size = obj.bit_size;
			if (obj.byte_size)
			{
				this->arr = new char[this->byte_size];
				for (size_t i = 0; i < this->byte_size; i++)
					this->arr[i] = obj.arr[i];
			}
		}
		~BitSet()
		{
			if (this->byte_size) delete[] this->arr;
			this->arr = nullptr;
			this->bit_size = 0;
			this->byte_size = 0;
		}

		void Resize(size_t bit_sz)
		{
			if ((this->byte_size << 3) > bit_sz)
			{
				this->bit_size = bit_sz;
				return;
			}
			size_t byte_sz = (bit_sz >> 3) + ( (bit_sz - ((bit_sz >> 3) << 3)) ? 1 : 0);
			if (byte_sz <= this->byte_size)
			{
				this->bit_size = bit_sz;
				return;
			}
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
			if (bit_sz < (this->byte_size << 3)) return;
			size_t byte_sz = (bit_sz >> 3) + ((bit_sz - ((bit_sz >> 3) << 3)) ? 1 : 0);
			if (byte_sz <= this->byte_size) return;
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
			if (byte_sz == this->byte_size) return;
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
			return this->arr[i1] & (1 << i2);
		}
		void SetValue(size_t iter, bool val)
		{
			if (this->bit_size <= iter) return;
			size_t i1 = iter >> 3;
			size_t i2 = iter - ((iter >> 3) << 3);
			if (val) this->arr[i1] = this->arr[i1] | (1 << i2);
			else this->arr[i1] = this->arr[i1] & (-2 << i2);
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
		void SetMemory(char *buff, size_t bit_sz)
		{
			Resize(bit_sz);
			for (size_t i = 0; i < ((bit_sz >> 3) + (bit_sz % 8 ? 1 : 0)); i++)
				this->arr[i] = buff[i];
		}
		char GetFirstByte()
		{
			return this->arr[0];
		}
		void ConcatSet(BitSet &bs)
		{
			if (!bs.bit_size) return;
			size_t d = ((this->byte_size << 3) - this->bit_size) % 8;
			size_t f_end = (this->bit_size >> 3) + (this->bit_size % 8 ? 1 : 0);
			if (bs.bit_size <= d)
			{
				this->arr[f_end - 1] = (this->arr[f_end - 1] & (1 << this->bit_size) - 1) | ((((1 << bs.bit_size) - 1) & bs.arr[0]) << this->bit_size);
				this->bit_size += bs.bit_size;
				return;
			}
			Reserve(this->bit_size + bs.bit_size + 8);
			if (d)
			{
				this->arr[f_end - 1] = (this->arr[f_end - 1] & ((1 << (8 - d)) - 1)) | ((((1 << d) - 1) & bs.arr[0]) << (8 - d));
				bs.MoveLeft(d);
				this->bit_size += d;
			}
			for (size_t i = 0; i < bs.byte_size - 1; i++)
				this->arr[f_end + i] = bs.arr[i];
			this->arr[f_end + bs.byte_size - 1] = bs.arr[bs.byte_size - 1];
			this->bit_size += bs.bit_size;
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
				for (size_t i = 0; i < this->byte_size - a; i++)
					this->arr[i] = this->arr[i + a];
				memset(this->arr + this->byte_size - a, 0, a);
				this->bit_size -= n;
				return;
			}
			for (size_t i = 0; i < this->byte_size - 1; i++)
				this->arr[i] = (this->arr[i] >> b) & (127 >> (b - 1)) | (((127 >> (8 - b - 1)) & this->arr[i + 1]) << (8 - b));
			this->bit_size -= n;
		}

		BitSet& operator=(BitSet &obj)
		{
			if (this->byte_size) delete[] this->arr;
			this->byte_size = obj.byte_size;
			this->bit_size = obj.bit_size;
			if (obj.byte_size)
			{
				this->arr = new char[this->byte_size];
				for (size_t i = 0; i < this->byte_size; i++)
					this->arr[i] = obj.arr[i];
			}
			return *this;
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
		friend bool operator> (BitSet &a, BitSet &b)
		{
			size_t sz = (a.bit_size < b.bit_size ? a.bit_size : b.bit_size);
			for (size_t i = 0; i < sz; i++)
			{
				if (a.GetValue(i) && !b.GetValue(i)) return true;
				if (!a.GetValue(i) && b.GetValue(i)) return false;
			}
			if (b.bit_size > a.bit_size) return false;
			return false;
		}
		bool operator == (BitSet &obj)
		{
			if (this->bit_size != obj.bit_size) return false;
			for (size_t i = 0; i < this->bit_size; i++)
				if (this->GetValue(i) != obj.GetValue(i)) return false;
			return true;
		}
	};
}