#pragma once
#include <fstream>
#include <iostream>
#include "AVL_Tree.h"
#include "BitSet.h"
using namespace std;

namespace spaceArchiver
{
	class Coder
	{
		struct HuffTrNode
		{
			string data;
			size_t cnt;
			HuffTrNode *left;
			HuffTrNode *right;
			HuffTrNode() = default;
			HuffTrNode(string _data, size_t _cnt, HuffTrNode *_left, HuffTrNode *_right) : data(_data), cnt(_cnt), left(_left), right(_right) {}
		};
		struct DataNode
		{
			string data;
			spaceBitSet::BitSet bs;
			size_t cnt;

			DataNode()
			{
				this->data.clear();
				bs.~BitSet();
				this->cnt = 0;
			}
			DataNode(string _data) : data(_data), cnt(1) { bs.~BitSet(); }
			DataNode(string _data, spaceBitSet::BitSet _bs) : data(_data), bs(_bs), cnt(1) {}
			~DataNode()
			{
				data.clear();
				bs.~BitSet();
			}
			friend bool operator< (DataNode &a, DataNode &b)
			{
				return (a.data < b.data);
			}
			friend bool operator> (DataNode &a, DataNode &b)
			{
				return (a.data > b.data);
			}
			bool operator == (DataNode &obj)
			{
				return (this->data == obj.data);
			}
			bool operator != (DataNode &obj)
			{
				return (this->data != obj.data);
			}
			DataNode& operator=(DataNode &obj)
			{
				this->data = obj.data;
				this->cnt = obj.cnt;
				this->bs = obj.bs;
				return *this;
			}
			DataNode& operator++()
			{
				this->cnt++;
				return *this;
			}
		};

		string path_from;
		string path_to;
		unsigned char cnt_byte;
		bool ready;

	public:
		Coder()
		{
			this->path_from.clear();
			this->path_to.clear();
			this->cnt_byte = 0;
			this->ready = false;
		}
		~Coder()
		{
			this->path_from.clear();
			this->path_to.clear();
			this->cnt_byte = 0;
			this->ready = false;
		}

