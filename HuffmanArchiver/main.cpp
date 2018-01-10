#include <iostream>
#include "Archiver.h"
using namespace std;

int main()
{
	string input = "D://2.Programming//file.txt";
	//string input = "D://2.Programming//1.pdf";
	string output = "D://2.Programming//file(compressed).pdf";
	size_t byte = 1;
	
	spaceArchiver::Coder coder;
	coder.Initialize(input, output, byte);
	if (!coder.Ready()) cout << "Error!" << endl;
	else coder.Run();

	system("pause");
	return 0;
}