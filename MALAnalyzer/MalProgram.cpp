#include "stdafx.h"
#include "MalProgram.h"
#include <string>
#include <list>

using namespace std;

MalProgram::MalProgram(list<string> &stringList)
{
	ProcessLines(stringList);
}

void MalProgram::ProcessLines(list<string> &stringList)
{
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

bool MalProgram::CheckLabel(string str)
{
	bool ret = (bool)_labelResolve.count(str);
	if (_labelResolve.count(str) == 0)
	{
		_labelResolve[str] = false;
		return false;
	}
	else
	{
		return _labelResolve[str];
	}
}