		bool Ready()
		{
			return this->ready;
		}
		void Initialize(string from, string to, unsigned char cnt)
		{
			if (from.empty() || to.empty()) return;
			fstream f(from);
			if (!f.is_open()) return;
			f.close();
			f.open(to);
			if (f.is_open()) { f.close(); return; }
			this->path_from = from;
			this->path_to = to;
			this->cnt_byte = cnt;
			this->ready = true;
		}
		void Run()
		{
			if (!this->ready) return;
			/// 1 step
			cout << "1/6) Coder initializing" << endl;

			fstream fin(this->path_from, ios::binary | ios::in);
			spaceAVL_Tree::AVL_Tree<DataNode> tree;
			char c; string word; size_t _i = 0; unsigned __int32 cnt_codes = 0;

			cout << "2/6) Reading file and creating avl tree" << endl;

			word.reserve(this->cnt_byte);
			while (fin.read(&c, sizeof(c)))
			{
				word.push_back(c); _i++;
				if (_i >= this->cnt_byte)
				{
					if (!tree.Insert(DataNode(word))) cnt_codes++;
					word.clear();
				_i = 0;
				}
			}
			if (_i)
			{
				for (size_t j = 0; _i < this->cnt_byte - _i; j++) word.push_back('\0');
				tree.Insert(DataNode(word));
				word.clear(); _i = 0;
			}
			fin.close();

			cout << "     Tree is created" << endl;
			cout << "3/6) Create Huffman tree" << endl;
			
			/// 2 step
			spacePriorityQueue::PriorityQueue<HuffTrNode, size_t> q;
			spaceArray::Array<DataNode> arr = tree.ReturnAllVals();
			for (size_t i = 0; i < arr.Size(); i++)
				q.Push(HuffTrNode(arr[i].data, arr[i].cnt, nullptr, nullptr), arr[i].cnt);
			
			/// 3 step
			HuffTrNode *hufftree;
			while (true)
			{
				HuffTrNode *tmp1 = new HuffTrNode();
				HuffTrNode *tmp2 = new HuffTrNode();
				*tmp1 = q.ExtractMin(); q.Pop();
				if (q.IsEmpty())
				{
					hufftree = tmp1;
					delete tmp2;
					break;
				}
				*tmp2 = q.ExtractMin(); 
				q.Pop();
				HuffTrNode *tmp3 = new HuffTrNode("", tmp1->cnt + tmp2->cnt, tmp1, tmp2);
				q.Push(*tmp3, tmp3->cnt);
				delete tmp3;
			}
			q.~PriorityQueue();

			cout << "     Tree is created" << endl;
			cout << "4/6) Create Huffman codes" << endl;
			
			/// 4 step
			spaceBitSet::BitSet bs; bs.Reserve(1);
			unsigned char max_bit_size = Hufffunc(hufftree, bs, tree) - 1;
			unsigned char max_byte_size = (max_bit_size >> 3) + ((max_bit_size - ((max_bit_size >> 3) << 3)) ? 1 : 0);
			hufftree = nullptr;
			bs.~BitSet();

			cout << "     Codes is created" << endl;
			cout << "5/6) Write code-table to file" << endl;

			/// 5 step
			// cnt_codes, max_bit_size, tree, arr
			fstream fout(this->path_to, ios::binary | ios::out);

			fout.write((char*)&cnt_codes, sizeof(cnt_codes));			// unsigned __int32 - 4 byte
			fout.write((char*)&this->cnt_byte, sizeof(this->cnt_byte)); // unsigned char    - 1 byte
			fout.write((char*)&max_byte_size, sizeof(max_byte_size));	// unsigned char    - 1 byte

			size_t sz = arr.Size();
			char *buff = new char[max_byte_size];
			for (size_t i = 0; i < sz; i++)
			{
				for (size_t j = 0; j < this->cnt_byte; j++) 
					fout.write((char*)&arr[i].data[j], sizeof(char));
				fout.write((char*)&max_bit_size, sizeof(max_bit_size));
				arr[i].bs.GetMemory(buff);
				for (size_t j = 0; j < max_byte_size; j++)
					fout.write((char*)&buff, sizeof(char));
			}
			delete[] buff;
			arr.~Array();

			cout << "     Code-table is written" << endl;
			cout << "6/6) Write compressed file" << endl;


			/// 6 step
			size_t bs_size = 2 * (max_bit_size < 8 ? 8 : max_bit_size);
			spaceBitSet::BitSet temp_bs;
			temp_bs.Reserve(bs_size);
			fin.open(this->path_from, ios::binary | ios::in);
			
			_i = 0; word.clear();
			while (fin.read(&c, sizeof(c)))
			{
				word.push_back(c); _i++;
				if (_i >= this->cnt_byte)
				{
					spaceBitSet::BitSet tmp = tree.FindVal(DataNode(word)).bs;
					temp_bs.ConcatSet(tmp);
					word.clear();
					_i = 0;
					if (temp_bs.BitSize() >= 8)
					{
						char c = temp_bs.GetFirstByte();
						fout.write(&c, sizeof(char));
						temp_bs.MoveLeft(8);
					}
				}
			}
			if (_i)
			{
				for (size_t j = 0; _i < this->cnt_byte - _i; j++) word.push_back('\0');
				spaceBitSet::BitSet tmp = tree.FindVal(DataNode(word)).bs;
				temp_bs.ConcatSet(tmp);
				word.clear();
				_i = 0;
				while (temp_bs.BitSize() >= 8)
				{
					char c = temp_bs.GetFirstByte();
					fout.write(&c, sizeof(char));
					temp_bs.MoveLeft(8);
				}
				if (temp_bs.BitSize())
				{
					size_t tmp = 8 - temp_bs.BitSize();
					for (size_t i = 0; i < tmp; i++)  temp_bs.PushBack(false);
					char c = temp_bs.GetFirstByte();
					fout.write(&c, sizeof(char));
				}
			}
			fin.close();
			fout.close();
			tree.~AVL_Tree();
			temp_bs.~BitSet();
			cout << "     File has already written" << endl;
		}

