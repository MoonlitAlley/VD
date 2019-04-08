#pragma once

#include <string>
#include <vector>
#include <set>
using namespace std;
class CmdParameters
{
public:
	void SetPathCount(int min, int max);

	bool SplitUserInput(const string & userInput);

private:
	vector < pair<string, bool>> SplitToWordBySpace(const string &input);

public:
	vector<string> m_pathItems;

	set<string> m_parsedOptions;
	set<string> m_definedOptions;
private:
	int min_pathNum;
	int max_pathNum;
};