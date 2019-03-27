#pragma once


#include<string>
#include <vector>
using namespace std;

class Path
{
public:
	Path();
	Path(const string &Unprocessed);

public:
	bool IsAbsolute() const;
	bool IsReal() const;
	Path& append(string pathStr);
	Path Self() const;
	Path Parent() const;
	string str();
	vector<string> split() const;

	//返回路径中的最后一个节点
	string FileName() const;
	string StartNode() const;
private:
	char startNode = 0;
	//路径信息，使用vector存储
	vector<string> pathItems;
};