#include "stdafx.h"
#include <string>
#include <regex>

//instruction regular expressions
static const std::regex labelRegex = std::regex("^([a-zA-Z]{1,5}):$");
static const std::regex addRegex = std::regex("^ADD$");

//operand regular expressions
static const std::regex regRegex = std::regex("^R[0-7],{0,1}$");
static const std::regex immRegex = std::regex("^[0-7]{0,7},{0,1}$");

enum WordType
{
	Register, Immediate, MemAddress, LabelPointer
};

class MalLine {
public:
	//constructor and ostream operator
	MalLine(std::string line);
	friend std::ostream & operator <<(std::ostream &out, const MalLine &malLine);
	
	//dumb accessors
	std::string GetLine();
	bool IsLineEmpty();
	bool IsLineValid();
	int GetCommentIndex();
	std::string GetErrorMessage();
private:
	//private variables
	std::string _line;
	std::string _lineNoComment;
	bool _validLine;
	bool _emptyLine;
	int _commentIndex;
	std::string _errorMessage;
	//line processor
	void ProcessLine();
	const std::string PopNext(std::string &line) const;
	bool ValidateWord(std::string &targ, WordType type, bool finalOp);
};