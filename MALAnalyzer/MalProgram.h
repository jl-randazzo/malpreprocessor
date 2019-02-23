#include "stdafx.h"
#include <string>
#include "MalLine.h"
#include <list>
#include <iostream>
#include <fstream>
#include <map>

typedef short lflags;

#define LOCATED 2 // binary flag 0010, indicating the leading label is found.
#define BRANCHED_TO 1 // binary flag 0001, indicating the label has been branched to.

class MalProgram
{
public:
	MalProgram(std::list<std::string> &stringList); //Only constructor, builds with all the lines of a given MAL Program
	friend void WriteReportA(const MalProgram &program, std::ofstream &file);
	friend void WriteReportB(const MalProgram &program, std::ofstream &file);
	friend void WriteReportC(const MalProgram &program, std::ofstream &file);
	//Accessor
	int GetErrCount() const;
	const int* GetErrTypeCountIt() const;
	int GetLineCount() const;
private:
	//private functions
	void ProcessLines(std::list<std::string> &stringList);
	void ResolveLabelLocation(std::string str); //called when a resolved label is found
	void ResolveLabelBranch(std::string str); //called when a branching label is found
	bool CheckLabelLocationFound(std::string str) const; //returns false if the label location is unresolved, true if it is resolved.
	bool CheckLabelBranching(std::string str) const; //returns false if the label branching is unresolved, true if it is resolved.
	std::list<MalLine> _lines; //Houses all of the lines and their encapsulated information
	int _errCount = 0; //the number of lines that have errors in them
	int _lineCount = 0;
	int _errTypeCounts[InvalidOpcode + 1];
	bool _ended = false; //was the program ended?
	std::string _programErr; //indicates an error with the program at large
	std::map<std::string, lflags> _labelResolve; //labels are the keys. the 2^0 bit position indicates it's been reference in a branch operation. The 2^1 bit position indicates the label position is defined.
};