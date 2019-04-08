#pragma once

#include<list>
#include <algorithm>
#include "Path.h"
#include "Common.h"
#include <ctime>
using namespace std;

class CellNode
{
public:
	CellNode();
	~CellNode();

	CellNode* GetNode(const Path &path);
	void RemoveFromPreSubList(bool release);

	CellNode* GetParent();
	void SetParent(CellNode* pre);

	void SetNodeType(FileNodeType type);
	FileNodeType GetNodeType();
	
	string GetCellName();
	void SetCellName(string name);

	void UpdateLastModifyTime();
	time_t GetlastModifyTime();
	void SetLastModifyTime(time_t value);

	Path GetNodePath();
	bool IsRootNode();
	string Target();

	//一个双目运算符重载应该在类外，所以要声明为友元函数
	friend istream& operator>>(istream& input, CellNode& self);
	friend ostream& operator<<(ostream& output, CellNode& self);

	virtual string GetNodeTypeStr();

	//FoldNode
	virtual bool AddSubNode(CellNode* node);
	virtual bool RemoveSubNode(CellNode* node, bool release);

	virtual list<CellNode*> FilterSubNode(string whildcard);

	//FileNode
	virtual vector<char>& Content();

	//LinkNode
	virtual vector<char> GetTargetNodePath();
	virtual void SetTargetNodePath(string TargetPath);

protected:
	list<CellNode*> subCellNodeList;

	vector<char> content;
	//自己的父目录
	CellNode* preLink;
	string cellName;
	FileNodeType nodeType;
	time_t lastModifyTime;
};
