#include <vld.h>
#include <iostream>
#include <vector>
#include "Archiver.h"
using namespace std;

int main()
{	
	{
		unsigned char byte = 1;
		vector<string> input(3);
		{
			input[0] = "D:/2.Programming/123/1.txt";
			input[1] = "D:/2.Programming/123/1.mp4";
			input[2] = "D:/2.Programming/123/1.jpg";
		}
		string output = "D:/2.Programming/123/1.compr";
		string output2 = "D:/2.Programming/123";

		remove(output.c_str());
		remove(output2.c_str());

		spaceArchiver::Coder coder;
		coder.Initialize(input, output, byte);
		if (!coder.Ready()) cout << "Error1!" << endl;
		else coder.Run();

		
		spaceArchiver::Decoder decoder;
		decoder.Initialize(output, output2);
		if (!decoder.Ready()) cout << "Error2!" << endl;
		else decoder.Run();
		
	}
	
	system("pause");
	return 0;
}