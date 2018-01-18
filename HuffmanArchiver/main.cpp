#include <vld.h>
#include <iostream>
#include "Archiver.h"
using namespace std;

int main()
{
/*	
	unsigned char byte = 1;
	string input = "D://2.Programming//1.txt";
	string output = "D://2.Programming//1.compr";
	remove(output.c_str());
	spaceArchiver::Coder coder;
	coder.Initialize(input, output, byte);
	if (!coder.Ready()) cout << "Error!" << endl;
	else coder.Run();
	coder.~Coder();
	
*/
	spaceAVL_Tree::AVL_Tree<int> tree;
	for (int i = 0; i < 12; i++) 
		tree.Insert(i);

	
	tree.DeleteNode(3);
	tree.DeleteNode(6);
	tree.DeleteNode(9);
	tree.DeleteNode(11);
	

	spaceArray::Array<int> ar1 = tree.ReturnAllVals();
	for (size_t i = 0; i < ar1.Size(); i++)
		cout << ar1[i] << " "; cout << endl;


	system("pause");
	return 0;
}