#include <vld.h>
#include <iostream>
#include "Archiver.h"
using namespace std;

int main()
{	
	/*
	unsigned char byte = 1;
	string input = "D://2.Programming//1.txt";
	string output = "D://2.Programming//1.txt.compr";
	string output2 = "D://2.Programming//1(compr).txt";

	remove(output.c_str());
	remove(output2.c_str());

	spaceArchiver::Coder coder;
	coder.Initialize(input, output, byte);
	if (!coder.Ready()) cout << "Error1!" << endl;
	else coder.Run();

	spaceArchiver::Decoder decoder;
	decoder.Initialize(output, output2);
	if (!decoder.Ready()) cout << "Error2!" << endl;
	else decoder.Run();*/

	spaceBitSet::BitSet bs1, bs2;
	bs1.Reserve(1);
	bs2.Reserve(1);

	bs1.PushBack(true);
	bs1.PushBack(false);
	bs1.PushBack(true);

	bs2.PushBack(true);
	bs2.PushBack(true);
	bs2.PushBack(true);

	for (size_t i = 0; i < bs1.BitSize(); i++)
		cout << bs1.GetValue(i); cout << endl;
	for (size_t i = 0; i < bs2.BitSize(); i++)
		cout << bs2.GetValue(i); cout << endl << endl;

	cout << (bs1 < bs2);
	cout << (bs1 == bs2);
	cout << (bs1 > bs2) << endl;

	system("pause");
	return 0;
}