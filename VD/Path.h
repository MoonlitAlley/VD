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

	string StartNode() const;
private:
	char startNode = 0;
	//·����Ϣ��ʹ��vector�洢
	vector<string> pathItems;
};