#pragma once
#include <string>
#include <fstream>
#include "PriorityQueue.cpp"
using namespace std;

class Archiver
{
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