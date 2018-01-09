#pragma once
#include <string>
#include <fstream>
#include "AVL_Tree.h"
#include "PriorityQueue.h"
using namespace std;

namespace spaceArchiver
{
	class Archiver
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
		Archiver()
		{
			this->path_from.clear();
			this->path_to.clear();
			this->cnt_byte = 0;
			this->ready = false;
		}
		~Archiver()
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
			cout << "Function is started" << endl;
			cout << "Crate avl tree" << endl;

			spaceAVL_Tree::AVL_Tree<string> tree;
			fstream fin(this->path_from, ios::binary | ios::in);
			char c; string word; size_t i = 0;
			word.reserve(this->cnt_byte);
			while (fin.read(&c, sizeof(c)))
			{
				word.push_back(c); i++;
				if (i >= this->cnt_byte)
				{
					tree.Insert(word);
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
			cout << "avl tree is created" << endl;
			cout << "create queue" << endl;

			spacePriorityQueue::PriorityQueue<HuffTrNode, size_t> q;
			while (!tree.IsEmpty())
			{
				size_t k = tree.GetRootCount();
				HuffTrNode t = HuffTrNode(tree.GetRootVal(), k, nullptr, nullptr);
			  	q.Push(t, k);
				tree.DeleteRoot();
			}
			tree.~AVL_Tree();
			cout << "queue is created" << endl;
			cout << "create huffman tree" << endl;
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
			cout << "huffman tree is created" << endl;
			 

		}
	};
}