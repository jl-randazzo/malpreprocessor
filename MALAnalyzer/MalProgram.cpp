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
			ResolveLabelLocation(ml.GetLeadingLabel());
		if (ml.HasBranchingLabel())
			ResolveLabelBranch(ml.GetBranchingLabel());
		_lines.push_back(ml);
	}
}

void MalProgram::ResolveLabelLocation(string str)
{
	_labelResolve[str] |= 0x2; //set the binary flag 0010, indicating the leading label is found.
}

void MalProgram::ResolveLabelBranch(string str)
{
	_labelResolve[str] |= 0x1; //set the binary flag 0001, indicating the label has been branched to.
}

bool MalProgram::CheckLabelLocationFound(string str)
{
	return (_labelResolve[str] & 0x2);
}

bool MalProgram::CheckLabelBranching(string str)
{
	return (_labelResolve[str] & 0x1);
}