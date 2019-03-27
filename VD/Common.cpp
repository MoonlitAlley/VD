#include "Common.h"
#include"MyString.h"
#include <algorithm>



bool Tools::isWildcard(const string & pattern)
{
	if (pattern.find('*') != string::npos)
	{
		return true;
	}
	else if (pattern.find('?') != string::npos)
	{
		return true;
	}
	return false;
}

bool Tools::IsWildcardMatch(const string & wildcard, const string & str)
{
	string pattern;
	for (size_t i = 0; i < wildcard.size(); ++i)
	{
		if (wildcard[i] == '?')
		{
			pattern += ".?";
		}
		else if (wildcard[i] == '*')
		{
			pattern += ".*";
		}
		else
		{
			pattern += wildcard[i];
		}
	}

	return regex_match(str, regex(pattern));
}

bool Tools::IsLegalPathName(const string & pathName)
{
	string temp = pathName;
	if (StrProcess::stripspace(temp).empty())
	{
		return false;
	}
	string IlegalChar = "\\/:*?\"<>|";
	if (any_of(pathName.begin(), pathName.end(), [&](char c) {return IlegalChar.find(c) != string::npos; }))
	{
		return false;
	}
	return true;
}

bool Tools::IsLegalFileName(const string & filename)
{
	if (!IsLegalPathName(filename))
	{
		return false;
	}
	if (filename == ".")
	{
		return false;
	}
	if (filename == "..")
	{
		return false;
	}

	return true;
}
