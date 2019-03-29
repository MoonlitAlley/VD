#include "Path.h"
#include "MyString.h"
#include "Common.h"
Path::Path()
{
}
Path::Path(const string & Unprocessed)
{
	startNode = 0;
	append(Unprocessed);
}

Path & Path::append(string pathStr)
{
	//首先，将pathStr中的所有\替换为/，然后在按照/来分离

	//取地址替换
	StrProcess::replace(pathStr, "\\", "/");


	//设置路径类型  ， （绝对）根路径，真实路径，（相对）当前路径
	if (startNode == 0)
	{
		startNode = pathStr.front();
		startNode = (startNode == '/' || startNode == '@') ? startNode : '.';
	}
	vector<string> items = StrProcess::SplitNoEmpty(pathStr, '/');

	for (size_t i = 0; i < items.size(); i++)
	{
		if (i == 0)
		{
			//如果用户输入为C盘开始的地址
			if (items[0] == "c:"||items[0] =="C:")
			{
				startNode = '/';
				continue;
			}
		}
		if (items[i] == ".")
		{

		}
		else if (items[i] == ".." && !pathItems.empty() && pathItems.back() != "..")
		{
			//如果是上一级目录的话，就将路径中的最后一项弹出，寻路时不在向下级寻找
			//如果被弹出的路径是符号链接，则可能出错
			pathItems.pop_back();
		}
		else
		{
			pathItems.push_back(items[i]);
		}
	}

	if (IsReal() && !pathItems.empty() && pathItems.front().front() == '@')
	{
		//如果是真实路径，则将路径名中的第一个节点的名字跳过“@”符号
		pathItems.front().assign(pathItems.front().begin() + 1, pathItems.front().end());
	}

	return *this;
}
Path Path::Self() const
{
	return Path(*this);
}
bool Path::IsAbsolute() const
{
	return startNode == '/';
}

bool Path::IsReal() const
{
	return startNode == '@';
}



Path Path::Parent() const
{
	//返回上级
	return this->Self().append("..");
}

//待实现
string Path::str()
{

	if (IsReal())
	{
		return StrProcess::join(pathItems, "\\");
	}
	else if (IsAbsolute())
	{
		return "/" + StrProcess::join(pathItems, "/");
	}
	else
	{
		return "./" + StrProcess::join(pathItems, "/");
	}
}

vector<string> Path::split() const
{
	return pathItems.empty() ? vector<string>{"."} : pathItems;
}

string Path::Title()
{
	string filename = FileName();
	size_t lastdot = filename.find_last_of('.');
	if (lastdot == string::npos)
	{
		return filename;
	}
	else
	{
		return filename.substr(0, lastdot);
	}
}

string Path::FileName() const
{
	if (pathItems.empty())
	{
		return "";
	}
	if (!Tools::IsLegalFileName(pathItems.back()))
	{
		return "";
	}
	return pathItems.back();
}

string Path::StartNode() const
{
	if (IsReal())
	{
		return "@";
	}
	else if (IsAbsolute())
	{
		return "/";
	}
	else
	{
		return ".";
	}
}
