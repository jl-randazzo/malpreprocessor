#include "stdafx.h"
#include <string>

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
	bool _emptyLine;
	int _commentIndex;
	std::string _errorMessage;
	//line processor
	void ProcessLine();
};