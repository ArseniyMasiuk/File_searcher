#include "Searcher.h"

int main()
{
	setlocale(LC_ALL, "Russian");

	string myPath = "E:/";
	string myFile = "fucking_name.txt";
	
	Manager man(myFile, myPath);

	system("pause");
}