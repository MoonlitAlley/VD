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
	//���ȣ���pathStr�е�����\�滻Ϊ/��Ȼ���ڰ���/������

	//ȡ��ַ�滻
	StrProcess::replace(pathStr, "\\", "/");


	//����·������  �� �����ԣ���·������ʵ·��������ԣ���ǰ·��
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
			//����û�����ΪC�̿�ʼ�ĵ�ַ
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
			//�������һ��Ŀ¼�Ļ����ͽ�·���е����һ�����Ѱ·ʱ�������¼�Ѱ��
			//�����������·���Ƿ������ӣ�����ܳ���
			pathItems.pop_back();
		}
		else
		{
			pathItems.push_back(items[i]);
		}
	}

	if (IsReal() && !pathItems.empty() && pathItems.front().front() == '@')
	{
		//�������ʵ·������·�����еĵ�һ���ڵ������������@������
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
	//�����ϼ�
	return this->Self().append("..");
}

//��ʵ��
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
