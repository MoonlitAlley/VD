#include "CmdParameters.h"
#include <iterator>
#include <algorithm>
#include "MyString.h"

void CmdParameters::SetPathCount(int min, int max)
{
	min_pathNum = min;
	max_pathNum = max;
}

//将用户输入分解为命令参数和命令路径
bool CmdParameters::SplitUserInput(string & userInput)
{
	vector < pair<string, bool>> items = SplitToWordBySpace(userInput);
	//不再处理命令名
	for (size_t i = 1; i < items.size(); i++)
	{
		string &word = items[i].first;
		bool inquote = items[i].second;

		if (word.empty())
		{
			continue;
		}
		//如果带有引号
		if (inquote)
		{
			m_pathItems.push_back(word);
		}
		else if (word[0] == '/' && word.size() >= 2 && word.size() <= 3)
		{
			//当前解析到的是参数
			string tmp = word;
			transform(word.begin(), word.end(), tmp.begin(), ::tolower);
			m_parsedOptions.insert(tmp);
		}
		else
		{
			m_pathItems.push_back(word);
		}
	}

	set<string> opAdditional;
	//将命令中包含的，系统没有定义的参数，放入参数set中
	set_difference(m_parsedOptions.begin(), m_parsedOptions.end(), m_definedOptions.begin(), m_definedOptions.end(), inserter(opAdditional, opAdditional.begin()));

	//如果用户输入的有系统未定义的参数，返回
	if (!opAdditional.empty())
	{
		return false;
	}
	if ((int)m_pathItems.size() < min_pathNum)
	{
		return false;
	}
	if (max_pathNum >= 0 && (int)m_pathItems.size() > max_pathNum)
	{
		return false;
	}
	return true;
}

vector<pair<string, bool>> CmdParameters::SplitToWordBySpace(string & input)
{
	vector<pair<string, bool>> ret;
	string word;
	bool inquote = false;

	for (size_t cur = 0; cur < input.size(); ++cur)
	{
		if (input[cur] == '\"')
		{
			if (!inquote)
			{
				inquote = true;
			}
			else
			{
				inquote = false;
				continue;
				/*if (!StrProcess::stripspace(word).empty())
				{
					ret.push_back(make_pair(word, true));
				}
				word.clear();*/
			}
			continue;
		}//在不遇到下一个引号之前直接执行continue，
		if (inquote)
		{
			word.push_back(input[cur]);
			continue;
		}//引号结束之后再继续执行
		if (isspace(input[cur]))
		{
			if (!StrProcess::stripspace(word).empty())
			{
				ret.push_back(make_pair(word, false));
			}
			word.clear();
		}
		else
		{
			word.push_back(input[cur]);
		}
	}
	//将最后一项放入ret中
	if (!inquote)
	{
		if (!StrProcess::stripspace(word).empty())
		{
			ret.push_back(make_pair(word, false));
		}
		word.clear();
	}

	return ret;
}
