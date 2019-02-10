// MALAnalyzer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MalProgram.h"


using namespace std;

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

}

void WriteReport(MalProgram &program, string fileName)
{
	try 
	{
		ofstream file(fileName + ".txt");
		WriteReportA(program, file);
		WriteReportB(program, file);
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
		if (fb.open(fileName + ".mal", ios::in))
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
	string str = "LOADI R7, 67321547";
	MalLine ml(str);
	cout << ml << endl;
	list<string> mylist = ReadMalFile("Text");
	for (string x : mylist)
	{
		cout << x << endl;
	}
	MalProgram prog(mylist);
	WriteReport(prog, "Test");
	getline(std::cin, str);
    return 0;
}