#include "stdafx.h"
#include "MalLine.h"
#include <string>
#include <iostream>
#include <regex>







using namespace std;

MalLine::MalLine(string line) : _line(line) { ProcessLine(); }

ostream & operator <<(ostream &out, const MalLine &malLine)
{
	out << malLine._lineNoComment << " " << endl << malLine._errorMessage << " " << endl;
	if (malLine._validLine)
	{
		out << "true";
	}
	else
	{
		out << "false";
	}
	out << endl;
	return out;
}

//Accessors
const string MalLine::GetLine()
{
	return _line;
}

const string MalLine::GetLineWithoutComment()
{
	return _lineNoComment;
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

ErrorCode MalLine::GetErrorCode()
{
	return _errorCode;
}

const string MalLine::GetErrorMessage()
{
	return _errorMessage;
}

//Does initial check if there's a label, if there is an isntruction, produces a working copy, and then sends
//the working copy and the word that should contain the opcode to the overloaded ProcessLine function
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

		if (regex_match(targ, anyLabelRegex)) //Is this a label?
		{
			if (regex_match(targ, labelRegex)) //valid label?
			{
				cout << "label";
				targ = PopNext(workingCopy);
				ProcessLine(targ, workingCopy);
			}
		}
		else 
		{
			ProcessLine(targ, workingCopy);
		}
	}
	else
	{
		_emptyLine = true;
	}
}

//Goes through the possible opcode cases. Calls the ValidateWord function with the appropriate, expected enumeration
void MalLine::ProcessLine(string &opcode, string &workingCopy)
{
	if (opcode._Equal("ADD") | opcode._Equal("SUB")) //Is this an ADD or SUB instruction?
	{
		string args[3];
		_errorCode = ExtractArgs(workingCopy, args, 3);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register, false) &&
				ValidateWord(args[1], Register, false) &&
				ValidateWord(args[2], Register, true);
		}
	}
	else if (opcode._Equal("INC") | opcode._Equal("DEC")) //Is this an INC or DEC instruction?
	{
		string args[1];
		_errorCode = ExtractArgs(workingCopy, args, 1);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register, true);
		}
	}
	else if (opcode._Equal("LOAD") | opcode._Equal("STORE")) //Is this a LOAD or STORE instruction?
	{
		string args[2];
		_errorCode = ExtractArgs(workingCopy, args, 2);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register, false) && ValidateWord(args[1], MemAddress, true);
		}
	}
	else if (opcode._Equal("LOADI")) //Is this a LOADI instruction?
	{
		string args[2];
		_errorCode = ExtractArgs(workingCopy, args, 2);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register, false) && ValidateWord(args[1], Immediate, true);
		}
	}
	else if (opcode._Equal("BEQ") | opcode._Equal("BGT") | opcode._Equal("BLT")) //Is this a BEQ, BLT, or BGT instruction?
	{
		string args[3];
		_errorCode = ExtractArgs(workingCopy, args, 2);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register, false) && ValidateWord(args[1], Immediate, false)
				&& ValidateWord(args[2], MemAddress, true);
		}
	}
	else if (opcode._Equal("B")) //Is this a B instruction?
	{
		string args[1];
		_errorCode = ExtractArgs(workingCopy, args, 1);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], MemAddress, true);
		}
	}
	else if (opcode._Equal("NOOP") | opcode._Equal("END")) //Is this a NOOP or END instruction?
	{
		_validLine = !HasNext(workingCopy); //There shouldn't be another operand
	}
	else
	{
		_validLine = false;
	}

	//if the _errorCode was altered above, we need to adjust the errorMessage
	switch (_errorCode)
	{
	case TooFewOps:
		_validLine = false;
		_errorMessage = "too few operands: for the specific opcode, there are fewer operands than required";
		break;
	case TooManyOps:
		_validLine = false;
		_errorMessage = "too many operands: for the specific opcode, there are more operands than required";
		break;
	default:
		break;
	}
}

ErrorCode MalLine::ExtractArgs(string &workingCopy, string args[], int count) const
{
	for (int i = 0; i < count; i++)
	{
		if (HasNext(workingCopy)) args[i] = PopNext(workingCopy);
		else
		{
			return TooFewOps;
		}
	}
	if (HasNext(workingCopy)) return TooManyOps;
	else return NoError;
}

bool MalLine::HasNext(const string &workingCopy) const
{
	int ind = workingCopy.find_first_not_of(" ", 0); 
	return ind >= 0;
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
		_errorCode = ValidateReg(targ);
		if (_errorCode == NoError) break;
		_errorMessage = "** error -- ill-formed operand: invalid register declaration \"" + targ + "\"\n" +
			"**         Registers should be typed in the form R0, R1, R2, and are enumerated 0-7.";
		return false;
	case Immediate:
		if (regex_match(targ, immRegex)) break;
		_errorCode = BadImmediate;
		_errorMessage = "ill-formed immediate value: expected octal value (0-7) but found \"" + targ + "\"";
		return false;
	case MemAddress:
		_errorCode = ValidateIdent(targ);
		switch (_errorCode)
		{
		default: //No Error
			break;
		case InvalidLength:
			_errorMessage = "** error -- ill-formed identifier: the identifier \"" + targ + "\" is too long.\n" +
				"**         Identifiers must be only letters (a-z, A-Z) and no longer than five characters.";
			return false;
		case InvalidCharacters:
			_errorMessage = "** error -- ill-formed identifier: the identifier \"" + targ + "\" has invalid characters.\n" +
				"**         Identifiers must be only letters (a-z, A-Z) and no longer than five characters.";
			return false;
		}
	}
	if (finalOp)
	{
		if (!targ.find_first_of(",", 0) == -1)
		{
			_errorCode = MisplacedComma;
			_errorMessage = "** error -- misplaced comma: a comma is placed after the final operand; it shouldn't be there.";
			return false;
		}
		else return true;
	}
	else
	{
		if (!targ.find(",", 0) == targ.length() - 1)
		{
			_errorCode = MissingComma;
			_errorMessage = "** error -- missing comma: you are missing a comma following \"" + targ + ".\"";
			return false;
		}
		else return true;
	}
}

ErrorCode MalLine::ValidateReg(const string &R) const
{
	bool ret = false;
	if (R.length() >= 2)
	{
		int ind = octal.find_first_of(R[1], 0);
		ret = (R[0] == 'R') && ind >= 0;
	}
	if (R.length() >= 3)
	{
		ret = ret && (R[2] == ',');
	}
	if (ret && (R.length() < 4)) return NoError;
	else return BadRegister;
}

ErrorCode MalLine::ValidateIdent(const string &ident) const
{
	bool ret = true;
	int searchLength = ident.length();
	for (int i = 0; i < searchLength; i++)
	{
		int ind = alphabet.find_first_of(ident[i], 0);
		ret = ret && ind > -1;
	}
	if (ret && searchLength > 0)
		if (searchLength < 6) return NoError;
		else return InvalidLength;
	else return InvalidCharacters;
}