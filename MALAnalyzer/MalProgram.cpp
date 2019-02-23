#include "stdafx.h"
#include "MalProgram.h"

using namespace std;

MalProgram::MalProgram(list<string> &stringList)
{
	ProcessLines(stringList);
}

//Accessors
int MalProgram::GetErrCount() const
{
	return _errCount;
}

const int* MalProgram::GetErrTypeCountIt() const
{
	return _errTypeCounts;
}

int MalProgram::GetLineCount() const
{
	return _lineCount;
}

void MalProgram::ProcessLines(list<string> &stringList)
{
	for (int i = 0; i <= (int)InvalidOpcode; i++)
		_errTypeCounts[i] = 0;

	int i = 0;
	for (string x : stringList)
	{
		//Build the line and perform all error checks
		MalLine ml(x);

		//If we have any labels, resolve them in the label dictionary
		if (ml.HasLeadingLabel())
		{
			if (CheckLabelLocationFound(ml.GetLeadingLabel())) ml.RepeatBranchingLabel();
			else ResolveLabelLocation(ml.GetLeadingLabel());
		}
		if (ml.HasBranchingLabel())
			ResolveLabelBranch(ml.GetBranchingLabel());

		//if we have an error code, iterate both the total count and the count for that specific error
		int err = ml.GetErrorCode();
		if (err)
		{
			_errCount++;
			_errTypeCounts[err]++;
		}
		_lines.push_back(ml);

		if (!ml.IsLineEmpty())
			_lineCount++;

		//set the program to ended if this line ends the program
		if (!_ended)
		{
			if (ml.IsEnd())
				_ended = true;
		}
		else //set error if the program continues after END
		{
			if (!ml.IsLineEmpty())
			{
				_errCount++; _errTypeCounts[BadEnd] = 1; _lineCount++;
				_programErr = "** error -- instructions continue after the END directive. All instructions must precede END.";
			}
		}
	}

	if (!_ended)
	{
		_errCount++; _errTypeCounts[BadEnd] = 1;
		_programErr = "** error -- the program was never terminated with the END directive.";
	}
}

void MalProgram::ResolveLabelLocation(string str)
{
	_labelResolve[str] |= LOCATED;
}

void MalProgram::ResolveLabelBranch(string str)
{
	_labelResolve[str] |= BRANCHED_TO;
}

bool MalProgram::CheckLabelLocationFound(string str) const
{
	return (_labelResolve.count(str) > 0 && _labelResolve.at(str) & LOCATED);
}

bool MalProgram::CheckLabelBranching(string str) const
{
	return (_labelResolve.count(str) > 0 && _labelResolve.at(str) & BRANCHED_TO);
}