// MALAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MalProgram.h"
#include <chrono>
#include <time.h>


using namespace std;

void WriteHeading(ofstream &file, string fileName, string reportName)
{
	file << "---MAL ANALYZER REPORT---" << endl << endl;
	file << "MAL program: " << fileName << endl;
	file << "Report name: " << reportName << endl;
	auto time = chrono::system_clock::to_time_t(chrono::system_clock::now());
	char ts[30]; ctime_s(ts, 29, &time);
	file << "System time: " << ts << endl;
	file << "CS 3210 -- Written by: Josiah \"Luke\" Randazzo" << endl << "_________________________________________" << endl;
}

//These are the three friend functions written for MalProgram
void WriteReportA(const MalProgram &program, ofstream &file)
{
	file << "REPORT A -- FULL, UNEDITED PROGRAM" << endl;
	int lineNum = 1;
	for (MalLine x : program._lines)
	{
		file << lineNum++ << ". " << x.GetLine() << endl;
	}
	file << endl;
}

void WriteReportB(const MalProgram &program, ofstream &file)
{
	file << "REPORT B -- PROGRAM WITH COMMENTS AND BLANK LINES REMOVED" << endl;
	int lineNum = 1;
	for (MalLine x : program._lines)
	{
		if (!x.IsLineEmpty())
		{
			file << lineNum << ". " << x.GetLineWithoutComment() << endl;
		}
		lineNum++;
	}
	file << endl;
}

void WriteReportC(const MalProgram &program, ofstream &file)
{
	file << "REPORT C -- PROGRAM WITH ERROR CODES" << endl;
	int lineNum = 1;
	for (MalLine x : program._lines)
	{
		if (!x.IsLineEmpty())
		{
			file << lineNum << ". " << x.GetLineWithoutComment() << endl;
			if (x.GetErrorCode() != NoError)
			{
				file << x.GetErrorMessage() << endl;
			}
			if (x.HasBranchingLabel() && !program.CheckLabelLocationFound(x.GetBranchingLabel()))
			{
				file << "** warning -- branching label \"" << x.GetBranchingLabel() << "\" is not defined in the program scope." << endl;
			}
			if (x.HasLeadingLabel() && !program.CheckLabelBranching(x.GetLeadingLabel()))
			{
				file << "** warning -- leading label \"" << x.GetLeadingLabel() << "\" is never branched to within the program scope." << endl;
			}
		}
		lineNum++;
	}
}

void WriteReport(MalProgram &program, string fileName, string reportName)
{
	try 
	{
		ofstream file(reportName);
		WriteHeading(file, fileName, reportName);
		WriteReportA(program, file);
		WriteReportB(program, file);
		WriteReportC(program, file);
		file.close();
	}
	catch (ostream::failure e)
	{
		perror("error in file-writing");
		exit(1);
	}
}

list<string> ReadMalFile(string fileName)
{
	try
	{
		std::filebuf fb;
		if (fb.open(fileName, ios::in))
		{
			istream is(&fb);
			list<string> retList;
			string str;
			while (is.peek() != EOF) //continue until we've reached the end of the file
			{
				getline(is, str);
				retList.push_back(str);
			}
			fb.close();
			return retList;
		}
	}
	catch (istream::failure e)
	{
		perror("error reading file, perhaps invalid file name? Input file should be just the name of the file (e.g. Text for Text.mal) and should be positioned in the same folder as this executable.");
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	//Check for command line argument
	if (argc < 2)
	{
		perror("MISSING COMMAND LINE ARG -- Please post the name of a .mal file to analyze. (e.g. Test.mal would be entered as \"Test\"");
		exit(1);
	}

	//process file, convert into a list of strings
	string fileName = argv[1];
	string reportName = "Report_" + fileName + ".txt";
	fileName += ".mal";
	list<string> mylist = ReadMalFile(fileName);

	//Build program from strings
	MalProgram prog(mylist);
	WriteReport(prog, fileName, reportName);

	cout << "Report successfully written and saved to \"Report_" << fileName << ".txt\" in the current directory" << endl
		<< "Press [RETURN]" << endl;
	getline(std::cin, fileName);
    return 0;
}