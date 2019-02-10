#include "stdafx.h"
#include <string>
#include "MalLine.h"
#include <list>
#include <iostream>
#include <fstream>
#include <map>

class MalProgram
{
public:
	MalProgram(std::list<std::string> &stringList); //Only constructor, builds with all the lines of a given MAL Program
	friend void WriteReportA(MalProgram &program, std::ofstream &file);
	friend void WriteReportB(MalProgram &program, std::ofstream &file);
	friend void WriteReportC(MalProgram &program, std::ofstream &file);
private:
	//private functions
	void ProcessLines(std::list<std::string> &stringList);
	void ResolveLabelLocation(std::string str); //called when a resolved label is found
	void ResolveLabelBranch(std::string str); //called when a branching label is found
	bool CheckLabelLocationFound(std::string str); //returns false if the label location is unresolved, true if it is resolved.
	bool CheckLabelBranching(std::string str); //returns false if the label branching is unresolved, true if it is resolved.
	std::list<MalLine> _lines;
	std::map<std::string, short> _labelResolve; //labels are the keys. the 2^0 bit position indicates it's been reference in a branch operation. The 2^1 bit position indicates the label position is defined.
};