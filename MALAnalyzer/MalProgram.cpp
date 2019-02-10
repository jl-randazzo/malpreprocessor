#include "stdafx.h"
#include "MalProgram.h"

using namespace std;

MalProgram::MalProgram(list<string> &stringList)
{
	ProcessLines(stringList);
}

void MalProgram::ProcessLines(list<string> &stringList)
{
	int i = 0;
	for (string x : stringList)
	{
		MalLine ml(x);
		if (ml.HasLeadingLabel())
			ResolveLabel(ml.GetLeadingLabel());
		_lines.push_back(ml);
	}
}

void MalProgram::ResolveLabel(string str)
{
	_labelResolve[str] = true;
}

bool MalProgram::CheckLabel(string str) const
{
	if (_labelResolve.count(str) == 0)
		return false;
	else return true;
}