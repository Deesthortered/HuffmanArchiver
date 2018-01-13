#include <iostream>
//#include "Archiver.h"
#include "BitSet.h"
using namespace std;

int main()
{
	/*
	size_t byte = 1;
	string input = "D://2.Programming//file.txt";
	string output = "D://2.Programming//file.compr";

	//string input = "D://2.Programming//1.pdf";
	//string output = "D://2.Programming//1.compr";

	spaceArchiver::Coder coder;
	coder.Initialize(input, output, byte);
	if (!coder.Ready()) cout << "Error!" << endl;
	else coder.Run();
	*/

	spaceBitSet::BitSet bs;
	bs.PushBack(true);
	bs.PushBack(false);
	bs.PushBack(true);
	bs.PushBack(false);
	bs.PushBack(false);
	bs.PushBack(true);
	bs.PushBack(true);
	for (size_t i = 0; i < bs.BitSize(); i++)
		cout << bs.GetValue(i); cout << endl;

	spaceBitSet::BitSet bs1;
	bs1.PushBack(false);
	bs1.PushBack(true);
	bs1.PushBack(true);
	bs1.PushBack(false);
	bs1.PushBack(false);
	for (size_t i = 0; i < bs1.BitSize(); i++)
		cout << bs1.GetValue(i); cout << endl;

	bs.ConcatSet(bs1);

	for (size_t i = 0; i < bs.BitSize(); i++)
		cout << bs.GetValue(i); cout << endl;


	system("pause");
	return 0;
}