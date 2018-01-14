#include <iostream>
#include "Archiver.h"
using namespace std;

int main()
{
	
	unsigned char byte = 1;

	string input = "D://2.Programming//1.txt";
	string output = "D://2.Programming//1.compr";
	remove(output.c_str());

	spaceArchiver::Coder coder;
	coder.Initialize(input, output, byte);
	if (!coder.Ready()) cout << "Error!" << endl;
	else coder.Run();


	system("pause");
	return 0;
}