// MALAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <list>
#include <string>
#include <iostream>
#include "MalLine.h"

using namespace std;

int main(int argc, char* argv[])
{
	string str = "Hello world!\n";
	MalLine ml(str);
	cout << ml << argc << argv[1];
	getline(std::cin, str);
    return 0;
}

