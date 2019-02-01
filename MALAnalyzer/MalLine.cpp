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
	if (malLine._validLine)
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

bool MalLine::IsLineValid()
{
	return _validLine;
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
		string workingCopy = _lineNoComment;
		string targ = PopNext(workingCopy);

		if (regex_match(targ, labelRegex)) //Is this a label?
		{
			cout << "label";
		}
		else if (regex_match(targ, addRegex)) //Is this an Add instruction?
		{
			cout << "add";
			string arg1 = PopNext(workingCopy), arg2 = PopNext(workingCopy), arg3 = PopNext(workingCopy);
			_validLine = ValidateWord(arg1, Register, false) &&
				ValidateWord(arg2, Register, false) &&
				ValidateWord(arg3, Register, true);
		}
	}
	else
	{
		_emptyLine = true;
	}
}

//adjustments to the workingCopy string are made purposefully
const string MalLine::PopNext(string &workingCopy) const
{
	int term = workingCopy.find_first_of(" ", 0);
	if (term == -1)
	{
		string ret = workingCopy;
		workingCopy = "";
		return ret;
	}
	else
	{
		string ret = workingCopy.substr(0, term);
		workingCopy = workingCopy.substr(term + 1, workingCopy.length() - (term + 1));
		return ret;
	}
}

bool MalLine::ValidateWord(string &targ, WordType type, bool finalOp)
{
	switch (type)
	{
	case Register:
		if (regex_match(targ, regRegex)) break;
		_errorMessage = "illformed register";
		return false;
	case Immediate:
	default:
		break;
	}
	if (finalOp) return targ.find_first_of(",", 0) == -1;
	else return targ.find(",", 0) == targ.length() - 1;
}