#pragma once
#include <string>
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
		Archiver();
		~Archiver();

		bool Ready();
		void Initialize(string, string, size_t);
		void Run();
	};
}