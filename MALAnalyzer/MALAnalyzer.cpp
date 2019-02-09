// MALAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "MalLine.h"


using namespace std;

list<string> ReadMalFile(string fileName)
{
	try
	{
		std::filebuf fb;
		if (fb.open(fileName + ".mal", ios::in))
		{
			istream is(&fb);
			list<string> retList;
			string str;
			while (is.peek() != EOF)
			{
				getline(is, str);
				retList.push_back(str);
			}
			fb.close();
			return retList;
		}
	}
	catch (exception e)
	{
		perror("error reading file");
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	string str = "LOADI R7, 67321547";
	MalLine ml(str);
	cout << ml << endl;
	list<string> mylist = ReadMalFile("Text");
	for (string x : mylist)
	{
		cout << x << endl;
	}
	getline(std::cin, str);
    return 0;
}

