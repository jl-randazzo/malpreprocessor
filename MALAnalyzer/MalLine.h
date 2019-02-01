#include "stdafx.h"
#include <string>
#include <regex>

static const std::regex labelRegex = std::regex("^([a-zA-Z]{1,5}):$");
static const std::regex addRegex = std::regex("^ADD$");


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
	int GetCommentIndex();
	std::string GetErrorMessage();
private:
	//private variables
	std::string _line;
	std::string _lineNoComment;
	bool _emptyLine;
	int _commentIndex;
	std::string _errorMessage;
	//line processor
	void ProcessLine();
	std::string PopNext(std::string &line);
	void ValidateNext(std::string &next, WordType type, bool finalOp);
};