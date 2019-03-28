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

	bool AddSubNode(CellNode* node);
	bool RemoveSubNode(CellNode* node, bool release);

	void RemoveFromPreSubList(bool release);
	list<CellNode*> FilterSubNode(string whildcard);

	CellNode* GetNode(const Path &path);
	CellNode* GetParent();

	vector<char>& Content();

	void SetNodeType(FileNodeType type);
	FileNodeType GetNodeType();
	string GetNodeTypeStr();

	string GetCellName();
	void SetCellName(string name);

	void UpdateLastModifyTime();
	time_t GetlastModifyTime();

	Path GetNodePath();
	bool IsRootNode();
	string Target();

	//һ��˫Ŀ���������Ӧ�������⣬����Ҫ����Ϊ��Ԫ����
	friend istream& operator>>(istream& input, CellNode& self);

	friend ostream& operator<<(ostream& output, CellNode& self);


private:
	list<CellNode*> subCellNodeList;


	vector<char> content;


	//�Լ��ĸ�Ŀ¼
	CellNode* preLink;
	string cellName;
	FileNodeType nodeType;

	time_t lastModifyTime;
};
