#include "stdafx.h"
#include <string>
#include "MalLine.h"
#include <list>
#include <map>

class MalProgram
{
public:
	MalProgram(std::list<std::string>); //Only constructor, builds with all the lines of a given MAL Program
	const std::list<MalLine> GetLines() const;
private:
	//private functions
	void ProcessLines();
	std::list<MalLine> lines;
	std::map<std::string, std::list<MalLine*>> _unresolvedLabels;
	std::list<std::string> _resolvedLabels;
};