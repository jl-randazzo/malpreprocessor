#include "stdafx.h"
#include "MalLine.h"
#include <string>
#include <regex>


using namespace std;

MalLine::MalLine(string line) : _line(line) { ProcessLine(); }

ostream & operator <<(ostream &out, const MalLine &malLine)
{
	out << malLine._line << " ";
	if (malLine._emptyLine)
	{
		out << "true";
	}
	else
	{
		out << "false";
	}
	return out;
}

//Accessors
string MalLine::GetLine()
{
	return _line;
}

bool MalLine::IsLineEmpty()
{
	return _emptyLine;
}

int MalLine::GetCommentIndex()
{
	return _commentIndex;
}

string MalLine::GetErrorMessage()
{
	return _errorMessage;
}

void MalLine::ProcessLine()
{
	int start = _line.find_first_not_of(" ", 0);
	if (start >= 0)
	{
		int len = _line.length() - start;
		string targ = _line.substr(start, len);
		if (regex_match(targ, label, regex_constants::match_default))
		{
			_commentIndex = start;
			_emptyLine = true;
		}
		else
		{
			_emptyLine = false;
		}
	}
}