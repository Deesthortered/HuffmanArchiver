#include <iostream>
#include "Archiver.h"
using namespace std;

int main()
{
	string input = "D://2.Programming//file.pdf";
	string output = "D://2.Programming//file(compressed).pdf";
	size_t byte = 1;
	
	spaceArchiver::Archiver archiver;
	archiver.Initialize(input, output, byte);
	if (!archiver.Ready()) cout << "Error!" << endl;
	else archiver.Run();
	
	system("pause");
	return 0;
}