#include "stdafx.h"
#include <string>
#include <regex>

//label regular expressions
static const std::regex anyLabelRegex = std::regex("^.*:$"); //ends with colon
static const std::regex labelRegex = std::regex("^([a-zA-Z]{1,5}):$"); //valid label

//operand regular expressions
static const std::regex immRegex = std::regex("^[0-7]{1,8},{0,1}$");
static const std::regex identRegex = std::regex("^[a-zA-Z]{1,5},{0,1}$");

enum ErrorCode
{
	NoError, TooFewOps, TooManyOps, BadRegister, BadLabel, BadImmediate, BadIdent
};
enum WordType
{
	Register, Immediate, MemAddress
};

//Encapsulates data processing and storage for a given line written in the fictional language MAL. 
class MalLine {
public:
	//constructor and ostream operator
	MalLine(std::string line);
	friend std::ostream & operator <<(std::ostream &out, const MalLine &malLine);
	
	//dumb accessors
	const std::string GetLine();
	const std::string GetLineWithoutComment();
	bool IsLineEmpty();
	bool IsLineValid();
	int GetCommentIndex();
	ErrorCode GetErrorCode();
	const std::string GetErrorMessage();
private:
	//private variables
	std::string _line;
	std::string _lineNoComment;
	bool _validLine;
	bool _emptyLine;
	int _commentIndex;
	ErrorCode _errorCode;
	std::string _errorMessage;
	//line processor and related functions
	void ProcessLine();
	void ProcessLine(std::string &opcode, std::string &workingCopy);
	const std::string PopNext(std::string &line) const;
	ErrorCode ExtractArgs(std::string &workingCopy, std::string args[], int count) const;
	bool HasNext(const std::string &workingCopy) const;
	bool ValidateWord(std::string &targ, WordType type, bool finalOp);
	bool ValidateReg(const std::string &R);
};