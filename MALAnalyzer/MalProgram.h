#include "stdafx.h"
#include <string>
#include "MalLine.h"
#include <list>
#include <map>

class MalProgram
{
public:
	MalProgram(std::list<std::string> &stringList); //Only constructor, builds with all the lines of a given MAL Program
	const std::list<MalLine> GetLines() const;
private:
	//private functions
	void ProcessLines(std::list<std::string> &stringList);
	void ResolveLabel(std::string str); //called when a resolved label is found
	bool CheckLabel(std::string str); //returns false if the label is unresolved, true if it is resolved. If it's not in the map, it adds it as 'false'
	std::list<MalLine> _lines;
	std::map<std::string, bool> _labelResolve; //encountered labels are the key, the bool being true means the actual label is found, false indicates it hasn't been
};