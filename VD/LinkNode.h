#pragma once
#include"CellNode.h"

class LinkNode :public CellNode
{
public:
	virtual vector<char> GetTargetNodePath() override;
	virtual void SetTargetNodePath(string TargetPath) override;
	virtual string GetNodeTypeStr() override;
};
