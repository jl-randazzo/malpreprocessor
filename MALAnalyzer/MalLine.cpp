#include "stdafx.h"
#include "MalLine.h"

using namespace std;

MalLine::MalLine(string line) : line(line) { ProcessLine(); }

ostream & operator <<(ostream &out, const MalLine &malLine)
{
	out << malLine.line;
	return out;
}

void MalLine::ProcessLine()
{
	return; 
}