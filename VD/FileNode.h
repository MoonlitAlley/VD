#pragma once
#include "CellNode.h"

class FileNode :public CellNode
{
public:
	FileNode();
	virtual vector<char>& Content() override;
	virtual string GetNodeTypeStr();
};