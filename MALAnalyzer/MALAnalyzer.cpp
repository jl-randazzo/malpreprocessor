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
	string str = "LOADI R1, 72";
	MalLine ml(str);
	cout << ml;
	getline(std::cin, str);
    return 0;
}

