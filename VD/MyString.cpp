#include "MyString.h"
#include <algorithm>
using namespace std;



vector<string> StrProcess::split(const string & str, char separa)
{
	vector<string> ret;

	//初始化第一个元素的位置
	string::size_type separa_pre = 0;
	string::size_type separa_cur = str.find(separa);

	//循环搜索至最后一个元素
	while (separa_cur != string::npos)
	{
		ret.push_back(str.substr(separa_pre, separa_cur - separa_pre));

		separa_pre = separa_cur + 1;
		separa_cur = str.find(separa, separa_pre);
	}

	//将最后一个元素放入vector中
	if (separa_pre < str.size())
	{
		ret.push_back(str.substr(separa_pre));
	}

	return ret;
}

vector<string> StrProcess::SplitNoEmpty(const string & str, char separa)
{
	auto ret = split(str, separa);

	auto target = remove_if(ret.begin(), ret.end(), [&](string item) {return item.empty(); });

	ret.erase(target, ret.end());
	return ret;
}

void StrProcess::replace(string & str, string from, string to)
{
	do
	{
		auto i = str.find(from);
		if (i == string::npos)
			break;
		str.replace(i, from.size(), to);
	} while (true);
}

string StrProcess::join(vector<string>& vec, string addStr)
{
	if (vec.empty())
	{
		return "";
	}
	else if (vec.size() == 1)
	{
		return vec.front();
	}
	string ret;
	for (size_t i = 0; i < vec.size()-1; i++)
	{
		ret += vec[i];
		ret += addStr;
	}
	ret += vec.back();
	return ret;
}

string StrProcess::stripspace(string & str)
{
	locale loc;
	string ret = str;
	while (ret.size() > 0 && isspace(ret.front()))
	{
		ret.erase(ret.begin());
	}
	while (ret.size() > 0 && isspace(ret.back()))
	{
		ret.pop_back();
	}
	return ret;
}
