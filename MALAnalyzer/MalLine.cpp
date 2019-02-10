#include "stdafx.h"
#include "MalLine.h"
#include <string>
#include <iostream>
#include <regex>



using namespace std;

MalLine::MalLine(string line) : _line(line) { ProcessLine(); }

// outstream operator designed for testing purposes
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
const string MalLine::GetLine() const
{
	return _line;
}

const string MalLine::GetLineWithoutComment() const
{
	return _lineNoComment;
}

const string MalLine::GetLeadingLabel() const
{
	return _leadingLabel;
}

const string MalLine::GetBranchingLabel() const
{
	return _branchingLabel;
}

bool MalLine::HasLeadingLabel() const
{
	return _hasLeadingLabel;
}

bool MalLine::HasBranchingLabel() const
{
	return _hasBranchingLabel;
}

bool MalLine::IsLineEmpty() const
{
	return _emptyLine;
}

bool MalLine::IsLineValid() const
{
	return _validLine;
}

int MalLine::GetCommentIndex() const
{
	return _commentIndex;
}

ErrorCode MalLine::GetErrorCode() const
{
	return _errorCode;
}

const string MalLine::GetErrorMessage() const
{
	return _errorMessage;
}

//Does initial check if there's a label, if there is an isntruction, produces a working copy, and then sends
//the working copy and the word that should contain the opcode to the ProcessOperation function
void MalLine::ProcessLine()
{
	int start = _line.find_first_not_of(" ", 0);
	_commentIndex = _line.find_first_of(";", 0);
	if (_commentIndex == -1) _commentIndex = _line.length();
	if (start >= 0 && (_commentIndex == -1 || start < _commentIndex))
	{
		_emptyLine = false;
		int len = _commentIndex - start;
		_lineNoComment = _line.substr(start, len);
		string workingCopy = _lineNoComment;
		string targ = PopNext(workingCopy);

		if (targ[targ.length() - 1] == ':') //Is this a label?
		{
			string labelcheck = targ.substr(0, targ.length() - 1);
			_errorCode = ValidateIdent(labelcheck);
			switch(_errorCode)
			{
			case NoError: // valid label?
				_leadingLabel = labelcheck;
				_hasLeadingLabel = true;
				targ = PopNext(workingCopy);
				ProcessOperation(targ, workingCopy);
				break;
			case InvalidLength:
				_errorMessage = "** error -- ill-formed leading label: the label \"" + labelcheck + "\" is too long.\n" +
					"**         Labels must be only letters (a-z, A-Z) and no longer than five characters.";
				break;
			case InvalidCharacters:
				_errorMessage = "** error -- ill-formed leading label: the label \"" + labelcheck + "\" has invalid characters.\n" +
					"**         Labels must be only letters (a-z, A-Z) and no longer than five characters.";
				break;
			}
		}
		else 
		{
			ProcessOperation(targ, workingCopy);
		}
	}
	else
	{
		_emptyLine = true;
	}
}

//Goes through the possible opcode cases. Calls the ValidateWord function with the appropriate, expected enumeration
void MalLine::ProcessOperation(string &opcode, string &workingCopy)
{
	string lastarg; // a reference string used to configure error message if there's a problem
	if (opcode._Equal("ADD") | opcode._Equal("SUB")) //Is this an ADD or SUB instruction?
	{
		string args[3];
		_errorCode = ExtractArgs(workingCopy, lastarg, args, 3);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register) &&
				ValidateWord(args[1], Register) &&
				ValidateWord(args[2], Register);
		}
	}
	else if (opcode._Equal("INC") | opcode._Equal("DEC")) //Is this an INC or DEC instruction?
	{
		string args[1];
		_errorCode = ExtractArgs(workingCopy, lastarg, args, 1);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register);
		}
	}
	else if (opcode._Equal("LOAD") | opcode._Equal("STORE")) //Is this a LOAD or STORE instruction?
	{
		string args[2];
		_errorCode = ExtractArgs(workingCopy, lastarg, args, 2);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register) && ValidateWord(args[1], MemAddress);
		}
	}
	else if (opcode._Equal("LOADI")) //Is this a LOADI instruction?
	{
		string args[2];
		_errorCode = ExtractArgs(workingCopy, lastarg, args, 2);
		if (_errorCode == NoError)
		{
			_validLine = ValidateWord(args[0], Register) && ValidateWord(args[1], Immediate);
		}
	}
	else if (opcode._Equal("BEQ") | opcode._Equal("BGT") | opcode._Equal("BLT")) //Is this a BEQ, BLT, or BGT instruction?
	{
		string args[3];
		_errorCode = ExtractArgs(workingCopy, lastarg, args, 2);
		if (_errorCode == NoError)
		{
			_branchingLabel = args[2];
			_hasBranchingLabel = true;
			_validLine = ValidateWord(args[0], Register) && ValidateWord(args[1], Register)
				&& ValidateWord(args[2], MemAddress);
		}
	}
	else if (opcode._Equal("B")) //Is this a B instruction?
	{
		string args[1];
		_errorCode = ExtractArgs(workingCopy, lastarg, args, 1);
		if (_errorCode == NoError)
		{
			_hasBranchingLabel = true;
			_branchingLabel = args[0];
			_validLine = ValidateWord(args[0], MemAddress);
		}
	}
	else if (opcode._Equal("NOOP") | opcode._Equal("END")) //Is this a NOOP or END instruction?
	{
		_validLine = !HasNext(workingCopy); //There shouldn't be another operand
	}
	else
	{
		_validLine = false;
		_errorCode = InvalidOpcode;
		_errorMessage = "** error -- \"" + opcode + "\" is an invalid label or operation code.\n" +
			"**          Labels must be immediately followed with ':', and operations are defined in the MAL manual.";
	}

	//if the _errorCode was altered above, we need to adjust the errorMessage
	switch (_errorCode)
	{
	case TooFewOps:
		_validLine = false;
		_errorMessage = "** error -- too few operands: for the specific opcode, there are fewer operands than required";
		break;
	case TooManyOps:
		_validLine = false;
		_errorMessage = "** error -- too many operands: for the specific opcode, there are more operands than required";
		break;
	case MissingComma:
		_validLine = false;
		_errorMessage = "** error -- missing argument delimiter: the argument \"" + lastarg + "\" doesn't have a comma immediately following it\n" +
			"**          All arguments except the final argument should have a ',' immediately following them.";
	case MisplacedComma:
		_validLine = false;
		_errorMessage = "** error -- errant argument delimiter: the argument \"" + lastarg + "\" should not have a comma following it.\n" +
			"**          All arguments except the final argument should have a ',' immediately following them.";
	default:
		break;
	}
}

