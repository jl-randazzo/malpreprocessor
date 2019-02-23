#include "stdafx.h"
#include <string>
#include <regex>


//operand regular expressions

//strings for character checking
static const std::string alphabet = std::string("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
static const std::string octal = std::string("01234567");

enum ErrorCode
{
	NoError = 0, TooFewOps, TooManyOps, BadRegister, BadLabel, BadImmediate, BadIdent, InvalidLength, InvalidCharacters,
	MisplacedComma, MissingComma, RepeatLabel, BadEnd, InvalidOpcode
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
	//public, static functions
	static ErrorCode ValidateReg(const std::string &R);
	static ErrorCode ValidateIdent(const std::string &ident);
	static ErrorCode ValidateImm(const std::string &imm);
	//dumb accessors
	const std::string GetLine() const;
	const std::string GetLineWithoutComment() const;
	const std::string GetLeadingLabel() const;
	const std::string GetBranchingLabel() const;
	bool HasLeadingLabel() const;
	bool HasBranchingLabel() const;
	bool IsLineEmpty() const;
	bool IsLineValid() const;
	bool IsEnd() const;
	int GetCommentIndex() const;
	ErrorCode GetErrorCode() const;
	const std::string GetErrorMessage() const;
	//Error-setters
	void RepeatBranchingLabel(); //mutator that notes that the discovered label has been encountered previously 
private:
	//private variables
	std::string _line;
	std::string _lineNoComment;
	std::string _leadingLabel = "";
	std::string _branchingLabel = "";
	bool _hasLeadingLabel = false;
	bool _hasBranchingLabel = false;
	bool _validLine;
	bool _emptyLine = false;
	bool _end = false;
	int _commentIndex;
	ErrorCode _errorCode = NoError;
	std::string _errorMessage;
	//line processor and related functions
	void ProcessLine();
	void ProcessOperation(std::string &opcode, std::string &workingCopy);
	const std::string PopNext(std::string &line) const;
	ErrorCode ExtractArgs(std::string &workingCopy, std::string &lastarg, std::string args[], int count) const;
	bool HasNext(const std::string &workingCopy) const;
	bool ValidateWord(std::string &targ, WordType type);
};