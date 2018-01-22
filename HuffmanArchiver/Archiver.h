#pragma once
#include <fstream>
#include <vector>
#include <direct.h>
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

		vector<string> path_from;
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
		void Initialize(vector<string> from, string to, unsigned char cnt)
		{
			if (from.empty() || to.empty()) return;
			for (size_t i = 0; i < from.size(); i++)
			{
				fstream f(from[i]);
				if (!f.is_open()) return;
				f.close();
			}
			fstream f(to);
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
			cout << "*** Coder ***" << endl;
			cout << "1/6) Coder initializing" << endl;

			unsigned __int32 cnt_files = path_from.size();
			vector<string> names(cnt_files);
			for (size_t i = 0; i < (size_t)cnt_files; i++)
				names[i] = this->path_from[i].substr(this->path_from[i].rfind('/') + 1);

			spaceAVL_Tree::AVL_Tree<DataNode> tree;
			unsigned __int32 cnt_codes = 0;
			cout << "2/6) Reading files and creating avl tree" << endl;

			for (size_t k = 0; k < (size_t)cnt_files; k++)
				for (size_t j = 0; j < names[k].size(); j += this->cnt_byte)
				{
					if (!tree.Insert(DataNode(names[k].substr(j, this->cnt_byte)))) cnt_codes++;
				}
			for (size_t k = 0; k < (size_t)cnt_files; k++)
			{
				fstream fin(this->path_from[k], ios::binary | ios::in);
				char c; string word; size_t _i = 0;
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
					tree.Insert(DataNode(word));
					word.clear(); _i = 0;
				}
				fin.close();
			}

			cout << "     Tree is created" << endl;
			cout << "3/6) Create Huffman tree" << endl;
			
			/// 2 step
			spacePriorityQueue::PriorityQueue<HuffTrNode, size_t> q;
			spaceArray::Array<DataNode> arr = tree.ReturnAllVals();
			for (size_t i = 0; i < arr.Size(); i++)
				q.Push(HuffTrNode(arr[i].data, arr[i].cnt, nullptr, nullptr), arr[i].cnt);
			q.Push(HuffTrNode("", 1, nullptr, nullptr), 1); // end-file-mark
			arr.~Array();
			
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
			spaceBitSet::BitSet bs, end_bs;
			unsigned char max_bit_size = Hufffunc(hufftree, bs, tree, end_bs) - 1;
			unsigned char max_byte_size = (max_bit_size >> 3) + ((max_bit_size - ((max_bit_size >> 3) << 3)) ? 1 : 0);
			hufftree = nullptr;
			bs.~BitSet();
			arr = tree.ReturnAllVals();

			cout << "     Codes is created" << endl;
			cout << "5/6) Write code-table to file" << endl;

			/// 5 step
			fstream fout(this->path_to, ios::binary | ios::out);

			fout.write((char*)&cnt_codes, sizeof(unsigned __int32));			// unsigned __int32 - 4 byte // количество букв
			fout.write((char*)&cnt_files, sizeof(unsigned __int32));			// unsigned __int32 - 4 byte // количество файлов
			fout.write((char*)&this->cnt_byte, sizeof(unsigned char));			// unsigned char    - 1 byte // вес одного куска-оригинала
			fout.write((char*)&max_byte_size, sizeof(unsigned char));			// unsigned char    - 1 byte // количество байт для кода максимальной длины
			
			char *buff = new char[max_byte_size];
			end_bs.GetMemory(buff);
			//Запись метки конца файла в таблицу
			unsigned char end_sz = static_cast<unsigned char>(end_bs.BitSize());
			fout.write((char*)&end_sz, sizeof(unsigned char));
			for (size_t i = 0; i < max_byte_size; i++)
				fout.write((char*)&buff[i], sizeof(char));

			// Запись остальной таблици
			for (size_t i = 0; i < arr.Size(); i++)
			{
				for (size_t j = 0; j < this->cnt_byte; j++) 
					fout.write((char*)&arr[i].data[j], sizeof(char));
				unsigned char s = static_cast<unsigned char>(arr[i].bs.BitSize());
				fout.write((char*)&s, sizeof(s));
				arr[i].bs.GetMemory(buff);
				for (size_t j = 0; j < max_byte_size; j++)
					fout.write((char*)&buff[j], sizeof(char));
			}
			delete[] buff;
			arr.~Array();

			cout << "     Code-table is written" << endl;
			cout << "6/6) Write compressed file" << endl;

			/// 6 step
			spaceBitSet::BitSet temp_bs;
			temp_bs.Reserve((max_bit_size < 8 ? 8 : max_bit_size) << 1);
			for (size_t k = 0; k < (size_t)cnt_files; k++)
			{
				char c; string word; size_t _i = 0;
				// Write name
				for (size_t j = 0; j < names[k].size(); j += this->cnt_byte)
				{
					spaceBitSet::BitSet a = tree.FindVal(names[k].substr(j, this->cnt_byte)).bs;
					temp_bs.ConcatSet(a);
					while (temp_bs.BitSize() >= 8)
					{
						char c = temp_bs.GetFirstByte();
						fout.write(&c, sizeof(char));
						temp_bs.MoveLeft(8);
					}
				}
				temp_bs.ConcatSet(end_bs);
				// Write file
				fstream fin(this->path_from[k], ios::binary | ios::in);
				while (fin.read(&c, sizeof(c)))
				{
					word.push_back(c); _i++;
					if (_i >= this->cnt_byte)
					{
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
					}
				}
				fin.close();
				if (_i)
				{
					spaceBitSet::BitSet tmp = tree.FindVal(DataNode(word)).bs;
					temp_bs.ConcatSet(tmp);
					word.clear();
					_i = 0;
				}
				temp_bs.ConcatSet(end_bs);
				while (temp_bs.BitSize() >= 8)
				{
					char c = temp_bs.GetFirstByte();
					fout.write(&c, sizeof(char));
					temp_bs.MoveLeft(8);
				}
				if ((k+1 == cnt_files) && temp_bs.BitSize()) // for last file
				{
					size_t tmp = 8 - temp_bs.BitSize();
					for (size_t i = 0; i < tmp; i++)  temp_bs.PushBack(false);
					char c = temp_bs.GetFirstByte();
					fout.write(&c, sizeof(char));
				}
			}
			fout.close();
			cout << "     File have already written" << endl << endl << endl;
		}
	private:
		unsigned char Hufffunc(HuffTrNode *&node, spaceBitSet::BitSet &bs, spaceAVL_Tree::AVL_Tree<DataNode> &tree, spaceBitSet::BitSet &end_bs)
		{
			if (!node) return 0;
			if (!node->data.empty())
			{
				DataNode a = DataNode(node->data, bs);
				tree.ReplaceVal(DataNode(node->data), a);
			}
			else if (!node->left && !node->right) end_bs = bs;

			spaceBitSet::BitSet l = bs;
			spaceBitSet::BitSet r = bs;
			l.PushBack(false);
			r.PushBack(true);

			unsigned char a = Hufffunc(node->left, l, tree, end_bs);
			unsigned char b = Hufffunc(node->right, r, tree, end_bs);
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
			cout << "*** Decoder ***" << endl;
			cout << "1/3) Decoder initializing" << endl;

			this->path_to += "/" + this->path_from.substr(this->path_from.rfind('/') + 1) + "(decompressed)";
			
			unsigned __int32 cnt_codes;		// количество букв
			unsigned __int32 cnt_files;		// количество файлов
			unsigned char cnt_byte;			// вес одного куска-оригинала
			unsigned char max_byte_size;	// количество байт для кода максимальной длины

			cout << "2/3) Reading code-table" << endl;
			/// step 1
			fstream fin(this->path_from, ios::binary | ios::in);

			fin.read((char*)&cnt_codes, sizeof(cnt_codes));
			fin.read((char*)&cnt_files, sizeof(cnt_files));
			fin.read((char*)&cnt_byte, sizeof(cnt_byte));
			fin.read((char*)&max_byte_size, sizeof(max_byte_size));

			string tmp_original; tmp_original.resize(cnt_byte);
			char tmp_bit_size;

			unsigned char end_sz;
			fin.read((char*)&end_sz, sizeof(end_sz));

			char *buff = new char[max_byte_size];
			for (unsigned char j = 0; j < max_byte_size; j++)
				fin.read(&buff[j], sizeof(char));
			spaceBitSet::BitSet end_bs; end_bs.SetMemory(buff, end_sz);
			spaceAVL_Tree::AVL_Tree<DataNode> tree;
			for (unsigned __int32 i = 0; i < cnt_codes; i++)
			{
				for (unsigned char j = 0; j < cnt_byte; j++) 
					fin.read((char*)&tmp_original[j], sizeof(char));
				fin.read((char*)&tmp_bit_size, sizeof(tmp_bit_size));
				for (unsigned char j = 0; j < max_byte_size; j++)
					fin.read(&buff[j], sizeof(char));
				tree.Insert(DataNode(tmp_original, tmp_bit_size, buff));
			}
			delete[] buff;

			cout << "     Code-table is read" << endl;
			cout << "3/3) Reading compressed file and writing originals" << endl;

			if (!_mkdir(this->path_to.c_str())) { cout << "Error! Can't create directory." << endl; };
			/// step 2
			spaceBitSet::BitSet tmp_bs;
			tmp_bs.Reserve(max_byte_size << 4);
			

			fstream fout(this->path_to, ios::binary | ios::out);
			char c;
			while (fin.read(&c, sizeof(c)))
			{
				spaceBitSet::BitSet bs;
				bs.SetMemory(&c, 8);
				tmp_bs.ConcatSet(bs);
				while (tmp_bs.BitSize() >= (size_t)(max_byte_size << 4))
				{
					size_t sz = tmp_bs.BitSize();
					size_t temp_sz = sz;
					while (temp_sz)
					{
						tmp_bs.Resize(temp_sz);
						DataNode node = tree.FindVal(DataNode(tmp_bs));
						if (node.original.empty()) { temp_sz--; if (!temp_sz) tmp_bs.Resize(sz); continue; }
						for (size_t i = 0; i < node.original.size(); i++)
							fout.write((char*)&node.original[i], sizeof(char));
						tmp_bs.Resize(sz);
						tmp_bs.MoveLeft(temp_sz);
						sz = tmp_bs.BitSize();
						temp_sz = sz;
					}
				}
			}
			{
				size_t sz = tmp_bs.BitSize();
				size_t temp_sz = sz;
				while (temp_sz)
				{
					tmp_bs.Resize(temp_sz);
					DataNode node = tree.FindVal(DataNode(tmp_bs));
					if (node.original.empty()) { temp_sz--; if (!temp_sz) tmp_bs.Resize(sz); continue; }
					for (size_t i = 0; i < node.original.size(); i++)
						fout.write((char*)&node.original[i], sizeof(char));
					tmp_bs.Resize(sz);
					tmp_bs.MoveLeft(temp_sz);
					sz = tmp_bs.BitSize();
					temp_sz = sz;
				}
			}
			fout.close();


			fin.close();

			cout << "     Original files is writen" << endl << endl << endl;
		}
	};
}