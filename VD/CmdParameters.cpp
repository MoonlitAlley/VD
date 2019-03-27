#include "CmdParameters.h"
#include <iterator>
#include <algorithm>
#include "MyString.h"

void CmdParameters::SetPathCount(int min, int max)
{
	min_pathNum = min;
	max_pathNum = max;
}

//���û�����ֽ�Ϊ�������������·��
bool CmdParameters::SplitUserInput(string & userInput)
{
	vector < pair<string, bool>> items = SplitToWordBySpace(userInput);
	//���ٴ���������
	for (size_t i = 1; i < items.size(); i++)
	{
		string &word = items[i].first;
		bool inquote = items[i].second;

		if (word.empty())
		{
			continue;
		}
		//�����������
		if (inquote)
		{
			m_pathItems.push_back(word);
		}
		else if (word[0] == '/' && word.size() >= 2 && word.size() <= 3)
		{
			//��ǰ���������ǲ���
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
	//�������а����ģ�ϵͳû�ж���Ĳ������������set��
	set_difference(m_parsedOptions.begin(), m_parsedOptions.end(), m_definedOptions.begin(), m_definedOptions.end(), inserter(opAdditional, opAdditional.begin()));

	//����û��������ϵͳδ����Ĳ���������
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
		}//�ڲ�������һ������֮ǰֱ��ִ��continue��
		if (inquote)
		{
			word.push_back(input[cur]);
			continue;
		}//���Ž���֮���ټ���ִ��
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
	//�����һ�����ret��
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
