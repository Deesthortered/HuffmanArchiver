#include <iostream>
#include "Archiver.h"
using namespace std;

int main()
{
	size_t byte = 1;
	string input = "D://2.Programming//file.txt";
	string output = "D://2.Programming//file.compr";

	//string input = "D://2.Programming//1.pdf";
	//string output = "D://2.Programming//1.compr";

	spaceArchiver::Coder coder;
	coder.Initialize(input, output, byte);
	if (!coder.Ready()) cout << "Error!" << endl;
	else coder.Run();

	system("pause");
	return 0;
}