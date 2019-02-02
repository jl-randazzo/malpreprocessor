#include "stdafx.h"
#include <string>
#include <regex>

//label regular expressions
static const std::regex anyLabelRegex = std::regex("^.*:$"); //ends with colon
static const std::regex labelRegex = std::regex("^([a-zA-Z]{1,5}):$"); //valid label

//opcode regular expressions
static const std::regex addsubRegex = std::regex("^(ADD)|(SUB)$");
static const std::regex incdecRegex = std::regex("^(INC)|(DEC)$");
static const std::regex lsRegex = std::regex("^(LOAD)|(STORE)$");
static const std::regex loadiRegex = std::regex("^LOADI$");
static const std::regex bcompRegex = std::regex("^B((EQ)|(LT)|(GT))$");
static const std::regex bRegex = std::regex("^B$");
static const std::regex noendRegex = std::regex("^(NOOP)|(END)$");

//operand regular expressions
static const std::regex regRegex = std::regex("^R[0-7],{0,1}$");
static const std::regex immRegex = std::regex("^[0-7]{1,8},{0,1}$");
static const std::regex identRegex = std::regex("^[a-zA-Z]{1,5},{0,1}$");

enum ErrorCode
{
	NoError, TooFewOps, TooManyOps
};
enum WordType
{
	Register, Immediate, MemAddress
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
	//line processor and related functions
	void ProcessLine();
	void ProcessLine(std::string &opcode, std::string &workingCopy);
	const std::string PopNext(std::string &line) const;
	ErrorCode ExtractArgs(std::string &workingCopy, std::string args[], int count) const;
	bool HasNext(const std::string &workingCopy) const;
	bool ValidateWord(std::string &targ, WordType type, bool finalOp);
};