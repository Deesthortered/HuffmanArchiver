#include "Archiver.h"
#include <fstream>
#include "AVL_Tree.h"
#include "PriorityQueue.h"

namespace spaceArchiver
{
	
	Archiver::Archiver()
	{
		this->path_from.clear();
		this->path_to.clear();
		this->cnt_byte = 0;
		this->ready = false;
	}
	Archiver::~Archiver()
	{
		this->path_from.clear();
		this->path_to.clear();
		this->cnt_byte = 0;
		this->ready = false;
	}

	bool Archiver::Ready()
	{
		return this->ready;
	}
	void Archiver::Initialize(string from, string to, size_t cnt)
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
	
	void Archiver::Run()
	{
		/*
		if (!this->ready) return;
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


		spacePriorityQueue::PriorityQueue<HuffTrNode, size_t> q;
		while (!tree.IsEmpty())
		{
			q.Push(HuffTrNode(tree.GetRootVal(), tree.GetRootCount(), nullptr, nullptr), tree.GetRootCount());
			tree.DeleteRoot();
		}
		tree.~AVL_Tree();

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
			*tmp2 = q.ExtractMin(); q.Pop();
			HuffTrNode *tmp3 = new HuffTrNode("", tmp1->cnt + tmp2->cnt, tmp1, tmp2);
			q.Push(*tmp3, tmp3->cnt);
			delete tmp3;
		}
		q.~PriorityQueue();*/
	}
	
}