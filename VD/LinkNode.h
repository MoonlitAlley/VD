#pragma once
#include"CellNode.h"

class LinkNode :public CellNode
{
public:
	virtual string GetTargetNodePath() override;
	virtual void SetTargetNodePath(const string& TargetPath) override;
	virtual string GetNodeTypeStr() override;
};