ErrorCode MalLine::ExtractArgs(string &workingCopy, string &lastarg, string args[], int count) const
{
	for (int i = 0; i < count - 1; i++)
	{
		if (HasNext(workingCopy)) 
		{
			lastarg = PopNext(workingCopy);
			args[i] = lastarg;
			if (workingCopy[0] == ',') continue;
			else return MissingComma;
		}
		else
		{
			return TooFewOps;
		}
	}

	if (HasNext(workingCopy))//need to perform one final extraction. 
	{
		lastarg = PopNext(workingCopy);
		args[count - 1] = lastarg;
	}

	if (HasNext(workingCopy)) return TooManyOps;
	if (workingCopy.find_first_of(',') != -1) return MisplacedComma;
	return NoError;
}

bool MalLine::HasNext(const string &workingCopy) const
{
	int ind = workingCopy.find_first_not_of(", ", 0); 
	return ind >= 0;
}

//adjustments to the workingCopy string are made purposefully
const string MalLine::PopNext(string &workingCopy) const
{
	int beg = workingCopy.find_first_not_of(", ", 0);
	int term = workingCopy.find_first_of(", ", beg);
	if (term == -1)
	{
		string ret = workingCopy.substr(beg);
		workingCopy = "";
		return ret;
	}
	else
	{
		string ret = workingCopy.substr(beg, term - beg);
		workingCopy = workingCopy.substr(term);
		return ret;
	}
}

//passes target word (targ) to the appropriate error checking routine based on WordType (type).
bool MalLine::ValidateWord(string &targ, WordType type)
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
		_errorCode = ValidateImm(targ);
		switch (_errorCode)
		{
		default:
			break;
		case InvalidLength:
			_errorMessage = "** error -- ill-formed immediate value: \"" + targ + "\" is too long.\n" +
				"**         Immediate values cannot have more than 8 characters and are written in octal representation (0-7).";
			return false;
		case InvalidCharacters:
			_errorMessage = "** error -- ill-formed immediate value: \"" + targ + "\" has invalid characters.\n" +
				"**         Immediate values cannot have more than 8 characters and are written in octal representation (0-7).";
			return false;
		}
		break;
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
		break;
	}
}

//Register words can have commas at the end or not
ErrorCode MalLine::ValidateReg(const string &R)
{
	bool ret = false;
	int ind = octal.find_first_of(R[1], 0);
	ret = (R[0] == 'R') && ind >= 0;
	if (ret && (R.length() == 2)) return NoError;
	else return BadRegister;
}

ErrorCode MalLine::ValidateImm(const string &imm)
{
	bool ret = imm.length() > 0;
	for (int i = 0; i < imm.length(); i++)
	{
		int ind = octal.find(imm[i], 0);
		ret = ret && ind > -1;
	}
	if (ret) 
		if (imm.length() < 9) return NoError; 
		else return InvalidLength;
	else return InvalidCharacters;
}

//Identifiers always appear last, so they should have nothing appended to the end
ErrorCode MalLine::ValidateIdent(const string &ident)
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