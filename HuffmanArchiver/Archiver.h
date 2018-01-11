#pragma once
#include <fstream>
#include <iostream>
#include "AVL_Tree.h"
using namespace std;
typedef unsigned long long ull;

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
		string path_from;
		string path_to;
		size_t cnt_byte;
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
		void Initialize(string from, string to, size_t cnt)
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

			fstream fin(this->path_from, ios::binary | ios::in);
			spaceAVL_Tree::AVL_Tree tree;
			char c; string word; size_t i = 0; ull cnt_codes = 0;

			word.reserve(this->cnt_byte);
			while (fin.read(&c, sizeof(c)))
			{
				word.push_back(c); i++;
				if (i >= this->cnt_byte)
				{
					if (!tree.Insert(word)) cnt_codes++;
					word.clear();
				i = 0;
				}
			}
			if (i)
			{
				for (size_t j = 0; i < this->cnt_byte - i; j++) word.push_back('\0');
				tree.Insert(word);
				word.clear(); i = 0;
			}
			fin.close();


			spacePriorityQueue::PriorityQueue<HuffTrNode, size_t> q;
			spaceArray::Array<spaceAVL_Tree::Node> arr = tree.ReturnNodes();
			for (size_t i = 0; i < arr.Size(); i++)
				q.Push(HuffTrNode(arr[i].data, arr[i].cnt, nullptr, nullptr), arr[i].cnt);
			

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
			
			spaceBitSet::BitSet bs; bs.Reserve(1);
			size_t max_bit_size = Hufffunc(hufftree, bs, tree) - 1;
			hufftree = nullptr;
			bs.~BitSet();

			// cnt_codes, max_bit_size, tree, arr
		}
	private:
		size_t Hufffunc(HuffTrNode *&node, spaceBitSet::BitSet &bs, spaceAVL_Tree::AVL_Tree &tree)
		{
			if (!node) return 0;
			if (!node->data.empty())
				tree.SetBitSet(node->data, bs);				

			spaceBitSet::BitSet l = bs;
			spaceBitSet::BitSet r = bs;
			l.PushBack(false);
			r.PushBack(true);

			size_t a = Hufffunc(node->left, l, tree);
			size_t b = Hufffunc(node->right, r, tree);
			delete node;
			return (a < b ? b : a) + 1;
		}
	};
	class Decoder
	{

	public:
		Decoder()
		{

		}
		~Decoder()
		{

		}
	};
}