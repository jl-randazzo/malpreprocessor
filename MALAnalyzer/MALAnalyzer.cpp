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
	string str = "ADD R1, R2, R3";
	MalLine ml(str);
	cout << ml;
	getline(std::cin, str);
    return 0;
}