	private:
		unsigned char Hufffunc(HuffTrNode *&node, spaceBitSet::BitSet &bs, spaceAVL_Tree::AVL_Tree<DataNode> &tree)
		{
			if (!node) return 0;
			if (!node->data.empty())
			{
				DataNode a = DataNode(node->data, bs);
				tree.ReplaceVal(DataNode(node->data), a);
			}

			spaceBitSet::BitSet l = bs;
			spaceBitSet::BitSet r = bs;
			l.PushBack(false);
			r.PushBack(true);

			unsigned char a = Hufffunc(node->left, l, tree);
			unsigned char b = Hufffunc(node->right, r, tree);
			delete node;
			return (a < b ? b : a) + 1;
		}
	};
	class Decoder
	{
		struct DataNode
		{
			string original;
			spaceBitSet::BitSet bs;
			DataNode()
			{
				this->original.clear();
				this->bs.~BitSet();
			}
			DataNode(string _original, unsigned char _bit_size, char *bs_memory) : original(_original)
			{
				this->bs.SetMemory(bs_memory, _bit_size);
			}
			DataNode(spaceBitSet::BitSet _bs)
			{
				this->original.clear();
				this->bs = _bs;
			}
			~DataNode()
			{
				original.clear();
				bs.~BitSet();
			}
			friend bool operator< (DataNode &a, DataNode &b)
			{
				return a.bs < b.bs;
			}
			friend bool operator> (DataNode &a, DataNode &b)
			{
				return a.bs > b.bs;
			}
			bool operator == (DataNode &obj)
			{
				return (this->bs == obj.bs);
			}
			bool operator != (DataNode &obj)
			{
				return !(this->bs == obj.bs);
			}
			DataNode& operator=(DataNode &obj)
			{
				this->original = obj.original;
				this->bs = obj.bs;
				return *this;
			}
			DataNode& operator++()
			{
				return *this;
			}
		};
		string path_from;
		string path_to;
		bool ready;

	public:
		Decoder()
		{
			this->path_from.clear();
			this->path_to.clear();
			this->ready = false;
		}
		~Decoder()
		{
			this->path_from.clear();
			this->path_to.clear();
			this->ready = false;
		}
		bool Ready()
		{
			return this->ready;
		}
		void Initialize(string from, string to)
		{
			if (from.empty() || to.empty()) return;
			fstream f(from);
			if (!f.is_open()) return;
			f.close();
			f.open(to);
			if (f.is_open()) { f.close(); return; }
			this->path_from = from;
			this->path_to = to;
			this->ready = true;
		}
		void Run()
		{
			cout << "1/6) Decoder initializing" << endl;
			unsigned __int32 cnt_codes;		// количество букв
			unsigned char cnt_byte;			// вес одного куска-оригинала
			unsigned char max_byte_size;	// количество байт для кода максимальной длины

			cout << "2/6) Reading code-table" << endl;
			/// step 1
			fstream fin(this->path_from, ios::binary | ios::in);

			fin.read((char*)&cnt_codes, sizeof(cnt_codes));
			fin.read((char*)&cnt_byte, sizeof(cnt_byte));
			fin.read((char*)&max_byte_size, sizeof(max_byte_size));

			string tmp_original; tmp_original.resize(cnt_codes);
			char tmp_bit_size;

			char *buff = new char[max_byte_size];

			spaceBitSet::BitSet tmp_bs;
			spaceAVL_Tree::AVL_Tree<DataNode> tree;
			for (unsigned __int32 i = 0; i < cnt_codes; i++)
			{
				for (unsigned char j = 0; j < cnt_byte; j++) 
					fin.read((char*)&tmp_original[j], sizeof(char));
				fin.read((char*)&tmp_bit_size, sizeof(tmp_bit_size));
				for (unsigned char j = 0; j < max_byte_size; j++)
					fin.read((char*)&buff[i], sizeof(char));
				tmp_bs.SetMemory(buff, tmp_bit_size);
				tree.Insert(DataNode(tmp_original, tmp_bit_size, buff));
			}
			//delete[] buff;
			tmp_bs.~BitSet();

			cout << "     Code-table is read" << endl;
			cout << "3/6) Reading compressed file and writing original" << endl;

			/// step 2
			fstream fout(this->path_to, ios::binary | ios::out);
			tmp_bs.Reserve(max_byte_size << 2);
			spaceBitSet::BitSet bs;
			char c;
			while (fin.read(&c, sizeof(c)))
			{
				bs.SetMemory(&c, 8);
				tmp_bs.ConcatSet(bs);
				while (tmp_bs.BitSize() >= max_byte_size)
				{
					size_t sz = tmp_bs.BitSize();
					size_t temp_sz = sz;
					while (temp_sz)
					{
						tmp_bs.Resize(temp_sz);
						DataNode node = tree.FindVal(DataNode(tmp_bs));
						if (node.original.empty()) { temp_sz--; continue; }
						for (size_t i = 0; i < node.original.size(); i++)
							fout.write((char*)&node.original[i], sizeof(char));
						tmp_bs.Resize(sz);
						tmp_bs.MoveLeft(temp_sz);
					}
				}
			}
			fin.close();
			fout.close();
			cout << "     Original file is writen" << endl;
		}
	};
}