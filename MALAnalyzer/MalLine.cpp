#include "stdafx.h"
#include "MalLine.h"
#include <string>
#include <iostream>
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
	_commentIndex = _line.find_first_of(";", 0);
	if (start >= 0 && (_commentIndex == -1 || start < _commentIndex))
	{
		_emptyLine = false;
		int len = _line.length() - start;
		_lineNoComment = _line.substr(start, len);
		string workingString = _lineNoComment;
		string targ = PopNext(workingString);
		if (regex_match(targ, labelRegex))
		{
			cout << "label";
		}
		else if (regex_match(targ, addRegex))
		{
			cout << "add";
		}
	}
	else
	{
		_emptyLine = true;
	}
}

//adjustments to the passed in string are made purposefully
string MalLine::PopNext(string &line)
{
	int term = line.find_first_of(" ", 0);
	if (term == -1)
	{
		string ret = line;
		line = "";
		return ret;
	}
	else
	{
		string ret = line.substr(0, term);
		line = line.substr(term + 1, line.length() - (term + 1));
		return ret;
	}
}

void MalLine::ValidateNext(string &next, WordType type, bool finalOp)
{
	return;
}