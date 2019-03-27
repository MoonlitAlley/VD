#pragma once

#include <string>
#include <vector>
#include<cstdarg>
#include <cassert>

namespace StrProcess
{
	using namespace std;

	vector<string> split(string &str, char separa);

	vector<string> SplitNoEmpty(string &str, char separa);

	void replace(string &str, string from, string to);


	string join(vector<string> & vec, string addStr);

	string stripspace(string & str);


	template<int buffsize>
	std::string to_str(const char* format, ...)
	{
		char temp[buffsize] = { 0 };
		va_list list;
		long ret;

		va_start(list, format);
		ret = vsnprintf(temp, buffsize, format, list);
		//assert(ret < buffsize);
		va_end(list);

		return temp;
	}

	template<int buffersize>
	const char* to_cstr(const char* format, ...)
	{
		static char static_buff[buffersize] = { 0 };
		va_list list;
		long ret;

		va_start(list, format);

		ret = vsnprintf(static_buff, buffersize, format, list);

		//assert(ret<buffersize>);

		va_end(list);
		return static_buff;
	}
#define cstr to_cstr<512>
#define sstr to_str<512>

}